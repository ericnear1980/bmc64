/*
 * circle_network.cpp - VICE socket layer for bare-metal Raspberry Pi.
 *
 * Implements the vice_network_* API (vicesocket.h) using Circle's TCP stack
 * via the circle_net_* C interface in circle.h.  Called from the VICE thread
 * (core 1); circle_yield() drives CNetSubSystem::Process() each frame.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

extern "C" {
#include "third_party/vice-3.9/src/vicesocket.h"
#include "third_party/common/circle.h"
}

struct vice_network_socket_s {
  circle_socket_t handle;
  int is_server;  // 1 = listen socket, 0 = data socket
};

struct vice_network_socket_address_s {
  char     ip[40];   // IPv4 dotted-decimal or "" for any
  unsigned short port;
};

// -------------------------------------------------------------------------

vice_network_socket_t *vice_network_server(
    const vice_network_socket_address_t *addr)
{
  if (!addr) return NULL;
  circle_socket_t h = circle_net_tcp_server(addr->port);
  if (!h) return NULL;
  vice_network_socket_t *s =
      (vice_network_socket_t *)malloc(sizeof(*s));
  s->handle    = h;
  s->is_server = 1;
  return s;
}

vice_network_socket_t *vice_network_client(
    const vice_network_socket_address_t *addr)
{
  if (!addr) return NULL;
  circle_socket_t h = circle_net_tcp_new();
  if (!h) return NULL;
  if (!circle_net_tcp_connect(h, addr->ip, addr->port)) {
    circle_net_close(h);
    return NULL;
  }
  vice_network_socket_t *s =
      (vice_network_socket_t *)malloc(sizeof(*s));
  s->handle    = h;
  s->is_server = 0;
  return s;
}

vice_network_socket_t *vice_network_accept(vice_network_socket_t *server)
{
  if (!server || !server->is_server) return NULL;
  circle_socket_t h = circle_net_tcp_accept(server->handle);
  if (!h) return NULL;
  vice_network_socket_t *s =
      (vice_network_socket_t *)malloc(sizeof(*s));
  s->handle    = h;
  s->is_server = 0;
  return s;
}

int vice_network_socket_close(vice_network_socket_t *s)
{
  if (!s) return -1;
  circle_net_close(s->handle);
  free(s);
  return 0;
}

// ssize_t mapped to int for bare-metal (no sys/types.h ssize_t)
int vice_network_send(vice_network_socket_t *s,
                      const void *buf, size_t len, int flags)
{
  if (!s) return -1;
  return circle_net_send(s->handle, buf, (unsigned)len);
}

int vice_network_receive(vice_network_socket_t *s,
                         void *buf, size_t len, int flags)
{
  if (!s) return -1;
  return circle_net_recv(s->handle, buf, (unsigned)len);
}

int vice_network_select_poll_one(vice_network_socket_t *s)
{
  if (!s) return 0;
  return circle_net_can_recv(s->handle);
}

// NULL-terminated array of sockets; returns index+1 of first readable, 0 if none.
int vice_network_select_multiple(vice_network_socket_t **socks)
{
  if (!socks) return 0;
  for (int i = 0; socks[i] != NULL; i++) {
    if (circle_net_can_recv(socks[i]->handle))
      return i + 1;
  }
  return 0;
}

vice_network_socket_address_t *vice_network_address_generate(
    const char *address, unsigned short port)
{
  vice_network_socket_address_t *a =
      (vice_network_socket_address_t *)malloc(sizeof(*a));
  if (address && *address)
    strncpy(a->ip, address, sizeof(a->ip) - 1);
  else
    a->ip[0] = '\0';
  a->ip[sizeof(a->ip) - 1] = '\0';
  a->port = port;
  return a;
}

void vice_network_address_close(vice_network_socket_address_t *a)
{
  free(a);
}

int vice_network_get_errorcode(void)
{
  return 0;
}
