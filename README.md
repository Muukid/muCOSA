

# muCOSA v2.0.0

muCOSA (COSA standing for cross operating-system API) is a public domain single-file C library for interacting with operating systems with a cross-platform API. Its header is automatically defined upon inclusion if not already included (`MUCOSA_H`), and the source code is defined if `MUCOSA_IMPLEMENTATION` is defined, following the interal structure of:

```c
#ifndef MUCOSA_H
#define MUCOSA_H
// (Header code)
#endif

#ifdef MUCOSA_IMPLEMENTATION
// (Source code)
#endif
```

Therefore, a standard inclusion of the file to get all automatic functionality looks like:

```c
#define MUCOSA_IMPLEMENTATION
#include "muCOSA.h"
```

More information about the general structure of a mu library is provided at [the mu library information GitHub repository](https://github.com/Muukid/mu-library-information).

# Demos

Demos are designed for muCOSA to both test its functionality and allow users to get the basic idea of the structure of the library quickly without having to read the documentation in full. These demos are available in the `demos` folder.

## Demo dependencies

Since the demos test the functionality of OpenGL, [glad](https://github.com/Dav1dde/glad) is used as an OpenGL loader in the demos (with [these settings](http://glad.sh/#api=gl%3Acore%3D3.3&extensions=&generator=c&options=HEADER_ONLY%2CLOADER) if you're interested), and therefore needs to be included when compiling the demos. Include dependencies are stored in the `include` folder within demos, and all files within this folder should be in the user's include directory when compiling them.

> Note that the inclusion of glad changes the conditions of the licensing due to Khronos's Apache 2.0 license for OpenGL specifications; more information is given in the [licensing](#licensing) section of this documentation.

# System dependencies

muCOSA is supported for Win32. This operating system requires the user to link to certain operating system files when compiling.

## Win32 dependencies

To compile with Windows, you need to link the following files under the given circumstances:

`user32.dll` and `imm32.dll` in any given circumstance.

`gdi32.dll` and `opengl32.dll` if `MU_SUPPORT_OPENGL` is defined by the user.

# Licensing

muCOSA is licensed under public domain or MIT, whichever you prefer. More information is provided in the accompanying file `license.md` and at the bottom of `muCOSA.h`.

However, note that the demos that use OpenGL use glad, which is built from the Khronos specification for OpenGL, so its Apache 2.0 license applies within that context. See [further clarification in this issue comment](https://github.com/KhronosGroup/OpenGL-Registry/issues/376#issuecomment-596187053).

# Multi-threading

muCOSA does not directly support thread safety, and must be implemented by the user themselves. Thread safety in muCOSA can be generally achieved by locking each object within a muCOSA context (ie `muWindow` for example), making sure that only one thread is interacting with the given object. There are no known exceptions to this to achieve thread safety, but multi-threading with muCOSA is not thoroughly tested.

# Known bugs and limitations

This section covers all of the known bugs and limitations with muCOSA.

## Limited support for most stuff

This version of muCOSA is intended to be very basic, meaning that it only supports Windows and OpenGL, and is not thoroughly tested on other devices. This, if not abandoned, will change in the future, as more support is added, but for now, this library's reach will be fairly limited.

## Minimal overhead attribute management

Currently, muCOSA gets/sets attributes using a single function that requires at least one get/set call for every attribute being modified. Theoretically, more overhead could be abolished by allowing to get/set multiple attributes in one function call, perhaps using the `muWindowInfo` struct and a flag system. This has not been outruled as an option, and muCOSA may stand to gain via this being implemented at some point.

## Inefficient window memory allocation on Win32

Due to limitations with how a window is identified via its handle in the window procedure functions, the implementation for Win32 has a memory buffer that contains pointers to each window. The logic for this uses filling in empty slots to ensure that a fair amount of memory is allocated even in the event of a large amount of windows being destroyed and created. However, the logic of it currently does not decrease the amount of memory allocated for the window pointers, meaning that a peak in the amount of windows created (across all muCOSA contexts) will peak the memory usage for the buffer, and will not decrease until all muCOSA contexts are destroyed.

This is generally okay, both because the maximum amount of memory allocated for this buffer is the size of a pointer multiplied by the closest power of 2 greater than the amount of windows. So, for example, if there were a maximum of 2718 windows reached at some point during the program's lifespan, and the device is using 64 bits per pointer, the memory usage by this buffer will peak at `64*4096`, or 262144 bytes, and will not decrease until all muCOSA contexts are destroyed. However, it is not only unlikely that a program will ever use this amount of windows, but it is even more unlikely that Windows itself will be able to run properly after this amount of windows have been created.


# Other library dependencies

muCOSA has a dependency on:

* [muUtility v2.0.1](https://github.com/Muukid/muUtility/releases/tag/v2.0.1).

> Note that mu libraries store their dependencies within their files, so you don't need to import these dependencies yourself; this section is purely to give more information about the contents that this file defines. The libraries listed may also have other dependencies that they also include that aren't explicitly listed here.

# Window systems

The type `muWindowSystem` (typedef for `uint8_m`) is used to define all of the currently supported window systems. It has the following defined values:

* `MU_WINDOW_NULL` - Unknown/Null window system; real value 0. This value can also act as an "auto" window system, such as when creating a muCOSA context with it, automatically picking the best currently available window system.

* `MU_WINDOW_WIN32` - Win32; real value 1.

## Window system names

> Note that although on most operating systems, only one window system can exist (such as macOS or Windows), some operating systems can have more than one window system, such as Linux with X11 or Wayland. Just in case, muCOSA allows more than one window system to be defined at once in its API, tying each muCOSA context to a particular window system, theoretically allowing for multiple muCOSA contexts to exist at once with different window systems in one program.

The name function `mu_window_system_get_name` returns a `const char*` representation of the given window sytem (for example, `MU_WINDOW_NULL` returns "MU_WINDOW_NULL"), defined below: 

```c
MUDEF const char* mu_window_system_get_name(muWindowSystem system);
```


It will return "MUCOSA_UNKNOWN" in the case that `system` is an invalid window system value.

The name function `mu_window_system_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_WINDOW_NULL` returns "Unknown/Auto"), defined below: 

```c
MUDEF const char* mu_window_system_get_nice_name(muWindowSystem system);
```


It will return "Unknown" in the case that `system` is an invalid window system value.

> These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.

## Manual window system support

Not all window systems are supported upfront, so it is automatically guessed based on the current operating system that the code is compiling for using macros provided by muUtility.

The explicit functionality is this:

* `MUCOSA_WIN32` is defined if `MU_WIN32` is defined (automatically provided by muUtility), which toggles support for Win32.

All of this functionality can be overrided by defining the macro `MUCOSA_MANUAL_OS_SUPPORT`, in which case, none of this is performed, and it is up to the user to manually define that operating systems are supported by defining their respective muCOSA macros (ie, if you're compiling on Windows and disabled automatic operating system recognition, you would need to define `MUCOSA_WIN32` yourself).

# Global context

muCOSA operates in a context, encapsulated by the type `muCOSAContext`, which has the following members:

* `void* inner` - pointer to internally-used manually-allocated memory for running the current operating system context. The user should never interact with this member.

* `muCOSAResult result` - the result of the latest non-successful non-result-checking function call regarding the context; starting value upon context creation is `MUCOSA_SUCCESS`, and is set to another value if a function whose result is not set manually by the user doesn't return a success result value.

## Creation and destruction

To create the context, the function `muCOSA_context_create` is used, defined below: 

```c
MUDEF void muCOSA_context_create(muCOSAContext* context, muWindowSystem system, muBool set_context);
```


The result of this function is stored within `context->result`. Upon success, this function automatically calls `muCOSA_context_set` on the created context unless `set_context` is equal to `MU_FALSE`.

For every successfully created context, it must be destroyed, which is done with the function `muCOSA_context_destroy`, defined below: 

```c
MUDEF void muCOSA_context_destroy(muCOSAContext* context);
```


This function cannot fail if given a valid pointer to an active context (otherwise, a crash is likely), so no result value is ever indicated by this function via any means.

## Global context

muCOSA uses a global variable to reference the currently "set" context whenever a function is called that assumes a context (ie it doesn't take a parameter for context). This global variable can be changed to reference a certain context via the function `muCOSA_context_set`, defined below: 

```c
MUDEF void muCOSA_context_set(muCOSAContext* context);
```


## Get context information

The only information that can be retrieved about an active context is its window system, using the function `muCOSA_context_get_window_system`, defined below: 

```c
MUDEF muWindowSystem muCOSA_context_get_window_system(muCOSAContext* context);
```


## Non-result/context checking functions

If a function takes a `muCOSAContext` and `muCOSAResult` parameter, there will likely be two defined macros for calling the function without explicitly passing these parameters, with the current global context being assumed for both parameters.

Non-result-checking functions are functions that assume the `muCOSAContext` parameter to be the current global context, and assume that the `muCOSAResult` parameter to be the current global context's result member. These functions' parameters are simply the normal function's parameters but without the context or result paramter, instead being routed to the current global context. The name of these functions are simply the normal name but `muCOSA_...` being replaced with just `mu_...`.

Result-checking functions are functions that also assume (and thus don't make you specify) the `muCOSAContext` parameter to be the current global context, but they still make you specify the `muCOSAResult` parameter, and the global context's result member goes unmodified. The name of these functions is the same as the non-result-checking functions, but with an underscore appended at the end.

For example, if the function `muCOSA_window_do_something` existed with these parameters:

```c
MUDEF void muCOSA_window_do_something(muCOSAContext* context, muCOSAResult* result, int a, int b);
```

then the function `mu_window_do_something`, the "non-result-checking function", exists with these parameters:

```c
MUDEF void mu_window_do_something(int a, int b);
// Calls muCOSA_window_do_something with the global context
// and its respective result member.
```

and the function `mu_window_do_something_`, the "result-checking function", exists with these parameters:

```c
MUDEF void mu_window_do_something_(muCOSAResult* result, int a, int b);
// Calls muCOSA_window_do_something with the global context
// and the given result pointer.
```

> Note that, in reality, the non-result and result-checking functions aren't defined as actual independent functions, but instead, macros to the original function. More information about the type `muCOSAResult` can be found in the [result section](#result).


# The window

muCOSA works in a desktop OS environment, and therefore, its functionality primarily centers around the window (respective type `muWindow`; macro for `void*`).

The documentation of this library will use the term "surface" in regards to the window's rendering space, and is simply the part of the window that is rendered to, excluding the "frame extents", which is comprised of things like the side bars, title bar, close button, etc.

## Window information

The struct `muWindowInfo` represents information about a window. It has the following members:

* `char* title` - the title of the window shown to the user in most interfaces (primarily the title bar).

* `uint32_m width` - the width of the window's surface, in pixels.

* `uint32_m height` - the height of the window's surface, in pixels.

* `int32_m x` - the x-coordinate of the top-leftest pixel in the window's surface relative to the entire window space of the window system.

* `int32_m y` - the y-coordinate of the top-leftest pixel in the window's surface relative to the entire window space of the window system.

> Note that due to the way certain window systems work, negative coordiantes may not function properly for a given window in regards to setting them to that value, and should not be relied upon for functionality.

## Window creation and destruction

In order to create a window, the function `muCOSA_window_create` must be called, defined below: 

```c
MUDEF muWindow muCOSA_window_create(muCOSAContext* context, muCOSAResult* result, muWindowInfo* info);
```


Upon failure (marked by the value of `result`), the creation function returns 0.

> The macro `mu_window_create` is the non-result-checking equivalent, and the macro `mu_window_create_` is the result-checking equivalent.

Every successfully created window must be destroyed at some point during the relevant context's lifetime using the function `muCOSA_window_destroy`, defined below: 

```c
MUDEF muWindow muCOSA_window_destroy(muCOSAContext* context, muWindow win);
```


The destruction function cannot fail if given a proper context and window, and thus, there is no `result` parameter.

> The macro `mu_window_destroy` is the non-result-checking equivalent.

## The main loop

The "main loop" of a window consists of three functions that are called each "frame"; `muCOSA_window_get_closed`, `muCOSA_window_swap_buffers`, and `muCOSA_window_update`. A main loop with these functions generally looks like this:

```c
// Initialization / Window creation here...

while (!mu_window_get_closed(window)) {

// ... (This is where the frame-by-frame logic would go) ...

mu_window_swap_buffers(window);
mu_window_update(window);
}

// Termination here...
```

### Get closed

The function `muCOSA_window_get_closed` returns whether or not a given window has been closed, defined below: 

```c
MUDEF muBool muCOSA_window_get_closed(muCOSAContext* context, muCOSAResult* result, muWindow win);
```


Once this function returns `MU_FALSE`, it is no longer usable in all circumstances other than destroying it with `muCOSA_window_destroy`; a window cannot be revived once closed.

> The macro `mu_window_get_closed` is the non-result-checking equivalent, and the macro `mu_window_get_closed_` is the result-checking equivalent.

### Swap buffers

The function `muCOSA_window_swap_buffers` swaps the image buffers of a given window, presenting the contents of the surface, defined below: 

```c
MUDEF void muCOSA_window_swap_buffers(muCOSAContext* context, muCOSAResult* result, muWindow win);
```


This function should be called every frame regardless of whether or not the actual surface has been rendered to.

> The macro `mu_window_swap_buffers` is the non-result-checking equivalent, and the macro `mu_window_swap_buffers_` is the result-checking equivalent.

### Update

The function `muCOSA_window_update` updates/refreshes a window and triggers all relevant callbacks, presenting the contents of the surface, defined below: 

```c
MUDEF void muCOSA_window_update(muCOSAContext* context, muCOSAResult* result, muWindow win);
```


> The macro `mu_window_update` is the non-result-checking equivalent, and the macro `mu_window_update_` is the result-checking equivalent.

## Window attributes

The window is described by several attributes, with each attribute represented by the type `muWindowAttrib` (typedef for `uint16_m`). It has the following values:

* `MU_WINDOW_TITLE` - the title of the window, represented by a `char*` UTF-8 string. This cannot be "get", but can be "set".

* `MU_WINDOW_DIMENSIONS` - the width and height of the window's surface, in pixels, represented by an array of two `uint32_m`s, where the first element is the width, and the second element is the height. This can be "get" and "set".

* `MU_WINDOW_POSITION` - the x- and y-coordinates of the top-leftest pixel of the window's surface relative to the entire window space of the window system, represented by an array of two `int32_m`s, where the first element is the x-coordinate, and the second element is the y-coordinate. This can be "get" and "set".

* `MU_WINDOW_KEYBOARD_MAP` - the [keyboard keymap](#keyboard-keymap), represented by a pointer to an array of booleans (type `muBool`) representing the state of each readable keyboard key. This can be "get", but not "set".

> Note that when being read, the data is not expected to be the actual array, but instead a pointer that will be set to the internally-used keymap array, which remains consistent for an entire window's lifespan. More information about the keyboard keymap can be found in the [keymap](#keymaps) section.

* `MU_WINDOW_MOUSE_MAP` - the [mouse keymap](#mouse-keymap), represented by a pointer to an array of booleans (type `muBool`) representing the state of each readable mouse key. This can be "get", but not "set".

* `MU_WINDOW_CURSOR` - the x- and y-coordinates of the visual cursor relative to the position of the window's surface, represented by an array of two `int32_m`s, where the first element is the x-coordinate, and the second element is the y-coordinate. This can be "get" and "set".

* `MU_WINDOW_CURSOR_STYLE` - the [style of the cursor](#cursor-style), represented by a single value `muCursorStyle`. This can be "get" and "set".

A value is "get" if calling `muCOSA_window_get` with it is valid, and a value is "set" if calling `muCOSA_window_set` with it is valid.

### Names

The name function for `muWindowAttrib` is `mu_window_attrib_get_name`, defined below: 

```c
MUDEF const char* mu_window_attrib_get_name(muWindowAttrib attrib);
```


> This function returns "MUCOSA_UNKNOWN" if the value of `attrib` is unrecognized.

The nice name function for `muWindowAttrib` is `mu_window_attrib_get_nice_name`, defined below: 

```c
MUDEF const char* mu_window_attrib_get_nice_name(muWindowAttrib attrib);
```


> This function returns "Unknown" if the value of `attrib` is unrecognized.

### Get and set window attributes

The function `muCOSA_window_get` retrieves an attribute of a window, defined below: 

```c
MUDEF void muCOSA_window_get(muCOSAContext* context, muCOSAResult* result, muWindow win, muWindowAttrib attrib, void* data);
```


> The macro `mu_window_get` is the non-result-checking equivalent, and the macro `mu_window_get_` is the result-checking equivalent.

The function `muCOSA_window_set` modifies an attribute of a window, defined below: 

```c
MUDEF void muCOSA_window_set(muCOSAContext* context, muCOSAResult* result, muWindow win, muWindowAttrib attrib, void* data);
```


> The macro `mu_window_set` is the non-result-checking equivalent, and the macro `mu_window_set_` is the result-checking equivalent.

For both functions, `data` is a pointer to data dictated by the value of `attrib`. In the case of `muCOOSA_window_get`, the data is derefenced and filled in corresponding to the window's requested attribute (if successful); in the case of `muCOSA_window_set`, the data is dereferenced and read, and the requested window attribute is changed to the given value(s) (if successful).

> `mu_window_set` will only read from `data` and never modify it. Likewise, `mu_window_get` will only dereference `data` and never read from it.

## Keymaps

In order to make input require as minimal overhead as possible, muCOSA allows the user to read key input using "keymaps". A keymap is an array of booleans (type `muBool`) that dictate the state of each key. Therefore, if a user wanted to check a particular key's state, they would retrieve the keymap, and index into it based on what key they want to check. This array is stored internally somewhere in the API, and, when retrieved (via a "get" function call), a pointer to this array is given. Since the keymap is stored as a pointer to inner memory used by muCOSA, it is automatically updated every call to `muCOSA_window_update`.

The big advantage of keymaps is that the array remains at the same point in memory for the entire window's lifespan; a user could grab the pointer to the keymap once at the beginning of the program, and instantly know the state of any key via indexing into it, without any need for refreshing or extra function calls as long as the window remains alive.

> Note that when the keymap is retrieved, a *pointer* to it is retrieved, not the array itself. This means that when calling a "get" function with a keymap, a pointer to a pointer should be given, to which muCOSA will dereference the initial pointer and set the pointer to the address of the keymap array.

> Note that keymaps are only meant to be read, not modified. Changing any value within a keymap array will result in undefined behavior.

### Keyboard keymap

The keyboard keymap represents keys on the keyboard readable by muCOSA, using type `muKeyboardKey` (typedef for `uint16_m`) as index. The length of the keymap is `MU_KEYBOARD_LENGTH`. It has the following indexes:

* `MU_KEYBOARD_UNKNOWN` - unknown key.

* `MU_KEYBOARD_BACKSPACE` - the [backspace key](https://en.wikipedia.org/wiki/Backspace).

* `MU_KEYBOARD_TAB` - the [tab key](https://en.wikipedia.org/wiki/Tab_key).

* `MU_KEYBOARD_CLEAR` - the [clear key](https://en.wikipedia.org/wiki/Clear_key).

* `MU_KEYBOARD_RETURN` - the [return key](https://en.wikipedia.org/wiki/Return_key).

* `MU_KEYBOARD_PAUSE` - the [pause key](https://en.wikipedia.org/wiki/Pause_key).

* `MU_KEYBOARD_ESCAPE` - the [escape key](https://en.wikipedia.org/wiki/Escape_key).

* `MU_KEYBOARD_MODECHANGE` - the [modechange key](https://en.wikipedia.org/wiki/Language_input_keys).

* `MU_KEYBOARD_SPACE` - the [space key](https://en.wikipedia.org/wiki/Space_bar).

* `MU_KEYBOARD_PRIOR` - the [page up key](https://en.wikipedia.org/wiki/Page_Up_and_Page_Down_keys).

* `MU_KEYBOARD_NEXT` - the [page down key](https://en.wikipedia.org/wiki/Page_Up_and_Page_Down_keys).

* `MU_KEYBOARD_END` - the [end key](https://en.wikipedia.org/wiki/End_key).

* `MU_KEYBOARD_HOME` - the [home key](https://en.wikipedia.org/wiki/Home_key).

* `MU_KEYBOARD_(LEFT/UP/RIGHT/DOWN)` - the left, up, right, and down (arrow keys)[https://en.wikipedia.org/wiki/Arrow_keys].

* `MU_KEYBOARD_SELECT` - the [select key](https://stackoverflow.com/questions/23995537/what-is-the-select-key).

* `MU_KEYBOARD_PRINT` - the [print key](https://en.wikipedia.org/wiki/Print_Screen).

* `MU_KEYBOARD_EXECUTE` - the execute key.

* `MU_KEYBOARD_INSERT` - the [insert key](https://en.wikipedia.org/wiki/Insert_key).

* `MU_KEYBOARD_DELETE` - the [delete key](https://en.wikipedia.org/wiki/Delete_key).

* `MU_KEYBOARD_HELP` - the [help key](https://en.wikipedia.org/wiki/Help_key).

* `MU_KEYBOARD_(0...9)` - [the number keys](https://en.wikipedia.org/wiki/Numerical_digit) (0-9).

* `MU_KEYBOARD_(A...Z)` - the [alphabet keys](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys) (A-Z).

* `MU_KEYBOARD_(LEFT/RIGHT)_WINDOWS` - the left and right [Windows](https://en.wikipedia.org/wiki/Windows_key)/[super](https://en.wikipedia.org/wiki/Super_key_(keyboard_button))/[command](https://en.wikipedia.org/wiki/Command_key) keys.

* `MU_KEYBOARD_NUMPAD_(0...9)` - the [numpad number keys](https://en.wikipedia.org/wiki/Numeric_keypad) (0-9).

* `MU_KEYBOARD_(ADD/SUBTRACT/MULTIPLY/DIVIDE)` - the addition, subtraction, multiplication, and division [numpad keys](https://en.wikipedia.org/wiki/Numeric_keypad).

* `MU_KEYBOARD_SEPARATOR` - the [separator key](https://stackoverflow.com/questions/67916923/what-physical-key-maps-to-keycode-108-vk-separator).

* `MU_KEYBOARD_DECIMAL` - the [decimal/period/dot key](https://en.wikipedia.org/wiki/Numeric_keypad).

* `MU_KEYBOARD_F(1...24)` - the [function keys](https://en.wikipedia.org/wiki/Function_key) (1-24).

* `MU_KEYBOARD_NUMLOCK` - the [Num Lock key](https://en.wikipedia.org/wiki/Num_Lock).

* `MU_KEYBOARD_SCROLL` - the [Scroll Lock key](https://en.wikipedia.org/wiki/Scroll_Lock).

* `MU_KEYBOARD_(LEFT/RIGHT)_SHIFT` - the left/right [shift keys](https://en.wikipedia.org/wiki/Shift_key).

* `MU_KEYBOARD_(LEFT/RIGHT)_CONTROL` - the left/right [control keys](https://en.wikipedia.org/wiki/Control_key).

* `MU_KEYBOARD_(LEFT/RIGHT)_MENU` - the left/right [menu keys](https://en.wikipedia.org/wiki/Menu_key).

* `MU_KEYBOARD_ATTN` - the [ATTN key](https://www.ibm.com/support/pages/apar/II04992).

* `MU_KEYBOARD_CRSEL` - the [CRSEL key](https://learn.microsoft.com/en-us/dotnet/api/system.windows.forms.keys?view=windowsdesktop-8.0).

* `MU_KEYBOARD_EXSEL` - the [EXSEL key](https://learn.microsoft.com/en-us/dotnet/api/system.windows.forms.keys?view=windowsdesktop-8.0).

* `MU_KEYBOARD_EREOF` - the [EREOF key](https://www.ibm.com/docs/en/wsfz-and-o/1.1?topic=descriptions-ereof-erase-end-field-key-statement).

* `MU_KEYBOARD_PLAY` - the play key.

* `MU_KEYBOARD_PA1` - the [PA1 key](https://www.ibm.com/docs/en/zos-basic-skills?topic=ispf-keyboard-keys-functions).

Once the pointer to the keyboard keymap array has been retrieved via `muCOSA_window_get`, these values can be used as indexes to see the status of any keyboard key, in which `MU_TRUE` indicates that the key is being pressed down, and `MU_FALSE` indicates that the key is released.

### Mouse keymap

The mouse keymap represents the keys on a common computer mouse readable by muCOSA, using type `muMouseKey` (typedef for `uint16_m`) as index. The length of the keymap is `MU_MOUSE_LENGTH`. It has the following indexes:

* `MU_MOUSE_UNKNOWN` - unknown mouse key.

* `MU_MOUSE_LEFT` - the left mouse key.

* `MU_MOUSE_RIGHT` - the right mouse key.

* `MU_MOUSE_MIDDLE` - the middle mouse key; this indicates whether or not the middle mouse key (usually the scroll wheel) is being clicked, not scrolled.

Once the pointer to the mouse keymap array has been retrieved via `muCOSA_window_get`, these values can be used as indexes to see the status of any mouse key, in which `MU_TRUE` indicates that the key is being pressed down, and `MU_FALSE` indicates that the key is released.

## Cursor style

The style of a cursor determines how it visually appears based on a number of presets for its look that the window system provides. Its type is `muCursorStyle` (typedef for `uint16_m`), and has the following values:

* `MU_CURSOR_UNKNOWN` - unknown cursor style.

* `MU_CURSOR_ARROW` - the normal arrow-looking cursor style; equivalent to `IDC_ARROW` in Win32.

* `MU_CURSOR_IBEAM` - the text-select cursor style, appearing as a vertical beam; equivalent to `IDC_IBEAM` in Win32.

* `MU_CURSOR_WAIT` - the waiting/busy/loading cursor style; equivalent to `IDC_WAIT` in Win32.

* `MU_CURSOR_WAIT_ARROW` - the waiting/busy/loading cursor style, but also with the normal arrow visible; equivalent to `IDC_APPSTARTING` in Win32.

* `MU_CURSOR_CROSSHAIR` - the crosshair cursor style; equivalent to `IDC_CROSS` in Win32.

* `MU_CURSOR_HAND` - the finger-pointing/link-select cursor style; equivalent to `IDC_HAND` in Win32.

* `MU_CURSOR_SIZE_LR` - the resize cursor style, pointing left-to-right horizontally; equivalent to `IDC_SIZEWE` in Win32.

* `MU_CURSOR_SIZE_TB` - the resize cursor style, pointing up-to-down vertically; equivalent to `IDC_SIZENS` in Win32.

* `MU_CURSOR_SIZE_TL_BR` - the resize cursor style, pointing from top-left to bottom-right sideways; equivalent to `IDC_NWSE` in Win32.

* `MU_CURSOR_SIZE_TR_BL` - the resize cursor style, pointing from top-right to bottom-left sideways; equivalent to `IDC_NESW` in Win32.

* `MU_CURSOR_ALL` - the move/resize-all cursor style, pointing outwards in all directions; equivalent to `IDC_SIZEALL` in Win32.

* `MU_CURSOR_NO` - the disallowing/error/not-allowed cursor style; equivalent to `IDC_NO` in WIn32.

# Time

Every muCOSA context has a "fixed time", which refers to the amount of seconds it has been since the context was first created, stored internally as a double. The "fixed time" is different than the "time", which is usually equal to the fixed time, unless it is manually overwritten by the user, which is available in the muCOSA API.

## Get fixed time

The function `muCOSA_fixed_time_get` retrieves the current amount of fixed time for a muCOSA context, defined below: 

```c
MUDEF double muCOSA_fixed_time_get(muCOSAContext* context);
```


This function cannot fail if the parameter(s) are valid.

> The macro `mu_fixed_time_get` is the non-result-checking equivalent.

## Get/Set time

The function `muCOSA_time_get` retrieves the current amount of time for a muCOSA context, defined below: 

```c
MUDEF double muCOSA_time_get(muCOSAContext* context);
```


> The macro `mu_time_get` is the non-result-checking equivalent.

The function `muCOSA_time_set` overwrites the current time for a muCOSA context, defined below: 

```c
MUDEF void muCOSA_time_set(muCOSAContext* context, double time);
```


This function is valid to call with negative values.

> The macro `mu_time_set` is the non-result-checking equivalent.

These functions cannot fail if the parameter(s) are valid.

# Sleep

The sleep function `muCOSA_sleep` is used to sleep for a given amount of seconds, defined below: 

```c
MUDEF void muCOSA_sleep(muCOSAContext* context, double time);
```


This function cannot fail if the parameter(s) are valid.

> The macro `mu_sleep` is the non-result-checking equivalent.

# Result

The type `muCOSAResult` (typedef for `uint16_m`) is used to represent how a task in muCOSA went. It has the following defined values:

* `MUCOSA_SUCCESS` - the task succeeded; real value 0.

* `MUCOSA_FAILED_NULL_WINDOW_SYSTEM` - rather an invalid window system value was given by the user, the window system value given by the user was unsupported, or no supported window system could be found.

* `MUCOSA_FAILED_MALLOC` - a call to `mu_malloc` failed, meaning that there is insufficient memory available to perform the task.

* `MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB` - an invalid `muWindowAttrib` value was given by the user.

* `MUCOSA_FAILED_REALLOC` - a call to `mu_realloc` failed, meaning that there is insufficient memory available to perform the task.

* `MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR` - a conversion from a UTF-8 string to a wide character string failed, rather due to the conversion itself failing or the allocation of memory required for the conversion; this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS` - a call to `RegisterClassExW` failed, meaning that the window class needed to create the window could not be created; this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_CREATE_WINDOW` - a call to `CreateWindowExW` failed, meaning that the window could not be created; this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB` - whatever function needed to retrieve the requested window attribute returned a non-success value; this is exclusive to Win32.

   * In the case of dimensions, `GetClientRect` failed.

   * In the case of position, `GetWindowRect` failed.

   * In the case of cursor, rather `GetCursorPos` or `muCOSA_window_get(...MU_WINDOW_POSITION)` failed.

* `MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB` - whatever function needed to modify the requested window attribute returned a non-success value; this is exclusive to Win32.

   * In the case of title, `SetWindowTextW` failed.

   * In the case of dimensions, rather `GetWindowInfo`, `AdjustWindowRect`, or `SetWindowPos` failed.

   * In the case of position, `SetWindowPos` failed.

   * In the case of cursor, rather `SetCursorPos` or `muCOSA_window_get(...MU_WINDOW_POSITION)` failed.

All non-success values (unless explicitly stated otherwise) mean that the function fully failed, and the library continues as if the function had never been called; so, for example, if something was supposed to be allocated, but the function failed, nothing was allocated.

## Result names

The name function `muCOSA_result_get_name` returns a `const char*` representation of the given result value (for example, `MUCOSA_SUCCESS` returns "MUCOSA_SUCCESS"), defined below: 

```c
MUDEF const char* muCOSA_result_get_name(muCOSAResult result);
```


It will return "MUCOSA_UNKNOWN" in the case that `result` is an invalid result value.

> This function is a "name" function, and therefore is only defined if `MUCOSA_NAMES` is also defined by the user.

# C standard library dependencies

muCOSA has several C standard library dependencies, all of which are overridable by defining them before the inclusion of its header. The following is a list of those dependencies.

## `string.h` dependencies

* `mu_memcpy` - equivalent to `memcpy`.

* `mu_memset` - equivalent to `memset`.

## `stdlib.h` dependencies

* `mu_malloc` - equivalent to `malloc`.

* `mu_free` - equivalent to `free`.

* `mu_realloc` - equivalent to `realloc`.

The original time the context was created.

The non-overwritable time.
