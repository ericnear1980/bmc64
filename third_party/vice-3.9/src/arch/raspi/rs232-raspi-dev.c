/*
 * rs232-raspi-dev.c - RS232 emulation for bare-metal Raspberry Pi (Circle).
 *
 * Implements a virtual modem over Circle's TCP stack.  Each "RS232 device"
 * is a virtual modem that responds to Hayes AT commands:
 *
 *   ATZ / AT      → OK
 *   ATDT host:port → opens Circle TCP connection → CONNECT
 *   ATH / +++ATH  → drops connection → NO CARRIER
 *
 * The device string (RsDevice1 resource) is ignored for dialling; the
 * destination is taken from the ATDT command itself.  The resource is used
 * only to set a default dial address if needed.
 *
 * Written for BMC64 (https://github.com/ericnear1980/bmc64)
 */

#include "vice.h"

#ifdef HAVE_RS232DEV

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "log.h"
#include "rs232.h"
#include "rs232drv.h"

/* C interface to Circle's TCP stack */
#include "circle.h"

/* -------------------------------------------------------------------------- */

#define RS232_MAX_FDS   4
#define AT_BUF_SIZE     128
#define TCP_BUF_SIZE    1024

typedef enum {
    STATE_COMMAND,   /* waiting for AT commands */
    STATE_CONNECTED  /* TCP connection active, pass-through mode */
} modem_state_t;

typedef struct {
    int              in_use;
    modem_state_t    state;
    circle_socket_t  sock;          /* Circle TCP socket (0 = none) */
    char             at_buf[AT_BUF_SIZE];
    int              at_pos;
    /* Bytes received from TCP waiting to be read by the C64 */
    uint8_t          rx_buf[TCP_BUF_SIZE];
    int              rx_head;
    int              rx_tail;
    /* Response bytes queued to send to the C64 (modem responses) */
    uint8_t          resp_buf[256];
    int              resp_head;
    int              resp_tail;
} modem_fd_t;

static modem_fd_t fds[RS232_MAX_FDS];
static log_t rs232dev_log = LOG_DEFAULT;

/* -------------------------------------------------------------------------- */

static void resp_queue(modem_fd_t *m, const char *s) {
    while (*s) {
        int next = (m->resp_tail + 1) % 256;
        if (next != m->resp_head) {
            m->resp_buf[m->resp_tail] = (uint8_t)*s++;
            m->resp_tail = next;
        } else {
            break;
        }
    }
}

static void modem_response(modem_fd_t *m, const char *msg) {
    resp_queue(m, "\r\n");
    resp_queue(m, msg);
    resp_queue(m, "\r\n");
}

/* Parse "ATDT host:port" or "ATDT host port" */
static int parse_atdt(const char *cmd, char *host, unsigned short *port) {
    const char *p = cmd;
    /* skip "ATDT" prefix (case-insensitive already uppercased) */
    if (strncmp(p, "ATDT", 4) != 0 && strncmp(p, "ATDP", 4) != 0)
        return 0;
    p += 4;
    while (*p == ' ') p++;
    /* copy host */
    int hi = 0;
    while (*p && *p != ':' && *p != ' ' && hi < 63)
        host[hi++] = *p++;
    host[hi] = '\0';
    if (*p == ':' || *p == ' ') p++;
    *port = (unsigned short)atoi(p);
    return (hi > 0 && *port > 0);
}

static void handle_at_command(modem_fd_t *m) {
    /* uppercase for comparison */
    char cmd[AT_BUF_SIZE];
    for (int i = 0; i < m->at_pos; i++)
        cmd[i] = (char)toupper((unsigned char)m->at_buf[i]);
    cmd[m->at_pos] = '\0';

    /* Strip leading whitespace */
    char *p = cmd;
    while (*p == ' ') p++;

    if (strcmp(p, "AT") == 0 || strcmp(p, "ATZ") == 0 ||
        strcmp(p, "ATE0") == 0 || strcmp(p, "ATE1") == 0 ||
        strcmp(p, "ATM0") == 0 || strcmp(p, "ATV1") == 0) {
        modem_response(m, "OK");
        return;
    }

    if (strcmp(p, "ATH") == 0 || strcmp(p, "ATH0") == 0) {
        if (m->sock) {
            circle_net_close(m->sock);
            m->sock = 0;
            m->state = STATE_COMMAND;
        }
        modem_response(m, "OK");
        return;
    }

    if (strncmp(p, "ATDT", 4) == 0 || strncmp(p, "ATDP", 4) == 0) {
        char host[64];
        unsigned short port = 23;
        if (!parse_atdt(p, host, &port)) {
            modem_response(m, "ERROR");
            return;
        }
        if (!circle_net_is_up()) {
            modem_response(m, "NO CARRIER");
            return;
        }
        circle_socket_t sock = circle_net_tcp_new();
        if (!sock) {
            modem_response(m, "NO CARRIER");
            return;
        }
        if (!circle_net_tcp_connect(sock, host, port)) {
            circle_net_close(sock);
            modem_response(m, "NO CARRIER");
            return;
        }
        m->sock  = sock;
        m->state = STATE_CONNECTED;
        modem_response(m, "CONNECT 38400");
        log_message(rs232dev_log, "Connected to %s:%u", host, port);
        return;
    }

    modem_response(m, "ERROR");
}

/* -------------------------------------------------------------------------- */
/* Public rs232dev_* interface                                                  */
/* -------------------------------------------------------------------------- */

int rs232dev_resources_init(void)   { return 0; }
void rs232dev_resources_shutdown(void) {}
int rs232dev_cmdline_options_init(void) { return 0; }

void rs232dev_init(void) {
    rs232dev_log = log_open("RS232");
    memset(fds, 0, sizeof(fds));
}

void rs232dev_reset(void) {
    for (int i = 0; i < RS232_MAX_FDS; i++) {
        if (fds[i].in_use && fds[i].sock) {
            circle_net_close(fds[i].sock);
        }
    }
    memset(fds, 0, sizeof(fds));
}

int rs232dev_open(int device) {
    for (int i = 0; i < RS232_MAX_FDS; i++) {
        if (!fds[i].in_use) {
            memset(&fds[i], 0, sizeof(fds[i]));
            fds[i].in_use = 1;
            fds[i].state  = STATE_COMMAND;
            /* Greet with modem-ready string */
            resp_queue(&fds[i], "BMC64 Virtual Modem\r\nOK\r\n");
            log_message(rs232dev_log, "rs232dev_open: slot %d for device %d", i, device);
            return i;
        }
    }
    log_message(rs232dev_log, "rs232dev_open: no free slots");
    return -1;
}

void rs232dev_close(int fd) {
    if (fd < 0 || fd >= RS232_MAX_FDS || !fds[fd].in_use) return;
    if (fds[fd].sock) {
        circle_net_close(fds[fd].sock);
        fds[fd].sock = 0;
    }
    fds[fd].in_use = 0;
}

/* Called by VICE to send a byte from the C64 to the "modem" */
int rs232dev_putc(int fd, uint8_t b) {
    if (fd < 0 || fd >= RS232_MAX_FDS || !fds[fd].in_use) return -1;
    modem_fd_t *m = &fds[fd];

    if (m->state == STATE_CONNECTED) {
        /* Pass-through to TCP */
        int r = circle_net_send(m->sock, &b, 1);
        return r >= 0 ? 0 : -1;
    }

    /* Command mode: buffer until CR */
    if (b == '\r' || b == '\n') {
        m->at_buf[m->at_pos] = '\0';
        if (m->at_pos > 0)
            handle_at_command(m);
        m->at_pos = 0;
    } else if (b == 8 || b == 127) { /* backspace */
        if (m->at_pos > 0) m->at_pos--;
    } else {
        if (m->at_pos < AT_BUF_SIZE - 1)
            m->at_buf[m->at_pos++] = (char)b;
    }
    return 0;
}

/* Called by VICE to get a byte to send to the C64 */
int rs232dev_getc(int fd, uint8_t *b) {
    if (fd < 0 || fd >= RS232_MAX_FDS || !fds[fd].in_use) return -1;
    modem_fd_t *m = &fds[fd];

    /* Drain any queued modem responses first */
    if (m->resp_head != m->resp_tail) {
        *b = m->resp_buf[m->resp_head];
        m->resp_head = (m->resp_head + 1) % 256;
        return 1;
    }

    if (m->state != STATE_CONNECTED || !m->sock) return 0;

    /* Try TCP receive */
    int n = circle_net_recv(m->sock, b, 1);
    return n > 0 ? 1 : 0;
}

int rs232dev_set_status(int fd, enum rs232handshake_out status) {
    return 0;
}

enum rs232handshake_in rs232dev_get_status(int fd) {
    return RS232_HSI_CTS | RS232_HSI_DSR;
}

void rs232dev_set_bps(int fd, unsigned int bps) {
    /* ignored — TCP doesn't have a baud rate */
}

#endif /* HAVE_RS232DEV */
