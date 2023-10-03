/*
muga.h - Muukid
Public domain single-file C library for creating contexts for cross-platform graphics APIs.
No warranty implied; use at your own risk.

Licensed under MIT License or public domain, whichever you prefer.
More explicit license information at the end of the file.
*/

#ifndef MUGA_H
#define MUGA_H

#ifdef __cplusplus
    extern "C" {
#endif

/* basics */

#define MUGA_VERSION_MAJOR 1
#define MUGA_VERSION_MINOR 0
#define MUGA_VERSION_PATCH 0

#ifndef MUGADEF
	#ifdef MUGA_STATIC
		#define MUGADEF static
	#else
		#define MUGADEF extern
	#endif
#endif

/* C standard library dependencies */

#ifndef size_m
	
	#include <stddef.h>
	#define size_m size_t

#endif

#ifndef wchar_m

    #include <wchar.h>
    #define wchar_m wchar_t

#endif

#if !defined(muga_malloc)  || \
    !defined(muga_free)    || \
    !defined(muga_realloc)

    #include <stdlib.h>

    #ifndef muga_malloc
    	#define muga_malloc malloc
    #endif

    #ifndef muga_free
    	#define muga_free free
    #endif

    #ifndef muga_realloc
    	#define muga_realloc realloc
    #endif

#endif

enum MUGA_BOOL { MUGA_FALSE, MUGA_TRUE };
typedef enum MUGA_BOOL MUGA_BOOL;

#define MUGA_RESULT  MUGA_BOOL
#define MUGA_FAILURE MUGA_FALSE
#define MUGA_SUCCESS MUGA_TRUE

#define MUGA_NULL     0
#define MUGA_NULL_PTR 0

/* general */

MUGADEF void muga_init(MUGA_RESULT* result);
MUGADEF void muga_term(MUGA_RESULT* result);

/* window */

typedef size_m muga_window;

MUGADEF muga_window muga_window_create(MUGA_RESULT* result, const wchar_m* name, unsigned int width, unsigned int height);
MUGADEF MUGA_BOOL muga_window_active(MUGA_RESULT* result, muga_window win);
MUGADEF void muga_window_update(MUGA_RESULT* result, muga_window win);
MUGADEF void muga_swap_buffers(MUGA_RESULT* result, muga_window win);

#ifdef __cplusplus
    }
#endif

#endif /* MUGA_H */



#ifdef MUGA_IMPLEMENTATION

#ifdef __cplusplus
    extern "C" {
#endif

/* pre-implementation setup */

#ifndef muga_print
    #ifdef MUGA_NO_PRINT
    	#define muga_print(msg, ...)
    #else
    	#include <stdio.h>
    	#define muga_print printf
    #endif
#endif


/* WINDOWS */

#if defined(_WIN32) || defined(WIN32)

#include <windows.h>

/* default window setup */

// basic window vars/structs

struct muga_windows_window {
	MUGA_BOOL active;
	MUGA_BOOL alive;
	// window class information
	WNDCLASSEXW window_class;
	// window handle
	HWND window_handle;
};
typedef struct muga_windows_window muga_windows_window;

// What a stupid name
muga_windows_window* muga_windows_windows      = MUGA_NULL_PTR;
// @TODO make the default windows size customizable
size_m               muga_windows_windows_length = 0;

// window funcs

// window proc
LRESULT CALLBACK muga_windows_default_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
}

// get the hinstance
// @TODO This doesn't account for DLLs, and will break
HINSTANCE muga_windows_get_hinstance() {
	return GetModuleHandle(NULL);
}

// reallocs muga_windows_windows if new length is needed
void muga_windows_windows_size_check(size_m new_length) {
	if (muga_windows_windows_length == new_length) {
		return;
	}
	muga_windows_windows_length = new_length;
	muga_windows_windows = 
	(muga_windows_window*) muga_realloc(
		muga_windows_windows, 
		muga_windows_windows_length * sizeof(muga_windows_window)
	);
}

// grabs new id for new window
size_m muga_windows_get_new_window_id() {
	// scan for inactive window struct
	for (size_m i = 0; i < muga_windows_windows_length; i++) {
		if (muga_windows_windows[i].active == MUGA_FALSE) {
			return i;
		}
	}
	// if we failed to find one,
	// we need to reallocate a new one
	muga_windows_windows_size_check(muga_windows_windows_length + 1);
	muga_windows_windows[muga_windows_windows_length-1].active = MUGA_FALSE;
	return muga_windows_windows_length-1;
}

// returns if id is valid or not
MUGA_BOOL muga_windows_is_id_valid(muga_window win) {
	if (win >= muga_windows_windows_length) {
		return MUGA_FALSE;
	}
	return muga_windows_windows[win].active;
}

/* initiation/termination */

// @TODO check if we have already initiated
MUGADEF void muga_init(MUGA_RESULT* result) {
	// initiate windows buffer
	muga_windows_windows_length = 1;
	muga_windows_windows = muga_malloc(sizeof(muga_windows_window) * muga_windows_windows_length);
	muga_windows_windows[0].active = MUGA_FALSE;

	// return
	if (result != MUGA_NULL) {
		*result = MUGA_SUCCESS;
	}
}

// @TODO check if we have already terminated
MUGADEF void muga_term(MUGA_RESULT* result) {
	// free windows buffer
	if (muga_windows_windows != MUGA_NULL_PTR) {
		muga_free(muga_windows_windows);
		muga_windows_windows = MUGA_NULL_PTR;
		muga_windows_windows_length = 0;
	}

	// return
	if (result != MUGA_NULL) {
		*result = MUGA_SUCCESS;
	}
}

/* basic window funcs */

MUGADEF muga_window muga_window_create(MUGA_RESULT* result, const wchar_m* name, unsigned int width, unsigned int height) {
	// initialize muga_windows_window struct

	muga_windows_window window_struct = {
		.active = MUGA_TRUE,
		.alive = MUGA_TRUE,
		// (WNDCLASSEXW)
		// @TODO expand this
		.window_class = {
			.cbSize = sizeof(WNDCLASSEXW),                     // size of struct
			.style =         0,                                // style (leaving 0 for now)
			.lpfnWndProc =   muga_windows_default_window_proc, // window process function
			.cbClsExtra =    0,                                // extra class allocation bytes
			.cbWndExtra =    0,                                // extra window instance allocation bytes
			.hInstance =     muga_windows_get_hinstance(),     // hInstance
			.hIcon =         LoadIcon(0, IDI_WINLOGO),         // window icon
			.hCursor =       LoadCursor(0, IDC_ARROW),         // window cursor
			.hbrBackground = 0,                                // background brush (0 is fine)
			.lpszMenuName =  name,                             // menu name
			.lpszClassName = name,                             // class name
			.hIconSm =       0                                 // small window icon
		}
	};
	RegisterClassExW(&window_struct.window_class);

	// create window

	window_struct.window_handle = CreateWindowExW(
		// @TODO figure out window styling
		// @TODO make default position customizable
		0,                                    // extra window style
		name,                                 // class name
		name,                                 // window name
		WS_OVERLAPPEDWINDOW,                  // window style
		CW_USEDEFAULT,                        // x-position
		CW_USEDEFAULT,                        // y-position
		width,                                // width
		height,                               // height
		NULL,                                 // parent window
		NULL,                                 // menu (?)
		window_struct.window_class.hInstance, // hInstance
		NULL                                  // additional stuff
	);

	if (window_struct.window_handle == NULL) {
		muga_print("[MUGA] Failed to create window.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return 0;
	}

	// assign window into window array
	muga_window win = muga_windows_get_new_window_id();
	muga_windows_windows[win] = window_struct;

	// @TODO make start invisibility option
	ShowWindow(window_struct.window_handle, SW_NORMAL);

	// return
	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	return 0;
}

MUGADEF MUGA_BOOL muga_window_active(MUGA_RESULT* result, muga_window win) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for checking if window is active is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return MUGA_FALSE;
	}
	
	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	return muga_windows_windows[win].alive;
}

MUGADEF void muga_window_update(MUGA_RESULT* result, muga_window win) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for updating is active is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	MSG msg = { 0 };
	if (GetMessage(&msg, NULL, 0, 0) <= 0) {
		muga_windows_windows[win].alive = MUGA_FALSE;
	} else {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_swap_buffers(MUGA_RESULT* result, muga_window win) {
	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

#endif /* WINDOWS */

#ifdef __cplusplus
    }
#endif

#endif /* MUGA_IMPLEMENTATION */

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2023 Samuel Carmichael
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