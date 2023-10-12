/*
muga.h - Muukid
Public domain single-file C library for creating windows and contexts for cross-platform graphics APIs. 
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

#if !defined(size_m)
	
	#include <stddef.h>

	#define size_m size_t

#endif

#if !defined(SIZE_MAX_M)

    #include <stdint.h>

    #define SIZE_MAX_M SIZE_MAX

#endif

#ifndef wchar_m
    #define wchar_m wchar_t
#endif

#ifndef muga_wstrlen
    #include <wchar.h>
    #define muga_wstrlen wcslen
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

#if !defined(muga_strcmp) || \
    !defined(muga_memset)

    #include <string.h>

    #ifndef muga_strcmp
    	#define muga_strcmp strcmp
    #endif

    #ifndef muga_memset
    	#define muga_memset memset
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

/* input */

enum muga_input_method {
	MUGA_NO_INPUT_METHOD,
	MUGA_KEYBOARD
};
typedef enum muga_input_method muga_input_method;

enum muga_input_key {
	MUGA_NO_INPUT_KEY,

	// keyboard
	// (https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes)

	MUGA_KEYBOARD_KEY_BACKSPACE,
	MUGA_KEYBOARD_KEY_TAB,
	MUGA_KEYBOARD_KEY_CLEAR,
	MUGA_KEYBOARD_KEY_RETURN,
	MUGA_KEYBOARD_KEY_PAUSE,
	MUGA_KEYBOARD_KEY_ESCAPE,
	MUGA_KEYBOARD_KEY_MODECHANGE,
	MUGA_KEYBOARD_KEY_SPACE,
	MUGA_KEYBOARD_KEY_PRIOR,
	MUGA_KEYBOARD_KEY_NEXT,
	MUGA_KEYBOARD_KEY_END,
	MUGA_KEYBOARD_KEY_HOME,
	MUGA_KEYBOARD_KEY_LEFT,
	MUGA_KEYBOARD_KEY_UP,
	MUGA_KEYBOARD_KEY_RIGHT,
	MUGA_KEYBOARD_KEY_DOWN,
	MUGA_KEYBOARD_KEY_SELECT,
	MUGA_KEYBOARD_KEY_PRINT,
	MUGA_KEYBOARD_KEY_EXECUTE,
	MUGA_KEYBOARD_KEY_INSERT,
	MUGA_KEYBOARD_KEY_DELETE,
	MUGA_KEYBOARD_KEY_HELP,
	MUGA_KEYBOARD_KEY_0,
	MUGA_KEYBOARD_KEY_1,
	MUGA_KEYBOARD_KEY_2,
	MUGA_KEYBOARD_KEY_3,
	MUGA_KEYBOARD_KEY_4,
	MUGA_KEYBOARD_KEY_5,
	MUGA_KEYBOARD_KEY_6,
	MUGA_KEYBOARD_KEY_7,
	MUGA_KEYBOARD_KEY_8,
	MUGA_KEYBOARD_KEY_9,
	MUGA_KEYBOARD_KEY_A,
	MUGA_KEYBOARD_KEY_B,
	MUGA_KEYBOARD_KEY_C,
	MUGA_KEYBOARD_KEY_D,
	MUGA_KEYBOARD_KEY_E,
	MUGA_KEYBOARD_KEY_F,
	MUGA_KEYBOARD_KEY_G,
	MUGA_KEYBOARD_KEY_H,
	MUGA_KEYBOARD_KEY_I,
	MUGA_KEYBOARD_KEY_J,
	MUGA_KEYBOARD_KEY_K,
	MUGA_KEYBOARD_KEY_L,
	MUGA_KEYBOARD_KEY_M,
	MUGA_KEYBOARD_KEY_N,
	MUGA_KEYBOARD_KEY_O,
	MUGA_KEYBOARD_KEY_P,
	MUGA_KEYBOARD_KEY_Q,
	MUGA_KEYBOARD_KEY_R,
	MUGA_KEYBOARD_KEY_S,
	MUGA_KEYBOARD_KEY_T,
	MUGA_KEYBOARD_KEY_U,
	MUGA_KEYBOARD_KEY_V,
	MUGA_KEYBOARD_KEY_W,
	MUGA_KEYBOARD_KEY_X,
	MUGA_KEYBOARD_KEY_Y,
	MUGA_KEYBOARD_KEY_Z,
	MUGA_KEYBOARD_KEY_LEFT_WINDOWS,
	MUGA_KEYBOARD_KEY_RIGHT_WINDOWS,
	MUGA_KEYBOARD_KEY_NUMPAD_0,
	MUGA_KEYBOARD_KEY_NUMPAD_1,
	MUGA_KEYBOARD_KEY_NUMPAD_2,
	MUGA_KEYBOARD_KEY_NUMPAD_3,
	MUGA_KEYBOARD_KEY_NUMPAD_4,
	MUGA_KEYBOARD_KEY_NUMPAD_5,
	MUGA_KEYBOARD_KEY_NUMPAD_6,
	MUGA_KEYBOARD_KEY_NUMPAD_7,
	MUGA_KEYBOARD_KEY_NUMPAD_8,
	MUGA_KEYBOARD_KEY_NUMPAD_9,
	MUGA_KEYBOARD_KEY_MULTIPLY,
	MUGA_KEYBOARD_KEY_ADD,
	MUGA_KEYBOARD_KEY_SEPARATOR,
	MUGA_KEYBOARD_KEY_SUBTRACT,
	MUGA_KEYBOARD_KEY_DECIMAL,
	MUGA_KEYBOARD_KEY_DIVIDE,
	MUGA_KEYBOARD_KEY_F1,
	MUGA_KEYBOARD_KEY_F2,
	MUGA_KEYBOARD_KEY_F3,
	MUGA_KEYBOARD_KEY_F4,
	MUGA_KEYBOARD_KEY_F5,
	MUGA_KEYBOARD_KEY_F6,
	MUGA_KEYBOARD_KEY_F7,
	MUGA_KEYBOARD_KEY_F8,
	MUGA_KEYBOARD_KEY_F9,
	MUGA_KEYBOARD_KEY_F10,
	MUGA_KEYBOARD_KEY_F11,
	MUGA_KEYBOARD_KEY_F12,
	MUGA_KEYBOARD_KEY_F13,
	MUGA_KEYBOARD_KEY_F14,
	MUGA_KEYBOARD_KEY_F15,
	MUGA_KEYBOARD_KEY_F16,
	MUGA_KEYBOARD_KEY_F17,
	MUGA_KEYBOARD_KEY_F18,
	MUGA_KEYBOARD_KEY_F19,
	MUGA_KEYBOARD_KEY_F20,
	MUGA_KEYBOARD_KEY_F21,
	MUGA_KEYBOARD_KEY_F22,
	MUGA_KEYBOARD_KEY_F23,
	MUGA_KEYBOARD_KEY_F24,
	MUGA_KEYBOARD_KEY_NUMLOCK,
	MUGA_KEYBOARD_KEY_SCROLL,
	MUGA_KEYBOARD_KEY_LEFT_SHIFT,
	MUGA_KEYBOARD_KEY_RIGHT_SHIFT,
	MUGA_KEYBOARD_KEY_LEFT_CONTROL,
	MUGA_KEYBOARD_KEY_RIGHT_CONTROL,
	MUGA_KEYBOARD_KEY_LEFT_MENU,
	MUGA_KEYBOARD_KEY_RIGHT_MENU,
	MUGA_KEYBOARD_KEY_ATTN,
	MUGA_KEYBOARD_KEY_CRSEL,
	MUGA_KEYBOARD_KEY_EXSEL,
	MUGA_KEYBOARD_KEY_EREOF,
	MUGA_KEYBOARD_KEY_PLAY,
	MUGA_KEYBOARD_KEY_PA1

#define MUGA_KEYBOARD_FIRST MUGA_KEYBOARD_KEY_BACKSPACE
#define MUGA_KEYBOARD_LAST MUGA_KEYBOARD_KEY_PA1
#define MUGA_IS_KEYBOARD(key) (key >= MUGA_KEYBOARD_FIRST && key <= MUGA_KEYBOARD_LAST)
};
typedef enum muga_input_key muga_input_key;

#define MUGA_KEY_BIT  MUGA_BOOL
#define MUGA_KEY_UP   MUGA_FALSE
#define MUGA_KEY_DOWN MUGA_TRUE

/* general */

MUGADEF void muga_init(MUGA_RESULT* result);
MUGADEF void muga_term(MUGA_RESULT* result);

MUGADEF double muga_time_get(MUGA_RESULT* result);
MUGADEF void   muga_time_set(MUGA_RESULT* result, double time);

/* window */

typedef size_m muga_window;
#define MUGA_NO_WINDOW SIZE_MAX_M

// basic window functionality

// pixel formatting
// https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt
// https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glXChooseFBConfig.xml
struct muga_pixel_format {
	MUGA_BOOL doublebuffer;

	unsigned int red_bits;
	unsigned int green_bits;
	unsigned int blue_bits;
	unsigned int alpha_bits;

	unsigned int depth_bits;
	unsigned int stencil_bits;
};
typedef struct muga_pixel_format muga_pixel_format;

// settings
struct muga_window_settings_struct {
	muga_pixel_format pixel_format;

	MUGA_BOOL visible;
	MUGA_BOOL resizable;

	// @TODO test negative values
	int x;
	int y;
};
typedef struct muga_window_settings_struct muga_window_settings_struct;

muga_window_settings_struct muga_window_settings = {
	.pixel_format = {
		.doublebuffer = MUGA_TRUE,
		.red_bits =     8,
		.green_bits =   8,
		.blue_bits =    8,
		.alpha_bits =   8,
		.depth_bits =   24,
		.stencil_bits = 8
	},

	.visible =   MUGA_TRUE,
	.resizable = MUGA_TRUE,

	.x = 400,
	.y = 200
};

MUGADEF muga_window muga_window_create(MUGA_RESULT* result, muga_graphics_api api, MUGA_BOOL (*load_functions)(void), const wchar_m* name, unsigned int width, unsigned int height);
MUGADEF void muga_window_destroy(MUGA_RESULT* result, muga_window win);

MUGADEF void muga_window_update(MUGA_RESULT* result, muga_window win);
MUGADEF void muga_window_swap_buffers(MUGA_RESULT* result, muga_window win);

// sets

MUGADEF void muga_window_set_context(MUGA_RESULT* result, muga_window win);

MUGADEF void muga_window_set_title(MUGA_RESULT* result, muga_window win, const wchar_m* title);

// get / toggle functions

MUGADEF MUGA_BOOL muga_window_get_closed(MUGA_RESULT* result, muga_window win);
MUGADEF void muga_window_close(MUGA_RESULT* result, muga_window win);

MUGADEF MUGA_BOOL muga_window_get_focused(MUGA_RESULT* result, muga_window win);
MUGADEF void      muga_window_focus(MUGA_RESULT* result, muga_window win);

// get / set window functions

MUGADEF MUGA_BOOL muga_window_get_visible(MUGA_RESULT* result, muga_window win);
MUGADEF void      muga_window_set_visible(MUGA_RESULT* result, muga_window win, MUGA_BOOL visible);

MUGADEF void muga_window_get_position(MUGA_RESULT* result, muga_window win, int* x, int* y);
MUGADEF void muga_window_set_position(MUGA_RESULT* result, muga_window win, int  x, int  y);

MUGADEF void muga_window_get_dimensions(MUGA_RESULT* result, muga_window win, unsigned int* width, unsigned int* height);
MUGADEF void muga_window_set_dimensions(MUGA_RESULT* result, muga_window win, unsigned int  width, unsigned int  height);

MUGADEF MUGA_BOOL muga_window_get_maximized(MUGA_RESULT* result, muga_window win);
MUGADEF void      muga_window_set_maximized(MUGA_RESULT* result, muga_window win, MUGA_BOOL maximized);

MUGADEF MUGA_BOOL muga_window_get_minimized(MUGA_RESULT* result, muga_window win);
MUGADEF void      muga_window_set_minimized(MUGA_RESULT* result, muga_window win, MUGA_BOOL minimized);

// input

MUGADEF MUGA_KEY_BIT muga_window_get_input_bit(MUGA_RESULT* result, muga_window win, muga_input_method method, muga_input_key key);

// callbacks

MUGADEF void muga_window_set_framebuffer_resize_callback(MUGA_RESULT* result, muga_window win, void (*framebuffer_resize_callback)(muga_window win, int new_width, int new_height));

/* opengl functions */

MUGADEF void* muga_get_opengl_function_address(const char* name);

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

/* graphics api flag settings */

#ifndef MUGA_OPENGL
    #ifndef MUGA_NO_OPENGL
    	#define MUGA_NO_OPENGL
    #endif
#endif

#ifdef MUGA_NO_API
    #ifndef MUGA_NO_OPENGL
    	#define MUGA_NO_OPENGL
    #endif
#endif

/* WINDOWS IMPLEMENTATION */

#if defined(_WIN32) || defined(WIN32)
#define MUGA_WINDOWS

#include <windows.h>

// get the hinstance
// thank you we luv u raymond chen <3
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
HINSTANCE muga_windows_get_hinstance() {
	return HINST_THISCOMPONENT;
}

/* OPENGL SETUP */
#ifndef MUGA_NO_OPENGL

#ifndef MUGA_NO_INCLUDE_OPENGL
	#ifndef glClearColor
		#include <gl/gl.h>
		#include <gl/glu.h>
	#endif
#endif

#define MUGA_OPENGL_CALL(stuff) stuff

/* WGL tokens */

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

// https://gist.github.com/nickrolfe/1127313ed1dbf80254b614a721b3ee9c

// initiates a dummy wgl context and gets the opengl extensions
// needed for more pixel format options
MUGA_BOOL muga_windows_has_initiated_opengl_extensions = MUGA_FALSE;
MUGA_RESULT muga_windows_init_opengl_extensions() {
	if (muga_windows_has_initiated_opengl_extensions) {
		return MUGA_SUCCESS;
	}
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

	int pixel_format = ChoosePixelFormat(dc, &format);
	if (!pixel_format) {
		muga_print("[MUGA] Failed to find a valid pixel format.\n");
		ReleaseDC(win, dc);
		DestroyWindow(win);
		return MUGA_FAILURE;
	}
	if (!SetPixelFormat(dc, pixel_format, &format)) {
		muga_print("[MUGA] Failed to set a pixel format.\n");
		ReleaseDC(win, dc);
		DestroyWindow(win);
		return MUGA_FAILURE;
	}

	HGLRC context = wglCreateContext(dc);
	if (!context) {
		muga_print("[MUGA] Failed to create a valid WGL context.\n");
		ReleaseDC(win, dc);
		DestroyWindow(win);
		return MUGA_FAILURE;
	}
	if (!wglMakeCurrent(dc, context)) {
		muga_print("[MUGA] Failed to load a WGL context.\n");
		wglDeleteContext(context);
		ReleaseDC(win, dc);
		DestroyWindow(win);
		return MUGA_FAILURE;
	}

	wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress("wglCreateContextAttribsARB");
	wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress("wglChoosePixelFormatARB");

	wglMakeCurrent(dc, 0);
	wglDeleteContext(context);
	ReleaseDC(win, dc);
	DestroyWindow(win);
	muga_windows_has_initiated_opengl_extensions = MUGA_TRUE;
	return MUGA_SUCCESS;
}

// creates an opengl context
MUGA_RESULT muga_windows_create_opengl_context(HDC device_context, HGLRC* context, muga_graphics_api api) {
	if (!muga_windows_init_opengl_extensions()) {
		return MUGA_FAILURE;
	}

	// pixel formatting
	int pixel_format_attributes[] = {
		WGL_DRAW_TO_WINDOW_ARB, MUGA_TRUE,
        WGL_SUPPORT_OPENGL_ARB, MUGA_TRUE,
        WGL_DOUBLE_BUFFER_ARB,  muga_window_settings.pixel_format.doublebuffer,
        WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
        WGL_RED_BITS_ARB,       muga_window_settings.pixel_format.red_bits,
        WGL_GREEN_BITS_ARB,     muga_window_settings.pixel_format.green_bits,
        WGL_BLUE_BITS_ARB,      muga_window_settings.pixel_format.blue_bits,
        WGL_ALPHA_BITS_ARB,     muga_window_settings.pixel_format.alpha_bits,
        WGL_DEPTH_BITS_ARB,     muga_window_settings.pixel_format.depth_bits,
        WGL_STENCIL_BITS_ARB,   muga_window_settings.pixel_format.stencil_bits,
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

#else

#define MUGA_OPENGL_CALL(...)

#endif /* MUGA_NO_OPENGL */

/* keyboard input */

int muga_windows_muga_key_to_windows_key(muga_input_key key) {
	switch (key) {
	default:
		return VK_NONAME;
		break;
	case MUGA_KEYBOARD_KEY_BACKSPACE:
		return VK_BACK;
		break;
	case MUGA_KEYBOARD_KEY_TAB:
		return VK_TAB;
		break;
	case MUGA_KEYBOARD_KEY_CLEAR:
		return VK_CLEAR;
		break;
	case MUGA_KEYBOARD_KEY_RETURN:
		return VK_RETURN;
		break;
	case MUGA_KEYBOARD_KEY_PAUSE:
		return VK_PAUSE;
		break;
	case MUGA_KEYBOARD_KEY_ESCAPE:
		return VK_ESCAPE;
		break;
	case MUGA_KEYBOARD_KEY_MODECHANGE:
		return VK_MODECHANGE;
		break;
	case MUGA_KEYBOARD_KEY_SPACE:
		return VK_SPACE;
		break;
	case MUGA_KEYBOARD_KEY_PRIOR:
		return VK_PRIOR;
		break;
	case MUGA_KEYBOARD_KEY_NEXT:
		return VK_NEXT;
		break;
	case MUGA_KEYBOARD_KEY_END:
		return VK_END;
		break;
	case MUGA_KEYBOARD_KEY_HOME:
		return VK_HOME;
		break;
	case MUGA_KEYBOARD_KEY_LEFT:
		return VK_LEFT;
		break;
	case MUGA_KEYBOARD_KEY_UP:
		return VK_UP;
		break;
	case MUGA_KEYBOARD_KEY_RIGHT:
		return VK_RIGHT;
		break;
	case MUGA_KEYBOARD_KEY_DOWN:
		return VK_DOWN;
		break;
	case MUGA_KEYBOARD_KEY_SELECT:
		return VK_SELECT;
		break;
	case MUGA_KEYBOARD_KEY_PRINT:
		return VK_PRINT;
		break;
	case MUGA_KEYBOARD_KEY_EXECUTE:
		return VK_EXECUTE;
		break;
	case MUGA_KEYBOARD_KEY_INSERT:
		return VK_INSERT;
		break;
	case MUGA_KEYBOARD_KEY_DELETE:
		return VK_DELETE;
		break;
	case MUGA_KEYBOARD_KEY_HELP:
		return VK_HELP;
		break;
	case MUGA_KEYBOARD_KEY_0:
		return 0x30;
		break;
	case MUGA_KEYBOARD_KEY_1:
		return 0x31;
		break;
	case MUGA_KEYBOARD_KEY_2:
		return 0x32;
		break;
	case MUGA_KEYBOARD_KEY_3:
		return 0x33;
		break;
	case MUGA_KEYBOARD_KEY_4:
		return 0x34;
		break;
	case MUGA_KEYBOARD_KEY_5:
		return 0x35;
		break;
	case MUGA_KEYBOARD_KEY_6:
		return 0x36;
		break;
	case MUGA_KEYBOARD_KEY_7:
		return 0x37;
		break;
	case MUGA_KEYBOARD_KEY_8:
		return 0x38;
		break;
	case MUGA_KEYBOARD_KEY_9:
		return 0x39;
		break;
	case MUGA_KEYBOARD_KEY_A:
		return 0x41;
		break;
	case MUGA_KEYBOARD_KEY_B:
		return 0x42;
		break;
	case MUGA_KEYBOARD_KEY_C:
		return 0x43;
		break;
	case MUGA_KEYBOARD_KEY_D:
		return 0x44;
		break;
	case MUGA_KEYBOARD_KEY_E:
		return 0x45;
		break;
	case MUGA_KEYBOARD_KEY_F:
		return 0x46;
		break;
	case MUGA_KEYBOARD_KEY_G:
		return 0x47;
		break;
	case MUGA_KEYBOARD_KEY_H:
		return 0x48;
		break;
	case MUGA_KEYBOARD_KEY_I:
		return 0x49;
		break;
	case MUGA_KEYBOARD_KEY_J:
		return 0x4A;
		break;
	case MUGA_KEYBOARD_KEY_K:
		return 0x4B;
		break;
	case MUGA_KEYBOARD_KEY_L:
		return 0x4C;
		break;
	case MUGA_KEYBOARD_KEY_M:
		return 0x4D;
		break;
	case MUGA_KEYBOARD_KEY_N:
		return 0x4E;
		break;
	case MUGA_KEYBOARD_KEY_O:
		return 0x4F;
		break;
	case MUGA_KEYBOARD_KEY_P:
		return 0x50;
		break;
	case MUGA_KEYBOARD_KEY_Q:
		return 0x51;
		break;
	case MUGA_KEYBOARD_KEY_R:
		return 0x52;
		break;
	case MUGA_KEYBOARD_KEY_S:
		return 0x53;
		break;
	case MUGA_KEYBOARD_KEY_T:
		return 0x54;
		break;
	case MUGA_KEYBOARD_KEY_U:
		return 0x55;
		break;
	case MUGA_KEYBOARD_KEY_V:
		return 0x56;
		break;
	case MUGA_KEYBOARD_KEY_W:
		return 0x57;
		break;
	case MUGA_KEYBOARD_KEY_X:
		return 0x58;
		break;
	case MUGA_KEYBOARD_KEY_Y:
		return 0x59;
		break;
	case MUGA_KEYBOARD_KEY_Z:
		return 0x5A;
		break;
	case MUGA_KEYBOARD_KEY_LEFT_WINDOWS:
		return VK_LWIN;
		break;
	case MUGA_KEYBOARD_KEY_RIGHT_WINDOWS:
		return VK_RWIN;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_0:
		return VK_NUMPAD0;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_1:
		return VK_NUMPAD1;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_2:
		return VK_NUMPAD2;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_3:
		return VK_NUMPAD3;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_4:
		return VK_NUMPAD4;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_5:
		return VK_NUMPAD5;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_6:
		return VK_NUMPAD6;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_7:
		return VK_NUMPAD7;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_8:
		return VK_NUMPAD8;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_9:
		return VK_NUMPAD9;
		break;
	case MUGA_KEYBOARD_KEY_MULTIPLY:
		return VK_MULTIPLY;
		break;
	case MUGA_KEYBOARD_KEY_ADD:
		return VK_ADD;
		break;
	case MUGA_KEYBOARD_KEY_SEPARATOR:
		return VK_SEPARATOR;
		break;
	case MUGA_KEYBOARD_KEY_SUBTRACT:
		return VK_SUBTRACT;
		break;
	case MUGA_KEYBOARD_KEY_DECIMAL:
		return VK_DECIMAL;
		break;
	case MUGA_KEYBOARD_KEY_DIVIDE:
		return VK_DIVIDE;
		break;
	case MUGA_KEYBOARD_KEY_F1:
		return VK_F1;
		break;
	case MUGA_KEYBOARD_KEY_F2:
		return VK_F2;
		break;
	case MUGA_KEYBOARD_KEY_F3:
		return VK_F3;
		break;
	case MUGA_KEYBOARD_KEY_F4:
		return VK_F4;
		break;
	case MUGA_KEYBOARD_KEY_F5:
		return VK_F5;
		break;
	case MUGA_KEYBOARD_KEY_F6:
		return VK_F6;
		break;
	case MUGA_KEYBOARD_KEY_F7:
		return VK_F7;
		break;
	case MUGA_KEYBOARD_KEY_F8:
		return VK_F8;
		break;
	case MUGA_KEYBOARD_KEY_F9:
		return VK_F9;
		break;
	case MUGA_KEYBOARD_KEY_F10:
		return VK_F10;
		break;
	case MUGA_KEYBOARD_KEY_F11:
		return VK_F11;
		break;
	case MUGA_KEYBOARD_KEY_F12:
		return VK_F12;
		break;
	case MUGA_KEYBOARD_KEY_F13:
		return VK_F13;
		break;
	case MUGA_KEYBOARD_KEY_F14:
		return VK_F14;
		break;
	case MUGA_KEYBOARD_KEY_F15:
		return VK_F15;
		break;
	case MUGA_KEYBOARD_KEY_F16:
		return VK_F16;
		break;
	case MUGA_KEYBOARD_KEY_F17:
		return VK_F17;
		break;
	case MUGA_KEYBOARD_KEY_F18:
		return VK_F18;
		break;
	case MUGA_KEYBOARD_KEY_F19:
		return VK_F19;
		break;
	case MUGA_KEYBOARD_KEY_F20:
		return VK_F20;
		break;
	case MUGA_KEYBOARD_KEY_F21:
		return VK_F21;
		break;
	case MUGA_KEYBOARD_KEY_F22:
		return VK_F22;
		break;
	case MUGA_KEYBOARD_KEY_F23:
		return VK_F23;
		break;
	case MUGA_KEYBOARD_KEY_F24:
		return VK_F24;
		break;
	case MUGA_KEYBOARD_KEY_NUMLOCK:
		return VK_NUMLOCK;
		break;
	case MUGA_KEYBOARD_KEY_SCROLL:
		return VK_SCROLL;
		break;
	case MUGA_KEYBOARD_KEY_LEFT_SHIFT:
		return VK_LSHIFT;
		break;
	case MUGA_KEYBOARD_KEY_RIGHT_SHIFT:
		return VK_RSHIFT;
		break;
	case MUGA_KEYBOARD_KEY_LEFT_CONTROL:
		return VK_LCONTROL;
		break;
	case MUGA_KEYBOARD_KEY_RIGHT_CONTROL:
		return VK_RCONTROL;
		break;
	case MUGA_KEYBOARD_KEY_LEFT_MENU:
		return VK_LMENU;
		break;
	case MUGA_KEYBOARD_KEY_RIGHT_MENU:
		return VK_RMENU;
		break;
	case MUGA_KEYBOARD_KEY_ATTN:
		return VK_ATTN;
		break;
	case MUGA_KEYBOARD_KEY_CRSEL:
		return VK_CRSEL;
		break;
	case MUGA_KEYBOARD_KEY_EXSEL:
		return VK_EXSEL;
		break;
	case MUGA_KEYBOARD_KEY_EREOF:
		return VK_EREOF;
		break;
	case MUGA_KEYBOARD_KEY_PLAY:
		return VK_PLAY;
		break;
	case MUGA_KEYBOARD_KEY_PA1:
		return VK_PA1;
		break;
	}
}

muga_input_key muga_windows_windows_key_to_muga_key(int key) {
	switch (key) {
	default:
		return MUGA_NO_INPUT_KEY;
		break;
	case VK_BACK:
		return MUGA_KEYBOARD_KEY_BACKSPACE;
		break;
	case VK_TAB:
		return MUGA_KEYBOARD_KEY_TAB;
		break;
	case VK_CLEAR:
		return MUGA_KEYBOARD_KEY_CLEAR;
		break;
	case VK_RETURN:
		return MUGA_KEYBOARD_KEY_RETURN;
		break;
	case VK_PAUSE:
		return MUGA_KEYBOARD_KEY_PAUSE;
		break;
	case VK_ESCAPE:
		return MUGA_KEYBOARD_KEY_ESCAPE;
		break;
	case VK_MODECHANGE:
		return MUGA_KEYBOARD_KEY_MODECHANGE;
		break;
	case VK_SPACE:
		return MUGA_KEYBOARD_KEY_SPACE;
		break;
	case VK_PRIOR:
		return MUGA_KEYBOARD_KEY_PRIOR;
		break;
	case VK_NEXT:
		return MUGA_KEYBOARD_KEY_NEXT;
		break;
	case VK_END:
		return MUGA_KEYBOARD_KEY_END;
		break;
	case VK_HOME:
		return MUGA_KEYBOARD_KEY_HOME;
		break;
	case VK_LEFT:
		return MUGA_KEYBOARD_KEY_LEFT;
		break;
	case VK_UP:
		return MUGA_KEYBOARD_KEY_UP;
		break;
	case VK_RIGHT:
		return MUGA_KEYBOARD_KEY_RIGHT;
		break;
	case VK_DOWN:
		return MUGA_KEYBOARD_KEY_DOWN;
		break;
	case VK_SELECT:
		return MUGA_KEYBOARD_KEY_SELECT;
		break;
	case VK_PRINT:
		return MUGA_KEYBOARD_KEY_PRINT;
		break;
	case VK_EXECUTE:
		return MUGA_KEYBOARD_KEY_EXECUTE;
		break;
	case VK_INSERT:
		return MUGA_KEYBOARD_KEY_INSERT;
		break;
	case VK_DELETE:
		return MUGA_KEYBOARD_KEY_DELETE;
		break;
	case VK_HELP:
		return MUGA_KEYBOARD_KEY_HELP;
		break;
	case 0x30:
		return MUGA_KEYBOARD_KEY_0;
		break;
	case 0x31:
		return MUGA_KEYBOARD_KEY_1;
		break;
	case 0x32:
		return MUGA_KEYBOARD_KEY_2;
		break;
	case 0x33:
		return MUGA_KEYBOARD_KEY_3;
		break;
	case 0x34:
		return MUGA_KEYBOARD_KEY_4;
		break;
	case 0x35:
		return MUGA_KEYBOARD_KEY_5;
		break;
	case 0x36:
		return MUGA_KEYBOARD_KEY_6;
		break;
	case 0x37:
		return MUGA_KEYBOARD_KEY_7;
		break;
	case 0x38:
		return MUGA_KEYBOARD_KEY_8;
		break;
	case 0x39:
		return MUGA_KEYBOARD_KEY_9;
		break;
	case 0x41:
		return MUGA_KEYBOARD_KEY_A;
		break;
	case 0x42:
		return MUGA_KEYBOARD_KEY_B;
		break;
	case 0x43:
		return MUGA_KEYBOARD_KEY_C;
		break;
	case 0x44:
		return MUGA_KEYBOARD_KEY_D;
		break;
	case 0x45:
		return MUGA_KEYBOARD_KEY_E;
		break;
	case 0x46:
		return MUGA_KEYBOARD_KEY_F;
		break;
	case 0x47:
		return MUGA_KEYBOARD_KEY_G;
		break;
	case 0x48:
		return MUGA_KEYBOARD_KEY_H;
		break;
	case 0x49:
		return MUGA_KEYBOARD_KEY_I;
		break;
	case 0x4A:
		return MUGA_KEYBOARD_KEY_J;
		break;
	case 0x4B:
		return MUGA_KEYBOARD_KEY_K;
		break;
	case 0x4C:
		return MUGA_KEYBOARD_KEY_L;
		break;
	case 0x4D:
		return MUGA_KEYBOARD_KEY_M;
		break;
	case 0x4E:
		return MUGA_KEYBOARD_KEY_N;
		break;
	case 0x4F:
		return MUGA_KEYBOARD_KEY_O;
		break;
	case 0x50:
		return MUGA_KEYBOARD_KEY_P;
		break;
	case 0x51:
		return MUGA_KEYBOARD_KEY_Q;
		break;
	case 0x52:
		return MUGA_KEYBOARD_KEY_R;
		break;
	case 0x53:
		return MUGA_KEYBOARD_KEY_S;
		break;
	case 0x54:
		return MUGA_KEYBOARD_KEY_T;
		break;
	case 0x55:
		return MUGA_KEYBOARD_KEY_U;
		break;
	case 0x56:
		return MUGA_KEYBOARD_KEY_V;
		break;
	case 0x57:
		return MUGA_KEYBOARD_KEY_W;
		break;
	case 0x58:
		return MUGA_KEYBOARD_KEY_X;
		break;
	case 0x59:
		return MUGA_KEYBOARD_KEY_Y;
		break;
	case 0x5A:
		return MUGA_KEYBOARD_KEY_Z;
		break;
	case VK_LWIN:
		return MUGA_KEYBOARD_KEY_LEFT_WINDOWS;
		break;
	case VK_RWIN:
		return MUGA_KEYBOARD_KEY_RIGHT_WINDOWS;
		break;
	case VK_NUMPAD0:
		return MUGA_KEYBOARD_KEY_NUMPAD_0;
		break;
	case VK_NUMPAD1:
		return MUGA_KEYBOARD_KEY_NUMPAD_1;
		break;
	case VK_NUMPAD2:
		return MUGA_KEYBOARD_KEY_NUMPAD_2;
		break;
	case VK_NUMPAD3:
		return MUGA_KEYBOARD_KEY_NUMPAD_3;
		break;
	case VK_NUMPAD4:
		return MUGA_KEYBOARD_KEY_NUMPAD_4;
		break;
	case VK_NUMPAD5:
		return MUGA_KEYBOARD_KEY_NUMPAD_5;
		break;
	case VK_NUMPAD6:
		return MUGA_KEYBOARD_KEY_NUMPAD_6;
		break;
	case VK_NUMPAD7:
		return MUGA_KEYBOARD_KEY_NUMPAD_7;
		break;
	case VK_NUMPAD8:
		return MUGA_KEYBOARD_KEY_NUMPAD_8;
		break;
	case VK_NUMPAD9:
		return MUGA_KEYBOARD_KEY_NUMPAD_9;
		break;
	case VK_MULTIPLY:
		return MUGA_KEYBOARD_KEY_MULTIPLY;
		break;
	case VK_ADD:
		return MUGA_KEYBOARD_KEY_ADD;
		break;
	case VK_SEPARATOR:
		return MUGA_KEYBOARD_KEY_SEPARATOR;
		break;
	case VK_SUBTRACT:
		return MUGA_KEYBOARD_KEY_SUBTRACT;
		break;
	case VK_DECIMAL:
		return MUGA_KEYBOARD_KEY_DECIMAL;
		break;
	case VK_DIVIDE:
		return MUGA_KEYBOARD_KEY_DIVIDE;
		break;
	case VK_F1:
		return MUGA_KEYBOARD_KEY_F1;
		break;
	case VK_F2:
		return MUGA_KEYBOARD_KEY_F2;
		break;
	case VK_F3:
		return MUGA_KEYBOARD_KEY_F3;
		break;
	case VK_F4:
		return MUGA_KEYBOARD_KEY_F4;
		break;
	case VK_F5:
		return MUGA_KEYBOARD_KEY_F5;
		break;
	case VK_F6:
		return MUGA_KEYBOARD_KEY_F6;
		break;
	case VK_F7:
		return MUGA_KEYBOARD_KEY_F7;
		break;
	case VK_F8:
		return MUGA_KEYBOARD_KEY_F8;
		break;
	case VK_F9:
		return MUGA_KEYBOARD_KEY_F9;
		break;
	case VK_F10:
		return MUGA_KEYBOARD_KEY_F10;
		break;
	case VK_F11:
		return MUGA_KEYBOARD_KEY_F11;
		break;
	case VK_F12:
		return MUGA_KEYBOARD_KEY_F12;
		break;
	case VK_F13:
		return MUGA_KEYBOARD_KEY_F13;
		break;
	case VK_F14:
		return MUGA_KEYBOARD_KEY_F14;
		break;
	case VK_F15:
		return MUGA_KEYBOARD_KEY_F15;
		break;
	case VK_F16:
		return MUGA_KEYBOARD_KEY_F16;
		break;
	case VK_F17:
		return MUGA_KEYBOARD_KEY_F17;
		break;
	case VK_F18:
		return MUGA_KEYBOARD_KEY_F18;
		break;
	case VK_F19:
		return MUGA_KEYBOARD_KEY_F19;
		break;
	case VK_F20:
		return MUGA_KEYBOARD_KEY_F20;
		break;
	case VK_F21:
		return MUGA_KEYBOARD_KEY_F21;
		break;
	case VK_F22:
		return MUGA_KEYBOARD_KEY_F22;
		break;
	case VK_F23:
		return MUGA_KEYBOARD_KEY_F23;
		break;
	case VK_F24:
		return MUGA_KEYBOARD_KEY_F24;
		break;
	case VK_NUMLOCK:
		return MUGA_KEYBOARD_KEY_NUMLOCK;
		break;
	case VK_SCROLL:
		return MUGA_KEYBOARD_KEY_SCROLL;
		break;
	case VK_LSHIFT:
		return MUGA_KEYBOARD_KEY_LEFT_SHIFT;
		break;
	case VK_RSHIFT:
		return MUGA_KEYBOARD_KEY_RIGHT_SHIFT;
		break;
	case VK_LCONTROL:
		return MUGA_KEYBOARD_KEY_LEFT_CONTROL;
		break;
	case VK_RCONTROL:
		return MUGA_KEYBOARD_KEY_RIGHT_CONTROL;
		break;
	case VK_LMENU:
		return MUGA_KEYBOARD_KEY_LEFT_MENU;
		break;
	case VK_RMENU:
		return MUGA_KEYBOARD_KEY_RIGHT_MENU;
		break;
	case VK_ATTN:
		return MUGA_KEYBOARD_KEY_ATTN;
		break;
	case VK_CRSEL:
		return MUGA_KEYBOARD_KEY_CRSEL;
		break;
	case VK_EXSEL:
		return MUGA_KEYBOARD_KEY_EXSEL;
		break;
	case VK_EREOF:
		return MUGA_KEYBOARD_KEY_EREOF;
		break;
	case VK_PLAY:
		return MUGA_KEYBOARD_KEY_PLAY;
		break;
	case VK_PA1:
		return MUGA_KEYBOARD_KEY_PA1;
		break;
	}
}

struct muga_windows_input {
	MUGA_KEY_BIT keyboard_down_status[MUGA_KEYBOARD_LAST-MUGA_KEYBOARD_FIRST+1];
};
typedef struct muga_windows_input muga_windows_input;

void muga_windows_input_set_status(muga_windows_input* input, int windows_key, MUGA_KEY_BIT bit) {
	muga_input_key key = muga_windows_windows_key_to_muga_key(windows_key);
	if (MUGA_IS_KEYBOARD(key)) {
		input->keyboard_down_status[key-MUGA_KEYBOARD_FIRST] = bit;
	}
}

MUGA_KEY_BIT muga_windows_input_get_status(muga_windows_input input, muga_input_key key) {
	if (MUGA_IS_KEYBOARD(key)) {
		return input.keyboard_down_status[key-MUGA_KEYBOARD_FIRST];
	}
	return MUGA_KEY_UP;
}

void muga_windows_input_flush(muga_windows_input* input) {
	for (size_m i = 0; i < MUGA_KEYBOARD_LAST-MUGA_KEYBOARD_FIRST+1; i++) {
		input->keyboard_down_status[i] = MUGA_KEY_UP;
	}
}

/* default window setup */

// basic window vars/structs

struct muga_windows_window {
	// active states whether or not a valid window is here
	MUGA_BOOL active;
	// closed states whether or not the window here has been closed
	MUGA_BOOL closed;
	// visible or invisible
	MUGA_BOOL visible;

	// class name
	wchar_m* class_name;
	// window class information
	WNDCLASSEXW window_class;
	// window handle
	HWND window_handle;
	// device context
	HDC device_context;
	// input
	muga_windows_input input;

	// api
	muga_graphics_api api;
	// opengl context
	MUGA_OPENGL_CALL(HGLRC opengl_context);

	// callbacks
	void (*framebuffer_resize_callback)(muga_window win, int new_width, int new_height);
};
typedef struct muga_windows_window muga_windows_window;

// What a stupid name
muga_windows_window* muga_windows_windows      = MUGA_NULL_PTR;
size_m               muga_windows_windows_length = 0;

MUGA_BOOL muga_windows_window_binded = MUGA_FALSE;
size_m muga_windows_binded_window = 0;

// window funcs

// window proc
// @TODO make render function and a way to bypass "the WM_NCLBUTTONDOWN trap"
// https://www.gamedev.net/forums/topic/488074-win32-message-pump-and-opengl---rendering-pauses-while-draggingresizing/
LRESULT CALLBACK muga_windows_default_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// find window id
	MUGA_BOOL found_window_id = MUGA_FALSE;
	muga_window win;
	for (size_m i = 0; i < muga_windows_windows_length; i++) {
		if (muga_windows_windows[i].active && muga_windows_windows[i].window_handle == hwnd) {
			found_window_id = MUGA_TRUE;
			win = (muga_window)i;
			break;
		}
	}
	if (found_window_id) {
		if (muga_windows_windows[win].closed == MUGA_TRUE) {
			return DefWindowProcW(hwnd, uMsg, wParam, lParam);
		}
		muga_window_set_context(MUGA_NULL_PTR, win);
	}

	// parse msg
	switch (uMsg) {

	default:
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		if (found_window_id) {
			muga_windows_windows[win].closed = MUGA_TRUE;
		}
		return 0;
		break;

	case WM_SIZE:
		if (found_window_id && muga_windows_windows[win].framebuffer_resize_callback != MUGA_NULL_PTR) {
			muga_windows_windows[win].framebuffer_resize_callback(win, (int)LOWORD(lParam), (int)HIWORD(lParam));
		}
		PostMessage(hwnd, WM_PAINT, 0, 0);
		return 0;
		break;

	case WM_KEYDOWN:
		if (found_window_id) {
			muga_windows_input_set_status(&muga_windows_windows[win].input, wParam, MUGA_KEY_DOWN);
		}
		return 0;
		break;

	case WM_KEYUP:
		if (found_window_id) {
			muga_windows_input_set_status(&muga_windows_windows[win].input, wParam, MUGA_KEY_UP);
		}
		return 0;
		break;

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
	if (win >= muga_windows_windows_length ||
		muga_windows_windows[win].active == MUGA_FALSE //||
		//muga_windows_windows[win].closed == MUGA_TRUE
	) {
		return MUGA_FALSE;
	}
	return MUGA_TRUE;
}

MUGA_BOOL muga_windows_is_id_valid_closed(muga_window win) {
	if (win >= muga_windows_windows_length ||
		muga_windows_windows[win].active == MUGA_FALSE
	) {
		return MUGA_FALSE;
	}
	return MUGA_TRUE;
}

void muga_windows_unbind() {
	if (muga_windows_window_binded && muga_windows_is_id_valid(muga_windows_binded_window)) {
		if (MUGA_IS_OPENGL(muga_windows_windows[muga_windows_binded_window].api)) {
			MUGA_OPENGL_CALL(wglMakeCurrent(NULL, NULL));
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
		MUGA_OPENGL_CALL(wglMakeCurrent(muga_windows_windows[win].device_context, muga_windows_windows[win].opengl_context));
	}
	muga_windows_window_binded = MUGA_TRUE;
	muga_windows_binded_window = win;
}

// https://stackoverflow.com/questions/1695288/getting-the-current-time-in-milliseconds-from-the-system-clock-in-windows
double muga_windows_get_current_time() {
	FILETIME file_time;
	GetSystemTimeAsFileTime(&file_time);
	LONGLONG ll_now = (LONGLONG)file_time.dwLowDateTime + ((LONGLONG)(file_time.dwHighDateTime) << 32LL);
	return (double)ll_now / (double)1.0e7;
}

/* initiation/termination */

MUGA_BOOL muga_windows_has_initiated = MUGA_FALSE;
double muga_windows_original_time = 0.f;

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
	muga_windows_windows[0].framebuffer_resize_callback = MUGA_NULL_PTR;

	muga_windows_original_time = muga_windows_get_current_time();

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

	muga_windows_unbind();

	for (size_m i = 0; i < muga_windows_windows_length; i++) {
		if (muga_windows_windows[i].active) {
			muga_window_destroy(MUGA_NULL_PTR, i);
		}
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

MUGADEF double muga_time_get(MUGA_RESULT* result) {
	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	return muga_windows_get_current_time() - muga_windows_original_time;
}

MUGADEF void muga_time_set(MUGA_RESULT* result, double time) {
	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	muga_windows_original_time = muga_windows_original_time + muga_time_get(result) - time;
}

/* basic window funcs */

MUGADEF muga_window muga_window_create(MUGA_RESULT* result, muga_graphics_api api, MUGA_BOOL (*load_functions)(void), const wchar_m* name, unsigned int width, unsigned int height) {
	muga_windows_unbind();

	// allocate class name
	muga_window win = muga_windows_get_new_window_id();
	wchar_m* class_name = muga_malloc(sizeof(wchar_m) * 2);
	class_name[0] = '!' + win;
	class_name[1] = '\0';

	// initialize muga_windows_window struct

	muga_windows_window window_struct = {
		.active = MUGA_FALSE,
		.closed = MUGA_FALSE,
		// (WNDCLASSEXW)
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
			.lpszClassName = class_name,                         // class name
			.hIconSm =       0                                   // small window icon
		},
		.framebuffer_resize_callback = MUGA_NULL_PTR
	};
	if (!RegisterClassExW(&window_struct.window_class)) {
		muga_print("[MUGA] Failed to register window class.\n");
		muga_free(class_name);
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return MUGA_NO_WINDOW;
	}

	// create window

	DWORD style = WS_OVERLAPPEDWINDOW;
	if (!muga_window_settings.resizable) {
		style = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
	}

	window_struct.window_handle = CreateWindowExW(
		// @TODO make default position customizable
		0,                                    // extra window style
		class_name,                           // class name
		name,                                 // window name
		style,                                // window style
		muga_window_settings.x,               // x-position
		muga_window_settings.y,               // y-position
		width,                                // width
		height,                               // height
		NULL,                                 // parent window
		NULL,                                 // menu (?)
		window_struct.window_class.hInstance, // hInstance
		NULL                                  // additional stuff
	);

	if (window_struct.window_handle == NULL) {
		muga_print("[MUGA] Failed to create window.\n");
		muga_free(class_name);
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return MUGA_NO_WINDOW;
	}

	// assign window into window array
	muga_windows_windows[win] = window_struct;
	muga_windows_windows[win].input = (muga_windows_input){0};

	// get some more info
	muga_windows_windows[win].device_context = GetDC(muga_windows_windows[win].window_handle);

	// make api context

	// @TODO make option to share contexts (child windows?)

	muga_windows_windows[win].api = api;
	// opengl
	if (MUGA_IS_OPENGL(api)) {
		MUGA_OPENGL_CALL(
			if (!muga_windows_create_opengl_context(
					muga_windows_windows[win].device_context,
					&muga_windows_windows[win].opengl_context,
					api
				)
			) {
				if (result != MUGA_NULL_PTR) {
					*result = MUGA_FAILURE;
				}
				ReleaseDC(
					muga_windows_windows[win].window_handle,
					muga_windows_windows[win].device_context
				);
				DestroyWindow(muga_windows_windows[win].window_handle);
				muga_free(class_name);
				return MUGA_NO_WINDOW;
			}
		);
	// no api
	} else if (api != MUGA_NO_GRAPHICS_API) {
		muga_print("[MUGA] Unsupported/Excluded (#define MUGA_NO_...) graphics API for Windows.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		ReleaseDC(
			muga_windows_windows[win].window_handle,
			muga_windows_windows[win].device_context
		);
		DestroyWindow(muga_windows_windows[win].window_handle);
		muga_free(class_name);
		return MUGA_NO_WINDOW;
	}

	if (load_functions != MUGA_NULL_PTR) {
	    if (!load_functions()) {
	    	muga_print("[MUGA] Failed to load functions for graphics API.\n");
	    	if (result != MUGA_NULL_PTR) {
				*result = MUGA_FAILURE;
			}
			ReleaseDC(
				muga_windows_windows[win].window_handle,
				muga_windows_windows[win].device_context
			);
			if (MUGA_IS_OPENGL(muga_windows_windows[win].api)) {
				MUGA_OPENGL_CALL(wglDeleteContext(muga_windows_windows[win].opengl_context));
			}
			DestroyWindow(muga_windows_windows[win].window_handle);
			muga_free(class_name);
			return MUGA_NO_WINDOW;
	    }
	}

	muga_windows_windows[win].class_name = class_name;
	muga_windows_windows[win].active = MUGA_TRUE;

	muga_windows_bind(win);

	muga_windows_windows[win].visible = muga_window_settings.visible;
	if (muga_windows_windows[win].visible) {
		ShowWindow(muga_windows_windows[win].window_handle, SW_NORMAL);
	} else {
		ShowWindow(muga_windows_windows[win].window_handle, SW_HIDE);
	}

	UpdateWindow(muga_windows_windows[win].window_handle);

	// return
	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	return win;
}

MUGADEF void muga_window_destroy(MUGA_RESULT* result, muga_window win) {
	if (!muga_windows_is_id_valid_closed(win)) {
		muga_print("[MUGA] Requested window ID for destruction is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	// unregister class
	UnregisterClassW(muga_windows_windows[win].class_name, muga_windows_windows[win].window_class.hInstance);
	muga_free(muga_windows_windows[win].class_name);

	// destroy context and window
	if (win == muga_windows_binded_window && muga_windows_window_binded) {
		muga_windows_unbind();
	}
	ReleaseDC(
		muga_windows_windows[win].window_handle,
		muga_windows_windows[win].device_context
	);
	if (MUGA_IS_OPENGL(muga_windows_windows[win].api)) {
		MUGA_OPENGL_CALL(wglDeleteContext(muga_windows_windows[win].opengl_context));
	}
	DestroyWindow(muga_windows_windows[win].window_handle);

	// open up to overriding
	muga_windows_windows[win].active = MUGA_FALSE;
	muga_windows_windows[win].closed = MUGA_FALSE;
	muga_windows_windows[win].framebuffer_resize_callback = MUGA_NULL_PTR;
	muga_windows_input_flush(&muga_windows_windows[win].input);

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_update(MUGA_RESULT* result, muga_window win) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for updating is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	muga_windows_bind(win);

	MSG msg = { 0 };
	while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			//muga_windows_windows[win].alive = MUGA_FALSE;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if (GetFocus() != muga_windows_windows[win].window_handle) {
		muga_windows_input_flush(&muga_windows_windows[win].input);
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
		MUGA_OPENGL_CALL(SwapBuffers(muga_windows_windows[win].device_context));
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF MUGA_BOOL muga_window_get_closed(MUGA_RESULT* result, muga_window win) {
	if (!muga_windows_is_id_valid_closed(win)) {
		muga_print("[MUGA] Requested window ID for checking if closed is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return MUGA_TRUE;
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	return muga_windows_windows[win].closed;
}

MUGADEF void muga_window_close(MUGA_RESULT* result, muga_window win) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for closing is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	muga_window_set_visible(result, win, MUGA_FALSE);
	muga_windows_windows[win].closed = MUGA_TRUE;

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF MUGA_BOOL muga_window_get_focused(MUGA_RESULT* result, muga_window win) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for getting focused state is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return MUGA_FALSE;
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	return GetFocus() == muga_windows_windows[win].window_handle;
}

MUGADEF void muga_window_focus(MUGA_RESULT* result, muga_window win) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for focusing is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	// https://stackoverflow.com/questions/71437203/proper-way-of-activating-a-window-using-winapi
	// a better approach is possible, but to do so i would need the automation api
	// and i'm not touching that with a ten foot pole
	SetForegroundWindow(muga_windows_windows[win].window_handle);
	if (GetForegroundWindow() != muga_windows_windows[win].window_handle) {
		SwitchToThisWindow(muga_windows_windows[win].window_handle, MUGA_TRUE);
		Sleep(2);
		SetForegroundWindow(muga_windows_windows[win].window_handle);
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_get_position(MUGA_RESULT* result, muga_window win, int* x, int* y) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for getting position is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	RECT rect = {0};
	GetWindowRect(muga_windows_windows[win].window_handle, &rect);

	if (x != MUGA_NULL_PTR) {
		*x = (int)rect.left;
	}
	if (y != MUGA_NULL_PTR) {
		*y = (int)rect.top;
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_set_position(MUGA_RESULT* result, muga_window win, int x, int y) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for setting position is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	SetWindowPos(muga_windows_windows[win].window_handle, HWND_TOP, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE );

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_get_dimensions(MUGA_RESULT* result, muga_window win, unsigned int* width, unsigned int* height) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for getting dimensions is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	RECT rect = {0};
	GetWindowRect(muga_windows_windows[win].window_handle, &rect);

	if (width != MUGA_NULL_PTR) {
		*width = (int)(rect.right - rect.left);
	}
	if (height != MUGA_NULL_PTR) {
		*height = (int)(rect.bottom - rect.top);
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_set_dimensions(MUGA_RESULT* result, muga_window win, unsigned int width, unsigned int height) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for setting dimensions is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	SetWindowPos(muga_windows_windows[win].window_handle, HWND_TOP, 0, 0, width, height, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
	
	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF MUGA_BOOL muga_window_get_maximized(MUGA_RESULT* result, muga_window win) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for getting maximized state is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return MUGA_FALSE;
	}

	WINDOWPLACEMENT wndpl = {0};
	wndpl.length = sizeof(wndpl);
	GetWindowPlacement(muga_windows_windows[win].window_handle, &wndpl);

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	return wndpl.showCmd == SW_MAXIMIZE;
}

MUGADEF void muga_window_set_maximized(MUGA_RESULT* result, muga_window win, MUGA_BOOL maximized) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for setting maximized state is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	if (muga_window_get_maximized(result, win) == maximized) return;

	if (maximized) {
		ShowWindow(muga_windows_windows[win].window_handle, SW_MAXIMIZE);
	} else {
		ShowWindow(muga_windows_windows[win].window_handle, SW_NORMAL);
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_set_context(MUGA_RESULT* result, muga_window win) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for setting context is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	muga_windows_bind(win);

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_set_title(MUGA_RESULT* result, muga_window win, const wchar_m* title) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for setting title is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	SetWindowTextW(muga_windows_windows[win].window_handle, title);

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF MUGA_BOOL muga_window_get_visible(MUGA_RESULT* result, muga_window win) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for getting visibility is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return MUGA_FALSE;
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	return muga_windows_windows[win].visible;
}

MUGADEF void muga_window_set_visible(MUGA_RESULT* result, muga_window win, MUGA_BOOL visible) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for setting visibility is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	if (visible == MUGA_FALSE && muga_windows_windows[win].visible == MUGA_TRUE) {
		ShowWindow(muga_windows_windows[win].window_handle, SW_HIDE);
	} else if (visible == MUGA_TRUE && muga_windows_windows[win].visible == MUGA_FALSE) {
		ShowWindow(muga_windows_windows[win].window_handle, SW_NORMAL);
	}

	muga_windows_windows[win].visible = visible;

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF MUGA_KEY_BIT muga_window_get_input_bit(MUGA_RESULT* result, muga_window win, muga_input_method method, muga_input_key key) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for getting input bit is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return MUGA_KEY_UP;
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	return muga_windows_input_get_status(muga_windows_windows[win].input, key);
}

MUGADEF void muga_window_set_framebuffer_resize_callback(MUGA_RESULT* result, muga_window win, void (*framebuffer_resize_callback)(muga_window win, int new_width, int new_height)) {
	if (!muga_windows_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for setting framebuffer resize callback is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	muga_windows_bind(win);
	muga_windows_windows[win].framebuffer_resize_callback = framebuffer_resize_callback;

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

/* opengl functions */

// https://stackoverflow.com/questions/76638441/how-to-init-glad-without-the-glfw-loader-using-windows-headers
MUGADEF void* muga_get_opengl_function_address(const char* name) {
	void *p = (void *)wglGetProcAddress(name);

	if (p == 0 ||
	   (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
	   (p == (void*)-1)) {
		HMODULE module = LoadLibraryA("opengl32.dll"); 
		p = (void *)GetProcAddress(module, name);
	}

	return p;
}

#endif /* WINDOWS */

#ifdef linux
#define MUGA_LINUX

#define XK_3270
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>

#include <time.h>

/* OPENGL SETUP */
#ifndef MUGA_NO_OPENGL

// this kinda doesn't make sense
#ifndef MUGA_NO_INCLUDE_OPENGL
	#ifndef glClearColor
		#include <GL/gl.h>
	#endif
#endif
#include <GL/glx.h>

#define MUGA_OPENGL_CALL(stuff) stuff

// context creation
// https://apoorvaj.io/creating-a-modern-opengl-context/
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
MUGA_BOOL muga_linux_opengl_initiated = MUGA_FALSE;
MUGA_RESULT muga_linux_init_opengl(Display* display, GLXContext* context, muga_graphics_api api) {
	if (muga_linux_opengl_initiated) {
		return MUGA_SUCCESS;
	}

	// choose framebuffer pixel format stuff

	// @TODO customizability here
	static int pixel_format_attributes[] = {
		GLX_RENDER_TYPE,   GLX_RGBA_BIT,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_DOUBLEBUFFER,  MUGA_TRUE,
		GLX_RED_SIZE,      1,
		GLX_GREEN_SIZE,    1,
		GLX_BLUE_SIZE,     1,
		None
	};

	int fbc_count = 0;
	GLXFBConfig* fbc = glXChooseFBConfig(
		display, 
		DefaultScreen(display),
		pixel_format_attributes,
		&fbc_count
	);

	if (!fbc) {
		muga_print("[MUGA] Failed to find compatible framebuffer configuration.\n");
		return MUGA_FAILURE;
	}

	// create context

	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
	glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");

	if (!glXCreateContextAttribsARB) {
		muga_print("[MUGA] Failed to find glX function to create an OpenGL context.\n");
		return MUGA_FAILURE;
	}

	static int opengl_attributes[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 3,
		GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
		None
	};

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
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_3_2_COMPATIBILITY:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 2;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_3_3_CORE:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_3_3_COMPATIBILITY:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_0_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 0;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_0_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 0;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_1_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 1;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_1_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 1;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_2_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 2;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_2_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 2;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_3_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_3_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_4_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 4;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_4_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 4;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_5_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 5;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_5_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 5;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_6_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 6;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MUGA_OPENGL_4_6_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 6;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
	}

	*context = glXCreateContextAttribsARB(display, fbc[0], MUGA_NULL, MUGA_TRUE, opengl_attributes);
	if (!*context) {
		muga_print("[MUGA] Failed to create valid OpenGL context.\n");
		return MUGA_FAILURE;
	}

	//muga_linux_opengl_initiated = MUGA_SUCCESS;
	return MUGA_SUCCESS;
}

#else

#define MUGA_OPENGL_CALL(...)

#endif /* MUGA_NO_OPENGL */

/* keyboard input */

// https://www.cl.cam.ac.uk/~mgk25/ucs/keysymdef.h
int muga_linux_muga_key_to_linux_key(muga_input_key key) {
	switch (key) {
	default:
		return XK_VoidSymbol;
		break;
	case MUGA_KEYBOARD_KEY_BACKSPACE:
		return XK_BackSpace;
		break;
	case MUGA_KEYBOARD_KEY_TAB:
		return XK_Tab;
		break;
	case MUGA_KEYBOARD_KEY_CLEAR:
		return XK_Clear;
		break;
	case MUGA_KEYBOARD_KEY_RETURN:
		return XK_Return;
		break;
	case MUGA_KEYBOARD_KEY_PAUSE:
		return XK_Pause;
		break;
	case MUGA_KEYBOARD_KEY_ESCAPE:
		return XK_Escape;
		break;
	case MUGA_KEYBOARD_KEY_MODECHANGE:
		return XK_Mode_switch;
		break;
	case MUGA_KEYBOARD_KEY_SPACE:
		return XK_space;
		break;
	case MUGA_KEYBOARD_KEY_PRIOR:
		return XK_Prior;
		break;
	case MUGA_KEYBOARD_KEY_NEXT:
		return XK_Next;
		break;
	case MUGA_KEYBOARD_KEY_END:
		return XK_End;
		break;
	case MUGA_KEYBOARD_KEY_HOME:
		return XK_Home;
		break;
	case MUGA_KEYBOARD_KEY_LEFT:
		return XK_Left;
		break;
	case MUGA_KEYBOARD_KEY_UP:
		return XK_Up;
		break;
	case MUGA_KEYBOARD_KEY_RIGHT:
		return XK_Right;
		break;
	case MUGA_KEYBOARD_KEY_DOWN:
		return XK_Down;
		break;
	case MUGA_KEYBOARD_KEY_SELECT:
		return XK_Select;
		break;
	case MUGA_KEYBOARD_KEY_PRINT:
		return XK_Print;
		break;
	case MUGA_KEYBOARD_KEY_EXECUTE:
		return XK_Execute;
		break;
	case MUGA_KEYBOARD_KEY_INSERT:
		return XK_Insert;
		break;
	case MUGA_KEYBOARD_KEY_DELETE:
		return XK_Delete;
		break;
	case MUGA_KEYBOARD_KEY_HELP:
		return XK_Help;
		break;
	case MUGA_KEYBOARD_KEY_0:
		return XK_0;
		break;
	case MUGA_KEYBOARD_KEY_1:
		return XK_1;
		break;
	case MUGA_KEYBOARD_KEY_2:
		return XK_2;
		break;
	case MUGA_KEYBOARD_KEY_3:
		return XK_3;
		break;
	case MUGA_KEYBOARD_KEY_4:
		return XK_4;
		break;
	case MUGA_KEYBOARD_KEY_5:
		return XK_5;
		break;
	case MUGA_KEYBOARD_KEY_6:
		return XK_6;
		break;
	case MUGA_KEYBOARD_KEY_7:
		return XK_7;
		break;
	case MUGA_KEYBOARD_KEY_8:
		return XK_8;
		break;
	case MUGA_KEYBOARD_KEY_9:
		return XK_9;
		break;
	case MUGA_KEYBOARD_KEY_A:
		return XK_a;
		break;
	case MUGA_KEYBOARD_KEY_B:
		return XK_b;
		break;
	case MUGA_KEYBOARD_KEY_C:
		return XK_c;
		break;
	case MUGA_KEYBOARD_KEY_D:
		return XK_d;
		break;
	case MUGA_KEYBOARD_KEY_E:
		return XK_e;
		break;
	case MUGA_KEYBOARD_KEY_F:
		return XK_f;
		break;
	case MUGA_KEYBOARD_KEY_G:
		return XK_g;
		break;
	case MUGA_KEYBOARD_KEY_H:
		return XK_h;
		break;
	case MUGA_KEYBOARD_KEY_I:
		return XK_i;
		break;
	case MUGA_KEYBOARD_KEY_J:
		return XK_j;
		break;
	case MUGA_KEYBOARD_KEY_K:
		return XK_k;
		break;
	case MUGA_KEYBOARD_KEY_L:
		return XK_l;
		break;
	case MUGA_KEYBOARD_KEY_M:
		return XK_m;
		break;
	case MUGA_KEYBOARD_KEY_N:
		return XK_n;
		break;
	case MUGA_KEYBOARD_KEY_O:
		return XK_o;
		break;
	case MUGA_KEYBOARD_KEY_P:
		return XK_p;
		break;
	case MUGA_KEYBOARD_KEY_Q:
		return XK_q;
		break;
	case MUGA_KEYBOARD_KEY_R:
		return XK_r;
		break;
	case MUGA_KEYBOARD_KEY_S:
		return XK_s;
		break;
	case MUGA_KEYBOARD_KEY_T:
		return XK_t;
		break;
	case MUGA_KEYBOARD_KEY_U:
		return XK_u;
		break;
	case MUGA_KEYBOARD_KEY_V:
		return XK_v;
		break;
	case MUGA_KEYBOARD_KEY_W:
		return XK_w;
		break;
	case MUGA_KEYBOARD_KEY_X:
		return XK_x;
		break;
	case MUGA_KEYBOARD_KEY_Y:
		return XK_y;
		break;
	case MUGA_KEYBOARD_KEY_Z:
		return XK_z;
		break;
	case MUGA_KEYBOARD_KEY_LEFT_WINDOWS:
		return XK_Super_L;
		break;
	case MUGA_KEYBOARD_KEY_RIGHT_WINDOWS:
		return XK_Super_R;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_0:
		return XK_KP_0;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_1:
		return XK_KP_1;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_2:
		return XK_KP_2;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_3:
		return XK_KP_3;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_4:
		return XK_KP_4;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_5:
		return XK_KP_5;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_6:
		return XK_KP_6;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_7:
		return XK_KP_7;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_8:
		return XK_KP_8;
		break;
	case MUGA_KEYBOARD_KEY_NUMPAD_9:
		return XK_KP_9;
		break;
	case MUGA_KEYBOARD_KEY_MULTIPLY:
		return XK_KP_Multiply;
		break;
	case MUGA_KEYBOARD_KEY_ADD:
		return XK_KP_Add;
		break;
	case MUGA_KEYBOARD_KEY_SEPARATOR:
		return XK_KP_Separator;
		break;
	case MUGA_KEYBOARD_KEY_SUBTRACT:
		return XK_KP_Subtract;
		break;
	case MUGA_KEYBOARD_KEY_DECIMAL:
		return XK_KP_Decimal;
		break;
	case MUGA_KEYBOARD_KEY_DIVIDE:
		return XK_KP_Divide;
		break;
	case MUGA_KEYBOARD_KEY_F1:
		return XK_F1;
		break;
	case MUGA_KEYBOARD_KEY_F2:
		return XK_F2;
		break;
	case MUGA_KEYBOARD_KEY_F3:
		return XK_F3;
		break;
	case MUGA_KEYBOARD_KEY_F4:
		return XK_F4;
		break;
	case MUGA_KEYBOARD_KEY_F5:
		return XK_F5;
		break;
	case MUGA_KEYBOARD_KEY_F6:
		return XK_F6;
		break;
	case MUGA_KEYBOARD_KEY_F7:
		return XK_F7;
		break;
	case MUGA_KEYBOARD_KEY_F8:
		return XK_F8;
		break;
	case MUGA_KEYBOARD_KEY_F9:
		return XK_F9;
		break;
	case MUGA_KEYBOARD_KEY_F10:
		return XK_F10;
		break;
	case MUGA_KEYBOARD_KEY_F11:
		return XK_F11;
		break;
	case MUGA_KEYBOARD_KEY_F12:
		return XK_F12;
		break;
	case MUGA_KEYBOARD_KEY_F13:
		return XK_F13;
		break;
	case MUGA_KEYBOARD_KEY_F14:
		return XK_F14;
		break;
	case MUGA_KEYBOARD_KEY_F15:
		return XK_F15;
		break;
	case MUGA_KEYBOARD_KEY_F16:
		return XK_F16;
		break;
	case MUGA_KEYBOARD_KEY_F17:
		return XK_F17;
		break;
	case MUGA_KEYBOARD_KEY_F18:
		return XK_F18;
		break;
	case MUGA_KEYBOARD_KEY_F19:
		return XK_F19;
		break;
	case MUGA_KEYBOARD_KEY_F20:
		return XK_F20;
		break;
	case MUGA_KEYBOARD_KEY_F21:
		return XK_F21;
		break;
	case MUGA_KEYBOARD_KEY_F22:
		return XK_F22;
		break;
	case MUGA_KEYBOARD_KEY_F23:
		return XK_F23;
		break;
	case MUGA_KEYBOARD_KEY_F24:
		return XK_F24;
		break;
	case MUGA_KEYBOARD_KEY_NUMLOCK:
		return XK_Num_Lock;
		break;
	case MUGA_KEYBOARD_KEY_SCROLL:
		return XK_Scroll_Lock;
		break;
	case MUGA_KEYBOARD_KEY_LEFT_SHIFT:
		return XK_Shift_L;
		break;
	case MUGA_KEYBOARD_KEY_RIGHT_SHIFT:
		return XK_Shift_R;
		break;
	case MUGA_KEYBOARD_KEY_LEFT_CONTROL:
		return XK_Control_L;
		break;
	case MUGA_KEYBOARD_KEY_RIGHT_CONTROL:
		return XK_Control_R;
		break;
	case MUGA_KEYBOARD_KEY_LEFT_MENU:
		return XK_Menu;
		break;
	case MUGA_KEYBOARD_KEY_RIGHT_MENU:
		return XK_Menu;
		break;
	case MUGA_KEYBOARD_KEY_ATTN:
		return XK_3270_Attn;
		break;
	case MUGA_KEYBOARD_KEY_CRSEL:
		return XK_3270_CursorSelect;
		break;
	case MUGA_KEYBOARD_KEY_EXSEL:
		return XK_3270_ExSelect;
		break;
	case MUGA_KEYBOARD_KEY_EREOF:
		return XK_3270_EraseEOF;
		break;
	case MUGA_KEYBOARD_KEY_PLAY:
		return XK_3270_Play;
		break;
	case MUGA_KEYBOARD_KEY_PA1:
		return XK_3270_PA1;
		break;
	}
}

muga_input_key muga_linux_linux_key_to_muga_key(int key) {
	switch (key) {
	default:
		return MUGA_NO_INPUT_KEY;
		break;
	case XK_BackSpace:
		return MUGA_KEYBOARD_KEY_BACKSPACE;
		break;
	case XK_Tab:
		return MUGA_KEYBOARD_KEY_TAB;
		break;
	case XK_Clear:
		return MUGA_KEYBOARD_KEY_CLEAR;
		break;
	case XK_Return:
		return MUGA_KEYBOARD_KEY_RETURN;
		break;
	case XK_Menu:
		return MUGA_KEYBOARD_KEY_LEFT_MENU;
		break;
	case XK_Pause:
		return MUGA_KEYBOARD_KEY_PAUSE;
		break;
	case XK_Escape:
		return MUGA_KEYBOARD_KEY_ESCAPE;
		break;
	case XK_Mode_switch:
		return MUGA_KEYBOARD_KEY_MODECHANGE;
		break;
	case XK_space:
		return MUGA_KEYBOARD_KEY_SPACE;
		break;
	case XK_Prior:
		return MUGA_KEYBOARD_KEY_PRIOR;
		break;
	case XK_Next:
		return MUGA_KEYBOARD_KEY_NEXT;
		break;
	case XK_End:
		return MUGA_KEYBOARD_KEY_END;
		break;
	case XK_Home:
		return MUGA_KEYBOARD_KEY_HOME;
		break;
	case XK_Left:
		return MUGA_KEYBOARD_KEY_LEFT;
		break;
	case XK_Up:
		return MUGA_KEYBOARD_KEY_UP;
		break;
	case XK_Right:
		return MUGA_KEYBOARD_KEY_RIGHT;
		break;
	case XK_Down:
		return MUGA_KEYBOARD_KEY_DOWN;
		break;
	case XK_Select:
		return MUGA_KEYBOARD_KEY_SELECT;
		break;
	case XK_Print:
		return MUGA_KEYBOARD_KEY_PRINT;
		break;
	case XK_Execute:
		return MUGA_KEYBOARD_KEY_EXECUTE;
		break;
	case XK_Insert:
		return MUGA_KEYBOARD_KEY_INSERT;
		break;
	case XK_Delete:
		return MUGA_KEYBOARD_KEY_DELETE;
		break;
	case XK_Help:
		return MUGA_KEYBOARD_KEY_HELP;
		break;
	case XK_0:
		return MUGA_KEYBOARD_KEY_0;
		break;
	case XK_1:
		return MUGA_KEYBOARD_KEY_1;
		break;
	case XK_2:
		return MUGA_KEYBOARD_KEY_2;
		break;
	case XK_3:
		return MUGA_KEYBOARD_KEY_3;
		break;
	case XK_4:
		return MUGA_KEYBOARD_KEY_4;
		break;
	case XK_5:
		return MUGA_KEYBOARD_KEY_5;
		break;
	case XK_6:
		return MUGA_KEYBOARD_KEY_6;
		break;
	case XK_7:
		return MUGA_KEYBOARD_KEY_7;
		break;
	case XK_8:
		return MUGA_KEYBOARD_KEY_8;
		break;
	case XK_9:
		return MUGA_KEYBOARD_KEY_9;
		break;
	case XK_a:
		return MUGA_KEYBOARD_KEY_A;
		break;
	case XK_b:
		return MUGA_KEYBOARD_KEY_B;
		break;
	case XK_c:
		return MUGA_KEYBOARD_KEY_C;
		break;
	case XK_d:
		return MUGA_KEYBOARD_KEY_D;
		break;
	case XK_e:
		return MUGA_KEYBOARD_KEY_E;
		break;
	case XK_f:
		return MUGA_KEYBOARD_KEY_F;
		break;
	case XK_g:
		return MUGA_KEYBOARD_KEY_G;
		break;
	case XK_h:
		return MUGA_KEYBOARD_KEY_H;
		break;
	case XK_i:
		return MUGA_KEYBOARD_KEY_I;
		break;
	case XK_j:
		return MUGA_KEYBOARD_KEY_J;
		break;
	case XK_k:
		return MUGA_KEYBOARD_KEY_K;
		break;
	case XK_l:
		return MUGA_KEYBOARD_KEY_L;
		break;
	case XK_m:
		return MUGA_KEYBOARD_KEY_M;
		break;
	case XK_n:
		return MUGA_KEYBOARD_KEY_N;
		break;
	case XK_o:
		return MUGA_KEYBOARD_KEY_O;
		break;
	case XK_p:
		return MUGA_KEYBOARD_KEY_P;
		break;
	case XK_q:
		return MUGA_KEYBOARD_KEY_Q;
		break;
	case XK_r:
		return MUGA_KEYBOARD_KEY_R;
		break;
	case XK_s:
		return MUGA_KEYBOARD_KEY_S;
		break;
	case XK_t:
		return MUGA_KEYBOARD_KEY_T;
		break;
	case XK_u:
		return MUGA_KEYBOARD_KEY_U;
		break;
	case XK_v:
		return MUGA_KEYBOARD_KEY_V;
		break;
	case XK_w:
		return MUGA_KEYBOARD_KEY_W;
		break;
	case XK_x:
		return MUGA_KEYBOARD_KEY_X;
		break;
	case XK_y:
		return MUGA_KEYBOARD_KEY_Y;
		break;
	case XK_z:
		return MUGA_KEYBOARD_KEY_Z;
		break;
	case XK_Super_L:
		return MUGA_KEYBOARD_KEY_LEFT_WINDOWS;
		break;
	case XK_Super_R:
		return MUGA_KEYBOARD_KEY_RIGHT_WINDOWS;
		break;
	case XK_KP_0:
		return MUGA_KEYBOARD_KEY_NUMPAD_0;
		break;
	case XK_KP_1:
		return MUGA_KEYBOARD_KEY_NUMPAD_1;
		break;
	case XK_KP_2:
		return MUGA_KEYBOARD_KEY_NUMPAD_2;
		break;
	case XK_KP_3:
		return MUGA_KEYBOARD_KEY_NUMPAD_3;
		break;
	case XK_KP_4:
		return MUGA_KEYBOARD_KEY_NUMPAD_4;
		break;
	case XK_KP_5:
		return MUGA_KEYBOARD_KEY_NUMPAD_5;
		break;
	case XK_KP_6:
		return MUGA_KEYBOARD_KEY_NUMPAD_6;
		break;
	case XK_KP_7:
		return MUGA_KEYBOARD_KEY_NUMPAD_7;
		break;
	case XK_KP_8:
		return MUGA_KEYBOARD_KEY_NUMPAD_8;
		break;
	case XK_KP_9:
		return MUGA_KEYBOARD_KEY_NUMPAD_9;
		break;
	case XK_KP_Multiply:
		return MUGA_KEYBOARD_KEY_MULTIPLY;
		break;
	case XK_KP_Add:
		return MUGA_KEYBOARD_KEY_ADD;
		break;
	case XK_KP_Separator:
		return MUGA_KEYBOARD_KEY_SEPARATOR;
		break;
	case XK_KP_Subtract:
		return MUGA_KEYBOARD_KEY_SUBTRACT;
		break;
	case XK_KP_Decimal:
		return MUGA_KEYBOARD_KEY_DECIMAL;
		break;
	case XK_KP_Divide:
		return MUGA_KEYBOARD_KEY_DIVIDE;
		break;
	case XK_F1:
		return MUGA_KEYBOARD_KEY_F1;
		break;
	case XK_F2:
		return MUGA_KEYBOARD_KEY_F2;
		break;
	case XK_F3:
		return MUGA_KEYBOARD_KEY_F3;
		break;
	case XK_F4:
		return MUGA_KEYBOARD_KEY_F4;
		break;
	case XK_F5:
		return MUGA_KEYBOARD_KEY_F5;
		break;
	case XK_F6:
		return MUGA_KEYBOARD_KEY_F6;
		break;
	case XK_F7:
		return MUGA_KEYBOARD_KEY_F7;
		break;
	case XK_F8:
		return MUGA_KEYBOARD_KEY_F8;
		break;
	case XK_F9:
		return MUGA_KEYBOARD_KEY_F9;
		break;
	case XK_F10:
		return MUGA_KEYBOARD_KEY_F10;
		break;
	case XK_F11:
		return MUGA_KEYBOARD_KEY_F11;
		break;
	case XK_F12:
		return MUGA_KEYBOARD_KEY_F12;
		break;
	case XK_F13:
		return MUGA_KEYBOARD_KEY_F13;
		break;
	case XK_F14:
		return MUGA_KEYBOARD_KEY_F14;
		break;
	case XK_F15:
		return MUGA_KEYBOARD_KEY_F15;
		break;
	case XK_F16:
		return MUGA_KEYBOARD_KEY_F16;
		break;
	case XK_F17:
		return MUGA_KEYBOARD_KEY_F17;
		break;
	case XK_F18:
		return MUGA_KEYBOARD_KEY_F18;
		break;
	case XK_F19:
		return MUGA_KEYBOARD_KEY_F19;
		break;
	case XK_F20:
		return MUGA_KEYBOARD_KEY_F20;
		break;
	case XK_F21:
		return MUGA_KEYBOARD_KEY_F21;
		break;
	case XK_F22:
		return MUGA_KEYBOARD_KEY_F22;
		break;
	case XK_F23:
		return MUGA_KEYBOARD_KEY_F23;
		break;
	case XK_F24:
		return MUGA_KEYBOARD_KEY_F24;
		break;
	case XK_Num_Lock:
		return MUGA_KEYBOARD_KEY_NUMLOCK;
		break;
	case XK_Scroll_Lock:
		return MUGA_KEYBOARD_KEY_SCROLL;
		break;
	case XK_Shift_L:
		return MUGA_KEYBOARD_KEY_LEFT_SHIFT;
		break;
	case XK_Shift_R:
		return MUGA_KEYBOARD_KEY_RIGHT_SHIFT;
		break;
	case XK_Control_L:
		return MUGA_KEYBOARD_KEY_LEFT_CONTROL;
		break;
	case XK_Control_R:
		return MUGA_KEYBOARD_KEY_RIGHT_CONTROL;
		break;
	case XK_3270_Attn:
		return MUGA_KEYBOARD_KEY_ATTN;
		break;
	case XK_3270_CursorSelect:
		return MUGA_KEYBOARD_KEY_CRSEL;
		break;
	case XK_3270_ExSelect:
		return MUGA_KEYBOARD_KEY_EXSEL;
		break;
	case XK_3270_EraseEOF:
		return MUGA_KEYBOARD_KEY_EREOF;
		break;
	case XK_3270_Play:
		return MUGA_KEYBOARD_KEY_PLAY;
		break;
	case XK_3270_PA1:
		return MUGA_KEYBOARD_KEY_PA1;
		break;
	}
}

struct muga_linux_input {
	MUGA_KEY_BIT keyboard_down_status[MUGA_KEYBOARD_LAST-MUGA_KEYBOARD_FIRST+1];
};
typedef struct muga_linux_input muga_linux_input;

void muga_linux_input_set_status(muga_linux_input* input, int linux_key, MUGA_KEY_BIT bit) {
	muga_input_key key = muga_linux_linux_key_to_muga_key(linux_key);
	if (MUGA_IS_KEYBOARD(key)) {
		input->keyboard_down_status[key-MUGA_KEYBOARD_FIRST] = bit;
	}
}

MUGA_KEY_BIT muga_linux_input_get_status(muga_linux_input input, muga_input_key key) {
	if (MUGA_IS_KEYBOARD(key)) {
		return input.keyboard_down_status[key-MUGA_KEYBOARD_FIRST];
	}
	return MUGA_KEY_UP;
}

void muga_linux_input_flush(muga_linux_input* input) {
	for (size_m i = 0; i < MUGA_KEYBOARD_LAST-MUGA_KEYBOARD_FIRST+1; i++) {
		input->keyboard_down_status[i] = MUGA_KEY_UP;
	}
}

/* window setup */

struct muga_linux_window {
	MUGA_BOOL active;
	MUGA_BOOL closed;
	MUGA_BOOL visible;

	// display to render on, result of XOpenDisplay
	Display* display;
	// parent window, result of DefaultRootWindow
	Window parent_window;
	// actual window
	Window window;
	// graphics api
	muga_graphics_api api;

	// event handling
	XEvent event;
	// delete message @TODO make sure this is the correct way of doing this
	Atom delete_message;
	// input
	muga_linux_input input;

	// opengl context
	MUGA_OPENGL_CALL(GLXContext opengl_context);

	// callbacks
	void (*framebuffer_resize_callback)(muga_window win, int new_width, int new_height);
};
typedef struct muga_linux_window muga_linux_window;

muga_linux_window* muga_linux_windows = MUGA_NULL_PTR;
size_m muga_linux_windows_length = 0;

MUGA_BOOL muga_linux_window_binded = MUGA_FALSE;
muga_window muga_linux_binded_window = 0;

size_m muga_linux_get_new_window_id() {
	for (size_m i = 0; i < muga_linux_windows_length; i++) {
		if (muga_linux_windows[i].active == MUGA_FALSE) {
			return i;
		}
	}

	muga_linux_windows_length++;
	muga_linux_windows = (muga_linux_window*)muga_realloc(
		muga_linux_windows, 
		muga_linux_windows_length * sizeof(muga_linux_window)
	);

	return muga_linux_windows_length - 1;
}

MUGA_BOOL muga_linux_is_id_valid(muga_window win) {
	if (win >= muga_linux_windows_length || 
		muga_linux_windows[win].active == MUGA_FALSE //|| 
		//muga_linux_windows[win].closed == MUGA_TRUE
	) {
		return MUGA_FALSE;
	}
	return MUGA_TRUE;
}

MUGA_BOOL muga_linux_is_valid_closed(muga_window win) {
	if (win >= muga_linux_windows_length || 
		muga_linux_windows[win].active == MUGA_FALSE
	) {
		return MUGA_FALSE;
	}
	return MUGA_TRUE;
}

void muga_linux_window_unbind() {
	if (muga_linux_window_binded && muga_linux_is_id_valid(muga_linux_binded_window)) {

		if (MUGA_IS_OPENGL(muga_linux_windows[muga_linux_binded_window].api)) {
			MUGA_OPENGL_CALL(
				glXMakeCurrent(
					muga_linux_windows[muga_linux_binded_window].display,
					None,
					NULL
				);
			);
		}

		muga_linux_window_binded = MUGA_FALSE;
	}
}

void muga_linux_window_bind(muga_window win) {
	muga_linux_window_unbind();

	if (MUGA_IS_OPENGL(muga_linux_windows[muga_linux_binded_window].api)) {
		MUGA_OPENGL_CALL(
			glXMakeCurrent(
				muga_linux_windows[win].display,
				muga_linux_windows[win].window,
				muga_linux_windows[win].opengl_context
			);
		);
	}

	muga_linux_binded_window = win;
	muga_linux_window_binded = MUGA_TRUE;
}

// https://stackoverflow.com/questions/3756323/how-to-get-the-current-time-in-milliseconds-from-c-in-linux
double muga_linux_get_global_time() {
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
    return (double)spec.tv_sec + ((double)spec.tv_nsec / (double)1.0e9);
}

/* initiation / termination */

MUGA_BOOL muga_linux_is_initiated = MUGA_FALSE;
double muga_linux_original_time = 0.f;

MUGADEF void muga_init(MUGA_RESULT* result) {
	if (muga_linux_is_initiated) {
		muga_print("[MUGA] An attempt to initiate muga despite already being initiated was made.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	if (muga_linux_windows == MUGA_NULL_PTR) {
		muga_linux_windows_length = 1;
		muga_linux_windows = muga_malloc(muga_linux_windows_length * sizeof(muga_linux_window));
		muga_linux_windows[0].active = MUGA_FALSE;
	}

	muga_linux_original_time = muga_linux_get_global_time();

	muga_linux_is_initiated = MUGA_TRUE;
	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_term(MUGA_RESULT* result) {
	if (!muga_linux_is_initiated) {
		muga_print("[MUGA] An attempt to terminate muga despite already being terminated (or never being initiated) was made.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	for (size_m i = 0; i < muga_linux_windows_length; i++) {
		if (muga_linux_windows[i].active) {
			muga_window_destroy(MUGA_NULL_PTR, i);
		}
	}

	if (muga_linux_windows != MUGA_NULL_PTR) {
		muga_free(muga_linux_windows);
		muga_linux_windows = MUGA_NULL_PTR;
		muga_linux_windows_length = 0;
	}

	muga_linux_is_initiated = MUGA_FALSE;
	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF double muga_time_get(MUGA_RESULT* result) {
	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	return muga_linux_get_global_time() - muga_linux_original_time;
}

MUGADEF void muga_time_set(MUGA_RESULT* result, double time) {
	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	muga_linux_original_time = muga_linux_original_time + muga_time_get(result) - time;
}

/* window functions */

MUGADEF muga_window muga_window_create(
	MUGA_RESULT* result, 
	muga_graphics_api api, 
	MUGA_BOOL (*load_functions)(void), 
	const wchar_m* name, 
	unsigned int width, unsigned int height
) {
	// window creation
	muga_window win = (muga_window)muga_linux_get_new_window_id();

	muga_linux_windows[win].display = XOpenDisplay(MUGA_NULL_PTR);
	muga_linux_windows[win].parent_window = DefaultRootWindow(muga_linux_windows[win].display);

	// Would like to not use XCreateSimpleWindow here but doing
	// otherwise makes me pick pixel formatting before picking
	// it with an API, so I'm kinda stuck here
	//int black_color = BlackPixel(muga_linux_windows[win].display, muga_linux_windows[win].parent_window);
	muga_linux_windows[win].window = XCreateSimpleWindow(
		muga_linux_windows[win].display,       // display
		muga_linux_windows[win].parent_window, // parent window
		muga_window_settings.x,                // position
		muga_window_settings.y,
		width, height,                         // dimensions
		0, 0,                        // border width and border color
		0                            // background
	);
	XSelectInput(muga_linux_windows[win].display, muga_linux_windows[win].window, ExposureMask | KeyPressMask | KeyReleaseMask);

	// api initialization

	muga_linux_windows[win].api = api;
	if (MUGA_IS_OPENGL(api)) {
		MUGA_OPENGL_CALL (
			if (muga_linux_init_opengl(
				muga_linux_windows[win].display,
				&muga_linux_windows[win].opengl_context,
				api
			) == MUGA_FAILURE) {
				if (result != MUGA_NULL_PTR) {
					*result = MUGA_FAILURE;
				}
				XDestroyWindow(
					muga_linux_windows[win].display,
					muga_linux_windows[win].window
				);
				XCloseDisplay(muga_linux_windows[win].display);
				return MUGA_NO_WINDOW;
			}
		);
	} else if (api != MUGA_NO_GRAPHICS_API) {
		muga_print("[MUGA] Unsupported/Excluded (#define MUGA_NO_...) graphics API for Linux.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		XDestroyWindow(
			muga_linux_windows[win].display,
			muga_linux_windows[win].window
		);
		XCloseDisplay(muga_linux_windows[win].display);
		return MUGA_NO_WINDOW;
	}

	// define more info

	// delete message
	// https://stackoverflow.com/questions/10792361/how-do-i-gracefully-exit-an-x11-event-loop
	muga_linux_windows[win].delete_message = XInternAtom(
		muga_linux_windows[win].display, "WM_DELETE_WINDOW", MUGA_FALSE
	);
	XSetWMProtocols(
		muga_linux_windows[win].display, 
		muga_linux_windows[win].window,
		&muga_linux_windows[win].delete_message,
		1
	);

	// callbacks
	muga_linux_windows[win].framebuffer_resize_callback = MUGA_NULL_PTR;

	// display window

	XSizeHints    my_hints = {0};

	my_hints.flags  = PPosition | PSize;
	my_hints.x      = 0;
	my_hints.y      = 0;
	my_hints.width  = 0; // not sure if setting width and
	my_hints.height = 0; // height to 0 here is safe...
	if (!muga_window_settings.resizable) {
		my_hints.flags |= PMinSize | PMaxSize;
		my_hints.min_width =  width;
		my_hints.min_height = height;
		my_hints.max_width =  width;
		my_hints.max_height = height;
	}

	XSetNormalHints(muga_linux_windows[win].display, muga_linux_windows[win].window, &my_hints);

	muga_linux_windows[win].active = MUGA_TRUE;
	muga_linux_windows[win].closed = MUGA_FALSE;
	muga_linux_windows[win].input = (muga_linux_input){0};
	muga_linux_windows[win].visible = muga_window_settings.visible;

	if (muga_linux_windows[win].visible) {
		XMapWindow(muga_linux_windows[win].display, muga_linux_windows[win].window);
	}

	// convert name from wchar_m* to char*
	size_m len = 0;
	for (size_m i = 0; name[i] != 0; i++) len++;
	char* name_c = muga_malloc(sizeof(char) * (len+1));
	for (size_m i = 0; i < len; i++) name_c[i] = (char)name[i];
	name_c[len] = 0;
	XStoreName(muga_linux_windows[win].display, muga_linux_windows[win].window, name_c);
	muga_free(name_c);

	muga_linux_window_bind(win);

	// load api

	if (load_functions != MUGA_NULL_PTR) {
	    if (!load_functions()) {
	    	muga_print("[MUGA] Failed to load functions for graphics API.\n");
	    	if (result != MUGA_NULL_PTR) {
				*result = MUGA_FAILURE;
			}
			if (MUGA_IS_OPENGL(muga_linux_windows[win].api)) {
				MUGA_OPENGL_CALL(
					glXDestroyContext(muga_linux_windows[win].display, muga_linux_windows[win].opengl_context);
				);
			}
			XDestroyWindow(
				muga_linux_windows[win].display,
				muga_linux_windows[win].window
			);
			XCloseDisplay(muga_linux_windows[win].display);
			return MUGA_NO_WINDOW;
	    }
	}

	// return success

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	return win;
}

MUGADEF void muga_window_destroy(MUGA_RESULT* result, muga_window win) {
	if (!muga_linux_is_valid_closed(win)) {
		muga_print("[MUGA] Requested window ID for destruction is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	if (win == muga_linux_binded_window) {
		muga_linux_window_unbind();
	}

	// not sure if anything else needs to be destroyed...
	if (MUGA_IS_OPENGL(muga_linux_windows[win].api)) {
		MUGA_OPENGL_CALL(
			glXDestroyContext(muga_linux_windows[win].display, muga_linux_windows[win].opengl_context);
		);
	}
	XDestroyWindow(
		muga_linux_windows[win].display,
		muga_linux_windows[win].window
	);
	XCloseDisplay(muga_linux_windows[win].display);

	muga_linux_input_flush(&muga_linux_windows[win].input);

	muga_linux_windows[win].active = MUGA_FALSE;

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_update(MUGA_RESULT* result, muga_window win) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for updating is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	XWindowAttributes attributes;
	XGetWindowAttributes(
		muga_linux_windows[win].display,
		muga_linux_windows[win].window,
		&attributes
	);

	while (XPending(muga_linux_windows[win].display)) {
		XNextEvent(muga_linux_windows[win].display, &muga_linux_windows[win].event);
		switch (muga_linux_windows[win].event.type) {

		case Expose:
			if (muga_linux_windows[win].framebuffer_resize_callback != MUGA_NULL_PTR) {
				muga_linux_windows[win].framebuffer_resize_callback(win, attributes.width, attributes.height);
			}
			break;

		case ClientMessage:
			if (muga_linux_windows[win].event.xclient.data.l[0] == muga_linux_windows[win].delete_message) {
				muga_linux_windows[win].closed = MUGA_TRUE;
			}
			XUnmapWindow(muga_linux_windows[win].display, muga_linux_windows[win].window);
			break;

		case KeyRelease:
			muga_linux_input_set_status(
				&muga_linux_windows[win].input,
				XkbKeycodeToKeysym(muga_linux_windows[win].display, muga_linux_windows[win].event.xkey.keycode, 0, 0),
				MUGA_KEY_UP
			);
			break;

		case KeyPress:
			muga_linux_input_set_status(
				&muga_linux_windows[win].input,
				XkbKeycodeToKeysym(muga_linux_windows[win].display, muga_linux_windows[win].event.xkey.keycode, 0, 0),
				MUGA_KEY_DOWN
			);
			break;

		default:
			break;
		}
	}

	Window focused_window;
	int revert_to;
	XGetInputFocus(muga_linux_windows[win].display, &focused_window, &revert_to);

	if (focused_window != muga_linux_windows[win].window) {
		muga_linux_input_flush(&muga_linux_windows[win].input);
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_swap_buffers(MUGA_RESULT* result, muga_window win) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for swapping buffers is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	if (MUGA_IS_OPENGL(muga_linux_windows[win].api)) {
		MUGA_OPENGL_CALL(
			glXSwapBuffers(muga_linux_windows[win].display, muga_linux_windows[win].window);
		);
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF MUGA_BOOL muga_window_get_closed(MUGA_RESULT* result, muga_window win) {
	if (!muga_linux_is_valid_closed(win)) {
		muga_print("[MUGA] Requested window ID for checking if closed is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return MUGA_TRUE;
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	return muga_linux_windows[win].closed;
}

MUGADEF void muga_window_close(MUGA_RESULT* result, muga_window win) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for closing is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	muga_window_set_visible(result, win, MUGA_FALSE);
	muga_linux_windows[win].closed = MUGA_TRUE;

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF MUGA_BOOL muga_window_get_focused(MUGA_RESULT* result, muga_window win) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for getting focused state is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return MUGA_FALSE;
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}

	Window focused;
	int revert_to;
	XGetInputFocus(muga_linux_windows[win].display, &focused, &revert_to);
	return focused == muga_linux_windows[win].window;
}

MUGADEF void muga_window_focus(MUGA_RESULT* result, muga_window win) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for focusing is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	// https://www.linuxquestions.org/questions/programming-9/how-to-read-the-state-by-using-_net_wm_state-in-xlib-836879/
	MUGA_BOOL minimized = MUGA_FALSE;
	Atom wmState = XInternAtom(muga_linux_windows[win].display, "_NET_WM_STATE", MUGA_TRUE);
	Atom type;
	int format;
	unsigned long nItem, bytesAfter;
	unsigned char* properties = MUGA_NULL;
	XGetWindowProperty(
		muga_linux_windows[win].display,
		muga_linux_windows[win].window,
		wmState,
		0,
		(~0L),
		MUGA_FALSE,
		AnyPropertyType,
		&type,
		&format,
		&nItem,
		&bytesAfter,
		&properties
	);

	for (size_m i = 0; i < nItem; i++) {
		Atom prop = ((Atom*)properties)[0];
		char* name = XGetAtomName(muga_linux_windows[win].display, prop);
		if (muga_strcmp(name, "_NET_WM_STATE_HIDDEN") == 0) {
			minimized = MUGA_TRUE;
			if (name) {
				XFree(name);
			}
			break;
		}
		if (name) {
			XFree(name);
		}
	}

	// https://stackoverflow.com/questions/30192347/how-to-restore-a-window-with-xlib
	if (minimized) {
		XClientMessageEvent ev;
		muga_memset(&ev, 0, sizeof(ev));
		ev.type = ClientMessage;
		ev.window = muga_linux_windows[win].window;
		ev.message_type = XInternAtom(
			muga_linux_windows[win].display,
			"_NET_ACTIVE_WINDOW",
			MUGA_TRUE
		);
		ev.format = 32;
		ev.data.l[0] = 1;
		ev.data.l[1] = CurrentTime;
		ev.data.l[2] = ev.data.l[3] = ev.data.l[4] = 0;
		XSendEvent(
			muga_linux_windows[win].display,
			muga_linux_windows[win].parent_window,
			MUGA_FALSE,
			SubstructureRedirectMask | SubstructureNotifyMask,
			(XEvent*)&ev
		);
		XFlush(muga_linux_windows[win].display);
	}

	XSetInputFocus(
		muga_linux_windows[win].display,
		muga_linux_windows[win].window,
		RevertToPointerRoot,
		CurrentTime
	);
	XRaiseWindow(
		muga_linux_windows[win].display,
		muga_linux_windows[win].window
	);

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_set_context(MUGA_RESULT* result, muga_window win) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for setting context is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	if ((!muga_linux_window_binded) || (muga_linux_window_binded && muga_linux_binded_window != win)) {
		muga_linux_window_bind(win);
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_set_title(MUGA_RESULT* result, muga_window win, const wchar_m* title) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for setting title is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	// convert title from wchar_m* to char*
	size_m len = 0;
	for (size_m i = 0; title[i] != 0; i++) len++;
	char* name_c = muga_malloc(sizeof(char) * (len+1));
	for (size_m i = 0; i < len; i++) name_c[i] = (char)title[i];
	name_c[len] = 0;
	XStoreName(muga_linux_windows[win].display, muga_linux_windows[win].window, name_c);
	muga_free(name_c);

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF MUGA_BOOL muga_window_get_visible(MUGA_RESULT* result, muga_window win) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for getting visibility is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return MUGA_FALSE;
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	return muga_linux_windows[win].visible;
}

MUGADEF void muga_window_set_visible(MUGA_RESULT* result, muga_window win, MUGA_BOOL visible) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for setting visibility is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	if (visible == MUGA_FALSE && muga_linux_windows[win].visible == MUGA_TRUE) {
		XUnmapWindow(muga_linux_windows[win].display, muga_linux_windows[win].window);
		muga_window_update(result, win);
		muga_linux_input_flush(&muga_linux_windows[win].input);
	} else if (visible == MUGA_TRUE && muga_linux_windows[win].visible == MUGA_FALSE) {
		XMapWindow(muga_linux_windows[win].display, muga_linux_windows[win].window);
		muga_window_update(result, win);
		muga_linux_input_flush(&muga_linux_windows[win].input);
	}

	muga_linux_windows[win].visible = visible;

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_get_position(MUGA_RESULT* result, muga_window win, int* x, int* y) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for getting position is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	// https://stackoverflow.com/questions/3806872/window-position-in-xlib
	int rx = 0, ry = 0;
	Window child;
	XWindowAttributes xwa;
	XTranslateCoordinates(
		muga_linux_windows[win].display,
		muga_linux_windows[win].window,
		muga_linux_windows[win].parent_window,
		0, 0,
		&rx, &ry,
		&child
	);
	XGetWindowAttributes(muga_linux_windows[win].display, muga_linux_windows[win].window, &xwa);

	if (x != MUGA_NULL_PTR) {
		*x = (int)(rx - xwa.x);
	}

	if (y != MUGA_NULL_PTR) {
		*y = (int)(ry - xwa.y);
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_set_position(MUGA_RESULT* result, muga_window win, int x, int y) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for setting position is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	XMoveWindow(
		muga_linux_windows[win].display, 
		muga_linux_windows[win].window, 
		x, y
	);

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_get_dimensions(MUGA_RESULT* result, muga_window win, unsigned int* width, unsigned int* height) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for getting dimensions is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	XWindowAttributes xwa;
	XGetWindowAttributes(muga_linux_windows[win].display, muga_linux_windows[win].window, &xwa);

	if (width != MUGA_NULL_PTR) {
		*width = xwa.width;
	}

	if (height != MUGA_NULL_PTR) {
		*height = xwa.height;
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF void muga_window_set_dimensions(MUGA_RESULT* result, muga_window win, unsigned int width, unsigned int height) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for setting dimensions is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	XResizeWindow(
		muga_linux_windows[win].display, 
		muga_linux_windows[win].window, 
		width, height
	);

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF MUGA_BOOL muga_window_get_maximized(MUGA_RESULT* result, muga_window win) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for getting maximized state is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return MUGA_FALSE;
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}

	Atom wmState = XInternAtom(muga_linux_windows[win].display, "_NET_WM_STATE", MUGA_TRUE);
	Atom type;
	int format;
	unsigned long nItem, bytesAfter;
	unsigned char* properties = MUGA_NULL;
	XGetWindowProperty(
		muga_linux_windows[win].display,
		muga_linux_windows[win].window,
		wmState,
		0,
		(~0L),
		MUGA_FALSE,
		AnyPropertyType,
		&type,
		&format,
		&nItem,
		&bytesAfter,
		&properties
	);

	for (size_m i = 0; i < nItem; i++) {
		Atom prop = ((Atom*)properties)[0];
		char* name = XGetAtomName(muga_linux_windows[win].display, prop);
		if (muga_strcmp(name, "_NET_WM_STATE_MAXIMIZED_VERT") == 0 || muga_strcmp(name, "_NET_WM_STATE_MAXIMIZED_HORZ") == 0) {
			if (name) {
				XFree(name);
			}
			return MUGA_TRUE;
		}
		if (name) {
			XFree(name);
		}
	}

	return MUGA_FALSE;
}

MUGADEF void muga_window_set_maximized(MUGA_RESULT* result, muga_window win, MUGA_BOOL maximized) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for setting maximized state is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	// https://forum.juce.com/t/how-to-maximize-not-fullscreen/28346
	XClientMessageEvent ev;
	muga_memset(&ev, 0, sizeof(ev));
	ev.type = ClientMessage;
	ev.window = muga_linux_windows[win].window;
	ev.message_type = XInternAtom(
		muga_linux_windows[win].display,
		"_NET_WM_STATE",
		MUGA_FALSE
	);
	ev.format = 32;
	ev.data.l[0] = maximized;
	ev.data.l[1] = XInternAtom(muga_linux_windows[win].display, "_NET_WM_STATE_MAXIMIZED_VERT", MUGA_FALSE);
	ev.data.l[2] = XInternAtom(muga_linux_windows[win].display, "_NET_WM_STATE_MAXIMIZED_HORZ", MUGA_FALSE);
	ev.data.l[3] = 1;
	ev.data.l[4] = 0;
	XSendEvent(
		muga_linux_windows[win].display,
		muga_linux_windows[win].parent_window,
		MUGA_FALSE,
		SubstructureRedirectMask | SubstructureNotifyMask,
		(XEvent*)&ev
	);
	XFlush(muga_linux_windows[win].display);

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF MUGA_BOOL muga_window_get_minimized(MUGA_RESULT* result, muga_window win) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for getting minimized state is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return MUGA_FALSE;
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}

	Atom wmState = XInternAtom(muga_linux_windows[win].display, "_NET_WM_STATE", MUGA_TRUE);
	Atom type;
	int format;
	unsigned long nItem, bytesAfter;
	unsigned char* properties = MUGA_NULL;
	XGetWindowProperty(
		muga_linux_windows[win].display,
		muga_linux_windows[win].window,
		wmState,
		0,
		(~0L),
		MUGA_FALSE,
		AnyPropertyType,
		&type,
		&format,
		&nItem,
		&bytesAfter,
		&properties
	);

	for (size_m i = 0; i < nItem; i++) {
		Atom prop = ((Atom*)properties)[0];
		char* name = XGetAtomName(muga_linux_windows[win].display, prop);
		if (muga_strcmp(name, "_NET_WM_STATE_HIDDEN") == 0) {
			if (name) {
				XFree(name);
			}
			return MUGA_TRUE;
		}
		if (name) {
			XFree(name);
		}
	}

	return MUGA_FALSE;
}

MUGADEF void muga_window_set_minimized(MUGA_RESULT* result, muga_window win, MUGA_BOOL minimized) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for setting minimized state is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	// might want to use functions like XIconifyWindow here...
	// don't know.
	if (minimized == MUGA_TRUE) {
		XClientMessageEvent ev;
		Atom prop;
		prop = XInternAtom(
			muga_linux_windows[win].display,
			"WM_CHANGE_STATE",
			MUGA_FALSE
		);
		ev.type = ClientMessage;
		ev.window = muga_linux_windows[win].window;
		ev.message_type = prop;
		ev.format = 32;
		ev.data.l[0] = IconicState;
		XSendEvent(
			muga_linux_windows[win].display,
			muga_linux_windows[win].parent_window,
			MUGA_FALSE,
			SubstructureRedirectMask|SubstructureNotifyMask,
			(XEvent*)&ev
		);
		XFlush(muga_linux_windows[win].display);
	} else {
		XClientMessageEvent ev;
		muga_memset(&ev, 0, sizeof(ev));
		ev.type = ClientMessage;
		ev.window = muga_linux_windows[win].window;
		ev.message_type = XInternAtom(
			muga_linux_windows[win].display,
			"_NET_ACTIVE_WINDOW",
			MUGA_TRUE
		);
		ev.format = 32;
		ev.data.l[0] = 1;
		ev.data.l[1] = CurrentTime;
		ev.data.l[2] = ev.data.l[3] = ev.data.l[4] = 0;
		XSendEvent(
			muga_linux_windows[win].display,
			muga_linux_windows[win].parent_window,
			MUGA_FALSE,
			SubstructureRedirectMask | SubstructureNotifyMask,
			(XEvent*)&ev
		);
		XFlush(muga_linux_windows[win].display);
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

MUGADEF MUGA_KEY_BIT muga_window_get_input_bit(MUGA_RESULT* result, muga_window win, muga_input_method method, muga_input_key key) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for getting input bit is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return MUGA_KEY_UP;
	}

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
	return muga_linux_input_get_status(muga_linux_windows[win].input, key);
}

MUGADEF void muga_window_set_framebuffer_resize_callback(
	MUGA_RESULT* result, 
	muga_window win, 
	void (*framebuffer_resize_callback)(muga_window win, int new_width, int new_height)
) {
	if (!muga_linux_is_id_valid(win)) {
		muga_print("[MUGA] Requested window ID for setting framebuffer resize callback is invalid.\n");
		if (result != MUGA_NULL_PTR) {
			*result = MUGA_FAILURE;
		}
		return;
	}

	muga_linux_windows[win].framebuffer_resize_callback = framebuffer_resize_callback;

	if (result != MUGA_NULL_PTR) {
		*result = MUGA_SUCCESS;
	}
}

/* opengl functions */

MUGA_OPENGL_CALL(
MUGADEF void* muga_get_opengl_function_address(const char* name) {
	return (void*)glXGetProcAddress((const GLubyte*)name);
}
)

#endif /* LINUX */

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
