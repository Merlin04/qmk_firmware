/*
 * This keymap has a few features:
 * - Only one layer (uses combos)
 * - Home row mods that use tap-dance, so double-tapping
 *   the key and pressing another while it is held will
 *   trigger the modifier. This is different from normal
 *   tap dance as it allows you to press the key multiple
 *   times without waiting the tapping term duration. This
 *   feature is what most of the code in this file is for.
 * - Tapping term can be changed per key
 * - Doesn't use the two outermost thumb keys, so you can
 *   easily port this layout over to a 34-key keyboard.
 *
 * This keymap relies on a fix to QMK being implemented,
 * see pull request 6259 or 9806 (a duplicate). It will
 * probably be a while before 6259 is merged as it is a
 * breaking change.
 */

/*
 * How to add a new home row td_th (tap dance tap hold) key:
 * Go to all the places that say "Edit this to add a td_th"
 * and modify the code to add the new key.
 * Ideally a lot of the changes would be unnecessary but
 * I couldn't figure out a way to remove them.
*/

#define BASE 0 // default layer
#define SYMB 1 // symbols

#include QMK_KEYBOARD_H
#include "g/keymap_combo.h"

// Tap dance

enum {
    SINGLE_TAP = 1,
    DOUBLE_TAP,
    DOUBLE_HOLD
};

typedef struct {
    uint8_t count;
    uint8_t type;
    uint16_t tap_kc;
    uint16_t hold_kc;
} td_tap_result;

enum {
    TD_TAP,
    TD_HOLD
};

// Tap dance enums
#define td_th_count 6
// Edit this to add a td_th
// This will determine the names of all of the td_th keys
enum {
    TD_A,
    TD_R,
    TD_S,
    TD_O,
    TD_I,
    TD_E
};

// TODO: find a way to make this less repetitive
// Edit this to add a td_th
// Just add one entry in the array for your new td_th key
// This should probably be in progmem but I couldn't figure
// out how to read it into a td_tap_result
static td_tap_result td_states[td_th_count] = {
    [TD_A] = {
        .count = 0,
        .type = 0,
        .tap_kc = KC_A,
        .hold_kc = KC_LCTL
    },
    [TD_R] = {
        .count = 0,
        .type = 0,
        .tap_kc = KC_R,
        .hold_kc = KC_LALT
    },
    [TD_S] = {
        .count = 0,
        .type = 0,
        .tap_kc = KC_S,
        .hold_kc = KC_LGUI
    },
    [TD_O] = {
        .count = 0,
        .type = 0,
        .tap_kc = KC_O,
        .hold_kc = KC_LCTL
    },
    [TD_I] = {
        .count = 0,
        .type = 0,
        .tap_kc = KC_I,
        .hold_kc = KC_LALT
    },
    [TD_E] = {
        .count = 0,
        .type = 0,
        .tap_kc = KC_E,
        .hold_kc = KC_LGUI
    }
};

// td_th: tap dance tap/hold
// typedef struct {
//     uint16_t tap_kc;
//     uint16_t hold_kc;
// } td_th;

// Edit this to add a td_th
// These are the definitions for what the keys do

void td_finished(tap_dance_state_t *state, void *user_data);
void td_reset(tap_dance_state_t *state, void *user_data);

/* Combomap
 *
 * ,-----------------------------.      ,-----------------------------.
 * |     `     |     |    ESC    |      |     |     |     =     -     |
 * |-----+-----+-----+-----+-----|      |-----------------------------|
 * |     |     |     |     |     |      |     '     |     |     |     |
 * |-----+-----+-----+-----+-----+		|-----------------------------|
 * |    TAB    |     |     |     |      |    HOME  PGDN  PGUP  END    |
 * `------+----+-----+-----+----'		`-----------------------------'
 *
 */

// Here's the actual keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_split_3x5_3(
        KC_Q, KC_W, KC_F, KC_P, KC_G, KC_J, KC_L, KC_U, KC_Y, KC_SCLN,
        TD(TD_A), TD(TD_R), TD(TD_S), KC_T, KC_D, KC_H, KC_N, TD(TD_E), TD(TD_I), TD(TD_O),
        KC_Z, KC_X, KC_C, KC_V, KC_B, KC_K, KC_M, KC_COMM, KC_DOT, KC_SLSH,
        KC_RSFT, KC_BSPC, MT(MOD_LSFT, KC_SPC), KC_SPC, LT(SYMB, KC_ENT), KC_RCTL
    ),
    [SYMB] = LAYOUT_split_3x5_3(
        KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0,
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT,
        KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_CAPS, KC_LBRC, KC_RBRC, KC_BSLS,
        KC_TRNS, KC_DEL, KC_TRNS, KC_TRNS, KC_TRNS, QK_BOOT
    )
};

// More tap dance stuff
#define DEFAULT_TAPPING_TERM 200
uint16_t default_tapping_term = DEFAULT_TAPPING_TERM;

void td_each(tap_dance_state_t *state, void *user_data) {
    // The only purpose of this is to set the tapping term to something lower for the next taps
    if(state->count == 2) {
        default_tapping_term = 100;
    }
}

void td_finished(tap_dance_state_t *state, void *user_data) {
    // uint16_t stateKeycode = state->interrupting_keycode;
    td_tap_result* this = user_data;
    this->count = state->count;
    if(this->count == 2 && !(state->interrupted) && state->pressed) this->type = TD_HOLD;
    else this->type = TD_TAP;
    switch(this->count) {
        case 1:
            register_code16(this->tap_kc);
            break;
        case 2:
            if(this->type == TD_HOLD) {
                register_code16(this->hold_kc);
            }
            else {
                tap_code16(this->tap_kc);
                register_code16(this->tap_kc);
            }
            break;
        default:
            for(uint8_t i = 0; i < this->count - 1; i++) {
                tap_code16(this->tap_kc);
            }
            register_code16(this->tap_kc);
            break;
    }
}

void td_reset(tap_dance_state_t *state, void *user_data) {
    // uint16_t stateKeycode = state->interrupting_keycode;
    td_tap_result* this = user_data;
    if(this->count == 2 && this->type == TD_HOLD) {
        unregister_code16(this->hold_kc);
    }
    else {
        unregister_code16(this->tap_kc);
    }
    this->count = 0;
    this->type = 0;
    default_tapping_term = DEFAULT_TAPPING_TERM;
}

#define ACTION_TAP_DANCE_FN_ADVANCED_2(user_fn_on_each_tap, user_fn_on_dance_finished, user_fn_on_dance_reset, ud) \
{ .fn = {user_fn_on_each_tap, user_fn_on_dance_finished, user_fn_on_dance_reset, NULL}, .user_data = ud, }

// Edit this to add a td_th
// Just add an item for your key
tap_dance_action_t tap_dance_actions[] = {
    [TD_A] = ACTION_TAP_DANCE_FN_ADVANCED_2(td_each, td_finished, td_reset, (&td_states[TD_A])),
    [TD_R] = ACTION_TAP_DANCE_FN_ADVANCED_2(td_each, td_finished, td_reset, (&td_states[TD_R])),
    [TD_S] = ACTION_TAP_DANCE_FN_ADVANCED_2(td_each, td_finished, td_reset, (&td_states[TD_S])),
    [TD_O] = ACTION_TAP_DANCE_FN_ADVANCED_2(td_each, td_finished, td_reset, (&td_states[TD_O])),
    [TD_I] = ACTION_TAP_DANCE_FN_ADVANCED_2(td_each, td_finished, td_reset, (&td_states[TD_I])),
    [TD_E] = ACTION_TAP_DANCE_FN_ADVANCED_2(td_each, td_finished, td_reset, (&td_states[TD_E]))
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case MT(MOD_LSFT, KC_SPC):
            return 100;
        case LT(SYMB, KC_ENT):
            return 150;
        default:
            return default_tapping_term;
    }
}

bool get_ignore_mod_tap_interrupt(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case MT(MOD_LSFT, KC_SPC):
            return false;
        default:
            return true;
    }
}