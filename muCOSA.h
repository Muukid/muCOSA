/*
muCOSA.h - Muukid
Public domain single-file C library for interacting with operating systems with a cross-platform API.
No warranty implied; use at your own risk.

Licensed under MIT License or public domain, whichever you prefer.
More explicit license information at the end of the file.
*/

#ifndef MUCOSA_H
#define MUCOSA_H

#ifdef __cplusplus
    extern "C" {
#endif

/* basics */

#define MUCOSA_VERSION_MAJOR 1
#define MUCOSA_VERSION_MINOR 0
#define MUCOSA_VERSION_PATCH 0

#ifndef MUDEF
	#ifdef MU_STATIC
		#define MUDEF static
	#else
		#define MUDEF extern
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

#if !defined(mu_strlen)  || \
	!defined(mu_wstrlen)

    #include <string.h>
	
	#ifndef mu_strlen
    	#define mu_strlen strlen
	#endif

	#ifndef mu_wstrlen
		#define mu_wstrlen wcslen
	#endif
#endif

#if !defined(mu_malloc)  || \
    !defined(mu_free)    || \
    !defined(mu_realloc)

    #include <stdlib.h>

    #ifndef mu_malloc
    	#define mu_malloc malloc
    #endif

    #ifndef mu_free
    	#define mu_free free
    #endif

    #ifndef mu_realloc
    	#define mu_realloc realloc
    #endif

#endif

#if !defined(mu_strcmp) || \
    !defined(mu_memset)

    #include <string.h>

    #ifndef mu_strcmp
    	#define mu_strcmp strcmp
    #endif

    #ifndef mu_memset
    	#define mu_memset memset
    #endif

#endif

#ifndef MU_BOOL
	#define MU_BOOL
	enum muBool { MU_FALSE, MU_TRUE };
	typedef enum muBool muBool;
#endif

#ifndef muResult
	#define muResult  muBool
	#define MU_FAILURE MU_FALSE
	#define MU_SUCCESS MU_TRUE
#endif

#ifndef MU_NULL
	#define MU_NULL     0
#endif

#ifndef MU_NULL_PTR
	#define MU_NULL_PTR 0
#endif

/* apis */

enum muGraphicsAPI {
	MU_NO_GRAPHICS_API,

	// opengl

	MU_OPENGL_1_0,
	MU_OPENGL_1_1,
	MU_OPENGL_1_2,
	MU_OPENGL_1_2_1,
	MU_OPENGL_1_3,
	MU_OPENGL_1_4,
	MU_OPENGL_1_5,
	MU_OPENGL_2_0,
	MU_OPENGL_2_1,
	MU_OPENGL_3_0,
	MU_OPENGL_3_1,
	MU_OPENGL_3_2_CORE,
	MU_OPENGL_3_2_COMPATIBILITY,
	MU_OPENGL_3_3_CORE,
	MU_OPENGL_3_3_COMPATIBILITY,
	MU_OPENGL_4_0_CORE,
	MU_OPENGL_4_0_COMPATIBILITY,
	MU_OPENGL_4_1_CORE,
	MU_OPENGL_4_1_COMPATIBILITY,
	MU_OPENGL_4_2_CORE,
	MU_OPENGL_4_2_COMPATIBILITY,
	MU_OPENGL_4_3_CORE,
	MU_OPENGL_4_3_COMPATIBILITY,
	MU_OPENGL_4_4_CORE,
	MU_OPENGL_4_4_COMPATIBILITY,
	MU_OPENGL_4_5_CORE,
	MU_OPENGL_4_5_COMPATIBILITY,
	MU_OPENGL_4_6_CORE,
	MU_OPENGL_4_6_COMPATIBILITY

#define MU_OPENGL_FIRST MU_OPENGL_1_0
#define MU_OPENGL_LAST MU_OPENGL_4_6_COMPATIBILITY
#define MU_IS_OPENGL(api) (api >= MU_OPENGL_FIRST && api <= MU_OPENGL_LAST)

};
typedef enum muGraphicsAPI muGraphicsAPI;

/* keyboard input */

enum muKeyboardKey {
	MU_KEYBOARD_KEY_UNKNOWN,

	// @TODO this list is missing quite a few ascii characters
	// like ~\[];./
	MU_KEYBOARD_KEY_BACKSPACE,
	MU_KEYBOARD_KEY_TAB,
	MU_KEYBOARD_KEY_CLEAR,
	MU_KEYBOARD_KEY_RETURN,
	MU_KEYBOARD_KEY_PAUSE,
	MU_KEYBOARD_KEY_ESCAPE,
	MU_KEYBOARD_KEY_MODECHANGE,
	MU_KEYBOARD_KEY_SPACE,
	MU_KEYBOARD_KEY_PRIOR,
	MU_KEYBOARD_KEY_NEXT,
	MU_KEYBOARD_KEY_END,
	MU_KEYBOARD_KEY_HOME,
	MU_KEYBOARD_KEY_LEFT,
	MU_KEYBOARD_KEY_UP,
	MU_KEYBOARD_KEY_RIGHT,
	MU_KEYBOARD_KEY_DOWN,
	MU_KEYBOARD_KEY_SELECT,
	MU_KEYBOARD_KEY_PRINT,
	MU_KEYBOARD_KEY_EXECUTE,
	MU_KEYBOARD_KEY_INSERT,
	MU_KEYBOARD_KEY_DELETE,
	MU_KEYBOARD_KEY_HELP,
	MU_KEYBOARD_KEY_0,
	MU_KEYBOARD_KEY_1,
	MU_KEYBOARD_KEY_2,
	MU_KEYBOARD_KEY_3,
	MU_KEYBOARD_KEY_4,
	MU_KEYBOARD_KEY_5,
	MU_KEYBOARD_KEY_6,
	MU_KEYBOARD_KEY_7,
	MU_KEYBOARD_KEY_8,
	MU_KEYBOARD_KEY_9,
	MU_KEYBOARD_KEY_A,
	MU_KEYBOARD_KEY_B,
	MU_KEYBOARD_KEY_C,
	MU_KEYBOARD_KEY_D,
	MU_KEYBOARD_KEY_E,
	MU_KEYBOARD_KEY_F,
	MU_KEYBOARD_KEY_G,
	MU_KEYBOARD_KEY_H,
	MU_KEYBOARD_KEY_I,
	MU_KEYBOARD_KEY_J,
	MU_KEYBOARD_KEY_K,
	MU_KEYBOARD_KEY_L,
	MU_KEYBOARD_KEY_M,
	MU_KEYBOARD_KEY_N,
	MU_KEYBOARD_KEY_O,
	MU_KEYBOARD_KEY_P,
	MU_KEYBOARD_KEY_Q,
	MU_KEYBOARD_KEY_R,
	MU_KEYBOARD_KEY_S,
	MU_KEYBOARD_KEY_T,
	MU_KEYBOARD_KEY_U,
	MU_KEYBOARD_KEY_V,
	MU_KEYBOARD_KEY_W,
	MU_KEYBOARD_KEY_X,
	MU_KEYBOARD_KEY_Y,
	MU_KEYBOARD_KEY_Z,
	MU_KEYBOARD_KEY_LEFT_WINDOWS,
	MU_KEYBOARD_KEY_RIGHT_WINDOWS,
	MU_KEYBOARD_KEY_NUMPAD_0,
	MU_KEYBOARD_KEY_NUMPAD_1,
	MU_KEYBOARD_KEY_NUMPAD_2,
	MU_KEYBOARD_KEY_NUMPAD_3,
	MU_KEYBOARD_KEY_NUMPAD_4,
	MU_KEYBOARD_KEY_NUMPAD_5,
	MU_KEYBOARD_KEY_NUMPAD_6,
	MU_KEYBOARD_KEY_NUMPAD_7,
	MU_KEYBOARD_KEY_NUMPAD_8,
	MU_KEYBOARD_KEY_NUMPAD_9,
	MU_KEYBOARD_KEY_MULTIPLY,
	MU_KEYBOARD_KEY_ADD,
	MU_KEYBOARD_KEY_SEPARATOR,
	MU_KEYBOARD_KEY_SUBTRACT,
	MU_KEYBOARD_KEY_DECIMAL,
	MU_KEYBOARD_KEY_DIVIDE,
	MU_KEYBOARD_KEY_F1,
	MU_KEYBOARD_KEY_F2,
	MU_KEYBOARD_KEY_F3,
	MU_KEYBOARD_KEY_F4,
	MU_KEYBOARD_KEY_F5,
	MU_KEYBOARD_KEY_F6,
	MU_KEYBOARD_KEY_F7,
	MU_KEYBOARD_KEY_F8,
	MU_KEYBOARD_KEY_F9,
	MU_KEYBOARD_KEY_F10,
	MU_KEYBOARD_KEY_F11,
	MU_KEYBOARD_KEY_F12,
	MU_KEYBOARD_KEY_F13,
	MU_KEYBOARD_KEY_F14,
	MU_KEYBOARD_KEY_F15,
	MU_KEYBOARD_KEY_F16,
	MU_KEYBOARD_KEY_F17,
	MU_KEYBOARD_KEY_F18,
	MU_KEYBOARD_KEY_F19,
	MU_KEYBOARD_KEY_F20,
	MU_KEYBOARD_KEY_F21,
	MU_KEYBOARD_KEY_F22,
	MU_KEYBOARD_KEY_F23,
	MU_KEYBOARD_KEY_F24,
	MU_KEYBOARD_KEY_NUMLOCK,
	MU_KEYBOARD_KEY_SCROLL,
	MU_KEYBOARD_KEY_LEFT_SHIFT,
	MU_KEYBOARD_KEY_RIGHT_SHIFT,
	MU_KEYBOARD_KEY_LEFT_CONTROL,
	MU_KEYBOARD_KEY_RIGHT_CONTROL,
	MU_KEYBOARD_KEY_LEFT_MENU,
	MU_KEYBOARD_KEY_RIGHT_MENU,
	MU_KEYBOARD_KEY_ATTN,
	MU_KEYBOARD_KEY_CRSEL,
	MU_KEYBOARD_KEY_EXSEL,
	MU_KEYBOARD_KEY_EREOF,
	MU_KEYBOARD_KEY_PLAY,
	MU_KEYBOARD_KEY_PA1

#define MU_KEYBOARD_KEY_FIRST MU_KEYBOARD_KEY_BACKSPACE
#define MU_KEYBOARD_KEY_LAST MU_KEYBOARD_KEY_PA1
#define MU_IS_KEYBOARD_KEY(key) (key >= MU_KEYBOARD_KEY_FIRST && key <= MU_KEYBOARD_KEY_LAST)
};
typedef enum muKeyboardKey muKeyboardKey;

#define muKeyboardKeyBit  muBool
#define MU_KEYBOARD_KEY_BIT_UP   MU_FALSE
#define MU_KEYBOARD_KEY_BIT_DOWN MU_TRUE

/* keyboard state */

enum muKeyboardState {
	MU_KEYBOARD_STATE_UNKNOWN,

	MU_KEYBOARD_STATE_CAPS_LOCK,
	MU_KEYBOARD_STATE_SCROLL_LOCK,
	MU_KEYBOARD_STATE_NUM_LOCK

#define MU_KEYBOARD_STATE_FIRST MU_KEYBOARD_STATE_CAPS_LOCK
#define MU_KEYBOARD_STATE_LAST MU_KEYBOARD_STATE_NUM_LOCK
#define MU_IS_KEYBOARD_STATE(state) (state >= MU_KEYBOARD_STATE_FIRST && state <= MU_KEYBOARD_STATE_LAST)
};
typedef enum muKeyboardState muKeyboardState;

#define muKeyboardStateBit muBool
#define MU_KEYBOARD_STATE_BIT_ON MU_TRUE
#define MU_KEYBOARD_STATE_BIT_OFF MU_FALSE

/* mouse input */

enum muMouseButton {
	MU_MOUSE_BUTTON_UNKNOWN,

	MU_MOUSE_BUTTON_LEFT,
	MU_MOUSE_BUTTON_RIGHT,
	MU_MOUSE_BUTTON_MIDDLE

#define MU_MOUSE_BUTTON_FIRST MU_MOUSE_BUTTON_LEFT
#define MU_MOUSE_BUTTON_LAST MU_MOUSE_BUTTON_MIDDLE
#define MU_IS_MOUSE_BUTTON(key) (key >= MU_MOUSE_BUTTON_FIRST && key <= MU_MOUSE_BUTTON_LAST)
};
typedef enum muMouseButton muMouseButton;

#define muMouseButtonBit  muBool
#define MU_MOUSE_BUTTON_BIT_UP   MU_FALSE
#define MU_MOUSE_BUTTON_BIT_DOWN MU_TRUE

/* mouse style */

enum muCursorStyle {
	MU_CURSOR_STYLE_UNKNOWN,

	// taken from LÖVE
	// https://love2d.org/wiki/CursorType
	// @TODO add invisible
	MU_CURSOR_STYLE_DEFAULT,
	MU_CURSOR_STYLE_ARROW,
	MU_CURSOR_STYLE_IBEAM,
	MU_CURSOR_STYLE_WAIT,
	MU_CURSOR_STYLE_WAIT_ARROW,
	MU_CURSOR_STYLE_CROSSHAIR,
	MU_CURSOR_STYLE_HAND,
	MU_CURSOR_STYLE_SIZE_EAST_WEST,
	MU_CURSOR_STYLE_SIZE_NORTH_SOUTH,
	MU_CURSOR_STYLE_SIZE_NORTH_EAST_SOUTH_WEST,
	MU_CURSOR_STYLE_SIZE_NORTH_WEST_SOUTH_EAST,
	MU_CURSOR_STYLE_SIZE_ALL,
	MU_CURSOR_STYLE_NO

#define MU_CURSOR_STYLE_FIRST MU_CURSOR_STYLE_ARROW
#define MU_CURSOR_STYLE_LAST MU_CURSOR_STYLE_NO
#define MU_IS_CURSOR_STYLE(style) (style >= MU_CURSOR_STYLE_FIRST && style <= MU_CURSOR_STYLE_LAST)
};
typedef enum muCursorStyle muCursorStyle;

/* general */

MUDEF void mu_COSA_init(muResult* result);
MUDEF void mu_COSA_term(muResult* result);

MUDEF double mu_time_get(muResult* result);
MUDEF void   mu_time_set(muResult* result, double time);

MUDEF size_m mu_clipboard_get(muResult* result, char* buffer, size_m len);
MUDEF void   mu_clipboard_set(muResult* result, char* string);

/* window */

typedef size_m muWindow;
#define MU_NO_WINDOW SIZE_MAX_M

// basic window functionality

// pixel formatting
// https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt
// https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glXChooseFBConfig.xml
struct muPixelFormat {
	muBool doublebuffer;

	unsigned int red_bits;
	unsigned int green_bits;
	unsigned int blue_bits;
	unsigned int alpha_bits;

	unsigned int depth_bits;
	unsigned int stencil_bits;

	unsigned int samples;
};
typedef struct muPixelFormat muPixelFormat;

// settings
struct muWindowSettingsStruct {
	muPixelFormat pixel_format;

	muBool visible;
	muBool resizable;

	muBool minimized;
	muBool maximized;

	// @TODO test negative values
	int x;
	int y;

	unsigned int minimum_width;
	unsigned int minimum_height;
	unsigned int maximum_width;
	unsigned int maximum_height;

	muCursorStyle cursor_style;
};
typedef struct muWindowSettingsStruct muWindowSettingsStruct;

muWindowSettingsStruct mu_window_settings = {
	.pixel_format = {
		.doublebuffer = MU_TRUE,
		.red_bits =     8,
		.green_bits =   8,
		.blue_bits =    8,
		.alpha_bits =   8,
		.depth_bits =   24,
		.stencil_bits = 8,
		.samples =      1
	},

	.visible =   MU_TRUE,
	.resizable = MU_TRUE,

	.minimized = MU_FALSE,
	.maximized = MU_FALSE,

	.x = 400,
	.y = 200,

	.minimum_width = 120,   // minimum width for Windows
	.minimum_height = 1,    // minimum height for Linux
	.maximum_width = 30720, // (32k 16:9)
	.maximum_height = 17280,

	.cursor_style = MU_CURSOR_STYLE_DEFAULT
};

MUDEF muWindow mu_window_create(muResult* result, muGraphicsAPI api, muBool (*load_functions)(void), const char* name, unsigned int width, unsigned int height);
MUDEF void mu_window_destroy(muResult* result, muWindow win);

MUDEF void mu_window_update(muResult* result, muWindow win);
MUDEF void mu_window_swap_buffers(muResult* result, muWindow win);

// sets

MUDEF void mu_window_set_context(muResult* result, muWindow win);

MUDEF void mu_window_set_title(muResult* result, muWindow win, const char* title);

// get / toggle functions

MUDEF muBool mu_window_get_closed(muResult* result, muWindow win);
MUDEF void mu_window_close(muResult* result, muWindow win);

MUDEF muBool mu_window_get_focused(muResult* result, muWindow win);
MUDEF void   mu_window_focus(muResult* result, muWindow win);

// get / set window functions

MUDEF muBool mu_window_get_visible(muResult* result, muWindow win);
MUDEF void   mu_window_set_visible(muResult* result, muWindow win, muBool visible);

MUDEF void mu_window_get_position(muResult* result, muWindow win, int* x, int* y);
MUDEF void mu_window_set_position(muResult* result, muWindow win, int  x, int  y);

MUDEF void mu_window_get_dimensions(muResult* result, muWindow win, unsigned int* width, unsigned int* height);
MUDEF void mu_window_set_dimensions(muResult* result, muWindow win, unsigned int  width, unsigned int  height);

MUDEF muBool mu_window_get_maximized(muResult* result, muWindow win);
MUDEF void   mu_window_set_maximized(muResult* result, muWindow win, muBool maximized);

MUDEF muBool mu_window_get_minimized(muResult* result, muWindow win);
MUDEF void   mu_window_set_minimized(muResult* result, muWindow win, muBool minimized);

MUDEF void mu_window_get_minimum_dimensions(muResult* result, muWindow win, unsigned int* width, unsigned int* height);
MUDEF void mu_window_set_minimum_dimensions(muResult* result, muWindow win, unsigned int  width, unsigned int  height);

MUDEF void mu_window_get_maximum_dimensions(muResult* result, muWindow win, unsigned int* width, unsigned int* height);
MUDEF void mu_window_set_maximum_dimensions(muResult* result, muWindow win, unsigned int  width, unsigned int  height);

MUDEF void mu_window_get_mouse_position(muResult* result, muWindow win, int* x, int* y);
MUDEF void mu_window_set_mouse_position(muResult* result, muWindow win, int  x, int  y);

MUDEF muCursorStyle mu_window_get_cursor_style(muResult* result, muWindow win);
MUDEF void          mu_window_set_cursor_style(muResult* result, muWindow win, muCursorStyle style);

MUDEF int  mu_window_get_scroll_level(muResult* result, muWindow win);
MUDEF void mu_window_set_scroll_level(muResult* result, muWindow win, int scroll_level);

// input

MUDEF muKeyboardKeyBit mu_window_get_keyboard_key_bit(muResult* result, muWindow win, muKeyboardKey key);
MUDEF muKeyboardStateBit mu_window_get_keyboard_state_bit(muResult* result, muWindow win, muKeyboardState state);
MUDEF muMouseButtonBit mu_window_get_mouse_button_bit(muResult* result, muWindow win, muMouseButton key);

// callbacks

MUDEF void mu_window_set_dimensions_callback    (muResult* result, muWindow win, void (*dimensions_callback)    (muWindow win, int width, int height));
MUDEF void mu_window_set_position_callback      (muResult* result, muWindow win, void (*position_callback)      (muWindow win, int x, int y));
MUDEF void mu_window_set_focus_callback         (muResult* result, muWindow win, void (*focus_callback)         (muWindow win, muBool focused));
MUDEF void mu_window_set_maximize_callback      (muResult* result, muWindow win, void (*maximize_callback)      (muWindow win, muBool maximized));
MUDEF void mu_window_set_minimize_callback      (muResult* result, muWindow win, void (*minimize_callback)      (muWindow win, muBool minimized));

MUDEF void mu_window_set_keyboard_callback      (muResult* result, muWindow win, void (*keyboard_callback)      (muWindow win, muKeyboardKey key, muKeyboardKeyBit bit));
MUDEF void mu_window_set_keyboard_state_callback(muResult* result, muWindow win, void (*keyboard_state_callback)(muWindow win, muKeyboardState state, muKeyboardStateBit bit));
MUDEF void mu_window_set_mouse_callback         (muResult* result, muWindow win, void (*mouse_callback)         (muWindow win, muMouseButton key, muMouseButtonBit bit));
MUDEF void mu_window_set_scroll_callback        (muResult* result, muWindow win, void (*scroll_callback)        (muWindow win, int scroll_level_add));

/* opengl functions */

MUDEF void* mu_get_opengl_function_address(const char* name);

#ifdef __cplusplus
    }
#endif

#endif /* MUCOSA_H */



#ifdef MUCOSA_IMPLEMENTATION

#ifdef __cplusplus
    extern "C" {
#endif

/* pre-implementation setup */

#ifndef mu_print
    #ifdef MUCOSA_NO_PRINT
    	#define mu_print(msg)
		#define mu_printf(msg, ...)
    #else
    	#include <stdio.h>
    	#define mu_print printf
		#define mu_printf printf
    #endif
#endif

/* graphics api flag settings */

#ifndef MUCOSA_OPENGL
    #ifndef MUCOSA_NO_OPENGL
    	#define MUCOSA_NO_OPENGL
    #endif
#endif

#ifdef MUCOSA_NO_API
    #ifndef MUCOSA_NO_OPENGL
    	#define MUCOSA_NO_OPENGL
    #endif
#endif

/* WINDOWS IMPLEMENTATION */

#if defined(_WIN32) || defined(WIN32)
#define MUCOSA_WINDOWS

#include <windows.h>

// get the hinstance
// thank you we luv u raymond chen <3
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
HINSTANCE mu_windows_get_hinstance() {
	return HINST_THISCOMPONENT;
}

/* OPENGL SETUP */
#ifndef MUCOSA_NO_OPENGL

#ifndef MUCOSA_NO_INCLUDE_OPENGL
	#ifndef glClearColor
		#include <gl/gl.h>
		#include <gl/glu.h>
	#endif
#endif

#define MU_OPENGL_CALL(stuff) stuff

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

// https://nehe.gamedev.net/tutorial/fullscreen_antialiasing/16008/
#define WGL_SAMPLE_BUFFERS_ARB  0x2041
#define WGL_SAMPLES_ARB     0x2042

// wgl variables

typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int *attribList);
wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;

typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
wglChoosePixelFormatARB_type *wglChoosePixelFormatARB;

// https://gist.github.com/nickrolfe/1127313ed1dbf80254b614a721b3ee9c

// initiates a dummy wgl context and gets the opengl extensions
// needed for more pixel format options
muBool mu_windows_has_initiated_opengl_extensions = MU_FALSE;
muResult mu_windows_init_opengl_extensions() {
	if (mu_windows_has_initiated_opengl_extensions) {
		return MU_SUCCESS;
	}
	WNDCLASSA win_class = {
		.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
		.lpfnWndProc = DefWindowProcA,
		.hInstance = mu_windows_get_hinstance(),
		.lpszClassName = "dummy wgl window"
	};

	if (!RegisterClassA(&win_class)) {
		mu_print("[muCOSA] Failed to register window class.\n");
		return MU_FAILURE;
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
		mu_print("[muCOSA] Failed to create window.\n");
		return MU_FAILURE;
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
		mu_print("[muCOSA] Failed to find a valid pixel format.\n");
		ReleaseDC(win, dc);
		DestroyWindow(win);
		return MU_FAILURE;
	}
	if (!SetPixelFormat(dc, pixel_format, &format)) {
		mu_print("[muCOSA] Failed to set a pixel format.\n");
		ReleaseDC(win, dc);
		DestroyWindow(win);
		return MU_FAILURE;
	}

	HGLRC context = wglCreateContext(dc);
	if (!context) {
		mu_print("[muCOSA] Failed to create a valid WGL context.\n");
		ReleaseDC(win, dc);
		DestroyWindow(win);
		return MU_FAILURE;
	}
	if (!wglMakeCurrent(dc, context)) {
		mu_print("[muCOSA] Failed to load a WGL context.\n");
		wglDeleteContext(context);
		ReleaseDC(win, dc);
		DestroyWindow(win);
		return MU_FAILURE;
	}

	wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress("wglCreateContextAttribsARB");
	wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress("wglChoosePixelFormatARB");

	wglMakeCurrent(dc, 0);
	wglDeleteContext(context);
	ReleaseDC(win, dc);
	DestroyWindow(win);
	mu_windows_has_initiated_opengl_extensions = MU_TRUE;
	return MU_SUCCESS;
}

// creates an opengl context
muResult mu_windows_create_opengl_context(HDC device_context, HGLRC* context, muGraphicsAPI api) {
	if (!mu_windows_init_opengl_extensions()) {
		return MU_FAILURE;
	}

	// pixel formatting
	int pixel_format_attributes[] = {
		WGL_DRAW_TO_WINDOW_ARB, MU_TRUE,
        WGL_SUPPORT_OPENGL_ARB, MU_TRUE,
        WGL_DOUBLE_BUFFER_ARB,  mu_window_settings.pixel_format.doublebuffer,
		WGL_SAMPLE_BUFFERS_ARB, MU_TRUE,
        WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
        WGL_RED_BITS_ARB,       mu_window_settings.pixel_format.red_bits,
        WGL_GREEN_BITS_ARB,     mu_window_settings.pixel_format.green_bits,
        WGL_BLUE_BITS_ARB,      mu_window_settings.pixel_format.blue_bits,
        WGL_ALPHA_BITS_ARB,     mu_window_settings.pixel_format.alpha_bits,
        WGL_DEPTH_BITS_ARB,     mu_window_settings.pixel_format.depth_bits,
        WGL_STENCIL_BITS_ARB,   mu_window_settings.pixel_format.stencil_bits,
		WGL_SAMPLES_ARB,        16,
        0
	};

	// get the closest available format
	int pixel_format;
	UINT format_count;
	wglChoosePixelFormatARB(device_context, pixel_format_attributes, 0, 1, &pixel_format, &format_count);
	if (!format_count) {
		mu_print("[muCOSA] Failed to find a compatible OpenGL pixel format.\n");
		return MU_FAILURE;
	}

	// set format
	PIXELFORMATDESCRIPTOR format;
	DescribePixelFormat(device_context, pixel_format, sizeof(PIXELFORMATDESCRIPTOR), &format);
	if (!SetPixelFormat(device_context, pixel_format, &format)) {
		mu_print("[muCOSA] Failed to set OpenGL pixel format.\n");
		return MU_FAILURE;
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
		case MU_OPENGL_1_0:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 0;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_1_1:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 1;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_1_2:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 2;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_1_2_1:
		// What the hell do I do here?
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 2;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_1_3:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 3;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_1_4:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 4;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_1_5:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 5;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_2_0:
			opengl_attributes[1] = 2;
			opengl_attributes[3] = 0;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_2_1:
			opengl_attributes[1] = 2;
			opengl_attributes[3] = 0;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_3_0:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 0;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_3_1:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 1;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_3_2_CORE:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 2;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_3_2_COMPATIBILITY:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 2;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_3_3_CORE:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_3_3_COMPATIBILITY:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_0_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 0;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_0_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 0;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_1_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 1;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_1_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 1;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_2_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 2;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_2_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 2;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_3_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_3_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_4_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 4;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_4_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 4;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_5_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 5;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_5_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 5;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_6_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 6;
			opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_6_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 6;
			opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
	}

	// create and activate context
	HGLRC opengl_context = wglCreateContextAttribsARB(device_context, 0, opengl_attributes);
	if (!opengl_context) {
		mu_print("[muCOSA] Failed to create a valid OpenGL context.");
		return MU_FAILURE;
	}
	if (!wglMakeCurrent(device_context, opengl_context)) {
		mu_print("[muCOSA] Failed to activate OpenGL context.");
		return MU_FAILURE;
	}
	*context = opengl_context;

	return MU_SUCCESS;
}

#else

#define MU_OPENGL_CALL(...)

#endif /* MUCOSA_NO_OPENGL */

/* keyboard input */

int mu_windows_mu_keyboard_to_windows_key(muKeyboardKey key) {
	switch (key) {
	default:
		return VK_NONAME;
		break;
	case MU_KEYBOARD_KEY_BACKSPACE:
		return VK_BACK;
		break;
	case MU_KEYBOARD_KEY_TAB:
		return VK_TAB;
		break;
	case MU_KEYBOARD_KEY_CLEAR:
		return VK_CLEAR;
		break;
	case MU_KEYBOARD_KEY_RETURN:
		return VK_RETURN;
		break;
	case MU_KEYBOARD_KEY_PAUSE:
		return VK_PAUSE;
		break;
	case MU_KEYBOARD_KEY_ESCAPE:
		return VK_ESCAPE;
		break;
	case MU_KEYBOARD_KEY_MODECHANGE:
		return VK_MODECHANGE;
		break;
	case MU_KEYBOARD_KEY_SPACE:
		return VK_SPACE;
		break;
	case MU_KEYBOARD_KEY_PRIOR:
		return VK_PRIOR;
		break;
	case MU_KEYBOARD_KEY_NEXT:
		return VK_NEXT;
		break;
	case MU_KEYBOARD_KEY_END:
		return VK_END;
		break;
	case MU_KEYBOARD_KEY_HOME:
		return VK_HOME;
		break;
	case MU_KEYBOARD_KEY_LEFT:
		return VK_LEFT;
		break;
	case MU_KEYBOARD_KEY_UP:
		return VK_UP;
		break;
	case MU_KEYBOARD_KEY_RIGHT:
		return VK_RIGHT;
		break;
	case MU_KEYBOARD_KEY_DOWN:
		return VK_DOWN;
		break;
	case MU_KEYBOARD_KEY_SELECT:
		return VK_SELECT;
		break;
	case MU_KEYBOARD_KEY_PRINT:
		return VK_PRINT;
		break;
	case MU_KEYBOARD_KEY_EXECUTE:
		return VK_EXECUTE;
		break;
	case MU_KEYBOARD_KEY_INSERT:
		return VK_INSERT;
		break;
	case MU_KEYBOARD_KEY_DELETE:
		return VK_DELETE;
		break;
	case MU_KEYBOARD_KEY_HELP:
		return VK_HELP;
		break;
	case MU_KEYBOARD_KEY_0:
		return 0x30;
		break;
	case MU_KEYBOARD_KEY_1:
		return 0x31;
		break;
	case MU_KEYBOARD_KEY_2:
		return 0x32;
		break;
	case MU_KEYBOARD_KEY_3:
		return 0x33;
		break;
	case MU_KEYBOARD_KEY_4:
		return 0x34;
		break;
	case MU_KEYBOARD_KEY_5:
		return 0x35;
		break;
	case MU_KEYBOARD_KEY_6:
		return 0x36;
		break;
	case MU_KEYBOARD_KEY_7:
		return 0x37;
		break;
	case MU_KEYBOARD_KEY_8:
		return 0x38;
		break;
	case MU_KEYBOARD_KEY_9:
		return 0x39;
		break;
	case MU_KEYBOARD_KEY_A:
		return 0x41;
		break;
	case MU_KEYBOARD_KEY_B:
		return 0x42;
		break;
	case MU_KEYBOARD_KEY_C:
		return 0x43;
		break;
	case MU_KEYBOARD_KEY_D:
		return 0x44;
		break;
	case MU_KEYBOARD_KEY_E:
		return 0x45;
		break;
	case MU_KEYBOARD_KEY_F:
		return 0x46;
		break;
	case MU_KEYBOARD_KEY_G:
		return 0x47;
		break;
	case MU_KEYBOARD_KEY_H:
		return 0x48;
		break;
	case MU_KEYBOARD_KEY_I:
		return 0x49;
		break;
	case MU_KEYBOARD_KEY_J:
		return 0x4A;
		break;
	case MU_KEYBOARD_KEY_K:
		return 0x4B;
		break;
	case MU_KEYBOARD_KEY_L:
		return 0x4C;
		break;
	case MU_KEYBOARD_KEY_M:
		return 0x4D;
		break;
	case MU_KEYBOARD_KEY_N:
		return 0x4E;
		break;
	case MU_KEYBOARD_KEY_O:
		return 0x4F;
		break;
	case MU_KEYBOARD_KEY_P:
		return 0x50;
		break;
	case MU_KEYBOARD_KEY_Q:
		return 0x51;
		break;
	case MU_KEYBOARD_KEY_R:
		return 0x52;
		break;
	case MU_KEYBOARD_KEY_S:
		return 0x53;
		break;
	case MU_KEYBOARD_KEY_T:
		return 0x54;
		break;
	case MU_KEYBOARD_KEY_U:
		return 0x55;
		break;
	case MU_KEYBOARD_KEY_V:
		return 0x56;
		break;
	case MU_KEYBOARD_KEY_W:
		return 0x57;
		break;
	case MU_KEYBOARD_KEY_X:
		return 0x58;
		break;
	case MU_KEYBOARD_KEY_Y:
		return 0x59;
		break;
	case MU_KEYBOARD_KEY_Z:
		return 0x5A;
		break;
	case MU_KEYBOARD_KEY_LEFT_WINDOWS:
		return VK_LWIN;
		break;
	case MU_KEYBOARD_KEY_RIGHT_WINDOWS:
		return VK_RWIN;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_0:
		return VK_NUMPAD0;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_1:
		return VK_NUMPAD1;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_2:
		return VK_NUMPAD2;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_3:
		return VK_NUMPAD3;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_4:
		return VK_NUMPAD4;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_5:
		return VK_NUMPAD5;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_6:
		return VK_NUMPAD6;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_7:
		return VK_NUMPAD7;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_8:
		return VK_NUMPAD8;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_9:
		return VK_NUMPAD9;
		break;
	case MU_KEYBOARD_KEY_MULTIPLY:
		return VK_MULTIPLY;
		break;
	case MU_KEYBOARD_KEY_ADD:
		return VK_ADD;
		break;
	case MU_KEYBOARD_KEY_SEPARATOR:
		return VK_SEPARATOR;
		break;
	case MU_KEYBOARD_KEY_SUBTRACT:
		return VK_SUBTRACT;
		break;
	case MU_KEYBOARD_KEY_DECIMAL:
		return VK_DECIMAL;
		break;
	case MU_KEYBOARD_KEY_DIVIDE:
		return VK_DIVIDE;
		break;
	case MU_KEYBOARD_KEY_F1:
		return VK_F1;
		break;
	case MU_KEYBOARD_KEY_F2:
		return VK_F2;
		break;
	case MU_KEYBOARD_KEY_F3:
		return VK_F3;
		break;
	case MU_KEYBOARD_KEY_F4:
		return VK_F4;
		break;
	case MU_KEYBOARD_KEY_F5:
		return VK_F5;
		break;
	case MU_KEYBOARD_KEY_F6:
		return VK_F6;
		break;
	case MU_KEYBOARD_KEY_F7:
		return VK_F7;
		break;
	case MU_KEYBOARD_KEY_F8:
		return VK_F8;
		break;
	case MU_KEYBOARD_KEY_F9:
		return VK_F9;
		break;
	case MU_KEYBOARD_KEY_F10:
		return VK_F10;
		break;
	case MU_KEYBOARD_KEY_F11:
		return VK_F11;
		break;
	case MU_KEYBOARD_KEY_F12:
		return VK_F12;
		break;
	case MU_KEYBOARD_KEY_F13:
		return VK_F13;
		break;
	case MU_KEYBOARD_KEY_F14:
		return VK_F14;
		break;
	case MU_KEYBOARD_KEY_F15:
		return VK_F15;
		break;
	case MU_KEYBOARD_KEY_F16:
		return VK_F16;
		break;
	case MU_KEYBOARD_KEY_F17:
		return VK_F17;
		break;
	case MU_KEYBOARD_KEY_F18:
		return VK_F18;
		break;
	case MU_KEYBOARD_KEY_F19:
		return VK_F19;
		break;
	case MU_KEYBOARD_KEY_F20:
		return VK_F20;
		break;
	case MU_KEYBOARD_KEY_F21:
		return VK_F21;
		break;
	case MU_KEYBOARD_KEY_F22:
		return VK_F22;
		break;
	case MU_KEYBOARD_KEY_F23:
		return VK_F23;
		break;
	case MU_KEYBOARD_KEY_F24:
		return VK_F24;
		break;
	case MU_KEYBOARD_KEY_NUMLOCK:
		return VK_NUMLOCK;
		break;
	case MU_KEYBOARD_KEY_SCROLL:
		return VK_SCROLL;
		break;
	case MU_KEYBOARD_KEY_LEFT_SHIFT:
		return VK_LSHIFT;
		break;
	case MU_KEYBOARD_KEY_RIGHT_SHIFT:
		return VK_RSHIFT;
		break;
	case MU_KEYBOARD_KEY_LEFT_CONTROL:
		return VK_LCONTROL;
		break;
	case MU_KEYBOARD_KEY_RIGHT_CONTROL:
		return VK_RCONTROL;
		break;
	case MU_KEYBOARD_KEY_LEFT_MENU:
		return VK_LMENU;
		break;
	case MU_KEYBOARD_KEY_RIGHT_MENU:
		return VK_RMENU;
		break;
	case MU_KEYBOARD_KEY_ATTN:
		return VK_ATTN;
		break;
	case MU_KEYBOARD_KEY_CRSEL:
		return VK_CRSEL;
		break;
	case MU_KEYBOARD_KEY_EXSEL:
		return VK_EXSEL;
		break;
	case MU_KEYBOARD_KEY_EREOF:
		return VK_EREOF;
		break;
	case MU_KEYBOARD_KEY_PLAY:
		return VK_PLAY;
		break;
	case MU_KEYBOARD_KEY_PA1:
		return VK_PA1;
		break;
	}
}

muKeyboardKey mu_windows_windows_key_to_mu_keyboard(int key) {
	switch (key) {
	default:
		return MU_KEYBOARD_KEY_UNKNOWN;
		break;
	case VK_BACK:
		return MU_KEYBOARD_KEY_BACKSPACE;
		break;
	case VK_TAB:
		return MU_KEYBOARD_KEY_TAB;
		break;
	case VK_CLEAR:
		return MU_KEYBOARD_KEY_CLEAR;
		break;
	case VK_RETURN:
		return MU_KEYBOARD_KEY_RETURN;
		break;
	case VK_PAUSE:
		return MU_KEYBOARD_KEY_PAUSE;
		break;
	case VK_ESCAPE:
		return MU_KEYBOARD_KEY_ESCAPE;
		break;
	case VK_MODECHANGE:
		return MU_KEYBOARD_KEY_MODECHANGE;
		break;
	case VK_SPACE:
		return MU_KEYBOARD_KEY_SPACE;
		break;
	case VK_PRIOR:
		return MU_KEYBOARD_KEY_PRIOR;
		break;
	case VK_NEXT:
		return MU_KEYBOARD_KEY_NEXT;
		break;
	case VK_END:
		return MU_KEYBOARD_KEY_END;
		break;
	case VK_HOME:
		return MU_KEYBOARD_KEY_HOME;
		break;
	case VK_LEFT:
		return MU_KEYBOARD_KEY_LEFT;
		break;
	case VK_UP:
		return MU_KEYBOARD_KEY_UP;
		break;
	case VK_RIGHT:
		return MU_KEYBOARD_KEY_RIGHT;
		break;
	case VK_DOWN:
		return MU_KEYBOARD_KEY_DOWN;
		break;
	case VK_SELECT:
		return MU_KEYBOARD_KEY_SELECT;
		break;
	case VK_PRINT:
		return MU_KEYBOARD_KEY_PRINT;
		break;
	case VK_EXECUTE:
		return MU_KEYBOARD_KEY_EXECUTE;
		break;
	case VK_INSERT:
		return MU_KEYBOARD_KEY_INSERT;
		break;
	case VK_DELETE:
		return MU_KEYBOARD_KEY_DELETE;
		break;
	case VK_HELP:
		return MU_KEYBOARD_KEY_HELP;
		break;
	case 0x30:
		return MU_KEYBOARD_KEY_0;
		break;
	case 0x31:
		return MU_KEYBOARD_KEY_1;
		break;
	case 0x32:
		return MU_KEYBOARD_KEY_2;
		break;
	case 0x33:
		return MU_KEYBOARD_KEY_3;
		break;
	case 0x34:
		return MU_KEYBOARD_KEY_4;
		break;
	case 0x35:
		return MU_KEYBOARD_KEY_5;
		break;
	case 0x36:
		return MU_KEYBOARD_KEY_6;
		break;
	case 0x37:
		return MU_KEYBOARD_KEY_7;
		break;
	case 0x38:
		return MU_KEYBOARD_KEY_8;
		break;
	case 0x39:
		return MU_KEYBOARD_KEY_9;
		break;
	case 0x41:
		return MU_KEYBOARD_KEY_A;
		break;
	case 0x42:
		return MU_KEYBOARD_KEY_B;
		break;
	case 0x43:
		return MU_KEYBOARD_KEY_C;
		break;
	case 0x44:
		return MU_KEYBOARD_KEY_D;
		break;
	case 0x45:
		return MU_KEYBOARD_KEY_E;
		break;
	case 0x46:
		return MU_KEYBOARD_KEY_F;
		break;
	case 0x47:
		return MU_KEYBOARD_KEY_G;
		break;
	case 0x48:
		return MU_KEYBOARD_KEY_H;
		break;
	case 0x49:
		return MU_KEYBOARD_KEY_I;
		break;
	case 0x4A:
		return MU_KEYBOARD_KEY_J;
		break;
	case 0x4B:
		return MU_KEYBOARD_KEY_K;
		break;
	case 0x4C:
		return MU_KEYBOARD_KEY_L;
		break;
	case 0x4D:
		return MU_KEYBOARD_KEY_M;
		break;
	case 0x4E:
		return MU_KEYBOARD_KEY_N;
		break;
	case 0x4F:
		return MU_KEYBOARD_KEY_O;
		break;
	case 0x50:
		return MU_KEYBOARD_KEY_P;
		break;
	case 0x51:
		return MU_KEYBOARD_KEY_Q;
		break;
	case 0x52:
		return MU_KEYBOARD_KEY_R;
		break;
	case 0x53:
		return MU_KEYBOARD_KEY_S;
		break;
	case 0x54:
		return MU_KEYBOARD_KEY_T;
		break;
	case 0x55:
		return MU_KEYBOARD_KEY_U;
		break;
	case 0x56:
		return MU_KEYBOARD_KEY_V;
		break;
	case 0x57:
		return MU_KEYBOARD_KEY_W;
		break;
	case 0x58:
		return MU_KEYBOARD_KEY_X;
		break;
	case 0x59:
		return MU_KEYBOARD_KEY_Y;
		break;
	case 0x5A:
		return MU_KEYBOARD_KEY_Z;
		break;
	case VK_LWIN:
		return MU_KEYBOARD_KEY_LEFT_WINDOWS;
		break;
	case VK_RWIN:
		return MU_KEYBOARD_KEY_RIGHT_WINDOWS;
		break;
	case VK_NUMPAD0:
		return MU_KEYBOARD_KEY_NUMPAD_0;
		break;
	case VK_NUMPAD1:
		return MU_KEYBOARD_KEY_NUMPAD_1;
		break;
	case VK_NUMPAD2:
		return MU_KEYBOARD_KEY_NUMPAD_2;
		break;
	case VK_NUMPAD3:
		return MU_KEYBOARD_KEY_NUMPAD_3;
		break;
	case VK_NUMPAD4:
		return MU_KEYBOARD_KEY_NUMPAD_4;
		break;
	case VK_NUMPAD5:
		return MU_KEYBOARD_KEY_NUMPAD_5;
		break;
	case VK_NUMPAD6:
		return MU_KEYBOARD_KEY_NUMPAD_6;
		break;
	case VK_NUMPAD7:
		return MU_KEYBOARD_KEY_NUMPAD_7;
		break;
	case VK_NUMPAD8:
		return MU_KEYBOARD_KEY_NUMPAD_8;
		break;
	case VK_NUMPAD9:
		return MU_KEYBOARD_KEY_NUMPAD_9;
		break;
	case VK_MULTIPLY:
		return MU_KEYBOARD_KEY_MULTIPLY;
		break;
	case VK_ADD:
		return MU_KEYBOARD_KEY_ADD;
		break;
	case VK_SEPARATOR:
		return MU_KEYBOARD_KEY_SEPARATOR;
		break;
	case VK_SUBTRACT:
		return MU_KEYBOARD_KEY_SUBTRACT;
		break;
	case VK_DECIMAL:
		return MU_KEYBOARD_KEY_DECIMAL;
		break;
	case VK_DIVIDE:
		return MU_KEYBOARD_KEY_DIVIDE;
		break;
	case VK_F1:
		return MU_KEYBOARD_KEY_F1;
		break;
	case VK_F2:
		return MU_KEYBOARD_KEY_F2;
		break;
	case VK_F3:
		return MU_KEYBOARD_KEY_F3;
		break;
	case VK_F4:
		return MU_KEYBOARD_KEY_F4;
		break;
	case VK_F5:
		return MU_KEYBOARD_KEY_F5;
		break;
	case VK_F6:
		return MU_KEYBOARD_KEY_F6;
		break;
	case VK_F7:
		return MU_KEYBOARD_KEY_F7;
		break;
	case VK_F8:
		return MU_KEYBOARD_KEY_F8;
		break;
	case VK_F9:
		return MU_KEYBOARD_KEY_F9;
		break;
	case VK_F10:
		return MU_KEYBOARD_KEY_F10;
		break;
	case VK_F11:
		return MU_KEYBOARD_KEY_F11;
		break;
	case VK_F12:
		return MU_KEYBOARD_KEY_F12;
		break;
	case VK_F13:
		return MU_KEYBOARD_KEY_F13;
		break;
	case VK_F14:
		return MU_KEYBOARD_KEY_F14;
		break;
	case VK_F15:
		return MU_KEYBOARD_KEY_F15;
		break;
	case VK_F16:
		return MU_KEYBOARD_KEY_F16;
		break;
	case VK_F17:
		return MU_KEYBOARD_KEY_F17;
		break;
	case VK_F18:
		return MU_KEYBOARD_KEY_F18;
		break;
	case VK_F19:
		return MU_KEYBOARD_KEY_F19;
		break;
	case VK_F20:
		return MU_KEYBOARD_KEY_F20;
		break;
	case VK_F21:
		return MU_KEYBOARD_KEY_F21;
		break;
	case VK_F22:
		return MU_KEYBOARD_KEY_F22;
		break;
	case VK_F23:
		return MU_KEYBOARD_KEY_F23;
		break;
	case VK_F24:
		return MU_KEYBOARD_KEY_F24;
		break;
	case VK_NUMLOCK:
		return MU_KEYBOARD_KEY_NUMLOCK;
		break;
	case VK_SCROLL:
		return MU_KEYBOARD_KEY_SCROLL;
		break;
	case VK_LSHIFT:
		return MU_KEYBOARD_KEY_LEFT_SHIFT;
		break;
	case VK_RSHIFT:
		return MU_KEYBOARD_KEY_RIGHT_SHIFT;
		break;
	case VK_LCONTROL:
		return MU_KEYBOARD_KEY_LEFT_CONTROL;
		break;
	case VK_RCONTROL:
		return MU_KEYBOARD_KEY_RIGHT_CONTROL;
		break;
	case VK_LMENU:
		return MU_KEYBOARD_KEY_LEFT_MENU;
		break;
	case VK_RMENU:
		return MU_KEYBOARD_KEY_RIGHT_MENU;
		break;
	case VK_ATTN:
		return MU_KEYBOARD_KEY_ATTN;
		break;
	case VK_CRSEL:
		return MU_KEYBOARD_KEY_CRSEL;
		break;
	case VK_EXSEL:
		return MU_KEYBOARD_KEY_EXSEL;
		break;
	case VK_EREOF:
		return MU_KEYBOARD_KEY_EREOF;
		break;
	case VK_PLAY:
		return MU_KEYBOARD_KEY_PLAY;
		break;
	case VK_PA1:
		return MU_KEYBOARD_KEY_PA1;
		break;
	}
}

muKeyboardState mu_windows_windows_key_to_muKeyboardState(int key) {
	switch (key) {
	default:
		return MU_KEYBOARD_STATE_UNKNOWN;
		break;
	case VK_CAPITAL:
		return MU_KEYBOARD_STATE_CAPS_LOCK;
		break;
	case VK_SCROLL:
		return MU_KEYBOARD_STATE_SCROLL_LOCK;
		break;
	case VK_NUMLOCK:
		return MU_KEYBOARD_STATE_NUM_LOCK;
		break;
	}
}

int mu_windows_muKeyboardState_to_windows_key(muKeyboardState state) {
	switch (state) {
	default:
		return VK_NONAME;
		break;
	case MU_KEYBOARD_STATE_CAPS_LOCK:
		return VK_CAPITAL;
		break;
	case MU_KEYBOARD_STATE_SCROLL_LOCK:
		return VK_SCROLL;
		break;
	case MU_KEYBOARD_STATE_NUM_LOCK:
		return VK_NUMLOCK;
		break;
	}
}

muMouseButton mu_windows_windows_key_to_mu_mouse(int key) {
	switch (key) {
	default:
		return MU_MOUSE_BUTTON_UNKNOWN;
		break;
	case VK_LBUTTON:
		return MU_MOUSE_BUTTON_LEFT;
		break;
	case VK_RBUTTON:
		return MU_MOUSE_BUTTON_RIGHT;
		break;
	case VK_MBUTTON:
		return MU_MOUSE_BUTTON_MIDDLE;
		break;
	}
}

void* mu_windows_mu_cursor_to_windows_cursor(muCursorStyle style) {
	switch (style) {
	default:
		return IDC_ARROW;
		break;
	case MU_CURSOR_STYLE_ARROW:
		return IDC_ARROW;
		break;
	case MU_CURSOR_STYLE_IBEAM:
		return IDC_IBEAM;
		break;
	case MU_CURSOR_STYLE_WAIT:
		return IDC_WAIT;
		break;
	case MU_CURSOR_STYLE_WAIT_ARROW:
		return IDC_APPSTARTING;
		break;
	case MU_CURSOR_STYLE_CROSSHAIR:
		return IDC_CROSS;
		break;
	case MU_CURSOR_STYLE_HAND:
		return IDC_HAND;
		break;
	case MU_CURSOR_STYLE_SIZE_EAST_WEST:
		return IDC_SIZEWE;
		break;
	case MU_CURSOR_STYLE_SIZE_NORTH_SOUTH:
		return IDC_SIZENS;
		break;
	case MU_CURSOR_STYLE_SIZE_NORTH_EAST_SOUTH_WEST:
		return IDC_SIZENESW;
		break;
	case MU_CURSOR_STYLE_SIZE_NORTH_WEST_SOUTH_EAST:
		return IDC_SIZENWSE;
		break;
	case MU_CURSOR_STYLE_SIZE_ALL:
		return IDC_SIZEALL;
		break;
	case MU_CURSOR_STYLE_NO:
		return IDC_NO;
		break;
	}
}

// done to avoid non-integer constant case label
#define MUCOSA_IDC_ARROW       32512
#define MUCOSA_IDC_IBEAM       32513
#define MUCOSA_IDC_WAIT        32514
#define MUCOSA_IDC_APPSTARTING 32650
#define MUCOSA_IDC_CROSS       32515
#define MUCOSA_IDC_HAND        32649
#define MUCOSA_IDC_SIZEWE      32644
#define MUCOSA_IDC_SIZENS      32645
#define MUCOSA_IDC_SIZENESW    32643
#define MUCOSA_IDC_SIZENWSE    32642
#define MUCOSA_IDC_SIZEALL     32646
#define MUCOSA_IDC_NO          32648

muCursorStyle mu_windows_cursor_to_mu_cursor(void* cursor) {
	switch ((int)cursor) {
	default:
		return MU_CURSOR_STYLE_IBEAM;
		break;
	case MUCOSA_IDC_ARROW:
		return MU_CURSOR_STYLE_ARROW;
		break;
	case MUCOSA_IDC_IBEAM:
		return MU_CURSOR_STYLE_IBEAM;
		break;
	case MUCOSA_IDC_WAIT:
		return MU_CURSOR_STYLE_WAIT;
		break;
	case MUCOSA_IDC_APPSTARTING:
		return MU_CURSOR_STYLE_WAIT_ARROW;
		break;
	case MUCOSA_IDC_CROSS:
		return MU_CURSOR_STYLE_CROSSHAIR;
		break;
	case MUCOSA_IDC_HAND:
		return MU_CURSOR_STYLE_HAND;
		break;
	case MUCOSA_IDC_SIZEWE:
		return MU_CURSOR_STYLE_SIZE_EAST_WEST;
		break;
	case MUCOSA_IDC_SIZENS:
		return MU_CURSOR_STYLE_SIZE_NORTH_SOUTH;
		break;
	case MUCOSA_IDC_SIZENESW:
		return MU_CURSOR_STYLE_SIZE_NORTH_EAST_SOUTH_WEST;
		break;
	case MUCOSA_IDC_SIZENWSE:
		return MU_CURSOR_STYLE_SIZE_NORTH_WEST_SOUTH_EAST;
		break;
	case MUCOSA_IDC_SIZEALL:
		return MU_CURSOR_STYLE_SIZE_ALL;
		break;
	case MUCOSA_IDC_NO:
		return MU_CURSOR_STYLE_NO;
		break;
	}
}

struct mu_windows_input {
	muKeyboardKeyBit keyboard_down_status[MU_KEYBOARD_KEY_LAST-MU_KEYBOARD_KEY_FIRST+1];
	muKeyboardStateBit keyboard_state_status[MU_KEYBOARD_STATE_LAST-MU_KEYBOARD_STATE_FIRST+1];
	muMouseButtonBit mouse_down_status[MU_MOUSE_BUTTON_LAST-MU_MOUSE_BUTTON_FIRST+1];
};
typedef struct mu_windows_input mu_windows_input;

void mu_windows_input_keyboard_set_status(mu_windows_input* input, int windows_key, muKeyboardKeyBit bit) {
	muKeyboardKey key = mu_windows_windows_key_to_mu_keyboard(windows_key);
	if (MU_IS_KEYBOARD_KEY(key)) {
		input->keyboard_down_status[key-MU_KEYBOARD_KEY_FIRST] = bit;
	}
}

muKeyboardKeyBit mu_windows_keyboard_input_get_status(mu_windows_input input, muKeyboardKey key) {
	if (MU_IS_KEYBOARD_KEY(key)) {
		return input.keyboard_down_status[key-MU_KEYBOARD_KEY_FIRST];
	}
	return MU_KEYBOARD_KEY_BIT_UP;
}

void mu_windows_input_keyboard_state_set_status(mu_windows_input* input, int windows_key, muKeyboardStateBit bit) {
	muKeyboardState state = mu_windows_windows_key_to_muKeyboardState(windows_key);
	if (MU_IS_KEYBOARD_STATE(state)) {
		input->keyboard_state_status[state-MU_KEYBOARD_STATE_FIRST] = bit;
	}
}

muKeyboardStateBit mu_windows_keyboard_state_get_status(mu_windows_input input, muKeyboardState state) {
	if (MU_IS_KEYBOARD_STATE(state)) {
		return input.keyboard_state_status[state-MU_KEYBOARD_STATE_FIRST];
	}
	return MU_KEYBOARD_STATE_BIT_OFF;
}

void mu_windows_input_mouse_set_status(mu_windows_input* input, int windows_key, muMouseButtonBit bit) {
	muMouseButton key = mu_windows_windows_key_to_mu_mouse(windows_key);
	if (MU_IS_MOUSE_BUTTON(key)) {
		input->mouse_down_status[key-MU_MOUSE_BUTTON_FIRST] = bit;
	}
}

muMouseButtonBit mu_windows_mouse_input_get_status(mu_windows_input input, muMouseButton key) {
	if (MU_IS_MOUSE_BUTTON(key)) {
		return input.mouse_down_status[key-MU_MOUSE_BUTTON_FIRST];
	}
	return MU_MOUSE_BUTTON_BIT_UP;
}

void mu_windows_input_flush(mu_windows_input* input) {
	for (size_m i = 0; i < MU_KEYBOARD_KEY_LAST-MU_KEYBOARD_KEY_FIRST+1; i++) {
		input->keyboard_down_status[i] = MU_KEYBOARD_KEY_BIT_UP;
	}
	for (size_m i = 0; i < MU_KEYBOARD_STATE_LAST-MU_KEYBOARD_STATE_FIRST+1; i++) {
		input->keyboard_state_status[i] = MU_KEYBOARD_STATE_BIT_OFF;
	}
	for (size_m i = 0; i < MU_MOUSE_BUTTON_LAST-MU_MOUSE_BUTTON_FIRST+1; i++) {
		input->mouse_down_status[i] = MU_MOUSE_BUTTON_BIT_UP;
	}
}

// https://stackoverflow.com/questions/15966642/how-do-you-tell-lshift-apart-from-rshift-in-wm-keydown-events
WPARAM mu_map_left_right_keys(WPARAM vk, LPARAM lParam) {
	WPARAM new_vk = vk;
	UINT scancode = (lParam & 0x00ff0000) >> 16;
	int extended  = (lParam & 0x01000000) != 0;

	switch (vk) {
	case VK_SHIFT:
		new_vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
		break;
	case VK_CONTROL:
		new_vk = extended ? VK_RCONTROL : VK_LCONTROL;
		break;
	case VK_MENU:
		new_vk = extended ? VK_RMENU : VK_LMENU;
		break;
	default:
		new_vk = vk;
		break;    
	}

	return new_vk;
}

/* default window setup */

// basic window vars/structs

struct mu_windows_window {
	// active states whether or not a valid window is here
	muBool active;
	// closed states whether or not the window here has been closed
	muBool closed;
	// visible or invisible
	muBool visible;
	// whether or not it's maximized for callback
	muBool maximized;
	// whether or not it's minimized for callback
	muBool minimized;

	// class name
	wchar_m* class_name;
	// window class information
	WNDCLASSEXW window_class;
	// window handle
	HWND window_handle;
	// device context
	HDC device_context;
	// input
	mu_windows_input input;

	// api
	muGraphicsAPI api;
	// opengl context
	MU_OPENGL_CALL(HGLRC opengl_context);

	// mins/maxs
	unsigned int minimum_width;
	unsigned int minimum_height;
	unsigned int maximum_width;
	unsigned int maximum_height;

	// callbacks
	void (*dimensions_callback)    (muWindow win, int new_width, int new_height);
	void (*position_callback)      (muWindow win, int x, int y);
	void (*focus_callback)         (muWindow win, muBool focused);
	void (*maximize_callback)      (muWindow win, muBool maximized);
	void (*minimize_callback)      (muWindow win, muBool minimized);
	void (*keyboard_callback)      (muWindow win, muKeyboardKey key, muKeyboardKeyBit bit);
	void (*keyboard_state_callback)(muWindow win, muKeyboardState state, muKeyboardStateBit bit);
	void (*mouse_callback)         (muWindow win, muMouseButton key, muMouseButtonBit bit);
	void (*scroll_callback)        (muWindow win, int scroll_level_add);

	// cursor/mouse stuff
	muCursorStyle cursor_style;
	HCURSOR cursor;
	int scroll_level;
};
typedef struct mu_windows_window mu_windows_window;

// What a stupid name
mu_windows_window* mu_windows_windows      = MU_NULL_PTR;
size_m               mu_windows_windows_length = 0;

muBool mu_windows_window_binded = MU_FALSE;
size_m mu_windows_binded_window = 0;

// window funcs

// window proc
// @TODO make render function and a way to bypass "the WM_NCLBUTTONDOWN trap"
// https://www.gamedev.net/forums/topic/488074-win32-message-pump-and-opengl---rendering-pauses-while-draggingresizing/
LRESULT CALLBACK mu_windows_default_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// find window id
	muBool found_window_id = MU_FALSE;
	muWindow win;
	for (size_m i = 0; i < mu_windows_windows_length; i++) {
		if (mu_windows_windows[i].active && mu_windows_windows[i].window_handle == hwnd) {
			found_window_id = MU_TRUE;
			win = (muWindow)i;
			break;
		}
	}
	if (found_window_id) {
		if (mu_windows_windows[win].closed == MU_TRUE) {
			return DefWindowProcW(hwnd, uMsg, wParam, lParam);
		}
		mu_window_set_context(MU_NULL_PTR, win);
	}

	// outer-proc stuff

	// keyboard state handling
	if (found_window_id) {
		for (size_m i = MU_KEYBOARD_STATE_FIRST; i < MU_KEYBOARD_STATE_LAST + 1; i++) {
			muKeyboardStateBit now = (GetKeyState(mu_windows_muKeyboardState_to_windows_key(i)) & 0x0001)!=0;
			if (mu_windows_keyboard_state_get_status(mu_windows_windows[win].input, i) != now) {
				mu_windows_input_keyboard_state_set_status(&mu_windows_windows[win].input, mu_windows_muKeyboardState_to_windows_key(i), now);
				if (mu_windows_windows[win].keyboard_state_callback != MU_NULL_PTR) {
					mu_windows_windows[win].keyboard_state_callback(win, i, now);
				}
			}
		}
	}

	// parse msg
	switch (uMsg) {

	default:
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		if (found_window_id) {
			mu_windows_windows[win].closed = MU_TRUE;
		}
		return 0;
		break;

	case WM_SIZE:
		if (found_window_id && mu_windows_windows[win].dimensions_callback != MU_NULL_PTR) {
			mu_windows_windows[win].dimensions_callback(win, (int)LOWORD(lParam), (int)HIWORD(lParam));
		}

		if (found_window_id && wParam == SIZE_MAXIMIZED && mu_windows_windows[win].maximize_callback != MU_NULL_PTR) {
			mu_windows_windows[win].maximize_callback(win, MU_TRUE);
			mu_windows_windows[win].maximized = MU_TRUE;
		} else if (found_window_id && 
			mu_windows_windows[win].maximized == MU_TRUE &&
			wParam != SIZE_MAXIMIZED && 
			wParam != SIZE_MAXHIDE && 
			wParam != SIZE_MAXSHOW && 
			mu_windows_windows[win].maximize_callback != MU_NULL_PTR) {
			mu_windows_windows[win].maximize_callback(win, MU_FALSE);
			mu_windows_windows[win].maximized = MU_FALSE;
		}

		if (found_window_id && wParam == SIZE_MINIMIZED && mu_windows_windows[win].minimize_callback != MU_NULL_PTR) {
			mu_windows_windows[win].minimize_callback(win, MU_TRUE);
			mu_windows_windows[win].minimized = MU_TRUE;
		} else if (found_window_id && 
			mu_windows_windows[win].minimized == MU_TRUE &&
			wParam != SIZE_MINIMIZED && 
			wParam != SIZE_MAXHIDE && 
			wParam != SIZE_MAXSHOW && 
			mu_windows_windows[win].minimize_callback != MU_NULL_PTR) {
			mu_windows_windows[win].minimize_callback(win, MU_FALSE);
			mu_windows_windows[win].minimized = MU_FALSE;
		}

		PostMessage(hwnd, WM_PAINT, 0, 0);
		return 0;
		break;

	case WM_KEYDOWN:
		if (found_window_id) {
			wParam = mu_map_left_right_keys(wParam, lParam);
			if (
				mu_windows_windows[win].keyboard_callback != MU_NULL_PTR &&
				mu_windows_keyboard_input_get_status(mu_windows_windows[win].input, mu_windows_windows_key_to_mu_keyboard(wParam)) == MU_FALSE
			) {
				mu_windows_windows[win].keyboard_callback(win, mu_windows_windows_key_to_mu_keyboard(wParam), MU_KEYBOARD_KEY_BIT_DOWN);
			}
			mu_windows_input_keyboard_set_status(&mu_windows_windows[win].input, wParam, MU_KEYBOARD_KEY_BIT_DOWN);
		}
		return 0;
		break;

	case WM_KEYUP:
		if (found_window_id) {
			wParam = mu_map_left_right_keys(wParam, lParam);
			mu_windows_input_keyboard_set_status(&mu_windows_windows[win].input, wParam, MU_KEYBOARD_KEY_BIT_UP);
			if (mu_windows_windows[win].keyboard_callback != MU_NULL_PTR) {
				mu_windows_windows[win].keyboard_callback(win, mu_windows_windows_key_to_mu_keyboard(wParam), MU_KEYBOARD_KEY_BIT_UP);
			}
		}
		return 0;
		break;

	case WM_MOVE:
		if (found_window_id && mu_windows_windows[win].position_callback != MU_NULL_PTR) {
			// https://www.autohotkey.com/boards/viewtopic.php?t=27857
			mu_windows_windows[win].position_callback(
				win,
				( lParam      & 0x8000 ? - ((~lParam    )&0x7FFF)+1 :  lParam     &0x7FFF),
				((lParam>>16) & 0x8000 ? - ((~lParam>>16)&0x7FFF)+1 : (lParam>>16)&0x7FFF)
			);
		}
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
		break;

	case WM_SETFOCUS:
		if (found_window_id && mu_windows_windows[win].focus_callback != MU_NULL_PTR) {
			mu_windows_windows[win].focus_callback(win, MU_TRUE);
		}
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
		break;
	
	case WM_KILLFOCUS:
		if (found_window_id && mu_windows_windows[win].focus_callback != MU_NULL_PTR) {
			mu_windows_windows[win].focus_callback(win, MU_FALSE);
		}
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
		break;

	case WM_GETMINMAXINFO:
		if (found_window_id) {
			LPMINMAXINFO lpmmi = (LPMINMAXINFO)lParam;
			// no clue why 16 and 39 have to be added here.
			// maybe other things about the window like the titlebar?
			lpmmi->ptMinTrackSize.x = mu_windows_windows[win].minimum_width + 16;
			lpmmi->ptMinTrackSize.y = mu_windows_windows[win].minimum_height + 39;
			lpmmi->ptMaxTrackSize.x = mu_windows_windows[win].maximum_width + 16;
			lpmmi->ptMaxTrackSize.y = mu_windows_windows[win].maximum_height + 39;
		}

		return 0;
		break;
	
	case WM_LBUTTONDOWN:
		if (found_window_id) {
			mu_windows_input_mouse_set_status(&mu_windows_windows[win].input, VK_LBUTTON, MU_MOUSE_BUTTON_BIT_DOWN);
			if (mu_windows_windows[win].mouse_callback != MU_NULL_PTR) {
				mu_windows_windows[win].mouse_callback(win, mu_windows_windows_key_to_mu_mouse(VK_LBUTTON), MU_MOUSE_BUTTON_BIT_DOWN);
			}
		}
		return 0;
		break;
	case WM_LBUTTONUP:
		if (found_window_id) {
			mu_windows_input_mouse_set_status(&mu_windows_windows[win].input, VK_LBUTTON, MU_MOUSE_BUTTON_BIT_UP);
			if (mu_windows_windows[win].mouse_callback != MU_NULL_PTR) {
				mu_windows_windows[win].mouse_callback(win, mu_windows_windows_key_to_mu_mouse(VK_LBUTTON), MU_MOUSE_BUTTON_BIT_UP);
			}
		}
		return 0;
		break;
	case WM_RBUTTONDOWN:
		if (found_window_id) {
			mu_windows_input_mouse_set_status(&mu_windows_windows[win].input, VK_RBUTTON, MU_MOUSE_BUTTON_BIT_DOWN);
			if (mu_windows_windows[win].mouse_callback != MU_NULL_PTR) {
				mu_windows_windows[win].mouse_callback(win, mu_windows_windows_key_to_mu_mouse(VK_RBUTTON), MU_MOUSE_BUTTON_BIT_DOWN);
			}
		}
		return 0;
		break;
	case WM_RBUTTONUP:
		if (found_window_id) {
			mu_windows_input_mouse_set_status(&mu_windows_windows[win].input, VK_RBUTTON, MU_MOUSE_BUTTON_BIT_UP);
			if (mu_windows_windows[win].mouse_callback != MU_NULL_PTR) {
				mu_windows_windows[win].mouse_callback(win, mu_windows_windows_key_to_mu_mouse(VK_RBUTTON), MU_MOUSE_BUTTON_BIT_UP);
			}
		}
		return 0;
		break;
	case WM_MBUTTONDOWN:
		if (found_window_id) {
			mu_windows_input_mouse_set_status(&mu_windows_windows[win].input, VK_MBUTTON, MU_MOUSE_BUTTON_BIT_DOWN);
			if (mu_windows_windows[win].mouse_callback != MU_NULL_PTR) {
				mu_windows_windows[win].mouse_callback(win, mu_windows_windows_key_to_mu_mouse(VK_MBUTTON), MU_MOUSE_BUTTON_BIT_DOWN);
			}
		}
		return 0;
		break;
	case WM_MBUTTONUP:
		if (found_window_id) {
			mu_windows_input_mouse_set_status(&mu_windows_windows[win].input, VK_MBUTTON, MU_MOUSE_BUTTON_BIT_UP);
			if (mu_windows_windows[win].mouse_callback != MU_NULL_PTR) {
				mu_windows_windows[win].mouse_callback(win, mu_windows_windows_key_to_mu_mouse(VK_MBUTTON), MU_MOUSE_BUTTON_BIT_UP);
			}
		}
		return 0;
		break;

	case WM_SETCURSOR:
		if (found_window_id && LOWORD(lParam) == HTCLIENT) {
			if (mu_windows_windows[win].cursor_style != MU_CURSOR_STYLE_DEFAULT) {
				DestroyCursor(mu_windows_windows[win].cursor);
				mu_windows_windows[win].cursor = LoadCursor(MU_NULL_PTR, mu_windows_mu_cursor_to_windows_cursor(mu_windows_windows[win].cursor_style));
				SetCursor(mu_windows_windows[win].cursor);
				return TRUE;
			}
		}
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
		break;
	
	case WM_MOUSEWHEEL:
		if (found_window_id) {
			mu_windows_windows[win].scroll_level += GET_WHEEL_DELTA_WPARAM(wParam);
			if (mu_windows_windows[win].scroll_callback != MU_NULL_PTR) {
				mu_windows_windows[win].scroll_callback(win, GET_WHEEL_DELTA_WPARAM(wParam));
			}
		}
		return 0;
		break;

	}
}

// reallocs mu_windows_windows if new length is needed
void mu_windows_windows_size_check(size_m new_length) {
	if (mu_windows_windows_length == new_length) {
		return;
	}
	mu_windows_windows_length = new_length;
	mu_windows_windows = 
	(mu_windows_window*) mu_realloc(
		mu_windows_windows, 
		mu_windows_windows_length * sizeof(mu_windows_window)
	);
}

// grabs new id for new window
size_m mu_windows_get_new_window_id() {
	// scan for inactive window struct
	for (size_m i = 0; i < mu_windows_windows_length; i++) {
		if (mu_windows_windows[i].active == MU_FALSE) {
			return i;
		}
	}
	// if we failed to find one,
	// we need to reallocate a new one
	mu_windows_windows_size_check(mu_windows_windows_length + 1);
	mu_windows_windows[mu_windows_windows_length-1].active = MU_FALSE;
	return mu_windows_windows_length-1;
}

// returns if id is valid or not
muBool mu_windows_is_id_valid(muWindow win) {
	if (win >= mu_windows_windows_length ||
		mu_windows_windows[win].active == MU_FALSE //||
		//mu_windows_windows[win].closed == MU_TRUE
	) {
		return MU_FALSE;
	}
	return MU_TRUE;
}

muBool mu_windows_is_id_valid_closed(muWindow win) {
	if (win >= mu_windows_windows_length ||
		mu_windows_windows[win].active == MU_FALSE
	) {
		return MU_FALSE;
	}
	return MU_TRUE;
}

void mu_windows_unbind() {
	if (mu_windows_window_binded && mu_windows_is_id_valid(mu_windows_binded_window)) {
		if (MU_IS_OPENGL(mu_windows_windows[mu_windows_binded_window].api)) {
			MU_OPENGL_CALL(wglMakeCurrent(NULL, NULL));
		}
		mu_windows_window_binded = MU_FALSE;
	}
}

void mu_windows_bind(muWindow win) {
	if (mu_windows_window_binded && mu_windows_binded_window == win) {
		return;
	}

	mu_windows_unbind();
	if (MU_IS_OPENGL(mu_windows_windows[win].api)) {
		MU_OPENGL_CALL(wglMakeCurrent(mu_windows_windows[win].device_context, mu_windows_windows[win].opengl_context));
	}
	mu_windows_window_binded = MU_TRUE;
	mu_windows_binded_window = win;
}

// https://stackoverflow.com/questions/1695288/getting-the-current-time-in-milliseconds-from-the-system-clock-in-windows
double mu_windows_get_current_time() {
	FILETIME file_time;
	GetSystemTimeAsFileTime(&file_time);
	LONGLONG ll_now = (LONGLONG)file_time.dwLowDateTime + ((LONGLONG)(file_time.dwHighDateTime) << 32LL);
	return (double)ll_now / (double)1.0e7;
}

wchar_m* mu_windows_utf8_to_wchar(char* str) {
	int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	wchar_m* wstr = mu_malloc(len * sizeof(wchar_m));
	MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, len);
	return wstr;
}

/* initiation/termination */

muBool mu_windows_has_initiated = MU_FALSE;
double mu_windows_original_time = 0.f;

MUDEF void mu_COSA_init(muResult* result) {
	// init check
	if (mu_windows_has_initiated == MU_TRUE) {
		mu_print("[muCOSA] An attempt to initiate muCOSA was made after already being initiated.\n");
		if (result != MU_NULL) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_has_initiated = MU_TRUE;
	// initiate windows buffer
	mu_windows_windows_length = 1;
	mu_windows_windows = mu_malloc(sizeof(mu_windows_window) * mu_windows_windows_length);
	mu_windows_windows[0].active = MU_FALSE;
	mu_windows_windows[0].dimensions_callback = MU_NULL_PTR;
	mu_windows_windows[0].position_callback = MU_NULL_PTR;
	mu_windows_windows[0].focus_callback = MU_NULL_PTR;
	mu_windows_windows[0].maximize_callback = MU_NULL_PTR;
	mu_windows_windows[0].minimize_callback = MU_NULL_PTR;
	mu_windows_windows[0].keyboard_callback = MU_NULL_PTR;
	mu_windows_windows[0].keyboard_state_callback = MU_NULL_PTR;
	mu_windows_windows[0].mouse_callback = MU_NULL_PTR;
	mu_windows_windows[0].scroll_callback = MU_NULL_PTR;

	mu_windows_original_time = mu_windows_get_current_time();

	// return
	if (result != MU_NULL) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_COSA_term(muResult* result) {
	// term check
	if (mu_windows_has_initiated == MU_FALSE) {
		mu_print("[muCOSA] An attempt to terminate muCOSA was made even though muCOSA was already not running.\n");
		if (result != MU_NULL) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_unbind();

	for (size_m i = 0; i < mu_windows_windows_length; i++) {
		if (mu_windows_windows[i].active) {
			mu_window_destroy(MU_NULL_PTR, i);
		}
	}

	mu_windows_has_initiated = MU_FALSE;
	// free windows buffer
	if (mu_windows_windows != MU_NULL_PTR) {
		mu_free(mu_windows_windows);
		mu_windows_windows = MU_NULL_PTR;
		mu_windows_windows_length = 0;
	}

	// return
	if (result != MU_NULL) {
		*result = MU_SUCCESS;
	}
}

MUDEF double mu_time_get(muResult* result) {
	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return mu_windows_get_current_time() - mu_windows_original_time;
}

MUDEF void mu_time_set(muResult* result, double time) {
	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	mu_windows_original_time = mu_windows_original_time + mu_time_get(result) - time;
}

MUDEF size_m mu_clipboard_get(muResult* result, char* buffer_c, size_m len) {
	// https://stackoverflow.com/questions/14762456/getclipboarddatacf-text
	if (!OpenClipboard(MU_NULL_PTR)) {
		mu_print("[muCOSA] Failed to get clipboard; couldn't open clipboard.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return 0;
	}

	HANDLE data = GetClipboardData(CF_UNICODETEXT);
	if (data == MU_NULL_PTR) {
		mu_print("[muCOSA] Failed to get clipboard; couldn't retrieve clipboard data in a Unicode text format.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		CloseClipboard();
		return 0;
	}

	wchar_m* ptext = (wchar_m*)GlobalLock(data);
	if (ptext == MU_NULL_PTR) {
		mu_print("[muCOSA] Failed to get clipboard; couldn't retrieve clipboard text pointer.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		CloseClipboard();
		return 0;
	}
	
	size_m ptext_wstrlen = mu_wstrlen(ptext);
	size_m ptext_len = WideCharToMultiByte(CP_UTF8, 0, ptext, ptext_wstrlen, NULL, 0, NULL, NULL);
	if (buffer_c == MU_NULL_PTR) {
		if (result != MU_NULL_PTR) {
			*result = MU_SUCCESS;
		}
		GlobalUnlock(data);
		CloseClipboard();
		return ptext_len;
	}
	
	if (len < ptext_len) {
		mu_print("[muCOSA] Failed to get clipboard; buffer length is too small to store.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		GlobalUnlock(data);
		CloseClipboard();
		return ptext_len;
	}

	char* cptext = mu_malloc(ptext_len);
	WideCharToMultiByte(CP_UTF8, 0, ptext, ptext_wstrlen, cptext, ptext_len, NULL, NULL);
	
	for (size_m i = 0; i < len && i < ptext_len; i++) {
		buffer_c[i] = cptext[i];
	}

	mu_free(cptext);

	GlobalUnlock(data);
	CloseClipboard();
	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return ptext_len;
}

MUDEF void mu_clipboard_set(muResult* result, char* string_c) {
	wchar_m* string = mu_windows_utf8_to_wchar(string_c);
	// https://stackoverflow.com/questions/1264137/how-to-copy-string-to-clipboard-in-c
	// not sure if the "len * sizeof(wchar_m)" usage is correct here...
	size_m len = mu_wstrlen(string) + 1;
	
	HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, len * sizeof(wchar_m));
	memcpy(GlobalLock(mem), string, len * sizeof(wchar_m));
	GlobalUnlock(mem);
	OpenClipboard(MU_NULL_PTR);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, mem);
	CloseClipboard();

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	mu_free(string);
}

/* basic window funcs */

MUDEF muWindow mu_window_create(muResult* result, muGraphicsAPI api, muBool (*load_functions)(void), const char* name_c, unsigned int width, unsigned int height) {
	wchar_m* name = mu_windows_utf8_to_wchar((char*)name_c);
	mu_windows_unbind();

	// allocate class name
	muWindow win = mu_windows_get_new_window_id();
	wchar_m* class_name = mu_malloc(sizeof(wchar_m) * 2);
	class_name[0] = '!' + win;
	class_name[1] = '\0';

	// initialize mu_windows_window struct

	mu_windows_window window_struct = {
		.active = MU_FALSE,
		.closed = MU_FALSE,
		.maximized = MU_FALSE,
		.minimized = MU_FALSE,
		// (WNDCLASSEXW)
		.window_class = {
			.cbSize = sizeof(WNDCLASSEXW),                       // size of struct
			.style =         CS_HREDRAW | CS_VREDRAW | CS_OWNDC, // style
			.lpfnWndProc =   mu_windows_default_window_proc,   // window process function
			.cbClsExtra =    0,                                  // extra class allocation bytes
			.cbWndExtra =    0,                                  // extra window instance allocation bytes
			.hInstance =     mu_windows_get_hinstance(),       // hInstance
			.hIcon =         LoadIcon(0, IDI_WINLOGO),           // window icon
			.hCursor =       LoadCursor(0, IDC_ARROW),           // window cursor
			.hbrBackground = 0,                                  // background brush (0 is fine)
			.lpszMenuName =  name,                               // menu name
			.lpszClassName = class_name,                         // class name
			.hIconSm =       0                                   // small window icon
		},
		.dimensions_callback = MU_NULL_PTR,
		.position_callback = MU_NULL_PTR,
		.focus_callback = MU_NULL_PTR,
		.maximize_callback = MU_NULL_PTR,
		.minimize_callback = MU_NULL_PTR,
		.keyboard_callback = MU_NULL_PTR,
		.keyboard_state_callback = MU_NULL_PTR,
		.mouse_callback = MU_NULL_PTR,
		.scroll_callback = MU_NULL_PTR,

		.scroll_level = 0
	};
	if (!RegisterClassExW(&window_struct.window_class)) {
		mu_print("[muCOSA] Failed to register window class.\n");
		mu_free(class_name);
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		mu_free(name);
		return MU_NO_WINDOW;
	}

	// create window

	DWORD style = WS_OVERLAPPEDWINDOW;
	if (!mu_window_settings.resizable) {
		style = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
	}

	/*
	( lParam      & 0x8000 ? - ((~lParam    )&0x7FFF)+1 :  lParam     &0x7FFF) + (int)(GetSystemMetrics(SM_CXSIZEFRAME)),
				((lParam>>16) & 0x8000 ? - ((~lParam>>16)&0x7FFF)+1 : (lParam>>16)&0x7FFF) + (int)(GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(92))
	*/

	window_struct.window_handle = CreateWindowExW(
		0,                                    // extra window style
		class_name,                           // class name
		name,                                 // window name
		style,                                // window style
		mu_window_settings.x - (int)(GetSystemMetrics(SM_CXSIZEFRAME)),                                                     // x-position
		mu_window_settings.y - (int)(GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(92)), // y-position
		width,                                // width
		height,                               // height
		NULL,                                 // parent window
		NULL,                                 // menu (?)
		window_struct.window_class.hInstance, // hInstance
		NULL                                  // additional stuff
	);

	if (window_struct.window_handle == NULL) {
		mu_print("[muCOSA] Failed to create window.\n");
		mu_free(class_name);
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		mu_free(name);
		return MU_NO_WINDOW;
	}

	// assign window into window array
	mu_windows_windows[win] = window_struct;
	mu_windows_windows[win].input = (mu_windows_input){0};

	// get some more info
	mu_windows_windows[win].device_context = GetDC(mu_windows_windows[win].window_handle);
	mu_windows_windows[win].cursor_style = mu_window_settings.cursor_style;
	mu_windows_windows[win].cursor = LoadCursor(MU_NULL_PTR, mu_windows_mu_cursor_to_windows_cursor(mu_windows_windows[win].cursor_style));
	SetCursor(mu_windows_windows[win].cursor);

	// make api context

	// @TODO make option to share contexts (child windows?)

	mu_windows_windows[win].api = api;
	// opengl
	if (MU_IS_OPENGL(api)) {
		MU_OPENGL_CALL(
			if (!mu_windows_create_opengl_context(
					mu_windows_windows[win].device_context,
					&mu_windows_windows[win].opengl_context,
					api
				)
			) {
				if (result != MU_NULL_PTR) {
					*result = MU_FAILURE;
				}
				ReleaseDC(
					mu_windows_windows[win].window_handle,
					mu_windows_windows[win].device_context
				);
				DestroyWindow(mu_windows_windows[win].window_handle);
				mu_free(class_name);
				mu_free(name);
				return MU_NO_WINDOW;
			}
		);
	// no api
	} else if (api != MU_NO_GRAPHICS_API) {
		mu_print("[muCOSA] Unsupported/Excluded (#define MUCOSA_NO_...) graphics API for Windows.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		ReleaseDC(
			mu_windows_windows[win].window_handle,
			mu_windows_windows[win].device_context
		);
		DestroyWindow(mu_windows_windows[win].window_handle);
		mu_free(class_name);
		mu_free(name);
		return MU_NO_WINDOW;
	}

	if (load_functions != MU_NULL_PTR) {
	    if (!load_functions()) {
	    	mu_print("[muCOSA] Failed to load functions for graphics API.\n");
	    	if (result != MU_NULL_PTR) {
				*result = MU_FAILURE;
			}
			ReleaseDC(
				mu_windows_windows[win].window_handle,
				mu_windows_windows[win].device_context
			);
			if (MU_IS_OPENGL(mu_windows_windows[win].api)) {
				MU_OPENGL_CALL(wglDeleteContext(mu_windows_windows[win].opengl_context));
			}
			DestroyWindow(mu_windows_windows[win].window_handle);
			mu_free(class_name);
			mu_free(name);
			return MU_NO_WINDOW;
	    }
	}

	mu_windows_windows[win].class_name = class_name;
	mu_windows_windows[win].active = MU_TRUE;

	mu_windows_windows[win].minimum_width = mu_window_settings.minimum_width;
	mu_windows_windows[win].minimum_height = mu_window_settings.minimum_height;
	mu_windows_windows[win].maximum_width = mu_window_settings.maximum_width;
	mu_windows_windows[win].maximum_height = mu_window_settings.maximum_height;

	mu_windows_bind(win);

	mu_windows_windows[win].visible = mu_window_settings.visible;
	if (mu_windows_windows[win].visible) {
		if (mu_window_settings.maximized) {
			ShowWindow(mu_windows_windows[win].window_handle, SW_MAXIMIZE);
		} else if (mu_window_settings.minimized) {
			ShowWindow(mu_windows_windows[win].window_handle, SW_MINIMIZE);
		} else {
			ShowWindow(mu_windows_windows[win].window_handle, SW_NORMAL);
		}
	} else {
		ShowWindow(mu_windows_windows[win].window_handle, SW_HIDE);
	}

	UpdateWindow(mu_windows_windows[win].window_handle);

	// return
	mu_free(name);

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return win;
}

MUDEF void mu_window_destroy(muResult* result, muWindow win) {
	if (!mu_windows_is_id_valid_closed(win)) {
		mu_print("[muCOSA] Requested window ID for destruction is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	// unregister class
	UnregisterClassW(mu_windows_windows[win].class_name, mu_windows_windows[win].window_class.hInstance);
	mu_free(mu_windows_windows[win].class_name);

	// destroy context and window
	if (win == mu_windows_binded_window && mu_windows_window_binded) {
		mu_windows_unbind();
	}
	ReleaseDC(
		mu_windows_windows[win].window_handle,
		mu_windows_windows[win].device_context
	);
	if (MU_IS_OPENGL(mu_windows_windows[win].api)) {
		MU_OPENGL_CALL(wglDeleteContext(mu_windows_windows[win].opengl_context));
	}
	DestroyWindow(mu_windows_windows[win].window_handle);

	// open up to overriding
	mu_windows_windows[win].active = MU_FALSE;
	mu_windows_windows[win].closed = MU_FALSE;
	mu_windows_windows[win].dimensions_callback = MU_NULL_PTR;
	mu_windows_input_flush(&mu_windows_windows[win].input);

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_update(muResult* result, muWindow win) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for updating is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_bind(win);

	MSG msg = { 0 };
	while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			//mu_windows_windows[win].alive = MU_FALSE;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if (GetFocus() != mu_windows_windows[win].window_handle) {
		mu_windows_input_flush(&mu_windows_windows[win].input);
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_swap_buffers(muResult* result, muWindow win) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for swapping buffers is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_bind(win);

	if (MU_IS_OPENGL(mu_windows_windows[win].api)) {
		MU_OPENGL_CALL(SwapBuffers(mu_windows_windows[win].device_context));
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF muBool mu_window_get_closed(muResult* result, muWindow win) {
	if (!mu_windows_is_id_valid_closed(win)) {
		mu_print("[muCOSA] Requested window ID for checking if closed is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_TRUE;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return mu_windows_windows[win].closed;
}

MUDEF void mu_window_close(muResult* result, muWindow win) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for closing is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_window_set_visible(result, win, MU_FALSE);
	mu_windows_windows[win].closed = MU_TRUE;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF muBool mu_window_get_focused(muResult* result, muWindow win) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting focused state is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_FALSE;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return GetFocus() == mu_windows_windows[win].window_handle;
}

MUDEF void mu_window_focus(muResult* result, muWindow win) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for focusing is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	// https://stackoverflow.com/questions/71437203/proper-way-of-activating-a-window-using-winapi
	// a better approach is possible, but to do so i would need the automation api
	// and i'm not touching that with a ten foot pole
	SetForegroundWindow(mu_windows_windows[win].window_handle);
	if (GetForegroundWindow() != mu_windows_windows[win].window_handle) {
		SwitchToThisWindow(mu_windows_windows[win].window_handle, MU_TRUE);
		Sleep(2);
		SetForegroundWindow(mu_windows_windows[win].window_handle);
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_get_position(muResult* result, muWindow win, int* x, int* y) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting position is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	RECT rect = {0};
	GetWindowRect(mu_windows_windows[win].window_handle, &rect);

	if (x != MU_NULL_PTR) {
		// https://stackoverflow.com/questions/431470/window-border-width-and-height-in-win32-how-do-i-get-it
		*x = (int)rect.left + (int)(GetSystemMetrics(SM_CXSIZEFRAME));
	}
	if (y != MU_NULL_PTR) {
		// https://stackoverflow.com/questions/28524463/how-to-get-the-default-caption-bar-height-of-a-window-in-windows/28524464#28524464
		// 92 = SM_CXPADDEDBORDER
		*y = (int)rect.top + (int)((GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(92)));
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_position(muResult* result, muWindow win, int x, int y) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting position is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	x -= (int)(GetSystemMetrics(SM_CXSIZEFRAME));
	y -= (int)(GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(92));

	SetWindowPos(mu_windows_windows[win].window_handle, HWND_TOP, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE );

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_get_dimensions(muResult* result, muWindow win, unsigned int* width, unsigned int* height) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting dimensions is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	RECT rect = {0};
	GetWindowRect(mu_windows_windows[win].window_handle, &rect);

	if (width != MU_NULL_PTR) {
		*width = (int)(rect.right - rect.left);
	}
	if (height != MU_NULL_PTR) {
		*height = (int)(rect.bottom - rect.top);
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_dimensions(muResult* result, muWindow win, unsigned int width, unsigned int height) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting dimensions is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	SetWindowPos(mu_windows_windows[win].window_handle, HWND_TOP, 0, 0, width, height, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
	
	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF muBool mu_window_get_maximized(muResult* result, muWindow win) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting maximized state is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_FALSE;
	}

	WINDOWPLACEMENT wndpl = {0};
	wndpl.length = sizeof(wndpl);
	GetWindowPlacement(mu_windows_windows[win].window_handle, &wndpl);

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return wndpl.showCmd == SW_MAXIMIZE;
}

MUDEF void mu_window_set_maximized(muResult* result, muWindow win, muBool maximized) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting maximized state is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	if (mu_window_get_maximized(result, win) == maximized) return;

	if (maximized) {
		ShowWindow(mu_windows_windows[win].window_handle, SW_MAXIMIZE);
	} else {
		ShowWindow(mu_windows_windows[win].window_handle, SW_NORMAL);
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF muBool mu_window_get_minimized(muResult* result, muWindow win) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting minimized state is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_FALSE;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return IsIconic(mu_windows_windows[win].window_handle);
}

MUDEF void mu_window_set_minimized(muResult* result, muWindow win, muBool minimized) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting minimized state is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	if (mu_window_get_minimized(result, win) == minimized) return;

	if (minimized) {
		ShowWindow(mu_windows_windows[win].window_handle, SW_MINIMIZE);
	} else {
		OpenIcon(mu_windows_windows[win].window_handle);
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_get_minimum_dimensions(muResult* result, muWindow win, unsigned int* width, unsigned int* height) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting minimum dimensions is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	if (width != MU_NULL_PTR) {
		*width = mu_windows_windows[win].minimum_width;
	}
	if (height != MU_NULL_PTR) {
		*height = mu_windows_windows[win].minimum_height;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_minimum_dimensions(muResult* result, muWindow win, unsigned int width, unsigned int height) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting minimum dimensions is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_windows[win].minimum_width = width;
	mu_windows_windows[win].minimum_height = height;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_get_maximum_dimensions(muResult* result, muWindow win, unsigned int* width, unsigned int* height) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting maximum dimensions is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	if (width != MU_NULL_PTR) {
		*width = mu_windows_windows[win].maximum_width;
	}
	if (height != MU_NULL_PTR) {
		*height = mu_windows_windows[win].maximum_height;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_maximum_dimensions(muResult* result, muWindow win, unsigned int width, unsigned int height) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting maximum dimensions is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_windows[win].maximum_width = width;
	mu_windows_windows[win].maximum_height = height;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_get_mouse_position(muResult* result, muWindow win, int* x, int* y) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting mouse position is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	POINT point = {0};
	GetCursorPos(&point);
	int window_pos_x = 0, window_pos_y = 0;
	mu_window_get_position(result, win, &window_pos_x, &window_pos_y);

	if (x != MU_NULL_PTR) {
		*x = point.x - window_pos_x;
	}

	if (y != MU_NULL_PTR) {
		*y = point.y - window_pos_y;
	}
	
	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_mouse_position(muResult* result, muWindow win, int x, int y) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting mouse position is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	int win_x=0, win_y=0;
	mu_window_get_position(result, win, &win_x, &win_y);
	SetCursorPos(win_x+x, win_y+y);

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF muCursorStyle mu_window_get_cursor_style(muResult* result, muWindow win) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting cursor style is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_CURSOR_STYLE_UNKNOWN;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return mu_windows_windows[win].cursor_style;
}

MUDEF void mu_window_set_cursor_style(muResult* result, muWindow win, muCursorStyle style) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting cursor style is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_windows[win].cursor_style = style;
	DestroyCursor(mu_windows_windows[win].cursor);
	mu_windows_windows[win].cursor = LoadCursor(MU_NULL_PTR, mu_windows_mu_cursor_to_windows_cursor(mu_windows_windows[win].cursor_style));
	SetCursor(mu_windows_windows[win].cursor);

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF int mu_window_get_scroll_level(muResult* result, muWindow win) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting scroll level is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return 0;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return mu_windows_windows[win].scroll_level;
}

MUDEF void mu_window_set_scroll_level(muResult* result, muWindow win, int scroll_level) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting scroll level is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_windows[win].scroll_level = scroll_level;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_context(muResult* result, muWindow win) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting context is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_bind(win);

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_title(muResult* result, muWindow win, const char* title_c) {
	wchar_m* title = mu_windows_utf8_to_wchar((char*)title_c);
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting title is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		mu_free(title);
		return;
	}

	SetWindowTextW(mu_windows_windows[win].window_handle, title);

	mu_free(title);
	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF muBool mu_window_get_visible(muResult* result, muWindow win) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting visibility is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_FALSE;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return mu_windows_windows[win].visible;
}

MUDEF void mu_window_set_visible(muResult* result, muWindow win, muBool visible) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting visibility is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	if (visible == MU_FALSE && mu_windows_windows[win].visible == MU_TRUE) {
		ShowWindow(mu_windows_windows[win].window_handle, SW_HIDE);
	} else if (visible == MU_TRUE && mu_windows_windows[win].visible == MU_FALSE) {
		ShowWindow(mu_windows_windows[win].window_handle, SW_NORMAL);
	}

	mu_windows_windows[win].visible = visible;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF muKeyboardKeyBit mu_window_get_keyboard_key_bit(muResult* result, muWindow win, muKeyboardKey key) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting keyboard bit is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_KEYBOARD_KEY_BIT_UP;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return mu_windows_keyboard_input_get_status(mu_windows_windows[win].input, key);
}

MUDEF muKeyboardStateBit mu_window_get_keyboard_state_bit(muResult* result, muWindow win, muKeyboardState state) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting keyboard state bit is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_KEYBOARD_STATE_BIT_OFF;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return (GetKeyState(mu_windows_muKeyboardState_to_windows_key(state)) & 0x0001) != 0;
}

MUDEF muMouseButtonBit mu_window_get_mouse_button_bit(muResult* result, muWindow win, muMouseButton key) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting mouse bit is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_MOUSE_BUTTON_BIT_UP;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return mu_windows_mouse_input_get_status(mu_windows_windows[win].input, key);
}

MUDEF void mu_window_set_dimensions_callback(muResult* result, muWindow win, void (*dimensions_callback)(muWindow win, int new_width, int new_height)) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting dimensions callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_bind(win);
	mu_windows_windows[win].dimensions_callback = dimensions_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_position_callback(muResult* result, muWindow win, void (*position_callback)(muWindow win, int x, int y)) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting position callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_windows[win].position_callback = position_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_focus_callback(muResult* result, muWindow win, void (*focus_callback)(muWindow win, muBool focused)) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting focus callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_windows[win].focus_callback = focus_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_maximize_callback(muResult* result, muWindow win, void (*maximize_callback)(muWindow win, muBool maximized)) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting maximize callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_windows[win].maximize_callback = maximize_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_minimize_callback(muResult* result, muWindow win, void (*minimize_callback)(muWindow win, muBool minimized)) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting minimize callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_windows[win].minimize_callback = minimize_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_keyboard_callback(muResult* result, muWindow win, void (*keyboard_callback)(muWindow win, muKeyboardKey key, muKeyboardKeyBit bit)) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting key callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_windows[win].keyboard_callback = keyboard_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_keyboard_state_callback(muResult* result, muWindow win, void (*keyboard_state_callback)(muWindow win, muKeyboardState state, muKeyboardStateBit bit)) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting key state callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_windows[win].keyboard_state_callback = keyboard_state_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_mouse_callback(muResult* result, muWindow win, void (*mouse_callback)(muWindow win, muMouseButton key, muMouseButtonBit bit)) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting mouse callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_windows[win].mouse_callback = mouse_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_scroll_callback(muResult* result, muWindow win, void (*scroll_callback)(muWindow win, int scroll_level_add)) {
	if (!mu_windows_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting scroll callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_windows_windows[win].scroll_callback = scroll_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

/* opengl functions */

MUDEF void* mu_get_opengl_function_address(const char* name) {
	// https://stackoverflow.com/questions/76638441/how-to-init-glad-without-the-glfw-loader-using-windows-headers
	PROC p = wglGetProcAddress(name);

	if (p == 0 ||
	   (p == (PROC)0x1) || (p == (PROC)0x2) || (p == (PROC)0x3) ||
	   (p == (PROC)-1)) {
		HMODULE module = LoadLibraryA("opengl32.dll"); 
		p = (PROC)GetProcAddress(module, name);
	}
	
	return p;
}

#endif /* WINDOWS */

#ifdef linux
#define MUCOSA_LINUX

#define XK_3270
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>

#include <time.h>
#include <pthread.h>

/* OPENGL SETUP */
#ifndef MUCOSA_NO_OPENGL

// this kinda doesn't make sense
#ifndef MUCOSA_NO_INCLUDE_OPENGL
	#ifndef glClearColor
		#include <GL/gl.h>
	#endif
#endif
#include <GL/glx.h>

#define MU_OPENGL_CALL(stuff) stuff

// context creation
// https://apoorvaj.io/creating-a-modern-opengl-context/
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
muBool mu_linux_opengl_initiated = MU_FALSE;
muResult mu_linux_init_opengl(Display* display, GLXContext* context, muGraphicsAPI api) {
	if (mu_linux_opengl_initiated) {
		return MU_SUCCESS;
	}

	// choose framebuffer pixel format stuff

	int pixel_format_attributes[] = {
		GLX_RENDER_TYPE,    GLX_RGBA_BIT,
		GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
		GLX_SAMPLE_BUFFERS, MU_TRUE,
		GLX_DOUBLEBUFFER,   mu_window_settings.pixel_format.doublebuffer,
		GLX_RED_SIZE,       mu_window_settings.pixel_format.red_bits,
		GLX_GREEN_SIZE,     mu_window_settings.pixel_format.green_bits,
		GLX_BLUE_SIZE,      mu_window_settings.pixel_format.blue_bits,
		GLX_ALPHA_SIZE,     mu_window_settings.pixel_format.alpha_bits,
		GLX_DEPTH_SIZE,     mu_window_settings.pixel_format.depth_bits,
		GLX_STENCIL_SIZE,   mu_window_settings.pixel_format.stencil_bits,
		GLX_SAMPLES,        mu_window_settings.pixel_format.samples,
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
		mu_print("[muCOSA] Failed to find compatible framebuffer configuration.\n");
		return MU_FAILURE;
	}

	// create context

	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
	glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");

	if (!glXCreateContextAttribsARB) {
		mu_print("[muCOSA] Failed to find glX function to create an OpenGL context.\n");
		return MU_FAILURE;
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
		case MU_OPENGL_1_0:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 0;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_1_1:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 1;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_1_2:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 2;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_1_2_1:
		// What the hell do I do here?
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 2;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_1_3:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 3;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_1_4:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 4;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_1_5:
			opengl_attributes[1] = 1;
			opengl_attributes[3] = 5;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_2_0:
			opengl_attributes[1] = 2;
			opengl_attributes[3] = 0;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_2_1:
			opengl_attributes[1] = 2;
			opengl_attributes[3] = 0;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_3_0:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 0;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_3_1:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 1;
			opengl_attributes[4] = 0;
			opengl_attributes[5] = 0;
			break;
		case MU_OPENGL_3_2_CORE:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 2;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_3_2_COMPATIBILITY:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 2;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_3_3_CORE:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_3_3_COMPATIBILITY:
			opengl_attributes[1] = 3;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_0_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 0;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_0_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 0;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_1_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 1;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_1_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 1;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_2_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 2;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_2_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 2;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_3_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_3_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 3;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_4_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 4;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_4_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 4;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_5_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 5;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_5_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 5;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_6_CORE:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 6;
			opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			break;
		case MU_OPENGL_4_6_COMPATIBILITY:
			opengl_attributes[1] = 4;
			opengl_attributes[3] = 6;
			opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			break;
	}

	*context = glXCreateContextAttribsARB(display, fbc[0], MU_NULL, MU_TRUE, opengl_attributes);
	if (!*context) {
		mu_print("[muCOSA] Failed to create valid OpenGL context.\n");
		return MU_FAILURE;
	}

	//mu_linux_opengl_initiated = MU_SUCCESS;
	return MU_SUCCESS;
}

#else

#define MU_OPENGL_CALL(...)

#endif /* MUCOSA_NO_OPENGL */

/* keyboard input */

// https://www.cl.cam.ac.uk/~mgk25/ucs/keysymdef.h
int mu_linux_MUCOSA_KEYBOARD_to_linux_key(muKeyboardKey key) {
	switch (key) {
	default:
		return XK_VoidSymbol;
		break;
	case MU_KEYBOARD_KEY_BACKSPACE:
		return XK_BackSpace;
		break;
	case MU_KEYBOARD_KEY_TAB:
		return XK_Tab;
		break;
	case MU_KEYBOARD_KEY_CLEAR:
		return XK_Clear;
		break;
	case MU_KEYBOARD_KEY_RETURN:
		return XK_Return;
		break;
	case MU_KEYBOARD_KEY_PAUSE:
		return XK_Pause;
		break;
	case MU_KEYBOARD_KEY_ESCAPE:
		return XK_Escape;
		break;
	case MU_KEYBOARD_KEY_MODECHANGE:
		return XK_Mode_switch;
		break;
	case MU_KEYBOARD_KEY_SPACE:
		return XK_space;
		break;
	case MU_KEYBOARD_KEY_PRIOR:
		return XK_Prior;
		break;
	case MU_KEYBOARD_KEY_NEXT:
		return XK_Next;
		break;
	case MU_KEYBOARD_KEY_END:
		return XK_End;
		break;
	case MU_KEYBOARD_KEY_HOME:
		return XK_Home;
		break;
	case MU_KEYBOARD_KEY_LEFT:
		return XK_Left;
		break;
	case MU_KEYBOARD_KEY_UP:
		return XK_Up;
		break;
	case MU_KEYBOARD_KEY_RIGHT:
		return XK_Right;
		break;
	case MU_KEYBOARD_KEY_DOWN:
		return XK_Down;
		break;
	case MU_KEYBOARD_KEY_SELECT:
		return XK_Select;
		break;
	case MU_KEYBOARD_KEY_PRINT:
		return XK_Print;
		break;
	case MU_KEYBOARD_KEY_EXECUTE:
		return XK_Execute;
		break;
	case MU_KEYBOARD_KEY_INSERT:
		return XK_Insert;
		break;
	case MU_KEYBOARD_KEY_DELETE:
		return XK_Delete;
		break;
	case MU_KEYBOARD_KEY_HELP:
		return XK_Help;
		break;
	case MU_KEYBOARD_KEY_0:
		return XK_0;
		break;
	case MU_KEYBOARD_KEY_1:
		return XK_1;
		break;
	case MU_KEYBOARD_KEY_2:
		return XK_2;
		break;
	case MU_KEYBOARD_KEY_3:
		return XK_3;
		break;
	case MU_KEYBOARD_KEY_4:
		return XK_4;
		break;
	case MU_KEYBOARD_KEY_5:
		return XK_5;
		break;
	case MU_KEYBOARD_KEY_6:
		return XK_6;
		break;
	case MU_KEYBOARD_KEY_7:
		return XK_7;
		break;
	case MU_KEYBOARD_KEY_8:
		return XK_8;
		break;
	case MU_KEYBOARD_KEY_9:
		return XK_9;
		break;
	case MU_KEYBOARD_KEY_A:
		return XK_a;
		break;
	case MU_KEYBOARD_KEY_B:
		return XK_b;
		break;
	case MU_KEYBOARD_KEY_C:
		return XK_c;
		break;
	case MU_KEYBOARD_KEY_D:
		return XK_d;
		break;
	case MU_KEYBOARD_KEY_E:
		return XK_e;
		break;
	case MU_KEYBOARD_KEY_F:
		return XK_f;
		break;
	case MU_KEYBOARD_KEY_G:
		return XK_g;
		break;
	case MU_KEYBOARD_KEY_H:
		return XK_h;
		break;
	case MU_KEYBOARD_KEY_I:
		return XK_i;
		break;
	case MU_KEYBOARD_KEY_J:
		return XK_j;
		break;
	case MU_KEYBOARD_KEY_K:
		return XK_k;
		break;
	case MU_KEYBOARD_KEY_L:
		return XK_l;
		break;
	case MU_KEYBOARD_KEY_M:
		return XK_m;
		break;
	case MU_KEYBOARD_KEY_N:
		return XK_n;
		break;
	case MU_KEYBOARD_KEY_O:
		return XK_o;
		break;
	case MU_KEYBOARD_KEY_P:
		return XK_p;
		break;
	case MU_KEYBOARD_KEY_Q:
		return XK_q;
		break;
	case MU_KEYBOARD_KEY_R:
		return XK_r;
		break;
	case MU_KEYBOARD_KEY_S:
		return XK_s;
		break;
	case MU_KEYBOARD_KEY_T:
		return XK_t;
		break;
	case MU_KEYBOARD_KEY_U:
		return XK_u;
		break;
	case MU_KEYBOARD_KEY_V:
		return XK_v;
		break;
	case MU_KEYBOARD_KEY_W:
		return XK_w;
		break;
	case MU_KEYBOARD_KEY_X:
		return XK_x;
		break;
	case MU_KEYBOARD_KEY_Y:
		return XK_y;
		break;
	case MU_KEYBOARD_KEY_Z:
		return XK_z;
		break;
	case MU_KEYBOARD_KEY_LEFT_WINDOWS:
		return XK_Super_L;
		break;
	case MU_KEYBOARD_KEY_RIGHT_WINDOWS:
		return XK_Super_R;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_0:
		return XK_KP_0;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_1:
		return XK_KP_1;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_2:
		return XK_KP_2;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_3:
		return XK_KP_3;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_4:
		return XK_KP_4;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_5:
		return XK_KP_5;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_6:
		return XK_KP_6;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_7:
		return XK_KP_7;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_8:
		return XK_KP_8;
		break;
	case MU_KEYBOARD_KEY_NUMPAD_9:
		return XK_KP_9;
		break;
	case MU_KEYBOARD_KEY_MULTIPLY:
		return XK_KP_Multiply;
		break;
	case MU_KEYBOARD_KEY_ADD:
		return XK_KP_Add;
		break;
	case MU_KEYBOARD_KEY_SEPARATOR:
		return XK_KP_Separator;
		break;
	case MU_KEYBOARD_KEY_SUBTRACT:
		return XK_KP_Subtract;
		break;
	case MU_KEYBOARD_KEY_DECIMAL:
		return XK_KP_Decimal;
		break;
	case MU_KEYBOARD_KEY_DIVIDE:
		return XK_KP_Divide;
		break;
	case MU_KEYBOARD_KEY_F1:
		return XK_F1;
		break;
	case MU_KEYBOARD_KEY_F2:
		return XK_F2;
		break;
	case MU_KEYBOARD_KEY_F3:
		return XK_F3;
		break;
	case MU_KEYBOARD_KEY_F4:
		return XK_F4;
		break;
	case MU_KEYBOARD_KEY_F5:
		return XK_F5;
		break;
	case MU_KEYBOARD_KEY_F6:
		return XK_F6;
		break;
	case MU_KEYBOARD_KEY_F7:
		return XK_F7;
		break;
	case MU_KEYBOARD_KEY_F8:
		return XK_F8;
		break;
	case MU_KEYBOARD_KEY_F9:
		return XK_F9;
		break;
	case MU_KEYBOARD_KEY_F10:
		return XK_F10;
		break;
	case MU_KEYBOARD_KEY_F11:
		return XK_F11;
		break;
	case MU_KEYBOARD_KEY_F12:
		return XK_F12;
		break;
	case MU_KEYBOARD_KEY_F13:
		return XK_F13;
		break;
	case MU_KEYBOARD_KEY_F14:
		return XK_F14;
		break;
	case MU_KEYBOARD_KEY_F15:
		return XK_F15;
		break;
	case MU_KEYBOARD_KEY_F16:
		return XK_F16;
		break;
	case MU_KEYBOARD_KEY_F17:
		return XK_F17;
		break;
	case MU_KEYBOARD_KEY_F18:
		return XK_F18;
		break;
	case MU_KEYBOARD_KEY_F19:
		return XK_F19;
		break;
	case MU_KEYBOARD_KEY_F20:
		return XK_F20;
		break;
	case MU_KEYBOARD_KEY_F21:
		return XK_F21;
		break;
	case MU_KEYBOARD_KEY_F22:
		return XK_F22;
		break;
	case MU_KEYBOARD_KEY_F23:
		return XK_F23;
		break;
	case MU_KEYBOARD_KEY_F24:
		return XK_F24;
		break;
	case MU_KEYBOARD_KEY_NUMLOCK:
		return XK_Num_Lock;
		break;
	case MU_KEYBOARD_KEY_SCROLL:
		return XK_Scroll_Lock;
		break;
	case MU_KEYBOARD_KEY_LEFT_SHIFT:
		return XK_Shift_L;
		break;
	case MU_KEYBOARD_KEY_RIGHT_SHIFT:
		return XK_Shift_R;
		break;
	case MU_KEYBOARD_KEY_LEFT_CONTROL:
		return XK_Control_L;
		break;
	case MU_KEYBOARD_KEY_RIGHT_CONTROL:
		return XK_Control_R;
		break;
	case MU_KEYBOARD_KEY_LEFT_MENU:
		return XK_Menu;
		break;
	case MU_KEYBOARD_KEY_RIGHT_MENU:
		return XK_Menu;
		break;
	case MU_KEYBOARD_KEY_ATTN:
		return XK_3270_Attn;
		break;
	case MU_KEYBOARD_KEY_CRSEL:
		return XK_3270_CursorSelect;
		break;
	case MU_KEYBOARD_KEY_EXSEL:
		return XK_3270_ExSelect;
		break;
	case MU_KEYBOARD_KEY_EREOF:
		return XK_3270_EraseEOF;
		break;
	case MU_KEYBOARD_KEY_PLAY:
		return XK_3270_Play;
		break;
	case MU_KEYBOARD_KEY_PA1:
		return XK_3270_PA1;
		break;
	}
}

muKeyboardKey mu_linux_linux_key_to_mu_key(int key) {
	switch (key) {
	default:
		return MU_KEYBOARD_KEY_UNKNOWN;
		break;
	case XK_BackSpace:
		return MU_KEYBOARD_KEY_BACKSPACE;
		break;
	case XK_Tab:
		return MU_KEYBOARD_KEY_TAB;
		break;
	case XK_Clear:
		return MU_KEYBOARD_KEY_CLEAR;
		break;
	case XK_Return:
		return MU_KEYBOARD_KEY_RETURN;
		break;
	case XK_Menu:
		return MU_KEYBOARD_KEY_LEFT_MENU;
		break;
	case XK_Pause:
		return MU_KEYBOARD_KEY_PAUSE;
		break;
	case XK_Escape:
		return MU_KEYBOARD_KEY_ESCAPE;
		break;
	case XK_Mode_switch:
		return MU_KEYBOARD_KEY_MODECHANGE;
		break;
	case XK_space:
		return MU_KEYBOARD_KEY_SPACE;
		break;
	case XK_Prior:
		return MU_KEYBOARD_KEY_PRIOR;
		break;
	case XK_Next:
		return MU_KEYBOARD_KEY_NEXT;
		break;
	case XK_End:
		return MU_KEYBOARD_KEY_END;
		break;
	case XK_Home:
		return MU_KEYBOARD_KEY_HOME;
		break;
	case XK_Left:
		return MU_KEYBOARD_KEY_LEFT;
		break;
	case XK_Up:
		return MU_KEYBOARD_KEY_UP;
		break;
	case XK_Right:
		return MU_KEYBOARD_KEY_RIGHT;
		break;
	case XK_Down:
		return MU_KEYBOARD_KEY_DOWN;
		break;
	case XK_Select:
		return MU_KEYBOARD_KEY_SELECT;
		break;
	case XK_Print:
		return MU_KEYBOARD_KEY_PRINT;
		break;
	case XK_Execute:
		return MU_KEYBOARD_KEY_EXECUTE;
		break;
	case XK_Insert:
		return MU_KEYBOARD_KEY_INSERT;
		break;
	case XK_Delete:
		return MU_KEYBOARD_KEY_DELETE;
		break;
	case XK_Help:
		return MU_KEYBOARD_KEY_HELP;
		break;
	case XK_0:
		return MU_KEYBOARD_KEY_0;
		break;
	case XK_1:
		return MU_KEYBOARD_KEY_1;
		break;
	case XK_2:
		return MU_KEYBOARD_KEY_2;
		break;
	case XK_3:
		return MU_KEYBOARD_KEY_3;
		break;
	case XK_4:
		return MU_KEYBOARD_KEY_4;
		break;
	case XK_5:
		return MU_KEYBOARD_KEY_5;
		break;
	case XK_6:
		return MU_KEYBOARD_KEY_6;
		break;
	case XK_7:
		return MU_KEYBOARD_KEY_7;
		break;
	case XK_8:
		return MU_KEYBOARD_KEY_8;
		break;
	case XK_9:
		return MU_KEYBOARD_KEY_9;
		break;
	case XK_a:
		return MU_KEYBOARD_KEY_A;
		break;
	case XK_b:
		return MU_KEYBOARD_KEY_B;
		break;
	case XK_c:
		return MU_KEYBOARD_KEY_C;
		break;
	case XK_d:
		return MU_KEYBOARD_KEY_D;
		break;
	case XK_e:
		return MU_KEYBOARD_KEY_E;
		break;
	case XK_f:
		return MU_KEYBOARD_KEY_F;
		break;
	case XK_g:
		return MU_KEYBOARD_KEY_G;
		break;
	case XK_h:
		return MU_KEYBOARD_KEY_H;
		break;
	case XK_i:
		return MU_KEYBOARD_KEY_I;
		break;
	case XK_j:
		return MU_KEYBOARD_KEY_J;
		break;
	case XK_k:
		return MU_KEYBOARD_KEY_K;
		break;
	case XK_l:
		return MU_KEYBOARD_KEY_L;
		break;
	case XK_m:
		return MU_KEYBOARD_KEY_M;
		break;
	case XK_n:
		return MU_KEYBOARD_KEY_N;
		break;
	case XK_o:
		return MU_KEYBOARD_KEY_O;
		break;
	case XK_p:
		return MU_KEYBOARD_KEY_P;
		break;
	case XK_q:
		return MU_KEYBOARD_KEY_Q;
		break;
	case XK_r:
		return MU_KEYBOARD_KEY_R;
		break;
	case XK_s:
		return MU_KEYBOARD_KEY_S;
		break;
	case XK_t:
		return MU_KEYBOARD_KEY_T;
		break;
	case XK_u:
		return MU_KEYBOARD_KEY_U;
		break;
	case XK_v:
		return MU_KEYBOARD_KEY_V;
		break;
	case XK_w:
		return MU_KEYBOARD_KEY_W;
		break;
	case XK_x:
		return MU_KEYBOARD_KEY_X;
		break;
	case XK_y:
		return MU_KEYBOARD_KEY_Y;
		break;
	case XK_z:
		return MU_KEYBOARD_KEY_Z;
		break;
	case XK_Super_L:
		return MU_KEYBOARD_KEY_LEFT_WINDOWS;
		break;
	case XK_Super_R:
		return MU_KEYBOARD_KEY_RIGHT_WINDOWS;
		break;
	case XK_KP_0:
		return MU_KEYBOARD_KEY_NUMPAD_0;
		break;
	case XK_KP_1:
		return MU_KEYBOARD_KEY_NUMPAD_1;
		break;
	case XK_KP_2:
		return MU_KEYBOARD_KEY_NUMPAD_2;
		break;
	case XK_KP_3:
		return MU_KEYBOARD_KEY_NUMPAD_3;
		break;
	case XK_KP_4:
		return MU_KEYBOARD_KEY_NUMPAD_4;
		break;
	case XK_KP_5:
		return MU_KEYBOARD_KEY_NUMPAD_5;
		break;
	case XK_KP_6:
		return MU_KEYBOARD_KEY_NUMPAD_6;
		break;
	case XK_KP_7:
		return MU_KEYBOARD_KEY_NUMPAD_7;
		break;
	case XK_KP_8:
		return MU_KEYBOARD_KEY_NUMPAD_8;
		break;
	case XK_KP_9:
		return MU_KEYBOARD_KEY_NUMPAD_9;
		break;
	case XK_KP_Multiply:
		return MU_KEYBOARD_KEY_MULTIPLY;
		break;
	case XK_KP_Add:
		return MU_KEYBOARD_KEY_ADD;
		break;
	case XK_KP_Separator:
		return MU_KEYBOARD_KEY_SEPARATOR;
		break;
	case XK_KP_Subtract:
		return MU_KEYBOARD_KEY_SUBTRACT;
		break;
	case XK_KP_Decimal:
		return MU_KEYBOARD_KEY_DECIMAL;
		break;
	case XK_KP_Divide:
		return MU_KEYBOARD_KEY_DIVIDE;
		break;
	case XK_F1:
		return MU_KEYBOARD_KEY_F1;
		break;
	case XK_F2:
		return MU_KEYBOARD_KEY_F2;
		break;
	case XK_F3:
		return MU_KEYBOARD_KEY_F3;
		break;
	case XK_F4:
		return MU_KEYBOARD_KEY_F4;
		break;
	case XK_F5:
		return MU_KEYBOARD_KEY_F5;
		break;
	case XK_F6:
		return MU_KEYBOARD_KEY_F6;
		break;
	case XK_F7:
		return MU_KEYBOARD_KEY_F7;
		break;
	case XK_F8:
		return MU_KEYBOARD_KEY_F8;
		break;
	case XK_F9:
		return MU_KEYBOARD_KEY_F9;
		break;
	case XK_F10:
		return MU_KEYBOARD_KEY_F10;
		break;
	case XK_F11:
		return MU_KEYBOARD_KEY_F11;
		break;
	case XK_F12:
		return MU_KEYBOARD_KEY_F12;
		break;
	case XK_F13:
		return MU_KEYBOARD_KEY_F13;
		break;
	case XK_F14:
		return MU_KEYBOARD_KEY_F14;
		break;
	case XK_F15:
		return MU_KEYBOARD_KEY_F15;
		break;
	case XK_F16:
		return MU_KEYBOARD_KEY_F16;
		break;
	case XK_F17:
		return MU_KEYBOARD_KEY_F17;
		break;
	case XK_F18:
		return MU_KEYBOARD_KEY_F18;
		break;
	case XK_F19:
		return MU_KEYBOARD_KEY_F19;
		break;
	case XK_F20:
		return MU_KEYBOARD_KEY_F20;
		break;
	case XK_F21:
		return MU_KEYBOARD_KEY_F21;
		break;
	case XK_F22:
		return MU_KEYBOARD_KEY_F22;
		break;
	case XK_F23:
		return MU_KEYBOARD_KEY_F23;
		break;
	case XK_F24:
		return MU_KEYBOARD_KEY_F24;
		break;
	case XK_Num_Lock:
		return MU_KEYBOARD_KEY_NUMLOCK;
		break;
	case XK_Scroll_Lock:
		return MU_KEYBOARD_KEY_SCROLL;
		break;
	case XK_Shift_L:
		return MU_KEYBOARD_KEY_LEFT_SHIFT;
		break;
	case XK_Shift_R:
		return MU_KEYBOARD_KEY_RIGHT_SHIFT;
		break;
	case XK_Control_L:
		return MU_KEYBOARD_KEY_LEFT_CONTROL;
		break;
	case XK_Control_R:
		return MU_KEYBOARD_KEY_RIGHT_CONTROL;
		break;
	case XK_3270_Attn:
		return MU_KEYBOARD_KEY_ATTN;
		break;
	case XK_3270_CursorSelect:
		return MU_KEYBOARD_KEY_CRSEL;
		break;
	case XK_3270_ExSelect:
		return MU_KEYBOARD_KEY_EXSEL;
		break;
	case XK_3270_EraseEOF:
		return MU_KEYBOARD_KEY_EREOF;
		break;
	case XK_3270_Play:
		return MU_KEYBOARD_KEY_PLAY;
		break;
	case XK_3270_PA1:
		return MU_KEYBOARD_KEY_PA1;
		break;
	}
}

// https://tronche.com/gui/x/xlib/appendix/b/
int mu_linux_get_linux_cursor_style(muCursorStyle style) {
	switch (style) {
	default:
		return 2;
		break;
	case MU_CURSOR_STYLE_IBEAM:
		return 152;
		break;
	case MU_CURSOR_STYLE_WAIT:
		return 150;
		break;
	case MU_CURSOR_STYLE_WAIT_ARROW:
		return 150;
		break;
	case MU_CURSOR_STYLE_CROSSHAIR:
		return 34;
		break;
	case MU_CURSOR_STYLE_HAND:
		return 58;
		break;
	case MU_CURSOR_STYLE_SIZE_EAST_WEST:
		return 108;
		break;
	case MU_CURSOR_STYLE_SIZE_NORTH_SOUTH:
		return 116;
		break;
	case MU_CURSOR_STYLE_SIZE_NORTH_EAST_SOUTH_WEST:
		return 120;
		break;
	case MU_CURSOR_STYLE_SIZE_NORTH_WEST_SOUTH_EAST:
		return 120;
		break;
	case MU_CURSOR_STYLE_SIZE_ALL:
		return 52;
		break;
	case MU_CURSOR_STYLE_NO:
		return 0;
		break;
	}
}

struct mu_linux_input {
	muKeyboardKeyBit keyboard_down_status[MU_KEYBOARD_KEY_LAST-MU_KEYBOARD_KEY_FIRST+1];
	muKeyboardStateBit keyboard_state_status[MU_KEYBOARD_STATE_LAST-MU_KEYBOARD_STATE_FIRST+1];
	muMouseButtonBit mouse_down_status[MU_MOUSE_BUTTON_LAST-MU_MOUSE_BUTTON_FIRST+1];
};
typedef struct mu_linux_input mu_linux_input;

void mu_linux_input_keyboard_set_status(mu_linux_input* input, int linux_key, muKeyboardKeyBit bit) {
	muKeyboardKey key = mu_linux_linux_key_to_mu_key(linux_key);
	if (MU_IS_KEYBOARD_KEY(key)) {
		input->keyboard_down_status[key-MU_KEYBOARD_KEY_FIRST] = bit;
	}
}

muKeyboardKeyBit mu_linux_input_keyboard_get_status(mu_linux_input input, muKeyboardKey key) {
	if (MU_IS_KEYBOARD_KEY(key)) {
		return input.keyboard_down_status[key-MU_KEYBOARD_KEY_FIRST];
	}
	return MU_KEYBOARD_KEY_BIT_UP;
}

void mu_linux_input_keyboard_state_set_status(mu_linux_input* input, muKeyboardState state, muKeyboardStateBit bit) {
	if (MU_IS_KEYBOARD_STATE(state)) {
		input->keyboard_state_status[state-MU_KEYBOARD_STATE_FIRST] = bit;
	}
}

muKeyboardStateBit mu_linux_input_keyboard_state_get_status(mu_linux_input input, muKeyboardState state) {
	if (MU_IS_KEYBOARD_STATE(state)) {
		return input.keyboard_state_status[state-MU_KEYBOARD_STATE_FIRST];
	}
	return MU_KEYBOARD_STATE_BIT_ON;
}

void mu_linux_input_mouse_set_status(mu_linux_input* input, muMouseButton mouse, muMouseButtonBit bit) {
	if (MU_IS_MOUSE_BUTTON(mouse)) {
		input->mouse_down_status[mouse-MU_MOUSE_BUTTON_FIRST] = bit;
	}
}

muMouseButtonBit mu_linux_input_mouse_get_status(mu_linux_input input, muMouseButton key) {
	if (MU_IS_MOUSE_BUTTON(key)) {
		return input.mouse_down_status[key-MU_MOUSE_BUTTON_FIRST];
	}
	return MU_MOUSE_BUTTON_BIT_UP;
}

void mu_linux_input_flush(mu_linux_input* input) {
	for (size_m i = 0; i < MU_KEYBOARD_KEY_LAST-MU_KEYBOARD_KEY_FIRST+1; i++) {
		input->keyboard_down_status[i] = MU_KEYBOARD_KEY_BIT_UP;
	}
	for (size_m i = 0; i < MU_KEYBOARD_STATE_LAST-MU_KEYBOARD_STATE_FIRST+1; i++) {
		input->keyboard_state_status[i] = MU_KEYBOARD_STATE_BIT_OFF;
	}
	for (size_m i = 0; i < MU_MOUSE_BUTTON_LAST-MU_MOUSE_BUTTON_FIRST+1; i++) {
		input->mouse_down_status[i] = MU_MOUSE_BUTTON_BIT_UP;
	}
}

/* window setup */

struct mu_linux_window {
	muBool active;
	muBool closed;
	muBool visible;

	// display to render on, result of XOpenDisplay
	Display* display;
	// parent window, result of DefaultRootWindow
	Window parent_window;
	// actual window
	Window window;
	// graphics api
	muGraphicsAPI api;
	// cursor style
	muCursorStyle cursor_style;
	// cursor
	Cursor cursor;

	// event handling
	XEvent event;
	// delete message
	Atom delete_message;
	// input
	mu_linux_input input;

	// opengl context
	MU_OPENGL_CALL(GLXContext opengl_context);

	// callbacks
	void (*dimensions_callback)    (muWindow win, int new_width, int new_height);
	void (*position_callback)      (muWindow win, int x, int y);
	void (*focus_callback)         (muWindow win, muBool focused);
	void (*maximize_callback)      (muWindow win, muBool maximized);
	void (*minimize_callback)      (muWindow win, muBool minimized);
	void (*keyboard_callback)      (muWindow win, muKeyboardKey key, muKeyboardKeyBit bit);
	void (*keyboard_state_callback)(muWindow win, muKeyboardState state, muKeyboardStateBit bit);
	void (*mouse_callback)         (muWindow win, muMouseButton key, muMouseButtonBit bit);
	void (*scroll_callback)        (muWindow win, int scroll_level_add);

	// last-frame checks
	int x;
	int y;
	muBool maximized;
	muBool minimized;

	// total values
	int scroll_level;
	unsigned int minimum_width;
	unsigned int minimum_height;
	unsigned int maximum_width;
	unsigned int maximum_height;

	// window deco stuff
	muBool gotten_deco;
	long left;
	long right;
	long top;
	long bottom;
};
typedef struct mu_linux_window mu_linux_window;

mu_linux_window* mu_linux_windows = MU_NULL_PTR;
size_m mu_linux_windows_length = 0;

muBool mu_linux_window_binded = MU_FALSE;
muWindow mu_linux_binded_window = 0;

size_m mu_linux_get_new_window_id() {
	for (size_m i = 0; i < mu_linux_windows_length; i++) {
		if (mu_linux_windows[i].active == MU_FALSE) {
			return i;
		}
	}

	mu_linux_windows_length++;
	mu_linux_windows = (mu_linux_window*)mu_realloc(
		mu_linux_windows, 
		mu_linux_windows_length * sizeof(mu_linux_window)
	);

	return mu_linux_windows_length - 1;
}

muBool mu_linux_is_id_valid(muWindow win) {
	if (win >= mu_linux_windows_length || 
		mu_linux_windows[win].active == MU_FALSE //|| 
		//mu_linux_windows[win].closed == MU_TRUE
	) {
		return MU_FALSE;
	}
	return MU_TRUE;
}

muBool mu_linux_is_valid_closed(muWindow win) {
	if (win >= mu_linux_windows_length || 
		mu_linux_windows[win].active == MU_FALSE
	) {
		return MU_FALSE;
	}
	return MU_TRUE;
}

void mu_linux_window_unbind() {
	if (mu_linux_window_binded && mu_linux_is_id_valid(mu_linux_binded_window)) {

		if (MU_IS_OPENGL(mu_linux_windows[mu_linux_binded_window].api)) {
			MU_OPENGL_CALL(
				glXMakeCurrent(
					mu_linux_windows[mu_linux_binded_window].display,
					None,
					NULL
				);
			);
		}

		mu_linux_window_binded = MU_FALSE;
	}
}

void mu_linux_window_bind(muWindow win) {
	mu_linux_window_unbind();

	if (MU_IS_OPENGL(mu_linux_windows[mu_linux_binded_window].api)) {
		MU_OPENGL_CALL(
			glXMakeCurrent(
				mu_linux_windows[win].display,
				mu_linux_windows[win].window,
				mu_linux_windows[win].opengl_context
			);
		);
	}

	mu_linux_binded_window = win;
	mu_linux_window_binded = MU_TRUE;
}

// https://stackoverflow.com/questions/3756323/how-to-get-the-current-time-in-milliseconds-from-c-in-linux
double mu_linux_get_global_time() {
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
    return (double)spec.tv_sec + ((double)spec.tv_nsec / (double)1.0e9);
}

/* initiation / termination */

muBool mu_linux_is_initiated = MU_FALSE;
double mu_linux_original_time = 0.f;

MUDEF void mu_COSA_init(muResult* result) {
	if (mu_linux_is_initiated) {
		mu_print("[muCOSA] An attempt to initiate muCOSA despite already being initiated was made.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	if (mu_linux_windows == MU_NULL_PTR) {
		mu_linux_windows_length = 1;
		mu_linux_windows = mu_malloc(mu_linux_windows_length * sizeof(mu_linux_window));
		mu_linux_windows[0].active = MU_FALSE;
	}

	mu_linux_original_time = mu_linux_get_global_time();

	mu_linux_is_initiated = MU_TRUE;
	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_COSA_term(muResult* result) {
	if (!mu_linux_is_initiated) {
		mu_print("[muCOSA] An attempt to terminate muCOSA despite already being terminated (or never being initiated) was made.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	for (size_m i = 0; i < mu_linux_windows_length; i++) {
		if (mu_linux_windows[i].active) {
			mu_window_destroy(MU_NULL_PTR, i);
		}
	}

	if (mu_linux_windows != MU_NULL_PTR) {
		mu_free(mu_linux_windows);
		mu_linux_windows = MU_NULL_PTR;
		mu_linux_windows_length = 0;
	}

	mu_linux_is_initiated = MU_FALSE;
	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF double mu_time_get(muResult* result) {
	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return mu_linux_get_global_time() - mu_linux_original_time;
}

MUDEF void mu_time_set(muResult* result, double time) {
	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	mu_linux_original_time = mu_linux_original_time + mu_time_get(result) - time;
}

MUDEF size_m mu_clipboard_get(muResult* result, char* buffer, size_m len) {
	// https://www.uninformativ.de/blog/postings/2017-04-02/0/POSTING-en.html
	Display* d;
	Window w;

	if (mu_linux_windows_length != 0) {
		d = mu_linux_windows[0].display;
		w = mu_linux_windows[0].window;
	} else {
		d = XOpenDisplay(NULL);
		w = XCreateSimpleWindow(d, RootWindow(d, DefaultScreen(d)), -10, -10, 1, 1, 0, 0, 0);
	}

	Atom sel = XInternAtom(d, "CLIPBOARD", False);
	Atom utf8 = XInternAtom(d, "UTF8_STRING", False);

	Window owner = XGetSelectionOwner(d, sel);
	if (owner == None) {
		mu_print("[muCOSA] Failed to get clipboard; no clipboard found.\n");
		return 0;
	}

	// maybe needs to be different per window
	Atom p = XInternAtom(d, "PENGUIN", False);

	XConvertSelection(
		d, sel, utf8, p, w, CurrentTime
	);

	XEvent ev;
	muBool got_notify = MU_FALSE;
	while (!got_notify) {
		XNextEvent(d, &ev);
		switch (ev.type) {
			case SelectionNotify: {
				got_notify = MU_TRUE;
				XSelectionEvent* sev = (XSelectionEvent*)&ev.xselection;
				if (sev->property == None) {
					mu_print("[muCOSA] Failed to get clipboard; failed to get clipboard in UTF-8 format.\n");
					return 0;
				} else {
					Atom da, incr, type;
					int di;
					unsigned long size, dul;
					unsigned char* prop_ret = NULL;

					XGetWindowProperty(
						d, w, p, 0, 0, False, AnyPropertyType,
						&type, &di, &dul, &size, &prop_ret
					);
					XFree(prop_ret);

					incr = XInternAtom(d, "INCR", False);
					if (type == incr) {
						// @TODO INCR
						mu_print("[muCOSA] Failed to get clipboard; data too large, implement INCR later please!\n");
						return 0;
					}

					if (size == 0) {
						mu_print("[MUGA} Failed to get clipboard; failed to retrieve data.\n");
						return 0;
					}
					
					if (buffer == MU_NULL_PTR) {
						return size;
					}

					XGetWindowProperty(
						d, w, p, 0, size, False, AnyPropertyType,
						&da, &di, &dul, &dul, &prop_ret
					);

					for (size_m i = 0; i < len && i < size; i++) {
						buffer[i] = (char)prop_ret[i];
					}
					
					XFree(prop_ret);

					XDeleteProperty(d, w, p);
				}
				break;
			}
		}
	}

	if (mu_linux_windows_length == 0) {
		XDestroyWindow(d, w);
		XCloseDisplay(d);
	}
	return 0;
}

muBool mu_linux_global_clipboard_stop = MU_FALSE;
muBool mu_linux_clipboard_thread_is_running = MU_FALSE;

void mu_linux_clipboard_set(char* string) {
	mu_linux_clipboard_thread_is_running = MU_TRUE;
	// https://www.uninformativ.de/blog/postings/2017-04-02/0/POSTING-en.html
	Display* d;
	Window w;

	d = XOpenDisplay(NULL);
	w = XCreateSimpleWindow(d, RootWindow(d, DefaultScreen(d)), -10, -10, 1, 1, 0, 0, 0);

	Atom sel = XInternAtom(d, "CLIPBOARD", False);
	Atom utf8 = XInternAtom(d, "UTF8_STRING", False);

	XSetSelectionOwner(d, sel, w, CurrentTime);

	XEvent ev;
	muBool got_event = MU_FALSE;
	while (!got_event && !mu_linux_global_clipboard_stop) {
		XNextEvent(d, &ev);
		switch (ev.type) {
			// we lost our ownership
			case SelectionClear: {
				got_event = MU_TRUE;
				break;
			}
			// somebody has asked for the clipboard
			case SelectionRequest: {
				XSelectionRequestEvent* sev = (XSelectionRequestEvent*)&ev.xselectionrequest;
				if (sev->target != utf8 || sev->property == None) {
					// "sorry i can't give you it"
					XSelectionEvent ssev;
					char* an;

					an = XGetAtomName(d, sev->target);
					if (an) {
						XFree(an);
					}

					ssev.type = SelectionNotify;
					ssev.requestor = sev->requestor;
					ssev.selection = sev->selection;
					ssev.target = sev->target;
					ssev.property = None;
					ssev.time = sev->time;

					XSendEvent(d, sev->requestor, True, NoEventMask, (XEvent*)&ssev);
				} else {
					// "ok here"
					XSelectionEvent ssev;
					char* an;

					an = XGetAtomName(d, sev->property);
					if (an) {
						XFree(an);
					}
					XChangeProperty(
						d, sev->requestor, sev->property, utf8, 8, PropModeReplace,
						(unsigned char*)string, mu_strlen(string)
					);

					ssev.type = SelectionNotify;
					ssev.requestor = sev->requestor;
					ssev.selection = sev->selection;
					ssev.target = sev->target;
					ssev.property = sev->property;
					ssev.time= sev->time;

					XSendEvent(d, sev->requestor, True, NoEventMask, (XEvent*)&ssev);
				}
				break;
			}
		}
	}

	XDestroyWindow(d, w);
	XCloseDisplay(d);
	mu_linux_clipboard_thread_is_running = MU_FALSE;
}

pthread_t mu_linux_clipboard_set_thread;

MUDEF void mu_clipboard_set(muResult* result, char* string) {
	if (mu_linux_clipboard_thread_is_running) {
		mu_linux_global_clipboard_stop = MU_TRUE;
		pthread_join(mu_linux_clipboard_set_thread, NULL);
	}
	/*int ret = */pthread_create(&mu_linux_clipboard_set_thread, NULL, (void*)mu_linux_clipboard_set, (void*)string);
}

/* window functions */

MUDEF muWindow mu_window_create(
	muResult* result, 
	muGraphicsAPI api, 
	muBool (*load_functions)(void), 
	const char* name, 
	unsigned int width, unsigned int height
) {
	// window creation
	muWindow win = (muWindow)mu_linux_get_new_window_id();

	mu_linux_windows[win].display = XOpenDisplay(MU_NULL_PTR);
	mu_linux_windows[win].parent_window = DefaultRootWindow(mu_linux_windows[win].display);

	// Would like to not use XCreateSimpleWindow here but doing
	// otherwise makes me pick pixel formatting before picking
	// it with an API, so I'm kinda stuck here
	mu_linux_windows[win].window = XCreateSimpleWindow(
		mu_linux_windows[win].display,       // display
		mu_linux_windows[win].parent_window, // parent window
		mu_window_settings.x,                // position
		mu_window_settings.y,
		width, height,                         // dimensions
		0, 0,                        // border width and border color
		0                            // background
	);
	
	XSelectInput(
		mu_linux_windows[win].display,
		mu_linux_windows[win].window,
		ExposureMask | 
		KeyPressMask | 
		KeyReleaseMask | 
		StructureNotifyMask | 
		FocusChangeMask |
		PointerMotionMask |
		ButtonPressMask |
		ButtonReleaseMask
	);

	// cursor

	mu_linux_windows[win].cursor_style = mu_window_settings.cursor_style;
	mu_linux_windows[win].cursor = XCreateFontCursor(mu_linux_windows[win].display, mu_linux_get_linux_cursor_style(mu_linux_windows[win].cursor_style));
	XDefineCursor(mu_linux_windows[win].display, mu_linux_windows[win].window, mu_linux_windows[win].cursor);

	// api initialization

	mu_linux_windows[win].api = api;
	if (MU_IS_OPENGL(api)) {
		MU_OPENGL_CALL (
			if (mu_linux_init_opengl(
				mu_linux_windows[win].display,
				&mu_linux_windows[win].opengl_context,
				api
			) == MU_FAILURE) {
				if (result != MU_NULL_PTR) {
					*result = MU_FAILURE;
				}
				XUndefineCursor(mu_linux_windows[win].display, mu_linux_windows[win].window);
				XFreeCursor(mu_linux_windows[win].display, mu_linux_windows[win].cursor);
				XDestroyWindow(
					mu_linux_windows[win].display,
					mu_linux_windows[win].window
				);
				XCloseDisplay(mu_linux_windows[win].display);
				return MU_NO_WINDOW;
			}
		);
	} else if (api != MU_NO_GRAPHICS_API) {
		mu_print("[muCOSA] Unsupported/Excluded (#define MUCOSA_NO_...) graphics API for Linux.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		XUndefineCursor(mu_linux_windows[win].display, mu_linux_windows[win].window);
		XFreeCursor(mu_linux_windows[win].display, mu_linux_windows[win].cursor);
		XDestroyWindow(
			mu_linux_windows[win].display,
			mu_linux_windows[win].window
		);
		XCloseDisplay(mu_linux_windows[win].display);
		return MU_NO_WINDOW;
	}

	// define more info

	// delete message
	// https://stackoverflow.com/questions/10792361/how-do-i-gracefully-exit-an-x11-event-loop
	mu_linux_windows[win].delete_message = XInternAtom(
		mu_linux_windows[win].display, "WM_DELETE_WINDOW", MU_FALSE
	);
	XSetWMProtocols(
		mu_linux_windows[win].display, 
		mu_linux_windows[win].window,
		&mu_linux_windows[win].delete_message,
		1
	);

	// callbacks
	mu_linux_windows[win].dimensions_callback = MU_NULL_PTR;
	mu_linux_windows[win].position_callback = MU_NULL_PTR;
	mu_linux_windows[win].focus_callback = MU_NULL_PTR;
	mu_linux_windows[win].maximize_callback = MU_NULL_PTR;
	mu_linux_windows[win].minimize_callback = MU_NULL_PTR;
	mu_linux_windows[win].keyboard_callback = MU_NULL_PTR;
	mu_linux_windows[win].keyboard_state_callback = MU_NULL_PTR;
	mu_linux_windows[win].mouse_callback = MU_NULL_PTR;
	mu_linux_windows[win].scroll_callback = MU_NULL_PTR;

	// last checks
	mu_linux_windows[win].x = mu_window_settings.x;
	mu_linux_windows[win].y = mu_window_settings.y;

	// reset values
	mu_linux_windows[win].scroll_level = 0;
	mu_linux_windows[win].gotten_deco = MU_FALSE;

	// display window

	XSizeHints    my_hints = {0};

	my_hints.flags  = PPosition | PSize;
	my_hints.x      = 0;
	my_hints.y      = 0;
	my_hints.width  = 0; // not sure if setting width and
	my_hints.height = 0; // height to 0 here is safe...
	if (!mu_window_settings.resizable) {
		my_hints.flags |= PMinSize | PMaxSize;
		my_hints.min_width =  width;
		my_hints.min_height = height;
		my_hints.max_width =  width;
		my_hints.max_height = height;
	}

	XSetNormalHints(mu_linux_windows[win].display, mu_linux_windows[win].window, &my_hints);

	mu_linux_windows[win].active = MU_TRUE;
	mu_linux_windows[win].closed = MU_FALSE;
	mu_linux_windows[win].input = (mu_linux_input){0};
	mu_linux_windows[win].visible = mu_window_settings.visible;

	if (mu_linux_windows[win].visible) {
		XMapWindow(mu_linux_windows[win].display, mu_linux_windows[win].window);
	}

	mu_linux_window_bind(win);

	// (to adjust for window decoration)
	mu_window_set_position(result, win, mu_window_settings.x, mu_window_settings.y);

	// window title

	XChangeProperty(
		mu_linux_windows[win].display, mu_linux_windows[win].window,
		XInternAtom(mu_linux_windows[win].display, "_NET_WM_NAME", False),
		XInternAtom(mu_linux_windows[win].display, "UTF8_STRING", False),
		8, PropModeReplace, (unsigned char*)name, mu_strlen(name)
	);

	// load api

	if (load_functions != MU_NULL_PTR) {
	    if (!load_functions()) {
	    	mu_print("[muCOSA] Failed to load functions for graphics API.\n");
	    	if (result != MU_NULL_PTR) {
				*result = MU_FAILURE;
			}
			if (MU_IS_OPENGL(mu_linux_windows[win].api)) {
				MU_OPENGL_CALL(
					glXDestroyContext(mu_linux_windows[win].display, mu_linux_windows[win].opengl_context);
				);
			}
			XUndefineCursor(mu_linux_windows[win].display, mu_linux_windows[win].window);
			XFreeCursor(mu_linux_windows[win].display, mu_linux_windows[win].cursor);
			XDestroyWindow(
				mu_linux_windows[win].display,
				mu_linux_windows[win].window
			);
			XCloseDisplay(mu_linux_windows[win].display);
			mu_linux_windows[win].active = MU_FALSE;
			return MU_NO_WINDOW;
	    }
	}

	mu_linux_windows[win].maximized = mu_window_settings.maximized;
	mu_linux_windows[win].minimized = mu_window_settings.minimized;

	XSizeHints* sizeHints = XAllocSizeHints();
	sizeHints->flags = PMinSize | PMaxSize;
	sizeHints->min_width = mu_window_settings.minimum_width;
	sizeHints->min_height = mu_window_settings.minimum_height;
	sizeHints->max_width = mu_window_settings.maximum_width;
	sizeHints->max_height = mu_window_settings.maximum_height;
	XSetWMNormalHints(
		mu_linux_windows[win].display,
		mu_linux_windows[win].window,
		sizeHints
	);
	XFree(sizeHints);

	mu_linux_windows[win].minimum_width = mu_window_settings.minimum_width;
	mu_linux_windows[win].minimum_height = mu_window_settings.minimum_height;
	mu_linux_windows[win].maximum_width = mu_window_settings.maximum_width;
	mu_linux_windows[win].maximum_height = mu_window_settings.maximum_height;

	// handle more window settings

	if (mu_window_settings.maximized) {
		mu_window_set_maximized(result, win, MU_TRUE);
	}

	if (mu_window_settings.minimized) {
		mu_window_set_minimized(result, win, MU_TRUE);
	}

	// return success

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return win;
}

MUDEF void mu_window_destroy(muResult* result, muWindow win) {
	if (!mu_linux_is_valid_closed(win)) {
		mu_print("[muCOSA] Requested window ID for destruction is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	if (win == mu_linux_binded_window) {
		mu_linux_window_unbind();
	}

	// not sure if anything else needs to be destroyed...
	if (MU_IS_OPENGL(mu_linux_windows[win].api)) {
		MU_OPENGL_CALL(
			glXDestroyContext(mu_linux_windows[win].display, mu_linux_windows[win].opengl_context);
		);
	}

	XUndefineCursor(mu_linux_windows[win].display, mu_linux_windows[win].window);
	XFreeCursor(mu_linux_windows[win].display, mu_linux_windows[win].cursor);

	XDestroyWindow(
		mu_linux_windows[win].display,
		mu_linux_windows[win].window
	);
	XCloseDisplay(mu_linux_windows[win].display);

	mu_linux_input_flush(&mu_linux_windows[win].input);

	mu_linux_windows[win].active = MU_FALSE;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_update(muResult* result, muWindow win) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for updating is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_linux_input pre_input = mu_linux_windows[win].input;

	XWindowAttributes attributes;
	XGetWindowAttributes(
		mu_linux_windows[win].display,
		mu_linux_windows[win].window,
		&attributes
	);

	while (XPending(mu_linux_windows[win].display)) {
		// https://stackoverflow.com/questions/36188154/get-x11-window-caption-height
		if (mu_linux_windows[win].gotten_deco == MU_FALSE) {
			Atom a = XInternAtom(mu_linux_windows[win].display, "_NET_FRAME_EXTENTS", True);
			Atom t;
			int f;
			unsigned long _n, b;
			unsigned char* data = 0;

			int r = XGetWindowProperty(
				mu_linux_windows[win].display,
				mu_linux_windows[win].window,
				a, 
				0, 4, False, AnyPropertyType,
				&t, &f, &_n, &b, &data
			);

			if (r == Success && _n == 4 && b == 0) {
				long* extents = (long*)data;

				mu_linux_windows[win].left = extents[0];
				mu_linux_windows[win].right = extents[1];
				mu_linux_windows[win].top = extents[2];
				mu_linux_windows[win].bottom = extents[3];

				mu_linux_windows[win].gotten_deco = MU_TRUE;
			}
		}

		XNextEvent(mu_linux_windows[win].display, &mu_linux_windows[win].event);

		switch (mu_linux_windows[win].event.type) {

		case Expose:
			if (mu_linux_windows[win].dimensions_callback != MU_NULL_PTR) {
				mu_linux_windows[win].dimensions_callback(win, attributes.width, attributes.height);
			}
			break;

		case ClientMessage:
			if (mu_linux_windows[win].event.xclient.data.l[0] == mu_linux_windows[win].delete_message) {
				mu_linux_windows[win].closed = MU_TRUE;
			}
			XUnmapWindow(mu_linux_windows[win].display, mu_linux_windows[win].window);
			break;

		case KeyRelease:
			mu_linux_input_keyboard_set_status(
				&mu_linux_windows[win].input,
				XkbKeycodeToKeysym(mu_linux_windows[win].display, mu_linux_windows[win].event.xkey.keycode, 0, 0),
				MU_KEYBOARD_KEY_BIT_UP
			);
			break;

		case KeyPress:
			mu_linux_input_keyboard_set_status(
				&mu_linux_windows[win].input,
				XkbKeycodeToKeysym(mu_linux_windows[win].display, mu_linux_windows[win].event.xkey.keycode, 0, 0),
				MU_KEYBOARD_KEY_BIT_DOWN
			);
			break;

		case ConfigureNotify:
			if (mu_linux_windows[win].event.xconfigure.x != mu_linux_windows[win].x ||
				mu_linux_windows[win].event.xconfigure.y != mu_linux_windows[win].y
			) {
				mu_linux_windows[win].x = mu_linux_windows[win].event.xconfigure.x;
				mu_linux_windows[win].y = mu_linux_windows[win].event.xconfigure.y;
				if (mu_linux_windows[win].position_callback != MU_NULL_PTR) {
					mu_linux_windows[win].position_callback(win, mu_linux_windows[win].x, mu_linux_windows[win].y);
				}
			}
			break;

		// @TODO FocusIn/Out get called when window is being dragged,
		// might wanna change that but there's no easy way to
		case FocusIn:
			if (mu_linux_windows[win].focus_callback != MU_NULL_PTR) {
				mu_linux_windows[win].focus_callback(win, MU_TRUE);
			}
			break;

		case FocusOut:
			if (mu_linux_windows[win].focus_callback != MU_NULL_PTR) {
				mu_linux_windows[win].focus_callback(win, MU_FALSE);
			}
			break;

		case ButtonPress:
			switch (mu_linux_windows[win].event.xbutton.button) {

			default:
				break;

			// https://stackoverflow.com/questions/16185286/how-to-detect-mouse-click-events-in-all-applications-in-x11
			// jesus i hate the x11/xlib api so much
			case 1:
				mu_linux_input_mouse_set_status(
					&mu_linux_windows[win].input,
					MU_MOUSE_BUTTON_LEFT,
					MU_MOUSE_BUTTON_BIT_DOWN
				);
				if (mu_linux_windows[win].mouse_callback != MU_NULL_PTR) {
					mu_linux_windows[win].mouse_callback(win, MU_MOUSE_BUTTON_LEFT, MU_MOUSE_BUTTON_BIT_DOWN);
				}
				break;

			case 2:
				mu_linux_input_mouse_set_status(
					&mu_linux_windows[win].input,
					MU_MOUSE_BUTTON_MIDDLE,
					MU_MOUSE_BUTTON_BIT_DOWN
				);
				if (mu_linux_windows[win].mouse_callback != MU_NULL_PTR) {
					mu_linux_windows[win].mouse_callback(win, MU_MOUSE_BUTTON_MIDDLE, MU_MOUSE_BUTTON_BIT_DOWN);
				}
				break;

			case 3:
				mu_linux_input_mouse_set_status(
					&mu_linux_windows[win].input,
					MU_MOUSE_BUTTON_RIGHT,
					MU_MOUSE_BUTTON_BIT_DOWN
				);
				if (mu_linux_windows[win].mouse_callback != MU_NULL_PTR) {
					mu_linux_windows[win].mouse_callback(win, MU_MOUSE_BUTTON_RIGHT, MU_MOUSE_BUTTON_BIT_DOWN);
				}
				break;

			}
			break;

		case ButtonRelease:
			switch (mu_linux_windows[win].event.xbutton.button) {

			default:
				break;

			// https://stackoverflow.com/questions/16185286/how-to-detect-mouse-click-events-in-all-applications-in-x11
			// jesus i hate the x11/xlib api so much
			case 1:
				mu_linux_input_mouse_set_status(
					&mu_linux_windows[win].input,
					MU_MOUSE_BUTTON_LEFT,
					MU_MOUSE_BUTTON_BIT_UP
				);
				if (mu_linux_windows[win].mouse_callback != MU_NULL_PTR) {
					mu_linux_windows[win].mouse_callback(win, MU_MOUSE_BUTTON_LEFT, MU_MOUSE_BUTTON_BIT_UP);
				}
				break;

			case 2:
				mu_linux_input_mouse_set_status(
					&mu_linux_windows[win].input,
					MU_MOUSE_BUTTON_MIDDLE,
					MU_MOUSE_BUTTON_BIT_UP
				);
				if (mu_linux_windows[win].mouse_callback != MU_NULL_PTR) {
					mu_linux_windows[win].mouse_callback(win, MU_MOUSE_BUTTON_MIDDLE, MU_MOUSE_BUTTON_BIT_UP);
				}
				break;

			case 3:
				mu_linux_input_mouse_set_status(
					&mu_linux_windows[win].input,
					MU_MOUSE_BUTTON_RIGHT,
					MU_MOUSE_BUTTON_BIT_UP
				);
				if (mu_linux_windows[win].mouse_callback != MU_NULL_PTR) {
					mu_linux_windows[win].mouse_callback(win, MU_MOUSE_BUTTON_RIGHT, MU_MOUSE_BUTTON_BIT_UP);
				}
				break;

			// not sure if 120 is the appropriate value here
			// for matching with window's WM_MOUSEWHEEL

			case 4:
				mu_linux_windows[win].scroll_level += 120;
				if (mu_linux_windows[win].scroll_callback != MU_NULL_PTR) {
					mu_linux_windows[win].scroll_callback(win, 120);
				}
				break;

			case 5:
				mu_linux_windows[win].scroll_level -= 120;
				if (mu_linux_windows[win].scroll_callback != MU_NULL_PTR) {
					mu_linux_windows[win].scroll_callback(win, -120);
				}
				break;

			}
			break;

		default:
			break;
		}
	}

	// http://levonp.blogspot.com/2010/08/retrieving-caps-lock-info-using-xlib.html
	unsigned int n;
	XkbGetIndicatorState(mu_linux_windows[win].display, XkbUseCoreKbd, &n);

	muBool check = (n & (XInternAtom(mu_linux_windows[win].display, "Caps Lock", False) - 1)) != 0;
	if (check != mu_linux_input_keyboard_state_get_status(mu_linux_windows[win].input, MU_KEYBOARD_STATE_CAPS_LOCK)) {
		mu_linux_input_keyboard_state_set_status(&mu_linux_windows[win].input, MU_KEYBOARD_STATE_CAPS_LOCK, check);
		if (mu_linux_windows[win].keyboard_state_callback != MU_NULL_PTR) {
			mu_linux_windows[win].keyboard_state_callback(win, MU_KEYBOARD_STATE_CAPS_LOCK, check);
		}
	}

	check = (n & (XInternAtom(mu_linux_windows[win].display, "Num Lock", False) - 1)) != 0;
	if (check != mu_linux_input_keyboard_state_get_status(mu_linux_windows[win].input, MU_KEYBOARD_STATE_NUM_LOCK)) {
		mu_linux_input_keyboard_state_set_status(&mu_linux_windows[win].input, MU_KEYBOARD_STATE_NUM_LOCK, check);
		if (mu_linux_windows[win].keyboard_state_callback != MU_NULL_PTR) {
			mu_linux_windows[win].keyboard_state_callback(win, MU_KEYBOARD_STATE_NUM_LOCK, check);
		}
	}

	// can't get scroll lock to register... don't know why?
	// maybe name isn't "Scroll Lock"
	check = (n & (XInternAtom(mu_linux_windows[win].display, "Scroll Lock", False)));
	if (check != mu_linux_input_keyboard_state_get_status(mu_linux_windows[win].input, MU_KEYBOARD_STATE_SCROLL_LOCK)) {
		mu_linux_input_keyboard_state_set_status(&mu_linux_windows[win].input, MU_KEYBOARD_STATE_SCROLL_LOCK, check);
		if (mu_linux_windows[win].keyboard_state_callback != MU_NULL_PTR) {
			mu_linux_windows[win].keyboard_state_callback(win, MU_KEYBOARD_STATE_SCROLL_LOCK, check);
		}
	}

	Window focused_window;
	int revert_to;
	XGetInputFocus(mu_linux_windows[win].display, &focused_window, &revert_to);

	if (focused_window != mu_linux_windows[win].window) {
		mu_linux_input_flush(&mu_linux_windows[win].input);
	}

	muBool maximized = mu_window_get_maximized(result, win);
	if (mu_linux_windows[win].maximized != maximized) {
		mu_linux_windows[win].maximized = maximized;
		if (mu_linux_windows[win].maximize_callback != MU_NULL_PTR) {
			mu_linux_windows[win].maximize_callback(win, maximized);
		}
	}

	muBool minimized = mu_window_get_minimized(result, win);
	if (mu_linux_windows[win].minimized != minimized) {
		mu_linux_windows[win].minimized = minimized;
		if (mu_linux_windows[win].minimize_callback != MU_NULL_PTR) {
			mu_linux_windows[win].minimize_callback(win, minimized);
		}
	}

	mu_linux_input post_input = mu_linux_windows[win].input;
	for (size_m i = 0; i < MU_KEYBOARD_KEY_LAST-MU_KEYBOARD_KEY_FIRST+1; i++) {
		if (pre_input.keyboard_down_status[i] != post_input.keyboard_down_status[i]) {
			if (mu_linux_windows[win].keyboard_callback != MU_NULL_PTR) {
				mu_linux_windows[win].keyboard_callback(
					win,
					i+MU_KEYBOARD_KEY_FIRST,
					post_input.keyboard_down_status[i]
				);
			}
		}
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_swap_buffers(muResult* result, muWindow win) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for swapping buffers is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	if (MU_IS_OPENGL(mu_linux_windows[win].api)) {
		MU_OPENGL_CALL(
			glXSwapBuffers(mu_linux_windows[win].display, mu_linux_windows[win].window);
		);
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF muBool mu_window_get_closed(muResult* result, muWindow win) {
	if (!mu_linux_is_valid_closed(win)) {
		mu_print("[muCOSA] Requested window ID for checking if closed is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_TRUE;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return mu_linux_windows[win].closed;
}

MUDEF void mu_window_close(muResult* result, muWindow win) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for closing is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_window_set_visible(result, win, MU_FALSE);
	mu_linux_windows[win].closed = MU_TRUE;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF muBool mu_window_get_focused(muResult* result, muWindow win) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting focused state is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_FALSE;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}

	Window focused;
	int revert_to;
	XGetInputFocus(mu_linux_windows[win].display, &focused, &revert_to);
	return focused == mu_linux_windows[win].window;
}

MUDEF void mu_window_focus(muResult* result, muWindow win) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for focusing is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	// https://www.linuxquestions.org/questions/programming-9/how-to-read-the-state-by-using-_net_wm_state-in-xlib-836879/
	muBool minimized = MU_FALSE;
	Atom wmState = XInternAtom(mu_linux_windows[win].display, "_NET_WM_STATE", MU_TRUE);
	Atom type;
	int format;
	unsigned long nItem, bytesAfter;
	unsigned char* properties = MU_NULL;
	XGetWindowProperty(
		mu_linux_windows[win].display,
		mu_linux_windows[win].window,
		wmState,
		0,
		(~0L),
		MU_FALSE,
		AnyPropertyType,
		&type,
		&format,
		&nItem,
		&bytesAfter,
		&properties
	);

	for (size_m i = 0; i < nItem; i++) {
		Atom prop = ((Atom*)properties)[0];
		char* name = XGetAtomName(mu_linux_windows[win].display, prop);
		if (mu_strcmp(name, "_NET_WM_STATE_HIDDEN") == 0) {
			minimized = MU_TRUE;
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
		mu_memset(&ev, 0, sizeof(ev));
		ev.type = ClientMessage;
		ev.window = mu_linux_windows[win].window;
		ev.message_type = XInternAtom(
			mu_linux_windows[win].display,
			"_NET_ACTIVE_WINDOW",
			MU_TRUE
		);
		ev.format = 32;
		ev.data.l[0] = 1;
		ev.data.l[1] = CurrentTime;
		ev.data.l[2] = ev.data.l[3] = ev.data.l[4] = 0;
		XSendEvent(
			mu_linux_windows[win].display,
			mu_linux_windows[win].parent_window,
			MU_FALSE,
			SubstructureRedirectMask | SubstructureNotifyMask,
			(XEvent*)&ev
		);
		XFlush(mu_linux_windows[win].display);
	}

	XSetInputFocus(
		mu_linux_windows[win].display,
		mu_linux_windows[win].window,
		RevertToPointerRoot,
		CurrentTime
	);
	XRaiseWindow(
		mu_linux_windows[win].display,
		mu_linux_windows[win].window
	);

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_context(muResult* result, muWindow win) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting context is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	if ((!mu_linux_window_binded) || (mu_linux_window_binded && mu_linux_binded_window != win)) {
		mu_linux_window_bind(win);
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_title(muResult* result, muWindow win, const char* title) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting title is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	// convert title from wchar_m* to char*

	XChangeProperty(
		mu_linux_windows[win].display, mu_linux_windows[win].window,
		XInternAtom(mu_linux_windows[win].display, "_NET_WM_NAME", False),
		XInternAtom(mu_linux_windows[win].display, "UTF8_STRING", False),
		8, PropModeReplace, (unsigned char*)title, mu_strlen(title)
	);

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF muBool mu_window_get_visible(muResult* result, muWindow win) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting visibility is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_FALSE;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return mu_linux_windows[win].visible;
}

MUDEF void mu_window_set_visible(muResult* result, muWindow win, muBool visible) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting visibility is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	if (visible == MU_FALSE && mu_linux_windows[win].visible == MU_TRUE) {
		XUnmapWindow(mu_linux_windows[win].display, mu_linux_windows[win].window);
		mu_window_update(result, win);
		mu_linux_input_flush(&mu_linux_windows[win].input);
	} else if (visible == MU_TRUE && mu_linux_windows[win].visible == MU_FALSE) {
		XMapWindow(mu_linux_windows[win].display, mu_linux_windows[win].window);
		mu_window_update(result, win);
		mu_linux_input_flush(&mu_linux_windows[win].input);
	}

	mu_linux_windows[win].visible = visible;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_get_position(muResult* result, muWindow win, int* x, int* y) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting position is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	// https://stackoverflow.com/questions/3806872/window-position-in-xlib
	int rx, ry;
	Window child;
	XTranslateCoordinates(mu_linux_windows[win].display, mu_linux_windows[win].window, mu_linux_windows[win].parent_window, 0, 0, &rx, &ry, &child);

	if (x != MU_NULL_PTR) {
		*x = rx;
	}

	if (y != MU_NULL_PTR) {
		*y = ry;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_position(muResult* result, muWindow win, int x, int y) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting position is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	// https://stackoverflow.com/questions/3806872/window-position-in-xlib
	XWindowAttributes xwa;
	XGetWindowAttributes(mu_linux_windows[win].display, mu_linux_windows[win].window, &xwa);

	XMoveWindow(
		mu_linux_windows[win].display, 
		mu_linux_windows[win].window, 
		x-xwa.x,
		y-xwa.y
	);

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_get_dimensions(muResult* result, muWindow win, unsigned int* width, unsigned int* height) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting dimensions is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	XWindowAttributes xwa;
	XGetWindowAttributes(mu_linux_windows[win].display, mu_linux_windows[win].window, &xwa);

	if (width != MU_NULL_PTR) {
		*width = xwa.width;
	}

	if (height != MU_NULL_PTR) {
		*height = xwa.height;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_dimensions(muResult* result, muWindow win, unsigned int width, unsigned int height) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting dimensions is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	XResizeWindow(
		mu_linux_windows[win].display, 
		mu_linux_windows[win].window, 
		width, height
	);

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF muBool mu_window_get_maximized(muResult* result, muWindow win) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting maximized state is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_FALSE;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}

	Atom wmState = XInternAtom(mu_linux_windows[win].display, "_NET_WM_STATE", MU_TRUE);
	Atom type;
	int format;
	unsigned long nItem, bytesAfter;
	unsigned char* properties = MU_NULL;
	XGetWindowProperty(
		mu_linux_windows[win].display,
		mu_linux_windows[win].window,
		wmState,
		0,
		(~0L),
		MU_FALSE,
		AnyPropertyType,
		&type,
		&format,
		&nItem,
		&bytesAfter,
		&properties
	);

	for (size_m i = 0; i < nItem; i++) {
		Atom prop = ((Atom*)properties)[0];
		char* name = XGetAtomName(mu_linux_windows[win].display, prop);
		if (mu_strcmp(name, "_NET_WM_STATE_MAXIMIZED_VERT") == 0 || mu_strcmp(name, "_NET_WM_STATE_MAXIMIZED_HORZ") == 0) {
			if (name) {
				XFree(name);
			}
			return MU_TRUE;
		}
		if (name) {
			XFree(name);
		}
	}

	return MU_FALSE;
}

MUDEF void mu_window_set_maximized(muResult* result, muWindow win, muBool maximized) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting maximized state is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	// https://forum.juce.com/t/how-to-maximize-not-fullscreen/28346
	XClientMessageEvent ev;
	mu_memset(&ev, 0, sizeof(ev));
	ev.type = ClientMessage;
	ev.window = mu_linux_windows[win].window;
	ev.message_type = XInternAtom(
		mu_linux_windows[win].display,
		"_NET_WM_STATE",
		MU_FALSE
	);
	ev.format = 32;
	ev.data.l[0] = maximized;
	ev.data.l[1] = XInternAtom(mu_linux_windows[win].display, "_NET_WM_STATE_MAXIMIZED_VERT", MU_FALSE);
	ev.data.l[2] = XInternAtom(mu_linux_windows[win].display, "_NET_WM_STATE_MAXIMIZED_HORZ", MU_FALSE);
	ev.data.l[3] = 1;
	ev.data.l[4] = 0;
	XSendEvent(
		mu_linux_windows[win].display,
		mu_linux_windows[win].parent_window,
		MU_FALSE,
		SubstructureRedirectMask | SubstructureNotifyMask,
		(XEvent*)&ev
	);
	XFlush(mu_linux_windows[win].display);

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF muBool mu_window_get_minimized(muResult* result, muWindow win) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting minimized state is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_FALSE;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}

	Atom wmState = XInternAtom(mu_linux_windows[win].display, "_NET_WM_STATE", MU_TRUE);
	Atom type;
	int format;
	unsigned long nItem, bytesAfter;
	unsigned char* properties = MU_NULL;
	XGetWindowProperty(
		mu_linux_windows[win].display,
		mu_linux_windows[win].window,
		wmState,
		0,
		(~0L),
		MU_FALSE,
		AnyPropertyType,
		&type,
		&format,
		&nItem,
		&bytesAfter,
		&properties
	);

	for (size_m i = 0; i < nItem; i++) {
		Atom prop = ((Atom*)properties)[0];
		char* name = XGetAtomName(mu_linux_windows[win].display, prop);
		if (mu_strcmp(name, "_NET_WM_STATE_HIDDEN") == 0) {
			if (name) {
				XFree(name);
			}
			return MU_TRUE;
		}
		if (name) {
			XFree(name);
		}
	}

	return MU_FALSE;
}

MUDEF void mu_window_set_minimized(muResult* result, muWindow win, muBool minimized) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting minimized state is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	// might want to use functions like XIconifyWindow here...
	// don't know.
	if (minimized == MU_TRUE) {
		XClientMessageEvent ev;
		Atom prop;
		prop = XInternAtom(
			mu_linux_windows[win].display,
			"WM_CHANGE_STATE",
			MU_FALSE
		);
		ev.type = ClientMessage;
		ev.window = mu_linux_windows[win].window;
		ev.message_type = prop;
		ev.format = 32;
		ev.data.l[0] = IconicState;
		XSendEvent(
			mu_linux_windows[win].display,
			mu_linux_windows[win].parent_window,
			MU_FALSE,
			SubstructureRedirectMask|SubstructureNotifyMask,
			(XEvent*)&ev
		);
		XFlush(mu_linux_windows[win].display);
	} else {
		XClientMessageEvent ev;
		mu_memset(&ev, 0, sizeof(ev));
		ev.type = ClientMessage;
		ev.window = mu_linux_windows[win].window;
		ev.message_type = XInternAtom(
			mu_linux_windows[win].display,
			"_NET_ACTIVE_WINDOW",
			MU_TRUE
		);
		ev.format = 32;
		ev.data.l[0] = 1;
		ev.data.l[1] = CurrentTime;
		ev.data.l[2] = ev.data.l[3] = ev.data.l[4] = 0;
		XSendEvent(
			mu_linux_windows[win].display,
			mu_linux_windows[win].parent_window,
			MU_FALSE,
			SubstructureRedirectMask | SubstructureNotifyMask,
			(XEvent*)&ev
		);
		XFlush(mu_linux_windows[win].display);
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_get_minimum_dimensions(muResult* result, muWindow win, unsigned int* width, unsigned int* height) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting minimum dimensions is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	XSizeHints* sizeHints = XAllocSizeHints();
	long supplied_return;
	XGetWMNormalHints(
		mu_linux_windows[win].display,
		mu_linux_windows[win].window,
		sizeHints,
		&supplied_return
	);
	if (width != MU_NULL_PTR) {
		*width = sizeHints->min_width;
	}
	if (height != MU_NULL_PTR) {
		*height = sizeHints->min_height;
	}
	XFree(sizeHints);

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_minimum_dimensions(muResult* result, muWindow win, unsigned int width, unsigned int height) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting minimum dimensions is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	XSizeHints* sizeHints = XAllocSizeHints();
	sizeHints->flags = PMinSize | PMaxSize;
	sizeHints->min_width = width;
	sizeHints->min_height = height;
	sizeHints->max_width = mu_linux_windows[win].maximum_width;
	sizeHints->max_height = mu_linux_windows[win].maximum_height;
	XSetWMNormalHints(
		mu_linux_windows[win].display,
		mu_linux_windows[win].window,
		sizeHints
	);
	XFree(sizeHints);

	mu_linux_windows[win].minimum_width = width;
	mu_linux_windows[win].minimum_height = height;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_get_maximum_dimensions(muResult* result, muWindow win, unsigned int* width, unsigned int* height) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting maximum dimensions is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	XSizeHints* sizeHints = XAllocSizeHints();
	long supplied_return;
	XGetWMNormalHints(
		mu_linux_windows[win].display,
		mu_linux_windows[win].window,
		sizeHints,
		&supplied_return
	);
	if (width != MU_NULL_PTR) {
		*width = sizeHints->max_width;
	}
	if (height != MU_NULL_PTR) {
		*height = sizeHints->max_height;
	}
	XFree(sizeHints);

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_maximum_dimensions(muResult* result, muWindow win, unsigned int width, unsigned int height) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting maximum dimensions is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	XSizeHints* sizeHints = XAllocSizeHints();
	sizeHints->flags = PMinSize | PMaxSize;
	sizeHints->min_width = mu_linux_windows[win].minimum_width;
	sizeHints->min_height = mu_linux_windows[win].minimum_height;
	sizeHints->max_width = width;
	sizeHints->max_height = height;
	XSetWMNormalHints(
		mu_linux_windows[win].display,
		mu_linux_windows[win].window,
		sizeHints
	);
	XFree(sizeHints);

	mu_linux_windows[win].maximum_width = width;
	mu_linux_windows[win].maximum_height = height;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_get_mouse_position(muResult* result, muWindow win, int* x, int* y) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting mouse position is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	// https://stackoverflow.com/questions/3585871/how-can-i-get-the-current-mouse-pointer-position-co-ordinates-in-x
	int number_of_screens = XScreenCount(mu_linux_windows[win].display);
	Window* root_windows = mu_malloc(sizeof(Window) * number_of_screens);

	for (size_m i = 0; i < number_of_screens; i++) {
		root_windows[i] = XRootWindow(mu_linux_windows[win].display, i);
	}

	Bool oresult = False;
	int root_x, root_y;
	for (size_m i = 0; i < number_of_screens; i++) {
		Window window_returned;
		int win_x, win_y;
		unsigned int mask_return;

		oresult = XQueryPointer(
			mu_linux_windows[win].display,
			root_windows[i],
			&window_returned,
			&window_returned,
			&root_x, &root_y,
			&win_x, &win_y,
			&mask_return
		);

		if (oresult == True) {
			break;
		}
	}

	mu_free(root_windows);

	if (oresult != True) {
		mu_print("[muCOSA] Failed to get mouse position; failed to find mouse.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	int wx=0, wy=0;
	mu_window_get_position(result, win, &wx, &wy);

	if (x != MU_NULL_PTR) {
		*x = root_x - wx;
	}
	if (y != MU_NULL_PTR) {
		*y = root_y - wy;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_mouse_position(muResult* result, muWindow win, int x, int y) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting mouse position is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	XWarpPointer(
		mu_linux_windows[win].display,
		mu_linux_windows[win].window,
		mu_linux_windows[win].window,
		0, 0, 0, 0,
		x, y
	);
	XFlush(mu_linux_windows[win].display);

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF muCursorStyle mu_window_get_cursor_style(muResult* result, muWindow win) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting cursor style is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_CURSOR_STYLE_UNKNOWN;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return mu_linux_windows[win].cursor_style;
}

MUDEF void mu_window_set_cursor_style(muResult* result, muWindow win, muCursorStyle style) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting cursor style is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	if (style == mu_linux_windows[win].cursor_style) {
		if (result != MU_NULL_PTR) {
			*result = MU_SUCCESS;
		}
		return;
	}

	XUndefineCursor(mu_linux_windows[win].display, mu_linux_windows[win].window);
	XFreeCursor(mu_linux_windows[win].display, mu_linux_windows[win].cursor);

	mu_linux_windows[win].cursor = XCreateFontCursor(mu_linux_windows[win].display, mu_linux_get_linux_cursor_style(style));
	XDefineCursor(
		mu_linux_windows[win].display,
		mu_linux_windows[win].window,
		mu_linux_windows[win].cursor
	);

	mu_linux_windows[win].cursor_style = style;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF int mu_window_get_scroll_level(muResult* result, muWindow win) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting scroll level is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return 0;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return mu_linux_windows[win].scroll_level;
}

MUDEF void mu_window_set_scroll_level(muResult* result, muWindow win, int scroll_level) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting scroll level is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_linux_windows[win].scroll_level = scroll_level;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF muKeyboardKeyBit mu_window_get_keyboard_key_bit(muResult* result, muWindow win, muKeyboardKey key) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting input bit is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_KEYBOARD_KEY_BIT_UP;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return mu_linux_input_keyboard_get_status(mu_linux_windows[win].input, key);
}

MUDEF muKeyboardStateBit mu_window_get_keyboard_state_bit(muResult* result, muWindow win, muKeyboardState state) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting keyboard state bit is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_KEYBOARD_STATE_BIT_OFF;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return mu_linux_input_keyboard_state_get_status(mu_linux_windows[win].input, state);
}

MUDEF muMouseButtonBit mu_window_get_mouse_button_bit(muResult* result, muWindow win, muMouseButton key) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for getting mouse bit is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return MU_MOUSE_BUTTON_BIT_UP;
	}

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
	return mu_linux_input_mouse_get_status(mu_linux_windows[win].input, key);
}

MUDEF void mu_window_set_dimensions_callback(
	muResult* result, 
	muWindow win, 
	void (*dimensions_callback)(muWindow win, int new_width, int new_height)
) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting dimensions callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_linux_windows[win].dimensions_callback = dimensions_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_position_callback(muResult* result, muWindow win, void (*position_callback)(muWindow win, int x, int y)) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting position callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_linux_windows[win].position_callback = position_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_focus_callback(muResult* result, muWindow win, void (*focus_callback)(muWindow win, muBool focused)) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting focus callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_linux_windows[win].focus_callback = focus_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_maximize_callback(muResult* result, muWindow win, void (*maximize_callback)(muWindow win, muBool maximized)) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting maximize callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_linux_windows[win].maximize_callback = maximize_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_minimize_callback(muResult* result, muWindow win, void (*minimize_callback)(muWindow win, muBool minimized)) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting minimize callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_linux_windows[win].minimize_callback = minimize_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_keyboard_callback(muResult* result, muWindow win, void (*keyboard_callback)(muWindow win, muKeyboardKey key, muKeyboardKeyBit bit)) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting keyboard callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_linux_windows[win].keyboard_callback = keyboard_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_keyboard_state_callback(muResult* result, muWindow win, void (*keyboard_state_callback)(muWindow win, muKeyboardState state, muKeyboardStateBit bit)) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting keyboard state callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_linux_windows[win].keyboard_state_callback = keyboard_state_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_mouse_callback(muResult* result, muWindow win, void (*mouse_callback)(muWindow win, muMouseButton key, muMouseButtonBit bit)) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting mouse callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_linux_windows[win].mouse_callback = mouse_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

MUDEF void mu_window_set_scroll_callback(muResult* result, muWindow win, void (*scroll_callback)(muWindow win, int scroll_level_add)) {
	if (!mu_linux_is_id_valid(win)) {
		mu_print("[muCOSA] Requested window ID for setting scroll callback is invalid.\n");
		if (result != MU_NULL_PTR) {
			*result = MU_FAILURE;
		}
		return;
	}

	mu_linux_windows[win].scroll_callback = scroll_callback;

	if (result != MU_NULL_PTR) {
		*result = MU_SUCCESS;
	}
}

/* opengl functions */

MU_OPENGL_CALL(
MUDEF void* mu_get_opengl_function_address(const char* name) {
	return (void*)glXGetProcAddress((const GLubyte*)name);
}
)

#endif /* LINUX */

#ifdef __cplusplus
    }
#endif

#endif /* MUCOSA_IMPLEMENTATION */

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
