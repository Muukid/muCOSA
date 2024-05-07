/*
============================================================
                        DEMO INFO

DEMO NAME:          create_info.c
DEMO WRITTEN BY:    Muukid
CREATION DATE:      2024-04-22
LAST UPDATED:       2024-05-03

============================================================
                        DEMO PURPOSE

This demo shows how the create info struct can be used to
customize how a window works.

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

	// Used to store the result of functions
	muCOSAResult result = MUCOSA_SUCCESS;
	// Macro which is used to print if the result is bad, meaning a function went wrong.
	#define scall(function_name) if (result != MUCOSA_SUCCESS) {printf("WARNING: '" #function_name "' returned %s\n", muCOSA_result_get_name(result));}

	// The window system (like Win32, X11, etc.)
	muWindowSystem window_system = MU_WINDOW_SYSTEM_AUTO;

	// The graphics API
	muGraphicsAPI graphics_api = MU_NO_GRAPHICS_API;

int main(void) {
/* Initiation */

	// Initiate muCOSA

	muCOSA_init(&result, window_system); scall(muCOSA_init)

	// Print currently running window system

	printf("Running window system \"%s\"\n", mu_window_system_get_nice_name(muCOSA_get_current_window_system(0)));

	// Create the window create info struct

	muWindowCreateInfo create_info = mu_window_default_create_info();

	// Whether or not the window is visible on creation
	create_info.visible = MU_TRUE;
	// Whether or not the window is resizable
	create_info.resizable = MU_TRUE;
	// The window's initial X and Y coordinates on screen
	create_info.x = 50;
	create_info.y = 100;
	// The window's min/max width/height
	create_info.min_width = 200;
	create_info.min_height = 300;
	create_info.max_width = 1000;
	create_info.max_height = 800;
	// The window's cursor style on creation
	create_info.cursor_style = MU_CURSOR_STYLE_HAND;
	// The rest of the members are just callbacks, which are covered in the callbacks.c demo.

	// Create window

	muWindow window = mu_window_create(&result, graphics_api, 0, (muByte*)"Empty Window", 800, 600, create_info);
	scall(mu_window_create)

	// Print current graphics API

	printf("Running graphics API \"%s\"\n", mu_graphics_api_get_nice_name(graphics_api));

/* Main loop */

	// Set up a loop that continues as long as the window isn't closed

	while (!mu_window_get_closed(&result, window)) {
		scall(mu_window_get_closed)

		// Swap buffers (which renders the screen)

		mu_window_swap_buffers(&result, window); scall(mu_window_swap_buffers)

		// Update window (which refreshes input and such)

		mu_window_update(&result, window); scall(mu_window_update)
	}

/* Termination */

	// Destroy window (optional)

	window = mu_window_destroy(&result, window); scall(mu_window_destroy)

	// Terminate muCOSA
	
	muCOSA_term(&result); scall(muCOSA_term)

	// Program should make a window whose properties are pretty strange, deemed by the initial
	// create_info variable.
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

