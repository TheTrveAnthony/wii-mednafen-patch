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

#include "main.h"

#include <ogc/machine/asm.h>
#include <ogc/machine/processor.h>
#include <ogc/system.h>
#include <string.h>

#include "mem2.h"

#ifdef WII_NETTRACE
#include <network.h>
#include "net_print.h"
#endif

static u8* mem2_ptr = NULL;
static u32 mem2_size = 0;
static u8* head = NULL;
static u8* last_head = NULL;
static u32 last_size = 0;

#define ROUNDUP32(v) (((u32)(v) + 0x1f) & ~0x1f)

void InitMem2Manager() {
    u32 level;
    _CPU_ISR_Disable(level);
    u32 size = ((uint32)(36 * 1024 * 1024)) + 1024 * 256;
    mem2_ptr = (u8*)ROUNDUP32((u32)SYS_GetArena2Hi() - size);
    SYS_SetArena2Hi(mem2_ptr);
    _CPU_ISR_Restore(level);
    mem2_size = size;
    Mem2ManagerReset();

#ifdef WII_NETTRACE
    net_print_string(NULL, 0, "InitMem2Manager: %d, %u\n", mem2_size, mem2_ptr);
#endif
}

void Mem2ManagerReset() {
    // memset( mem2_ptr, 0x0, mem2_size );
    head = last_head = (u8*)ROUNDUP32((u32)mem2_ptr);  // Align to 32 bytes
    last_size = 0;
}

u8* Mem2ManagerAlloc(u32 size, const char* purpose) {
    if ((((u32)head + size) - (u32)mem2_ptr) > mem2_size) {
        MDFN_PrintError("Unable to allocate '%s': %u bytes", purpose, size);
        return NULL;
    } else {
        last_head = head;
        last_size = size;
        head += size;
        head = (u8*)ROUNDUP32((u32)head);  // Align to 32 bytes

#ifdef WII_NETTRACE
        net_print_string(NULL, 0, "Mem2ManagerAlloc: %s = 0x%x, %0.2f\n",
                         purpose, last_head, ((float)(size) / 1048576.0));
#endif
        memset(last_head, 0x0, size);
        return last_head;
    }
}

u8* Mem2ManagerCalloc(int count, u32 size, const char* purpose) {
#ifdef WII_NETTRACE
    net_print_string(NULL, 0, "Mem2ManagerCalloc: %d 0x%x %s\n", count, size,
                     purpose);
#endif

    u32 realSize = size * count;
    u8* result = Mem2ManagerAlloc(realSize, purpose);
    return result;
}

u8* Mem2ManagerAdjust(u8* mem, u32 size, const char* purpose) {
    if (mem != last_head) {
        MDFN_PrintError("Unable to adjust '%s', not last allocation.", purpose);
        return NULL;
    } else if ((((u32)last_head + size) - (u32)mem2_ptr) > mem2_size) {
        MDFN_PrintError("Unable to adjust '%s': %u bytes", purpose, size);
        return NULL;
    }

    head = last_head + size;
    head = (u8*)ROUNDUP32((u32)head);  // Align to 32 bytes
    last_size = size;
#ifdef WII_NETTRACE
    net_print_string(NULL, 0, "Mem2ManagerAdjust: %s = 0x%x, %0.2f\n", purpose,
                     last_head, ((float)(size) / 1048576.0));
#endif
    return last_head;
}