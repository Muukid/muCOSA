

# muCOSA v2.0.1

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

muCOSA does not directly support thread safety, and must be implemented by the user themselves. Thread safety in muCOSA can be generally achieved by locking each object within a muCOSA context (ie `muWindow` for example), making sure that only one thread is interacting with the given object. There are a few known exceptions to this to achieve total thread safety, which are detailed below, but multi-threading with muCOSA is not thoroughly tested.

## Context creation

If `MU_SUPPORT_OPENGL` is defined, two contexts cannot be created at the same time. This is technically a limitation, as it has to do with the generation of unique class names for a dummy WGL-loading window.

## Window updating

Due to limitations with the handling of messages on Win32, no more than one window can be updated safely at any given time across threads.

## Window resizing/moving

Due to the way that Win32 handles messages when the window is being resized or moved, a call to `muCOSA_window_update` will hang for the entire duration of the window being dragged/moved. Handling implemented by users of muCOSA should expect this, and handle vital functionality that needs to be executed over this time on a separate thread.

# Known bugs and limitations

This section covers all of the known bugs and limitations with muCOSA.

## Limited support for most stuff

This version of muCOSA is intended to be very basic, meaning that it only supports Windows and OpenGL, and is not thoroughly tested on other devices. Additionally, many features that might be needed on certain programs are absent. This, if not abandoned, will change in the future, as more support is added, but for now, this library's reach will be fairly limited.

## Minimal overhead attribute management

Currently, muCOSA gets/sets attributes using a single function that requires at least one get/set call for every attribute being modified. Theoretically, more overhead could be abolished by allowing to get/set multiple attributes in one function call, perhaps using the `muWindowInfo` struct and a flag system. This has not been outruled as an option, and muCOSA may stand to gain via this being implemented at some point.

## Unique class name generation

Currently, a unique class name for every window is generated by using the manually-allocated pointer for each window, which, although making it highly likely that a unique name will be generated each time, doesn't make it entirely likely. More optimally, a system of generating valid unique names should be implemented, likely using an atomic counter and manual allocation per name, but this has yet to be implemented.

## Inefficient window memory allocation on Win32

Due to limitations with how a window is identified via its handle in the window procedure functions, the implementation for Win32 has a memory buffer that contains pointers to each window. The logic for this uses filling in empty slots to ensure that a fair amount of memory is allocated even in the event of a large amount of windows being destroyed and created. However, the logic of it currently does not decrease the amount of memory allocated for the window pointers, meaning that a peak in the amount of windows created (across all muCOSA contexts) will peak the memory usage for the buffer, and will not decrease until all muCOSA contexts are destroyed.

This is generally okay, both because the maximum amount of memory allocated for this buffer is the size of a pointer multiplied by the closest power of 2 greater than the amount of windows. So, for example, if there were a maximum of 2718 windows reached at some point during the program's lifespan, and the device is using 64 bits per pointer, the memory usage by this buffer will peak at `64*4096`, or 262144 bytes, and will not decrease until all muCOSA contexts are destroyed. However, it is not only unlikely that a program will ever use this amount of windows, but it is even more unlikely that Windows itself will be able to run properly after this amount of windows have been created.

## Testing with multiple pixel formats

Uncommon pixel formats (such as no-alpha pixel formats) are not tested thoroughly in muCOSA, and in fields where pixel format information is meant to be specified, the information may not be formatted correctly by muCOSA, leading to a bad result being returned. The documentation on what makes "valid" pixel format attributes for OpenGL ranges from confusing to non-existent, so this area can definitely be improved upon later.


# Other library dependencies

muCOSA has a dependency on:

* [muUtility v2.0.1](https://github.com/Muukid/muUtility/releases/tag/v2.0.1).

> Note that mu libraries store their dependencies within their files, so you don't need to import these dependencies yourself; this section is purely to give more information about the contents that this file defines. The libraries listed may also have other dependencies that they also include that aren't explicitly listed here.

# Version

The macros `MUCOSA_VERSION_MAJOR`, `MUCOSA_VERSION_MINOR`, and `MUCOSA_VERSION_PATCH` are defined to match its respective release version, following the formatting of `MAJOR.MINOR.PATCH`.

# Window systems

The type `muWindowSystem` (typedef for `uint8_m`) is used to define all of the currently supported window systems. It has the following defined values:

* `MU_WINDOW_NULL` - Unknown/Null window system; real value 0. This value can also act as an "auto" window system, such as when creating a muCOSA context with it, automatically picking the best currently available window system.

* `MU_WINDOW_WIN32` - Win32; real value 1.

> Note that although on most operating systems, only one window system can exist (such as macOS or Windows), some operating systems can have more than one window system, such as Linux with X11 or Wayland. Just in case, muCOSA allows more than one window system to be defined at once in its API, tying each muCOSA context to a particular window system, theoretically allowing for multiple muCOSA contexts to exist at once with different window systems in one program.

## Window system names

The name function `mu_window_system_get_name` returns a `const char*` representation of the given window sytem (for example, `MU_WINDOW_NULL` returns "MU_WINDOW_NULL"), defined below: 

```c
MUDEF const char* mu_window_system_get_name(muWindowSystem system);
```


It will return "MU_UNKNOWN" in the case that `system` is an invalid window system value.

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

* `uint32_m min_width` - the minimum width of the window's surface, in pixels; a value of 0 implies no minimum.

* `uint32_m min_height` - the minimum height of the window's surface, in pixels; a value of 0 implies no minimum.

* `uint32_m max_width` - the maximum width of the window's surface, in pixels; a value of 0 implies no maximum.

* `uint32_m max_height` - the maximum height of the window's surface, in pixels; a value of 0 implies no maximum.

* `int32_m x` - the x-coordinate of the top-leftest pixel in the window's surface relative to the entire window space of the window system.

* `int32_m y` - the y-coordinate of the top-leftest pixel in the window's surface relative to the entire window space of the window system.

* `muPixelFormat* pixel_format` - the pixel format for the window's surface. If the value of this member is equal to 0, no pixel format is specified, and a default compatible one is chosen. If the pixel format is specified, muCOSA attempts to choose it, and if unsupported, muCOSA will throw a non-fatal error and default on a compatible pixel format.

* `muWindowCallbacks* callbacks` - the [callback functions](#window-callbacks) for various attributes of the window. If this member is equal to 0, no callbacks are specified. If this member is not equal to 0, it should be a valid pointer to a `muWindowCallbacks` struct specifying callbacks for the window.

> Due to restrictions on certain operating systems, the minimum width that will work on all operating systems is 120 pixels, and the minimum height that will surely work is 1 pixel. Additionally, negative coordiantes may not function properly for a given window in regards to setting them to that value, and should not be relied upon for functionality.

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

The "main loop" of a window consists of two functions that are called each "frame": `muCOSA_window_get_closed` and `muCOSA_window_update`. A main loop with these functions generally looks like this:

```c
// Initialization / Window creation here...

while (!mu_window_get_closed(window)) {

// ... (This is where the frame-by-frame logic would go) ...

mu_window_update(window);
}

// Termination here...
```

OpenGL contexts slightly change this; an OpenGL context renders directly to the window's surface, meaning that swapping the buffers of a window needs to occur each frame with a valid OpenGL context binded. For example, if `window` had an OpenGL context created from it named `gl`, a main loop for it would look like this:

```c
// Initialization / Window / OpenGL context creation here...

// Bind OpenGL context (necessary for mu_gl_swap_buffers to work)
mu_gl_bind(gl);

while (!mu_window_get_closed(window)) {

// ... (This is where the frame-by-frame logic would go) ...

mu_gl_swap_buffers(window);
mu_window_update(window);
}

```

More information is provided in the [OpenGL context](#opengl-context) section.

### Get closed

The function `muCOSA_window_get_closed` returns whether or not a given window has been closed, defined below: 

```c
MUDEF muBool muCOSA_window_get_closed(muCOSAContext* context, muCOSAResult* result, muWindow win);
```


Once this function returns `MU_FALSE`, it is no longer usable in all circumstances other than destroying it with `muCOSA_window_destroy`; a window cannot be revived once closed.

> The macro `mu_window_get_closed` is the non-result-checking equivalent, and the macro `mu_window_get_closed_` is the result-checking equivalent.

### Close window

The function `muCOSA_window_close` closes a given window, defined below: 

```c
MUDEF void muCOSA_window_close(muCOSAContext* context, muWindow win);
```


This function cannot if given a valid unclosed window and a valid context corresponding to the window, and thus, has no result parameter.

> The macro `mu_window_close` is the non-result-checking equivalent.

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

* `MU_WINDOW_KEYSTATE_MAP` - the [keystate keymap](#keystate-keymap), represented by a pointer to an array of booleans (type `muBool`) representing the state of all keyboard states (such as caps lock, for example). This can be "get" but not "set".

* `MU_WINDOW_MOUSE_MAP` - the [mouse keymap](#mouse-keymap), represented by a pointer to an array of booleans (type `muBool`) representing the state of each readable mouse key. This can be "get", but not "set".

* `MU_WINDOW_SCROLL_LEVEL` - the scroll level of the cursor associated with the window, represented by a single `int32_m` value representing how far it is scrolled up (positive) or down (negative). This can be "get" and "set".

> One full scroll up/down on a mouse wheel is worth 120 units.

* `MU_WINDOW_CURSOR` - the x- and y-coordinates of the visual cursor relative to the position of the window's surface, represented by an array of two `int32_m`s, where the first element is the x-coordinate, and the second element is the y-coordinate. This can be "get" and "set".

* `MU_WINDOW_CURSOR_STYLE` - the [style of the cursor](#cursor-style), represented by a single value `muCursorStyle`. This can be "get" and "set".

* `MU_WINDOW_..._CALLBACK` - the callback attributes. These all cannot be "get", but can be "set".

   * `MU_WINDOW_DIMENSIONS_CALLBACK` - the dimensions callback.

   * `MU_WINDOW_POSITION_CALLBACK` - the position callback.

   * `MU_WINDOW_KEYBOARD_CALLBACK` - the keyboard callback.

   * `MU_WINDOW_KEYSTATE_CALLBACK` - the keystate callback.

   * `MU_WINDOW_MOUSE_KEY_CALLBACK` - the mouse key callback.

   * `MU_WINDOW_CURSOR_CALLBACK` - the cursor callback.

   * `MU_WINDOW_SCROLL_CALLBACK` - the scroll callback.

   > When callbacks are being set via `muCOSA_window_set`, note that they are pointers *to* the function pointers; function "`fun`" would be set via `muCOSA_window_set(..., &fun)`. For more information about the callbacks, see the (callbacks section)(#window-callbacks). The types listed in the `muWindowCallbacks` struct match the types expected for the callback window attributes.

A value is "get" if calling `muCOSA_window_get` with it is valid, and a value is "set" if calling `muCOSA_window_set` with it is valid.

### Names

The name function for `muWindowAttrib` is `mu_window_attrib_get_name`, defined below: 

```c
MUDEF const char* mu_window_attrib_get_name(muWindowAttrib attrib);
```


> This function returns "MU_UNKNOWN" if the value of `attrib` is unrecognized.

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

## Window callbacks

A window callback is a function that is called whenever the window registers that a certain attribute has changed. Every callback function is called while the window is being updated via the function `muCOSA_window_update`.

Window callbacks are specified in the struct `muWindowCallbacks`, which has the following members:

* `void (*dimensions)` - the dimensions callback, called every time that the window's dimensions are modified, defined below: 

```c
void (*dimensions)(muWindow win, uint32_m width, uint32_m height);
```


* `void (*position)` - the position callback, called every time that the window's position is modified, defined below: 

```c
void (*position)(muWindow win, int32_m x, int32_m y);
```


* `void (*keyboard)` - the keyboard callback, called every time that the status of a keyboard key on the [keyboard keymap](#keyboard-keymap) changes, defined below: 

```c
void (*keyboard)(muWindow win, muKeyboardKey key, muBool status);
```


* `void (*keystate)` - the keystate callback, called every time that the status of a keystate on the [keystate keymap](#keystate-keymap) changes, defined below: 

```c
void (*keystate)(muWindow win, muKeyboardState state, muBool status);
```


* `void (*mouse_key)` - the mouse key callback, called every time that the status of a mouse key on the [mouse keymap](#mouse-keymap) changes, defined below: 

```c
void (*mouse_key)(muWindow win, muMouseKey key, muBool status);
```


* `void (*cursor)` - the cursor position callback, called every time that the cursor position changes, defined below: 

```c
void (*cursor)(muWindow win, int32_m x, int32_m y);
```


* `void (*scroll)` - the scroll callback, called every time that the scroll level changes, defined below: 

```c
void (*scroll)(muWindow win, int32_m add);
```


Setting the value of any member to 0 dictates no callback function. Changes to an attribute made via the program (such as a `mu_window_set` call) are not guaranteed to generate corresponding callbacks.

Initial callbacks, AKA callbacks involving the window's attributes being set upon creation, are not guaranteed to be called. Additionally, duplicate callbacks (ie callbacks issuing the same value, such a keyboard callback issuing a key status that already matched the previous key status, AKA a key being pressed/released twice) can occur; in fact, duplicate key presses are used to represent a key being held down for a long time, and the rate of repetition varies between window systems and their settings.

Callbacks involving queryable attributes of a window are called after their attribute has been updated. For example, if a keyboard callback is triggered, the keyboard keymap for the corresponding window has already been updated for the key in question.

Users should also avoid possible callback loops, such as a position callback that changes the position, which can theoretically trigger an infinite loop and cause a nasty crash.

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

#### Keyboard names

The name function `mu_keyboard_key_get_name` returns a `const char*` representation of the given keyboard key (for example, `MU_KEYBOARD_W` returns "MU_KEYBOARD_W"), defined below: 

```c
MUDEF const char* mu_keyboard_key_get_name(muKeyboardKey key);
```


It will return "MU_UNKNOWN" in the case that `key` is an invalid keyboard key value.

The name function `mu_keyboard_key_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_KEYBOARD_W` returns "W"), defined below: 

```c
MUDEF const char* mu_keyboard_key_get_nice_name(muKeyboardKey key);
```


It will return "Unknown" in the case that `key` is an invalid keyboard key value.

> These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.

### Keystate keymap

The keystate keymap represents the state of certain modifiers on the keyboard readable by muCOSA, using type `muKeyboardState` (typedef for `uint8_m`) as index. The length of the keymap is `MU_KEYSTATE_LENGTH`. It has the following indexes:

* `MU_KEYSTATE_UNKNOWN` - unknown keystate.

* `MU_KEYSTATE_CAPS_LOCK` - the [caps lock](https://en.wikipedia.org/wiki/Caps_Lock) state.

* `MU_KEYSTATE_SCROLL_LOCK` - the [scroll lock](https://en.wikipedia.org/wiki/Scroll_Lock) state.

* `MU_KEYSTATE_NUM_LOCK` - the [num lock](https://en.wikipedia.org/wiki/Num_Lock) state.

Once the pointer to the keystate keymap array has been retrieved via `muCOSA_window_get`, these values can be used as indexes to see the status of any mouse key, in which `MU_TRUE` indicates that the keystate is active, and `MU_FALSE` indicates that the keystate is inactive.

#### Keystate names

The name function `mu_keystate_get_name` returns a `const char*` representation of the given keystate (for example, `MU_KEYSTATE_CAPS_LOCK` returns "MU_KEYSTATE_CAPS_LOCK"), defined below: 

```c
MUDEF const char* mu_keystate_get_name(muKeyboardState state);
```


It will return "MU_UNKNOWN" in the case that `state` is an invalid keystate value.

The name function `mu_keystate_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_KEYSTATE_CAPS_LOCK` returns "Caps Lock"), defined below: 

```c
MUDEF const char* mu_keystate_get_nice_name(muKeyboardState state);
```


It will return "Unknown" in the case that `state` is an invalid keystate value.

> These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.

### Mouse keymap

The mouse keymap represents the keys on a common computer mouse readable by muCOSA, using type `muMouseKey` (typedef for `uint16_m`) as index. The length of the keymap is `MU_MOUSE_LENGTH`. It has the following indexes:

* `MU_MOUSE_UNKNOWN` - unknown mouse key.

* `MU_MOUSE_LEFT` - the left mouse key.

* `MU_MOUSE_RIGHT` - the right mouse key.

* `MU_MOUSE_MIDDLE` - the middle mouse key; this indicates whether or not the middle mouse key (usually the scroll wheel) is being clicked, not scrolled.

Once the pointer to the mouse keymap array has been retrieved via `muCOSA_window_get`, these values can be used as indexes to see the status of any mouse key, in which `MU_TRUE` indicates that the key is being pressed down, and `MU_FALSE` indicates that the key is released.

#### Mouse key names

The name function `mu_mouse_key_get_name` returns a `const char*` representation of the given mouse key (for example, `MU_MOUSE_LEFT` returns "MU_MOUSE_LEFT"), defined below: 

```c
MUDEF const char* mu_mouse_key_get_name(muMouseKey key);
```


It will return "MU_UNKNOWN" in the case that `key` is an invalid mouse key value.

The name function `mu_mouse_key_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_MOUSE_LEFT` returns "Left"), defined below: 

```c
MUDEF const char* mu_mouse_key_get_nice_name(muMouseKey key);
```


It will return "Unknown" in the case that `key` is an invalid mouse key value.

> These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.

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

* `MU_CURSOR_SIZE_ALL` - the move/resize-all cursor style, pointing outwards in all directions; equivalent to `IDC_SIZEALL` in Win32.

* `MU_CURSOR_NO` - the disallowing/error/not-allowed cursor style; equivalent to `IDC_NO` in WIn32.

### Cursor style names

The name function `mu_cursor_style_get_name` returns a `const char*` representation of the given cursor style (for example, `MU_CURSOR_HAND` returns "MU_CURSOR_HAND"), defined below: 

```c
MUDEF const char* mu_cursor_style_get_name(muCursorStyle style);
```


It will return "MU_UNKNOWN" in the case that `style` is an invalid cursor style value.

The name function `mu_cursor_style_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_CURSOR_HAND` returns "Hand"), defined below: 

```c
MUDEF const char* mu_cursor_style_get_nice_name(muCursorStyle style);
```


It will return "Unknown" in the case that `style` is an invalid cursor style value.

> These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.

## Text input

muCOSA is able to get text input from a window, which uses a callback to give Unicode character codepoints based on what the user is typing. This system is recommended over trying to emulate text input via keyboard callbacks, as it automatically handles variables such as key states and input method managers.

muCOSA needs to know where the text input is being taken visually on the window's surface; for this, the "text cursor" exists, whose position dictates where features like virtual keyboards render their preview characters, and is relative to the top-leftest pixel on the window's surface, and whose coordinates should always be within the window's surface (AKA less than the window's dimensions).

### Get text input focus

The function `muCOSA_window_get_text_input` gets text input from a window, defined below: 

```c
MUDEF void muCOSA_window_get_text_input(muCOSAContext* context, muCOSAResult* result, muWindow win, uint32_m text_cursor_x, uint32_m text_cursor_y, void (*callback)(muWindow window, uint8_m* data));
```


The callback will be called with a UTF-8-encoded character representing what character has been input by the user. This callback will only be called while the given window is updating (AKA while `muCOSA_window_update` is being called on it), just like all other window callbacks.

Once text input is successfully retrieved for a window, it should be manually let go of by the user at some point before the window is destroyed and before this function is called on another window. Text input stops being sent to the window while it's unfocused, but text input focus is still retained, and does not need to be called again one the window is refocused.

> The macro `mu_window_get_text_input` is the non-result-checking equivalent, and the macro `mu_window_get_text_input_focus_` is the result-checking equivalent.

### Let go of text input focus

The function `muCOSA_window_let_text_input` lets go of text input focus for the given window that has text input focus, defined below: 

```c
MUDEF void muCOSA_window_let_text_input(muCOSAContext* context, muWindow win);
```


The given window must have text input focus before this function is called. If the parameters are valid, this function cannot fail, and thus, has no `result` parameter.

> The macro `mu_window_destroy` is the non-result-checking equivalent.

### Update text cursor position

The function `muCOSA_window_update_text_cursor` updates the position of a text cursor for a window that has text input focus, defined below: 

```c
MUDEF void muCOSA_window_update_text_cursor(muCOSAContext* context, muCOSAResult* result, muWindow win, uint32_m x, uint32_m y);
```


The given window must have text input focus before this function is called.

> The macro `mu_window_update_text_cursor` is the non-result-checking equivalent, and the macro `mu_window_update_text_cursor_` is the result-checking equivalent.

## Pixel format

A window's pixel format is used to define what data will be used when representing the window's surface. Its respective type is `muPixelFormat`, and has the following members:

* `uint16_m red_bits` - the amount of bits used for the red channel.

* `uint16_m green_bits` - the amount of bits used for the green channel.

* `uint16_m blue_bits` - the amount of bits used for the blue channel.

* `uint16_m alpha_bits` - the amount of bits used for the alpha channel.

* `uint16_m depth_bits` - the amount of bits used for the depth channel.

* `uint16_m stencil_bits` - the amount of bits used for the stencil channel.

* `uint8_m samples` - the amount of samples used for each pixel. A value of 1 means no multi-sampling is performed. Any value other than 1 indicates multi-sampling, and must be a power of 2.

0 bits means that the data does not include it; for example, if `depth_bits` is equal to 0, then no depth data is defined in the pixel format.

## Graphics APIs

muCOSA supports windows creating graphics APIs associated with them (respective type `muGraphicsAPI`, typedef for `uint16_m`). These are the currently defined and supported graphics APIs:

* `MU_NULL_GRAPHICS_API` - unknown/no graphics API. This value can be given when the user does not want a window to be associated with a particular graphics API, and will attempt to load no graphics API.

* `MU_OPENGL_1_0` - [OpenGL v1.0](https://registry.khronos.org/OpenGL/specs/gl/glspec10.pdf).

* `MU_OPENGL_1_1` - [OpenGL v1.1](https://registry.khronos.org/OpenGL/specs/gl/glspec11.pdf).

* `MU_OPENGL_1_2` - [OpenGL v1.2](https://registry.khronos.org/OpenGL/specs/gl/glspec121.pdf).

* `MU_OPENGL_1_2_1` - [OpenGL v1.2.1](https://registry.khronos.org/OpenGL/specs/gl/glspec121.pdf).

* `MU_OPENGL_1_3` - [OpenGL v1.3](https://registry.khronos.org/OpenGL/specs/gl/glspec13.pdf).

* `MU_OPENGL_1_4` - [OpenGL v1.4](https://registry.khronos.org/OpenGL/specs/gl/glspec14.pdf).

* `MU_OPENGL_1_5` - [OpenGL v1.5](https://registry.khronos.org/OpenGL/specs/gl/glspec15.pdf).

* `MU_OPENGL_2_0` - [OpenGL v2.0](https://registry.khronos.org/OpenGL/specs/gl/glspec20.pdf).

* `MU_OPENGL_2_1` - [OpenGL v2.1](https://registry.khronos.org/OpenGL/specs/gl/glspec21.pdf).

* `MU_OPENGL_3_0` - [OpenGL v3.0](https://registry.khronos.org/OpenGL/specs/gl/glspec30.pdf).

* `MU_OPENGL_3_1` - [OpenGL v3.1](https://registry.khronos.org/OpenGL/specs/gl/glspec31.pdf).

* `MU_OPENGL_3_2_CORE` - [OpenGL v3.2 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec32.core.pdf).

* `MU_OPENGL_3_2_COMPATIBILITY` - [OpenGL v3.2 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec32.compatibility.pdf).

* `MU_OPENGL_3_3_CORE` - [OpenGL v3.3 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec33.core.pdf).

* `MU_OPENGL_3_3_COMPATIBILITY` - [OpenGL v3.3 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec33.compatibility.pdf).

* `MU_OPENGL_4_0_CORE` - [OpenGL v4.0 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec40.core.pdf).

* `MU_OPENGL_4_0_COMPATIBILITY` - [OpenGL v4.0 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec40.compatibility.pdf).

* `MU_OPENGL_4_1_CORE` - [OpenGL v4.1 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec41.core.pdf).

* `MU_OPENGL_4_1_COMPATIBILITY` - [OpenGL v4.1 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec41.compatibility.pdf).

* `MU_OPENGL_4_2_CORE` - [OpenGL v4.2 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec42.core.pdf).

* `MU_OPENGL_4_2_COMPATIBILITY` - [OpenGL v4.2 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec42.compatibility.pdf).

* `MU_OPENGL_4_3_CORE` - [OpenGL v4.3 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec43.core.pdf).

* `MU_OPENGL_4_3_COMPATIBILITY` - [OpenGL v4.3 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec43.compatibility.pdf).

* `MU_OPENGL_4_4_CORE` - [OpenGL v4.4 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec44.core.pdf).

* `MU_OPENGL_4_4_COMPATIBILITY` - [OpenGL v4.4 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec44.compatibility.pdf).

* `MU_OPENGL_4_5_CORE` - [OpenGL v4.5 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec45.core.pdf).

* `MU_OPENGL_4_5_COMPATIBILITY` - [OpenGL v4.5 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec45.compatibility.pdf).

* `MU_OPENGL_4_6_CORE` - [OpenGL v4.6 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec46.core.pdf).

* `MU_OPENGL_4_6_COMPATIBILITY` - [OpenGL v4.6 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec46.compatibility.pdf).

Note that OpenGL will only work if `MU_SUPPORT_OPENGL` is defined before `muCOSA.h` is first included.

### Graphics API macro customization

Files necessary to define OpenGL features (such as `gl/gh.`/`gl/glu.h` on Win32) are automatically included if `MU_SUPPORT_OPENGL` is defined; the inclusion of these files can be manually turned off (in case they have already been included) via defining `MUCOSA_NO_INCLUDE_OPENGL`.

### Graphics API names

The name function `mu_graphics_api_get_name` returns a `const char*` representation of the given graphics API (for example, `MU_OPENGL_3_3_CORE` returns "MU_OPENGL_3_3_CORE"), defined below: 

```c
MUDEF const char* mu_graphics_api_get_name(muGraphicsAPI api);
```


It will return "MU_UNKNOWN" in the case that `api` is an invalid graphics API value.

The name function `mu_graphics_api_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_OPENGL_3_3_CORE` returns "OpenGL 3.3 (Core Profile)"), defined below: 

```c
MUDEF const char* mu_graphics_api_get_nice_name(muGraphicsAPI api);
```


It will return "Unknown" in the case that `api` is an invalid graphics API value.

> These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.

## OpenGL context

A window can be used to create a valid OpenGL context that is linked to the window (respective type `muGLContext`; macro for `void*`). Any operations to an OpenGL context are regarding to the currently bound OpenGL context on the current thread (besides the binding function itself), so some OpenGL context-related functions do not take the actual OpenGL context as a parameter.

### Create / Destroy OpenGL context

The function `muCOSA_gl_context_create` creates an OpenGL context, defined below: 

```c
MUDEF muGLContext muCOSA_gl_context_create(muCOSAContext* context, muCOSAResult* result, muWindow win, muGraphicsAPI api);
```


Every successfully created OpenGL context must be destroyed before the window that created it is destroyed. This function does not modify any previously binded OpenGL context.

> The macro `mu_gl_context_create` is the non-result-checking equivalent, and the macro `mu_gl_context_create_` is the result-checking equivalent.

The function `muCOSA_gl_context_destroy` destroys an OpenGL context, defined below: 

```c
MUDEF muGLContext muCOSA_gl_context_destroy(muCOSAContext* context, muWindow win, muGLContext gl_context);
```


The destruction function cannot fail if given a proper context and window, and thus, there is no `result` parameter.

> The macro `mu_gl_context_destroy` is the non-result-checking equivalent.

### Bind OpenGL context

The function `muCOSA_gl_bind` binds an OpenGL context to the current thread, defined below: 

```c
MUDEF void muCOSA_gl_bind(muCOSAContext* context, muCOSAResult* result, muWindow win, muGLContext gl_context);
```


If `gl_context` is not 0, it is expected to be a valid OpenGL context. If `gl_context` is 0, this function unbinds any OpenGL context currently binded on the calling thread; doing this without any OpenGL context previously binded is undefined behavior.

This function is, unfortunately, one of the few functions whose defined functionality can differ based on the currently running window system. If this function gives a fatal return value on Win32, the previously binded OpenGL context is unbinded; on X11, the previously binded OpenGL context stays binded.

> The macro `mu_gl_bind` is the non-result-checking equivalent, and the macro `mu_gl_bind_` is the result-checking equivalent.

### Swap buffers for OpenGL

The function `muCOSA_gl_swap_buffers` swaps the buffers of a window associated with at least one OpenGL context, defined below: 

```c
MUDEF void muCOSA_gl_swap_buffers(muCOSAContext* context, muCOSAResult* result, muWindow win);
```


Because this function acts purely on the window, and not directly with any associated OpenGL context, this should only be called if an OpenGL context exists that was created from the given window, and should be called every frame before `muCOSA_window_update` for as long as there is an OpenGL context associated with the window.

> The macro `mu_gl_swap_buffers` is the non-result-checking equivalent, and the macro `mu_gl_swap_buffers_` is the result-checking equivalent.

### Get procedure address

The function `muCOSA_gl_get_proc_address` returns the address of a requested OpenGL function by name, defined below: 

```c
MUDEF void* muCOSA_gl_get_proc_address(muCOSAContext* context, const char* name);
```


This function must be called with a valid OpenGL context binded. On failure, this function returns 0, and on success, returns a non-zero value. A valid address given by this function can only be guaranteed to be valid in relation to the currently binded OpenGL context.

> The macro `mu_gl_get_proc_address` is the non-result-checking equivalent.

### Swap interval

The function `muCOSA_gl_swap_interval` acts as a call to `wglSwapIntervalEXT`, defined below: 

```c
MUDEF muBool muCOSA_gl_swap_interval(muCOSAContext* context, muCOSAResult* result, int interval);
```


On Win32, this function returns the return value of `wglSwapIntervalEXT` if `result` is set to a non-fatal value, and 0 if otherwise.

> The macro `mu_gl_swap_interval` is the non-result-checking equivalent, and the macro `mu_gl_swap_interval_` is the result-checking equivalent.

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

# Clipboard

muCOSA offers functionality for getting and setting the current text clipboard.

## Get clipboard

The function `muCOSA_clipboard_get` retrieves the current text clipboard, defined below: 

```c
MUDEF uint8_m* muCOSA_clipboard_get(muCOSAContext* context, muCOSAResult* result);
```


On success, this function rather returns 0 (implying that there is no text clipboard set), or a pointer to data manually allocated by muCOSA; in the latter circumstance, it must be freed by the user manually when they are finished using the data.

On failure, this function returns 0, and `result` is set to the failure value.

> The macro `mu_clipboard_get` is the non-result-checking equivalent, and the macro `mu_clipboard_get_` is the result-checking equivalent.

## Set clipboard

The function `muCOSA_clipboard_set` sets the current text clipboard, defined below: 

```c
MUDEF void muCOSA_clipboard_set(muCOSAContext* context, muCOSAResult* result, uint8_m* data, size_m datalen);
```


On success, this function sets the current text clipboard to the given UTF-8 text data, of length `datalen` (including null-terminating character).

> The macro `mu_clipboard_set` is the non-result-checking equivalent, and the macro `mu_clipboard_set_` is the result-checking equivalent.

# Result

The type `muCOSAResult` (typedef for `uint16_m`) is used to represent how a task in muCOSA went. It has the following defined values:

* `MUCOSA_SUCCESS` - the task succeeded; real value 0.

* `MUCOSA_FAILED_NULL_WINDOW_SYSTEM` - rather an invalid window system value was given by the user, the window system value given by the user was unsupported, or no supported window system could be found.

* `MUCOSA_FAILED_MALLOC` - a call to `mu_malloc` failed, meaning that there is insufficient memory available to perform the task.

* `MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB` - an invalid `muWindowAttrib` value was given by the user.

* `MUCOSA_FAILED_REALLOC` - a call to `mu_realloc` failed, meaning that there is insufficient memory available to perform the task.

* `MUCOSA_FAILED_UNKNOWN_GRAPHICS_API` - an invalid `muGraphicsAPI` value was given by the user.

* `MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API` - a function relating to a graphics API was called despite the fact that support for the graphics API was not enabled.

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

* `MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS` - a call to `RegisterClassA` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.

* `MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW` - a call to `CreateWindowExA` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.

* `MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT` - a call to `GetPixelFormat` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.

* `MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT` - a call to `DescribePixelFormat` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.

* `MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT` - a call to `SetPixelFormat` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.

* `MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT` - a call to `wglCreateContext` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.

* `MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT` - a call to `wglMakeCurrent` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.

* `MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS` - the function `wglCreateContextAttribsARB` could not be found, which is necessary to creating OpenGL functions; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.

* `MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT` - the function `wglChoosePixelFormatARB` could not be found, which is necessary to creating OpenGL functions; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.

* `MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT` - the function `wglChoosePixelFormatARB` returned a failure value when creating an OpenGL context; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but the pixel format will likely not be what the user requested.

* `MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT` - the function `GetPixelFormat` returned a failure value when creating an OpenGL context; this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT` - the function `DescribePixelFormat` returned a failure value when creating an OpenGL context; this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT` - the function `SetPixelFormat` returned a failure value when creating an OpenGL context; this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT` - the function `wglCreateContextAttribsARB` returned a failure value when creating an OpenGL context; this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT` - the function `wglMakeCurrent` returned a failure value when binding the OpenGL context; this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS` - the function `SwapBuffers` returned a failure value when swapping the buffers; this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION` - the corresponding OpenGL function could not be located; this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_ASSOCIATE_IMM` - the function `ImmAssociateContextEx` returned a failure value when getting text input focus; this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION` - the function `ImmSetCompositionWindow` returned a failure value when attempting to move it to the current text cursor position; this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD` - the function `OpenClipboard` returned a failure value when attempting to retrieve the clipboard data (`muCOSA_clipboard_get`) or overwrite it (`muCOSA_clipboard_set`); this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA` - the function `GlobalLock` returned a failure value when attempting to retrieve a pointer to the clipboard data when attempting to retrieve the clipboard data (`muCOSA_clipboard_get`); this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT` - the conversion between UTF-16 wide-character data and UTF-8 `uint8_m*` data (rather converting from UTF-8 to UTF-16 when setting the clipboard data (`muCOSA_clipboard_set`), or converting from UTF-16 to UTF-8 when getting the clipboard data (`muCOSA_clipboard_get`)) failed, rather due to allocation or to the data itself being invalid; this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA` - rather the function `GlobalAlloc` or `GlobalLock` failed when attempting to allocate and get a pointer to the global data for the clipboard when setting the clipboard (`muCOSA_clipboard_set`); this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA` - the function `SetClipboardData` failed when attempting to set the clipboard data; this is exclusive to Win32.

All non-success values (unless explicitly stated otherwise) mean that the function fully failed, AKA it was "fatal", and the library continues as if the function had never been called; so, for example, if something was supposed to be allocated, but the function fatally failed, nothing was allocated.

There are non-fatal failure values, which mean that the function still executed, but not fully to the extent that the user would expect from the function. The function `muCOSA_result_is_fatal` returns whether or not a given result function value is fatal, defined below: 

```c
MUDEF muBool muCOSA_result_is_fatal(muCOSAResult result);
```


> This function returns `MU_TRUE` if the value of `result` is invalid. It returns `MU_FALSE` if `result` is equal to `MUCOSA_SUCCESS`.

## Result names

The name function `muCOSA_result_get_name` returns a `const char*` representation of the given result value (for example, `MUCOSA_SUCCESS` returns "MUCOSA_SUCCESS"), defined below: 

```c
MUDEF const char* muCOSA_result_get_name(muCOSAResult result);
```


It will return "MU_UNKNOWN" in the case that `result` is an invalid result value.

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
