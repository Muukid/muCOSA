/*
============================================================
                        DEMO INFO

DEMO NAME:          text_input.c
DEMO WRITTEN BY:    Muukid
CREATION DATE:      2024-04-25
LAST UPDATED:       2024-04-26

============================================================
                        DEMO PURPOSE

This demo shows the basics behind using text input to get
input from the user.

============================================================

============================================================
                        LICENSE INFO

All code is licensed under MIT License or public domain, 
whichever you prefer.
More explicit license information at the end of file.

============================================================
*/

// Include muCOSA

#include <stdio.h> // For printf

#define MUCOSA_NAMES // For name functions
#define MUCOSA_IMPLEMENTATION
#include <muCOSA.h>

#include <locale.h> // For setlocale
#include <string.h> // For strlen

/* Variables */

	// Used to store the result of functions
	muCOSAResult result = MUCOSA_SUCCESS;

	// The window system (like Win32, X11, etc.)
	muWindowSystem window_system = MU_WINDOW_SYSTEM_AUTO;

	// The graphics API
	muGraphicsAPI graphics_api = MU_NO_GRAPHICS_API;

/* Callback */

	// Text input callback to print text input
	void text_input_callback(muWindow window, muByte* input) {
		if (window) {} // (to avoid unused parameter warnings)

		size_m input_size = strlen((char*)input);

		// Print input
		printf("%s ", (char*)input);

		// Print codepoint(s)

		// (possible null)
		if (input[0] == 0 || input_size == 0) {
			printf("[0]\n");
			return;
		}

		printf("[ ");

		size_m offset = 0;
		while (input[offset] != 0 && offset < input_size) {
			muCodePoint code_point = mu_UTF8_get_code_point(0, &input[offset], input_size-offset);
			printf("%lu ", (long unsigned)code_point);

			size_m code_point_size = mu_UTF8_get_code_point_size(0, code_point);
			if (code_point_size == 0) {
				printf("]\n");
				return;
			}
			offset += code_point_size;
		}

		printf("]\n");
	}

	// Key input to toggle text input
	void keyboard_key_callback(muWindow window, muKeyboardKey keyboard_key, muButtonState state) {
		muCOSAResult res = MUCOSA_SUCCESS;

		if (state != MU_BUTTON_STATE_HELD) {
			return;
		}

		// If F1 is pressed:
		if (keyboard_key == MU_KEYBOARD_KEY_F1) {
			printf("Taking input...\n");
			// Get text input focus, putting the text cursor in the middle of the window
			uint32_m width=0, height=0;
			mu_window_get_dimensions(0, window, &width, &height);

			mu_window_get_text_input_focus(&res, window, (int32_m)width/2, (int32_m)height/2, text_input_callback);
			if (result != MUCOSA_SUCCESS) printf("WARNING: mu_window_get_text_input_focus returned %s\n", muCOSA_result_get_name(result));
		}

		// If F2 is pressed:
		else if (keyboard_key == MU_KEYBOARD_KEY_F2) {
			printf("Not taking input...\n");
			// Let go of input text focus
			mu_window_let_text_input_focus(&res, window);
			if (result != MUCOSA_SUCCESS) printf("WARNING: mu_window_let_text_input_focus returned %s\n", muCOSA_result_get_name(result));
		}
	}

int main() {
/* Initiation */

	// Set locale to be able to print/use UTF-8
	setlocale(LC_ALL, "");

	// Initiate muCOSA

	muCOSA_init(&result, window_system);
	if (result != MUCOSA_SUCCESS) printf("WARNING: muCOSA_init returned %s\n", muCOSA_result_get_name(result));

	// Print currently running window system

	printf("Running window system \"%s\"\n", mu_window_system_get_nice_name(muCOSA_get_current_window_system(0)));

	// Create window

	muWindow window = mu_window_create(&result, graphics_api, 0, (muByte*)"Empty Window", 800, 600, mu_window_default_create_info());
	if (result != MUCOSA_SUCCESS) printf("WARNING: mu_window_create returned %s\n", muCOSA_result_get_name(result));

	// Print current graphics API

	printf("Running graphics API \"%s\"\n", mu_graphics_api_get_nice_name(graphics_api));

	// Get text input

	mu_window_get_text_input_focus(&result, window, 800/2, 600/2, text_input_callback);
	if (result != MUCOSA_SUCCESS) printf("WARNING: mu_window_get_text_input_focus returned %s\n", muCOSA_result_get_name(result));
	printf("Taking input...\n");

	// Set keyboard key callback

	mu_window_set_keyboard_key_callback(&result, window, keyboard_key_callback);
	if (result != MUCOSA_SUCCESS) printf("WARNING: mu_window_set_keyboard_key_callback returned %s\n", muCOSA_result_get_name(result));

/* Main loop */

	// Set up a loop that continues as long as the window isn't closed

	while (!mu_window_get_closed(&result, window)) {
		if (result != MUCOSA_SUCCESS) printf("WARNING: mu_window_get_closed returned %s\n", muCOSA_result_get_name(result));

		// Update window (which refreshes input and such)

		mu_window_update(&result, window);
		if (result != MUCOSA_SUCCESS) printf("WARNING: mu_window_update returned %s\n", muCOSA_result_get_name(result));

		// Update text cursor position

		uint32_m width=0, height=0;
		mu_window_get_dimensions(0, window, &width, &height);

		mu_window_update_text_cursor(&result, window, (int32_m)width/2, (int32_m)height/2);
		if (result != MUCOSA_SUCCESS) printf("WARNING: mu_window_update_text_cursor returned %s\n", muCOSA_result_get_name(result));

		// Swap buffers (which renders the screen)

		mu_window_swap_buffers(&result, window);
		if (result != MUCOSA_SUCCESS) printf("WARNING: mu_window_swap_buffers returned %s\n", muCOSA_result_get_name(result));
	}

/* Termination */

	// Destroy window (optional)

	window = mu_window_destroy(&result, window);
	if (result != MUCOSA_SUCCESS) printf("WARNING: mu_window_destroy returned %s\n", muCOSA_result_get_name(result));

	// Terminate muCOSA
	
	muCOSA_term(&result);
	if (result != MUCOSA_SUCCESS) printf("WARNING: muCOSA_term returned %s\n", muCOSA_result_get_name(result));

	// Program should make a window that takes in text input, toggleable by F1 (to turn on
	// (default)) and F2 (to turn off).
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

