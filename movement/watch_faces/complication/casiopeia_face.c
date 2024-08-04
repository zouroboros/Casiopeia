/*
 * MIT License
 *
 * Copyright (c) 2024 <#author_name#>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "casiopeia_face.h"

char* messages[] = {
  "KOMM MIT",
  "KEINE ANGST",
  "STILL",
  "RUECKWAERTS GEHEN",
  "WIR SIND DA",
  "NIE",
  "GENAU",
  "SIE WIRD",
  "DAS, WAS ICH WEISS",
  "KASSIOPEIA",
  "WER DEN SONST?",
  "ICH FRUEHSTUECKE",
  "DEIN WUNSCH",
  "MEIN WUNSCH",
  "BITTE",
  "KEINER MEHR DA",
  "NIE MEHR",
  "ALLE FORT",
  "HAST LANG GESCHLAFEN",
  "JAHR UND TAG",
  "NIEMAND MEHR DA",
  "VERGANGEN",
  "ICH BIN BEI DIR",
  "?",
  "WIRSTS GLEICH WISSEN",
  "LACKAFFE",
  "IST ES ABER",
  "WARTE",
  "ER KOMMT GLEICH",
  "LEBEWOHL",
  "ICH GEH DICH SUCHEN",
  "DA BIN ICH WIEDER",
  "WAS SOLL DER UNFUG",
  "FREUST DICH WOHL GAR NICHT",
  "MUSS DOCH SEHR BITTEN",
  "FREILICH",
  "WUSSTE ES VORHER",
  "WIR GEHEN ZU HORA",
  "WIR BEGEGNEN KEINEM",
  "ES IST GANZ NAH",
  "LEIDER NEIN",
  "DER WEG IST IN MIR",
  "JE LANGSAMER, DESTO SCHNELLER",
  "WEISS NUR VORHER",
  "DENKE NICHT NACH",
  "FRUESTUECKEN",
  "ICH GEH MIT DIR",
  "NATUERLICH",
  "JEMAND MUSS DOCH AUF SIE AUFPASSEN",
  "ABER DEINE ZEIT VERLIERST DU",
  "WEITER",
  "DU FINDEST SIE, LAUF WEITER",
  "DU MACHST DIE TUER ZU",
  "MIT DER BLUME BERUEHREN",
  "DU WIRST ES TUN",
  "DU MACHST DIE TUER AUF",
  "FLIEG HEIM, KLEINE MOMO, FLIEG HEIM",
  "SPAETER",
  "UND WIE",
  "DANKE",
  "ENDE"
};

uint8_t number_of_messages = 60;

static void rotate_chars(char* message, uint8_t window_length, uint8_t offset, char* output) {
    uint8_t message_length = strlen(message);

    uint8_t first_part_length = message_length - offset;

    strncpy(output, message + offset, first_part_length);

    for (uint8_t i = first_part_length; i < window_length; i++) {
        output[i] = ' ';
    }

    output[window_length] = '\0';
}

static void show_message(casiopeia_state_t *state) {
    char relevantChars[9];
    char* current_message = messages[state->current_message_index];

    rotate_chars(current_message, 8, state->current_message_offset, relevantChars);
    
    watch_display_string(relevantChars, 4);
}

static bool update_offset(casiopeia_state_t *state) {
    watch_date_time current_date_time = watch_rtc_get_date_time();

    if (current_date_time.unit.second % 2 == 0) {
        uint8_t current_message_length = strlen(messages[state->current_message_index]);
        state->current_message_offset = (state->current_message_offset + 1) % current_message_length;
        
        return true;
    }

    return false;
}

static bool update_message_index(casiopeia_state_t *state) {
    watch_date_time current_date_time = watch_rtc_get_date_time();

    if (current_date_time.unit.minute != state->last_index_update_minute && current_date_time.unit.minute % 15 == 0) {
        state->last_index_update_minute = current_date_time.unit.minute;
        state->current_message_offset = 0;
        // Emulator: use rand. Hardware: use arc4random.
        #if __EMSCRIPTEN__
        state->current_message_index = rand() % (number_of_messages + 1);
        #else
        state->current_message_index = arc4random_uniform(number_of_messages + 1);
        #endif
        
        return true;
    }

    return false;
}

void casiopeia_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr) {
    (void) settings;
    (void) watch_face_index;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(casiopeia_state_t));
        casiopeia_state_t *state = (casiopeia_state_t *) memset(*context_ptr, 0, sizeof(casiopeia_state_t));
        // Do any one-time tasks in here; the inside of this conditional happens only at boot.
        state->current_message_index = 0;
        state->current_message_offset = 0;
        state->last_index_update_minute = 0;
    }
    // Do any pin or peripheral setup here; this will be called whenever the watch wakes from deep sleep.
}

void casiopeia_face_activate(movement_settings_t *settings, void *context) {
    (void) settings;
    casiopeia_state_t *state = (casiopeia_state_t *)context;
    // Handle any tasks related to your watch face coming on screen.
    update_message_index(state);
}

bool casiopeia_face_loop(movement_event_t event, movement_settings_t *settings, void *context) {
    casiopeia_state_t *state = (casiopeia_state_t *)context;

    bool update_display = false;

    switch (event.event_type) {
        case EVENT_ACTIVATE:
            // Show your initial UI here.
            update_display = true;
            break;
        case EVENT_TICK:
            // If needed, update your display here.
            update_display = update_display || update_offset(state);
            update_display = update_display || update_message_index(state);
            break;
        case EVENT_LIGHT_BUTTON_UP:
            // You can use the Light button for your own purposes. Note that by default, Movement will also
            // illuminate the LED in response to EVENT_LIGHT_BUTTON_DOWN; to suppress that behavior, add an
            // empty case for EVENT_LIGHT_BUTTON_DOWN.
            break;
        case EVENT_ALARM_BUTTON_UP:
            // Just in case you have need for another button.
            break;
        case EVENT_TIMEOUT:
            // Your watch face will receive this event after a period of inactivity. If it makes sense to resign,
            // you may uncomment this line to move back to the first watch face in the list:
            // movement_move_to_face(0);
            break;
        case EVENT_LOW_ENERGY_UPDATE:
            // If you did not resign in EVENT_TIMEOUT, you can use this event to update the display once a minute.
            // Avoid displaying fast-updating values like seconds, since the display won't update again for 60 seconds.
            // You should also consider starting the tick animation, to show the wearer that this is sleep mode:
            // watch_start_tick_animation(500);
            
            if (state->current_message_offset != 0) {
                state->current_message_offset = 0;
                update_display = true;
            }

            update_display = update_display || update_message_index(state);
            break;
        default:
            // Movement's default loop handler will step in for any cases you don't handle above:
            // * EVENT_LIGHT_BUTTON_DOWN lights the LED
            // * EVENT_MODE_BUTTON_UP moves to the next watch face in the list
            // * EVENT_MODE_LONG_PRESS returns to the first watch face (or skips to the secondary watch face, if configured)
            // You can override any of these behaviors by adding a case for these events to this switch statement.
            return movement_default_loop_handler(event, settings);
    }

    if (update_display) {
        show_message(state);
    }

    // return true if the watch can enter standby mode. Generally speaking, you should always return true.
    // Exceptions:
    //  * If you are displaying a color using the low-level watch_set_led_color function, you should return false.
    //  * If you are sounding the buzzer using the low-level watch_set_buzzer_on function, you should return false.
    // Note that if you are driving the LED or buzzer using Movement functions like movement_illuminate_led or
    // movement_play_alarm, you can still return true. This guidance only applies to the low-level watch_ functions.
    return true;
}

void casiopeia_face_resign(movement_settings_t *settings, void *context) {
    (void) settings;
    (void) context;

    // handle any cleanup before your watch face goes off-screen.
}

