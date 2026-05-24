/*
 * missing.c
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

#include "missing.h"

#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/time.h>

#include "ui.h"
#include "raspi_machine.h"
#include "videoarch.h"

// ------------------------------------------------------------------------
// These are stubs to get things compiling. The vast majority of these
// routines will not require an implementation. Once a routine is given
// an implementation, it should be moved out of missing.c and into
// one of the other .c files in this directory (or a new one if it deserves
// it).
// ------------------------------------------------------------------------

/* --- UI stubs --- */
char *ui_get_file(const char *format, ...) { return 0; }
char *uimon_get_in(char **p1, const char *p2) { return 0; }
char video_canvas_can_resize(struct video_canvas_s *canvas) { return 0; }
int archdep_rtc_get_centisecond(void) { return 0; }

int c128ui_init_early(void) { return 0; }
int c128ui_init(void) { ui_init_menu(); return 0; }
int c64dtvui_init_early(void) { return 0; }
int c64dtvui_init(void) { return 0; }
int c64scui_init_early(void) { return 0; }
int c64scui_init(void) { return 0; }
int c64ui_init_early(void) { return 0; }
int c64ui_init(void) { ui_init_menu(); return 0; }
int cbm2ui_init_early(void) { return 0; }
int cbm2ui_init(void) { return 0; }
int cbm5x0ui_init_early(void) { return 0; }
int cbm5x0ui_init(void) { return 0; }
int console_close_all(void) { return 0; }
int console_init(void) { return 0; }
int dthread_ui_init_finish(void) { return 0; }
int dthread_ui_init(int *argc, char **argv) { return 0; }
int joy_arch_cmdline_options_init(void) { return 0; }
int joy_arch_resources_init(void) { return 0; }
int joy_arch_set_device(int port_idx, int new_dev) { return 0; }
int mui_init(void) { return 0; }
int petui_init_early(void) { return 0; }
int petui_init(void) { ui_init_menu(); return 0; }
int plus4ui_init_early(void) { return 0; }
int plus4ui_init(void) { ui_init_menu(); return 0; }
int scpu64ui_init_early(void) { return 0; }
int scpu64ui_init(void) { return 0; }
int ui_cmdline_options_init(void) { return 0; }
int ui_extend_image_dialog(void) { return 0; }
/* ui_init signature changed in VICE 3.5: no longer takes argc/argv */
int ui_init(void) { return 0; }
void ui_init_with_args(int *argc, char **argv) {}
int ui_init_finalize(void) { return 0; }
int ui_resources_init(void) { return 0; }
int vic20ui_init_early(void) { return 0; }
int vic20ui_init(void) { ui_init_menu(); return 0; }
int video_arch_cmdline_options_init(void) { return 0; }
int video_arch_resources_init(void) { return 0; }
int video_init(void) { return 0; }
int vsid_ui_init(void) { return 0; }
struct console_s *uimon_window_open(void) { return 0; }
struct console_s *uimon_window_resume(void) { return 0; }
ui_jam_action_t ui_jam_dialog(const char *format, ...) { return UI_JAM_NONE; }

/* --- New in VICE 3.5+: keyboard arch --- */
void kbd_arch_shutdown(void) {}
void kbd_hotkey_init(void) {}
void kbd_hotkey_shutdown(void) {}

/* --- New in VICE 3.5+: UI actions / pause --- */
int ui_pause_active(void) { return 0; }
void ui_pause_enable(void) {}
void ui_pause_disable(void) {}
void arch_ui_activate(void) {}
int ui_action_get_id(const char *name) { return -1; }
void ui_action_trigger(int action) {}
void ui_update_lightpen(void) {}
void ui_dispatch_events(void) {}
void ui_display_reset(int drive_num, int drive_led_color) {}
void ui_display_statustext(const char *text, bool fade_out) {}
void ui_message(const char *format, ...) {}

/* --- New in VICE 3.5+: video arch --- */
/* Returns VIDEO_CHIP_VICII (0) by default; videoarch.c overrides this */
int video_arch_get_active_chip(void) { return 0; }

/* --- New in VICE 3.5+: vsync advance frame (single-step) --- */
void vsyncarch_advance_frame(void) {}

/* --- New in VICE 3.5+: uimon binary protocol outputs --- */
int uimon_petscii_out(const char *buffer, int num) { return 0; }
int uimon_petscii_upper_out(const char *buffer, int num) { return 0; }
int uimon_scrcode_out(const char *buffer, int num) { return 0; }
int uimon_scrcode_upper_out(const char *buffer, int num) { return 0; }

/* --- New in VICE 3.x: VSID (SID player) stubs --- */
void vsid_ui_close(void) {}
void vsid_ui_display_author(const char *author) {}
void vsid_ui_display_copyright(const char *copyright) {}
void vsid_ui_display_irqtype(const char *irq) {}
void vsid_ui_display_name(const char *name) {}
void vsid_ui_display_nr_of_tunes(int count) {}
void vsid_ui_display_sid_model(int model) {}
void vsid_ui_display_sync(int sync) {}
void vsid_ui_display_time(unsigned int sec) {}
void vsid_ui_display_tune_nr(int nr) {}
void vsid_ui_set_data_size(int size) {}
void vsid_ui_set_default_tune(int tune) {}
void vsid_ui_set_driver_addr(unsigned int addr) {}
void vsid_ui_set_init_addr(unsigned int addr) {}
void vsid_ui_set_load_addr(unsigned int addr) {}
void vsid_ui_set_play_addr(unsigned int addr) {}
void vsid_ui_setdrv(char *driver_info_text) {}

/* --- Misc stubs --- */
void archdep_signals_init(int do_core_dumps) {}
void c128ui_shutdown(void) {}
void c64dtvui_shutdown(void) {}
void c64scui_shutdown(void) {}
void c64ui_shutdown(void) {}
void cbm2ui_shutdown(void) {}
void cbm5x0ui_shutdown(void) {}
void fullscreen_capability(struct cap_fullscreen_s *cap_fullscreen) {}
void joy_arch_init_default_mapping(int joynum) {}
void petui_shutdown(void) {}
void plus4ui_shutdown(void) {}
void scpu64ui_shutdown(void) {}
void signals_init(int do_core_dumps) {}
void ui_display_drive_current_image(unsigned int unit_number,
                                    unsigned int drive_number,
                                    const char *image) {}
void ui_display_drive_track(unsigned int drive_number, unsigned int drive_base,
                            unsigned int half_track_number,
                            unsigned int disk_side) {}
void ui_display_event_time(unsigned int current, unsigned int total) {}
void ui_display_joyport(uint16_t *joyport) {}
void ui_display_playback(int playback_status, char *version) {}
void ui_display_recording(int recording_status) {}
void ui_display_tape_current_image(int port, const char *image) {}
void ui_error_string(const char *text) {}
void uimon_notify_change(void) {}
int uimon_out(const char *buffer) { return 0; }
void uimon_set_interface(struct monitor_interface_s **p1, int p2) {}
void uimon_window_close(void) {}
void uimon_window_suspend(void) {}
void ui_resources_shutdown(void) {}
void ui_set_tape_status(int port, int tape_status) {}
void ui_shutdown(void) {}
void ui_update_menus(void) {}
void vic20ui_shutdown(void) {}
void video_arch_resources_shutdown(void) {}
void video_canvas_destroy(struct video_canvas_s *canvas) {}
void video_canvas_resize(struct video_canvas_s *canvas, char resize_canvas) {}
void video_shutdown(void) {}
void vsyncarch_display_speed(double speed, double fps, int warp_enabled) {}
void ui_display_volume(int vol) {}
void main_exit(void) {}

extern struct video_canvas_s *vic_canvas;
video_canvas_t *ui_get_active_canvas(void) { return vic_canvas; }
bool ui_pause_loop_iteration(void) { return false; }


/* --- POSIX stubs --- */
#include <string.h>
#include <sys/types.h>
char *getcwd(char *buf, size_t size) { if (buf && size > 1) { buf[0] = '/'; buf[1] = 0; } return buf; }
int mkdir(const char *path, mode_t mode) { return -1; }
int rmdir(const char *path) { return -1; }
char *realpath(const char *path, char *resolved) { if (resolved) { strncpy(resolved, path, 4095); resolved[4095] = 0; } return resolved; }
int execvp(const char *file, char *const argv[]) { return -1; }
uid_t getuid(void) { return 0; }
struct passwd { const char *pw_dir; };
struct passwd *getpwuid(uid_t uid) { return NULL; }
pid_t waitpid(pid_t pid, int *status, int options) { return -1; }
int usleep(unsigned long usec) { return 0; }
int nanosleep(const struct timespec *req, struct timespec *rem) { return 0; }

/* --- sid_done stub --- */
void sid_done(void) {}

/* --- UI action stubs --- */
#include "arch/shared/uiactions.h"
void ui_actions_init(void) {}
void ui_actions_shutdown(void) {}
bool ui_action_is_valid(int action) { return false; }
const char *ui_action_get_name(int action) { return ""; }
ui_action_map_t *ui_action_map_set_hotkey(int action, uint32_t vice_keysym, uint32_t vice_modmask, uint32_t arch_keysym, uint32_t arch_modmask) { return NULL; }
void ui_action_map_clear_hotkey(ui_action_map_t *map) {}
ui_action_map_t *ui_action_map_get(int action) { return NULL; }
ui_action_map_t *ui_action_map_get_by_hotkey(uint32_t keysym, uint32_t modmask) { return NULL; }
void ui_hotkeys_arch_install_by_map(ui_action_map_t *map) {}
void ui_hotkeys_arch_remove_by_map(ui_action_map_t *map) {}
void ui_hotkeys_arch_shutdown(void) {}
uint32_t ui_hotkeys_arch_keysym_to_arch(uint32_t key) { return 0; }
uint32_t ui_hotkeys_arch_modifier_to_arch(uint32_t key) { return 0; }
uint32_t ui_hotkeys_arch_modmask_to_arch(uint32_t mask) { return 0; }

/* Network is implemented in circle_network.cpp */
