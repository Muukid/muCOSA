/*
============================================================
                        DEMO INFO

DEMO NAME:          window.c
DEMO WRITTEN BY:    Muukid
CREATION DATE:      2024-08-03
LAST UPDATED:       2024-08-07

============================================================
                        DEMO PURPOSE

This demo shows the basics behind creating a default, empty
window in muCOSA.

Program should pop up a window on screen named "Window". 
Note that since no graphics API is being used, there is no
easy way to clear the screen with a color, and in muCOSA, a
screen that doesn't get cleared has undefined contents,
meaning that it can look different depending on the OS.

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
		// Min/Max resolution (none)
		0, 0, 0, 0,
		// Coordinates (x and y)
		50, 50,
		// Pixel format (default)
		0
	};

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

/* Main loop */

	// Set up a loop that continues as long as the window isn't closed

	while (!mu_window_get_closed(win))
	{
		// ... This is where we would do our frame-by-frame logic

		// Update window (which refreshes input and such)
		mu_window_update(win);
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

