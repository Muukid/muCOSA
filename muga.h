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

/* apis */

enum muga_graphics_api {
	// @TODO properly implement this
	MUGA_NO_GRAPHICS_API,

	// opengl

	MUGA_OPENGL_1_0,
	MUGA_OPENGL_1_1,
	MUGA_OPENGL_1_2,
	MUGA_OPENGL_1_2_1,
	MUGA_OPENGL_1_3,
	MUGA_OPENGL_1_4,
	MUGA_OPENGL_1_5,
	MUGA_OPENGL_2_0,
	MUGA_OPENGL_2_1,
	MUGA_OPENGL_3_0,
	MUGA_OPENGL_3_1,
	MUGA_OPENGL_3_2_CORE,
	MUGA_OPENGL_3_2_COMPATIBILITY,
	MUGA_OPENGL_3_3_CORE,
	MUGA_OPENGL_3_3_COMPATIBILITY,
	MUGA_OPENGL_4_0_CORE,
	MUGA_OPENGL_4_0_COMPATIBILITY,
	MUGA_OPENGL_4_1_CORE,
	MUGA_OPENGL_4_1_COMPATIBILITY,
	MUGA_OPENGL_4_2_CORE,
	MUGA_OPENGL_4_2_COMPATIBILITY,
	MUGA_OPENGL_4_3_CORE,
	MUGA_OPENGL_4_3_COMPATIBILITY,
	MUGA_OPENGL_4_4_CORE,
	MUGA_OPENGL_4_4_COMPATIBILITY,
	MUGA_OPENGL_4_5_CORE,
	MUGA_OPENGL_4_5_COMPATIBILITY,
	MUGA_OPENGL_4_6_CORE,
	MUGA_OPENGL_4_6_COMPATIBILITY

#define MUGA_OPENGL_FIRST MUGA_OPENGL_1_0
#define MUGA_OPENGL_LAST MUGA_OPENGL_4_6_COMPATIBILITY
#define MUGA_IS_OPENGL(api) (api >= MUGA_OPENGL_FIRST && api <= MUGA_OPENGL_LAST)

};
typedef enum muga_graphics_api muga_graphics_api;



/* general */

MUGADEF void muga_init(MUGA_RESULT* result);
MUGADEF void muga_term(MUGA_RESULT* result);

/* window */

typedef size_m muga_window;

MUGADEF muga_window muga_window_create(MUGA_RESULT* result, muga_graphics_api api, const wchar_m* name, unsigned int width, unsigned int height);
MUGADEF void muga_window_destroy(MUGA_RESULT* result, muga_window win);
MUGADEF MUGA_BOOL muga_window_active(MUGA_RESULT* result, muga_window win);
MUGADEF void muga_window_update(MUGA_RESULT* result, muga_window win);
MUGADEF void muga_window_swap_buffers(MUGA_RESULT* result, muga_window win);

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

// @TODO add flags to not include some graphics libs

// opengl
#include <gl/gl.h>
#include <gl/glu.h>

// wgl tokens

// https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_create_context.txt

#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB             0x2093
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126

#define WGL_CONTEXT_DEBUG_BIT_ARB               0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x0002

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#define ERROR_INVALID_VERSION_ARB               0x2095
#define ERROR_INVALID_PROFILE_ARB               0x2096

// https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt

#define WGL_NUMBER_PIXEL_FORMATS_ARB            0x2000
#define WGL_DRAW_TO_WINDOW_ARB                  0x2001
#define WGL_DRAW_TO_BITMAP_ARB                  0x2002
#define WGL_ACCELERATION_ARB                    0x2003
#define WGL_NEED_PALETTE_ARB                    0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB             0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB              0x2006
#define WGL_SWAP_METHOD_ARB                     0x2007
#define WGL_NUMBER_OVERLAYS_ARB                 0x2008
#define WGL_NUMBER_UNDERLAYS_ARB                0x2009
#define WGL_TRANSPARENT_ARB                     0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB           0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB         0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB          0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB         0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB         0x203B
#define WGL_SHARE_DEPTH_ARB                     0x200C
#define WGL_SHARE_STENCIL_ARB                   0x200D
#define WGL_SHARE_ACCUM_ARB                     0x200E
#define WGL_SUPPORT_GDI_ARB                     0x200F
#define WGL_SUPPORT_OPENGL_ARB                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                   0x2011
#define WGL_STEREO_ARB                          0x2012
#define WGL_PIXEL_TYPE_ARB                      0x2013
#define WGL_COLOR_BITS_ARB                      0x2014
#define WGL_RED_BITS_ARB                        0x2015
#define WGL_RED_SHIFT_ARB                       0x2016
#define WGL_GREEN_BITS_ARB                      0x2017
#define WGL_GREEN_SHIFT_ARB                     0x2018
#define WGL_BLUE_BITS_ARB                       0x2019
#define WGL_BLUE_SHIFT_ARB                      0x201A
#define WGL_ALPHA_BITS_ARB                      0x201B
#define WGL_ALPHA_SHIFT_ARB                     0x201C
#define WGL_ACCUM_BITS_ARB                      0x201D
#define WGL_ACCUM_RED_BITS_ARB                  0x201E
#define WGL_ACCUM_GREEN_BITS_ARB                0x201F
#define WGL_ACCUM_BLUE_BITS_ARB                 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB                0x2021
#define WGL_DEPTH_BITS_ARB                      0x2022
#define WGL_STENCIL_BITS_ARB                    0x2023
#define WGL_AUX_BUFFERS_ARB                     0x2024

#define WGL_NO_ACCELERATION_ARB                 0x2025
#define WGL_GENERIC_ACCELERATION_ARB            0x2026
#define WGL_FULL_ACCELERATION_ARB               0x2027

#define WGL_SWAP_EXCHANGE_ARB                   0x2028
#define WGL_SWAP_COPY_ARB                       0x2029
#define WGL_SWAP_UNDEFINED_ARB                  0x202A

#define WGL_TYPE_RGBA_ARB                       0x202B
#define WGL_TYPE_COLORINDEX_ARB                 0x202C

// wgl variables

typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int *attribList);
wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;

typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
wglChoosePixelFormatARB_type *wglChoosePixelFormatARB;

// pre-def functions

// get the hinstance
// thank you we luv u raymond chen <3
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
HINSTANCE muga_windows_get_hinstance() {
	return HINST_THISCOMPONENT;
}

// https://gist.github.com/nickrolfe/1127313ed1dbf80254b614a721b3ee9c

// initiates a dummy wgl context and gets the opengl extensions
// needed for more pixel format options
MUGA_RESULT muga_windows_init_opengl_extensions() {
	WNDCLASSA win_class = {
		.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
		.lpfnWndProc = DefWindowProcA,
		.hInstance = muga_windows_get_hinstance(),
		.lpszClassName = "dummy wgl window"
	};

	if (!RegisterClassA(&win_class)) {
		muga_print("[MUGA] Failed to register window class.\n");
		return MUGA_FAILURE;
	}

	HWND win = CreateWindowExA(
		0,
		win_class.lpszClassName,
		"dummy wgl window",
		0,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		win_class.hInstance,
		0
	);

	if (!win) {
		muga_print("[MUGA] Failed to create window.\n");
		return MUGA_FAILURE;
	}

	HDC dc = GetDC(win);

	PIXELFORMATDESCRIPTOR format = {
		.nSize = sizeof(PIXELFORMATDESCRIPTOR),
		.nVersion = 1,
		.iPixelType = PFD_TYPE_RGBA,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .cColorBits = 32,
        .cAlphaBits = 8,
        .iLayerType = PFD_MAIN_PLANE,
        .cDepthBits = 24,
        .cStencilBits = 8
	};

	// @TODO make sure these failures destroy window and other stuff!
	int pixel_format = ChoosePixelFormat(dc, &format);
	if (!pixel_format) {
		muga_print("[MUGA] Failed to find a valid pixel format.\n");
		return MUGA_FAILURE;
	}
	if (!SetPixelFormat(dc, pixel_format, &format)) {
		muga_print("[MUGA] Failed to set a pixel format.\n");
		return MUGA_FAILURE;
	}

	HGLRC context = wglCreateContext(dc);
	if (!context) {
		muga_print("[MUGA] Failed to create a valid WGL context.\n");
		return MUGA_FAILURE;
	}
	if (!wglMakeCurrent(dc, context)) {
		muga_print("[MUGA] Failed to load a WGL context.\n");
		return MUGA_FAILURE;
	}

	wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress("wglCreateContextAttribsARB");
	wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress("wglChoosePixelFormatARB");

	wglMakeCurrent(dc, 0);
	wglDeleteContext(context);
	ReleaseDC(win, dc);
	DestroyWindow(win);
	return MUGA_SUCCESS;
}

// creates an opengl context
MUGA_RESULT muga_windows_create_opengl_context(HDC device_context, HGLRC* context, muga_graphics_api api) {
	if (!muga_windows_init_opengl_extensions()) {
		return MUGA_FAILURE;
	}

	// pixel formatting
	// @TODO make this customizable
	int pixel_format_attributes[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
        WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,     32,
        WGL_DEPTH_BITS_ARB,     24,
        WGL_STENCIL_BITS_ARB,   8,
        0
	};

	// get the closest available format
	int pixel_format;
	UINT format_count;
	wglChoosePixelFormatARB(device_context, pixel_format_attributes, 0, 1, &pixel_format, &format_count);
	if (!format_count) {
		muga_print("[MUGA] Failed to find a compatible OpenGL pixel format.\n");
		return MUGA_FAILURE;
	}

	// set format
	PIXELFORMATDESCRIPTOR format;
	DescribePixelFormat(device_context, pixel_format, sizeof(PIXELFORMATDESCRIPTOR), &format);
	if (!SetPixelFormat(device_context, pixel_format, &format)) {
		muga_print("[MUGA] Failed to set OpenGL pixel format.\n");
		return MUGA_FAILURE;
	}

	// create opengl attributes descriptor
	int opengl_attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	// Is there a better way of doing this?
	switch (api) {
		default:
			// ?
			break;
		case MUGA_OPENGL_1_0:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 0;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MUGA_OPENGL_1_1:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 1;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MUGA_OPENGL_1_2:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 2;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MUGA_OPENGL_1_2_1:
		// What the hell do I do here?
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 2;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MUGA_OPENGL_1_3:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 3;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MUGA_OPENGL_1_4:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 4;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MUGA_OPENGL_1_5:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 5;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MUGA_OPENGL_2_0:
			opengl_attributes[1] = 2;
			opengl_attributes[3] = 0;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MUGA_OPENGL_2_1:
			opengl_attributes[1] = 2;
			opengl_attributes[3] = 0;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MUGA_OPENGL_3_0:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 0;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MUGA_OPENGL_3_1:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 1;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MUGA_OPENGL_3_2_CORE:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 2;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_3_2_COMPATIBILITY:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 2;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_3_3_CORE:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_3_3_COMPATIBILITY:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_0_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 0;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_0_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 0;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_1_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 1;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_1_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 1;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_2_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 2;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_2_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 2;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_3_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_3_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_4_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 4;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_4_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 4;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_5_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 5;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_5_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 5;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_6_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 6;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_6_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 6;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
	}

	// create and activate context
	HGLRC opengl_context = wglCreateContextAttribsARB(device_context, 0, opengl_attributes);
	if (!opengl_context) {
		muga_print("[MUGA] Failed to create a valid OpenGL context.");
		return MUGA_FAILURE;
	}
	if (!wglMakeCurrent(device_context, opengl_context)) {
		muga_print("[MUGA] Failed to activate OpenGL context.");
		return MUGA_FAILURE;
	}
	*context = opengl_context;

	return MUGA_SUCCESS;
}

/* default window setup */

// basic window vars/structs

struct muga_windows_window {
	// activity
	MUGA_BOOL active;
	MUGA_BOOL alive;

	// window class information
	WNDCLASSEXW window_class;
	// window handle
	HWND window_handle;
	// device context
	HDC device_context;

	// api
	muga_graphics_api api;
	// @TODO don't store info for apis that aren't running
	// opengl context
	HGLRC opengl_context;
};
typedef struct muga_windows_window muga_windows_window;

// What a stupid name
muga_windows_window* muga_windows_windows      = MUGA_NULL_PTR;
size_m               muga_windows_windows_length = 0;

MUGA_BOOL muga_windows_window_binded = MUGA_FALSE;
size_m muga_windows_binded_window = 0;

// window funcs

// window proc
LRESULT CALLBACK muga_windows_default_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	default:
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
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

void muga_windows_unbind() {
	if (muga_windows_window_binded) {
		if (MUGA_IS_OPENGL(muga_windows_windows[muga_windows_binded_window].api)) {
			wglMakeCurrent(NULL, NULL);
			printf("unbinded\n");
		}
		muga_windows_window_binded = MUGA_FALSE;
	}
}

void muga_windows_bind(muga_window win) {
	if (muga_windows_window_binded && muga_windows_binded_window == win) {
		return;
	}

	muga_windows_unbind();
	if (MUGA_IS_OPENGL(muga_windows_windows[win].api)) {
		wglMakeCurrent(muga_windows_windows[win].device_context, muga_windows_windows[win].opengl_context);
	}
	muga_windows_window_binded = MUGA_TRUE;
	muga_windows_binded_window = win;
}

/* initiation/termination */

MUGA_BOOL muga_windows_has_initiated = MUGA_FALSE;

MUGADEF void muga_init(MUGA_RESULT* result) {
	// init check
	if (muga_windows_has_initiated == MUGA_TRUE) {
		muga_print("[MUGA] An attempt to initiate muga was made after already being initiated.\n");
		if (result != MUGA_NULL) {
			*result = MUGA_FAILURE;
		}
		return;
	}
	muga_windows_has_initiated = MUGA_TRUE;
	// initiate windows buffer
	muga_windows_windows_length = 1;
	muga_windows_windows = muga_malloc(sizeof(muga_windows_window) * muga_windows_windows_length);
	muga_windows_windows[0].active = MUGA_FALSE;

	// return
	if (result != MUGA_NULL) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_term(MUGA_RESULT* result) {
	// term check
	if (muga_windows_has_initiated == MUGA_FALSE) {
		muga_print("[MUGA] An attempt to terminate muga was made even though muga was already not running.\n");
		if (result != MUGA_NULL) {
			*result = MUGA_FAILURE;
		}
		return;
	}
	muga_windows_has_initiated = MUGA_FALSE;
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

// @TODO make sure window gets destroyed if it fails
MUGADEF muga_window muga_window_create(MUGA_RESULT* result, muga_graphics_api api, const wchar_m* name, unsigned int width, unsigned int height) {
	muga_windows_unbind();

	// initialize muga_windows_window struct

	muga_windows_window window_struct = {
		.active = MUGA_TRUE,
		.alive = MUGA_TRUE,
		// (WNDCLASSEXW)
		// @TODO add customizability to a lot of this
		.window_class = {
			.cbSize = sizeof(WNDCLASSEXW),                       // size of struct
			.style =         CS_HREDRAW | CS_VREDRAW | CS_OWNDC, // style
			.lpfnWndProc =   muga_windows_default_window_proc,   // window process function
			.cbClsExtra =    0,                                  // extra class allocation bytes
			.cbWndExtra =    0,                                  // extra window instance allocation bytes
			.hInstance =     muga_windows_get_hinstance(),       // hInstance
			.hIcon =         LoadIcon(0, IDI_WINLOGO),           // window icon
			.hCursor =       LoadCursor(0, IDC_ARROW),           // window cursor
			.hbrBackground = 0,                                  // background brush (0 is fine)
			.lpszMenuName =  name,                               // menu name
			.lpszClassName = name,                               // class name
			.hIconSm =       0                                   // small window icon
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

	// get some more info
	muga_windows_windows[win].device_context = GetDC(muga_windows_windows[win].window_handle);

	// make api context

	// @TODO make option to share contexts (child windows?)

	muga_windows_windows[win].api = api;
	// opengl
	if (MUGA_IS_OPENGL(api)) {
		if (!muga_windows_create_opengl_context(
				muga_windows_windows[win].device_context,
				&muga_windows_windows[win].opengl_context,
				api
			)
		) {
			if (result != MUGA_NULL_PTR) {
				*result = MUGA_FAILURE;
			}
			return 0;
		}
	// no api
	} else if (api != MUGA_NO_GRAPHICS_API) {
		muga_print("[MUGA] Unsupported graphics API for Windows.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		// @TODO handle this edge case
		return 0;
	}

	// @TODO make start invisibility option
	ShowWindow(muga_windows_windows[win].window_handle, SW_NORMAL);
	UpdateWindow(muga_windows_windows[win].window_handle);
	muga_windows_bind(win);

	// return
	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	return win;
}

MUGADEF void muga_window_destroy(MUGA_RESULT* result, muga_window win) {

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

	muga_windows_bind(win);

	MSG msg = { 0 };
	while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			muga_windows_windows[win].alive = MUGA_FALSE;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_swap_buffers(MUGA_RESULT* result, muga_window win) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for swapping buffers is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	muga_windows_bind(win);

	if (MUGA_IS_OPENGL(muga_windows_windows[win].api)) {
		SwapBuffers(muga_windows_windows[win].device_context);
	}

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
