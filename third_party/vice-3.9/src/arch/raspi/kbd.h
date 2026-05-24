/*
 * kbd.h - Raspberry Pi keyboard definitions for VICE 3.9.
 *
 * KBD_PORT_PREFIX tells VICE 3.9's keymap system to look for
 * rpi_sym.vkm and rpi_pos.vkm in the C64/ directory on the SD card.
 */

#ifndef VICE_KBD_H
#define VICE_KBD_H

#define KBD_PORT_PREFIX "rpi"

void kbd_arch_init(void);
void kbd_arch_shutdown(void);
int kbd_arch_get_host_mapping(void);
void kbd_initialize_numpad_joykeys(int *joykeys);

signed long kbd_arch_keyname_to_keynum(char *keyname);
const char *kbd_arch_keynum_to_keyname(signed long keynum);

void kbd_hotkey_init(void);
void kbd_hotkey_shutdown(void);

#endif
