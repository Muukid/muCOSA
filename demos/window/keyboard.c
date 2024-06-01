/*
============================================================
                        DEMO INFO

DEMO NAME:          keyboard.c
DEMO WRITTEN BY:    Muukid
CREATION DATE:      2024-04-23
LAST UPDATED:       2024-06-01

============================================================
                        DEMO PURPOSE

This demo shows how the keyboard key and keyboard state can
be retrieved and used.

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
#include "muCOSA.h"

#include <stdio.h> // For printf

/* Variables */

	// Global context
	muCOSAContext muCOSA;

	// The window system (like Win32, X11, etc.)
	muWindowSystem window_system = MU_WINDOW_SYSTEM_AUTO;

	// The graphics API
	muGraphicsAPI graphics_api = MU_NO_GRAPHICS_API;

int main(void) {
/* Initiation */

	// Initiate muCOSA

	muCOSA_context_create(&muCOSA, window_system, MU_TRUE);

	// Print currently running window system

	printf("Running window system \"%s\"\n", mu_window_system_get_nice_name(muCOSA_context_get_window_system(&muCOSA)));

	// Create window

	muWindow window = mu_window_create(graphics_api, 0, "Empty Window", 800, 600, mu_window_default_create_info());

	// Print current graphics API

	printf("Running graphics API \"%s\"\n", mu_graphics_api_get_nice_name(graphics_api));

/* Main loop */

	// Set up a loop that continues as long as the window isn't closed

	while (!mu_window_get_closed(window)) {

		// Store the caps lock state

		muState caps_state = mu_window_get_keyboard_state_state(window, MU_KEYBOARD_STATE_CAPS_LOCK);

		// Only do the following code every .2 seconds:

		if (mu_time_get() > .2f) {

			// Loop through each keyboard key from A to Z and print it if its held down, capital if
			// caps lock is on and not capital if caps lock is off.

			for (muKeyboardKey key = MU_KEYBOARD_KEY_A; key <= MU_KEYBOARD_KEY_Z; key++) {
				if (mu_window_get_keyboard_key_state(window, key)) {
					if (caps_state == MU_OFF) {
						printf("%c\n", (char)((key-MU_KEYBOARD_KEY_A)+'a'));
					} else {
						printf("%c\n", (char)((key-MU_KEYBOARD_KEY_A)+'A'));
					}
				}
			}

			// Reset time

			mu_time_set(0.f);
		}

		// Swap buffers (which renders the screen)

		mu_window_swap_buffers(window);

		// Update window (which refreshes input and such)

		mu_window_update(window);
	}

/* Termination */

	// Destroy window

	window = mu_window_destroy(window);

	// Terminate muCOSA
	
	muCOSA_context_destroy(&muCOSA);

	// Print possible error

	if (muCOSA.result != MUCOSA_SUCCESS) {
		printf("Something went wrong during that; result: %s\n", muCOSA_result_get_name(muCOSA.result));
	} else {
		printf("Successful\n");
	}

	// Program should print the A-Z characters every .2 seconds if they're held down, and be
	// capitalized or uncapitalized based on the caps lock state.
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

