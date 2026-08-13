/*
 * This file is part of Cleanflight and Betaflight.
 *
 * Cleanflight and Betaflight are free software. You can redistribute
 * this software and/or modify this software under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Cleanflight and Betaflight are distributed in the hope that they
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */

// fb_osd_impl backend for the SITL_LOCKSTEP target: the "framebuffer" is
// a bare character grid. The firmware's OSD pipeline (osd.c,
// osd_elements.c via io/displayport_fb_osd.c) draws into it exactly as
// it would drive a MAX7456; the grid is firmware state, so the
// multi-instance pipeline gives every instance its own screen for free.
// A host renderer maps font indices to glyphs to display it.

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "platform.h"

#if ENABLE_FB_OSD

#include "drivers/display.h"
#include "drivers/fb_osd_impl.h"
#include "drivers/osd.h"

#include "pg/pilot.h"

#include "sitl_lockstep.h"

#define OSD_SCREEN_ROWS VIDEO_LINES_PAL
#define OSD_SCREEN_COLS VIDEO_COLUMNS_SD

// Font index per cell (MAX7456-style: 0x20 = blank) and the displayport
// attribute per cell (severity in the low bits, DISPLAYPORT_BLINK in
// bit 7) so the renderer can colour and blink like goggles would.
static uint8_t osdScreen[OSD_SCREEN_ROWS][OSD_SCREEN_COLS];
static uint8_t osdAttr[OSD_SCREEN_ROWS][OSD_SCREEN_COLS];
static uint32_t osdDrawCount;

fbOsdInitStatus_e fbOsdInit(const struct fbOsdConfig_s *fbOsdConfig, const struct vcdProfile_s *vcdProfile)
{
    UNUSED(fbOsdConfig);
    UNUSED(vcdProfile);
    fbOsdClearScreen();
    return FB_OSD_INIT_OK;
}

bool fbOsdReInitIfRequired(bool forceStallCheck)
{
    UNUSED(forceStallCheck);
    return false; // a character grid cannot stall
}

bool fbOsdDrawScreen(void)
{
    // The grid is always fully drawn; never "still in progress".
    osdDrawCount++;
    return false;
}

bool fbOsdWriteFontCharacter(uint8_t charAddress, const uint8_t *fontData)
{
    UNUSED(charAddress);
    UNUSED(fontData);
    return true; // accept and ignore font uploads; glyphs live host-side
}

uint8_t fbOsdGetRowsCount(void)
{
    return OSD_SCREEN_ROWS;
}

void fbOsdWriteChar(uint8_t x, uint8_t y, uint8_t attr, uint8_t c)
{
    if (x < OSD_SCREEN_COLS && y < OSD_SCREEN_ROWS) {
        osdScreen[y][x] = c;
        osdAttr[y][x] = attr;
    }
}

void fbOsdWrite(uint8_t x, uint8_t y, uint8_t attr, const char *text)
{
    for (unsigned i = 0; text[i]; i++) {
        fbOsdWriteChar(x + i, y, attr, (uint8_t)text[i]);
    }
}

void fbOsdClearScreen(void)
{
    memset(osdScreen, 0x20, sizeof(osdScreen));
    memset(osdAttr, 0, sizeof(osdAttr));
}

void fbOsdRefreshAll(void)
{
    // NOOP: nothing downstream of the grid to re-push
}

bool fbOsdBufferInUse(void)
{
    return false;
}

bool fbOsdLayerSupported(displayPortLayer_e layer)
{
    return layer == DISPLAYPORT_LAYER_FOREGROUND;
}

bool fbOsdLayerSelect(displayPortLayer_e layer)
{
    return layer == DISPLAYPORT_LAYER_FOREGROUND;
}

bool fbOsdLayerCopy(displayPortLayer_e destLayer, displayPortLayer_e sourceLayer)
{
    UNUSED(destLayer);
    UNUSED(sourceLayer);
    return false;
}

void fbOsdSetBackgroundType(displayPortBackground_e backgroundType)
{
    UNUSED(backgroundType);
}

bool fbOsdDrawItem(osd_items_e item, uint8_t elemPosX, uint8_t elemPosY, bool isBackground)
{
    UNUSED(item);
    UNUSED(elemPosX);
    UNUSED(elemPosY);
    UNUSED(isBackground);
    return false; // no specialised handlers: every element draws as characters
}

void fbOsdRedrawBackground(void)
{
    // NOOP: background elements land in the same grid
}

void fbOsdFontUpdateCompletion(void)
{
    // NOOP
}

// ===========================================================================
// Harness access. Firmware-side like every bfl* accessor: the returned
// pointer is the active instance's grid, not the template's.
// ===========================================================================

const uint8_t *bflOsdScreen(void)
{
    return &osdScreen[0][0];
}

const uint8_t *bflOsdAttrs(void)
{
    return &osdAttr[0][0];
}

unsigned bflOsdRows(void)
{
    return OSD_SCREEN_ROWS;
}

unsigned bflOsdCols(void)
{
    return OSD_SCREEN_COLS;
}

uint32_t bflOsdDrawCount(void)
{
    return osdDrawCount;
}

// Default craft name for the active instance (shown by OSD_CRAFT_NAME).
// A name from a loaded config wins: only an empty name is replaced.
void bflOsdDefaultCraftName(const char *name)
{
    if (pilotConfig()->craftName[0] == '\0') {
        strncpy(pilotConfigMutable()->craftName, name, MAX_NAME_LENGTH);
        pilotConfigMutable()->craftName[MAX_NAME_LENGTH] = '\0';
    }
}

// Stock defaults enable no OSD elements; apply a classic FPV layout only
// when the loaded config has none of its own.
void bflOsdApplyDemoLayoutIfBlank(void)
{
    osdElementConfig_t *cfg = osdElementConfigMutable();
    for (int i = 0; i < OSD_ITEM_COUNT; i++) {
        // warnings is visible in the stock defaults (and draws nothing
        // most of the time); it alone does not make a layout
        if (i != OSD_WARNINGS && VISIBLE_IN_OSD_PROFILE(cfg->item_pos[i], 1)) {
            return;
        }
    }

    static const struct { uint8_t item, x, y; } demo[] = {
        { OSD_CRAFT_NAME,          6,  0 },
        { OSD_ITEM_TIMER_2,       23,  0 },
        { OSD_CROSSHAIRS,         13,  6 },
        // the AH band renders rows y..y+8 with level at y+4
        { OSD_ARTIFICIAL_HORIZON, 14,  2 },
        { OSD_HORIZON_SIDEBARS,   14,  6 },
        { OSD_WARNINGS,            9, 11 },
        { OSD_DISARMED,           11, 12 },
        { OSD_THROTTLE_POS,        1, 13 },
        { OSD_ALTITUDE,           23, 13 },
        { OSD_MAIN_BATT_VOLTAGE,   1, 14 },
        { OSD_MOTOR_DIAG,         13, 14 },
        { OSD_FLYMODE,            24, 14 },
    };
    for (unsigned i = 0; i < ARRAYLEN(demo); i++) {
        cfg->item_pos[demo[i].item] = OSD_POS(demo[i].x, demo[i].y) | OSD_PROFILE_1_FLAG;
    }
    osdAnalyzeActiveElements();
}

#endif // ENABLE_FB_OSD
