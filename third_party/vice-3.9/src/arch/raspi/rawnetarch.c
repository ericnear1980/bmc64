/*
 * rawnetarch.c - Raw ethernet interface for bare-metal Raspberry Pi (Circle).
 *
 * This implements the rawnetarch API that VICE's CS8900a/TFE emulation
 * calls to send and receive raw Ethernet frames.
 *
 * On the Raspberry Pi, Circle's network stack (CNetSubSystem) provides
 * access to the Ethernet hardware. Raw frame injection/capture requires
 * Circle's CBRawSocket or equivalent low-level interface.
 *
 * Written by
 *  Randy Rossi <randy.rossi@gmail.com>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "vice.h"

#ifdef HAVE_RAWNET

#include <stdint.h>
#include <string.h>

#include "lib.h"
#include "log.h"
#include "rawnetarch.h"

/* The circle.h header provides the C interface to the Circle kernel */
#include "circle.h"

static log_t rawnet_arch_log_local = LOG_DEFAULT;
log_t rawnet_arch_log = LOG_DEFAULT;

/* Receiver filter state */
static int rx_broadcast  = 1;
static int rx_ia         = 1;
static int rx_multicast  = 0;
static int rx_correct    = 1;
static int rx_promiscuous = 0;
static int rx_iahash     = 0;

static int tx_enabled    = 0;
static int rx_enabled    = 0;

static uint8_t current_mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* --------------------------------------------------------------------------
 * Resource / cmdline init — no resources needed for bare-metal
 * -------------------------------------------------------------------------- */

int rawnet_arch_resources_init(void)
{
    return 0;
}

int rawnet_arch_cmdline_options_init(void)
{
    return 0;
}

void rawnet_arch_resources_shutdown(void)
{
}

/* --------------------------------------------------------------------------
 * Lifecycle
 * -------------------------------------------------------------------------- */

int rawnet_arch_init(void)
{
    rawnet_arch_log_local = log_open("RawNet");
    rawnet_arch_log = rawnet_arch_log_local;
    return 1; /* success */
}

void rawnet_arch_pre_reset(void)
{
}

void rawnet_arch_post_reset(void)
{
}

int rawnet_arch_activate(const char *interface_name)
{
    log_message(rawnet_arch_log_local, "rawnet_arch_activate: %s (net_up=%d)",
                interface_name ? interface_name : "(null)",
                circle_net_is_up());
    /* Fetch the hardware MAC so we can filter received frames correctly. */
    if (circle_net_is_up()) {
        circle_net_get_mac(current_mac);
    }
    return 1; /* success — network may still be coming up */
}

void rawnet_arch_deactivate(void)
{
    /*
     * TODO: Close the raw socket and/or shut down Circle net.
     */
}

/* --------------------------------------------------------------------------
 * MAC and hash filter
 * -------------------------------------------------------------------------- */

void rawnet_arch_set_mac(const uint8_t mac[6])
{
    memcpy(current_mac, mac, 6);
}

void rawnet_arch_set_hashfilter(const uint32_t hash_mask[2])
{
    /* TODO: apply hash filter if Circle raw sockets support it */
}

/* --------------------------------------------------------------------------
 * Receive / transmit control
 * -------------------------------------------------------------------------- */

void rawnet_arch_recv_ctl(int bBroadcast, int bIA, int bMulticast,
                          int bCorrect, int bPromiscuous, int bIAHash)
{
    rx_broadcast  = bBroadcast;
    rx_ia         = bIA;
    rx_multicast  = bMulticast;
    rx_correct    = bCorrect;
    rx_promiscuous = bPromiscuous;
    rx_iahash     = bIAHash;
}

void rawnet_arch_line_ctl(int bEnableTransmitter, int bEnableReceiver)
{
    tx_enabled = bEnableTransmitter;
    rx_enabled = bEnableReceiver;
}

/* --------------------------------------------------------------------------
 * Transmit
 * -------------------------------------------------------------------------- */

void rawnet_arch_transmit(int force, int onecoll, int inhibit_crc,
                          int tx_pad_dis, int txlength, uint8_t *txframe)
{
    if (!tx_enabled) {
        return;
    }
    circle_net_send_frame(txframe, (unsigned)txlength);
}

/* --------------------------------------------------------------------------
 * Receive
 * -------------------------------------------------------------------------- */

int rawnet_arch_receive(uint8_t *pbuffer, int *plen, int *phashed,
                        int *phash_index, int *prx_ok, int *pcorrect_mac,
                        int *pbroadcast, int *pcrc_error)
{
    if (!rx_enabled) {
        return 0;
    }

    unsigned frame_len = 0;
    if (!circle_net_recv_frame(pbuffer, &frame_len) || frame_len == 0) {
        return 0;
    }

    *plen        = (int)frame_len;
    *prx_ok      = 1;
    *pcrc_error  = 0;
    *phashed     = 0;
    *phash_index = 0;

    /* Determine if frame is broadcast (dest ff:ff:ff:ff:ff:ff) */
    *pbroadcast = (pbuffer[0] == 0xff && pbuffer[1] == 0xff &&
                   pbuffer[2] == 0xff && pbuffer[3] == 0xff &&
                   pbuffer[4] == 0xff && pbuffer[5] == 0xff) ? 1 : 0;

    /* Check if dest MAC matches our assigned MAC */
    *pcorrect_mac = (memcmp(pbuffer, current_mac, 6) == 0 ||
                     *pbroadcast || rx_promiscuous) ? 1 : 0;

    return 1;
}

/* --------------------------------------------------------------------------
 * Adapter enumeration — bare metal has exactly one interface
 * -------------------------------------------------------------------------- */

static int enum_done = 0;

int rawnet_arch_enumadapter_open(void)
{
    enum_done = 0;
    return 1;
}

int rawnet_arch_enumadapter(char **ppname, char **ppdescription)
{
    if (enum_done) {
        return 0;
    }
    *ppname        = lib_strdup("eth0");
    *ppdescription = lib_strdup("Raspberry Pi Ethernet");
    enum_done = 1;
    return 1;
}

int rawnet_arch_enumadapter_close(void)
{
    return 1;
}

char *rawnet_arch_get_standard_interface(void)
{
    return lib_strdup("eth0");
}

/* --------------------------------------------------------------------------
 * Driver enumeration (new in VICE 3.9)
 * -------------------------------------------------------------------------- */

static int enum_drv_done = 0;

int rawnet_arch_enumdriver_open(void)
{
    enum_drv_done = 0;
    return 1;
}

int rawnet_arch_enumdriver(char **ppname, char **ppdescription)
{
    if (enum_drv_done) {
        return 0;
    }
    *ppname        = lib_strdup("circle");
    *ppdescription = lib_strdup("Circle bare-metal network driver");
    enum_drv_done = 1;
    return 1;
}

int rawnet_arch_enumdriver_close(void)
{
    return 1;
}

char *rawnet_arch_get_standard_driver(void)
{
    return lib_strdup("circle");
}

#endif /* HAVE_RAWNET */
