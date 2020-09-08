/* Copyright 2020 KeyPCB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _NUMBERS,
    _FUNCTION
};

#define KB_1 KC_W
#define KB_2 KC_Y
#define KB_3 KC_R
#define KB_4 KC_I
#define KB_N_1 KC_5
#define KB_N_2 KC_4
#define KB_N_3 KC_3
#define KB_N_4 KC_2
#define KB_F_1 KC_F5
#define KB_F_2 KC_F4
#define KB_F_3 KC_F3
#define KB_F_4 KC_F2

#define KB_BLACK KC_SPC
#define KB_RED OSL(_NUMBERS)
#define KB_BLUE OSM(MOD_LSFT)
#define KB_N_BLACK KC_1
#define KB_N_RED KC_SPC
// KB_N_BLUE is same as KB_BLUE
#define KB_F_BLACK KC_F1
#define KB_F_RED KC_NO
// KB_F_BLUE is same as KB_BLUE

enum custom_keycodes {
    PAREN_SHIFT,
    KC_00,
    KC_000,
    LATCH,
    RIGHT,
    // Custom ctrl, gui, alt
    CCTRL,
    CGUI,
    CALT
};

#include "g/keymap_combo.h"

// All of the keys entered with chords are in combos_source.def
// The keys entered with a single keypress are in the above #define block
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
        KB_1, KB_2, KB_3, KB_4,
              KB_RED, KB_BLACK, KB_BLUE
    ),
    [_NUMBERS] = LAYOUT(
        KB_N_1, KB_N_2, KB_N_3, KB_N_4,
                KB_N_RED, KB_N_BLACK, KB_BLUE
    ),
    [_FUNCTION] = LAYOUT(
        KB_F_1, KB_F_2, KB_F_3, KB_F_4,
                KB_F_RED, KB_F_BLACK, KB_BLUE
    )
};

bool latchMode = false;
uint16_t latchedKey = 0;

bool rightMode = false;

void process_mod_latch_support(uint16_t mod_left, uint16_t mod_right, bool keyPressed) {
    uint8_t result = rightMode ? mod_right : mod_left;
    if(latchMode && keyPressed) {
        if(latchedKey != 0) {
            unregister_code16(latchedKey);
        }
        register_code16(result);
        latchedKey = result;
    }
    else {
        if(keyPressed) {
            register_code16(OSM(MOD_BIT(result)));
        }
        else {
            unregister_code16(OSM(MOD_BIT(result)));
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case PAREN_SHIFT: {
            uint16_t result = (keyboard_report->mods & (MOD_BIT(KC_LSFT))) ? KC_RPRN : KC_LPRN;
            if(record->event.pressed) {
                register_code16(result);
            } else {
                unregister_code16(result);
            }
            break;
        }
        case KC_00:
            if(record->event.pressed) {
                tap_code16(KC_KP_0);
                tap_code16(KC_KP_0);
            }
            break;
        case KC_000:
            if(record->event.pressed) {
                tap_code16(KC_KP_0);
                tap_code16(KC_KP_0);
                tap_code16(KC_KP_0);
            }
            break;
        case LATCH:
            if(record->event.pressed) {
                if(latchMode && latchedKey != 0) {
                    // Unlatch
                    unregister_code16(latchedKey);
                    latchedKey = 0;
                }
                latchMode = !latchMode;
            }
            break;
        case RIGHT:
            rightMode = !rightMode;
            break;
        case CCTRL:
            process_mod_latch_support(KC_LCTL, KC_RCTL, record->event.pressed);
            break;
        case CGUI:
            process_mod_latch_support(KC_LGUI, KC_RGUI, record->event.pressed);
            break;
        case CALT:
            process_mod_latch_support(KC_LALT, KC_RALT, record->event.pressed);
            break;
    }

    return true;
}