/*
 * archdep.c - Raspberry Pi bare-metal arch dependencies
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

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "archdep.h"
#include "archdep_defs.h"
#include "circle.h"
#include "keyboard.h"
#include "keymap.h"
#include "lib.h"
#include "log.h"
#include "machine.h"
#include "util.h"

int archdep_init(int *argc, char **argv) { return 0; }

const char *archdep_boot_path(void) { return ""; }

static char *default_path;

char *archdep_default_sysfile_pathlist(const char *emu_id) {
  /* VICE 3.9: sysfile_load() appends subpath (e.g. "C64", "DRIVES") via
   * findpath(). Return root "/" so findpath builds "/C64/kernal" etc. */
  if (default_path == NULL) {
    default_path = lib_strdup("/");
  }
  return default_path;
}

void archdep_shutdown(void) {
  if (default_path != NULL) {
    lib_free(default_path);
    default_path = NULL;
  }
}

/*
 * archdep_stat: raspi-specific override — calls circle_yield() first so
 * fake kernel threads can run while we do file I/O.
 */
int archdep_stat(const char *file_name, unsigned int *len,
                 unsigned int *isdir) {
  struct stat st;

  circle_yield();

  if (file_name == NULL) {
    *len = 0;
    *isdir = 0;
    return -1;
  }

  /* Ignore Pi boot files */
  if (strcasecmp("./kernel7.img", file_name) == 0 ||
      strcasecmp("./kernel~1.img", file_name) == 0 ||
      strcasecmp("./kernel8-32.img", file_name) == 0 ||
      strcasecmp("./config.txt", file_name) == 0 ||
      strcasecmp("./cmdline.txt", file_name) == 0 ||
      strcasecmp("./bootstat.txt", file_name) == 0 ||
      strcasecmp("./bootcode.bin", file_name) == 0 ||
      strcasecmp("./start.elf", file_name) == 0) {
    return -1;
  }

  if (stat(file_name, &st) == 0) {
    *len = st.st_size;
    *isdir = st.st_mode & S_IFDIR;
    return 0;
  }
  return -1;
}

int archdep_vice_atexit(void (*function)(void)) { return 0; }

int kbd_arch_get_host_mapping(void) { return KBD_MAPPING_US; }
