

# muCOSA v1.0.1

muCOSA (COSA standing for cross-operating-system API) is a public domain header-only single-file C library for interacting with operating systems with a cross-platform API. To use it, download the `muCOSA.h` file, add it to your include path, and include it like so:

```c
#define MUCOSA_IMPLEMENTATION
#include "muCOSA.h"
```

More information about the general structure of a mu library is provided at [the mu library information GitHub repository.](https://github.com/Muukid/mu-library-information)

# Demos

Demos that quickly show the gist of the library and how it works are available in the `demos` folder.

# Basic library concepts

This is an overview of how this library works conceptually. More explicit information about the API, such as licensing, functions, and enumerators, can be found further below this section.

## The window

A window (equivalent type `muWindow`) is a reference to a 'window' in the desktop-OS-sense.

The window encapsulates the **surface**, which is the actual space for rendering and excludes other parts of the window such as the title bar and borders. Around the surface are the **frame extents**, which add additional readable and interactable elements.

### The main loop

A window is alive for a certain amount of time before being closed, rather by the user or by the program itself. In order to keep updating during this time, a main loop is established which runs as long as the window's closed state is false. The main loop, generally, looks like this:

```c
// Initialization / Window creation here...

while (!mu_window_get_closed(window)) {
// ... (This is where the frame-by-frame logic would go) ...

mu_window_swap_buffers(window);
mu_window_update(window);
}

// Termination here...
```

### Window properties

A window has several properties that describe it in the muCOSA API, those being:

**dimensions**: the width and height of the window's surface in pixels. Dimensions below a width of 120 and a height of 1 are not guaranteed to work.

**position**: the position of the window's surface in pixels relative to the upper-left corner of the display and the upper-left corner of the surface. A position with negative x and y values is not guaranteed to work.

**closed**: whether or not the window has been closed by the user or the program, permanently disabling the window.

**name**: the title shown on the window's title bar and in other menus referencing it.

**visible**: whether or not it can be seen on the display. This attribute usually also applies to its visibility in other common menus, such as the task bar.

**resizable**: whether or not the window can be resized by the user when dragging around its frame extents.

**minimized**: when the window cannot be seen on the display but is visible in other menus such as the task bar.

**maximized**: when the window takes up the majority of the screen, leaving only the title bar and other menus usually always visible such as the task bar.

**focused**: when the window is the selected window by the user, appearing above all other windows and being primarily interactable with.

**cursor style**: the style of the cursor when the cursor is focused and within the surface.

**cursor position**: the position of the cursor relative to the upper-left corner of the surface.

**scroll level**: how much the user has scrolled up and down on a scrollable device (such as a mouse or trackpad).

**callbacks**: collection of user-defined functions that can be setup to be automatically called when an event occurs regarding the window, usually when an attribute is modified, such as the dimensions or position of a window. Note that it's generally unsafe to call functions on a window from within a callback, as it's very easy to cause a feedback loop where callbacks keep triggering other callbacks, causing a crash.

### Text input

A window is able to take in text input, and has separate functionality for that purpose, allowing the focus for text input to be retrieved and let go. Having it like this allows for multiple things to be automatically handled by the operating system and not the user:

1. States; states like caps lock and shift are automatically handled.

2. Non-ASCII input; OS's usually have some way of inputting non-ASCII characters, and the text input in muCOSA automatically takes it in so the user doesn't need to handle it themselves.

3. Holding down keys; the keyboard key callback's behavior in regards to what happens when a user holds down a key is generally undefined (usually just sends repetitive hold/release messages or just hold), but in text input, the exact key is sent at the exact rate designated by the OS.

Text input is sent via an array of bytes in UTF-8 representing the text input. It's guaranteed to represent just one codepoint and have an ending null termating byte.

Note that text input does also include control codepoints.

Note that text input focus is not guaranteed to be preserved when the user unfocuses and refocuses the window.

Note that text input is automatically let go of on termination.

## Graphics APIs

muCOSA has direct support for OpenGL and Vulkan.

### OpenGL

OpenGL support is enabled by defining `MU_OPENGL` before the implementation of muCOSA is defined. From there, OpenGL versions 1.0 to 4.6 Core/Compatibility can be used, although a function to load OpenGL must be provided by the user.

A common inclusion of OpenGL v3.3 Core with glad would look like this:

```c
#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#define MUCOSA_OPENGL
#define MUCOSA_IMPLEMENTATION
#include "muCOSA.h"

void* load_gl_func(const char* name) {
// Point glad to the function 'mu_opengl_get_function_address'
// provided by muCOSA, so that glad can find each function for
// us.
return mu_opengl_get_function_address(name);
}

muBool load_gl_funcs(void) {
return gladLoadGL((GLADloadfunc)load_gl_func);
}

int main(void) {
// ...

muWindow window = mu_window_create(
MU_OPENGL_3_3_CORE, load_gl_funcs,
// ...
);

// ...
}
```

This automatically handles the loading of OpenGL with glad and handles the OS-dependent creation of an OpenGL context associatable with a window.

### Vulkan

Vulkan support is enabled by defining `MU_VULKAN` before the implementation of muCOSA is defined, as well as `MUCOSA_VULKAN_INCLUDE_PATH` for the path to include Vulkan. This is needed so that muCOSA can handle OS-specific macros that need to be defined before Vulkan is included.

A common inclusion of Vulkan with vkbind would look like this:

```c
#define VKBIND_IMPLEMENTATION
#define MUCOSA_VULKAN_INCLUDE_PATH "vkbind.h"

#define MUCOSA_VULKAN
#define MUCOSA_IMPLEMENTATION
#include "muCOSA.h"

// ...

VkResult vkres = VK_SUCCESS;
VkInstance instance;
VkSurfaceKHR surface;

// ...

int main(void) {
// ...

muWindow window = mu_window_create(
MU_NO_GRAPHICS_API, 0,
// ...
);

// ...

vk_init();

// ...

size_m surface_extension_count = 0;
const char** surface_extensions = mu_vulkan_get_surface_instance_extensions(&surface_extension_count);

// ...

mu_vulkan_create_window_surface(window, &vkres, &instance, 0, &surface);

// ...
}
```

This allows the user to associate the window with no graphics API, initiate Vulkan themselves, and then query information about and ultimately retrieve a handle to the window's surface, which can then be rendered to by the user as they please.

## Time

The 'time' in muCOSA is a reference to a timer, in seconds, that automatically starts when the relevant muCOSA context is created. The time of this timer can be retrieved and set to a different value (to which, if a different value is set, when the time is retrieved, it will return that time that was set plus the additional time that has passed since).

# System dependencies

muCOSA is supported for Win32 and X11.

## Linker flags

### Global linker flags

Some linker flags must be specified regardless of macro settings. These include:

the math library (`-lm` in most command-line C compilers).

### Win32

In order to compile for Win32, you need to link with the following files under the given circumstances:

`user32.dll` and `imm32.dll` in any given circumstance.

`gdi32.dll` and `opengl32.dll` if `MU_OPENGL` is defined before the implementation of muCOSA is given.

### X11

In order to compile for X11, you need to link with the following libraries under the given circumstances:

`libX11` and `libpthread` in any given circumstance. Note that the XKB extension is needed for X11.

`libGL` if `MU_OPENGL` is defined before the implementation of muCOSA is given.

# Licensing

muCOSA is licensed under public domain or MIT, whichever you prefer. More information is provided in the accompanying file `license.md` and at the bottom of `muCOSA.h`.

However, note that the demos that use OpenGL use glad which is built from the Khronos specification, so its Apache 2.0 license applies within that context. See [further clarification in this issue comment](https://github.com/KhronosGroup/OpenGL-Registry/issues/376#issuecomment-596187053).


# Other library dependencies

muCOSA has a dependency on:

[muUtility v1.1.0](https://github.com/Muukid/muUtility/releases/tag/v1.1.0).

Note that mu libraries store their dependencies within their files, so you don't need to import these dependencies yourself.

Note that the libraries listed may also have other dependencies that they also include that aren't listed here.


## Demo library dependencies

The demos have an `include` folder for other files needed to compile the demos, which is usually for other library dependencies. These library dependencies are:

[glad](https://github.com/Dav1dde/glad); specifically generated with [this generator on these settings](http://glad.sh/#api=gl%3Acore%3D3.3&extensions=&generator=c&options=HEADER_ONLY%2CLOADER).

[vkbind](https://github.com/mackron/vkbind).

The files for these libraries are all self-contained in the `include` folder found in the `demos` folder.


# Macros

## `muWindow`

The macro `muWindow` is a void pointer that muCOSA uses to allocate and keep track of a window's data, defined below: 

```c
#define muWindow void*
```


## States

Two macros are defined to represent the state of something. These are:

`muButtonState`: the state of a button (`MU_BUTTON_STATE_RELEASED` (0) meaning the button is not being held, and `MU_BUTTON_STATE_HELD` (1) meaning the button is being held), represented with the type `muBool`.

`muState`: the state of something (`MU_ON` (1) meaning *on* and `MU_OFF` (0) meaning *off*), represented with the type `muBool`.

## Window system definitions

The window systems defined by muCOSA are automatically handled based on what system is running; Unix systems automatically define `MUCOSA_X11` and Win32 systems automatically define `MUCOSA_WIN32`. This automatic choice can be disabled via defining `MUCOSA_NO_AUTOMATIC_API`, and then the user has to manually define these themselves.

This behaviour is handled in the header of muCOSA.

## Locale modification

On X11, in order for text input to fully work, it is recommended to set the locale to "". This is automatically performed unless `MUCOSA_NO_LOCALE_MOD` is defined before the implementation of muCOSA is defined.

## Version macro

muCOSA defines three macros to define the version of muCOSA: `MUCOSA_VERSION_MAJOR`, `MUCOSA_VERSION_MINOR`, and `MUCOSA_VERSION_PATCH`, following the format of `vMAJOR.MINOR.PATCH`.

# Enums

## Result enumerator

muCOSA uses the `muCOSAResult` enumerator to represent how a function went. It has the following possible values:


`MUCOSA_SUCCESS`: the task succeeded.

`MUCOSA_ALREADY_ACTIVE`: the functionality being requested to be activated is already active.

`MUCOSA_ALLOCATION_FAILED`: memory necessary to perform the task failed to allocate.

`MUCOSA_UNKNOWN_WINDOW_SYSTEM`: a window system enumerator value given is an invalid value or is not supported on this system.

`MUCOSA_UNKNOWN_GRAPHICS_API`: a graphics API enumerator value given is an invalid value.

`MUCOSA_UNKNOWN_WINDOW_HANDLE`: a window handle API enumerator value given is an invalid value or is not supported on this system.

`MUCOSA_UNSUPPORTED_OPENGL_FEATURE`: an OpenGL feature necessary to perform the task is not available.

`MUCOSA_UNSUPPORTED_GRAPHICS_API`: the graphics API being requested has not been defined (for example, if you're trying to use OpenGL, `MUCOSA_OPENGL` must be defined before the inclusion of the header file).

`MUCOSA_FAILED_CONNECTION_TO_SERVER`: a Display object could not be opened. This is X11 exclusive.

`MUCOSA_FAILED_CREATE_WINDOW`: the call to create the window failed.

`MUCOSA_FAILED_LOAD_FUNCTIONS`: the function given to the window creation function returned a bad value.

`MUCOSA_FAILED_CREATE_OPENGL_CONTEXT`: the OpenGL context could not be created.

`MUCOSA_FAILED_LOAD_OPENGL_CONTEXT`: the created OpenGL context could not be loaded or made the current OpenGL context.

`MUCOSA_FAILED_CREATE_THREAD`: a thread necessary to perform the task could not be created.

`MUCOSA_FAILED_CREATE_INPUT_METHOD`: an input method for text input could not be created. This is X11 exclusive.

`MUCOSA_FAILED_GET_INPUT_STYLES`: the input styles for text input could not be retrieved. This is X11 exclusive.

`MUCOSA_FAILED_FIND_COMPATIBLE_INPUT_STYLE`: a compatible input style for muCOSA's API for text input could not be created. This is X11 exclusive.

`MUCOSA_FAILED_CREATE_INPUT_CONTEXT`: the input context for text input could not be created. This is X11 exclusive.

`MUCOSA_FAILED_REGISTER_WINDOW_CLASS`: the window class necessary to create a window could not be registered. This is Win32 exclusive.

`MUCOSA_FAILED_CONVERT_UTF8_TO_WCHAR`: a string conversion from UTF-8 to wide-character necessary to perform the task could not be performed. This is Win32 exclusive.

`MUCOSA_FAILED_CONVERT_WCHAR_TO_UTF8`: a string conversion from wide-character to UTF-8 necessary to perform the task could not be performed. This is Win32 exclusive.

`MUCOSA_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS`: the temporary WGL window class necessary to create a valid WGL context could not be registered. This is Win32 exclusive.

`MUCOSA_FAILED_CREATE_DUMMY_WGL_WINDOW`: the temporary WGL window necessary to create a valid WGL context could not be created. This is Win32 exclusive.

`MUCOSA_FAILED_FIND_COMPATIBLE_PIXEL_FORMAT`: a compatible pixel format for OpenGL could not be found. This is Win32 exclusive.

`MUCOSA_FAILED_DESCRIBE_PIXEL_FORMAT`: the pixel format for OpenGL could not be described. This is Win32 exclusive.

`MUCOSA_FAILED_SET_PIXEL_FORMAT`: the pixel format for OpenGL could not be set. This is Win32 exclusive.

`MUCOSA_FAILED_QUERY_WINDOW_INFO`: the requested information about the window could not be retrieved at the current moment. This is Win32 exclusive.

`MUCOSA_FAILED_SET_WINDOW_INFO`: the requested information about the window could be set at the current moment. This is Win32 exclusive.

`MUCOSA_FAILED_ASSOCIATE_IME_CONTEXT`: the IME context could not be associated with the given window. This is Win32 exclusive.

`MUCOSA_FAILED_GLOBAL_ALLOCATION`: global memory necessary to perform the task could not be allocated. This is Win32 exclusive.

`MUCOSA_FAILED_GLOBAL_LOCK`: global memory necessary to perform the task could not be locked. This is Win32 exclusive.

`MUCOSA_FAILED_HOLD_CLIPBOARD`: the clipboard could not be held successfully. This is Win32 exclusive.

`MUCOSA_FAILED_SET_CLIPBOARD`: the clipboard could not be set successfully. This is Win32 exclusive.

`MUCOSA_INVALID_MINIMUM_MAXIMUM_BOOLS`: the min/max booleans specified in the window create info struct are invalid or don't make sense.

`MUCOSA_INVALID_MINIMUM_MAXIMUM_DIMENSIONS`: the min/max dimensions specified are invalid or don't make sense.

`MUCOSA_INVALID_SAMPLE_COUNT`: the sample count given is invalid; it must be a positive power of two with a maximum value of 16.

`MUCOSA_INVALID_WINDOW_STATE`: the state of the window is invalid to perform the given task.

`MUCOSA_NONEXISTENT_DEVICE`: a device necessary to perform the task is not available.

`MUCOSA_OVERSIZED_CLIPBOARD`: the clipboard was too large to perform the task necessary. This can occur on X11 with reasonable clipboard sizes due to INCR not being implemented at this moment.

Unless specified otherwise above, if the result of a function does not equal `MUCOSA_SUCCESS`, whatever the function was supposed to do failed, and it will be as if the function was never called (meaning the function call had no permanent effect).

## Window system

The enum `muWindowSystem` defines a window system. It has these possible values:


`MU_WINDOW_SYSTEM_UNKNOWN`: an unknown window system.

`MU_WINDOW_SYSTEM_AUTO`: choose the window system automatically.

`MU_WINDOW_SYSTEM_X11`: the [X11 window system](https://en.wikipedia.org/wiki/X_Window_System).

`MU_WINDOW_SYSTEM_WIN32`: the [Win32 Windows API](https://en.wikipedia.org/wiki/Windows_API#Major_versions).

## Window handle

The enum `muWindowHandle` defines a window handle. It has these possible values:

`MU_WINDOWS_HWND`: Win32's [HWND](https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types#HWND).

`MU_X11_DISPLAY`: a pointer to X11's [Display](https://tronche.com/gui/x/xlib/display/opening.html#Display).

`MU_X11_WINDOW`: X11's [Window](https://tronche.com/gui/x/xlib/window/).

`MU_X11_PARENT_WINDOW`: X11's [parent Window](https://tronche.com/gui/x/xlib/window-information/XQueryTree.html).

## Graphics API

The enum `muGraphicsAPI` defines a graphics API. It has these possible values:

`MU_NO_GRAPHICS_API`: no graphics API.

`MU_OPENGL_1_0`: [OpenGL v1.0](https://registry.khronos.org/OpenGL/specs/gl/glspec10.pdf).

`MU_OPENGL_1_1`: [OpenGL v1.1](https://registry.khronos.org/OpenGL/specs/gl/glspec11.pdf).

`MU_OPENGL_1_2`: [OpenGL v1.2](https://registry.khronos.org/OpenGL/specs/gl/glspec121.pdf).

`MU_OPENGL_1_2_1`: [OpenGL v1.2.1](https://registry.khronos.org/OpenGL/specs/gl/glspec121.pdf).

`MU_OPENGL_1_3`: [OpenGL v1.3](https://registry.khronos.org/OpenGL/specs/gl/glspec13.pdf).

`MU_OPENGL_1_4`: [OpenGL v1.4](https://registry.khronos.org/OpenGL/specs/gl/glspec14.pdf).

`MU_OPENGL_1_5`: [OpenGL v1.5](https://registry.khronos.org/OpenGL/specs/gl/glspec15.pdf).

`MU_OPENGL_2_0`: [OpenGL v2.0](https://registry.khronos.org/OpenGL/specs/gl/glspec20.pdf).

`MU_OPENGL_2_1`: [OpenGL v2.1](https://registry.khronos.org/OpenGL/specs/gl/glspec21.pdf).

`MU_OPENGL_3_0`: [OpenGL v3.0](https://registry.khronos.org/OpenGL/specs/gl/glspec30.pdf).

`MU_OPENGL_3_1`: [OpenGL v3.1](https://registry.khronos.org/OpenGL/specs/gl/glspec31.pdf).

`MU_OPENGL_3_2_CORE`: [OpenGL v3.2 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec32.core.pdf).

`MU_OPENGL_3_2_COMPATIBILITY`: [OpenGL v3.2 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec32.compatibility.pdf).

`MU_OPENGL_3_3_CORE`: [OpenGL v3.3 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec33.core.pdf).

`MU_OPENGL_3_3_COMPATIBILITY`: [OpenGL v3.3 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec33.compatibility.pdf).

`MU_OPENGL_4_0_CORE`: [OpenGL v4.0 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec40.core.pdf).

`MU_OPENGL_4_0_COMPATIBILITY`: [OpenGL v4.0 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec40.compatibility.pdf).

`MU_OPENGL_4_1_CORE`: [OpenGL v4.1 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec41.core.pdf).

`MU_OPENGL_4_1_COMPATIBILITY`: [OpenGL v4.1 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec41.compatibility.pdf).

`MU_OPENGL_4_2_CORE`: [OpenGL v4.2 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec42.core.pdf).

`MU_OPENGL_4_2_COMPATIBILITY`: [OpenGL v4.2 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec42.compatibility.pdf).

`MU_OPENGL_4_3_CORE`: [OpenGL v4.3 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec43.core.pdf).

`MU_OPENGL_4_3_COMPATIBILITY`: [OpenGL v4.3 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec43.compatibility.pdf).

`MU_OPENGL_4_4_CORE`: [OpenGL v4.4 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec44.core.pdf).

`MU_OPENGL_4_4_COMPATIBILITY`: [OpenGL v4.4 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec44.compatibility.pdf).

`MU_OPENGL_4_5_CORE`: [OpenGL v4.5 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec45.core.pdf).

`MU_OPENGL_4_5_COMPATIBILITY`: [OpenGL v4.5 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec45.compatibility.pdf).

`MU_OPENGL_4_6_CORE`: [OpenGL v4.6 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec46.core.pdf).

`MU_OPENGL_4_6_COMPATIBILITY`: [OpenGL v4.6 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec46.compatibility.pdf).

The macros `MUCOSA_OPENGL_FIRST` and `MUCOSA_OPENGL_LAST` are also used to define the first and last valid graphics APIs (those being `MU_OPENGL_1_0` and `MU_OPENGL_4_6_COMPATIBILITY` respectively).

## Cursor style

The enum `muCursorStyle` defines what a cursor looks like. It has these possible values:

`MU_CURSOR_STYLE_UNKNOWN`: unknown cursor style.

`MU_CURSOR_STYLE_DEFAULT`: undefined cursor style.

`MU_CURSOR_STYLE_ARROW`: the normal arrow-looking cursor style (equivalent to `IDC_ARROW` in Win32).

`MU_CURSOR_STYLE_IBEAM`: the text-select cursor style (equivalent to `IDC_IBEAM` in Win32).

`MU_CURSOR_STYLE_WAIT`: the waiting/busy/loading cursor style (equivalent to `IDC_WAIT` in Win32).

`MU_CURSOR_STYLE_WAIT_ARROW`: the waiting/busy/loading cursor style but with the arrow cursor as well (equivalent to `IDC_APPSTARTING` in Win32).

`MU_CURSOR_STYLE_CROSSHAIR`: the crosshair cursor style (equivalent to `IDC_CROSS` in Win32).

`MU_CURSOR_STYLE_HAND`: the finger-pointing/link-select cursor style (equivalent to `IDC_HAND` in Win32).

`MU_CURSOR_STYLE_SIZE_EAST_WEST`: the resize cursor style, pointing left to right (equivalent to `IDC_SIZEWE` in Win32).

`MU_CURSOR_STYLE_SIZE_NORTH_SOUTH`: the resize cursor style, pointing up to down (equivalent to `IDC_SIZENS` in Win32).

`MU_CURSOR_STYLE_SIZE_NORTH_EAST_SOUTH_WEST`: the resize cursor style, pointing top-right to bottom-left (equivalent to `IDC_SIZENESW` in Win32).

`MU_CURSOR_STYLE_SIZE_NORTH_WEST_SOUTH_EAST`: the resize cursor style, pointing top-left to bottom-right (equivalent to `IDC_SIZENWSE` in Win32).

`MU_CURSOR_STYLE_SIZE_ALL`: the move/resize-all cursor style, pointing in all directions (equivalent to `IDC_SIZEALL` in Win32).

`MU_CURSOR_STYLE_NO`: the disallowing/error/not-allowed cursor style (equivalent to `IDC_NO` in Win32).

## Keyboard key

The enum `muKeyboardKey` defines a keyboard key. It has these possible values:

`MU_KEYBOARD_KEY_UNKNOWN`: unknown key.

`MU_KEYBOARD_KEY_BACKSPACE`: the [backspace key](https://en.wikipedia.org/wiki/Backspace).

`MU_KEYBOARD_KEY_TAB`: the [tab key](https://en.wikipedia.org/wiki/Tab_key).

`MU_KEYBOARD_KEY_CLEAR`: the [clear key](https://en.wikipedia.org/wiki/Clear_key).

`MU_KEYBOARD_KEY_RETURN`: the [return key](https://en.wikipedia.org/wiki/Return_key).

`MU_KEYBOARD_KEY_PAUSE`: the [pause key](https://en.wikipedia.org/wiki/Pause_key).

`MU_KEYBOARD_KEY_ESCAPE`: the [escape key](https://en.wikipedia.org/wiki/Escape_key).

`MU_KEYBOARD_KEY_MODECHANGE`: the [modechange key](https://en.wikipedia.org/wiki/Language_input_keys).

`MU_KEYBOARD_KEY_SPACE`: the [space key](https://en.wikipedia.org/wiki/Space_bar).

`MU_KEYBOARD_KEY_PRIOR`: the [page up key](https://en.wikipedia.org/wiki/Page_Up_and_Page_Down_keys).

`MU_KEYBOARD_KEY_NEXT`: the [page down key](https://en.wikipedia.org/wiki/Page_Up_and_Page_Down_keys).

`MU_KEYBOARD_KEY_END`: the [end key](https://en.wikipedia.org/wiki/End_key).

`MU_KEYBOARD_KEY_HOME`: the [home key](https://en.wikipedia.org/wiki/Home_key).

`MU_KEYBOARD_KEY_LEFT`: the [left key](https://en.wikipedia.org/wiki/Arrow_keys).

`MU_KEYBOARD_KEY_UP`: the [up key](https://en.wikipedia.org/wiki/Arrow_keys).

`MU_KEYBOARD_KEY_RIGHT`: the [right key](https://en.wikipedia.org/wiki/Arrow_keys).

`MU_KEYBOARD_KEY_DOWN`: the [down key](https://en.wikipedia.org/wiki/Arrow_keys).

`MU_KEYBOARD_KEY_SELECT`: the [select key](https://stackoverflow.com/questions/23995537/what-is-the-select-key).

`MU_KEYBOARD_KEY_PRINT`: the [print key](https://en.wikipedia.org/wiki/Print_Screen).

`MU_KEYBOARD_KEY_EXECUTE`: the execute key.

`MU_KEYBOARD_KEY_INSERT`: the [insert key](https://en.wikipedia.org/wiki/Insert_key).

`MU_KEYBOARD_KEY_DELETE`: the [delete key](https://en.wikipedia.org/wiki/Delete_key).

`MU_KEYBOARD_KEY_HELP`: the [help key](https://en.wikipedia.org/wiki/Help_key).

`MU_KEYBOARD_KEY_0`: the [0 key](https://en.wikipedia.org/wiki/Numerical_digit).

`MU_KEYBOARD_KEY_1`: the [1 key](https://en.wikipedia.org/wiki/Numerical_digit).

`MU_KEYBOARD_KEY_2`: the [2 key](https://en.wikipedia.org/wiki/Numerical_digit).

`MU_KEYBOARD_KEY_3`: the [3 key](https://en.wikipedia.org/wiki/Numerical_digit).

`MU_KEYBOARD_KEY_4`: the [4 key](https://en.wikipedia.org/wiki/Numerical_digit).

`MU_KEYBOARD_KEY_5`: the [5 key](https://en.wikipedia.org/wiki/Numerical_digit).

`MU_KEYBOARD_KEY_6`: the [6 key](https://en.wikipedia.org/wiki/Numerical_digit).

`MU_KEYBOARD_KEY_7`: the [7 key](https://en.wikipedia.org/wiki/Numerical_digit).

`MU_KEYBOARD_KEY_8`: the [8 key](https://en.wikipedia.org/wiki/Numerical_digit).

`MU_KEYBOARD_KEY_9`: the [9 key](https://en.wikipedia.org/wiki/Numerical_digit).

`MU_KEYBOARD_KEY_A`: the [A key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_B`: the [B key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_C`: the [C key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_D`: the [D key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_E`: the [E key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_F`: the [F key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_G`: the [G key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_H`: the [H key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_I`: the [I key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_J`: the [J key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_K`: the [K key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_L`: the [L key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_M`: the [M key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_N`: the [N key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_O`: the [O key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_P`: the [P key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_Q`: the [Q key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_R`: the [R key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_S`: the [S key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_T`: the [T key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_U`: the [U key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_V`: the [V key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_W`: the [W key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_X`: the [X key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_Y`: the [Y key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_Z`: the [Z key](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys).

`MU_KEYBOARD_KEY_LEFT_WINDOWS`: the left [Windows](https://en.wikipedia.org/wiki/Windows_key)/[super](https://en.wikipedia.org/wiki/Super_key_(keyboard_button))/[command](https://en.wikipedia.org/wiki/Command_key) key.

`MU_KEYBOARD_KEY_RIGHT_WINDOWS`: the right [Windows](https://en.wikipedia.org/wiki/Windows_key)/[super](https://en.wikipedia.org/wiki/Super_key_(keyboard_button))/[command](https://en.wikipedia.org/wiki/Command_key) key.

`MU_KEYBOARD_KEY_NUMPAD_0`: the [0 numpad key](https://en.wikipedia.org/wiki/Numeric_keypad).

`MU_KEYBOARD_KEY_NUMPAD_1`: the [1 numpad key](https://en.wikipedia.org/wiki/Numeric_keypad).

`MU_KEYBOARD_KEY_NUMPAD_2`: the [2 numpad key](https://en.wikipedia.org/wiki/Numeric_keypad).

`MU_KEYBOARD_KEY_NUMPAD_3`: the [3 numpad key](https://en.wikipedia.org/wiki/Numeric_keypad).

`MU_KEYBOARD_KEY_NUMPAD_4`: the [4 numpad key](https://en.wikipedia.org/wiki/Numeric_keypad).

`MU_KEYBOARD_KEY_NUMPAD_5`: the [5 numpad key](https://en.wikipedia.org/wiki/Numeric_keypad).

`MU_KEYBOARD_KEY_NUMPAD_6`: the [6 numpad key](https://en.wikipedia.org/wiki/Numeric_keypad).

`MU_KEYBOARD_KEY_NUMPAD_7`: the [7 numpad key](https://en.wikipedia.org/wiki/Numeric_keypad).

`MU_KEYBOARD_KEY_NUMPAD_8`: the [8 numpad key](https://en.wikipedia.org/wiki/Numeric_keypad).

`MU_KEYBOARD_KEY_NUMPAD_9`: the [9 numpad key](https://en.wikipedia.org/wiki/Numeric_keypad).

`MU_KEYBOARD_KEY_MULTIPLY`: the [multiply key](https://en.wikipedia.org/wiki/Numeric_keypad).

`MU_KEYBOARD_KEY_ADD`: the [add key](https://en.wikipedia.org/wiki/Numeric_keypad).

`MU_KEYBOARD_KEY_SEPARATOR`: the [separator key](https://stackoverflow.com/questions/67916923/what-physical-key-maps-to-keycode-108-vk-separator).

`MU_KEYBOARD_KEY_SUBTRACT`: the [subtract key](https://en.wikipedia.org/wiki/Numeric_keypad).

`MU_KEYBOARD_KEY_DECIMAL`: the [decimal key](https://en.wikipedia.org/wiki/Numeric_keypad).

`MU_KEYBOARD_KEY_DIVIDE`: the [divide key](https://en.wikipedia.org/wiki/Numeric_keypad).

`MU_KEYBOARD_KEY_F1`: the [F1 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F2`: the [F2 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F3`: the [F3 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F4`: the [F4 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F5`: the [F5 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F6`: the [F6 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F7`: the [F7 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F8`: the [F8 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F9`: the [F9 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F10`: the [F10 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F11`: the [F11 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F12`: the [F12 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F13`: the [F13 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F14`: the [F14 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F15`: the [F15 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F16`: the [F16 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F17`: the [F17 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F18`: the [F18 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F19`: the [F19 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F20`: the [F20 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F21`: the [F21 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F22`: the [F22 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F23`: the [F23 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_F24`: the [F24 key](https://en.wikipedia.org/wiki/Function_key).

`MU_KEYBOARD_KEY_NUMLOCK`: the [Num Lock key](https://en.wikipedia.org/wiki/Num_Lock).

`MU_KEYBOARD_KEY_SCROLL`: the [Scroll Lock key](https://en.wikipedia.org/wiki/Scroll_Lock).

`MU_KEYBOARD_KEY_LEFT_SHIFT`: the [left shift key](https://en.wikipedia.org/wiki/Shift_key).

`MU_KEYBOARD_KEY_RIGHT_SHIFT`: the [right shift key](https://en.wikipedia.org/wiki/Shift_key).

`MU_KEYBOARD_KEY_LEFT_CONTROL`: the [left control key](https://en.wikipedia.org/wiki/Control_key).

`MU_KEYBOARD_KEY_RIGHT_CONTROL`: the [right control key](https://en.wikipedia.org/wiki/Control_key).

`MU_KEYBOARD_KEY_LEFT_MENU`: the [left menu key](https://en.wikipedia.org/wiki/Menu_key).

`MU_KEYBOARD_KEY_RIGHT_MENU`: the [right menu key](https://en.wikipedia.org/wiki/Menu_key).

`MU_KEYBOARD_KEY_ATTN`: the [ATTN key](https://www.ibm.com/support/pages/apar/II04992).

`MU_KEYBOARD_KEY_CRSEL`: the [CRSEL key](https://learn.microsoft.com/en-us/dotnet/api/system.windows.forms.keys?view=windowsdesktop-8.0).

`MU_KEYBOARD_KEY_EXSEL`: the [EXSEL key](https://learn.microsoft.com/en-us/dotnet/api/system.windows.forms.keys?view=windowsdesktop-8.0).

`MU_KEYBOARD_KEY_EREOF`: the [EREOF key](https://www.ibm.com/docs/en/wsfz-and-o/1.1?topic=descriptions-ereof-erase-end-field-key-statement).

`MU_KEYBOARD_KEY_PLAY`: the play key.

`MU_KEYBOARD_KEY_PA1`: the [PA1 key](https://www.ibm.com/docs/en/zos-basic-skills?topic=ispf-keyboard-keys-functions).

The macros `MU_KEYBOARD_KEY_FIRST` and `MU_KEYBOARD_KEY_LAST` are also used to define the first and last valid keyboard keys (those being `MU_KEYBOARD_KEY_BACKSPACE` and `MU_KEYBOARD_KEY_PA1` respectively).

Note that this list is by no means exhaustive, and is even missing some basic keys like the semicolon key (in which case, the only realistic way of getting input would be to use text input, which is not a very good idea considering that it causes virtual keyboards to appear); I plan to add more keys later.

## Keyboard state

The enum `muKeyboardState` defines a keyboard state. It has these possible values:

`MU_KEYBOARD_STATE_UNKNOWN`: unknown keyboard state.

`MU_KEYBOARD_STATE_CAPS_LOCK`: [caps lock state](https://en.wikipedia.org/wiki/Caps_Lock).

`MU_KEYBOARD_STATE_SCROLL_LOCK`: [scroll lock state](https://en.wikipedia.org/wiki/Scroll_Lock).

`MU_KEYBOARD_STATE_NUM_LOCK`: [num lock state](https://en.wikipedia.org/wiki/Num_Lock).

The macros `MU_KEYBOARD_STATE_FIRST` and `MU_KEYBOARD_STATE_LAST` are also used to define the first and last valid keyboard states (those being `MU_KEYBOARD_STATE_CAPS_LOCK` and `MU_KEYBOARD_STATE_NUM_LOCK` respectively).

## Mouse button

The enum `muMouseButton` defines a [mouse button](https://en.wikipedia.org/wiki/Mouse_button). It has these possible values:

`MU_MOUSE_BUTTON_UNKNOWN`: unknown mouse button.

`MU_MOUSE_BUTTON_LEFT`: left mouse button.

`MU_MOUSE_BUTTON_RIGHT`: right mouse button.

`MU_MOUSE_BUTTON_MIDDLE`: middle mouse button.

The macros `MU_MOUSE_BUTTON_FIRST` and `MU_MOUSE_BUTTON_LAST` are also used to define the first and last valid mouse buttons (those being `MU_MOUSE_BUTTON_LEFT` and `MU_MOUSE_BUTTON_MIDDLE` respectively).

# Structs

## Context

The struct `muCOSAContext` is used to reference a muCOSA context. It has one member, `result` (of type `muCOSAResult`), that represents the result of the latest non-explicit result-checking function call.

## Pixel format

The struct `muPixelFormat` is used to reference a pixel format. It has the following members:

`red_bits`: the amount of bits used for the red channel, expressed with the type `uint16_m`. Its default value is 8.

`green_bits`: the amount of bits used for the green channel, expressed with the type `uint16_m`. Its default value is 8.

`blue_bits`: the amount of bits used for the blue channel, expressed with the type `uint16_m`. Its default value is 8.

`alpha_bits`: the amount of bits used for the alpha channel, expressed with the type `uint16_m`. Its default value is 0.

`depth_bits`: the amount of bits used for the depth channel, expressed with the type `uint16_m`. Its default value is 0.

`stencil_bits`: the amount of bits used for the stencil channel, expressed with the type `uint16_m`. Its default value is 0.

`samples`: the amount of samples used for sampling, expressed with the type `uint8_m`. Its default value is 1. This value must be a power of 2, and its max value is 16.

## Window create info

The struct `muWindowCreateInfo` is used to reference advanced attributes about a window. It has the following members:

`pixel_format`: the pixel format of the window surface (applies only if the graphics API of the window is not `MU_NO_GRAPHICS_API`), expressed with the type `muPixelFormat`.

`visible`: a `muBool` representing the window's visibility.

`resizable`: a `muBool` representing whether or not the window can be resized.

`minimized`: a `muBool` representing the window's minimized state.

`maximized`: a `muBool` representing the window's maximized state.

`x`: the window's x position, expressed with the type `int32_m`.

`y`: the window's y position, expressed with the type `int32_m`.

`min_width`: the minimum width of the window, expressed with the type `uint32_m`.

`min_height`: the minimum height of the window, expressed with the type `uint32_m`.

`max_width`: the maximum width of the window, expressed with the type `uint32_m`.

`max_height`: the maximum height of the window, expressed with the type `uint32_m`.

`cursor_style`: the cursor style of the window, expressed with the type `muCursorStyle`. This style is not permanant, only existing as the style first used upon the window's creation.

`dimensions_callback`: the callback used for changes in the window's dimensions, defined below: 

```c
void (*dimensions_callback)(muWindow window, uint32_m width, uint32_m height);
```


`position_callback`: the callback used for changes in the window's position, defined below: 

```c
void (*position_callback)(muWindow window, int32_m x, int32_m y);
```


`focus_callback`: the callback used for changes in the window's focused state, defined below: 

```c
void (*focus_callback)(muWindow window, muBool focused);
```


`maximize_callback`: the callback used for changes in the window's maximized state, defined below: 

```c
void (*maximize_callback)(muWindow window, muBool maximized);
```


`minimize_callback`: the callback used for changes in the window's minimized state, defined below: 

```c
void (*minimize_callback)(muWindow window, muBool minimized);
```


`keyboard_key_callback`: the callback used for changes in the window's keyboard key states, defined below: 

```c
void (*keyboard_key_callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state);
```


`keyboard_state_callback`: the callback used for changes in the window's keyboard state states, defined below: 

```c
void (*keyboard_state_callback)(muWindow window, muKeyboardState keyboard_state, muState state);
```


`cursor_position_callback`: the callback used for changes in the window's cursor position, defined below: 

```c
void (*cursor_position_callback)(muWindow window, int32_m x, int32_m y);
```


`mouse_button_callback`: the callback used for changes in the window's mouse button states, defined below: 

```c
void (*mouse_button_callback)(muWindow window, muMouseButton mouse_button, muButtonState state);
```


`scroll_callback`: the callback used for changes in the window's scroll level, defined below: 

```c
void (*scroll_callback)(muWindow window, int32_m scroll_level_add);
```


# Functions

## Context functions

### Creation and destruction

#### Creation

The function `muCOSA_context_create` creates a context, defined below: 

```c
MUDEF void muCOSA_context_create(muCOSAContext* context, muWindowSystem system, muBool set_context);
```


Note that the result of this function is stored within `context->result`.

Note that, upon success, this function automatically calls `muCOSA_context_set` on the created context unless `set_context` is equal to `MU_FALSE`.

#### Destruction

The function `muCOSA_context_destroy` destroys a context, defined below: 

```c
MUDEF void muCOSA_context_destroy(muCOSAContext* context);
```


Note that this function cannot fail.

### Context setting

The function `muCOSA_context_set` sets the global context to the given context, defined below: 

```c
MUDEF void muCOSA_context_set(muCOSAContext* context);
```


Note that this function can be automatically called on a created context with the function `muCOSA_context_create`.

Note that the global context can also be accessed manually via the global variable `muCOSA_global_context`, although this is not recommended.

### Context window information

The function `muCOSA_context_get_window_system` retrieves the window system of a given context, defined below: 

```c
MUDEF muWindowSystem muCOSA_context_get_window_system(muCOSAContext* context);
```


## Window functions

### Creation and destruction

#### Default window create info values

The default values for the `muWindowCreateInfo` struct passed into `mu_window_create` can be retrieved with the function `mu_window_default_create_info`, defined below: 

```c
MUDEF muWindowCreateInfo mu_window_default_create_info(void);
```


The values filled in by this function can be found in the "Structs" section.

#### Creation

The function `muCOSA_window_create` creates a window, defined below: 

```c
MUDEF muWindow muCOSA_window_create(muCOSAContext* context, muCOSAResult* result, muGraphicsAPI api, muBool (*load_functions)(void), const char* name, uint16_m width, uint16_m height, muWindowCreateInfo create_info);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_create(...) muCOSA_window_create(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_create_(result, ...) muCOSA_window_create(muCOSA_global_context, result, __VA_ARGS__)
```


#### Destruction

The function `muCOSA_window_destroy` destroys a window, defined below: 

```c
MUDEF muWindow muCOSA_window_destroy(muCOSAContext* context, muCOSAResult* result, muWindow window);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_destroy(...) muCOSA_window_destroy(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_destroy_(result, ...) muCOSA_window_destroy(muCOSA_global_context, result, __VA_ARGS__)
```


### Main loop functions

#### Get closed state

The function `muCOSA_window_get_closed` returns the closed state of the window, defined below: 

```c
MUDEF muBool muCOSA_window_get_closed(muCOSAContext* context, muCOSAResult* result, muWindow window);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_closed(...) muCOSA_window_get_closed(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_closed_(result, ...) muCOSA_window_get_closed(muCOSA_global_context, result, __VA_ARGS__)
```


#### Close window

The function `muCOSA_window_close` closes a given window, defined below: 

```c
MUDEF void muCOSA_window_close(muCOSAContext* context, muCOSAResult* result, muWindow window);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_close(...) muCOSA_window_close(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_close_(result, ...) muCOSA_window_close_(muCOSA_global_context, result, __VA_ARGS__)
```


#### Update

The function `muCOSA_window_update` updates/refreshes a window and triggers all relevant callbacks, defined below: 

```c
MUDEF void muCOSA_window_update(muCOSAContext* context, muCOSAResult* result, muWindow window);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_update(...) muCOSA_window_update(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_update_(result, ...) muCOSA_window_update(muCOSA_global_context, result, __VA_ARGS__)
```


#### Swap buffers

The function `muCOSA_window_swap_buffers` swaps the image buffers of a given window if the window is associated with a graphics API, defined below: 

```c
MUDEF void muCOSA_window_swap_buffers(muCOSAContext* context, muCOSAResult* result, muWindow window);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_swap_buffers(...) muCOSA_window_swap_buffers(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_swap_buffers_(result, ...) muCOSA_window_swap_buffers(muCOSA_global_context, result, __VA_ARGS__)
```


### Get/Set window states/properties

#### Get focused state

The function `muCOSA_window_get_focused` gets the focused state of a window, defined below: 

```c
MUDEF muBool muCOSA_window_get_focused(muCOSAContext* context, muCOSAResult* result, muWindow window);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_focused(...) muCOSA_window_get_focused(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_focused_(result, ...) muCOSA_window_get_focused(muCOSA_global_context, result, __VA_ARGS__)
```


#### Focus window

The function `muCOSA_window_focus` focuses a given window, defined below: 

```c
MUDEF void muCOSA_window_focus(muCOSAContext* context, muCOSAResult* result, muWindow window);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_focus(...) muCOSA_window_focus(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_focus_(result, ...) muCOSA_window_focus(muCOSA_global_context, result, __VA_ARGS__)
```


Note that due to the nature of how most window systems are built, muCOSA_window_focus is not guaranteed to work at all times, but should at least alert the user to focus on the window.

#### Get visibility state

The function `muCOSA_window_get_visible` gets the visible state of a window, defined below: 

```c
MUDEF muBool muCOSA_window_get_visible(muCOSAContext* context, muCOSAResult* result, muWindow window);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_visible(...) muCOSA_window_get_visible(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_visible_(result, ...) muCOSA_window_get_visible(muCOSA_global_context, result, __VA_ARGS__)
```


#### Set visibility state

The function `muCOSA_window_set_visible` sets the visible state of a window, defined below: 

```c
MUDEF void muCOSA_window_set_visible(muCOSAContext* context, muCOSAResult* result, muWindow window, muBool visible);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_visible(...) muCOSA_window_set_visible(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_visible_(result, ...) muCOSA_window_set_visible(muCOSA_global_context, result, __VA_ARGS__)
```


#### Get position

The function `muCOSA_window_get_position` gets the position of a window, defined below: 

```c
MUDEF void muCOSA_window_get_position(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m* x, int32_m* y);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_position(...) muCOSA_window_get_position(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_position_(result, ...) muCOSA_window_get_position(muCOSA_global_context, result, __VA_ARGS__)
```


#### Set position

The function `muCOSA_window_set_position` sets the position of a window, defined below: 

```c
MUDEF void muCOSA_window_set_position(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m x, int32_m y);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_position(...) muCOSA_window_set_position(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_position_(result, ...) muCOSA_window_set_position(muCOSA_global_context, result, __VA_ARGS__)
```


#### Get dimensions

The function `muCOSA_window_get_dimensions` gets the dimensions of a window, defined below: 

```c
MUDEF void muCOSA_window_get_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m* width, uint32_m* height);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_dimensions(...) muCOSA_window_get_dimensions(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_dimensions_(result, ...) muCOSA_window_get_dimensions(muCOSA_global_context, result, __VA_ARGS__)
```


#### Set dimensions

The function `muCOSA_window_set_dimensions` sets the dimensions of a window, defined below: 

```c
MUDEF void muCOSA_window_set_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m width, uint32_m height);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_dimensions(...) muCOSA_window_set_dimensions(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_dimensions_(result, ...) muCOSA_window_set_dimensions(muCOSA_global_context, result, __VA_ARGS__)
```


#### Get maximized state

The function `muCOSA_window_get_maximized` gets the maximized state of a window, defined below: 

```c
MUDEF muBool muCOSA_window_get_maximized(muCOSAContext* context, muCOSAResult* result, muWindow window);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_maximized(...) muCOSA_window_get_maximized(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_maximized_(result, ...) muCOSA_window_get_maximized(muCOSA_global_context, result, __VA_ARGS__)
```


#### Set maximized state

The function `muCOSA_window_set_maximized` sets the maximized state of a window, defined below: 

```c
MUDEF void muCOSA_window_set_maximized(muCOSAContext* context, muCOSAResult* result, muWindow window, muBool maximized);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_maximized(...) muCOSA_window_set_maximized(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_maximized_(result, ...) muCOSA_window_set_maximized(muCOSA_global_context, result, __VA_ARGS__)
```


#### Get minimized state

The function `muCOSA_window_get_minimized` gets the minimized state of a window, defined below: 

```c
MUDEF muBool muCOSA_window_get_minimized(muCOSAContext* context, muCOSAResult* result, muWindow window);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_minimized(...) muCOSA_window_get_minimized(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_minimized_(result, ...) muCOSA_window_get_minimized(muCOSA_global_context, result, __VA_ARGS__)
```


#### Set minimized state

The function `muCOSA_window_set_minimized` sets the minimized state of a window, defined below: 

```c
MUDEF void muCOSA_window_set_minimized(muCOSAContext* context, muCOSAResult* result, muWindow window, muBool minimized);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_minimized(...) muCOSA_window_set_minimized(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_minimized_(result, ...) muCOSA_window_set_minimized(muCOSA_global_context, result, __VA_ARGS__)
```


#### Get minimum dimensions

The function `muCOSA_window_get_minimum_dimensions` gets the minimum dimensions of a window, defined below: 

```c
MUDEF void muCOSA_window_get_minimum_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m* min_width, uint32_m* min_height);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_minimum_dimensions(...) muCOSA_window_get_minimum_dimensions(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_minimum_dimensions_(result, ...) muCOSA_window_get_minimum_dimensions(muCOSA_global_context, result, __VA_ARGS__)
```


#### Set minimum dimensions

The function `muCOSA_window_set_minimum_dimensions` sets the minimum dimensions of a window, defined below: 

```c
MUDEF void muCOSA_window_set_minimum_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m min_width, uint32_m min_height);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_minimum_dimensions(...) muCOSA_window_set_minimum_dimensions(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_minimum_dimensions_(result, ...) muCOSA_window_set_minimum_dimensions(muCOSA_global_context, result, __VA_ARGS__)
```


#### Get maximum dimensions

The function `muCOSA_window_get_maximum_dimensions` gets the maximum dimensions of a window, defined below: 

```c
MUDEF void muCOSA_window_get_maximum_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m* max_width, uint32_m* max_height);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_maximum_dimensions(...) muCOSA_window_get_maximum_dimensions(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_maximum_dimensions_(result, ...) muCOSA_window_get_maximum_dimensions(muCOSA_global_context, result, __VA_ARGS__)
```


#### Set maximum dimensions

The function `muCOSA_window_set_maximum_dimensions` sets the maximum dimensions of a window, defined below: 

```c
MUDEF void muCOSA_window_set_maximum_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m max_width, uint32_m max_height);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_maximum_dimensions(...) muCOSA_window_set_maximum_dimensions(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_maximum_dimensions_(result, ...) muCOSA_window_set_maximum_dimensions(muCOSA_global_context, result, __VA_ARGS__)
```


#### Get cursor position

The function `muCOSA_window_get_cursor_position` gets the position of the cursor relative to a window, defined below: 

```c
MUDEF void muCOSA_window_get_cursor_position(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m* x, int32_m* y);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_cursor_position(...) muCOSA_window_get_cursor_position(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_cursor_position_(result, ...) muCOSA_window_get_cursor_position(muCOSA_global_context, result, __VA_ARGS__)
```


#### Set cursor position

The function `muCOSA_window_set_cursor_position` sets the position of the cursor relative to a window, defined below: 

```c
MUDEF void muCOSA_window_set_cursor_position(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m x, int32_m y);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_cursor_position(...) muCOSA_window_set_cursor_position(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_cursor_position_(result, ...) muCOSA_window_set_cursor_position(muCOSA_global_context, result, __VA_ARGS__)
```


#### Get cursor style

The function `muCOSA_window_get_cursor_style` gets the cursor style of a window, defined below: 

```c
MUDEF muCursorStyle muCOSA_window_get_cursor_style(muCOSAContext* context, muCOSAResult* result, muWindow window);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_cursor_style(...) muCOSA_window_get_cursor_style(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_cursor_style_(result, ...) muCOSA_window_get_cursor_style(muCOSA_global_context, result, __VA_ARGS__)
```


#### Set cursor style

The function `muCOSA_window_set_cursor_style` sets the cursor style of a window, defined below: 

```c
MUDEF void muCOSA_window_set_cursor_style(muCOSAContext* context, muCOSAResult* result, muWindow window, muCursorStyle style);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_cursor_style(...) muCOSA_window_set_cursor_style(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_cursor_style_(result, ...) muCOSA_window_set_cursor_style(muCOSA_global_context, result, __VA_ARGS__)
```


#### Get scroll level

The function `muCOSA_window_get_scroll_level` gets the scroll level of a window, defined below: 

```c
MUDEF int32_m muCOSA_window_get_scroll_level(muCOSAContext* context, muCOSAResult* result, muWindow window);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_scroll_level(...) muCOSA_window_get_scroll_level(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_scroll_level_(result, ...) muCOSA_window_get_scroll_level(muCOSA_global_context, result, __VA_ARGS__)
```


#### Set scroll level

The function `muCOSA_window_set_scroll_level` sets the scroll level of a window, defined below: 

```c
MUDEF void muCOSA_window_set_scroll_level(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m scroll_level);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_scroll_level(...) muCOSA_window_set_scroll_level(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_scroll_level_(result, ...) muCOSA_window_set_scroll_level(muCOSA_global_context, result, __VA_ARGS__)
```


### Text input functions

#### Get text input focus

The function `muCOSA_window_get_text_input_focus` focuses text input on a window, defined below: 

```c
MUDEF void muCOSA_window_get_text_input_focus(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m text_cursor_x, int32_m text_cursor_y, void (*callback)(muWindow window, const char* input));
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_text_input_focus(...) muCOSA_window_get_text_input_focus(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_text_input_focus_(result, ...) muCOSA_window_get_text_input_focus(muCOSA_global_context, result, __VA_ARGS__)
```


#### Update text cursor

The function `muCOSA_window_update_text_cursor` updates the text cursor for text input to a position relative to a window, defined below: 

```c
MUDEF void muCOSA_window_update_text_cursor(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m x, int32_m y);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_update_text_cursor(...) muCOSA_window_update_text_cursor(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_update_text_cursor_(result, ...) muCOSA_window_update_text_cursor(muCOSA_global_context, result, __VA_ARGS__)
```


Note that this function is safe to call even when text input is not focused on the window.

#### Let go of text input focus

The function `muCOSA_window_let_text_input_focus` lets go of the text input focus that a window has, defined below: 

```c
MUDEF void muCOSA_window_let_text_input_focus(muCOSAContext* context, muCOSAResult* result, muWindow window);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_let_text_input_focus(...) muCOSA_window_let_text_input_focus(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_let_text_input_focus_(result, ...) muCOSA_window_let_text_input_focus(muCOSA_global_context, result, __VA_ARGS__)
```


### Get window states/properties

#### Get frame extents

The function `muCOSA_window_get_frame_extents` gets the frame extents of a window, defined below: 

```c
MUDEF void muCOSA_window_get_frame_extents(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m* left, uint32_m* right, uint32_m* top, uint32_m* bottom);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_frame_extents(...) muCOSA_window_get_frame_extents(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_frame_extents_(result, ...) muCOSA_window_get_frame_extents(muCOSA_global_context, result, __VA_ARGS__)
```


#### Get keyboard key state

The function `muCOSA_window_get_keyboard_key_state` gets the state of a keyboard key in regards to a given window, defined below: 

```c
MUDEF muButtonState muCOSA_window_get_keyboard_key_state(muCOSAContext* context, muCOSAResult* result, muWindow window, muKeyboardKey key);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_keyboard_key_state(...) muCOSA_window_get_keyboard_key_state(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_keyboard_key_state_(result, ...) muCOSA_window_get_keyboard_key_state(muCOSA_global_context, result, __VA_ARGS__)
```


#### Get keyboard state state

The function `muCOSA_window_get_keyboard_state_state` gets the state of a keyboard state in regards to a given window, defined below: 

```c
MUDEF muState muCOSA_window_get_keyboard_state_state(muCOSAContext* context, muCOSAResult* result, muWindow window, muKeyboardState state);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_keyboard_state_state(...) muCOSA_window_get_keyboard_state_state(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_keyboard_state_state_(result, ...) muCOSA_window_get_keyboard_state_state(muCOSA_global_context, result, __VA_ARGS__)
```


#### Get mouse button state

The function `muCOSA_window_get_mouse_button_state` gets the state of a mouse button in regards to a given window, defined below: 

```c
MUDEF muButtonState muCOSA_window_get_mouse_button_state(muCOSAContext* context, muCOSAResult* result, muWindow window, muMouseButton button);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_get_mouse_button_state(...) muCOSA_window_get_mouse_button_state(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_get_mouse_button_state_(result, ...) muCOSA_window_get_mouse_button_state(muCOSA_global_context, result, __VA_ARGS__)
```


### Set window state/properties

#### Set title

The function `muCOSA_window_set_title` sets the title of a window, defined below: 

```c
MUDEF void muCOSA_window_set_title(muCOSAContext* context, muCOSAResult* result, muWindow window, const char* title);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_title(...) muCOSA_window_set_title(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_title_(result, ...) muCOSA_window_set_title(muCOSA_global_context, result, __VA_ARGS__)
```


### Set callback functions

Note that these functions override any previous function associated with a particular callback event; there is no given way to stack callback functions.

Note that for all of these functions, setting the callback to 0 is safe, and will simply disable the callback.

#### Dimensions callback

The function `muCOSA_window_set_dimensions_callback` sets the callback for the event of a window's dimensions changing, defined below: 

```c
MUDEF void muCOSA_window_set_dimensions_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, uint32_m width, uint32_m height));
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_dimensions_callback(...) muCOSA_window_set_dimensions_callback(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_dimensions_callback_(result, ...) muCOSA_window_set_dimensions_callback(muCOSA_global_context, result, __VA_ARGS__)
```


#### Position callback

The function `muCOSA_window_set_position_callback` sets the callback for the event of a window's position changing, defined below: 

```c
MUDEF void muCOSA_window_set_position_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m x, int32_m y));
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_position_callback(...) muCOSA_window_set_position_callback(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_position_callback_(result, ...) muCOSA_window_set_position_callback(muCOSA_global_context, result, __VA_ARGS__)
```


#### Focus callback

The function `muCOSA_window_set_focus_callback` sets the callback for the event of a window's focused state changing, defined below: 

```c
MUDEF void muCOSA_window_set_focus_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool focused));
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_focus_callback(...) muCOSA_window_set_focus_callback(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_focus_callback_(result, ...) muCOSA_window_set_focus_callback(muCOSA_global_context, result, __VA_ARGS__)
```


#### Maximize callback

The function `muCOSA_window_set_maximize_callback` sets the callback for the event of a window's maximized state changing, defined below: 

```c
MUDEF void muCOSA_window_set_maximize_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool maximized));
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_maximize_callback(...) muCOSA_window_set_maximize_callback(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_maximize_callback_(result, ...) muCOSA_window_set_maximize_callback(muCOSA_global_context, result, __VA_ARGS__)
```


#### Minimize callback

The function `muCOSA_window_set_minimize_callback` sets the callback for the event of a window's minimized state changing, defined below: 

```c
MUDEF void muCOSA_window_set_minimize_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool minimized));
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_minimize_callback(...) muCOSA_window_set_minimize_callback(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_minimize_callback_(result, ...) muCOSA_window_set_minimize_callback(muCOSA_global_context, result, __VA_ARGS__)
```


#### Keyboard key callback

The function `muCOSA_window_set_keyboard_key_callback` sets the callback for the event of a keyboard key state changing in regards to a window, defined below: 

```c
MUDEF void muCOSA_window_set_keyboard_key_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state));
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_keyboard_key_callback(...) muCOSA_window_set_keyboard_key_callback(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_keyboard_key_callback_(result, ...) muCOSA_window_set_keyboard_key_callback(muCOSA_global_context, result, __VA_ARGS__)
```


#### Keyboard state callback

The function `muCOSA_window_set_keyboard_state_callback` sets the callback for the event of a keyboard state state changing in regards to a window, defined below: 

```c
MUDEF void muCOSA_window_set_keyboard_state_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardState keyboard_state, muState state));
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_keyboard_state_callback(...) muCOSA_window_set_keyboard_state_callback(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_keyboard_state_callback_(result, ...) muCOSA_window_set_keyboard_state_callback(muCOSA_global_context, result, __VA_ARGS__)
```


#### Cursor position callback

The function `muCOSA_window_set_cursor_position_callback` sets the callback for the event of the cursor position changing in regards to a window, defined below: 

```c
MUDEF void muCOSA_window_set_cursor_position_callback(muCOSAContext* context, muCOSAResult* result, muWindow win, void (*callback)(muWindow window, int32_m x, int32_m y));
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_cursor_position_callback(...) muCOSA_window_set_cursor_position_callback(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_cursor_position_callback_(result, ...) muCOSA_window_set_cursor_position_callback(muCOSA_global_context, result, __VA_ARGS__)
```


#### Mouse button callback

The function `muCOSA_window_set_mouse_button_callback` sets the callback for the event of a mouse button state changing in regards to a window, defined below: 

```c
MUDEF void muCOSA_window_set_mouse_button_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muMouseButton mouse_button, muButtonState state));
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_mouse_button_callback(...) muCOSA_window_set_mouse_button_callback(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_mouse_button_callback_(result, ...) muCOSA_window_set_mouse_button_callback(muCOSA_global_context, result, __VA_ARGS__)
```


#### Scroll callback

The function `muCOSA_window_set_scroll_callback` sets the callback for the event of scrolling in regards to a window, defined below: 

```c
MUDEF void muCOSA_window_set_scroll_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m scroll_level_add));
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_window_set_scroll_callback(...) muCOSA_window_set_scroll_callback(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_window_set_scroll_callback_(result, ...) muCOSA_window_set_scroll_callback(muCOSA_global_context, result, __VA_ARGS__)
```


## Time functions

### Get time

The function `muCOSA_time_get` returns the time since the given muCOSA context has been created, defined below: 

```c
MUDEF double muCOSA_time_get(muCOSAContext* context, muCOSAResult* result);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_time_get() muCOSA_time_get(muCOSA_global_context, &muCOSA_global_context->result)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_time_get_(result) muCOSA_time_get(muCOSA_global_context, result)
```


Note that the time can be manually changed via `muCOSA_time_set`, which would change the values returned by this function respectively.

### Set time

The function `muCOSA_time_set` sets the time used by the given muCOSA context to refer to how long it has been since the context has been created, defined below: 

```c
MUDEF void muCOSA_time_set(muCOSAContext* context, muCOSAResult* result, double time);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_time_set(...) muCOSA_time_set(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_time_set_(result, ...) muCOSA_time_set(muCOSA_global_context, result, __VA_ARGS__)
```


Note that this interferes with the values returned by `muCOSA_time_get`.

### Sleep

The function `muCOSA_sleep` blocks for a given amount of time (in seconds), defined below: 

```c
MUDEF void muCOSA_sleep(muCOSAContext* context, muCOSAResult* result, double time);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_sleep(...) muCOSA_sleep(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_sleep_(result, ...) muCOSA_sleep(muCOSA_global_context, result, __VA_ARGS__)
```


## Clipboard functions

### Get clipboard

The function `muCOSA_clipboard_get` retrieves the current global clipboard, defined below: 

```c
MUDEF char* muCOSA_clipboard_get(muCOSAContext* context, muCOSAResult* result);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_clipboard_get() muCOSA_clipboard_get(muCOSA_global_context, &muCOSA_global_context->result)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_clipboard_get_(result) muCOSA_clipboard_get(muCOSA_global_context, result)
```


Note that this function can return 0 and still have gone successfully in the event of there being no global clipboard at the time of the function being called.

### Set clipboard

The function `muCOSA_clipboard_set` sets the global clipboard, defined below: 

```c
MUDEF void muCOSA_clipboard_set(muCOSAContext* context, muCOSAResult* result, const char* text, size_m text_size);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_clipboard_set(...) muCOSA_clipboard_set(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_clipboard_set_(result, ...) muCOSA_clipboard_set(muCOSA_global_context, result, __VA_ARGS__)
```


## OS functions

### Get window handle

The function `muCOSA_os_get_window_handle` retrieves a pointer to an OS-specfic handle in regards to a window: 

```c
MUDEF void* muCOSA_os_get_window_handle(muCOSAContext* context, muCOSAResult* result, muWindow window, muWindowHandle handle);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_os_get_window_handle(...) muCOSA_os_get_window_handle(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_os_get_window_handle_(result, ...) muCOSA_os_get_window_handle(muCOSA_global_context, result, __VA_ARGS__)
```


Note that this function returns a *pointer* to the requested type; retrieving the `HWND` associated with a window in Win32 could look like this:

```c
void* v_ptr = muCOSA_os_get_window_handle(...);
HWND hwnd = *((HWND*)v_ptr);
```

This also means that if the type of the requested handle itself is a pointer, the function returns a pointer *to* the pointer; retrieving the `Display` associated with a window in X11 could look like this:

```c
void* v_ptr = muCOSA_os_get_window_handle(...);
Display* dpy = *((Display**)v_ptr);
```

## OpenGL functions

### Bind window's OpenGL context

The function `muCOSA_opengl_bind_window` binds the OpenGL context associated with a given window, defined below: 

```c
MUDEF void muCOSA_opengl_bind_window(muCOSAContext* context, muCOSAResult* result, muWindow window);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_opengl_bind_window(...) muCOSA_opengl_bind_window(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_opengl_bind_window_(result, ...) muCOSA_opengl_bind_window(muCOSA_global_context, result, __VA_ARGS__)
```


Note that this function must be called with a window that was created with an OpenGL graphics API as its graphics API.

### Get OpenGL function address

The function `muCOSA_opengl_get_function_address` returns the address of a given OpenGL function if it could be found, defined below: 

```c
MUDEF void* muCOSA_opengl_get_function_address(muCOSAContext* context, const char* name);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_opengl_get_function_address(...) muCOSA_opengl_get_function_address(muCOSA_global_context, __VA_ARGS__)
```


Note that there is no result-checking equivalent of this function.

### Set OpenGL swap interval

The function `muCOSA_opengl_window_swap_interval` sets the swap interval of an OpenGL context associated with a given window, defined below: 

```c
MUDEF void muCOSA_opengl_window_swap_interval(muCOSAContext* context, muCOSAResult* result, muWindow window, int interval);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_opengl_window_swap_interval(...) muCOSA_opengl_window_swap_interval(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_opengl_window_swap_interval_(result, ...) muCOSA_opengl_window_swap_interval(muCOSA_global_context, result, __VA_ARGS__)
```


## Vulkan functions

### Get Vulkan surface extensions

The function `muCOSA_vulkan_get_surface_instance_extensions` returns a list of Vulkan etensions necessary to create a window surface for the window system associated with the given muCOSA context, defined below: 

```c
MUDEF const char** muCOSA_vulkan_get_surface_instance_extensions(muCOSAContext* context, muCOSAResult* result, size_m* count);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_vulkan_get_surface_instance_extensions(...) muCOSA_vulkan_get_surface_instance_extensions(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_vulkan_get_surface_instance_extensions_(result, ...) muCOSA_vulkan_get_surface_instance_extensions(muCOSA_global_context, result, __VA_ARGS__)
```


### Create Vulkan window surface

The function `muCOSA_vulkan_create_window_surface` creates a Vulkan surface for a given window, defined below: 

```c
MUDEF void muCOSA_vulkan_create_window_surface(muCOSAContext* context, muCOSAResult* result, muWindow window, void* vk_result, void* instance, void* allocator, void* surface);
```


Its non-result-checking equivalent macro is defined below: 

```c
#define mu_vulkan_create_window_surface(...) muCOSA_vulkan_create_window_surface(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
```


Its result-checking equivalent macro is defined below: 

```c
#define mu_vulkan_create_window_surface_(result, ...) muCOSA_vulkan_create_window_surface(muCOSA_global_context, result, __VA_ARGS__)
```


Note that the parameters of this function are `void*` to allow the function to at least be defined even if Vulkan is not supported; their interpreted types are:

`vk_result` as `VkResult*`.

`instance` as `VkInstance*`.

`allocator` as `const VkAllocationCallbacks**`.

`surface` as `VkSurfaceKHR*`.

Note that with this function, `result` represents if anything went wrong on the muCOSA end of things (such as Vulkan support not being defined), and `vk_result` represents if a Vulkan function went wrong.

## Name functions

There are several functions used to convert enumerator values into `const char*` representations of them, which are ***only defined if `MUCOSA_NAMES` is defined***. These are:

converting `muCOSAResult` to `const char*`: 

```c
MUDEF const char* muCOSA_result_get_name(muCOSAResult result);
```


converting `muWindowSystem` to `const char*`: 

```c
MUDEF const char* mu_window_system_get_name(muWindowSystem system);
```




```c
MUDEF const char* mu_window_system_get_nice_name(muWindowSystem system);
```


converting `muWindowHandle` to `const char*`: 

```c
MUDEF const char* mu_window_handle_get_name(muWindowHandle handle);
```




```c
MUDEF const char* mu_window_handle_get_nice_name(muWindowHandle handle);
```


converting `muGraphicsAPI` to `const char*`: 

```c
MUDEF const char* mu_graphics_api_get_name(muGraphicsAPI api);
```




```c
MUDEF const char* mu_graphics_api_get_nice_name(muGraphicsAPI api);
```


converting `muCursorStyle` to `const char*`: 

```c
MUDEF const char* mu_cursor_style_get_name(muCursorStyle style);
```




```c
MUDEF const char* mu_cursor_style_get_nice_name(muCursorStyle style);
```


converting `muKeyboardKey` to `const char*`: 

```c
MUDEF const char* mu_keyboard_key_get_name(muKeyboardKey key);
```




```c
MUDEF const char* mu_keyboard_key_get_nice_name(muKeyboardKey key);
```


converting `muKeyboardState` to `const char*`: 

```c
MUDEF const char* mu_keyboard_state_get_name(muKeyboardState state);
```




```c
MUDEF const char* mu_keyboard_state_get_nice_name(muKeyboardState state);
```


converting `muMouseButton` to `const char*`: 

```c
MUDEF const char* mu_mouse_button_get_name(muMouseButton button);
```




```c
MUDEF const char* mu_mouse_button_get_nice_name(muMouseButton button);
```


Note that the `nice_name` functions return nicer-looking `const char*` representations of the enum values. For example:

```c
mu_graphics_api_get_nice_name(MU_OPENGL_3_3_COMPATIBILITY)
```
returns
```c
"OpenGL 3.3 Compatibility"
```
and
```c
mu_graphics_api_get_name(MU_OPENGL_3_3_COMPATIBILITY)
```
returns
```c
"MU_OPENGL_3_3_COMPATIBILITY"
```

# C standard library dependencies

muCOSA has several C standard library dependencies not provided by its other library dependencies, all of which are overridable by defining them before the inclusion of its header. This is a list of all of those dependencies.

## `stdlib.h` dependencies

`mu_malloc`: equivalent to `malloc`.

`mu_free`: equivalent to `free`.

`mu_realloc`: equivalent to `realloc`.

## `string.h` dependencies

`mu_memcpy`: equivalent to `memcpy`.

`mu_memset`: equivalent to `memset`.

`mu_strlen`: equivalent to `strlen`.

`mu_strcmp`: equivalent to `strcmp`.

## `stdio.h` dependencies

`mu_fflush`: equivalent to `fflush`.

`mu_stdout`: equivalent to `stdout`.

## `locale.h` dependencies

`mu_setlocale`: equivalent to `setlocale`.

`MU_LC_CTYPE`: equivalent to `LC_CTYPE`.
