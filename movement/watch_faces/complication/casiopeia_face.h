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

#ifndef CASIOPEIA_FACE_H_
#define CASIOPEIA_FACE_H_

#include <stdlib.h>
#include <string.h>
#include "movement.h"
#include "watch.h"

/*
 * A DESCRIPTION OF YOUR WATCH FACE
 *
 * and a description of how use it
 *
 */

typedef struct {
    // Anything you need to keep track of, put it here!
    uint8_t current_message_index;
    uint8_t current_message_offset;
    uint8_t last_index_update_minute;
} casiopeia_state_t;

void casiopeia_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr);
void casiopeia_face_activate(movement_settings_t *settings, void *context);
bool casiopeia_face_loop(movement_event_t event, movement_settings_t *settings, void *context);
void casiopeia_face_resign(movement_settings_t *settings, void *context);

#define casiopeia_face ((const watch_face_t){ \
    casiopeia_face_setup, \
    casiopeia_face_activate, \
    casiopeia_face_loop, \
    casiopeia_face_resign, \
    NULL, \
})

#endif // CASIOPEIA_FACE_H_

