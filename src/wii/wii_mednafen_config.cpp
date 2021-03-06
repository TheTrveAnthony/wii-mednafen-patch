/*--------------------------------------------------------------------------*\
|  __      __.__.__   _____             .___             _____               |
| /  \    /  \__|__| /     \   ____   __| _/____ _____ _/ ____\____   ____   |
| \   \/\/   /  |  |/  \ /  \_/ __ \ / __ |/    \\__  \\   __\/ __ \ /    \  |
|  \        /|  |  /    Y    \  ___// /_/ |   |  \/ __ \|  | \  ___/|   |  \ |
|   \__/\  / |__|__\____|__  /\___  >____ |___|  (____  /__|  \___  >___|  / |
|        \/                \/     \/     \/    \/     \/          \/     \/  |
|                                                                            |
|    WiiMednafen by raz0red                                                  |
|    Wii port of the Mednafen emulator                                       |
|                                                                            |
|    [github.com/raz0red/wii-mednafen]                                       |
|                                                                            |
+----------------------------------------------------------------------------+
|                                                                            |
|    This program is free software; you can redistribute it and/or           |
|    modify it under the terms of the GNU General Public License             |
|    as published by the Free Software Foundation; either version 2          |
|    of the License, or (at your option) any later version.                  |
|                                                                            |
|    This program is distributed in the hope that it will be useful,         |
|    but WITHOUT ANY WARRANTY; without even the implied warranty of          |
|    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           |
|    GNU General Public License for more details.                            |
|                                                                            |
|    You should have received a copy of the GNU General Public License       |
|    along with this program; if not, write to the Free Software             |
|    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA           |
|    02110-1301, USA.                                                        |
|                                                                            |
\*--------------------------------------------------------------------------*/

#include <stdio.h>

#include "wii_mednafen.h"
#include "wii_config.h"
#include "wii_util.h"

#include "Emulators.h"
#include "networkop.h"

/**
 * Handles reading a particular configuration value
 *
 * @param   name The name of the config value
 * @param   valueThe config value
 */
void wii_config_handle_read_value(char* name, char* value) {
    if (strcmp(name, "debug") == 0) {
        wii_debug = Util_sscandec(value);
    } else if (strcmp(name, "top_menu_exit") == 0) {
        wii_top_menu_exit = Util_sscandec(value);
    } else if (strcmp(name, "sel_offset") == 0) {
        wii_menu_sel_offset = Util_sscandec(value);
    } else if (strcmp(name, "sel_color") == 0) {
        Util_hextorgba(value, &wii_menu_sel_color);
    } else if (strcmp(name, "mote_menu_vertical") == 0) {
        wii_mote_menu_vertical = Util_sscandec(value);
    } else if (strcmp(name, "language") == 0) {
        Util_strlcpy(wii_language, value, sizeof(wii_language));
    } else if (strcmp(name, "video_filter") == 0) {
        wii_filter = Util_sscandec(value);
    } else if (strcmp(name, "cheats") == 0) {
        wii_cheats = Util_sscandec(value);
    } else if (strcmp(name, "double_strike") == 0) {
        wii_double_strike_mode = Util_sscandec(value);
    } else if (strcmp(name, "full_widescreen") == 0) {
        wii_full_widescreen = Util_sscandec(value);
    } else if (strcmp(name, "roms_dir") == 0) {
        wii_set_roms_dir(value);
    } else if (strcmp(name, "share_ip") == 0) {
        setSmbAddress(value);
    } else if (strcmp(name, "share_name") == 0) {
        setSmbShare(value);
    } else if (strcmp(name, "share_user") == 0) {
        setSmbUser(value);
    } else if (strcmp(name, "share_pass") == 0) {
        setSmbPassword(value);
    } else if (strcmp(name, "usb_keepalive") == 0) {
        wii_usb_keepalive = Util_sscandec(value);
    } else if (strcmp(name, "trap_filter") == 0) {
        wii_trap_filter = Util_sscandec(value);
    } else if (strcmp(name, "vi_gx_scaler") == 0) {
        wii_gx_vi_scaler = Util_sscandec(value);
    } else if (strcmp(name, "16_9_correct") == 0) {
        wii_16_9_correction = Util_sscandec(value);
    } else if (strcmp(name, "volume") == 0) {
        wii_volume = Util_sscandec(value);
    } else if (strcmp(name, "rewind") == 0) {
        wii_rewind = Util_sscandec(value);
    } else if (strcmp(name, "rewind_add_buttons") == 0) {
        wii_rewind_add_buttons = Util_sscandec(value);
    }

    else {
        // Read configuration value for emulators
        emuRegistry.readConfigValue(name, value);
    }
}

/**
 * Handles writing of the configuration file
 *
 * @param   fp The file pointer
 */
void wii_config_handle_write_config(FILE* fp) {
    fprintf(fp, "share_ip=%s\n", getSmbAddress());
    fprintf(fp, "share_name=%s\n", getSmbShare());
    fprintf(fp, "share_user=%s\n", getSmbUser());
    fprintf(fp, "share_pass=%s\n", getSmbPassword());
    fprintf(fp, "debug=%d\n", wii_debug);
    fprintf(fp, "top_menu_exit=%d\n", wii_top_menu_exit);
    fprintf(fp, "sel_offset=%d\n", wii_menu_sel_offset);
    fprintf(fp, "mote_menu_vertical=%d\n", wii_mote_menu_vertical);
    fprintf(fp, "video_filter=%d\n", wii_filter);
    fprintf(fp, "cheats=%d\n", wii_cheats);
    fprintf(fp, "language=%s\n", wii_language);
    fprintf(fp, "roms_dir=%s\n", wii_get_roms_dir());
    fprintf(fp, "double_strike=%d\n", wii_double_strike_mode);
    fprintf(fp, "full_widescreen=%d\n", wii_full_widescreen);
    fprintf(fp, "usb_keepalive=%d\n", wii_usb_keepalive);
    fprintf(fp, "trap_filter=%d\n", wii_trap_filter);
    fprintf(fp, "vi_gx_scaler=%d\n", wii_gx_vi_scaler);
    fprintf(fp, "16_9_correct=%d\n", wii_16_9_correction);
    fprintf(fp, "volume=%d\n", wii_volume);
    fprintf(fp, "rewind=%d\n", wii_rewind);
    fprintf(fp, "rewind_add_buttons=%d\n", wii_rewind_add_buttons);

    // Write configuration settings for emulators
    emuRegistry.writeConfig(fp);
}