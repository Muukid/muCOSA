/*
============================================================
                        DEMO INFO

DEMO NAME:          callbacks.c
DEMO WRITTEN BY:    Muukid
CREATION DATE:      2024-04-21
LAST UPDATED:       2024-04-21

============================================================
                        DEMO PURPOSE

This demo shows how each window callback generally works.

============================================================

============================================================
                        LICENSE INFO

All code is licensed under MIT License or public domain, 
whichever you prefer.
More explicit license information at the end of file.

============================================================
*/

// Include muCOSA

#define MUCOSA_NAMES // For name functions
#define MUCOSA_IMPLEMENTATION
#include <muCOSA.h>

#include <stdio.h> // For printf
#include <inttypes.h> // For print type names

/* Variables */

	// Used to store the result of functions
	muCOSAResult result = MUCOSA_SUCCESS;

	// The window system (like Win32, X11, etc.)
	muWindowSystem window_system = MU_WINDOW_SYSTEM_AUTO;

	// The graphics API
	muGraphicsAPI graphics_api = MU_NO_GRAPHICS_API;

/* Callbacks */

	// Dimensions callback; gets called whenever the window's dimensions are changed, such as when
	// the user resizes the window.
	void dimensions_callback(muWindow window, uint32_m width, uint32_m height) {
		printf("[Window %zu] ", window);
		printf("Dimensions: %" PRIu32 ", %" PRIu32 "\n", width, height);
	}

	// Position callback; gets called whenever the windows' position is changed, such as when the
	// user drags the window.
	void position_callback(muWindow window, int32_m x, int32_m y) {
		printf("[Window %zu] ", window);
		printf("Position: %" PRId32 ", %" PRId32 "\n", x, y);
	}

	// Focus callback; gets called whenever the window gets focused and unfocused.
	void focus_callback(muWindow window, muBool focused) {
		printf("[Window %zu] ", window);
		if (focused) {
			printf("focused\n");
		} else {
			printf("unfocused\n");
		}
	}

	// Maximize callback; gets called whenever a window gets maximized.
	void maximize_callback(muWindow window, muBool maximized) {
		printf("[Window %zu] ", window);
		if (maximized) {
			printf("maximized\n");
		} else {
			printf("unmaximized\n");
		}
	}

	// Minimize callback; gets called whenever a window gets minimized.
	void minimize_callback(muWindow window, muBool minimized) {
		printf("[Window %zu] ", window);
		if (minimized) {
			printf("minimized\n");
		} else {
			printf("unminimized\n");
		}
	}

	// Keyboard key callback; gets called whenever a keyboard key is pressed/released/held.
	void keyboard_key_callback(muWindow window, muKeyboardKey key, muButtonState state) {
		printf("[Window %zu] ", window);
		printf("Keyboard key \"%s\" ", mu_keyboard_key_get_nice_name(key));
		if (state == MU_BUTTON_STATE_HELD) {
			printf("hold\n");
		} else {
			printf("release\n");
		}
	}

	// Keyboard state callback; gets called whenever a keyboard state changes (such as caps locks)
	void keyboard_state_callback(muWindow window, muKeyboardState keyboard_state, muState state) {
		printf("[Window %zu] ", window);
		printf("Keyboard state \"%s\" ", mu_keyboard_state_get_nice_name(keyboard_state));
		if (state == MU_ON) {
			printf("on\n");
		} else {
			printf("off\n");
		}
	}

	// Cursor position callback; gets called whenever the cursor position changes
	void cursor_position_callback(muWindow window, int32_m x, int32_m y) {
		printf("[Window %zu] ", window);
		printf("Cursor position: %" PRId32 ", %" PRId32 "\n", x, y);
	}

	// Mouse button callback; gets called whenever a mouse button gets pressed or released.
	void mouse_button_callback(muWindow window, muMouseButton mouse_button, muButtonState state) {
		printf("[Window %zu] ", window);
		printf("Mouse button \"%s\" ", mu_mouse_button_get_nice_name(mouse_button));
		if (state == MU_BUTTON_STATE_HELD) {
			printf("held\n");
		} else {
			printf("released\n");
		}
	}

	// Scroll callback; gets called whenever the mouse/touchpad gets scrolled up or down an amount.
	void scroll_callback(muWindow window, int32_m scroll_level_add) {
		printf("[Window %zu] ", window);
		printf("Scroll: %" PRId32 "\n", scroll_level_add);
	}

int main() {
/* Initiation */

	// Initiate muCOSA

	muCOSA_init(&result, window_system);

	if (result != MUCOSA_SUCCESS)
		printf("WARNING: muCOSA_init returned %s\n", muCOSA_result_get_name(result));

	// Print currently running window system

	printf("Running window system \"%s\"\n", mu_window_system_get_nice_name(muCOSA_get_current_window_system(0)));

	// Create window info for callbacks

	muWindowCreateInfo create_info = mu_window_default_create_info();
	create_info.dimensions_callback = dimensions_callback;
	create_info.position_callback = position_callback;
	create_info.focus_callback = focus_callback;
	create_info.maximize_callback = maximize_callback;
	create_info.minimize_callback = minimize_callback;
	create_info.keyboard_key_callback = keyboard_key_callback;
	create_info.keyboard_state_callback = keyboard_state_callback;
	create_info.cursor_position_callback = cursor_position_callback;
	create_info.mouse_button_callback = mouse_button_callback;
	create_info.scroll_callback = scroll_callback;

	// Create window

	muWindow window = mu_window_create(&result, graphics_api, 0, (muByte*)"Empty Window", 800, 600, create_info);

	if (result != MUCOSA_SUCCESS)
		printf("WARNING: mu_window_create returned %s\n", muCOSA_result_get_name(result));

	// Print current graphics API

	printf("Running graphics API \"%s\"\n", mu_graphics_api_get_nice_name(graphics_api));

/* Main loop */

	// Set up a loop that continues as long as the window isn't closed

	while (!mu_window_get_closed(&result, window)) {
		if (result != MUCOSA_SUCCESS)
			printf("WARNING: mu_window_get_closed returned %s\n", muCOSA_result_get_name(result));

		// Update window (which refreshes input and such)

		mu_window_update(&result, window);
		if (result != MUCOSA_SUCCESS)
			printf("WARNING: mu_window_update returned %s\n", muCOSA_result_get_name(result));

		// This is where we would do our frame-by-frame logic.

		// Swap buffers (which renders the screen)

		mu_window_swap_buffers(&result, window);
		
		if (result != MUCOSA_SUCCESS)
			printf("WARNING: mu_window_swap_buffers returned %s\n", muCOSA_result_get_name(result));
	}

/* Termination */

	// Destroy window (optional)

	window = mu_window_destroy(&result, window);

	if (result != MUCOSA_SUCCESS)
		printf("WARNING: mu_window_destroy returned %s\n", muCOSA_result_get_name(result));

	// Terminate muCOSA
	
	muCOSA_term(&result);

	if (result != MUCOSA_SUCCESS)
		printf("WARNING: muCOSA_term returned %s\n", muCOSA_result_get_name(result));

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

