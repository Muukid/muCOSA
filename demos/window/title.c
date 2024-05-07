/*
============================================================
                        DEMO INFO

DEMO NAME:          title.c
DEMO WRITTEN BY:    Muukid
CREATION DATE:      2024-04-26
LAST UPDATED:       2024-05-04

============================================================
                        DEMO PURPOSE

This demo shows how changing the title of a window works.

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
#include <string.h> // For strlen

/* Variables */

	// Used to store the result of functions
	muCOSAResult result = MUCOSA_SUCCESS;
	// Macro which is used to print if the result is bad, meaning a function went wrong.
	#define scall(function_name) if (result != MUCOSA_SUCCESS) {printf("WARNING: '" #function_name "' returned %s\n", muCOSA_result_get_name(result));}

	// Used to store the result of muString functions
	muString mus_res = MUS_SUCCESS;
	// Macro which is used to print if the mus result is bad, meaning a function went wrong.
	#define mus_scall(function_name) if (mus_res != MUS_SUCCESS) {printf("WARNING: '" #function_name "' returned %s\n", mus_result_get_name(result));}

	// The window system (like Win32, X11, etc.)
	muWindowSystem window_system = MU_WINDOW_SYSTEM_AUTO;

	// The graphics API
	muGraphicsAPI graphics_api = MU_NO_GRAPHICS_API;

	// String used to hold the title
	muString title = MU_NONE;

/* Callback */

	// Text input callback to add to window title
	void text_input_callback(muWindow window, muByte* input) {
		size_m input_size = strlen((char*)input);

		// Make sure codepoint is good

		muCodePoint code_point = mu_UTF8_get_code_point(&mus_res, input, input_size);
		mus_scall(mu_UTF8_get_code_point)

		if (code_point < 32) {
			// Avoid printing message for backspace since we are using it
			if (code_point != (muCodePoint)'\b') {
				printf("Control character, not using it\n");
			}
			return;
		}

		// Insert input into our title string and update window title

		size_m title_data_size = 0;
		mu_string_get_raw_data(&mus_res, title, &title_data_size);
		mus_scall(mu_string_get_raw_data)

		mu_string_insert_raw_string(&mus_res, title, MU_UTF8, input, input_size, title_data_size-1);
		mus_scall(mu_string_insert_raw_string)

		mu_window_set_title(&result, window, mu_string_get_raw_data(0, title, 0));
		scall(mu_window_set_title)
	}

	// Keyboard key callback to clear window title
	void keyboard_key_callback(muWindow window, muKeyboardKey keyboard_key, muButtonState state) {
		if (keyboard_key == MU_KEYBOARD_KEY_BACKSPACE && state == MU_BUTTON_STATE_HELD) {
			// Destroy title string, replace with ""

			title = mu_string_destroy(&mus_res, title);
			mus_scall(mu_string_destroy)

			title = mu_string_create(&mus_res, MU_UTF8, (muByte*)"", 1);
			mus_scall(mu_string_create)

			// Update window title

			mu_window_set_title(&result, window, mu_string_get_raw_data(0, title, 0));
			scall(mu_window_set_title)
		}
	}

int main(void) {
/* Initiation */

	// Initiate muCOSA

	muCOSA_init(&result, window_system); scall(muCOSA_init)

	// Print currently running window system

	printf("Running window system \"%s\"\n", mu_window_system_get_nice_name(muCOSA_get_current_window_system(0)));

	// Create window

	muWindow window = mu_window_create(&result, graphics_api, 0, (muByte*)"", 800, 600, mu_window_default_create_info());
	scall(mu_window_create)

	// Print current graphics API

	printf("Running graphics API \"%s\"\n", mu_graphics_api_get_nice_name(graphics_api));

	// Initiate muString and then the title string

	mus_init(&mus_res); mus_scall(mus_init)

	title = mu_string_create(&mus_res, MU_UTF8, (muByte*)"", 1);
	mus_scall(mu_string_create)

	// Get text input

	mu_window_get_text_input_focus(&result, window, 800/2, 600/2, text_input_callback);
	scall(mu_window_get_text_input_focus)

	// Set keyboard key callback

	mu_window_set_keyboard_key_callback(&result, window, keyboard_key_callback);
	scall(mu_window_set_keyboard_key_callback)

/* Main loop */

	// Set up a loop that continues as long as the window isn't closed

	while (!mu_window_get_closed(&result, window)) {
		scall(mu_window_get_closed)

		// Update text cursor position

		uint32_m width=0, height=0;
		mu_window_get_dimensions(0, window, &width, &height);
		scall(mu_window_get_dimensions)

		mu_window_update_text_cursor(&result, window, (int32_m)width/2, (int32_m)height/2);
		scall(mu_window_update_text_cursor)

		// Swap buffers (which renders the screen)

		mu_window_swap_buffers(&result, window);
		scall(mu_window_swap_buffers)

		// Update window (which refreshes input and such)

		mu_window_update(&result, window);
		scall(mu_window_update)
	}

/* Termination */

	// Destroy window (optional)

	window = mu_window_destroy(&result, window);
	scall(mu_window_destroy)

	// Terminate muString

	mus_term(&mus_res); mus_scall(mus_term)

	// Terminate muCOSA
	
	muCOSA_term(&result); scall(muCOSA_term)

	// Program should make a window that takes in text input and changes the window title based on
	// what the user typed out, clearing the window title if they press backspace.
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

