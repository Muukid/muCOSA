

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

# Known bugs and limitations

This section covers all of the known bugs and limitations with muCOSA.

## Limited support for most stuff

This version of muCOSA is intended to be very basic, meaning that it only supports Windows and OpenGL, and is not thoroughly tested on other devices. This, if not abandoned, will change in the future, as more support is added, but for now, this library's reach will be fairly limited.


# Other library dependencies

muCOSA has a dependency on:

* [muUtility v2.0.1](https://github.com/Muukid/muUtility/releases/tag/v2.0.1).

> Note that mu libraries store their dependencies within their files, so you don't need to import these dependencies yourself; this section is purely to give more information about the contents that this file defines. The libraries listed may also have other dependencies that they also include that aren't explicitly listed here.

# Window systems

The type `muWindowSystem` (typedef for `uint8_m`) is used to define all of the currently supported window systems. It has the following defined values:

* `MU_WINDOW_NULL` - Unknown/Null window system; real value 0. This value can also act as an "auto" window system, such as when creating a muCOSA context with it, automatically picking the best currently available window system.

* `MU_WINDOW_WIN32` - Win32; real value 1.

## Window system names

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

* `char* name` - the name of the window.

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

# Result

The type `muCOSAResult` (typedef for `uint16_m`) is used to represent how a task in muCOSA went. It has the following defined values:

* `MUCOSA_SUCCESS` - the task succeeded; real value 0.

* `MUCOSA_FAILED_NULL_WINDOW_SYSTEM` - rather an invalid window system value was given by the user, the window system value given by the user was unsupported, or no supported window system could be found.

* `MUCOSA_FAILED_MALLOC` - a call to malloc failed, meaning that there is insufficient memory available to perform the task.

* `MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR` - a conversion from a UTF-8 string to a wide character string failed, rather due to the conversion itself failing or the allocation of memory required for the conversion; this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS` - a call to `RegisterClassExW` failed, meaning that the window class needed to create the window could not be created; this is exclusive to Win32.

* `MUCOSA_WIN32_FAILED_CREATE_WINDOW` - a call to `CreateWindowExW` failed, meaning that the window could not be created; this is exclusive to Win32.

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
