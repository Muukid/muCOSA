/*
============================================================
                        DEMO INFO

DEMO NAME:          get_set.c
DEMO WRITTEN BY:    Muukid
CREATION DATE:      2024-08-04
LAST UPDATED:       2024-08-04

============================================================
                        DEMO PURPOSE

This demo shows how a window's attributes can be get and set
via the muCOSA API.

Program should create a window that can be moved using the
WASD keys, and resized using the arrow keys. Any change in
the dimensions or position of the window trigger a print of
the new values.
The window should be moveable by holding down the left-click
mouse button on the window's surface, dragging it with the
mouse.
The cursor style can be changed via the J and L keys.

============================================================
                        LICENSE INFO

All code is licensed under MIT License or public domain, 
whichever you prefer.
More explicit license information at the end of file.

============================================================
*/

/* Inclusion */

	// Include muCOSA
	#define MUCOSA_NAMES // For name functions
	#define MUCOSA_IMPLEMENTATION // For source code
	#include "muCOSA.h"
	
	// Include stdio for print functions
	#include <stdio.h>
	
	// Include inttypes for printing certain types
	#include <inttypes.h>

/* Variables */
	
	// Global context
	muCOSAContext muCOSA;

	// The window system
	muWindowSystem window_system = MU_WINDOW_NULL; // (Auto)

	// Window handle
	muWindow win;

	// Window information
	muWindowInfo wininfo = {
		// Title
		(char*)"Window",
		// Resolution (width & height)
		800, 600,
		// Coordinates (x and y)
		50, 50
	};

	// Window keyboard map
	muBool* keyboard;

	// Window mouse map
	muBool* mouse;

	// Current cursor style
	muCursorStyle cursor_style = MU_CURSOR_ARROW;

int main(void)
{

/* Initiation */

	// Initiate muCOSA
	muCOSA_context_create(&muCOSA, window_system, MU_TRUE);

	// Print currently running window system
	printf("Running window system \"%s\"\n",
		mu_window_system_get_nice_name(muCOSA_context_get_window_system(&muCOSA))
	);

	// Create window
	win = mu_window_create(&wininfo);

	// Get keyboard map
	mu_window_get(win, MU_WINDOW_KEYBOARD_MAP, &keyboard);

	// Get mouse map
	mu_window_get(win, MU_WINDOW_MOUSE_MAP, &mouse);

/* Print explanation */

	printf("WASD to move window, arrow keys to resize\n");
	printf("Hold down left-click on the window's surface to drag it\n");
	printf("Use J/L keys to change cursor style\n");

/* Main loop */

	// Initialize position, dimension, cursor, and key trackers
	int32_m pos[2] = { wininfo.x, wininfo.y };
	uint32_m dim[2] = { wininfo.width, wininfo.height };
	int32_m cur[2];
	muBool prevcur = MU_FALSE; // (last state of left button being down)
	muBool last_L = MU_FALSE; // (last state of L key)
	muBool last_J = MU_FALSE; // (last state of J key)

	// Desired FPS
	double FPS = 60.0;
	// Inverse desired FPS (aka how much time each frame is)
	double invFPS = 1.0 / FPS;
	// Current time (used for deltatime)
	double current_time = mu_time_get();

	// Amount of pixels moving per frame
	uint32_m ppf = 2;

	// Loop while window isn't closed:
	while (!mu_window_get_closed(win))
	{
		// Get window position
		int32_m new_pos[2];
		mu_window_get(win, MU_WINDOW_POSITION, new_pos);
		// Get window dimensions
		uint32_m new_dim[2];
		mu_window_get(win, MU_WINDOW_DIMENSIONS, new_dim);

		// Move window based on WASD
		// - Up/Down
		new_pos[1] += ppf*keyboard[MU_KEYBOARD_S];
		new_pos[1] -= ppf*keyboard[MU_KEYBOARD_W];
		// - Left/Right
		new_pos[0] += ppf*keyboard[MU_KEYBOARD_D];
		new_pos[0] -= ppf*keyboard[MU_KEYBOARD_A];

		// Resize window based on arrow keys
		// - Width
		new_dim[0] += ppf*keyboard[MU_KEYBOARD_RIGHT];
		new_dim[0] -= ppf*keyboard[MU_KEYBOARD_LEFT];
		// - Height
		new_dim[1] += ppf*keyboard[MU_KEYBOARD_DOWN];
		new_dim[1] -= ppf*keyboard[MU_KEYBOARD_UP];

		// Handle mouse dragging
		if (mouse[MU_MOUSE_LEFT]) {
			// If this is the first frame, just get coordinates and leave
			if (!prevcur) {
				prevcur = MU_TRUE;
				mu_window_get(win, MU_WINDOW_CURSOR, cur);
				// Make coordinates non-relative to window for consistency
				cur[0] += new_pos[0];
				cur[1] += new_pos[1];
			}
			// If this isn't the first frame:
			else {
				// Get new cursor position
				int32_m new_cur[2];
				mu_window_get(win, MU_WINDOW_CURSOR, new_cur);
				// Make non-relative to window for consistency
				new_cur[0] += new_pos[0];
				new_cur[1] += new_pos[1];
				// Move window based on how much cursor moved
				new_pos[0] += new_cur[0]-cur[0];
				new_pos[1] += new_cur[1]-cur[1];
				// Set new cursor position
				cur[0] = new_cur[0];
				cur[1] = new_cur[1];
			}
		} else {
			prevcur = MU_FALSE;
		}

		// Set new position/dimensions
		mu_window_set(win, MU_WINDOW_POSITION, new_pos);
		mu_window_set(win, MU_WINDOW_DIMENSIONS, new_dim);

		// Print if position has changed
		if (new_pos[0] != pos[0] || new_pos[1] != pos[1]) {
			printf("Position: (%" PRIi32 ", %" PRIi32 ")\n", new_pos[0], new_pos[1]);
			pos[0] = new_pos[0];
			pos[1] = new_pos[1];
		}

		// Print if dimensions have changed
		if (new_dim[0] != dim[0] || new_dim[1] != dim[1]) {
			printf("Dimensions: (%" PRIu32 ", %" PRIu32 ")\n", new_dim[0], new_dim[1]);
			dim[0] = new_dim[0];
			dim[1] = new_dim[1];
		}

		// Change cursor style if needed
		if (keyboard[MU_KEYBOARD_L] && !last_L) {
			// Handle wrapping
			if (cursor_style == MU_CURSOR_NO) {
				cursor_style = MU_CURSOR_IBEAM;
			} else {
				++cursor_style;
			}
			// Set cursor style
			mu_window_set(win, MU_WINDOW_CURSOR_STYLE, &cursor_style);
			// Print new cursor style (USE GET)
			// ...
		}
		if (keyboard[MU_KEYBOARD_J] && !last_J) {
			// Handle wrapping
			if (cursor_style == MU_CURSOR_IBEAM) {
				cursor_style = MU_CURSOR_NO;
			} else {
				--cursor_style;
			}
			// Set cursor style
			mu_window_set(win, MU_WINDOW_CURSOR_STYLE, &cursor_style);
			// Print new cursor style
			// ...
		}
		// Update last key trackers
		last_L = keyboard[MU_KEYBOARD_L];
		last_J = keyboard[MU_KEYBOARD_J];

		// Swap buffers (which renders the screen)
		mu_window_swap_buffers(win);

		// Update window (which refreshes input and such)
		mu_window_update(win);

		// Calculate deltatime
		double deltatime = mu_time_get() - current_time;
		current_time = mu_time_get();

		// Sleep for time needed to achieve (approx.) the desired FPS
		double sleep_time = invFPS-deltatime;
		if (sleep_time > 0.0) {
			mu_sleep(sleep_time);
		}
	}

/* Termination */

	// Destroy window (required)
	win = mu_window_destroy(win);

	// Terminate muCOSA (required)
	muCOSA_context_destroy(&muCOSA);

	// Print possible error
	if (muCOSA.result != MUCOSA_SUCCESS) {
		printf("Something went wrong during the program's lifespan; result: %s\n",
			muCOSA_result_get_name(muCOSA.result)
		);
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

