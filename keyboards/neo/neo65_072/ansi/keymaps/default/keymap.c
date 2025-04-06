/* 
Copyright 2023 NEO

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

enum custom_keycodes {
    TG_NUL = SAFE_RANGE,
    TG_DBG = SAFE_RANGE + 1,
};

static bool TOGGLE_NULL = true;

static bool TOGGLE_DEBUG = false;

static bool a_held, d_held, a_scrip, d_scrip = 0;

static uint16_t repeat_timer = 0;
static uint16_t hold_timer = 0;
static uint16_t last_keycode = 0;
static bool is_repeating = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT_hot(
		KC_ESC,  KC_1,    KC_2,    KC_3,   KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_BSPC, KC_DEL,   
		KC_TAB,  KC_Q,    KC_W,    KC_E,   KC_R,     KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,  
		KC_CAPS, KC_A,    KC_S,    KC_D,   KC_F,     KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGDN, 
		KC_LSFT,          KC_Z,    KC_X,   KC_C,     KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_END, 
		KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                             KC_RALT, MO(1),            KC_LEFT, KC_DOWN, KC_RIGHT
	),

	[1] = LAYOUT_hot(
		KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_TRNS, TG_DBG, TG_NUL,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MPRV,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, NK_TOGG, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_MPLY,
		KC_TRNS,          RGB_TOG, RGB_MOD, RGB_RMOD,RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, KC_TRNS, KC_TRNS, KC_PRINT_SCREEN, KC_MNXT,
		KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS,                            KC_TRNS, KC_TRNS,          KC_AUDIO_VOL_DOWN, KC_HOME, KC_AUDIO_VOL_UP     
	)//
};

#ifdef RGB_MATRIX_ENABLE

void keyboard_post_init_user(void) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(HSV_WHITE);
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (host_keyboard_led_state().caps_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(31, 0x80, 0x00, 0x80);
    } else {
        RGB_MATRIX_INDICATOR_SET_COLOR(31, 0xFF, 0xFF, 0xFF);
    }

    if (TOGGLE_NULL) {
        RGB_MATRIX_INDICATOR_SET_COLOR(15, 0x80, 0x00, 0x80);
    } else {
        RGB_MATRIX_INDICATOR_SET_COLOR(15, 0xFF, 0xFF, 0xFF);
    }

    if (TOGGLE_DEBUG) {
    RGB_MATRIX_INDICATOR_SET_COLOR(13, 0x80, 0x00, 0x80);
    RGB_MATRIX_INDICATOR_SET_COLOR(14, 0x80, 0x00, 0x80);
    } else {
    RGB_MATRIX_INDICATOR_SET_COLOR(13, 0xFF, 0xFF, 0xFF);
    RGB_MATRIX_INDICATOR_SET_COLOR(14, 0xFF, 0xFF, 0xFF);
    }
    return false;
}

#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    //Check for Ctrl + any key repetition
    bool ctrl_active = get_mods() & (MOD_BIT(KC_LCTL) & (!MOD_BIT(KC_LALT)));
    if (ctrl_active && keycode != KC_LCTL && keycode != KC_LSFT) {
        if (record->event.pressed) {
            uprintf("Ctrl pressed with keycode: %u\n", keycode);
            last_keycode = keycode;
            repeat_timer = timer_read();
            hold_timer = timer_read();
            is_repeating = true;
        } else {
            uprintf("Key released: %u\n", keycode);
            is_repeating = false;
            last_keycode = 0;
            hold_timer = 0;
        }
    }

    if (keycode == KC_AUDIO_VOL_DOWN || keycode == KC_AUDIO_VOL_UP) {
        if (record->event.pressed) {
            uprintf("VOL DOWN/UP: %u\n", keycode);
            last_keycode = keycode;
            repeat_timer = timer_read();
            hold_timer = timer_read();
            is_repeating = true;
        } else {
            is_repeating = false;
            last_keycode = 0;
            hold_timer = 0;
        }
    }

    switch (keycode) {

        case KC_1 ... KC_0:  // Numbers 1-0 
            if ((get_mods() & (MOD_BIT(KC_LALT) | MOD_BIT(KC_LCTL))) == (MOD_BIT(KC_LALT) | MOD_BIT(KC_LCTL))) {
                tap_code16(KC_F1 + (keycode - KC_1));    // Convert number to F-key
                return false;
            } else if (get_mods() == MOD_BIT(KC_LALT) && keycode == KC_4) {
                        if (record->event.pressed) {
                            tap_code16(LALT(KC_F4));
                        }
                        return false;
                    }
            return true;

        case TG_DBG:
            if (record->event.pressed) {
                TOGGLE_DEBUG = !TOGGLE_DEBUG;
                if (!TOGGLE_DEBUG) {
                    debug_enable = false;
                    debug_matrix = false;
                    debug_keyboard = false;
                    uprintf("Toggled Debug OFF\n");
                } else {
                    debug_enable = true;
                    debug_matrix = true;
                    debug_keyboard = true;
                    uprintf("Toggled Debug ON\n");
                }
            }
            return false;

        case TG_NUL:
            if (record->event.pressed) {
                TOGGLE_NULL = !TOGGLE_NULL;
                uprintf("TOGGLE_NULL is now: %d\n", TOGGLE_NULL);
                // reset state
                if (!TOGGLE_NULL) {
                    if (a_scrip) {
                        a_scrip = false;
                        unregister_code(KC_A);
                    }
                    if (d_scrip) {
                        d_scrip = false;
                        unregister_code(KC_D);
                    }
                    uprintf("Toggled null movement OFF\n");
                    //rgb_matrix_sethsv_noeeprom(HSV_WHITE);
                } else {
                    uprintf("Toggled null movement ON\n");
                    //rgb_matrix_sethsv_noeeprom(HSV_CHARTREUSE);
                }
            }
            return false;

        case KC_A:
            if (TOGGLE_NULL) {
                if (record->event.pressed) {
                    a_held = true;
                    if (d_scrip) {
                        d_scrip = false;
                        unregister_code(KC_D);
                    }
                    if (!a_scrip) {
                        a_scrip = true;
                        register_code(KC_A);
                    }
                } else {
                    a_held = false;
                    if (a_scrip) {
                        a_scrip = false;
                        unregister_code(KC_A);
                    }
                    if (d_held && !d_scrip) {
                        d_scrip = true;
                        register_code(KC_D);
                    }
                }
                return false;
            }
            return true;

        case KC_D:
            if (TOGGLE_NULL) {
                if (record->event.pressed) {
                    d_held = true;
                    if (a_scrip) {
                        a_scrip = false;
                        unregister_code(KC_A);
                    }
                    if (!d_scrip) {
                        d_scrip = true;
                        register_code(KC_D);
                    }
                } else {
                    d_held = false;
                    if (d_scrip) {
                        d_scrip = false;
                        unregister_code(KC_D);
                    }
                    if (a_held && !a_scrip) {
                        a_scrip = true;
                        register_code(KC_A);
                    }
                }
                return false;
            }
            return true;
        // handle other keycodes...
        default:
            return true;
    }
    return true;
}

void matrix_scan_user(void) {
    if (is_repeating && last_keycode != 0) {
        if ((get_mods() & (MOD_BIT(KC_LCTL) | MOD_BIT(KC_RCTL))) || last_keycode == KC_AUDIO_VOL_DOWN || last_keycode == KC_AUDIO_VOL_UP) {
            uint16_t hold_time = timer_elapsed(hold_timer);
            uint16_t delay = 200 - ((hold_time > 2000 ? 150 : (150 * hold_time) / 2000));
            if (timer_elapsed(repeat_timer) > delay) {
                tap_code(last_keycode);
                repeat_timer = timer_read();
            }
        } else {
            is_repeating = false;
            last_keycode = 0;
            hold_timer = 0;
        }
    }
}