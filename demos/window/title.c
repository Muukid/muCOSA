/*
============================================================
                        DEMO INFO

DEMO NAME:          title.c
DEMO WRITTEN BY:    Muukid
CREATION DATE:      2024-04-26
LAST UPDATED:       2024-06-28

============================================================
                        DEMO PURPOSE

This demo shows how changing the title of a window works.

Program should make a window that takes in text input and
changes the window title based on what the user typed out,
clearing the window title if they press backspace.

============================================================
                        LICENSE INFO

All code is licensed under MIT License or public domain, 
whichever you prefer.
More explicit license information at the end of file.

============================================================
*/

// Include muCOSA

#define MUCOSA_NAMES // For name functions
#define MUS_NAMES // For muString name functions
#define MUCOSA_IMPLEMENTATION
#include "muCOSA.h"

// Other dependencies

#include <stdio.h> // For printf
#include <string.h> // For strlen and mem functions
#include <stdlib.h> // For allocation / free functions

/* Variables */

	// Global context
	muCOSAContext muCOSA;

	// The window system (like Win32, X11, etc.)
	muWindowSystem window_system = MU_WINDOW_SYSTEM_AUTO;

	// The graphics API
	muGraphicsAPI graphics_api = MU_NO_GRAPHICS_API;

	// String used to hold the title
	muByte* title = 0;
	size_m title_size = 0;

/* UTF-8 logic functions */

		// Function for getting the size of a codepoint in UTF-8
	size_m get_utf8_codepoint_size(muByte byte) {
		// 00000000 <= n <= 01111111
		if (/*byte >= 0 && */byte <= 127) {
			return 1;
		// 11000000 <= n < 11100000
		} else if (byte >= 192 && byte < 224) {
			return 2;
		// 11100000 <= n < 11110000
		} else if (byte >= 224 && byte < 240) {
			return 3;
		// 11110000 <= n <= 11110111
		} else if (byte >= 240 && byte <= 247) {
			return 4;
		} else {
			// Value is 10xxxxxx or 11111xxx; invalid.
			return 0;
		}
	}

	// Function for getting the codepoint of a UTF-8 character and get its size
	size_m get_utf8_codepoint(muByte* data) {
		size_m size = get_utf8_codepoint_size(data[0]);
		switch (size) {
			default: return 0; break;
			case 1: return data[0]; break;

			case 2: {
				return
				// 110xxxxx
				(size_m)(data[0] & 31) << 6 |
				// 10xxxxxx
				(size_m)(data[1] & 63);
			} break;

			case 3: {
				return
				// 1110xxxx
				(size_m)(data[0] & 15) << 12 |
				// 10xxxxxx
				(size_m)(data[1] & 63) << 6 |
				// 10xxxxxx
				(size_m)(data[2] & 63);
			} break;

			case 4: {
				return
				// 1110xxxx
				(size_m)(data[0] & 7) << 18 |
				// 10xxxxxx
				(size_m)(data[1] & 63) << 12 |
				// 10xxxxxx
				(size_m)(data[2] & 63) << 6 |
				// 10xxxxxx
				(size_m)(data[3] & 63);
			} break;
		}
	}

/* Callback */

	// Text input callback to add to window title
	void text_input_callback(muWindow window, const char* input) {
		size_m input_size = strlen(input);

		// Make sure codepoint is good

		size_m code_point = get_utf8_codepoint((muByte*)input);

		if (code_point < 32) {
			// Avoid printing message for backspace since we are using it
			if (code_point != (size_m)'\b') {
				printf("Control character, not using it\n");
			}
			return;
		}

		// Insert input into our title string and update window title

		title_size += get_utf8_codepoint_size(((muByte*)input)[0]);
		title = (muByte*)realloc(title, title_size);
		memcpy(&title[title_size-input_size-1], input, input_size);
		title[title_size-1] = 0;

		mu_window_set_title(window, (const char*)title);
	}

	// Keyboard key callback to clear window title
	void keyboard_key_callback(muWindow window, muKeyboardKey keyboard_key, muButtonState state) {
		// On backspace:
		if (keyboard_key == MU_KEYBOARD_KEY_BACKSPACE && state == MU_BUTTON_STATE_HELD) {
			// Replace string with just a null terminator

			if (title_size != 1) {
				title = (muByte*)realloc(title, 1);
				title_size = 1;
			}
			title[0] = 0;

			// Update window title

			mu_window_set_title(window, (const char*)title);
		}
	}

int main(void) {
/* Initiation */

	// Initiate muCOSA

	muCOSA_context_create(&muCOSA, window_system, MU_TRUE);

	// Print currently running window system

	printf("Running window system \"%s\"\n", mu_window_system_get_nice_name(muCOSA_context_get_window_system(&muCOSA)));

	// Allocate window title to initially just be a null terminator

	title_size = 1;
	title = (muByte*)malloc(1);
	title[0] = 0;

	// Create window

	muWindow window = mu_window_create(graphics_api, 0, (const char*)title, 800, 600, mu_window_default_create_info());

	// Print current graphics API

	printf("Running graphics API \"%s\"\n", mu_graphics_api_get_nice_name(graphics_api));

	// Get text input

	mu_window_get_text_input_focus(window, 800/2, 600/2, text_input_callback);

	// Set keyboard key callback

	mu_window_set_keyboard_key_callback(window, keyboard_key_callback);

/* Main loop */

	// Set up a loop that continues as long as the window isn't closed

	while (!mu_window_get_closed(window)) {

		// Update text cursor position

		uint32_m width=0, height=0;
		mu_window_get_dimensions(window, &width, &height);

		mu_window_update_text_cursor(window, (int32_m)width/2, (int32_m)height/2);

		// Swap buffers (which renders the screen)

		mu_window_swap_buffers(window);

		// Update window (which refreshes input and such)

		mu_window_update(window);
	}

/* Termination */

	// Destroy window

	window = mu_window_destroy(window);

	// Free title

	free(title);

	// Terminate muCOSA
	
	muCOSA_context_destroy(&muCOSA);

	// Print possible error

	if (muCOSA.result != MUCOSA_SUCCESS) {
		printf("Something went wrong during that; result: %s\n", muCOSA_result_get_name(muCOSA.result));
	} else {
		printf("Successful\n");
	}

	return 0;
}

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2024 Hum
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/

