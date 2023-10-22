# muCOSA
muCOSA (COSA short for Cross-platform Operating System API) is a public domain single-file C library for interacting with operating systems with a cross-platform API. To use it, simply download the `muCOSA.h` file, add it to your include path, and include it like so:

```
#include MUCOSA_IMPLEMENTATION
#include "muCOSA.h"
```

Note that *not* defining `MUCOSA_IMPLEMENTATION` will define the functions, structs, and enums, but will not give an implementation for the functions, meaning that you can include it across multiple files without redefinition errors as long as you define `MUCOSA_IMPLEMENTATION` once in your main file.

# Licensing
muCOSA is licensed under public domain or MIT, whichever you prefer. More information is provided in the accompanying file `license.md` and at the bottom of `muCOSA.h`.

# Support and compiling
muCOSA has full support for Windows and Linux. Detail on their individual support and how to compile with them is provided below.

## Windows
Windows is fully supported by muCOSA. It has compatibility with versions of OpenGL from 1.0 to 4.6, including core and compatibility.

To compile on Windows, link with `gdi32` and `opengl32`.

## Linux
Linux is fully supported by muCOSA. It has compatibility with versions of OpenGL from 1.0 to 4.6, including core and compatibility.

To compile on Linux, link with `GL` and `X11`.

# General information

## General function syntax

Most muCOSA functions have `muResult* result` as their first parameter. It's used to check if the function succeeded or failed. The function will dereference the given pointer as `MU_TRUE` or `MU_FALSE` to indicate so. `MU_NULL_PTR` can always be passed in if you decide to not check the result of the function.

## String formatting

All strings in muCOSA are interpreted as null-terminated UTF-8 char pointers.

# Macros

## Version
Near the top of the file, the version of muCOSA is defined with `MUCOSA_VERSION_MAJOR`, `MUCOSA_VERSION_MINOR`, and `MUCOSA_VERSION_PATCH`. These versions match the releases on the GitHub page and can be used to check if the correct version of muCOSA is running.

## Static/Extern defines
Defining `MU_STATIC` before including `muCOSA.h` will set all the definitions of the functions to `static`. Otherwise, it defaults to `extern`. Additionally, you can define `MUDEF` directly to have full control over its definition.

## Graphics API inclusion

In order to use a given graphics API, it must be defined before including the implementation for `muCOSA.h`.

If `MUCOSA_OPENGL` is defined, OpenGL functionality will be included.

If `MUCOSA_NO_API` is defined, it will override all other specified inclusions for other graphics APIs and turn all graphics API functionality off.

## Inclusion macros

The macro `MUCOSA_NO_INCLUDE_OPENGL` forces muCOSA to not include any OpenGL header files, allowing the user to control them before the inclusion of the implementation for `muCOSA.h`.

## C standard library dependencies
muCOSA relies on several C standard library functions, types, and defines. The list of types can be found below:

`size_m` equivalent to `size_t`, uses `stddef.h` for definition.

`wchar_m`: equivalent to `wchar_t`, uses `stddef.h` for definition.

The list of functions can be found below:

`mu_strlen`: equivalent to `strlen`, uses `string.h` for definition.

`mu_strcmp`: equivalent to `strcmp`, uses `string.h` for definition.

`mu_memset`: equivalent to `memset`, uses `string.h` for definition.

`mu_wstrlen`: equivalent to `wcslen`, uses `wchar.h` for definition.

`mu_malloc`: equivalent to `malloc`, uses `stdlib.h` for definition.

`mu_free`: equivalent to `free`, uses `stdlib.h` for definition.

`mu_realloc`: equivalent to `realloc`, uses `stdlib.h` for definition.

The list of defines can be found below:

`SIZE_MAX_M`: equivalent to `SIZE_MAX`, uses `stdint.h` for definition.

Note that all of these macros can be overwritten by defining them before including `muCOSA.h`.

Note that overwriting all macros relating to a C standard library header file prevents it from being included. For example, if `size_m` and `wchar_m` are overwritten, which are the only two macros that are used in relation to `stddef.h`, then `stddef.h` will not be included.

## Null macros
muCOSA uses two macros for null values, `MU_NULL` and `MU_NULL_PTR`, who both are just 0.

## No macros

There is one macro used for referring to no object, that being `MU_NO_WINDOW`, which is a macro for `SIZE_MAX_M`.

## Boolean macros
There are a few macros who are based on the `muBool` enum.

Defined below is `muResult`, used to determine the result of a task:

```
#define muResult   muBool
#define MU_FAILURE MU_FALSE
#define MU_SUCCESS MU_TRUE
```

Defined below is `muKeyboardKeyBit`, used to determine whether or not a keyboard key is up or down:

```
#define muKeyboardKeyBit         muBool
#define MU_KEYBOARD_KEY_BIT_UP   MU_FALSE
#define MU_KEYBOARD_KEY_BIT_DOWN MU_TRUE
```

Defined below is `muKeyboardStateBit`, used to determine whether or not a keyboard state is on or off:

```
#define muKeyboardStateBit        muBool
#define MU_KEYBOARD_STATE_BIT_ON  MU_TRUE
#define MU_KEYBOARD_STATE_BIT_OFF MU_FALSE
```

Defined below is `muMouseButtonBit`, used to determine whether or not a mouse button is up or down:

```
#define muMouseButtonBit         muBool
#define MU_MOUSE_BUTTON_BIT_UP   MU_FALSE
#define MU_MOUSE_BUTTON_BIT_DOWN MU_TRUE
```

## Print macros

There are two macros used to print, `mu_print` for printing just a string, and `mu_printf` for printing a formatted string. Both use `printf`, and thus have a dependency on `stdio.h`. If `MUCOSA_NO_PRINT` is defined, these macros are set to do nothing, muCOSA is guaranteed to never print any information, and `stdio.h` is never included. Additionally, `mu_print` and `mu_printf` can be overwritten by the user. If both are overwritten, `stdio.h` is not included.

# Enumerators

## Boolean values
There is one enum that is used for boolean values, `muBool`, defined below:

```
enum muBool { MU_FALSE, MU_TRUE };
typedef enum muBool muBool;
```

All other boolean enums are simply macros to `muBool`, covered in the **Boolean macros** section of **Macros**.

## Graphics APIs
The enum used for graphics APIs is `muGraphicsAPI`, defined below:

```
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
};
typedef enum muGraphicsAPI muGraphicsAPI;
```

The first and last OpenGL API enums can be referred to with the macros `MU_OPENGL_FIRST` and `MU_OPENGL_LAST` respectively. Whether or not an API is OpenGL can be checked with the macro function `MU_IS_OPENGL(api)`.

## Keyboard keys

The enum used for keyboard keys is `muKeyboardKey`, defined below:

```
enum muKeyboardKey {
	MU_KEYBOARD_KEY_UNKNOWN,

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
};
typedef enum muKeyboardKey muKeyboardKey;
```

The first and last keyboard key enums can be referred to with the macros `MU_KEYBOARD_KEY_FIRST` and `MU_KEYBOARD_KEY_LAST` respectively. Whether or not an API is OpenGL can be checked with the macro function `MU_IS_KEYBOARD_KEY(key)`.

## Keyboard states

The enum used for keyboard states is `muKeyboardState`, defined below:

```
enum muKeyboardState {
	MU_KEYBOARD_STATE_UNKNOWN,

	MU_KEYBOARD_STATE_CAPS_LOCK,
	MU_KEYBOARD_STATE_SCROLL_LOCK,
	MU_KEYBOARD_STATE_NUM_LOCK
};
typedef enum muKeyboardState muKeyboardState;
```

The first and last keyboard key enums can be referred to with the macros `MU_KEYBOARD_STATE_FIRST` and `MU_KEYBOARD_STATE_LAST` respectively. Whether or not an API is OpenGL can be checked with the macro function `MU_IS_KEYBOARD_STATE(state)`.

## Mouse buttons

The enum used for mouse buttons is `muMouseButton`, defined below:

```
enum muMouseButton {
	MU_MOUSE_BUTTON_UNKNOWN,

	MU_MOUSE_BUTTON_LEFT,
	MU_MOUSE_BUTTON_RIGHT,
	MU_MOUSE_BUTTON_MIDDLE
};
typedef enum muMouseButton muMouseButton;
```

The first and last keyboard key enums can be referred to with the macros `MU_MOUSE_BUTTON_FIRST` and `MU_MOUSE_BUTTON_LAST` respectively. Whether or not an API is OpenGL can be checked with the macro function `MU_IS_MOUSE_BUTTON(key)`.

## Cursor styles

The enum used for cursor styles is `muCursorStyle`, defined below:

```
enum muCursorStyle {
	MU_CURSOR_STYLE_UNKNOWN,

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
};
typedef enum muCursorStyle muCursorStyle;
```

The first and last keyboard key enums can be referred to with the macros `MU_CURSOR_STYLE_FIRST` and `MU_CURSOR_STYLE_LAST` respectively. Whether or not an API is OpenGL can be checked with the macro function `MU_IS_CURSOR_STYLE(style)`.

# Typedefs

## Index typedefs

There is only one typedef who refers to an index, that being `muWindow`, which is a typedef for `size_m`.

# Structs

## Pixel format struct

The struct used to specify a pixel format is `muPixelFormat`, defined below:

```
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
```

Note that `samples` refers to the amount of multisample buffers. If 1 is its value, then no multisampling is performed.

## Window settings struct

The struct used to specify the settings of a window is `muWindowSettingsStruct`, defined below:

```
struct muWindowSettingsStruct {
	muPixelFormat pixel_format;

	muBool visible;
	muBool resizable;

	muBool minimized;
	muBool maximized;
    
	int x;
	int y;

	unsigned int minimum_width;
	unsigned int minimum_height;
	unsigned int maximum_width;
	unsigned int maximum_height;

	muCursorStyle cursor_style;
};
typedef struct muWindowSettingsStruct muWindowSettingsStruct;
```

# Functions

## Initiation and termination

### Initiation

The function `mu_COSA_init` is used to initiate muCOSA, defined below:

```
MUDEF void mu_COSA_init(muResult* result);
```

Note that this function *must* be called before using any other muCOSA functions.

### Termination

The function `mu_COSA_term` is used to terminate muCOSA, defined below:

```
MUDEF void mu_COSA_term(muResult* result);
```

Note that this function *must* be called before the program ends if `mu_COSA_init` has been called. If not, it will most likely lead to memory leaks.

## Time

### Get time

The function `mu_time_get` is used to get how many seconds it has been since `mu_COSA_init` was most recently called, defined below:

```
MUDEF double mu_time_get(muResult* result);
```

Note that the time that `mu_COSA_init` was most recently called can be modified by `mu_time_set`, meaning that `mu_time_get` is not *guaranteed* to return how many seconds it has been since `mu_COSA_init` was most recently called.

Note that the accuracy of this function can differ between operating system implementations. On Windows, it's accurate to the millisecond. On Linux, it's accurate to the nanosecond.

### Set time

The function `mu_time_set` is used to override the value used by `mu_time_get` to determine how many seconds it has been since `mu_COSA_init` was most recently called, defined below:

```
MUDEF void mu_time_set(muResult* result, double time);
```

Note that this function permits the usage of negative values.

## Clipboard

### Get clipboard

The function `mu_clipboard_get` is used to get the current string stored within the global clipboard, defined below:

```
MUDEF size_m mu_clipboard_get(muResult* result, char* buffer, size_m len);
```

Note that `buffer` gets filled with the clipboard data given if `len` is long enough to store the clipboard text.

Note that if `buffer` is equal to `MU_NULL_PTR`, it will return the size necessary to store the current clipboard text.

### Set clipboard

The function `mu_clipboard_set` is used to set the current clipboard string, defined below:

```
MUDEF void mu_clipboard_set(muResult* result, char* string);
```

Note that on Linux, if this function is called, the program is guaranteed to not stop until another application sets the clipboard to another value.

## Window creation/destruction

### Window creation

The function `mu_window_create` is used to create a window, defined below:

```
MUDEF muWindow mu_window_create(muResult* result, muGraphicsAPI api, muBool (*load_functions)(void), const char* name, unsigned int width, unsigned int height);
```

Note that if this function fails, it returns `MU_NO_WINDOW`.

Note that `load_functions` is a function used to load the given API's functions, and should return whether or not it succeeded. It can safely be passed as `MU_NULL_PTR`.

### Window destruction

The function `mu_window_destroy` is used to destroy a window, defined below:

```
MUDEF void mu_window_destroy(muResult* result, muWindow win);
```

Note that this function is automatically called on all existing windows when `mu_COSA_term` is called, meaning that this function doesn't need to be called, but is recommended.

## Window per-loop functions

The following functions should be called every frame.

### Update window

The function `mu_window_update` is used to update a window, defined below:

```
MUDEF void mu_window_update(muResult* result, muWindow win);
```

Note that this function should be called *after* `mu_window_swap_buffers`, right at the end of the per-frame window loop.

Note that this function calls all of the necessary window callback functions for that frame.

### Swap window buffers

The function `mu_window_swap_buffers` is used to swap the buffers of a window, defined below:

```
MUDEF void mu_window_swap_buffers(muResult* result, muWindow win);
```

Note that this function should be called right *before* `mu_window_update`, near the end of the per-frame window loop.

## Set-only window functions

The following functions modify attributes about a window or the general muCOSA context in relation to a window.

### Set window API context

The function `mu_window_set_context` sets the window's corresponding API context to the current API context, defined below:

```
MUDEF void mu_window_set_context(muResult* result, muWindow win);
```

Note that this function should be called before calling graphics-API-specific functions. Ie, if you're using OpenGL and want to use `glClearColor` on a window, you should call `mu_window_set_context` on the given window before calling it. If you're using only one window, you only need to call it once, but if you're using multiple, you will need to switch between window contexts accordingly.

### Set window title

The function `mu_window_set_title` changes a window's title, defined below:

```
MUDEF void mu_window_set_title(muResult* result, muWindow win, const char* title);
```

## Window closed state

### Get closed state

The function `mu_window_get_closed` returns whether or not a window has been closed by the user, defined below:

```
MUDEF muBool mu_window_get_closed(muResult* result, muWindow win);
```

Note that this function can be used to establish a per-frame window loop.

### Close window

The function `mu_window_close` closes a window, defined below:

```
MUDEF void mu_window_close(muResult* result, muWindow win);
```

## Window focused state

### Get focused state

The function `mu_window_get_focused` returns whether or not a window is the window currently focused on by the user or not, defined below:

```
MUDEF muBool mu_window_get_focused(muResult* result, muWindow win);
```

### Focus window

The function `mu_window_focus` focuses a window, defined below:

```
MUDEF void mu_window_focus(muResult* result, muWindow win);
```

Note that this function can be disruptive, as it automatically unfocuses whatever window the user had focused before.

### Focus callback

The function `mu_window_set_focus_callback` sets the focus callback function for a window, defined below:

```
MUDEF void mu_window_set_focus_callback(muResult* result, muWindow win, void (*focus_callback)(muWindow win, muBool focused));
```

Note that this function gets called everytime the focused state of the given window is changed.

## Window visibility state

### Get visible state

The function `mu_window_get_visible` returns whether or not a given window is visible, defined below:

```
MUDEF muBool mu_window_get_visible(muResult* result, muWindow win);
```

### Set visible state

The function `mu_window_set_visible` sets whether a given window is visible or not, defined below:

```
MUDEF void mu_window_set_visible(muResult* result, muWindow win, muBool visible);
```

## Window position

Note that a window's position is determined, in pixels, by the distance between the top left of the first monitor and the top left of the window's rendering area, not including the decoration of the window, which includes things like the title bar and border.

### Get position

The function `mu_window_get_position` gets the current position of a given window, defined below:

```
MUDEF void mu_window_get_position(muResult* result, muWindow win, int* x, int* y);
```

Note that `x` and/or `y` are allowed to be `MU_NULL_PTR` if you wish to not retrieve its value.

### Set position

The function `mu_window_set_position` sets the current position of a given window, defined below:

```
MUDEF void mu_window_set_position(muResult* result, muWindow win, int x, int y);
```

### Position callback

The function `mu_window_set_position_callback` sets a given window's position callback function, defined below:

```
MUDEF void mu_window_set_position_callback(muResult* result, muWindow win, void (*position_callback)(muWindow win, int x, int y));
```

Note that this function is called every time the position of a window is changed.

## Window dimensions

Note that a window's dimensions is the rendering space of a window, measured in pixels.

### Get dimensions

The function `mu_window_get_dimensions` gets a given window's dimensions, defined below:

```
MUDEF void mu_window_get_dimensions(muResult* result, muWindow win, unsigned int* width, unsigned int* height);
```

Note that `width` and/or `height` are allowed to be `MU_NULL_PTR` if you wish to not retrieve its value.

### Set dimensions

The function `mu_window_set_dimensions` sets the dimensions of a given window, defined below:

```
MUDEF void mu_window_set_dimensions(muResult* result, muWindow win, unsigned int width, unsigned int height);
```

Note that `width` and `height` must abide by the window's given minimum/maximum dimensions.

### Dimensions callback

The function `mu_window_set_dimensions_callback` sets a given window's dimensions callback function, defined below:

```
MUDEF void mu_window_set_dimensions_callback(muResult* result, muWindow win, void (*dimensions_callback)(muWindow win, unsigned int width, unsigned int height));
```

## Window maximized state

Note that 'maximized' refers to when the window takes up the entire screen besides the  taskbar, top window title bar, and any other windows that may be above it.

### Get maximized state

The function `mu_window_get_maximized` returns whether or not a given window is maximized, defined below:

```
MUDEF muBool mu_window_get_maximized(muResult* result, muWindow win);
```

### Set maximized state

The function `mu_window_set_maximized` sets the maximized state of a given window, defined below:

```
MUDEF void mu_window_set_maximized(muResult* result, muWindow win, muBool maximized);
```

### Maximized callback

The function `mu_window_set_maximize_callback` sets a given window's maximize callback function, defined below:

```
MUDEF void mu_window_set_maximize_callback(muResult* result, muWindow win, void (*maximize_callback)(muWindow win, muBool maximized));
```

## Window minimized state

Note that 'minimized' refers to when the window is not visible on screen but is visible & selectable in the taskbar.

### Get minimized state

The function `mu_window_get_minimized` returns whether or not a given window is minimized, defined below:

```
MUDEF muBool mu_window_get_minimized(muResult* result, muWindow win);
```

### Set minimized state

The function `mu_window_set_minimized` sets the minimized state of a given window, defined below:

```
MUDEF void mu_window_set_minimized(muResult* result, muWindow win, muBool minimized);
```

### Minimized callback

The function `mu_window_set_minimize_callback` sets a given window's minimize callback function, defined below:

```
MUDEF void mu_window_set_minimize_callback(muResult* result, muWindow win, void (*minimize_callback)(muWindow win, muBool minimized));
```

## Window minimum/maximum dimensions

### Get min/max dimensions

The functions `mu_window_get_minimum_dimensions` and `mu_window_get_maximum_dimensions` get a given window's minimum and maximum dimensions respectively, both respectively defined below:

```
MUDEF void mu_window_get_minimum_dimensions(muResult* result, muWindow win, unsigned int* width, unsigned int* height);
MUDEF void mu_window_get_maximum_dimensions(muResult* result, muWindow win, unsigned int* width, unsigned int* height);
```

Note that `width` and/or `height` can be `MU_NULL_PTR` if you wish to not retrieve its data.

### Set min/max dimensions

The functions `mu_window_set_minimum_dimensions` and `mu_window_set_maximum_dimensions` set a given window's minimum and maximum dimensions respectively, both respectively defined below:

```
MUDEF void mu_window_set_minimum_dimensions(muResult* result, muWindow win, unsigned int width, unsigned int height);
MUDEF void mu_window_set_maximum_dimensions(muResult* result, muWindow win, unsigned int width, unsigned int height);
```

## Window mouse position

### Get mouse position

The function `mu_window_get_mouse_position` gets the mouse's position relative to a given window's position, defined below:

```
MUDEF void mu_window_get_mouse_position(muResult* result, muWindow win, int* x, int* y);
```

Note that `x` and/or `y` can be `MU_NULL_PTR` if you wish to not retrieve its data.

### Set mouse position

The function `mu_window_set_mouse_position` sets the mouse's position relative to a given window's position, defined below:

```
MUDEF void mu_window_set_mouse_position(muResult* result, muWindow win, int x, int y);
```

## Window cursor style

Note that a cursor style is per-window, meaning that the style you apply to a window will only be active when the user is focused on that window and their mouse position is within the window's rendering space.

### Get cursor style

The function `mu_window_get_cursor_style` returns a given window's cursor style, defined below:

```
MUDEF muCursorStyle mu_window_get_cursor_style(muResult* result, muWindow win);
```

### Set cursor style

The function `mu_window_set_cursor_style` sets a given window's cursor style, defined below:

```
MUDEF void mu_window_set_cursor_style(muResult* result, muWindow win, muCursorStyle style);
```

## Window scroll level

Note that a window's scroll level increments and decrements in units of 120 on a mouse's scroll wheel, and in smaller units on trackpads.

Note that a window's scroll level begins at 0 upon creation.

### Get scroll level

The function `mu_window_get_scroll_level` returns a given window's current scroll level, defined below:

```
MUDEF int mu_window_get_scroll_level(muResult* result, muWindow win);
```

### Set scroll level

The function `mu_window_set_scroll_level` changes a given window's scroll level, defined below:

```
MUDEF void mu_window_set_scroll_level(muResult* result, muWindow win, int scroll_level);
```

### Scroll callback

The function `mu_window_set_scroll_callback` sets a given window's scroll callback function, defined below:

```
MUDEF void mu_window_set_scroll_callback(muResult* result, muWindow win, void (*scroll_callback)(muWindow win, int scroll_level_add));
```

## Window keyboard key input

### Get keyboard key bit

The function `mu_window_get_keyboard_key_bit` returns the given bit state of a given keyboard key in regards to a given window, defined below:

```
MUDEF muKeyboardKeyBit mu_window_get_keyboard_key_bit(muResult* result, muWindow win, muKeyboardKey key);
```

### Keyboard key callback

The function `mu_window_set_keyboard_key_callback` sets a given window's keyboard key callback function, defined below:

```
MUDEF void mu_window_set_keyboard_key_callback(muResult* result, muWindow win, void (*keyboard_key_callback)(muWindow win, muKeyboardKey key, muKeyboardKeyBit bit));
```

Note that this callback only gets called if the given window is focused.

## Window keyboard state input

### Get keyboard state bit

The function `mu_window_get_keyboard_state_bit` returns the given bit state of a given keyboard state in regards to a given window, defined below:

```
MUDEF muKeyboardStateBit mu_window_get_keyboard_state_bit(muResult* result, muWindow win, muKeyboardState state);
```

### Keyboard state callback

The function `mu_window_set_keyboard_state_callback` sets a given window's keyboard state callback function, defined below:

```
MUDEF void mu_window_set_keyboard_state_callback(muResult* result, muWindow win, void (*keyboard_state_callback)(muWindow win, muKeyboardState state, muKeyboardStateBit bit));
```

Note that this callback only gets called if the given window is focused.

## Window mouse button input

### Get mouse button bit

The function `mu_window_get_mouse_button_bit` returns the given bit state of a given mouse button in regards to a given window, defined below:

```
MUDEF muMouseButtonBit mu_window_get_mouse_button_bit(muResult* result, muWindow win, muMouseButton key);
```

### Mouse button callback

The function `mu_window_set_mouse_button_callback` sets a given window's mouse button callback function, defined below:

```
MUDEF void mu_window_set_mouse_button_callback(muResult* result, muWindow win, void (*mouse_button_callback)(muWindow win, muMouseButton key, muMouseButtonBit bit));
```

Note that this callback only gets called if the given window is focused.

## OpenGL functions

### Get OpenGL function address

The function `mu_get_opengl_function_address` returns the address of a given OpenGL function, defined below:

```
MUDEF void* mu_get_opengl_function_address(const char* name);
```

# Global modifiable variables

## Window settings

To modify constant specifics about how a window before it's created, modify the global variable `mu_window_settings`, which is the `muWindowSettingsStruct` struct. Its default values are defined below:

```
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
```

Note that all of these variables, besides `pixel_format` and `resizable`, can be modified after a window is created, and act as default values for a window to be created with.

Note that a window gets and uses these settings upon being created, meaning that different windows can have different settings depending on whatever the values of `mu_window_settings` were when it was created.
