/*
muCOSA.h - Muukid
Public domain single-file C library for interacting with operating systems with a cross-platform API.
No warranty implied; use at your own risk.

Licensed under MIT License or public domain, whichever you prefer.
More explicit license information at the end of file.

@TODO Audio.
@TODO Non-text clipboards.
@TODO Drag & drop.
@TODO X11 clipboard INCR.
@TODO X_HAVE_UTF8_STRING.
@TODO Copy + paste for text input on X11.
@TODO "Stay awake" function that prevents the computer from auto-sleeping.
@TODO More OS-specific functions that allow you to have finer control over window attributes (like
window styling on windows).
@TODO Make termination threadsafe.
@TODO Avoid the infamous WM_NCLBUTTONDOWN trap.
@TODO Replace all "VK_" macros with defines.
@TODO Remove redundant callback parts of X11/Win32 inner funcs.
@TODO Make sure X11 clears prior window struct in window creation func.
@TODO Make time in X11 threadsafe.
@TODO Figure out how to allow the window to do resizing with the cursor showing as resizing...
maybe add a manual override?
@TODO Fix Win32 not giving correct frame extents no matter what when it's maximized. I LOVE BILL!!!
@TODO State overriding (AKA make it auto-return an error if, for example, you're trying to move a
window while it's maximized).
@TODO Fix Win32 IME flickering.
@TODO Replace duplicate functions with macros, maybe.
@TODO Add processing for multiple string types when retrieving clipboard in X11 (most likely
requires a string library...)
@TODO Test on more Linux distros.

As of right now, muCOSA assumes 2 things:
1. A keyboard and mouse are available, being the primary forms of input.
2. A "window" exists in the traditional desktop OS sense.
If muCOSA were to ever expand to, say, mobile devices, these would need to change. A framework I'm
thinking of right now essentially has 2 types of devices:
1. Raw input device (keyboard, mouse, microphone, controller, MIDI, et cetera).
2. OS input device (keyboard (including virtual keyboard), mouse, mouse cursor, et cetera)
A raw input device is directly plugged into the computer (or is a component of the computer) and
receives input that way, while an OS input device has the OS's abstraction for it (vaguely). It's
hard to describe, but here's an example:
keyboard_get_raw_input(...) directly returns input from a PARTICULAR keyboard, identified by a
device ID (as there can be multiple keyboards). It DIRECTLY returns whether or not a SPECIFIC key
on the keyboard is pressed or not.
keyboard_get_os_input(...) returns a text input from the OS in Unicode codepoint form. On PC's,
this would grab focus of the keyboard input. On mobile devices, this will pop up a little keyboard
menu. Note that this allows for virtual keyboard integration.
There are some devices that exist only as OS input, such as a mouse cursor, as that is a concept
entirely created by the OS. There are some devices that exist only as raw input, such as a
microphone.
The only thing they have in common is that each "type" of its input (ie mouse, keyboard, etc.) has
a part of the API dedicated to querying information about it. The only thing guaranteed in these
parts is that for every raw input device, you can query a list to see how many devices of that type
are available and identify them by name, but for the OS input device, there's only one and it can
be queried if it exists or not (ie, a mouse cursor may not be a concept on the current OS).
Another thing about this theoretical framework is how a "window" is defined and used. As of right
now, a "window" is defined in the traditional desktop OS sense, taking up a portion of the screen,
being moveable, resizable, et cetera. Meanwhile, on a mobile OS, it takes up virtually the whole
screen. It's hard to think of fitting this into the API without drastically changing it. Plus, the
concept of a window being "created" and "destroyed" is now gone, since it's one big window.
Maybe have a separate object called "muDisplay" that works VERY similarly to a window except for
it's not created but retrieved (as there's one global display), and is built to work on either
mobile or desktop. Dunno.
Not doing all of that, right now tho; muCOSA 1.X.X is meant to be pretty simplistic and built
primarily around a traditional desktop OS environment.
*/

#ifndef MUCOSA_H
	#define MUCOSA_H

	// @IGNORE
	/* muUtility v1.1.0 header */
	
		#if !defined(MU_CHECK_VERSION_MISMATCHING) && defined(MUU_H) && \
			(MUU_VERSION_MAJOR != 1 || MUU_VERSION_MINOR != 1 || MUU_VERSION_PATCH != 0)

			#pragma message("[MUCOSA] muUtility's header has already been defined, but version doesn't match the version that this library is built for. This may lead to errors, warnings, or unexpected behavior. Define MU_CHECK_VERSION_MISMATCHING before this to turn off this message.")

		#endif

		#ifndef MUU_H
			#define MUU_H
			
			// @DOCLINE # Secure warnings
			// @DOCLINE mu libraries often use non-secure functions that will trigger warnings on certain compilers. These warnings are, to put it lightly, dumb, so the header section of muUtility defines `_CRT_SECURE_NO_WARNINGS`. However, it is not guaranteed that this definition will actually turn the warnings off, which at that point, they have to be manually turned off by the user.
			#if !defined(MU_SECURE_WARNINGS) && !defined(_CRT_SECURE_NO_WARNINGS)
				#define _CRT_SECURE_NO_WARNINGS
			#endif

			#ifdef __cplusplus
			extern "C" { // }
			#endif

			// @DOCLINE # C standard library dependencies

				// @DOCLINE muUtility has several C standard library dependencies, all of which are overridable by defining them before the inclusion of its header. The following is a list of those dependencies:

				// @DOCLINE ## `stdint.h` dependencies
				#if !defined(int8_m)      || \
					!defined(uint8_m)     || \
					!defined(int16_m)     || \
					!defined(uint16_m)    || \
					!defined(int32_m)     || \
					!defined(uint32_m)    || \
					!defined(int64_m)     || \
					!defined(uint64_m)    || \
					!defined(MU_SIZE_MAX)

					#define __STDC_LIMIT_MACROS
					#define __STDC_CONSTANT_MACROS
					#include <stdint.h>

					// @DOCLINE `int8_m`: equivalent to `int8_t` if `INT8_MAX` is defined, `char` if otherwise
					#ifndef int8_m
						#ifdef INT8_MAX
							#define int8_m int8_t
						#else
							#define int8_m char
						#endif
					#endif

					// @DOCLINE `uint8_m`: equivalent to `uint8_t` if `UINT8_MAX` is defined, `unsigned char` if otherwise
					#ifndef uint8_m
						#ifdef UINT8_MAX
							#define uint8_m uint8_t
						#else
							#define uint8_m unsigned char
						#endif
					#endif

					// @DOCLINE `int16_m`: equivalent to `int16_t` if `INT16_MAX` is defined, `short` if otherwise
					#ifndef int16_m
						#ifdef INT16_MAX
							#define int16_m int16_t
						#else
							#define int16_m short
						#endif
					#endif

					// @DOCLINE `uint16_m`: equivalent to `uint16_t` if `UINT16_MAX` is defined, `unsigned short` if otherwise
					#ifndef uint16_m
						#ifdef UINT16_MAX
							#define uint16_m uint16_t
						#else
							#define uint16_m unsigned short
						#endif
					#endif

					// @DOCLINE `int32_m`: equivalent to `int32_t` if `INT32_MAX` is defined, `long` if otherwise
					#ifndef int32_m
						#ifdef INT32_MAX
							#define int32_m int32_t
						#else
							#define int32_m long
						#endif
					#endif

					// @DOCLINE `uint32_m`: equivalent to `uint32_t` if `UINT32_MAX` is defined, `unsigned long` if otherwise
					#ifndef uint32_m
						#ifdef UINT32_MAX
							#define uint32_m uint32_t
						#else
							#define uint32_m unsigned long
						#endif
					#endif

					// @DOCLINE `int64_m`: equivalent to `int64_t` if `INT64_MAX` is defined, `long long` if otherwise
					#ifndef int64_m
						#ifdef INT64_MAX
							#define int64_m int64_t
						#else
							#define int64_m long long
						#endif
					#endif

					// @DOCLINE `uint64_m`: equivalent to `uint64_t` if `UINT64_MAX` is defined, `unsigned long long` if otherwise
					#ifndef uint64_m
						#ifdef UINT64_MAX
							#define uint64_m uint64_t
						#else
							#define uint64_m unsigned long long
						#endif
					#endif

					// @DOCLINE `MU_SIZE_MAX`: equivalent to `SIZE_MAX`
					#ifndef MU_SIZE_MAX
						#define MU_SIZE_MAX SIZE_MAX
					#endif

				#endif

				// @DOCLINE ## `stddef.h` dependencies
				#if !defined(size_m)

					#include <stddef.h>

					// @DOCLINE `size_m`: equivalent to `size_t`
					#ifndef size_m
						#define size_m size_t
					#endif

				#endif

				// @DOCLINE ## `stdbool.h` dependencies
				#if !defined(muBool)   || \
					!defined(MU_TRUE)  || \
					!defined(MU_FALSE)

					#include <stdbool.h>

					// @DOCLINE `muBool`: equivalent to `bool`
					#ifndef muBool
						#define muBool bool
					#endif

					// @DOCLINE `MU_TRUE`: equivalent to `true`
					#ifndef MU_TRUE
						#define MU_TRUE true
					#endif

					// @DOCLINE `MU_FALSE`: equivalent to `false`
					#ifndef MU_FALSE
						#define MU_FALSE false
					#endif

				#endif

			// @DOCLINE # Macros

				// @DOCLINE ## Version

					// @DOCLINE The macros `MUU_VERSION_MAJOR`, `MUU_VERSION_MINOR`, and `MUU_VERSION_PATCH` are defined to match its respective release version, following the formatting of `MAJOR.MINOR.PATCH`.

					#define MUU_VERSION_MAJOR 1
					#define MUU_VERSION_MINOR 1
					#define MUU_VERSION_PATCH 0

				// @DOCLINE ## `MUDEF`

					// @DOCLINE The `MUDEF` macro is a macro used by virtually all mu libraries, which is generally added before a header-defined variable or function. Its default value is `extern`, but can be changed to `static` by defining `MU_STATIC` before the header section of muUtility is defined. Its value can also be overwritten entirely to anything else by directly defining `MUDEF`.

					#ifndef MUDEF
						#ifdef MU_STATIC
							#define MUDEF static
						#else
							#define MUDEF extern
						#endif
					#endif

				// @DOCLINE ## Zero struct

					// @DOCLINE There are two macros, `MU_ZERO_STRUCT` and `MU_ZERO_STRUCT_CONST`, which are macro functions used to zero-out a struct's contents, with their only parameter being the struct type. The reason this needs to be defined is because the way C and C++ syntax handles an empty struct are different, and need to be adjusted for.

					#ifndef MU_ZERO_STRUCT
						#ifdef __cplusplus
							#define MU_ZERO_STRUCT(s) {}
						#else
							#define MU_ZERO_STRUCT(s) (s){0}
						#endif
					#endif

					#ifndef MU_ZERO_STRUCT_CONST
						#ifdef __cplusplus
							#define MU_ZERO_STRUCT_CONST(s) {}
						#else
							#define MU_ZERO_STRUCT_CONST(s) {0}
						#endif
					#endif

				// @DOCLINE ## Byte

					// @DOCLINE Several macros are defined to interact with the idea of a byte.

					// @DOCLINE The macro `muByte` exists as a macro for `uint8_m`, and represents a byte as a type.

					#ifndef muByte
						#define muByte uint8_m
					#endif

					// @DOCLINE There are also several macro functions used to read and write to bytes. These macros are:

					// @DOCLINE `mu_rle_uint8(b)`: Creates an expression of type `uint8_m` that reads the value stored in the 1-byte-length little-endian byte array `b`.
					#ifndef mu_rle_uint8
						#define mu_rle_uint8(b) ((uint8_m)b[0] << 0)
					#endif
					// @DOCLINE `mu_rbe_uint8(b)`: Creates an expression of type `uint8_m` that reads the value stored in the 1-byte-length big-endian byte array `b`.
					#ifndef mu_rbe_uint8
						#define mu_rbe_uint8(b) ((uint8_m)b[0] << 0)
					#endif

					// @DOCLINE `mu_rle_uint16(b)`: Creates an expression of type `uint16_m` that reads the value stored in the 2-byte-length little-endian byte array `b`.
					#ifndef mu_rle_uint16
						#define mu_rle_uint16(b) ((uint16_m)b[0] << 0 | (uint16_m)b[1] << 8)
					#endif
					// @DOCLINE `mu_rbe_uint16(b)`: Creates an expression of type `uint16_m` that reads the value stored in the 2-byte-length big-endian byte array `b`.
					#ifndef mu_rbe_uint16
						#define mu_rbe_uint16(b) ((uint16_m)b[1] << 0 | (uint16_m)b[0] << 8)
					#endif

					// @DOCLINE `mu_rle_uint32(b)`: Creates an expression of type `uint32_m` that reads the value stored in the 4-byte-length little-endian byte array `b`.
					#ifndef mu_rle_uint32
						#define mu_rle_uint32(b) ((uint32_m)b[0] << 0 | (uint32_m)b[1] << 8 | (uint32_m)b[2] << 16 | (uint32_m)b[3] << 24)
					#endif
					// @DOCLINE `mu_rbe_uint32(b)`: Creates an expression of type `uint32_m` that reads the value stored in the 4-byte-length big-endian byte array `b`.
					#ifndef mu_rbe_uint32
						#define mu_rbe_uint32(b) ((uint32_m)b[3] << 0 | (uint32_m)b[2] << 8 | (uint32_m)b[1] << 16 | (uint32_m)b[0] << 24)
					#endif

					// @DOCLINE `mu_rle_uint64(b)`: Creates an expression of type `uint64_m` that reads the value stored in the 8-byte-length little-endian byte array `b`.
					#ifndef mu_rle_uint64
						#define mu_rle_uint64(b) ((uint64_m)b[0] << 0 | (uint64_m)b[1] << 8 | (uint64_m)b[2] << 16 | (uint64_m)b[3] << 24 | (uint64_m)b[4] << 32 | (uint64_m)b[5] << 40 | (uint64_m)b[6] << 48 | (uint64_m)b[7] << 56)
					#endif
					// @DOCLINE `mu_rbe_uint64(b)`: Creates an expression of type `uint64_m` that reads the value stored in the 8-byte-length big-endian byte array `b`.
					#ifndef mu_rbe_uint64
						#define mu_rbe_uint64(b) ((uint64_m)b[7] << 0 | (uint64_m)b[6] << 8 | (uint64_m)b[5] << 16 | (uint64_m)b[4] << 24 | (uint64_m)b[3] << 32 | (uint64_m)b[2] << 40 | (uint64_m)b[1] << 48 | (uint64_m)b[0] << 56)
					#endif

					// @DOCLINE `mu_wle_uint8(b, i)`: Reads the given contents of the 1-byte-length little-endian byte array `b` into the given `uint8_m` variable `i`.
					#ifndef mu_wle_uint8
						#define mu_wle_uint8(b, i) b[0] = (uint8_m)(i >> 0);
					#endif
					// @DOCLINE `mu_wbe_uint8(b, i)`: Reads the given contents of the 1-byte-length big-endian byte array `b` into the given `uint8_m` variable `i`.
					#ifndef mu_wbe_uint8
						#define mu_wbe_uint8(b, i) b[0] = (uint8_m)(i >> 0);
					#endif

					// @DOCLINE `mu_wle_uint16(b, i)`: Reads the given contents of the 2-byte-length little-endian byte array `b` into the given `uint16_m` variable `i`.
					#ifndef mu_wle_uint16
						#define mu_wle_uint16(b, i) b[0] = (uint8_m)(i >> 0); b[1] = (uint8_m)(i >> 8);
					#endif
					// @DOCLINE `mu_wbe_uint16(b, i)`: Reads the given contents of the 2-byte-length big-endian byte array `b` into the given `uint16_m` variable `i`.
					#ifndef mu_wbe_uint16
						#define mu_wbe_uint16(b, i) b[1] = (uint8_m)(i >> 0); b[0] = (uint8_m)(i >> 8);
					#endif

					// @DOCLINE `mu_wle_uint32(b, i)`: Reads the given contents of the 4-byte-length little-endian byte array `b` into the given `uint32_m` variable `i`.
					#ifndef mu_wle_uint32
						#define mu_wle_uint32(b, i) b[0] = (uint8_m)(i >> 0); b[1] = (uint8_m)(i >> 8); b[2] = (uint8_m)(i >> 16); b[3] = (uint8_m)(i >> 24);
					#endif
					// @DOCLINE `mu_wbe_uint32(b, i)`: Reads the given contents of the 4-byte-length big-endian byte array `b` into the given `uint32_m` variable `i`.
					#ifndef mu_wbe_uint32
						#define mu_wbe_uint32(b, i) b[3] = (uint8_m)(i >> 0); b[2] = (uint8_m)(i >> 8); b[1] = (uint8_m)(i >> 16); b[0] = (uint8_m)(i >> 24);
					#endif

					// @DOCLINE `mu_wle_uint64(b, i)`: Reads the given contents of the 8-byte-length little-endian byte array `b` into the given `uint64_m` variable `i`.
					#ifndef mu_wle_uint64
						#define mu_wle_uint64(b, i) b[0] = (uint8_m)(i >> 0); b[1] = (uint8_m)(i >> 8); b[2] = (uint8_m)(i >> 16); b[3] = (uint8_m)(i >> 24); b[4] = (uint8_m)(i >> 32); b[5] = (uint8_m)(i >> 40); b[6] = (uint8_m)(i >> 48); b[7] = (uint8_m)(i >> 56);
					#endif
					// @DOCLINE `mu_wbe_uint64(b, i)`: Reads the given contents of the 8-byte-length big-endian byte array `b` into the given `uint64_m` variable `i`.
					#ifndef mu_wbe_uint64
						#define mu_wbe_uint64(b, i) b[7] = (uint8_m)(i >> 0); b[6] = (uint8_m)(i >> 8); b[5] = (uint8_m)(i >> 16); b[4] = (uint8_m)(i >> 24); b[3] = (uint8_m)(i >> 32); b[2] = (uint8_m)(i >> 40); b[1] = (uint8_m)(i >> 48); b[0] = (uint8_m)(i >> 56);
					#endif

				// @DOCLINE ## Null values

					// @DOCLINE The `MU_NULL_PTR` macro is a macro value used to represent a null pointer. It is simply 0.
					#ifndef MU_NULL_PTR
						#define MU_NULL_PTR 0
					#endif

					// @DOCLINE The `MU_NULL` macro is a macro value used to represent a null value. It is simply 0.
					#ifndef MU_NULL
						#define MU_NULL 0
					#endif

				// @DOCLINE ## Set result

					/* @DOCBEGIN The `MU_SET_RESULT(res, val)` macro is a macro function that checks if the given parameter `res` is a null pointer. If it is, it does nothing, but if it isn't, it sets its value to the given parameter `val`. This macro saves a lot of code, shrinking down what would be this:

					```c
					if (result != MU_NULL_PTR) {
					    *result = ...;
					}
					```

					into this:
					
					```c
					MU_SET_RESULT(result, ...)
					```

					@DOCEND */

					#ifndef MU_SET_RESULT
						#define MU_SET_RESULT(res, val) if(res!=MU_NULL_PTR){*res=val;}
					#endif

				// @DOCLINE ## Enum

					/* @DOCBEGIN

					The `MU_ENUM(name, ...)` macro is a macro function used to declare an enumerator. The reason why one would prefer this over the traditional way of declaring enumerators is because it actually makes it a `size_m` which can avoid errors on certain compilers (looking at you, Microsoft) in regards to treating enumerators like values. It expands like this:

					```c
					enum _##name{
					    __VA_ARGS__
					};
					typedef enum _##name _##name;
					typedef size_m name;
					```

					@DOCEND */

					#define MU_ENUM(name, ...) enum _##name{__VA_ARGS__};typedef enum _##name _##name; typedef size_m name;

				// @DOCLINE ## Operating system recognition

					/* @DOCBEGIN The macros `MU_WIN32` or `MU_UNIX` are defined (if neither were defined before) in order to allow mu libraries to easily check if they're running on a Windows or Unix system.

					`MU_WIN32` will be defined if `WIN32` or `_WIN32` are defined, one of which is usually pre-defined on Windows systems.

					`MU_UNIX` will be defined if `__unix__` is defined or both `__APPLE__` and `__MACH__` are defined.

					@DOCEND */

					#if !defined(MU_WIN32) && !defined(MU_UNIX)
						#if defined(WIN32) || defined(_WIN32)
							#define MU_WIN32
						#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
							#define MU_UNIX
						#endif
					#endif

			#ifdef __cplusplus
			}
			#endif

		#endif /* MUU_H */
	// @ATTENTION

	#ifdef __cplusplus
	extern "C" { // }
	#endif

	/* Objects */

		#define muWindow void*

	/* Macros */

		/* States */

			#define muButtonState muBool
			#define MU_BUTTON_STATE_RELEASED 0
			#define MU_BUTTON_STATE_HELD 1

			#define muState muBool
			#define MU_ON 1
			#define MU_OFF 0

		/* OS macros */

			#ifndef MUCOSA_NO_AUTOMATIC_API

				#ifdef MU_UNIX
					#define MUCOSA_X11
				#endif

				#ifdef MU_WIN32
					#define MUCOSA_WIN32
				#endif

			#endif

			#if defined(MUCOSA_VULKAN) && !defined(MUCOSA_NO_INCLUDE_VULKAN)
				#ifdef MUCOSA_WIN32
					#define VK_USE_PLATFORM_WIN32_KHR
				#elif defined(MUCOSA_X11)
					#define VK_USE_PLATFORM_XLIB_KHR
				#endif
			#endif

	/* Enums */

		MU_ENUM(muCOSAResult,
			MUCOSA_SUCCESS,

			MUCOSA_ALREADY_INITIALIZED,
			MUCOSA_ALREADY_TERMINATED,
			MUCOSA_ALREADY_ACTIVE,
			MUCOSA_ALREADY_INACTIVE,

			MUCOSA_NOT_YET_INITIALIZED,

			MUCOSA_ALLOCATION_FAILED,

			MUCOSA_UNKNOWN_WINDOW_SYSTEM,
			MUCOSA_UNKNOWN_GRAPHICS_API,
			MUCOSA_UNKNOWN_KEYBOARD_KEY,
			MUCOSA_UNKNOWN_KEYBOARD_STATE,
			MUCOSA_UNKNOWN_MOUSE_BUTTON,
			MUCOSA_UNKNOWN_WINDOW_HANDLE,

			MUCOSA_UNSUPPORTED_WINDOW_SYSTEM,
			MUCOSA_UNSUPPORTED_FEATURE, // Could mean that it rather can't be (or hasn't been) implemented, or that
			// the specific thing you're trying to do won't work on this device/OS for some reason (for example, on X11,
			// an Atom needed for that task can't be found).
			MUCOSA_UNSUPPORTED_OPENGL_FEATURE, // Something relating to OpenGL isn't supported or couldn't be found.
			MUCOSA_UNSUPPORTED_GRAPHICS_API, // Graphics API function was called when that API
			// wasn't explicity defined.

			MUCOSA_FAILED_CONNECTION_TO_SERVER,
			MUCOSA_FAILED_CREATE_WINDOW,
			MUCOSA_FAILED_LOAD_FUNCTIONS,
			MUCOSA_FAILED_FIND_COMPATIBLE_FRAMEBUFFER, // Not fatal
			MUCOSA_FAILED_CREATE_OPENGL_CONTEXT,
			MUCOSA_FAILED_LOAD_OPENGL_CONTEXT,
			MUCOSA_FAILED_USE_PIXEL_FORMAT, // Not fatal
			MUCOSA_FAILED_JOIN_THREAD,
			MUCOSA_FAILED_CREATE_THREAD,
			MUCOSA_FAILED_CREATE_INPUT_METHOD, // (X11)
			MUCOSA_FAILED_GET_INPUT_STYLES, // (X11)
			MUCOSA_FAILED_FIND_COMPATIBLE_INPUT_STYLE, // (X11)
			MUCOSA_FAILED_CREATE_INPUT_CONTEXT, // (X11)
			MUCOSA_FAILED_REGISTER_WINDOW_CLASS, // (Win32)
			MUCOSA_FAILED_CONVERT_UTF8_TO_WCHAR, // (Win32)
			MUCOSA_FAILED_CONVERT_WCHAR_TO_UTF8, // (Win32)
			MUCOSA_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS, // (Win32)
			MUCOSA_FAILED_CREATE_DUMMY_WGL_WINDOW, // (Win32)
			MUCOSA_FAILED_FIND_COMPATIBLE_PIXEL_FORMAT, // (Win32)
			MUCOSA_FAILED_DESCRIBE_PIXEL_FORMAT, // (Win32)
			MUCOSA_FAILED_SET_PIXEL_FORMAT, // (Win32)
			MUCOSA_FAILED_QUERY_WINDOW_INFO, // (Win32)
			MUCOSA_FAILED_SET_WINDOW_INFO, // (Win32)
			MUCOSA_FAILED_ASSOCIATE_IME_CONTEXT, // (Win32)
			MUCOSA_FAILED_LET_IMM_CONTEXT, // (Win32)
			MUCOSA_FAILED_GLOBAL_ALLOCATION, // (Win32)
			MUCOSA_FAILED_GLOBAL_LOCK, // (Win32)
			MUCOSA_FAILED_HOLD_CLIPBOARD, // (Win32)
			MUCOSA_FAILED_SET_CLIPBOARD, // (Win32)

			MUCOSA_INVALID_MINIMUM_MAXIMUM_BOOLS,
			MUCOSA_INVALID_MINIMUM_MAXIMUM_DIMENSIONS,
			MUCOSA_INVALID_ID,
			MUCOSA_INVALID_SAMPLE_COUNT,
			MUCOSA_INVALID_DIMENSIONS, // Often caused by width/height being outside of min/max width/height
			MUCOSA_INVALID_POINTER, // Like if you pass in a null pointer when a real pointer is
			// expected.
			MUCOSA_INVALID_WINDOW_STATE, // For example, when you try to focus a window that is
			// invisible or minimized in X11 (but only sometimes, IT'S COMPLICATED).
			MUCOSA_INVALID_TIME, // Like if you try passing a negative value to sleep for example.

			MUCOSA_NONEXISTENT_DEVICE, // For example, you requested the position of the cursor,
			// but there is no cursor. For whatever reason, lmao.

			MUCOSA_OVERSIZED_CLIPBOARD, // Clipboard was too big to copy. As of right now, on X11,
			// this is triggered by INCR not being implemented, which is my bad and should be done
			// at some point.

			MUCOSA_WINDOW_NON_RESIZABLE,
		)

		MU_ENUM(muWindowSystem,
			MU_WINDOW_SYSTEM_UNKNOWN,

			MU_WINDOW_SYSTEM_AUTO,
			MU_WINDOW_SYSTEM_X11,
			MU_WINDOW_SYSTEM_WIN32,
		)

		MU_ENUM(muWindowHandle,
			MU_WINDOWS_HWND,
			MU_X11_DISPLAY,
			MU_X11_WINDOW,
			MU_X11_PARENT_WINDOW,
		)

		MU_ENUM(muGraphicsAPI,
			MU_NO_GRAPHICS_API,

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
		)
		#define MUCOSA_OPENGL_FIRST MU_OPENGL_1_0
		#define MUCOSA_OPENGL_LAST MU_OPENGL_4_6_COMPATIBILITY

		MU_ENUM(muCursorStyle,
			MU_CURSOR_STYLE_UNKNOWN=0,

			// Taken from LÖVE
			// https://love2d.org/wiki/CursorType
			// @TODO Add invisible
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
		)

		MU_ENUM(muKeyboardKey,
			MU_KEYBOARD_KEY_UNKNOWN,

			// Note: this list is by no means complete;
			// missing many basic ASCII characters like:
			// ~ \ [] ; . /
			// Plan to add more later.

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
			MU_KEYBOARD_KEY_PA1,
		)
		#define MU_KEYBOARD_KEY_FIRST MU_KEYBOARD_KEY_BACKSPACE
		#define MU_KEYBOARD_KEY_LAST MU_KEYBOARD_KEY_PA1

		MU_ENUM(muKeyboardState,
			MU_KEYBOARD_STATE_UNKNOWN,

			MU_KEYBOARD_STATE_CAPS_LOCK,
			MU_KEYBOARD_STATE_SCROLL_LOCK,
			MU_KEYBOARD_STATE_NUM_LOCK,
		)
		#define MU_KEYBOARD_STATE_FIRST MU_KEYBOARD_STATE_CAPS_LOCK
		#define MU_KEYBOARD_STATE_LAST MU_KEYBOARD_STATE_NUM_LOCK

		MU_ENUM(muMouseButton,
			MU_MOUSE_BUTTON_UNKNOWN,

			MU_MOUSE_BUTTON_LEFT,
			MU_MOUSE_BUTTON_RIGHT,
			MU_MOUSE_BUTTON_MIDDLE,
		)
		#define MU_MOUSE_BUTTON_FIRST MU_MOUSE_BUTTON_LEFT
		#define MU_MOUSE_BUTTON_LAST MU_MOUSE_BUTTON_MIDDLE

	/* Structs */

		struct muCOSAContext {
			muCOSAResult result;

			void* inner;
		};

		struct muPixelFormat {
			uint16_m red_bits;
			uint16_m green_bits;
			uint16_m blue_bits;
			uint16_m alpha_bits;

			uint16_m depth_bits;
			uint16_m stencil_bits;

			uint8_m samples; // Max: 16, must be a power of 2
		};
		typedef struct muPixelFormat muPixelFormat;

		struct muWindowCreateInfo {
			muPixelFormat pixel_format;

			muBool visible;
			muBool resizable;

			muBool minimized;
			muBool maximized;

			int32_m x;
			int32_m y;

			uint32_m min_width;
			uint32_m min_height;
			uint32_m max_width;
			uint32_m max_height;

			muCursorStyle cursor_style; // <-- Not permanant, only existing as the style first used upon the window's creation

			void (*dimensions_callback)(muWindow window, uint32_m width, uint32_m height);
			void (*position_callback)(muWindow window, int32_m x, int32_m y);
			void (*focus_callback)(muWindow window, muBool focused);
			void (*maximize_callback)(muWindow window, muBool maximized);
			void (*minimize_callback)(muWindow window, muBool minimized);

			void (*keyboard_key_callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state);
			void (*keyboard_state_callback)(muWindow window, muKeyboardState keyboard_state, muState state);

			void (*cursor_position_callback)(muWindow window, int32_m x, int32_m y);
			void (*mouse_button_callback)(muWindow window, muMouseButton mouse_button, muButtonState state);
			void (*scroll_callback)(muWindow window, int32_m scroll_level_add);
		};
		typedef struct muWindowCreateInfo muWindowCreateInfo;

	/* Functions */

		/* Context */

			typedef struct muCOSAContext muCOSAContext;

			// Note: result of these are stored within muCOSAContext->result
			MUDEF void muCOSA_context_create(muCOSAContext* context, muWindowSystem system, muBool set_context);
			MUDEF void muCOSA_context_destroy(muCOSAContext* context);

			MUDEF void muCOSA_context_set(muCOSAContext* context);

			MUDEF muWindowSystem muCOSA_context_get_window_system(muCOSAContext* context);

		/* Window */

			/* Creation / Destruction */

				MUDEF muWindowCreateInfo mu_window_default_create_info(void);

				MUDEF muWindow mu_window_create(muGraphicsAPI api, muBool (*load_functions)(void), const char* name, uint16_m width, uint16_m height, muWindowCreateInfo create_info);
					MUDEF muWindow mu_window_create_(muCOSAResult* result, muGraphicsAPI api, muBool (*load_functions)(void), const char* name, uint16_m width, uint16_m height, muWindowCreateInfo create_info);
					MUDEF muWindow muCOSA_window_create(muCOSAContext* context, muCOSAResult* result, muGraphicsAPI api, muBool (*load_functions)(void), const char* name, uint16_m width, uint16_m height, muWindowCreateInfo create_info);

				MUDEF muWindow mu_window_destroy(muWindow window);
					MUDEF muWindow mu_window_destroy_(muCOSAResult* result, muWindow window);
					MUDEF muWindow muCOSA_window_destroy(muCOSAContext* context, muCOSAResult* result, muWindow window);

			/* Main loop */

				MUDEF muBool mu_window_get_closed(muWindow window);
					MUDEF muBool mu_window_get_closed_(muCOSAResult* result, muWindow window);
					MUDEF muBool muCOSA_window_get_closed(muCOSAContext* context, muCOSAResult* result, muWindow window);
				MUDEF void mu_window_close(muWindow window);
					MUDEF void mu_window_close_(muCOSAResult* result, muWindow window);
					MUDEF void muCOSA_window_close(muCOSAContext* context, muCOSAResult* result, muWindow window);

				MUDEF void mu_window_update(muWindow window);
					MUDEF void mu_window_update_(muCOSAResult* result, muWindow window);
					MUDEF void muCOSA_window_update(muCOSAContext* context, muCOSAResult* result, muWindow window);
				MUDEF void mu_window_swap_buffers(muWindow window);
					MUDEF void mu_window_swap_buffers_(muCOSAResult* result, muWindow window);
					MUDEF void muCOSA_window_swap_buffers(muCOSAContext* context, muCOSAResult* result, muWindow window);

			/* Get / Set */

				MUDEF muBool mu_window_get_focused(muWindow window);
					MUDEF muBool mu_window_get_focused_(muCOSAResult* result, muWindow window);
					MUDEF muBool muCOSA_window_get_focused(muCOSAContext* context, muCOSAResult* result, muWindow window);
				MUDEF void mu_window_focus(muWindow window);
					MUDEF void mu_window_focus_(muCOSAResult* result, muWindow window);
					MUDEF void muCOSA_window_focus(muCOSAContext* context, muCOSAResult* result, muWindow window);

				MUDEF muBool mu_window_get_visible(muWindow window);
					MUDEF muBool mu_window_get_visible_(muCOSAResult* result, muWindow window);
					MUDEF muBool muCOSA_window_get_visible(muCOSAContext* context, muCOSAResult* result, muWindow window);
				MUDEF void mu_window_set_visible(muWindow window, muBool visible);
					MUDEF void mu_window_set_visible_(muCOSAResult* result, muWindow window, muBool visible);
					MUDEF void muCOSA_window_set_visible(muCOSAContext* context, muCOSAResult* result, muWindow window, muBool visible);

				MUDEF void mu_window_get_position(muWindow window, int32_m* x, int32_m* y);
					MUDEF void mu_window_get_position_(muCOSAResult* result, muWindow window, int32_m* x, int32_m* y);
					MUDEF void muCOSA_window_get_position(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m* x, int32_m* y);
				MUDEF void mu_window_set_position(muWindow window, int32_m x, int32_m y);
					MUDEF void mu_window_set_position_(muCOSAResult* result, muWindow window, int32_m x, int32_m y);
					MUDEF void muCOSA_window_set_position(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m x, int32_m y);

				MUDEF void mu_window_get_dimensions(muWindow window, uint32_m* width, uint32_m* height);
					MUDEF void mu_window_get_dimensions_(muCOSAResult* result, muWindow window, uint32_m* width, uint32_m* height);
					MUDEF void muCOSA_window_get_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m* width, uint32_m* height);
				MUDEF void mu_window_set_dimensions(muWindow window, uint32_m width, uint32_m height);
					MUDEF void mu_window_set_dimensions_(muCOSAResult* result, muWindow window, uint32_m width, uint32_m height);
					MUDEF void muCOSA_window_set_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m width, uint32_m height);

				MUDEF muBool mu_window_get_maximized(muWindow window);
					MUDEF muBool mu_window_get_maximized_(muCOSAResult* result, muWindow window);
					MUDEF muBool muCOSA_window_get_maximized(muCOSAContext* context, muCOSAResult* result, muWindow window);
				MUDEF void mu_window_set_maximized(muWindow window, muBool maximized);
					MUDEF void mu_window_set_maximized_(muCOSAResult* result, muWindow window, muBool maximized);
					MUDEF void muCOSA_window_set_maximized(muCOSAContext* context, muCOSAResult* result, muWindow window, muBool maximized);

				MUDEF muBool mu_window_get_minimized(muWindow window);
					MUDEF muBool mu_window_get_minimized_(muCOSAResult* result, muWindow window);
					MUDEF muBool muCOSA_window_get_minimized(muCOSAContext* context, muCOSAResult* result, muWindow window);
				MUDEF void mu_window_set_minimized(muWindow window, muBool minimized);
					MUDEF void mu_window_set_minimized_(muCOSAResult* result, muWindow window, muBool minimized);
					MUDEF void muCOSA_window_set_minimized(muCOSAContext* context, muCOSAResult* result, muWindow window, muBool minimized);

				MUDEF void mu_window_get_minimum_dimensions(muWindow window, uint32_m* min_width, uint32_m* min_height);
					MUDEF void mu_window_get_minimum_dimensions_(muCOSAResult* result, muWindow window, uint32_m* min_width, uint32_m* min_height);
					MUDEF void muCOSA_window_get_minimum_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m* min_width, uint32_m* min_height);
				MUDEF void mu_window_set_minimum_dimensions(muWindow window, uint32_m min_width, uint32_m min_height);
					MUDEF void mu_window_set_minimum_dimensions_(muCOSAResult* result, muWindow window, uint32_m min_width, uint32_m min_height);
					MUDEF void muCOSA_window_set_minimum_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m min_width, uint32_m min_height);
				MUDEF void mu_window_get_maximum_dimensions(muWindow window, uint32_m* max_width, uint32_m* max_height);
					MUDEF void mu_window_get_maximum_dimensions_(muCOSAResult* result, muWindow window, uint32_m* max_width, uint32_m* max_height);
					MUDEF void muCOSA_window_get_maximum_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m* max_width, uint32_m* max_height);
				MUDEF void mu_window_set_maximum_dimensions(muWindow window, uint32_m max_width, uint32_m max_height);
					MUDEF void mu_window_set_maximum_dimensions_(muCOSAResult* result, muWindow window, uint32_m max_width, uint32_m max_height);
					MUDEF void muCOSA_window_set_maximum_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m max_width, uint32_m max_height);

				MUDEF void mu_window_get_cursor_position(muWindow window, int32_m* x, int32_m* y);
					MUDEF void mu_window_get_cursor_position_(muCOSAResult* result, muWindow window, int32_m* x, int32_m* y);
					MUDEF void muCOSA_window_get_cursor_position(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m* x, int32_m* y);
				MUDEF void mu_window_set_cursor_position(muWindow window, int32_m x, int32_m y);
					MUDEF void mu_window_set_cursor_position_(muCOSAResult* result, muWindow window, int32_m x, int32_m y);
					MUDEF void muCOSA_window_set_cursor_position(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m x, int32_m y);

				MUDEF muCursorStyle mu_window_get_cursor_style(muWindow window);
					MUDEF muCursorStyle mu_window_get_cursor_style_(muCOSAResult* result, muWindow window);
					MUDEF muCursorStyle muCOSA_window_get_cursor_style(muCOSAContext* context, muCOSAResult* result, muWindow window);
				MUDEF void mu_window_set_cursor_style(muWindow window, muCursorStyle style);
					MUDEF void mu_window_set_cursor_style_(muCOSAResult* result, muWindow window, muCursorStyle style);
					MUDEF void muCOSA_window_set_cursor_style(muCOSAContext* context, muCOSAResult* result, muWindow window, muCursorStyle style);

				MUDEF int32_m mu_window_get_scroll_level(muWindow window);
					MUDEF int32_m mu_window_get_scroll_level_(muCOSAResult* result, muWindow window);
					MUDEF int32_m muCOSA_window_get_scroll_level(muCOSAContext* context, muCOSAResult* result, muWindow window);
				MUDEF void mu_window_set_scroll_level(muWindow window, int32_m scroll_level);
					MUDEF void mu_window_set_scroll_level_(muCOSAResult* result, muWindow window, int32_m scroll_level);
					MUDEF void muCOSA_window_set_scroll_level(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m scroll_level);

			/* Get / Let */

				MUDEF void mu_window_get_text_input_focus(muWindow window, int32_m text_cursor_x, int32_m text_cursor_y, void (*callback)(muWindow window, const char* input));
					MUDEF void mu_window_get_text_input_focus_(muCOSAResult* result, muWindow window, int32_m text_cursor_x, int32_m text_cursor_y, void (*callback)(muWindow window, const char* input));
					MUDEF void muCOSA_window_get_text_input_focus(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m text_cursor_x, int32_m text_cursor_y, void (*callback)(muWindow window, const char* input));
				MUDEF void mu_window_update_text_cursor(muWindow window, int32_m x, int32_m y);
					MUDEF void mu_window_update_text_cursor_(muCOSAResult* result, muWindow window, int32_m x, int32_m y);
					MUDEF void muCOSA_window_update_text_cursor(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m x, int32_m y);
				MUDEF void mu_window_let_text_input_focus(muWindow window);
					MUDEF void mu_window_let_text_input_focus_(muCOSAResult* result, muWindow window);
					MUDEF void muCOSA_window_let_text_input_focus(muCOSAContext* context, muCOSAResult* result, muWindow window);

			/* Get */

				MUDEF void mu_window_get_frame_extents(muWindow window, uint32_m* left, uint32_m* right, uint32_m* top, uint32_m* bottom);
					MUDEF void mu_window_get_frame_extents_(muCOSAResult* result, muWindow window, uint32_m* left, uint32_m* right, uint32_m* top, uint32_m* bottom);
					MUDEF void muCOSA_window_get_frame_extents(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m* left, uint32_m* right, uint32_m* top, uint32_m* bottom);

				MUDEF muButtonState mu_window_get_keyboard_key_state(muWindow window, muKeyboardKey key);
					MUDEF muButtonState mu_window_get_keyboard_key_state_(muCOSAResult* result, muWindow window, muKeyboardKey key);
					MUDEF muButtonState muCOSA_window_get_keyboard_key_state(muCOSAContext* context, muCOSAResult* result, muWindow window, muKeyboardKey key);
				MUDEF muState mu_window_get_keyboard_state_state(muWindow window, muKeyboardState state);
					MUDEF muState mu_window_get_keyboard_state_state_(muCOSAResult* result, muWindow window, muKeyboardState state);
					MUDEF muState muCOSA_window_get_keyboard_state_state(muCOSAContext* context, muCOSAResult* result, muWindow window, muKeyboardState state);

				MUDEF muButtonState mu_window_get_mouse_button_state(muWindow window, muMouseButton button);
					MUDEF muButtonState mu_window_get_mouse_button_state_(muCOSAResult* result, muWindow window, muMouseButton button);
					MUDEF muButtonState muCOSA_window_get_mouse_button_state(muCOSAContext* context, muCOSAResult* result, muWindow window, muMouseButton button);

			/* Set */

				MUDEF void mu_window_set_title(muWindow window, const char* title);
					MUDEF void mu_window_set_title_(muCOSAResult* result, muWindow window, const char* title);
					MUDEF void muCOSA_window_set_title(muCOSAContext* context, muCOSAResult* result, muWindow window, const char* title);

				MUDEF void mu_window_set_dimensions_callback(muWindow window, void (*callback)(muWindow window, uint32_m width, uint32_m height));
					MUDEF void mu_window_set_dimensions_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, uint32_m width, uint32_m height));
					MUDEF void muCOSA_window_set_dimensions_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, uint32_m width, uint32_m height));
				MUDEF void mu_window_set_position_callback(muWindow window, void (*callback)(muWindow window, int32_m x, int32_m y));
					MUDEF void mu_window_set_position_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m x, int32_m y));
					MUDEF void muCOSA_window_set_position_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m x, int32_m y));
				MUDEF void mu_window_set_focus_callback(muWindow window, void (*callback)(muWindow window, muBool focused));
					MUDEF void mu_window_set_focus_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool focused));
					MUDEF void muCOSA_window_set_focus_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool focused));
				MUDEF void mu_window_set_maximize_callback(muWindow window, void (*callback)(muWindow window, muBool maximized));
					MUDEF void mu_window_set_maximize_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool maximized));
					MUDEF void muCOSA_window_set_maximize_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool maximized));
				MUDEF void mu_window_set_minimize_callback(muWindow window, void (*callback)(muWindow window, muBool minimized));
					MUDEF void mu_window_set_minimize_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool minimized));
					MUDEF void muCOSA_window_set_minimize_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool minimized));

				MUDEF void mu_window_set_keyboard_key_callback(muWindow window, void (*callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state));
					MUDEF void mu_window_set_keyboard_key_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state));
					MUDEF void muCOSA_window_set_keyboard_key_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state));
				MUDEF void mu_window_set_keyboard_state_callback(muWindow window, void (*callback)(muWindow window, muKeyboardState keyboard_state, muState state));
					MUDEF void mu_window_set_keyboard_state_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardState keyboard_state, muState state));
					MUDEF void muCOSA_window_set_keyboard_state_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardState keyboard_state, muState state));

				MUDEF void mu_window_set_cursor_position_callback(muWindow win, void (*callback)(muWindow window, int32_m x, int32_m y));
					MUDEF void mu_window_set_cursor_position_callback_(muCOSAResult* result, muWindow win, void (*callback)(muWindow window, int32_m x, int32_m y));
					MUDEF void muCOSA_window_set_cursor_position_callback(muCOSAContext* context, muCOSAResult* result, muWindow win, void (*callback)(muWindow window, int32_m x, int32_m y));
				MUDEF void mu_window_set_mouse_button_callback(muWindow window, void (*callback)(muWindow window, muMouseButton mouse_button, muButtonState state));
					MUDEF void mu_window_set_mouse_button_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muMouseButton mouse_button, muButtonState state));
					MUDEF void muCOSA_window_set_mouse_button_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muMouseButton mouse_button, muButtonState state));
				MUDEF void mu_window_set_scroll_callback(muWindow window, void (*callback)(muWindow window, int32_m scroll_level_add));
					MUDEF void mu_window_set_scroll_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m scroll_level_add));
					MUDEF void muCOSA_window_set_scroll_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m scroll_level_add));

		/* Time */

			MUDEF double mu_time_get(void);
				MUDEF double mu_time_get_(muCOSAResult* result);
				MUDEF double muCOSA_time_get(muCOSAContext* context, muCOSAResult* result);

			MUDEF void mu_time_set(double time);
				MUDEF void mu_time_set_(muCOSAResult* result, double time);
				MUDEF void muCOSA_time_set(muCOSAContext* context, muCOSAResult* result, double time);

			MUDEF void mu_sleep(double time);
				MUDEF void mu_sleep_(muCOSAResult* result, double time);
				MUDEF void muCOSA_sleep(muCOSAContext* context, muCOSAResult* result, double time);

		/* Clipboard */

			MUDEF char* mu_clipboard_get(void);
				MUDEF char* mu_clipboard_get_(muCOSAResult* result);
				MUDEF char* muCOSA_clipboard_get(muCOSAContext* context, muCOSAResult* result);
			MUDEF void mu_clipboard_set(const char* text, size_m text_size);
				MUDEF void mu_clipboard_set_(muCOSAResult* result, const char* text, size_m text_size);
				MUDEF void muCOSA_clipboard_set(muCOSAContext* context, muCOSAResult* result, const char* text, size_m text_size);

		/* OS functions */

			MUDEF void* mu_os_get_window_handle(muWindow window, muWindowHandle handle);
				MUDEF void* mu_os_get_window_handle_(muCOSAResult* result, muWindow window, muWindowHandle handle);
				MUDEF void* muCOSA_os_get_window_handle(muCOSAContext* context, muCOSAResult* result, muWindow window, muWindowHandle handle);

		/* OpenGL */

			MUDEF void mu_opengl_bind_window(muWindow window);
				MUDEF void mu_opengl_bind_window_(muCOSAResult* result, muWindow window);
				MUDEF void muCOSA_opengl_bind_window(muCOSAContext* context, muCOSAResult* result, muWindow window);

			MUDEF void* mu_opengl_get_function_address(const char* name);
				MUDEF void* muCOSA_opengl_get_function_address(muCOSAContext* context, const char* name);

			MUDEF void mu_opengl_window_swap_interval(muWindow window, int interval);
				MUDEF void mu_opengl_window_swap_interval_(muCOSAResult* result, muWindow window, int interval);
				MUDEF void muCOSA_opengl_window_swap_interval(muCOSAContext* context, muCOSAResult* result, muWindow window, int interval);

		/* Vulkan */

			// Note: also not necessarily UTF-8
			MUDEF const char** mu_vulkan_get_surface_instance_extensions(size_m* count);
				MUDEF const char** mu_vulkan_get_surface_instance_extensions_(muCOSAResult* result, size_m* count);
				MUDEF const char** muCOSA_vulkan_get_surface_instance_extensions(muCOSAContext* context, muCOSAResult* result, size_m* count);

			// vk_result interpreted as VkResult*, instance as VkInstance*, allocator as const VkAllocationCallbacks*, and surface as VkSurfaceKHR*
			MUDEF void mu_vulkan_create_window_surface(muWindow window, void* vk_result, void* instance, void* allocator, void* surface);
				MUDEF void mu_vulkan_create_window_surface_(muCOSAResult* result, muWindow window, void* vk_result, void* instance, void* allocator, void* surface);
				MUDEF void muCOSA_vulkan_create_window_surface(muCOSAContext* context, muCOSAResult* result, muWindow window, void* vk_result, void* instance, void* allocator, void* surface);

		/* Names */

			#ifdef MUCOSA_NAMES
				MUDEF const char* muCOSA_result_get_name(muCOSAResult result);

				MUDEF const char* mu_window_system_get_name(muWindowSystem system);
				MUDEF const char* mu_window_system_get_nice_name(muWindowSystem system);

				MUDEF const char* mu_window_handle_get_name(muWindowHandle handle);
				MUDEF const char* mu_window_handle_get_nice_name(muWindowHandle handle);

				MUDEF const char* mu_graphics_api_get_name(muGraphicsAPI api);
				MUDEF const char* mu_graphics_api_get_nice_name(muGraphicsAPI api);

				MUDEF const char* mu_cursor_style_get_name(muCursorStyle style);
				MUDEF const char* mu_cursor_style_get_nice_name(muCursorStyle style);

				MUDEF const char* mu_keyboard_key_get_name(muKeyboardKey key);
				MUDEF const char* mu_keyboard_key_get_nice_name(muKeyboardKey key);

				MUDEF const char* mu_keyboard_state_get_name(muKeyboardState state);
				MUDEF const char* mu_keyboard_state_get_nice_name(muKeyboardState state);

				MUDEF const char* mu_mouse_button_get_name(muMouseButton button);
				MUDEF const char* mu_mouse_button_get_nice_name(muMouseButton button);
			#endif

	/* C standard library dependencies */

		#if !defined(mu_malloc) || \
			!defined(mu_free) || \
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

		#if !defined(mu_memcpy) || \
			!defined(mu_memset) || \
			!defined(mu_strlen) || \
			!defined(mu_strcmp)

			#include <string.h>

			#ifndef mu_memcpy
				#define mu_memcpy memcpy
			#endif

			#ifndef mu_memset
				#define mu_memset memset
			#endif

			#ifndef mu_strlen
				#define mu_strlen strlen
			#endif

			#ifndef mu_strcmp
				#define mu_strcmp strcmp
			#endif

		#endif

		#if !defined(mu_fflush) || \
			!defined(mu_stdout)

			#include <stdio.h>

			#ifndef mu_fflush
				#define mu_fflush fflush
			#endif

			#ifndef mu_stdout
				#define mu_stdout stdout
			#endif

		#endif

	#ifdef __cplusplus
	}
	#endif
#endif /* MUCOSA_H */


#ifdef MUCOSA_IMPLEMENTATION
	#ifdef __cplusplus
	extern "C" { // }
	#endif

	/* Macro handling */

		#ifdef MUCOSA_OPENGL
			#define MUCOSA_OPENGL_CALL(...) __VA_ARGS__
		#else
			#define MUCOSA_OPENGL_CALL(...)
		#endif

		#if defined(MUCOSA_VULKAN) && !defined(MUCOSA_NO_INCLUDE_VULKAN)
			#ifndef MUCOSA_VULKAN_INCLUDE_PATH
				#pragma message("[MUCOSA] If MUCOSA_VULKAN is defined and MUCOSA_NO_INCLUDE_VULKAN is not defined, then the include path for Vulkan is expected to be defined in the macro MUCOSA_VULKAN_INCLUDE_PATH; acting as if MUCOSA_VULKAN was not defined from this point on")
				#undef MUCOSA_VULKAN
			#endif
		#endif

	/* Shared functions */

		muCOSAResult muCOSA_verify_pixel_format(muPixelFormat pf) {
			if (pf.samples != 1 && pf.samples != 2 && pf.samples != 4 && pf.samples != 8 && pf.samples != 16) {
				return MUCOSA_INVALID_SAMPLE_COUNT;
			}
			return MUCOSA_SUCCESS;
		}

		// @TODO Call this in high-level window creation function
		muCOSAResult muCOSA_verify_window_create_info(muWindowCreateInfo ci) {
			if (ci.minimized && ci.maximized) {
				return MUCOSA_INVALID_MINIMUM_MAXIMUM_BOOLS;
			}
			if (ci.min_width > ci.max_width || ci.min_height > ci.max_height) {
				return MUCOSA_INVALID_MINIMUM_MAXIMUM_DIMENSIONS;
			}
			return muCOSA_verify_pixel_format(ci.pixel_format);
		}

	/* Win32 */

	#ifdef MUCOSA_WIN32

		#define MUCOSA_WIN32_CALL(...) __VA_ARGS__

		#include <windows.h> // For all of the stuff that can't be included directly (CURSE YOU BILL)

		/* Shared functions */

			// Thank you, Raymond Chen...
			EXTERN_C IMAGE_DOS_HEADER __ImageBase;
			HINSTANCE muCOSA_Win32_get_hinstance(void) {
				return ((HINSTANCE)&__ImageBase);
			}

			/* Strings */

				wchar_t* muCOSA_Win32_utf8_to_wchar(const char* str) {
					// Note: we can use -1 here, since it's expected for it to be null-terminated.
					int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
					if (len == 0) {
						return 0;
					}

					wchar_t* wstr = (wchar_t*)mu_malloc(len * sizeof(wchar_t));
					if (wstr == 0) {
						return 0;
					}

					if (MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)wstr, len) == 0) {
						mu_free(wstr);
						return 0;
					}

					return wstr;
				}

				muByte* muCOSA_Win32_wchar_to_utf8(wchar_t* wstr) {
					// Note: we don't need length here, either. This function is only intended to
					// be called with null-terminated wchar_t strings; for example, in the
					// clipboard get function, the format "CF_UNICODETEXT" is specified, and,
					// according to the docs: "A null character signals the end of the data."
					int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
					if (len == 0) {
						return 0;
					}

					muByte* str = (muByte*)mu_malloc(len);
					if (str == 0) {
						return 0;
					}

					if (WideCharToMultiByte(CP_UTF8, 0, wstr, -1, (LPSTR)str, len, NULL, NULL) == 0) {
						mu_free(str);
						return 0;
					}

					return str;
				}

			/* Window */

				void muCOSA_Win32_window_get_def_frame_extents(uint32_m* l, uint32_m* r, uint32_m* t, uint32_m* b) {
					MU_SET_RESULT(l, (uint32_m)GetSystemMetrics(SM_CXSIZEFRAME))
					MU_SET_RESULT(r, (uint32_m)GetSystemMetrics(SM_CXSIZEFRAME))
					MU_SET_RESULT(t, (uint32_m)(GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(92)))
					MU_SET_RESULT(b, (uint32_m)GetSystemMetrics(SM_CXSIZEFRAME))
				}

				/* Pointer map */

					struct muCOSA_Win32_window_pmap {
						void** pointers;
						size_m pointer_count;
						LONG volatile locked;
						size_m access_count;
					};
					typedef struct muCOSA_Win32_window_pmap muCOSA_Win32_window_pmap;

					muCOSA_Win32_window_pmap muCOSA_Win32_global_window_pmap = MU_ZERO_STRUCT_CONST(muCOSA_Win32_window_pmap);

					void muCOSA_Win32_window_pmap_lock(void) {
						while (InterlockedCompareExchange(&muCOSA_Win32_global_window_pmap.locked, 1, 0) == 1) {}
					}

					void muCOSA_Win32_window_pmap_unlock(void) {
						_interlockedbittestandreset(&muCOSA_Win32_global_window_pmap.locked, 0);
					}

					muBool muCOSA_Win32_window_pmap_add(void* p, size_m size) {
						muCOSA_Win32_window_pmap_lock();

						muCOSA_Win32_global_window_pmap.pointer_count += 1;
						if (!muCOSA_Win32_global_window_pmap.pointers) {
							muCOSA_Win32_global_window_pmap.pointers = (void**)mu_malloc(size * muCOSA_Win32_global_window_pmap.pointer_count);
							if (!muCOSA_Win32_global_window_pmap.pointers) {
								muCOSA_Win32_window_pmap_unlock();
								return MU_FALSE;
							}
						} else {
							void** old_ptr = muCOSA_Win32_global_window_pmap.pointers;
							muCOSA_Win32_global_window_pmap.pointers = (void**)mu_realloc(muCOSA_Win32_global_window_pmap.pointers, size * muCOSA_Win32_global_window_pmap.pointer_count);
							if (!muCOSA_Win32_global_window_pmap.pointers) {
								muCOSA_Win32_global_window_pmap.pointers = old_ptr;
								muCOSA_Win32_window_pmap_unlock();
								return MU_FALSE;
							}
						}

						muCOSA_Win32_global_window_pmap.pointers[muCOSA_Win32_global_window_pmap.pointer_count-1] = p;

						muCOSA_Win32_window_pmap_unlock();
						return MU_TRUE;
					}

					void muCOSA_Win32_window_pmap_remove(void* p, size_m size) {
						muCOSA_Win32_window_pmap_lock();

						for (size_m i = 0; i < muCOSA_Win32_global_window_pmap.pointer_count; i++) {
							if (muCOSA_Win32_global_window_pmap.pointers[i] == (void**)p) {
								muCOSA_Win32_global_window_pmap.pointers[i] = 0;
							}
						}

						muCOSA_Win32_window_pmap_unlock();
						return; if (size) {}
					}

					// Note: doesn't lock
					void muCOSA_Win32_window_pmap_cleanup(void) {
						if (muCOSA_Win32_global_window_pmap.pointers) {
							mu_free(muCOSA_Win32_global_window_pmap.pointers);
							muCOSA_Win32_global_window_pmap = MU_ZERO_STRUCT(muCOSA_Win32_window_pmap);
						}
					}

					void muCOSA_Win32_window_pmap_access(void) {
						muCOSA_Win32_window_pmap_lock();
						muCOSA_Win32_global_window_pmap.access_count += 1;
						muCOSA_Win32_window_pmap_unlock();
					}

					void muCOSA_Win32_window_pmap_deaccess(void) {
						muCOSA_Win32_window_pmap_lock();
						muCOSA_Win32_global_window_pmap.access_count -= 1;
						if (muCOSA_Win32_global_window_pmap.access_count == 0) {
							muCOSA_Win32_window_pmap_cleanup();
						}
						muCOSA_Win32_window_pmap_unlock();
					}

		/* Time */

			/* Inner */

				// https://stackoverflow.com/questions/1695288/getting-the-current-time-in-milliseconds-from-the-system-clock-in-windows
				double muCOSA_Win32_get_current_time(void) {
					FILETIME file_time;
					GetSystemTimeAsFileTime(&file_time);
					LONGLONG ll_now = (LONGLONG)file_time.dwLowDateTime + ((LONGLONG)(file_time.dwHighDateTime) << 32LL);
					return (double)ll_now / (double)1.0e7;
				}

			/* Outer */

				struct muCOSA_Win32_time {
					double orig_time;
				};
				typedef struct muCOSA_Win32_time muCOSA_Win32_time;

				void muCOSA_Win32_time_init(muCOSA_Win32_time* time) {
					time->orig_time = muCOSA_Win32_get_current_time();
				}

		/* OpenGL */

		#ifdef MUCOSA_OPENGL

			/* Setup */

				#ifndef MUCOSA_NO_INCLUDE_OPENGL
					#ifndef glClearColor
						#include <gl/gl.h>
						#include <gl/glu.h>
					#endif
				#endif

				#include <wingdi.h>

				typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int* attribList);
				typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);

				struct muCOSA_Win32_WGL {
					wglCreateContextAttribsARB_type* wglCreateContextAttribsARB;
					wglChoosePixelFormatARB_type* wglChoosePixelFormatARB;
					BOOL (*wglSwapIntervalEXT)(int interval);
				};
				typedef struct muCOSA_Win32_WGL muCOSA_Win32_WGL;

			/* WGL tokens */

				// https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_create_context.txt

				#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
				#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
				#define WGL_CONTEXT_LAYER_PLANE_ARB               0x2093
				#define WGL_CONTEXT_FLAGS_ARB                     0x2094
				#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

				#define WGL_CONTEXT_DEBUG_BIT_ARB                 0x0001
				#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB    0x0002

				#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
				#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

				#define ERROR_INVALID_VERSION_ARB                 0x2095
				#define ERROR_INVALID_PROFILE_ARB                 0x2096

				// https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt

				#define WGL_NUMBER_PIXEL_FORMATS_ARB    0x2000
				#define WGL_DRAW_TO_WINDOW_ARB          0x2001
				#define WGL_DRAW_TO_BITMAP_ARB          0x2002
				#define WGL_ACCELERATION_ARB            0x2003
				#define WGL_NEED_PALETTE_ARB            0x2004
				#define WGL_NEED_SYSTEM_PALETTE_ARB     0x2005
				#define WGL_SWAP_LAYER_BUFFERS_ARB      0x2006
				#define WGL_SWAP_METHOD_ARB             0x2007
				#define WGL_NUMBER_OVERLAYS_ARB         0x2008
				#define WGL_NUMBER_UNDERLAYS_ARB        0x2009
				#define WGL_TRANSPARENT_ARB             0x200A
				#define WGL_TRANSPARENT_RED_VALUE_ARB   0x2037
				#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
				#define WGL_TRANSPARENT_BLUE_VALUE_ARB  0x2039
				#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
				#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
				#define WGL_SHARE_DEPTH_ARB             0x200C
				#define WGL_SHARE_STENCIL_ARB           0x200D
				#define WGL_SHARE_ACCUM_ARB             0x200E
				#define WGL_SUPPORT_GDI_ARB             0x200F
				#define WGL_SUPPORT_OPENGL_ARB          0x2010
				#define WGL_DOUBLE_BUFFER_ARB           0x2011
				#define WGL_STEREO_ARB                  0x2012
				#define WGL_PIXEL_TYPE_ARB              0x2013
				#define WGL_COLOR_BITS_ARB              0x2014
				#define WGL_RED_BITS_ARB                0x2015
				#define WGL_RED_SHIFT_ARB               0x2016
				#define WGL_GREEN_BITS_ARB              0x2017
				#define WGL_GREEN_SHIFT_ARB             0x2018
				#define WGL_BLUE_BITS_ARB               0x2019
				#define WGL_BLUE_SHIFT_ARB              0x201A
				#define WGL_ALPHA_BITS_ARB              0x201B
				#define WGL_ALPHA_SHIFT_ARB             0x201C
				#define WGL_ACCUM_BITS_ARB              0x201D
				#define WGL_ACCUM_RED_BITS_ARB          0x201E
				#define WGL_ACCUM_GREEN_BITS_ARB        0x201F
				#define WGL_ACCUM_BLUE_BITS_ARB         0x2020
				#define WGL_ACCUM_ALPHA_BITS_ARB        0x2021
				#define WGL_DEPTH_BITS_ARB              0x2022
				#define WGL_STENCIL_BITS_ARB            0x2023
				#define WGL_AUX_BUFFERS_ARB             0x2024

				#define WGL_NO_ACCELERATION_ARB         0x2025
				#define WGL_GENERIC_ACCELERATION_ARB    0x2026
				#define WGL_FULL_ACCELERATION_ARB       0x2027

				#define WGL_SWAP_EXCHANGE_ARB           0x2028
				#define WGL_SWAP_COPY_ARB               0x2029
				#define WGL_SWAP_UNDEFINED_ARB          0x202A

				#define WGL_TYPE_RGBA_ARB               0x202B
				#define WGL_TYPE_COLORINDEX_ARB         0x202C

				// https://nehe.gamedev.net/tutorial/fullscreen_antialiasing/16008/

				#define WGL_SAMPLE_BUFFERS_ARB  0x2041
				#define WGL_SAMPLES_ARB     0x2042

			/* Functions */

				// Note: result must be valid pointer
				void muCOSA_Win32_init_opengl_extensions(muCOSAResult* result, muCOSA_Win32_WGL* wgl) {
					WNDCLASSA wclass = MU_ZERO_STRUCT(WNDCLASSA);
					wclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
					wclass.lpfnWndProc = DefWindowProcA;
					wclass.hInstance = muCOSA_Win32_get_hinstance();
					wclass.lpszClassName = "Dummy WGL window class";
					if (!RegisterClassA(&wclass)) {
						*result = MUCOSA_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS;
						return;
					}

					HWND win = CreateWindowExA(0, wclass.lpszClassName, "Dummy WGL window", 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, wclass.hInstance, 0);
					if (win == NULL) {
						*result = MUCOSA_FAILED_CREATE_DUMMY_WGL_WINDOW;
						UnregisterClassA(wclass.lpszClassName, wclass.hInstance);
						return;
					}

					HDC dc = GetDC(win);

					// Note: this pixel format NEEDS to be chosen to make this function work. In
					// particular, the call to wglCreateContext fails if we don't. The choice of
					// which pixel format is arbitrary.

					PIXELFORMATDESCRIPTOR format = MU_ZERO_STRUCT(PIXELFORMATDESCRIPTOR);
					format.nSize = sizeof(PIXELFORMATDESCRIPTOR);
					format.nVersion = 1;
					format.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
					format.iPixelType = PFD_TYPE_RGBA;
					format.cColorBits = 32;
					format.cAlphaBits = 8;
					format.cDepthBits = 24;
					format.cStencilBits = 8;
					format.iLayerType = PFD_MAIN_PLANE;

					// Doing no error checking here in case the ATTEMPT of setting one allows
					// wglCreateContext to pass...
					int pixel_format = ChoosePixelFormat(dc, &format);
					SetPixelFormat(dc, pixel_format, &format);

					HGLRC context = wglCreateContext(dc);
					if (!context) {
						*result = MUCOSA_FAILED_CREATE_OPENGL_CONTEXT;
						ReleaseDC(win, dc);
						DestroyWindow(win);
						UnregisterClassA(wclass.lpszClassName, wclass.hInstance);
						return;
					}
					if (!wglMakeCurrent(dc, context)) {
						*result = MUCOSA_FAILED_LOAD_OPENGL_CONTEXT;
						wglDeleteContext(context);
						ReleaseDC(win, dc);
						DestroyWindow(win);
						UnregisterClassA(wclass.lpszClassName, wclass.hInstance);
						return;
					}

					PROC wglCreateContextAttribsARB_proc = wglGetProcAddress("wglCreateContextAttribsARB");
					mu_memcpy(&wgl->wglCreateContextAttribsARB, &wglCreateContextAttribsARB_proc, sizeof(PROC));
					PROC wglChoosePixelFormatARB_proc = wglGetProcAddress("wglChoosePixelFormatARB");
					mu_memcpy(&wgl->wglChoosePixelFormatARB, &wglChoosePixelFormatARB_proc, sizeof(PROC));

					wglMakeCurrent(dc, 0);
					wglDeleteContext(context);
					ReleaseDC(win, dc);
					DestroyWindow(win);
					UnregisterClassA(wclass.lpszClassName, wclass.hInstance);

					if (wgl->wglCreateContextAttribsARB == 0 || wgl-> wglChoosePixelFormatARB == 0) {
						*result = MUCOSA_UNSUPPORTED_OPENGL_FEATURE;
						return;
					}
				}

				// Note: result must be valid pointer
				void muCOSA_Win32_create_opengl_context(muCOSAResult* result, HDC dc, muCOSA_Win32_WGL* wgl, muPixelFormat pf, HGLRC* context, muGraphicsAPI api) {
					int pixel_format_attributes[] = {
						WGL_DRAW_TO_WINDOW_ARB, MU_TRUE,
						WGL_SUPPORT_OPENGL_ARB, MU_TRUE,
						WGL_DOUBLE_BUFFER_ARB,  MU_TRUE,
						WGL_SAMPLE_BUFFERS_ARB, MU_TRUE, // ?
						WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB, // ?
						WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
						WGL_RED_BITS_ARB,       pf.red_bits,
						WGL_GREEN_BITS_ARB,     pf.green_bits,
						WGL_BLUE_BITS_ARB,      pf.blue_bits,
						WGL_ALPHA_BITS_ARB,     pf.alpha_bits,
						WGL_DEPTH_BITS_ARB,     pf.depth_bits,
						WGL_STENCIL_BITS_ARB,   pf.stencil_bits,
						WGL_SAMPLES_ARB,        pf.samples,
						0
					};

					int pixel_format;
					UINT format_count;
					wgl->wglChoosePixelFormatARB(dc, pixel_format_attributes, 0, 1, &pixel_format, &format_count);
					if (!format_count) {
						*result = MUCOSA_FAILED_FIND_COMPATIBLE_PIXEL_FORMAT;
						return;
					}

					PIXELFORMATDESCRIPTOR format;
					if (DescribePixelFormat(dc, pixel_format, sizeof(PIXELFORMATDESCRIPTOR), &format) == 0) {
						*result = MUCOSA_FAILED_DESCRIBE_PIXEL_FORMAT;
						return;
					}

					if (!SetPixelFormat(dc, pixel_format, &format)) {
						*result = MUCOSA_FAILED_SET_PIXEL_FORMAT;
						return;
					}

					int opengl_attributes[] = {
						WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
						WGL_CONTEXT_MINOR_VERSION_ARB, 3,
						WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
						0
					};

					switch (api) {
						default: *result = MUCOSA_UNKNOWN_GRAPHICS_API; return; break;
						case MU_OPENGL_1_0:   { opengl_attributes[1] = 1; opengl_attributes[3] = 0; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
						case MU_OPENGL_1_1:   { opengl_attributes[1] = 1; opengl_attributes[3] = 1; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
						case MU_OPENGL_1_2:   { opengl_attributes[1] = 1; opengl_attributes[3] = 2; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
						// No idea what to do here lmao
						case MU_OPENGL_1_2_1: { opengl_attributes[1] = 1; opengl_attributes[3] = 2; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
						case MU_OPENGL_1_3:   { opengl_attributes[1] = 1; opengl_attributes[3] = 3; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
						case MU_OPENGL_1_4:   { opengl_attributes[1] = 1; opengl_attributes[3] = 4; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
						case MU_OPENGL_1_5:   { opengl_attributes[1] = 1; opengl_attributes[3] = 5; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
						case MU_OPENGL_2_0:   { opengl_attributes[1] = 2; opengl_attributes[3] = 0; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
						case MU_OPENGL_2_1:   { opengl_attributes[1] = 2; opengl_attributes[3] = 1; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
						case MU_OPENGL_3_0:   { opengl_attributes[1] = 3; opengl_attributes[3] = 0; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
						case MU_OPENGL_3_1:   { opengl_attributes[1] = 3; opengl_attributes[3] = 1; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
						case MU_OPENGL_3_2_CORE:          { opengl_attributes[1] = 3; opengl_attributes[3] = 2; opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_3_2_COMPATIBILITY: { opengl_attributes[1] = 3; opengl_attributes[3] = 2; opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_3_3_CORE:          { opengl_attributes[1] = 3; opengl_attributes[3] = 3; opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_3_3_COMPATIBILITY: { opengl_attributes[1] = 3; opengl_attributes[3] = 3; opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_4_0_CORE:          { opengl_attributes[1] = 4; opengl_attributes[3] = 0; opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_4_0_COMPATIBILITY: { opengl_attributes[1] = 4; opengl_attributes[3] = 0; opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_4_1_CORE:          { opengl_attributes[1] = 4; opengl_attributes[3] = 1; opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_4_1_COMPATIBILITY: { opengl_attributes[1] = 4; opengl_attributes[3] = 1; opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_4_2_CORE:          { opengl_attributes[1] = 4; opengl_attributes[3] = 2; opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_4_2_COMPATIBILITY: { opengl_attributes[1] = 4; opengl_attributes[3] = 2; opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_4_3_CORE:          { opengl_attributes[1] = 4; opengl_attributes[3] = 3; opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_4_3_COMPATIBILITY: { opengl_attributes[1] = 4; opengl_attributes[3] = 3; opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_4_4_CORE:          { opengl_attributes[1] = 4; opengl_attributes[3] = 4; opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_4_4_COMPATIBILITY: { opengl_attributes[1] = 4; opengl_attributes[3] = 4; opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_4_5_CORE:          { opengl_attributes[1] = 4; opengl_attributes[3] = 5; opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_4_5_COMPATIBILITY: { opengl_attributes[1] = 4; opengl_attributes[3] = 5; opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_4_6_CORE:          { opengl_attributes[1] = 4; opengl_attributes[3] = 6; opengl_attributes[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
						case MU_OPENGL_4_6_COMPATIBILITY: { opengl_attributes[1] = 4; opengl_attributes[3] = 6; opengl_attributes[5] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
					}

					*context = wgl->wglCreateContextAttribsARB(dc, 0, opengl_attributes);
					if (!*context) {
						*result = MUCOSA_FAILED_CREATE_OPENGL_CONTEXT;
						return;
					}
					if (!wglMakeCurrent(dc, *context)) {
						*result = MUCOSA_FAILED_LOAD_OPENGL_CONTEXT;
						return;
					}
				}

		#endif /* MUCOSA_OPENGL */

		/* Context */

			struct muCOSA_Win32_context {
				muCOSA_Win32_time time;
			};
			typedef struct muCOSA_Win32_context muCOSA_Win32_context;

			void muCOSA_Win32_context_init(muCOSA_Win32_context* context) {
				muCOSA_Win32_window_pmap_access();
				muCOSA_Win32_time_init(&context->time);
			}

			void muCOSA_Win32_context_term(muCOSA_Win32_context* context) {
				muCOSA_Win32_window_pmap_deaccess();
				return; if (context) {}
			}

		/* API functions */

			/* Window */

				struct muCOSA_Win32_input {
					muButtonState keyboard_key_states[MU_KEYBOARD_KEY_LAST-MU_KEYBOARD_KEY_FIRST+1];
					muState keyboard_state_states[MU_KEYBOARD_STATE_LAST-MU_KEYBOARD_STATE_FIRST+1];
					muButtonState mouse_button_states[MU_MOUSE_BUTTON_LAST-MU_MOUSE_BUTTON_FIRST+1];
				};
				typedef struct muCOSA_Win32_input muCOSA_Win32_input;

				struct muCOSA_Win32_window {
					wchar_t wclass_name[2];
					HINSTANCE hinstance;
					HWND hwnd;
					HDC dc;

					muGraphicsAPI api;
					MUCOSA_OPENGL_CALL(
						HGLRC glrc;
						muCOSA_Win32_WGL wgl;
					)

					muCOSA_Win32_input input;
					void (*text_input_callback)(muWindow window, const char* input);
					int32_m text_cursor_x;
					int32_m text_cursor_y;

					muBool closed;
					muBool visible;
					muBool maximized;
					muBool minimized;

					uint32_m min_width;
					uint32_m min_height;
					uint32_m max_width;
					uint32_m max_height;

					muCursorStyle cursor_style;
					HCURSOR cursor;
					int32_m scroll_level;
					int32_m cx, cy;

					void (*dimensions_callback)(muWindow window, uint32_m width, uint32_m height);
					void (*position_callback)(muWindow window, int32_m x, int32_m y);
					void (*focus_callback)(muWindow window, muBool focused);
					void (*maximize_callback)(muWindow window, muBool maximized);
					void (*minimize_callback)(muWindow window, muBool minimized);

					void (*keyboard_key_callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state);
					void (*keyboard_state_callback)(muWindow window, muKeyboardState keyboard_state, muState state);

					void (*cursor_position_callback)(muWindow window, int32_m x, int32_m y);
					void (*mouse_button_callback)(muWindow window, muMouseButton mouse_button, muButtonState state);
					void (*scroll_callback)(muWindow window, int32_m scroll_level_add);
				};
				typedef struct muCOSA_Win32_window muCOSA_Win32_window;

				/* Input */

					void muCOSA_innerWin32_possible_input_flushing(muCOSA_Win32_window* win) {
						if (GetFocus() != win->hwnd) {
							win->input = MU_ZERO_STRUCT(muCOSA_Win32_input);
						}
					}

					/* Keyboard key */

						WPARAM muCOSA_innerWin32_map_left_right_keys(WPARAM vk, LPARAM lParam) {
							UINT scancode = (lParam & 0x00ff0000) >> 16;
							int extended  = (lParam & 0x01000000) != 0;

							switch (vk) {
								default: return vk; break;
								case VK_SHIFT: return MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX); break;
								case VK_CONTROL: return extended ? VK_RCONTROL : VK_LCONTROL; break;
								case VK_MENU: return extended ? VK_RMENU : VK_LMENU; break;
							}
						}

						muKeyboardKey muCOSA_Win32_key_to_keyboard_key(int key) {
							switch (key) {
								default: return MU_KEYBOARD_KEY_UNKNOWN; break;
								case VK_BACK: return MU_KEYBOARD_KEY_BACKSPACE; break;
								case VK_TAB: return MU_KEYBOARD_KEY_TAB; break;
								case VK_CLEAR: return MU_KEYBOARD_KEY_CLEAR; break;
								case VK_RETURN: return MU_KEYBOARD_KEY_RETURN; break;
								case VK_PAUSE: return MU_KEYBOARD_KEY_PAUSE; break;
								case VK_ESCAPE: return MU_KEYBOARD_KEY_ESCAPE; break;
								case VK_MODECHANGE: return MU_KEYBOARD_KEY_MODECHANGE; break;
								case VK_SPACE: return MU_KEYBOARD_KEY_SPACE; break;
								case VK_PRIOR: return MU_KEYBOARD_KEY_PRIOR; break;
								case VK_NEXT: return MU_KEYBOARD_KEY_NEXT; break;
								case VK_END: return MU_KEYBOARD_KEY_END; break;
								case VK_HOME: return MU_KEYBOARD_KEY_HOME; break;
								case VK_LEFT: return MU_KEYBOARD_KEY_LEFT; break;
								case VK_UP: return MU_KEYBOARD_KEY_UP; break;
								case VK_RIGHT: return MU_KEYBOARD_KEY_RIGHT; break;
								case VK_DOWN: return MU_KEYBOARD_KEY_DOWN; break;
								case VK_SELECT: return MU_KEYBOARD_KEY_SELECT; break;
								case VK_PRINT: return MU_KEYBOARD_KEY_PRINT; break;
								case VK_EXECUTE: return MU_KEYBOARD_KEY_EXECUTE; break;
								case VK_INSERT: return MU_KEYBOARD_KEY_INSERT; break;
								case VK_DELETE: return MU_KEYBOARD_KEY_DELETE; break;
								case VK_HELP: return MU_KEYBOARD_KEY_HELP; break;
								case 0x30: return MU_KEYBOARD_KEY_0; break;
								case 0x31: return MU_KEYBOARD_KEY_1; break;
								case 0x32: return MU_KEYBOARD_KEY_2; break;
								case 0x33: return MU_KEYBOARD_KEY_3; break;
								case 0x34: return MU_KEYBOARD_KEY_4; break;
								case 0x35: return MU_KEYBOARD_KEY_5; break;
								case 0x36: return MU_KEYBOARD_KEY_6; break;
								case 0x37: return MU_KEYBOARD_KEY_7; break;
								case 0x38: return MU_KEYBOARD_KEY_8; break;
								case 0x39: return MU_KEYBOARD_KEY_9; break;
								case 0x41: return MU_KEYBOARD_KEY_A; break;
								case 0x42: return MU_KEYBOARD_KEY_B; break;
								case 0x43: return MU_KEYBOARD_KEY_C; break;
								case 0x44: return MU_KEYBOARD_KEY_D; break;
								case 0x45: return MU_KEYBOARD_KEY_E; break;
								case 0x46: return MU_KEYBOARD_KEY_F; break;
								case 0x47: return MU_KEYBOARD_KEY_G; break;
								case 0x48: return MU_KEYBOARD_KEY_H; break;
								case 0x49: return MU_KEYBOARD_KEY_I; break;
								case 0x4A: return MU_KEYBOARD_KEY_J; break;
								case 0x4B: return MU_KEYBOARD_KEY_K; break;
								case 0x4C: return MU_KEYBOARD_KEY_L; break;
								case 0x4D: return MU_KEYBOARD_KEY_M; break;
								case 0x4E: return MU_KEYBOARD_KEY_N; break;
								case 0x4F: return MU_KEYBOARD_KEY_O; break;
								case 0x50: return MU_KEYBOARD_KEY_P; break;
								case 0x51: return MU_KEYBOARD_KEY_Q; break;
								case 0x52: return MU_KEYBOARD_KEY_R; break;
								case 0x53: return MU_KEYBOARD_KEY_S; break;
								case 0x54: return MU_KEYBOARD_KEY_T; break;
								case 0x55: return MU_KEYBOARD_KEY_U; break;
								case 0x56: return MU_KEYBOARD_KEY_V; break;
								case 0x57: return MU_KEYBOARD_KEY_W; break;
								case 0x58: return MU_KEYBOARD_KEY_X; break;
								case 0x59: return MU_KEYBOARD_KEY_Y; break;
								case 0x5A: return MU_KEYBOARD_KEY_Z; break;
								case VK_LWIN: return MU_KEYBOARD_KEY_LEFT_WINDOWS; break;
								case VK_RWIN: return MU_KEYBOARD_KEY_RIGHT_WINDOWS; break;
								case VK_NUMPAD0: return MU_KEYBOARD_KEY_NUMPAD_0; break;
								case VK_NUMPAD1: return MU_KEYBOARD_KEY_NUMPAD_1; break;
								case VK_NUMPAD2: return MU_KEYBOARD_KEY_NUMPAD_2; break;
								case VK_NUMPAD3: return MU_KEYBOARD_KEY_NUMPAD_3; break;
								case VK_NUMPAD4: return MU_KEYBOARD_KEY_NUMPAD_4; break;
								case VK_NUMPAD5: return MU_KEYBOARD_KEY_NUMPAD_5; break;
								case VK_NUMPAD6: return MU_KEYBOARD_KEY_NUMPAD_6; break;
								case VK_NUMPAD7: return MU_KEYBOARD_KEY_NUMPAD_7; break;
								case VK_NUMPAD8: return MU_KEYBOARD_KEY_NUMPAD_8; break;
								case VK_NUMPAD9: return MU_KEYBOARD_KEY_NUMPAD_9; break;
								case VK_MULTIPLY: return MU_KEYBOARD_KEY_MULTIPLY; break;
								case VK_ADD: return MU_KEYBOARD_KEY_ADD; break;
								case VK_SEPARATOR: return MU_KEYBOARD_KEY_SEPARATOR; break;
								case VK_SUBTRACT: return MU_KEYBOARD_KEY_SUBTRACT; break;
								case VK_DECIMAL: return MU_KEYBOARD_KEY_DECIMAL; break;
								case VK_DIVIDE: return MU_KEYBOARD_KEY_DIVIDE; break;
								case VK_F1: return MU_KEYBOARD_KEY_F1; break;
								case VK_F2: return MU_KEYBOARD_KEY_F2; break;
								case VK_F3: return MU_KEYBOARD_KEY_F3; break;
								case VK_F4: return MU_KEYBOARD_KEY_F4; break;
								case VK_F5: return MU_KEYBOARD_KEY_F5; break;
								case VK_F6: return MU_KEYBOARD_KEY_F6; break;
								case VK_F7: return MU_KEYBOARD_KEY_F7; break;
								case VK_F8: return MU_KEYBOARD_KEY_F8; break;
								case VK_F9: return MU_KEYBOARD_KEY_F9; break;
								case VK_F10: return MU_KEYBOARD_KEY_F10; break;
								case VK_F11: return MU_KEYBOARD_KEY_F11; break;
								case VK_F12: return MU_KEYBOARD_KEY_F12; break;
								case VK_F13: return MU_KEYBOARD_KEY_F13; break;
								case VK_F14: return MU_KEYBOARD_KEY_F14; break;
								case VK_F15: return MU_KEYBOARD_KEY_F15; break;
								case VK_F16: return MU_KEYBOARD_KEY_F16; break;
								case VK_F17: return MU_KEYBOARD_KEY_F17; break;
								case VK_F18: return MU_KEYBOARD_KEY_F18; break;
								case VK_F19: return MU_KEYBOARD_KEY_F19; break;
								case VK_F20: return MU_KEYBOARD_KEY_F20; break;
								case VK_F21: return MU_KEYBOARD_KEY_F21; break;
								case VK_F22: return MU_KEYBOARD_KEY_F22; break;
								case VK_F23: return MU_KEYBOARD_KEY_F23; break;
								case VK_F24: return MU_KEYBOARD_KEY_F24; break;
								case VK_NUMLOCK: return MU_KEYBOARD_KEY_NUMLOCK; break;
								case VK_SCROLL: return MU_KEYBOARD_KEY_SCROLL; break;
								case VK_LSHIFT: return MU_KEYBOARD_KEY_LEFT_SHIFT; break;
								case VK_RSHIFT: return MU_KEYBOARD_KEY_RIGHT_SHIFT; break;
								case VK_LCONTROL: return MU_KEYBOARD_KEY_LEFT_CONTROL; break;
								case VK_RCONTROL: return MU_KEYBOARD_KEY_RIGHT_CONTROL; break;
								case VK_LMENU: return MU_KEYBOARD_KEY_LEFT_MENU; break;
								case VK_RMENU: return MU_KEYBOARD_KEY_RIGHT_MENU; break;
								case VK_ATTN: return MU_KEYBOARD_KEY_ATTN; break;
								case VK_CRSEL: return MU_KEYBOARD_KEY_CRSEL; break;
								case VK_EXSEL: return MU_KEYBOARD_KEY_EXSEL; break;
								case VK_EREOF: return MU_KEYBOARD_KEY_EREOF; break;
								case VK_PLAY: return MU_KEYBOARD_KEY_PLAY; break;
								case VK_PA1: return MU_KEYBOARD_KEY_PA1; break;
							}
						}

					/* Keyboard state */

						int muCOSA_innerWin32_keyboard_state_get_win32(muKeyboardState state) {
							switch (state) {
								default: return VK_NONAME; break;
								case MU_KEYBOARD_STATE_CAPS_LOCK: return VK_CAPITAL; break;
								case MU_KEYBOARD_STATE_SCROLL_LOCK: return VK_SCROLL; break;
								case MU_KEYBOARD_STATE_NUM_LOCK: return VK_NUMLOCK; break;
							}
						}

						void muCOSA_innerWin32_update_keyboard_state(muCOSA_Win32_window* win) {
							for (size_m i = MU_KEYBOARD_STATE_FIRST; i <= MU_KEYBOARD_STATE_LAST; i++) {
								muState state = MU_OFF;
								if ((GetKeyState(muCOSA_innerWin32_keyboard_state_get_win32(i)) & 0x0001) != 0) {
									state = MU_ON;
								}

								if (win->input.keyboard_state_states[i] != state) {
									win->input.keyboard_state_states[i] = state;
									if (win->keyboard_state_callback != 0) {
										win->keyboard_state_callback(win, i, state);
									}
								}
							}
						}

					/* Mouse buttons */

						// ...

					/* Cursor */

						/* Conversion */

							void* muCOSA_Win32_cursor_to_win_cursor(muCursorStyle style) {
								switch (style) {
									default: case MU_CURSOR_STYLE_ARROW: return IDC_ARROW; break;
									case MU_CURSOR_STYLE_IBEAM: return IDC_IBEAM; break;
									case MU_CURSOR_STYLE_WAIT: return IDC_WAIT; break;
									case MU_CURSOR_STYLE_WAIT_ARROW: return IDC_APPSTARTING; break;
									case MU_CURSOR_STYLE_CROSSHAIR: return IDC_CROSS; break;
									case MU_CURSOR_STYLE_HAND: return IDC_HAND; break;
									case MU_CURSOR_STYLE_SIZE_EAST_WEST: return IDC_SIZEWE; break;
									case MU_CURSOR_STYLE_SIZE_NORTH_SOUTH: return IDC_SIZENS; break;
									case MU_CURSOR_STYLE_SIZE_NORTH_EAST_SOUTH_WEST: return IDC_SIZENESW; break;
									case MU_CURSOR_STYLE_SIZE_NORTH_WEST_SOUTH_EAST: return IDC_SIZENWSE; break;
									case MU_CURSOR_STYLE_SIZE_ALL: return IDC_SIZEALL; break;
									case MU_CURSOR_STYLE_NO: return IDC_NO; break;
								}
							}

						/* Position */

							void muCOSA_Win32_window_get_position(muCOSAResult* result, muCOSA_Win32_window* win, int32_m* x, int32_m* y);
							void muCOSA_Win32_window_get_cursor_position(muCOSAResult* result, muCOSA_Win32_window* win, int32_m* x, int32_m* y) {
								POINT p;
								if (GetCursorPos(&p) == 0) {
									MU_SET_RESULT(result, MUCOSA_FAILED_QUERY_WINDOW_INFO)
									return;
								}

								int32_m wx=0, wy=0;
								muCOSAResult res = MUCOSA_SUCCESS;
								muCOSA_Win32_window_get_position(&res, win, &wx, &wy);
								if (res != MUCOSA_SUCCESS) {
									MU_SET_RESULT(result, res)
									return;
								}

								MU_SET_RESULT(x, p.x-wx)
								MU_SET_RESULT(y, p.y-wy)
							}

							void muCOSA_Win32_window_set_cursor_position(muCOSAResult* result, muCOSA_Win32_window* win, int32_m x, int32_m y) {
								int32_m wx=0, wy=0;
								muCOSAResult res = MUCOSA_SUCCESS;
								muCOSA_Win32_window_get_position(&res, win, &wx, &wy);
								if (res != MUCOSA_SUCCESS) {
									MU_SET_RESULT(result, res)
									return;
								}

								if (SetCursorPos(wx+x, wy+y) == 0) {
									MU_SET_RESULT(result, MUCOSA_FAILED_SET_WINDOW_INFO)
								}
							}

							void muCOSA_innerWin32_handle_cursor_position(muCOSA_Win32_window* win) {
								muCOSAResult result = MUCOSA_SUCCESS;

								int32_m cx=0, cy=0;
								muCOSA_Win32_window_get_cursor_position(&result, win, &cx, &cy);

								if (result == MUCOSA_SUCCESS && (cx != win->cx || cy != win->cy)) {
									win->cx = cx;
									win->cy = cy;

									if (win->cursor_position_callback != 0) {
										win->cursor_position_callback(win, win->cx, win->cy);
									}
								}
							}

						/* Style */

							muCursorStyle muCOSA_Win32_window_get_cursor_style(muCOSA_Win32_window* win) {
								return win->cursor_style;
							}

							void muCOSA_Win32_window_set_cursor_style(muCOSA_Win32_window* win, muCursorStyle style) {
								win->cursor_style = style;
								if (win->cursor != NULL) {
									DestroyCursor(win->cursor);
								}
								win->cursor = LoadCursor(0, (LPCSTR)muCOSA_Win32_cursor_to_win_cursor(style));
								SetCursor(win->cursor);
							}

							void muCOSA_Win32_window_get_dimensions(muCOSAResult* result, muCOSA_Win32_window* win, uint32_m* width, uint32_m* height);
							// This doesn't work... :L
							void muCOSA_innerWin32_update_cursor_style(muCOSA_Win32_window* win) {
								muCOSAResult result = MUCOSA_SUCCESS;
								return;

								int32_m cx=0,cy=0;
								muCOSA_Win32_window_get_cursor_position(&result, win, &cx, &cy);
								if (result != MUCOSA_SUCCESS) {
									return;
								}
								uint32_m ucx=cx, ucy=cy;

								uint32_m ww=0,wh=0;
								muCOSA_Win32_window_get_dimensions(&result, win, &ww, &wh);
								if (result != MUCOSA_SUCCESS) {
									return;
								}

								if (win->cursor == NULL) {
									if (ucx < ww && ucy < wh && cx > 0 && cy > 0) {
										muCOSA_Win32_window_set_cursor_style(win, win->cursor_style);
									}
								} else {
									if (ucx >= ww || ucy >= wh || cx <= 0 || cy <= 0) {
										DestroyCursor(win->cursor);
										win->cursor = NULL;
									}
								}
							}

						/* Scroll level */

							int32_m muCOSA_Win32_window_get_scroll_level(muCOSA_Win32_window* win) {
								return win->scroll_level;
							}

							void muCOSA_Win32_window_set_scroll_level(muCOSA_Win32_window* win, int32_m scroll_level) {
								win->scroll_level = scroll_level;
							}

					/* Proc */

						struct muCOSA_Win32_msginfo {
							UINT uMsg;
							WPARAM wParam;
							LPARAM lParam;

							muCOSA_Win32_window* win;
						};
						typedef struct muCOSA_Win32_msginfo muCOSA_Win32_msginfo;

						LRESULT CALLBACK muCOSA_innerWin32_handle_quit(muCOSA_Win32_msginfo msg) {
							PostQuitMessage(0);
							msg.win->closed = MU_TRUE;
							return 0;
						}

						LRESULT CALLBACK muCOSA_innerWin32_handle_size(muCOSA_Win32_msginfo msg) {
							if (msg.win->dimensions_callback != 0) {
								msg.win->dimensions_callback(msg.win, (int)LOWORD(msg.lParam), (int)HIWORD(msg.lParam));
							}

							if (msg.wParam == SIZE_MAXIMIZED && msg.win->maximize_callback != 0) {
								msg.win->maximized = MU_TRUE;
								msg.win->maximize_callback(msg.win, MU_TRUE);
							// ?
							} else if (msg.win->maximized && msg.wParam != SIZE_MAXIMIZED && msg.wParam != SIZE_MAXHIDE && msg.wParam != SIZE_MAXSHOW && msg.win->maximize_callback != 0) {
								msg.win->maximized = MU_FALSE;
								msg.win->maximize_callback(msg.win, MU_FALSE);
							}

							if (msg.wParam == SIZE_MINIMIZED && msg.win->minimize_callback != 0) {
								msg.win->minimized = MU_TRUE;
								msg.win->minimize_callback(msg.win, MU_TRUE);
							// ?
							} else if (msg.win->minimized
								&& msg.wParam != SIZE_MINIMIZED && msg.wParam != SIZE_MAXHIDE && msg.wParam != SIZE_MAXSHOW
								&& msg.win->minimize_callback != 0) {
								msg.win->minimized = MU_FALSE;
								msg.win->minimize_callback(msg.win, MU_FALSE);
							}

							PostMessage(msg.win->hwnd, WM_PAINT, 0, 0);
							return 0;
						}

						LRESULT CALLBACK muCOSA_innerWin32_handle_key(muCOSA_Win32_msginfo msg, muButtonState state) {
							msg.wParam = muCOSA_innerWin32_map_left_right_keys(msg.wParam, msg.lParam);
							muKeyboardKey key = muCOSA_Win32_key_to_keyboard_key(msg.wParam);
							if (key == MU_KEYBOARD_KEY_UNKNOWN) {
								return 0;
							}

							msg.win->input.keyboard_key_states[key-MU_KEYBOARD_KEY_FIRST] = state;

							if (msg.win->keyboard_key_callback != MU_NULL_PTR) {
								msg.win->keyboard_key_callback(msg.win, key, state);
							}

							return 0;
						}

						LRESULT CALLBACK muCOSA_innerWin32_handle_move(muCOSA_Win32_msginfo msg) {
							if (msg.win->position_callback != 0) {
								// https://www.autohotkey.com/boards/viewtopic.php?t=27857
								// BILL GATES... MY LOVE... YOU MAKE NO MISTAKES IN YOUR FLAWLESS,
								// IMPECCABLE API. HOW COULD AN API GET BETTER IF THE DOCUMENTATION FOR IT
								// IS UNIVERSALLY UNDERSTOOD AS BEING COMPLETELY WORTHLESS? I'D LET YOU
								// FORCE UPDATE A MILLION TIMES FOR YOUR PERFECT API... Oh, Bill...
								msg.win->position_callback(msg.win,
									(int32_m)( msg.lParam      & 0x8000 ? - ((~msg.lParam    )&0x7FFF)+1 :  msg.lParam     &0x7FFF),
									(int32_m)((msg.lParam>>16) & 0x8000 ? - ((~msg.lParam>>16)&0x7FFF)+1 : (msg.lParam>>16)&0x7FFF)
								);
							}

							return 0;
						}

						LRESULT CALLBACK muCOSA_innerWin32_handle_focus(muCOSA_Win32_msginfo msg, muBool focus) {
							if (msg.win->focus_callback != 0) {
								msg.win->focus_callback(msg.win, focus);
							}

							return 0;
						}

						LRESULT CALLBACK muCOSA_innerWin32_handle_minmax_info(muCOSA_Win32_msginfo msg) {
							LPMINMAXINFO lp = (LPMINMAXINFO)msg.lParam;
							// No idea why 16 and 39 need to be added here. It can't be broder stuff like
							// the titlebar, because the border width is almost never 16 whole pixels in
							// Windows. :|
							lp->ptMinTrackSize.x = msg.win->min_width + 16;
							lp->ptMinTrackSize.y = msg.win->min_height + 39;
							lp->ptMaxTrackSize.x = msg.win->max_width + 16;
							lp->ptMaxTrackSize.y = msg.win->max_height + 39;

							return 0;
						}

						// Note: button and state must be valid, this func doesn't perform checks
						LRESULT CALLBACK muCOSA_innerWin32_handle_mouse_button(muCOSA_Win32_msginfo msg, muMouseButton button, muButtonState state) {
							msg.win->input.mouse_button_states[button-MU_MOUSE_BUTTON_FIRST] = state;

							if (msg.win->mouse_button_callback != 0) {
								msg.win->mouse_button_callback(msg.win, button, state);
							}

							return 0;
						}

						LRESULT CALLBACK muCOSA_innerWin32_handle_set_cursor(muCOSA_Win32_msginfo msg) {
							if (LOWORD(msg.lParam) == HTCLIENT && msg.win->cursor_style != MU_CURSOR_STYLE_DEFAULT) {
								DestroyCursor(msg.win->cursor);
								msg.win->cursor = LoadCursor(0, (LPCSTR)muCOSA_Win32_cursor_to_win_cursor(msg.win->cursor_style));
								SetCursor(msg.win->cursor);
								return TRUE;
							}

							return 0;
						}

						LRESULT CALLBACK muCOSA_innerWin32_handle_mouse_wheel(muCOSA_Win32_msginfo msg) {
							msg.win->scroll_level += GET_WHEEL_DELTA_WPARAM(msg.wParam);

							if (msg.win->scroll_callback != 0) {
								msg.win->scroll_callback(msg.win, GET_WHEEL_DELTA_WPARAM(msg.wParam));
							}

							return 0;
						}

						size_m muCOSA_innerWin32_UTF8_get_code_point_size(size_m code_point) {
							// Storable in 7 bits = 1 byte
							if (code_point < 128) {
								return 1;
							}
							// Storable in 11 bits = 2 bytes
							if (code_point < 2048) {
								return 2;
							}
							// Storable in 16 bits = 3 bytes
							if (code_point < 65536) {
								return 3;
							}
							return 4;
						}

						void muCOSA_innerWin32_UTF8_set_code_point(size_m code_point, muByte* data) {
							size_m code_point_size = muCOSA_innerWin32_UTF8_get_code_point_size(code_point);

							switch (code_point_size) { default: return; break;
								case 1: {
									data[0] = code_point;
								} break;

								case 2: {
									// 110xxxxx
									data[0] = (muByte)(((code_point >> 6) & 31) | 192);
									// 10xxxxxx
									data[1] = (muByte)((code_point & 63) | 128);
								} break;

								case 3: {
									// 1110xxxx
									data[0] = (muByte)(((code_point >> 12) & 15) | 224);
									// 10xxxxxx
									data[1] = (muByte)(((code_point >> 6) & 63) | 128);
									// 10xxxxxx
									data[2] = (muByte)((code_point & 63) | 128);
								} break;

								case 4: {
									// 11110xxx
									data[0] = (muByte)(((code_point >> 18) & 7) | 240);
									// 10xxxxxx
									data[1] = (muByte)(((code_point >> 12) & 63) | 128);
									// 10xxxxxx
									data[2] = (muByte)(((code_point >> 6) & 63) | 128);
									// 10xxxxxx
									data[3] = (muByte)((code_point & 63) | 128);
								} break;
							}
						}

						// This code assumes that wParam is the Unicode codepoint for the character, which
						// I'm pretty sure it is.
						LRESULT CALLBACK muCOSA_innerWin32_handle_wmchar(muCOSA_Win32_msginfo msg) {
							if (msg.win->text_input_callback == 0) {
								return DefWindowProcW(msg.win->hwnd, msg.uMsg, msg.wParam, msg.lParam);
							}

							size_m codepoint = (size_m)msg.wParam;

							size_m codepoint_size = muCOSA_innerWin32_UTF8_get_code_point_size(codepoint);
							codepoint_size += 1; // For \0

							muByte buf[6]; mu_memset(buf, 0, sizeof(buf));
							muByte* input = 0;

							if (codepoint_size > sizeof(buf)) {
								input = (muByte*)mu_malloc(codepoint_size);
								if (input == 0) {
									return 0;
								}
							} else {
								input = buf;
							}

							input[codepoint_size-1] = 0;

							muCOSA_innerWin32_UTF8_set_code_point(codepoint, buf);

							msg.win->text_input_callback(msg.win, (const char*)input);
							if (input != buf) {
								mu_free(input);
							}

							return 0;
						}

						LRESULT CALLBACK muCOSA_innerWin32_handle_umsg(muCOSA_Win32_msginfo msg) {
							switch (msg.uMsg) {
								default: return DefWindowProcW(msg.win->hwnd, msg.uMsg, msg.wParam, msg.lParam); break;
								case WM_DESTROY: return muCOSA_innerWin32_handle_quit(msg); break;

								case WM_SIZE: return muCOSA_innerWin32_handle_size(msg); break;

								case WM_KEYDOWN: return muCOSA_innerWin32_handle_key(msg, MU_BUTTON_STATE_HELD); break;
								case WM_KEYUP: return muCOSA_innerWin32_handle_key(msg, MU_BUTTON_STATE_RELEASED); break;

								case WM_MOVE: return muCOSA_innerWin32_handle_move(msg); break;

								case WM_SETFOCUS: return muCOSA_innerWin32_handle_focus(msg, MU_TRUE); break;
								case WM_KILLFOCUS: return muCOSA_innerWin32_handle_focus(msg, MU_FALSE); break;

								case WM_GETMINMAXINFO: return muCOSA_innerWin32_handle_minmax_info(msg); break;

								case WM_LBUTTONDOWN: return muCOSA_innerWin32_handle_mouse_button(msg, MU_MOUSE_BUTTON_LEFT, MU_BUTTON_STATE_HELD); break;
								case WM_RBUTTONDOWN: return muCOSA_innerWin32_handle_mouse_button(msg, MU_MOUSE_BUTTON_RIGHT, MU_BUTTON_STATE_HELD); break;
								case WM_LBUTTONUP: return muCOSA_innerWin32_handle_mouse_button(msg, MU_MOUSE_BUTTON_LEFT, MU_BUTTON_STATE_RELEASED); break;
								case WM_RBUTTONUP: return muCOSA_innerWin32_handle_mouse_button(msg, MU_MOUSE_BUTTON_RIGHT, MU_BUTTON_STATE_RELEASED); break;

								case WM_SETCURSOR: return muCOSA_innerWin32_handle_set_cursor(msg); break;

								case WM_MOUSEWHEEL: return muCOSA_innerWin32_handle_mouse_wheel(msg); break;

								case WM_CHAR: return muCOSA_innerWin32_handle_wmchar(msg); break;
							}
						}

						LRESULT CALLBACK muCOSA_Win32_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
							muCOSA_Win32_window_pmap_lock();

							muCOSA_Win32_window* win = 0;
							for (size_m i = 0; i < muCOSA_Win32_global_window_pmap.pointer_count; i++) {
								if (muCOSA_Win32_global_window_pmap.pointers[i] != 0) {
									if (((muCOSA_Win32_window*)muCOSA_Win32_global_window_pmap.pointers[i])->hwnd == hwnd) {
										win = (muCOSA_Win32_window*)muCOSA_Win32_global_window_pmap.pointers[i];
									}
								}
							}

							muCOSA_Win32_window_pmap_unlock();

							if (win == 0) {
								return DefWindowProcW(hwnd, uMsg, wParam, lParam);
							}

							muCOSA_Win32_msginfo msginfo = MU_ZERO_STRUCT(muCOSA_Win32_msginfo);
							msginfo.uMsg = uMsg;
							msginfo.wParam = wParam;
							msginfo.lParam = lParam;
							msginfo.win = win;
							LRESULT ret = muCOSA_innerWin32_handle_umsg(msginfo);

							return ret;
						}

				/* Creation / Destruction */

					void muCOSA_Win32_window_create(muCOSAResult* result, muGraphicsAPI api, muBool (*load_functions)(void), const char* name, uint16_m width, uint16_m height, muWindowCreateInfo create_info, muCOSA_Win32_window* win) {

						muCOSAResult res = MUCOSA_SUCCESS;

						if (!muCOSA_Win32_window_pmap_add(win, sizeof(muCOSA_Win32_window*))) {
							MU_SET_RESULT(result, MUCOSA_ALLOCATION_FAILED)
							return;
						}

						/* Set default attributes */

							win->closed = MU_FALSE;
							win->maximized = create_info.maximized;
							win->minimized = create_info.minimized;
							win->dimensions_callback = create_info.dimensions_callback;
							win->position_callback = create_info.position_callback;
							win->focus_callback = create_info.focus_callback;
							win->maximize_callback = create_info.maximize_callback;
							win->minimize_callback = create_info.minimize_callback;
							win->keyboard_key_callback = create_info.keyboard_key_callback;
							win->keyboard_state_callback = create_info.keyboard_state_callback;
							win->cursor_position_callback = create_info.cursor_position_callback;
							win->mouse_button_callback = create_info.mouse_button_callback;
							win->scroll_callback = create_info.scroll_callback;
							win->min_width = create_info.min_width;
							win->min_height = create_info.min_height;
							win->max_width = create_info.max_width;
							win->max_height = create_info.max_height;
							win->input = MU_ZERO_STRUCT(muCOSA_Win32_input);
							win->text_cursor_x = 0;
							win->text_cursor_y = 0;
							win->text_input_callback = 0;

						/* Class */

							wchar_t* wname = muCOSA_Win32_utf8_to_wchar(name);
							if (!wname) {
								MU_SET_RESULT(result, MUCOSA_FAILED_CONVERT_UTF8_TO_WCHAR)
								return;
							}

							// This might be a bad idea... :)
							win->wclass_name[0] = (wchar_t)'!';

							wchar_t add;
							if (sizeof(wchar_t) < sizeof(win)) {
								mu_memcpy(&add, &win, sizeof(wchar_t));
							} else {
								mu_memcpy(&add, &win, sizeof(win));
							}
							win->wclass_name[0] += add;

							win->wclass_name[1] = (wchar_t)'\0';

							win->hinstance = muCOSA_Win32_get_hinstance();

							WNDCLASSEXW wclass = MU_ZERO_STRUCT(WNDCLASSEXW);
							wclass.cbSize = sizeof(wclass);
							wclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
							wclass.lpfnWndProc = muCOSA_Win32_proc;
							wclass.hInstance = win->hinstance;
							wclass.lpszMenuName = wname;
							wclass.lpszClassName = win->wclass_name;

							if (!RegisterClassExW(&wclass)) {
								MU_SET_RESULT(result, MUCOSA_FAILED_REGISTER_WINDOW_CLASS)
								mu_free(wname);
								return;
							}

						/* Window */

							DWORD style = WS_OVERLAPPEDWINDOW;
							if (!create_info.resizable) {
								style = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
							}

							RECT r;
							r.left = r.top = 0;
							r.right = (LONG)width;
							r.bottom = (LONG)height;
							AdjustWindowRect(&r, style, FALSE);

							win->hwnd = CreateWindowExW(0, wclass.lpszClassName, wclass.lpszMenuName, style, (int)create_info.x, (int)create_info.y, r.right-r.left, r.bottom-r.top, NULL, NULL, win->hinstance, NULL);

							// ?
							mu_free(wname);

							if (win->hwnd == NULL) {
								MU_SET_RESULT(result, MUCOSA_FAILED_CREATE_WINDOW)
								UnregisterClassW(win->wclass_name, win->hinstance);
								return;
							}

							win->dc = GetDC(win->hwnd);

						/* API */

							win->api = api;

							MUCOSA_OPENGL_CALL(
								if (api >= MUCOSA_OPENGL_FIRST && api <= MUCOSA_OPENGL_LAST) {
									muCOSA_Win32_init_opengl_extensions(&res, &win->wgl);
									if (res != MUCOSA_SUCCESS) {
										MU_SET_RESULT(result, res)
										ReleaseDC(win->hwnd, win->dc);
										DestroyWindow(win->hwnd);
										UnregisterClassW(win->wclass_name, win->hinstance);
										return;
									}

									muCOSA_Win32_create_opengl_context(&res, win->dc, &win->wgl, create_info.pixel_format, &win->glrc, api);
									if (res != MUCOSA_SUCCESS) {
										MU_SET_RESULT(result, res)
										ReleaseDC(win->hwnd, win->dc);
										DestroyWindow(win->hwnd);
										UnregisterClassW(win->wclass_name, win->hinstance);
										return;
									}
								}
							)

							if (load_functions != 0) {
								if (!load_functions()) {
									MU_SET_RESULT(result, MUCOSA_FAILED_LOAD_FUNCTIONS)
									MUCOSA_OPENGL_CALL(
										if (api >= MUCOSA_OPENGL_FIRST && api <= MUCOSA_OPENGL_LAST) {
											wglDeleteContext(win->glrc);
										}
									)
									ReleaseDC(win->hwnd, win->dc);
									DestroyWindow(win->hwnd);
									UnregisterClassW(win->wclass_name, win->hinstance);
									return;
								}
							}

						/* Cursor */

							win->cursor_style = create_info.cursor_style;
							win->cursor = LoadCursor(0, (LPCSTR)muCOSA_Win32_cursor_to_win_cursor(create_info.cursor_style));
							SetCursor(win->cursor);
							win->scroll_level = 0;
							win->cx = 0;
							win->cy = 0;

						/* Show window */

							win->visible = create_info.visible;

							if (win->visible) {
								if (create_info.maximized) {
									ShowWindow(win->hwnd, SW_MAXIMIZE);
								} else if (create_info.minimized) {
									ShowWindow(win->hwnd, SW_MINIMIZE);
								} else {
									ShowWindow(win->hwnd, SW_NORMAL);
								}
							} else {
								ShowWindow(win->hwnd, SW_HIDE);
							}

						/* Auto-disable IME */

							if (ImmAssociateContextEx(win->hwnd, NULL, 0) == FALSE) {
								// ？
							}

						/* Return */

							return; if (res) {}
					}

					void muCOSA_Win32_window_destroy(muCOSA_Win32_window* win) {
						muCOSA_Win32_window_pmap_remove(win, sizeof(muCOSA_Win32_window*));

						MUCOSA_OPENGL_CALL(
							if (win->api >= MUCOSA_OPENGL_FIRST && win->api <= MUCOSA_OPENGL_LAST) {
								wglDeleteContext(win->glrc);
							}
						)

						ReleaseDC(win->hwnd, win->dc);
						DestroyWindow(win->hwnd);
						UnregisterClassW(win->wclass_name, win->hinstance);
					}

				/* Main loop */

					muBool muCOSA_Win32_window_get_closed(muCOSA_Win32_window* win) {
						return win->closed;
					}

					void muCOSA_Win32_window_close(muCOSA_Win32_window* win) {
						win->closed = MU_TRUE;
					}

					void muCOSA_Win32_window_update(muCOSA_Win32_window* win) {
						MSG msg = MU_ZERO_STRUCT(MSG);
						while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}

						muCOSA_innerWin32_update_keyboard_state(win);
						muCOSA_innerWin32_possible_input_flushing(win);

						muCOSA_innerWin32_handle_cursor_position(win);
						muCOSA_innerWin32_update_cursor_style(win);
					}

					void muCOSA_Win32_window_swap_buffers(muCOSA_Win32_window* win) {
						MUCOSA_OPENGL_CALL(
							if (win->api >= MUCOSA_OPENGL_FIRST && win->api <= MUCOSA_OPENGL_LAST) {
								SwapBuffers(win->dc);
							}
						)
						return; if (win) {}
					}

				/* Get / Set */

					muBool muCOSA_Win32_window_get_focused(muCOSA_Win32_window* win) {
						return GetFocus() == win->hwnd;
					}

					muBool muCOSA_Win32_window_get_minimized(muCOSA_Win32_window* win);
					void muCOSA_Win32_window_set_minimized(muCOSAResult* result, muCOSA_Win32_window* win, muBool minimized);
					void muCOSA_Win32_window_focus(muCOSA_Win32_window* win) {
						// Attempt to restore the window if it is minimized.
						if (muCOSA_Win32_window_get_minimized(win)) {
							muCOSA_Win32_window_set_minimized(0, win, MU_FALSE);
						}

						// https://stackoverflow.com/questions/71437203/proper-way-of-activating-a-window-using-winapi
						// This can be made more consistent with the automation API, but I don't want
						// to touch that with a 51!-foot pole.
						SetForegroundWindow(win->hwnd);
						if (GetForegroundWindow() != win->hwnd) {
							SwitchToThisWindow(win->hwnd, MU_TRUE);
							Sleep(2); // I would kiss Bill Gates on the mouth if I ever met him.
							SetForegroundWindow(win->hwnd);
						}
					}

					muBool muCOSA_Win32_window_get_visible(muCOSA_Win32_window* win) {
						return win->visible;
					}

					void muCOSA_Win32_window_set_visible(muCOSA_Win32_window* win, muBool visible) {
						if (!visible && win->visible) {
							ShowWindow(win->hwnd, SW_HIDE);
						} else if (visible && !win->visible) {
							ShowWindow(win->hwnd, SW_NORMAL);
						}

						win->visible = visible;
					}

					void muCOSA_Win32_window_get_frame_extents(muCOSAResult* result, muCOSA_Win32_window* win, uint32_m* l, uint32_m* r, uint32_m* t, uint32_m* b);
					void muCOSA_Win32_window_get_position(muCOSAResult* result, muCOSA_Win32_window* win, int32_m* x, int32_m* y) {
						RECT rect;
						if (GetWindowRect(win->hwnd, &rect) == 0) {
							MU_SET_RESULT(result, MUCOSA_FAILED_QUERY_WINDOW_INFO)
							return;
						}

						muCOSAResult res = MUCOSA_SUCCESS;
						uint32_m l=0, t=0;
						muCOSA_Win32_window_get_frame_extents(&res, win, &l, 0, &t, 0);
						if (res != MUCOSA_SUCCESS) {
							MU_SET_RESULT(result, res)
							return;
						}

						MU_SET_RESULT(x, (int32_m)rect.left + (int32_m)l)
						MU_SET_RESULT(y, (int32_m)rect.top + (int32_m)t)
					}

					void muCOSA_Win32_window_set_position(muCOSAResult* result, muCOSA_Win32_window* win, int32_m x, int32_m y) {
						muCOSAResult res = MUCOSA_SUCCESS;
						uint32_m xborder=0, yborder=0;
						muCOSA_Win32_window_get_frame_extents(&res, win, &xborder, 0, &yborder, 0);
						if (res != MUCOSA_SUCCESS) {
							MU_SET_RESULT(result, res)
							return;
						}

						x -= (int32_m)xborder;
						y -= (int32_m)yborder;

						if (SetWindowPos(win->hwnd, HWND_TOP, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE) == 0) {
							MU_SET_RESULT(result, MUCOSA_FAILED_SET_WINDOW_INFO)
						}
					}

					void muCOSA_Win32_window_get_dimensions(muCOSAResult* result, muCOSA_Win32_window* win, uint32_m* width, uint32_m* height) {
						RECT rect;
						if (GetClientRect(win->hwnd, &rect) == 0) {
							MU_SET_RESULT(result, MUCOSA_FAILED_QUERY_WINDOW_INFO)
							return;
						}

						MU_SET_RESULT(width, (uint32_m)(rect.right - rect.left))
						MU_SET_RESULT(height, (uint32_m)(rect.bottom - rect.top))
					}

					void muCOSA_Win32_window_set_dimensions(muCOSAResult* result, muCOSA_Win32_window* win, uint32_m width, uint32_m height) {
						WINDOWINFO wi;
						if (GetWindowInfo(win->hwnd, &wi) == 0) {
							MU_SET_RESULT(result, MUCOSA_FAILED_QUERY_WINDOW_INFO)
							return;
						}

						RECT rect;
						rect.left = rect.top = 0;
						rect.right = width;
						rect.bottom = height;
						if (AdjustWindowRect(&rect, wi.dwStyle, FALSE) == 0) {
							MU_SET_RESULT(result, MUCOSA_FAILED_SET_WINDOW_INFO)
							return;
						}

						if (SetWindowPos(win->hwnd, HWND_TOP, 0, 0, rect.right-rect.left, rect.bottom-rect.top, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE) == 0) {
							MU_SET_RESULT(result, MUCOSA_FAILED_SET_WINDOW_INFO)
						}
					}

					muBool muCOSA_Win32_window_get_maximized(muCOSAResult* result, muCOSA_Win32_window* win) {
						WINDOWPLACEMENT wp = MU_ZERO_STRUCT(WINDOWPLACEMENT);
						wp.length = sizeof(WINDOWPLACEMENT);
						if (GetWindowPlacement(win->hwnd, &wp) == 0) {
							MU_SET_RESULT(result, MUCOSA_FAILED_QUERY_WINDOW_INFO)
							return MU_FALSE;
						}

						return wp.showCmd == SW_MAXIMIZE;
					}

					void muCOSA_Win32_window_set_maximized(muCOSA_Win32_window* win, muBool maximized) {
						muCOSAResult res = MUCOSA_SUCCESS;
						muBool b_maximized = muCOSA_Win32_window_get_maximized(&res, win);

						if (res == MUCOSA_SUCCESS && b_maximized == maximized) {
							return;
						}

						if (maximized) {
							ShowWindow(win->hwnd, SW_MAXIMIZE);
						} else {
							ShowWindow(win->hwnd, SW_NORMAL);
						}
					}

					muBool muCOSA_Win32_window_get_minimized(muCOSA_Win32_window* win) {
						return IsIconic(win->hwnd);
					}

					void muCOSA_Win32_window_set_minimized(muCOSAResult* result, muCOSA_Win32_window* win, muBool minimized) {
						muBool b_minimized = muCOSA_Win32_window_get_minimized(win);
						if (b_minimized == minimized) {
							return;
						}

						if (minimized) {
							ShowWindow(win->hwnd, SW_MINIMIZE);
						} else {
							if (OpenIcon(win->hwnd) == 0) {
								MU_SET_RESULT(result, MUCOSA_FAILED_SET_WINDOW_INFO)
							}
						}
					}

					void muCOSA_Win32_window_get_minimum_dimensions(muCOSA_Win32_window* win, uint32_m* min_width, uint32_m* min_height) {
						MU_SET_RESULT(min_width, win->min_width)
						MU_SET_RESULT(min_height, win->min_height)
					}

					void muCOSA_Win32_window_set_minimum_dimensions(muCOSA_Win32_window* win, uint32_m min_width, uint32_m min_height) {
						win->min_width = min_width;
						win->min_height = min_height;
					}

					void muCOSA_Win32_window_get_maximum_dimensions(muCOSA_Win32_window* win, uint32_m* max_width, uint32_m* max_height) {
						MU_SET_RESULT(max_width, win->max_width)
						MU_SET_RESULT(max_height, win->max_height)
					}

					void muCOSA_Win32_window_set_maximum_dimensions(muCOSA_Win32_window* win, uint32_m max_width, uint32_m max_height) {
						win->max_width = max_width;
						win->max_height = max_height;
					}

				/* Get / Let */

					void muCOSA_Win32_window_update_text_cursor(muCOSA_Win32_window* win, int32_m x, int32_m y);
					void muCOSA_Win32_window_get_text_input_focus(muCOSAResult* result, muCOSA_Win32_window* win, int32_m text_cursor_x, int32_m text_cursor_y, void (*callback)(muWindow window, const char* input)) {
						if (ImmAssociateContextEx(win->hwnd, NULL, IACE_DEFAULT) == FALSE) {
							MU_SET_RESULT(result, MUCOSA_FAILED_ASSOCIATE_IME_CONTEXT)
							return;
						}

						muCOSA_Win32_window_update_text_cursor(win, text_cursor_x, text_cursor_y);
						win->text_input_callback = callback;
					}

					void muCOSA_Win32_window_let_text_input_focus(muCOSA_Win32_window* win) {
						win->text_input_callback = 0;

						if (ImmAssociateContextEx(win->hwnd, NULL, 0) == FALSE) {
							// ?
						}
					}

					void muCOSA_Win32_window_update_text_cursor(muCOSA_Win32_window* win, int32_m x, int32_m y) {
						if (win->text_input_callback == 0) {
							return;
						}

						// Maybe flickering is caused by context getting retrieved and released per-update...
						// @TODO Check
						HIMC imm = ImmGetContext(win->hwnd);

						COMPOSITIONFORM cf = MU_ZERO_STRUCT(COMPOSITIONFORM);
						cf.dwStyle = CFS_FORCE_POSITION;
						cf.ptCurrentPos.x = x;
						cf.ptCurrentPos.y = y;
						ImmSetCompositionWindow(imm, &cf);

						if (ImmReleaseContext(win->hwnd, imm) == 0) {
							// ?
						}
					}

				/* Get */

					// I LOVE YOU BILL GATES!!!!
					void muCOSA_Win32_window_get_frame_extents(muCOSAResult* result, muCOSA_Win32_window* win, uint32_m* l, uint32_m* r, uint32_m* t, uint32_m* b) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						// (Fallback)
						muCOSA_Win32_window_get_def_frame_extents(l, r, t, b);

						RECT wrect, crect;
						if (GetWindowRect(win->hwnd, &wrect) == 0) {
							MU_SET_RESULT(result, MUCOSA_FAILED_QUERY_WINDOW_INFO)
							return;
						}
						if (GetClientRect(win->hwnd, &crect) == 0) {
							MU_SET_RESULT(result, MUCOSA_FAILED_QUERY_WINDOW_INFO)
							return;
						}
						MapWindowPoints(win->hwnd, NULL, (LPPOINT)&crect, 2);

						MU_SET_RESULT(l, (uint32_m)(crect.left-wrect.left))
						MU_SET_RESULT(r, (uint32_m)(wrect.right-crect.right))
						MU_SET_RESULT(t, (uint32_m)(crect.top-wrect.top))
						MU_SET_RESULT(b, (uint32_m)(wrect.bottom-crect.bottom))

						/*LONG styles = GetWindowLongA(win->hwnd, GWL_STYLE);
						LONG exstyles = GetWindowLongA(win->hwnd, GWL_EXSTYLE);

						RECT rect;
						if (AdjustWindowRectEx(&rect, styles, FALSE, exstyles) == 0) {
							MU_SET_RESULT(result, MUCOSA_FAILED_QUERY_WINDOW_INFO)
							return;
						}

						MU_SET_RESULT(l, rect.left)
						MU_SET_RESULT(r, rect.right)
						MU_SET_RESULT(t, rect.top)
						MU_SET_RESULT(b, rect.bottom)*/
					}

					muButtonState muCOSA_Win32_window_get_keyboard_key_state(muCOSA_Win32_window* win, muKeyboardKey key) {
						return win->input.keyboard_key_states[key-MU_KEYBOARD_KEY_FIRST];
					}

					muState muCOSA_Win32_window_get_keyboard_state_state(muCOSA_Win32_window* win, muKeyboardState state) {
						return (GetKeyState(muCOSA_innerWin32_keyboard_state_get_win32(state)) & 0x0001) != 0;
						if (win) {}
					}

					muButtonState muCOSA_Win32_window_get_mouse_button_state(muCOSA_Win32_window* win, muMouseButton button) {
						return win->input.mouse_button_states[button-MU_MOUSE_BUTTON_FIRST];
					}

				/* Set */

					void muCOSA_Win32_window_set_title(muCOSAResult* result, muCOSA_Win32_window* win, const char* title) {
						wchar_t* wtitle = muCOSA_Win32_utf8_to_wchar(title);
						if (wtitle == 0) {
							MU_SET_RESULT(result, MUCOSA_ALLOCATION_FAILED)
							return;
						}

						if (SetWindowTextW(win->hwnd, wtitle) == 0) {
							MU_SET_RESULT(result, MUCOSA_FAILED_SET_WINDOW_INFO)
						}

						mu_free(wtitle);
					}

					void muCOSA_Win32_window_set_dimensions_callback(muCOSA_Win32_window* win, void (*callback)(muWindow window, uint32_m width, uint32_m height)) {
						win->dimensions_callback = callback;
					}

					void muCOSA_Win32_window_set_position_callback(muCOSA_Win32_window* win, void (*callback)(muWindow window, int32_m x, int32_m y)) {
						win->position_callback = callback;
					}

					void muCOSA_Win32_window_set_focus_callback(muCOSA_Win32_window* win, void (*callback)(muWindow window, muBool focused)) {
						win->focus_callback = callback;
					}

					void muCOSA_Win32_window_set_maximize_callback(muCOSA_Win32_window* win, void (*callback)(muWindow window, muBool maximized)) {
						win->maximize_callback = callback;
					}

					void muCOSA_Win32_window_set_minimize_callback(muCOSA_Win32_window* win, void (*callback)(muWindow window, muBool minimized)) {
						win->minimize_callback = callback;
					}

					void muCOSA_Win32_window_set_keyboard_key_callback(muCOSA_Win32_window* win, void (*callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state)) {
						win->keyboard_key_callback = callback;
					}

					void muCOSA_Win32_window_set_keyboard_state_callback(muCOSA_Win32_window* win, void (*callback)(muWindow window, muKeyboardState keyboard_state, muState state)) {
						win->keyboard_state_callback = callback;
					}

					void muCOSA_Win32_window_set_cursor_position_callback(muCOSA_Win32_window* win, void (*callback)(muWindow window, int32_m x, int32_m y)) {
						win->cursor_position_callback = callback;
					}

					void muCOSA_Win32_window_set_mouse_button_callback(muCOSA_Win32_window* win, void (*callback)(muWindow window, muMouseButton mouse_button, muButtonState state)) {
						win->mouse_button_callback = callback;
					}

					void muCOSA_Win32_window_set_scroll_callback(muCOSA_Win32_window* win, void (*callback)(muWindow window, int32_m scroll_level_add)) {
						win->scroll_callback = callback;
					}

			/* Time */

				double muCOSA_Win32_time_get(muCOSA_Win32_context* context, muCOSAResult* result) {
					return muCOSA_Win32_get_current_time() - context->time.orig_time;
					if (result) {}
				}

				void muCOSA_Win32_time_set(muCOSA_Win32_context* context, muCOSAResult* result, double time) {
					context->time.orig_time = muCOSA_Win32_get_current_time() - time;
					return; if (result) {}
				}

				void muCOSA_Win32_sleep(muCOSA_Win32_context* context, muCOSAResult* result, double time) {
					Sleep((DWORD)((time*(double)1000.f)));
					return; if (context) {} if (result) {}
				}

			/* Clipboard */

				char* muCOSA_Win32_clipboard_get(muCOSAResult* result) {
					if (OpenClipboard(NULL) == 0) {
						MU_SET_RESULT(result, MUCOSA_FAILED_HOLD_CLIPBOARD)
						return 0;
					}

					HANDLE data = GetClipboardData(CF_UNICODETEXT);
					if (data == 0) {
						// I'm pretty sure this can happen if no clipboard is available, so I'm not
						// throwing an error here.
						CloseClipboard();
						return 0;
					}

					wchar_t* wdata = (wchar_t*)GlobalLock(data);
					if (wdata == NULL) {
						MU_SET_RESULT(result, MUCOSA_FAILED_GLOBAL_LOCK)
						CloseClipboard();
						return 0;
					}

					muByte* utf8_data = muCOSA_Win32_wchar_to_utf8(wdata);
					if (utf8_data == 0) {
						MU_SET_RESULT(result, MUCOSA_FAILED_CONVERT_WCHAR_TO_UTF8)
						GlobalUnlock(data);
						CloseClipboard();
						return 0;
					}

					return (char*)utf8_data;
				}

				void muCOSA_Win32_clipboard_set(muCOSAResult* result, const char* text, size_m text_size) {
					int wlen = MultiByteToWideChar(CP_UTF8, 0, text, text_size, NULL, 0);
					if (wlen == 0) {
						MU_SET_RESULT(result, MUCOSA_FAILED_CONVERT_UTF8_TO_WCHAR)
						return;
					}

					HGLOBAL g_mem = GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)wlen * sizeof(wchar_t));
					if (g_mem == NULL) {
						MU_SET_RESULT(result, MUCOSA_FAILED_GLOBAL_ALLOCATION)
						return;
					}

					LPVOID p_mem = GlobalLock(g_mem);
					if (p_mem == NULL) {
						MU_SET_RESULT(result, MUCOSA_FAILED_GLOBAL_LOCK)
						GlobalFree(g_mem);
						return;
					}

					if (MultiByteToWideChar(CP_UTF8, 0, text, text_size, (LPWSTR)p_mem, wlen) == 0) {
						MU_SET_RESULT(result, MUCOSA_FAILED_CONVERT_UTF8_TO_WCHAR)
						GlobalUnlock(g_mem);
						GlobalFree(g_mem);
						return;
					}

					GlobalUnlock(g_mem);

					if (OpenClipboard(NULL) == 0) {
						MU_SET_RESULT(result, MUCOSA_FAILED_HOLD_CLIPBOARD)
						GlobalFree(g_mem);
						return;
					}

					if (EmptyClipboard() == 0) {
						// I'm going to assume that this can occur if the clipboard was already
						// empty, so I don't wanna throw an error here. But if that's not the case,
						// this could cause a bad memory leak. :L
					}

					// Is CF_UNICODETEXT right here? Pretty sure it is...
					if (SetClipboardData(CF_UNICODETEXT, g_mem) == NULL) {
						MU_SET_RESULT(result, MUCOSA_FAILED_SET_CLIPBOARD)
						GlobalFree(g_mem);
						CloseClipboard();
						return;
					}

					CloseClipboard();
				}

			/* OS functions */

				void* muCOSA_Win32_os_get_window_handle(muCOSAResult* result, muCOSA_Win32_window* win, muWindowHandle handle) {
					void* p = 0;
					switch (handle) {
						default: MU_SET_RESULT(result, MUCOSA_UNKNOWN_WINDOW_HANDLE) break;
						case MU_WINDOWS_HWND: p = (void*)&win->hwnd; break;
					}

					return p;
				}

			/* OpenGL */

				void muCOSA_Win32_opengl_bind_window(muCOSAResult* result, muCOSA_Win32_window* win) {
					#ifndef MUCOSA_OPENGL
						MU_SET_RESULT(result, MUCOSA_UNSUPPORTED_GRAPHICS_API)
						return;
						if (win) {}
					#else
						if (win->api >= MUCOSA_OPENGL_FIRST && win->api <= MUCOSA_OPENGL_LAST) {
							if (wglMakeCurrent(win->dc, win->glrc) != TRUE) {
								MU_SET_RESULT(result, MUCOSA_FAILED_LOAD_OPENGL_CONTEXT)
								return;
							}
						}
					#endif
				}

				void* muCOSA_Win32_opengl_get_function_address(const char* name) {
					MUCOSA_OPENGL_CALL(
						PROC p = (PROC)wglGetProcAddress(name);

						if (p == 0 || (p == (PROC)0x1) || (p == (PROC)0x2) || (p == (PROC)0x3) || (p == (PROC)-1)) {
							HMODULE module = LoadLibraryA("opengl32.dll");
							p = (PROC)GetProcAddress(module, name);
						}

						void* vptr = 0;
						mu_memcpy(&vptr, &p, sizeof(void*));
						return vptr;
					)
					if (name) {}
					return 0;
				}

				void muCOSA_Win32_opengl_window_swap_interval(muCOSAResult* result, muCOSA_Win32_window* win, int interval) {
					#ifndef MUCOSA_OPENGL
						MU_SET_RESULT(result, MUCOSA_UNSUPPORTED_GRAPHICS_API)
						return;
						if (win) {} if (interval) {}
					#else
						// Check if extension "wglGetExtensionsStringEXT" is present
						if (win->wgl.wglSwapIntervalEXT == 0) {
							void* vptr = muCOSA_Win32_opengl_get_function_address("wglGetExtensionsStringEXT");

							if (vptr == 0) {
								MU_SET_RESULT(result, MUCOSA_UNSUPPORTED_OPENGL_FEATURE)
								return;
							}

							// @TODO Store this within the window probably lol
							vptr = muCOSA_Win32_opengl_get_function_address("wglSwapIntervalEXT");
							mu_memcpy(&win->wgl.wglSwapIntervalEXT, &vptr, sizeof(void*));
						}

						if (win->wgl.wglSwapIntervalEXT == 0) {
							return;
						}

						win->wgl.wglSwapIntervalEXT(interval);
					#endif
				}

			/* Vulkan */

				#if defined(MUCOSA_VULKAN) && !defined(MUCOSA_NO_INCLUDE_VULKAN)
					#include MUCOSA_VULKAN_INCLUDE_PATH
				#endif

				const char** muCOSA_Win32_vulkan_get_surface_instance_extensions(muCOSAResult* result, size_m* count) {
					#ifndef MUCOSA_VULKAN
						MU_SET_RESULT(result, MUCOSA_UNSUPPORTED_GRAPHICS_API)
						return 0;
						if (count) {}
					#else
						// I'm pretty sure this is good...
						static const char* exts[] = { "VK_KHR_surface", "VK_KHR_win32_surface" };
						MU_SET_RESULT(count, 2)
						return (const char**)exts;
						if (result) {}
					#endif
				}

				void muCOSA_Win32_vulkan_create_window_surface(muCOSAResult* result, muCOSA_Win32_window* win, void* vk_result, void* instance, void* allocator, void* surface) {
					#ifndef MUCOSA_VULKAN
						MU_SET_RESULT(result, MUCOSA_UNSUPPORTED_GRAPHICS_API)
						return;
						if (win) {} if (vk_result) {} if (instance) {} if (allocator) {} if (surface) {}
					#else
						VkWin32SurfaceCreateInfoKHR ci = MU_ZERO_STRUCT(VkWin32SurfaceCreateInfoKHR);
						ci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
						ci.hinstance = win->hinstance;
						ci.hwnd = win->hwnd;

						VkInstance r_instance = VK_NULL_HANDLE;
						if (instance != 0) {
							r_instance = *((VkInstance*)instance);
						}

						VkResult vkres = vkCreateWin32SurfaceKHR(r_instance, &ci, (const VkAllocationCallbacks*)allocator, (VkSurfaceKHR*)surface);
						if (vk_result != 0) {
							*((VkResult*)vk_result) = vkres;
						}

						return; if (result) {}
					#endif
				}

	#else

		#define MUCOSA_WIN32_CALL(...)

	#endif /* MUCOSA_WIN32 */

	/* X11 */

	#ifdef MUCOSA_X11

		#define MUCOSA_X11_CALL(...) __VA_ARGS__

		#include <X11/Xlib.h> // Includes <X11/X.h>
		#include <X11/Xutil.h> // Must be included AFTER <X11/Xlib.h>
		#include <X11/XKBlib.h> // For XkbKeycodeToKeysym
		#ifndef MUCOSA_NO_LOCALE_MOD
			#include <X11/Xlocale.h> // For XSetLocaleModifiers
		#endif

		#include <time.h>
		#include <pthread.h>
		#include <unistd.h> // For nanosleep (and sleep as a backup because Linux is Linux)

		/* OpenGL */

		#ifdef MUCOSA_OPENGL

			#ifndef MUCOSA_NO_INCLUDE_OPENGL
				// There's gotta be a better way to do this lmao
				#ifndef glClearColor
					#include <GL/gl.h>
				#endif
			#endif

			#include <GL/glx.h>

			struct muCOSA_X11_gl_attributes {
				GLint att[25];
			};
			typedef struct muCOSA_X11_gl_attributes muCOSA_X11_gl_attributes;

			muCOSA_X11_gl_attributes muCOSA_X11_get_gl_attributes(muPixelFormat pf) {
				muCOSA_X11_gl_attributes a = MU_ZERO_STRUCT(muCOSA_X11_gl_attributes);
				a.att[0] = GLX_X_RENDERABLE; a.att[1] = True;
				a.att[2] = GLX_DRAWABLE_TYPE; a.att[3] = GLX_WINDOW_BIT;
				a.att[4] = GLX_RENDER_TYPE; a.att[5] = GLX_RGBA_BIT;
				a.att[6] = GLX_X_VISUAL_TYPE; a.att[7] = GLX_TRUE_COLOR; // ?
				a.att[8] = GLX_RED_SIZE; a.att[9] = pf.red_bits;
				a.att[10] = GLX_GREEN_SIZE; a.att[11] = pf.green_bits;
				a.att[12] = GLX_BLUE_SIZE; a.att[13] = pf.blue_bits;
				a.att[14] = GLX_ALPHA_SIZE; a.att[15] = pf.alpha_bits;
				a.att[16] = GLX_DEPTH_SIZE; a.att[17] = pf.depth_bits;
				a.att[18] = GLX_STENCIL_SIZE; a.att[19] = pf.stencil_bits;
				a.att[20] = GLX_DOUBLEBUFFER; a.att[21] = True;
				a.att[22] = GLX_SAMPLES; a.att[23] = pf.samples;
				a.att[24] = None;
				return a;
			}

			// Note: per-window
			// https://apoorvaj.io/creating-a-modern-opengl-context/
			muCOSAResult muCOSA_X11_init_opengl(Display* display, GLXContext* context, muGraphicsAPI api, GLXFBConfig fbc) {
				GLXContext (*glXCreateContextAttribsARB)(Display*, GLXFBConfig, GLXContext, Bool, const int*) = 
					(GLXContext (*)(Display*, GLXFBConfig, GLXContext, Bool, const int*))glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
				if (!glXCreateContextAttribsARB) {
					return MUCOSA_FAILED_CREATE_OPENGL_CONTEXT;
				}

				int opengl_attributes[] = {
					GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
					GLX_CONTEXT_MINOR_VERSION_ARB, 3,
					GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
					None
				};

				switch (api) {
					default: return MUCOSA_UNKNOWN_GRAPHICS_API; break;
					case MU_OPENGL_1_0:   { opengl_attributes[1] = 1; opengl_attributes[3] = 0; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
					case MU_OPENGL_1_1:   { opengl_attributes[1] = 1; opengl_attributes[3] = 1; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
					case MU_OPENGL_1_2:   { opengl_attributes[1] = 1; opengl_attributes[3] = 2; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
					// No idea what to do here lmao
					case MU_OPENGL_1_2_1: { opengl_attributes[1] = 1; opengl_attributes[3] = 2; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
					case MU_OPENGL_1_3:   { opengl_attributes[1] = 1; opengl_attributes[3] = 3; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
					case MU_OPENGL_1_4:   { opengl_attributes[1] = 1; opengl_attributes[3] = 4; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
					case MU_OPENGL_1_5:   { opengl_attributes[1] = 1; opengl_attributes[3] = 5; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
					case MU_OPENGL_2_0:   { opengl_attributes[1] = 2; opengl_attributes[3] = 0; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
					case MU_OPENGL_2_1:   { opengl_attributes[1] = 2; opengl_attributes[3] = 1; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
					case MU_OPENGL_3_0:   { opengl_attributes[1] = 3; opengl_attributes[3] = 0; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
					case MU_OPENGL_3_1:   { opengl_attributes[1] = 3; opengl_attributes[3] = 1; opengl_attributes[4] = 0; opengl_attributes[5] = 0; } break;
					case MU_OPENGL_3_2_CORE:          { opengl_attributes[1] = 3; opengl_attributes[3] = 2; opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_3_2_COMPATIBILITY: { opengl_attributes[1] = 3; opengl_attributes[3] = 2; opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_3_3_CORE:          { opengl_attributes[1] = 3; opengl_attributes[3] = 3; opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_3_3_COMPATIBILITY: { opengl_attributes[1] = 3; opengl_attributes[3] = 3; opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_4_0_CORE:          { opengl_attributes[1] = 4; opengl_attributes[3] = 0; opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_4_0_COMPATIBILITY: { opengl_attributes[1] = 4; opengl_attributes[3] = 0; opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_4_1_CORE:          { opengl_attributes[1] = 4; opengl_attributes[3] = 1; opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_4_1_COMPATIBILITY: { opengl_attributes[1] = 4; opengl_attributes[3] = 1; opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_4_2_CORE:          { opengl_attributes[1] = 4; opengl_attributes[3] = 2; opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_4_2_COMPATIBILITY: { opengl_attributes[1] = 4; opengl_attributes[3] = 2; opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_4_3_CORE:          { opengl_attributes[1] = 4; opengl_attributes[3] = 3; opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_4_3_COMPATIBILITY: { opengl_attributes[1] = 4; opengl_attributes[3] = 3; opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_4_4_CORE:          { opengl_attributes[1] = 4; opengl_attributes[3] = 4; opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_4_4_COMPATIBILITY: { opengl_attributes[1] = 4; opengl_attributes[3] = 4; opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_4_5_CORE:          { opengl_attributes[1] = 4; opengl_attributes[3] = 5; opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_4_5_COMPATIBILITY: { opengl_attributes[1] = 4; opengl_attributes[3] = 5; opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_4_6_CORE:          { opengl_attributes[1] = 4; opengl_attributes[3] = 6; opengl_attributes[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB; } break;
					case MU_OPENGL_4_6_COMPATIBILITY: { opengl_attributes[1] = 4; opengl_attributes[3] = 6; opengl_attributes[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB; } break;
				}

				*context = glXCreateContextAttribsARB(display, fbc, MU_NULL, MU_TRUE, opengl_attributes);
				if (!*context) {
					return MUCOSA_FAILED_CREATE_OPENGL_CONTEXT;
				}

				return MUCOSA_SUCCESS;
			}

		#endif /* MUCOSA_OPENGL */

		/* Time */

			/* Setup */

				struct muCOSA_X11_time {
					double orig_time;
				};
				typedef struct muCOSA_X11_time muCOSA_X11_time;

				double muCOSA_X11_get_system_time(void);
				void muCOSA_X11_time_init(muCOSA_X11_time* time) {
					time->orig_time = muCOSA_X11_get_system_time();
				}

			/* Important functions */

				// https://stackoverflow.com/questions/3756323/how-to-get-the-current-time-in-milliseconds-from-c-in-linux
				double muCOSA_X11_get_system_time(void) {
					struct timespec spec;
					clock_gettime(CLOCK_REALTIME, &spec);
					return (double)((double)(spec.tv_sec) + ((double)(spec.tv_nsec) / (double)(1.0e9)));
				}

				double muCOSA_X11_time_get(muCOSA_X11_time* time) {
					return muCOSA_X11_get_system_time() - time->orig_time;
				}

				void muCOSA_X11_time_set(muCOSA_X11_time* time, double dtime) {
					time->orig_time = muCOSA_X11_get_system_time() - dtime;
				}

				void muCOSA_X11_sleep(double time) {
					// https://stackoverflow.com/a/1157217

					struct timespec ts;
					ts.tv_sec = (time_t)time;
					ts.tv_nsec = (long)(time * (double)1e+9);

					mu_fflush(mu_stdout);
					int n = nanosleep(&ts, NULL);

					// Dunno about you guys, but nanosleep literally just doesn't work on my system.
					// Literally, it always returns -1, I have never gotten it to work. YAY!
					// - Yup, doesn't work on mine. Maybe cuz of Mint? :L
					if (n != 0) {
						sleep((unsigned)time);
					}
				}

		/* Clipboard */

			/* Setup */

				struct muCOSA_X11_clipboard {
					pthread_t thread;
					muBool thread_exists;
					muBool thread_running;
					muByte* text;
					size_m size;
				};
				typedef struct muCOSA_X11_clipboard muCOSA_X11_clipboard;

				void muCOSA_X11_clipboard_init(muCOSA_X11_clipboard* clipboard) {
					clipboard->thread_exists = MU_FALSE;
					clipboard->thread_running = MU_FALSE;
					clipboard->text = 0;
				}

				void muCOSA_X11_clipboard_term(muCOSA_X11_clipboard* clipboard) {
					// Kill thread
					if (clipboard->thread_exists) {
						if (clipboard->thread_running) {
							// Note: this essentially acts as the kill signal to the clipboard thread; it will
							// recognize that this is now false and exit soon after, so we can safely join.
							clipboard->thread_running = MU_FALSE;
							pthread_join(clipboard->thread, 0);
						}
						pthread_cancel(clipboard->thread);
						clipboard->thread_exists = MU_FALSE;
					}
					// Free text
					if (clipboard->text) {
						mu_free(clipboard->text);
						clipboard->text = 0;
						clipboard->size = 0;
					}
				}

			/* Important functions */

				// Note: this is meant to be used as the function for the thread.
				// p = (muCOSA_X11_clipboard*)
				void* muCOSA_X11_thread_clipboard_set(void* p) {
					muCOSA_X11_clipboard* c = (muCOSA_X11_clipboard*)p;
					Display* d = XOpenDisplay(NULL);
					Window w = XCreateSimpleWindow(d, RootWindow(d, DefaultScreen(d)), -10, -10, 1, 1, 0, 0, 0);

					Atom utf8 = XInternAtom(d, "UTF8_STRING", False);
					XSetSelectionOwner(d, XInternAtom(d, "CLIPBOARD", False), w, CurrentTime);

					XEvent ev;
					muBool got_event = MU_FALSE;
					while (!got_event && c->thread_running) {
						while (!XPending(d)) {
							if (!c->thread_running) {
								XCloseDisplay(d);
								return 0;
							}
							// Yuck.
							muCOSA_X11_sleep(0.05);
						}
						XNextEvent(d, &ev);
						switch (ev.type) {
							default: break;

							/* Ownership loss */
							case SelectionClear: {
								got_event = MU_TRUE;
							} break;

							/* Clipboard request */
							case SelectionRequest: {
								XSelectionRequestEvent* sev = (XSelectionRequestEvent*)&ev.xselectionrequest;

								if (sev->target != utf8 || sev->property == None) {
									// "Can't give it to you, mate, I only got UTF-8."
									char* an = XGetAtomName(d, sev->target);
									if (an) {
										XFree(an);
									}

									XSelectionEvent ssev = MU_ZERO_STRUCT(XSelectionEvent);
									ssev.type = SelectionNotify;
									ssev.requestor = sev->requestor;
									ssev.selection = sev->selection;
									ssev.target = sev->target;
									ssev.property = None;
									ssev.time = sev->time;
									XSendEvent(d, sev->requestor, True, NoEventMask, (XEvent*)&ssev);
								} else {
									// "Right, here ya go."
									char* an = XGetAtomName(d, sev->property);
									if (an) {
										XFree(an);
									}
									XChangeProperty(d, sev->requestor, sev->property, utf8, 8, PropModeReplace,
										(unsigned char*)c->text, c->size
									);

									XSelectionEvent ssev = MU_ZERO_STRUCT(XSelectionEvent);
									ssev.type = SelectionNotify;
									ssev.requestor = sev->requestor;
									ssev.selection = sev->selection;
									ssev.target = sev->target;
									ssev.property = sev->property;
									ssev.time = sev->time;
									XSendEvent(d, sev->requestor, True, NoEventMask, (XEvent*)&ssev);
								}
							} break;
						}
					}

					XCloseDisplay(d);
					c->thread_running = MU_FALSE;
					return 0;
				}

				void muCOSA_X11_clipboard_set(muCOSAResult* result, muCOSA_X11_clipboard* clipboard, const char* text, size_m size) {
					muCOSA_X11_clipboard_term(clipboard);

					clipboard->text = (muByte*)mu_malloc(size);
					if (!clipboard->text) {
						MU_SET_RESULT(result, MUCOSA_ALLOCATION_FAILED)
						return;
					}
					mu_memcpy(clipboard->text, text, size);
					clipboard->size = size;

					clipboard->thread_running = MU_TRUE;
					if (pthread_create(&clipboard->thread, NULL, muCOSA_X11_thread_clipboard_set, (void*)clipboard) != 0) {
						MU_SET_RESULT(result, MUCOSA_FAILED_CREATE_THREAD)
						clipboard->thread_running = MU_FALSE;
						return;
					}

					clipboard->thread_exists = MU_TRUE;
				}

				// https://www.uninformativ.de/blog/postings/2017-04-02/0/POSTING-en.html

				// I kinda like that this function is so independent... it's nice. Still ugly, tho.
				muByte* muCOSA_X11_clipboard_get(muCOSAResult* result) {
					Display* d = XOpenDisplay(NULL);
					Window w = XCreateSimpleWindow(d, RootWindow(d, DefaultScreen(d)), -10, -10, 1, 1, 0, 0, 0);

					Atom sel = XInternAtom(d, "CLIPBOARD", False);
					Window owner = XGetSelectionOwner(d, sel);
					if (owner == None) {
						return 0;
					}

					// ?
					Atom p = XInternAtom(d, "PENGUIN", False);
					XConvertSelection(d, sel, XInternAtom(d, "UTF8_STRING", False), p, w, CurrentTime);

					XEvent ev;
					muBool got_notify = MU_FALSE;
					muByte* data = 0;
					while (!got_notify) {
						XNextEvent(d, &ev);
						switch (ev.type) {
							default: break;

							case SelectionNotify: {
								got_notify = MU_TRUE;
								XSelectionEvent* sev = (XSelectionEvent*)&ev.xselection;
								if (sev->property == None) {
									return 0;
								} else {
									Atom type;
									int di;
									unsigned long dul, size;
									unsigned char* prop_ret = 0;
									XGetWindowProperty(d, w, p, 0, 0, False, AnyPropertyType, &type, &di, &dul, &size, &prop_ret);
									XFree(prop_ret);

									if (type == XInternAtom(d, "INCR", False)) {
										MU_SET_RESULT(result, MUCOSA_OVERSIZED_CLIPBOARD)
										return 0;
									}
									if (size == 0) {
										return 0;
									}

									Atom da;
									XGetWindowProperty(d, w, p, 0, size, False, AnyPropertyType, &da, &di, &dul, &dul, &prop_ret);
									XDeleteProperty(d, w, p);
									data = (muByte*)prop_ret;
								}
							} break;
						}
					}

					XCloseDisplay(d);
					return data;
				}

		/* Context */

			struct muCOSA_X11_context {
				muCOSA_X11_time time;
				muCOSA_X11_clipboard clipboard;
			};
			typedef struct muCOSA_X11_context muCOSA_X11_context;

			void muCOSA_X11_context_init(muCOSA_X11_context* context) {
				muCOSA_X11_time_init(&context->time);
				muCOSA_X11_clipboard_init(&context->clipboard);
			}

			void muCOSA_X11_context_term(muCOSA_X11_context* context) {
				muCOSA_X11_clipboard_term(&context->clipboard);
			}

		/* Window */

			/* Setup */

				struct muCOSA_X11_input {
					muButtonState keyboard_key_states[MU_KEYBOARD_KEY_LAST-MU_KEYBOARD_KEY_FIRST+1];
					muState keyboard_state_states[MU_KEYBOARD_STATE_LAST-MU_KEYBOARD_STATE_FIRST+1];
					muButtonState mouse_button_states[MU_MOUSE_BUTTON_LAST-MU_MOUSE_BUTTON_FIRST+1];
				};
				typedef struct muCOSA_X11_input muCOSA_X11_input;

				struct muCOSA_X11_ic {
					muBool active;
					void (*callback)(muWindow window, const char* input);
					XIM im;
					XIC ic;
				};
				typedef struct muCOSA_X11_ic muCOSA_X11_ic;

				struct muCOSA_X11_window {
					XSizeHints* size_hints;
					Display* display;

					Window parent_window;
					Window window;

					muGraphicsAPI api;
					MUCOSA_OPENGL_CALL(
						GLXContext gl_context;
					)

					muCursorStyle cursor_style;
					Cursor cursor;

					Atom delete_atom;

					muCOSA_X11_input input;
					muCOSA_X11_ic ic;

					muBool closed;
					muBool visible;
					muBool resizable;
					uint32_m min_width; uint32_m min_height;
					uint32_m max_width; uint32_m max_height;
					int32_m scroll_level;
					int32_m x; int32_m y;
					muBool minimized;
					muBool maximized;
					void (*dimensions_callback)(muWindow window, uint32_m width, uint32_m height);
					void (*position_callback)(muWindow window, int32_m x, int32_m y);
					void (*focus_callback)(muWindow window, muBool focused);
					void (*maximize_callback)(muWindow window, muBool maximized);
					void (*minimize_callback)(muWindow window, muBool minimized);
					void (*keyboard_key_callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state);
					void (*keyboard_state_callback)(muWindow window, muKeyboardState keyboard_state, muState state);
					void (*cursor_position_callback)(muWindow window, int32_m x, int32_m y);
					void (*mouse_button_callback)(muWindow window, muMouseButton mouse_button, muButtonState state);
					void (*scroll_callback)(muWindow window, int32_m scroll_level_add);
				};
				typedef struct muCOSA_X11_window muCOSA_X11_window;

				/* IC */

					/* Event handling */

						void muCOSA_X11_handle_text_input_event(muCOSA_X11_window* win, XKeyEvent* p_kev) {
							// 12 should be enough for UTF-8
							char utf8_buf[12]; mu_memset(utf8_buf, 0, sizeof(utf8_buf));
							Status status = 0;
							KeySym ksym;
							size_m n = Xutf8LookupString(win->ic.ic, p_kev, utf8_buf, sizeof(utf8_buf)-1, &ksym, &status);

							if (status == XBufferOverflow) {
								// :P
								return;
							}

							if (n && win->ic.callback) {
								win->ic.callback(win, utf8_buf);
							}
						}

					/* Get / Let */

						void muCOSA_X11_window_update_text_cursor(muCOSA_X11_window* win, int32_m x, int32_m y) {
							if (!win->ic.active) {
								return;
							}
							XPoint point = { (short)x, (short)y };
							XVaNestedList icval = XVaCreateNestedList(0, XNSpotLocation, &point, NULL);
							if (icval) {
								XSetICValues(win->ic.ic, XNPreeditAttributes, icval, NULL);
								XFree(icval);
							}
						}

						void muCOSA_X11_window_get_text_input_focus(muCOSAResult* result, muCOSA_X11_window* win, int32_m x, int32_m y, void (*callback)(muWindow window, const char* input)) {
							if (win->ic.active) {
								MU_SET_RESULT(result, MUCOSA_ALREADY_ACTIVE)
								return;
							}

							// https://handmade.network/forums/articles/t/2834-tutorial_a_tour_through_xlib_and_related_technologies
							win->ic.im = XOpenIM(win->display, 0, 0, 0);
							if (!win->ic.im) {
								MU_SET_RESULT(result, MUCOSA_FAILED_CREATE_INPUT_METHOD)
								return;
							}

							XIMStyles* styles = 0;
							if (XGetIMValues(win->ic.im, XNQueryInputStyle, &styles, NULL) || !styles) {
								MU_SET_RESULT(result, MUCOSA_FAILED_GET_INPUT_STYLES)
								XCloseIM(win->ic.im);
								return;
							}

							XIMStyle style = 0;
							for (size_m i = 0; i < styles->count_styles; i++) {
								if (styles->supported_styles[i] == (XIMPreeditNothing | XIMStatusNothing)) {
									style = styles->supported_styles[i];
									break;
								}
							}
							XFree(styles);

							if (!style) {
								MU_SET_RESULT(result, MUCOSA_FAILED_FIND_COMPATIBLE_INPUT_STYLE)
								XCloseIM(win->ic.im);
								return;
							}

							win->ic.ic = XCreateIC(win->ic.im, XNInputStyle, style, XNClientWindow, win->window, NULL);
							if (!win->ic.ic) {
								MU_SET_RESULT(result, MUCOSA_FAILED_CREATE_INPUT_CONTEXT)
								XCloseIM(win->ic.im);
								return;
							}
							XSetICFocus(win->ic.ic);

							muCOSA_X11_window_update_text_cursor(win, x, y);
							win->ic.callback = callback;
							win->ic.active = MU_TRUE;
						}

						void muCOSA_X11_window_let_text_input_focus(muCOSA_X11_window* win) {
							if (!win->ic.active) {
								return;
							}

							XDestroyIC(win->ic.ic);
							XCloseIM(win->ic.im);

							win->ic.active = MU_FALSE;
							win->ic.callback = 0;
						}

				/* Input */

					/* Keyboard key */

						/* Useful functions */

							// https://www.cl.cam.ac.uk/~mgk25/ucs/keysymdef.h
								#define MUCOSA_X11_XK_VoidSymbol 0xffffff
								#define MUCOSA_X11_XK_BackSpace 0xff08
								#define MUCOSA_X11_XK_Tab 0xff09
								#define MUCOSA_X11_XK_Clear 0xff0b
								#define MUCOSA_X11_XK_Return 0xff0d
								#define MUCOSA_X11_XK_Pause 0xff13
								#define MUCOSA_X11_XK_Escape 0xff1b
								#define MUCOSA_X11_XK_Mode_switch 0xff7e
								#define MUCOSA_X11_XK_space 0x0020
								#define MUCOSA_X11_XK_Prior 0xff55
								#define MUCOSA_X11_XK_Next 0xff56
								#define MUCOSA_X11_XK_End 0xff57
								#define MUCOSA_X11_XK_Home 0xff50
								#define MUCOSA_X11_XK_Left 0xff51
								#define MUCOSA_X11_XK_Up 0xff52
								#define MUCOSA_X11_XK_Right 0xff53
								#define MUCOSA_X11_XK_Down 0xff54
								#define MUCOSA_X11_XK_Select 0xff60
								#define MUCOSA_X11_XK_Print 0xff61
								#define MUCOSA_X11_XK_Execute 0xff62
								#define MUCOSA_X11_XK_Insert 0xff63
								#define MUCOSA_X11_XK_Delete 0xffff
								#define MUCOSA_X11_XK_Help 0xff6a
								#define MUCOSA_X11_XK_0 0x0030
								#define MUCOSA_X11_XK_1 0x0031
								#define MUCOSA_X11_XK_2 0x0032
								#define MUCOSA_X11_XK_3 0x0033
								#define MUCOSA_X11_XK_4 0x0034
								#define MUCOSA_X11_XK_5 0x0035
								#define MUCOSA_X11_XK_6 0x0036
								#define MUCOSA_X11_XK_7 0x0037
								#define MUCOSA_X11_XK_8 0x0038
								#define MUCOSA_X11_XK_9 0x0039
								#define MUCOSA_X11_XK_a 0x0061
								#define MUCOSA_X11_XK_b 0x0062
								#define MUCOSA_X11_XK_c 0x0063
								#define MUCOSA_X11_XK_d 0x0064
								#define MUCOSA_X11_XK_e 0x0065
								#define MUCOSA_X11_XK_f 0x0066
								#define MUCOSA_X11_XK_g 0x0067
								#define MUCOSA_X11_XK_h 0x0068
								#define MUCOSA_X11_XK_i 0x0069
								#define MUCOSA_X11_XK_j 0x006a
								#define MUCOSA_X11_XK_k 0x006b
								#define MUCOSA_X11_XK_l 0x006c
								#define MUCOSA_X11_XK_m 0x006d
								#define MUCOSA_X11_XK_n 0x006e
								#define MUCOSA_X11_XK_o 0x006f
								#define MUCOSA_X11_XK_p 0x0070
								#define MUCOSA_X11_XK_q 0x0071
								#define MUCOSA_X11_XK_r 0x0072
								#define MUCOSA_X11_XK_s 0x0073
								#define MUCOSA_X11_XK_t 0x0074
								#define MUCOSA_X11_XK_u 0x0075
								#define MUCOSA_X11_XK_v 0x0076
								#define MUCOSA_X11_XK_w 0x0077
								#define MUCOSA_X11_XK_x 0x0078
								#define MUCOSA_X11_XK_y 0x0079
								#define MUCOSA_X11_XK_z 0x007a
								#define MUCOSA_X11_XK_Super_L 0xffeb
								#define MUCOSA_X11_XK_Super_R 0xffec
								#define MUCOSA_X11_XK_KP_0 0xffb0
								#define MUCOSA_X11_XK_KP_1 0xffb1
								#define MUCOSA_X11_XK_KP_2 0xffb2
								#define MUCOSA_X11_XK_KP_3 0xffb3
								#define MUCOSA_X11_XK_KP_4 0xffb4
								#define MUCOSA_X11_XK_KP_5 0xffb5
								#define MUCOSA_X11_XK_KP_6 0xffb6
								#define MUCOSA_X11_XK_KP_7 0xffb7
								#define MUCOSA_X11_XK_KP_8 0xffb8
								#define MUCOSA_X11_XK_KP_9 0xffb9
								#define MUCOSA_X11_XK_KP_Multiply 0xffaa
								#define MUCOSA_X11_XK_KP_Add 0xffab
								#define MUCOSA_X11_XK_KP_Separator 0xffac
								#define MUCOSA_X11_XK_KP_Subtract 0xffad
								#define MUCOSA_X11_XK_KP_Decimal 0xffae
								#define MUCOSA_X11_XK_KP_Divide 0xffaf
								#define MUCOSA_X11_XK_F1 0xffbe
								#define MUCOSA_X11_XK_F2 0xffbf
								#define MUCOSA_X11_XK_F3 0xffc0
								#define MUCOSA_X11_XK_F4 0xffc1
								#define MUCOSA_X11_XK_F5 0xffc2
								#define MUCOSA_X11_XK_F6 0xffc3
								#define MUCOSA_X11_XK_F7 0xffc4
								#define MUCOSA_X11_XK_F8 0xffc5
								#define MUCOSA_X11_XK_F9 0xffc6
								#define MUCOSA_X11_XK_F10 0xffc7
								#define MUCOSA_X11_XK_F11 0xffc8
								#define MUCOSA_X11_XK_F12 0xffc9
								#define MUCOSA_X11_XK_F13 0xffca
								#define MUCOSA_X11_XK_F14 0xffcb
								#define MUCOSA_X11_XK_F15 0xffcc
								#define MUCOSA_X11_XK_F16 0xffcd
								#define MUCOSA_X11_XK_F17 0xffce
								#define MUCOSA_X11_XK_F18 0xffcf
								#define MUCOSA_X11_XK_F19 0xffd0
								#define MUCOSA_X11_XK_F20 0xffd1
								#define MUCOSA_X11_XK_F21 0xffd2
								#define MUCOSA_X11_XK_F22 0xffd3
								#define MUCOSA_X11_XK_F23 0xffd4
								#define MUCOSA_X11_XK_F24 0xffd5
								#define MUCOSA_X11_XK_Num_Lock 0xff7f
								#define MUCOSA_X11_XK_Scroll_Lock 0xff14
								#define MUCOSA_X11_XK_Shift_L 0xffe1
								#define MUCOSA_X11_XK_Shift_R 0xffe2
								#define MUCOSA_X11_XK_Control_L 0xffe3
								#define MUCOSA_X11_XK_Control_R 0xffe4
								#define MUCOSA_X11_XK_Menu 0xff67
								#define MUCOSA_X11_XK_3270_Attn 0xfd0e
								#define MUCOSA_X11_XK_3270_CursorSelect 0xfd1c
								#define MUCOSA_X11_XK_3270_ExSelect 0xfd1b
								#define MUCOSA_X11_XK_3270_EraseEOF 0xfd06
								#define MUCOSA_X11_XK_3270_Play 0xfd16
								#define MUCOSA_X11_XK_3270_PA1 0xfd0a

							muKeyboardKey muCOSA_X11_XK_key_to_keyboard_key(int key) {
								switch (key) {
									default: return MU_KEYBOARD_KEY_UNKNOWN; break;
									case MUCOSA_X11_XK_BackSpace: return MU_KEYBOARD_KEY_BACKSPACE; break;
									case MUCOSA_X11_XK_Tab: return MU_KEYBOARD_KEY_TAB; break;
									case MUCOSA_X11_XK_Clear: return MU_KEYBOARD_KEY_CLEAR; break;
									case MUCOSA_X11_XK_Return: return MU_KEYBOARD_KEY_RETURN; break;
									case MUCOSA_X11_XK_Menu: return MU_KEYBOARD_KEY_LEFT_MENU; break;
									case MUCOSA_X11_XK_Pause: return MU_KEYBOARD_KEY_PAUSE; break;
									case MUCOSA_X11_XK_Escape: return MU_KEYBOARD_KEY_ESCAPE; break;
									case MUCOSA_X11_XK_Mode_switch: return MU_KEYBOARD_KEY_MODECHANGE; break;
									case MUCOSA_X11_XK_space: return MU_KEYBOARD_KEY_SPACE; break;
									case MUCOSA_X11_XK_Prior: return MU_KEYBOARD_KEY_PRIOR; break;
									case MUCOSA_X11_XK_Next: return MU_KEYBOARD_KEY_NEXT; break;
									case MUCOSA_X11_XK_End: return MU_KEYBOARD_KEY_END; break;
									case MUCOSA_X11_XK_Home: return MU_KEYBOARD_KEY_HOME; break;
									case MUCOSA_X11_XK_Left: return MU_KEYBOARD_KEY_LEFT; break;
									case MUCOSA_X11_XK_Up: return MU_KEYBOARD_KEY_UP; break;
									case MUCOSA_X11_XK_Right: return MU_KEYBOARD_KEY_RIGHT; break;
									case MUCOSA_X11_XK_Down: return MU_KEYBOARD_KEY_DOWN; break;
									case MUCOSA_X11_XK_Select: return MU_KEYBOARD_KEY_SELECT; break;
									case MUCOSA_X11_XK_Print: return MU_KEYBOARD_KEY_PRINT; break;
									case MUCOSA_X11_XK_Execute: return MU_KEYBOARD_KEY_EXECUTE; break;
									case MUCOSA_X11_XK_Insert: return MU_KEYBOARD_KEY_INSERT; break;
									case MUCOSA_X11_XK_Delete: return MU_KEYBOARD_KEY_DELETE; break;
									case MUCOSA_X11_XK_Help: return MU_KEYBOARD_KEY_HELP; break;
									case MUCOSA_X11_XK_0: return MU_KEYBOARD_KEY_0; break;
									case MUCOSA_X11_XK_1: return MU_KEYBOARD_KEY_1; break;
									case MUCOSA_X11_XK_2: return MU_KEYBOARD_KEY_2; break;
									case MUCOSA_X11_XK_3: return MU_KEYBOARD_KEY_3; break;
									case MUCOSA_X11_XK_4: return MU_KEYBOARD_KEY_4; break;
									case MUCOSA_X11_XK_5: return MU_KEYBOARD_KEY_5; break;
									case MUCOSA_X11_XK_6: return MU_KEYBOARD_KEY_6; break;
									case MUCOSA_X11_XK_7: return MU_KEYBOARD_KEY_7; break;
									case MUCOSA_X11_XK_8: return MU_KEYBOARD_KEY_8; break;
									case MUCOSA_X11_XK_9: return MU_KEYBOARD_KEY_9; break;
									case MUCOSA_X11_XK_a: return MU_KEYBOARD_KEY_A; break;
									case MUCOSA_X11_XK_b: return MU_KEYBOARD_KEY_B; break;
									case MUCOSA_X11_XK_c: return MU_KEYBOARD_KEY_C; break;
									case MUCOSA_X11_XK_d: return MU_KEYBOARD_KEY_D; break;
									case MUCOSA_X11_XK_e: return MU_KEYBOARD_KEY_E; break;
									case MUCOSA_X11_XK_f: return MU_KEYBOARD_KEY_F; break;
									case MUCOSA_X11_XK_g: return MU_KEYBOARD_KEY_G; break;
									case MUCOSA_X11_XK_h: return MU_KEYBOARD_KEY_H; break;
									case MUCOSA_X11_XK_i: return MU_KEYBOARD_KEY_I; break;
									case MUCOSA_X11_XK_j: return MU_KEYBOARD_KEY_J; break;
									case MUCOSA_X11_XK_k: return MU_KEYBOARD_KEY_K; break;
									case MUCOSA_X11_XK_l: return MU_KEYBOARD_KEY_L; break;
									case MUCOSA_X11_XK_m: return MU_KEYBOARD_KEY_M; break;
									case MUCOSA_X11_XK_n: return MU_KEYBOARD_KEY_N; break;
									case MUCOSA_X11_XK_o: return MU_KEYBOARD_KEY_O; break;
									case MUCOSA_X11_XK_p: return MU_KEYBOARD_KEY_P; break;
									case MUCOSA_X11_XK_q: return MU_KEYBOARD_KEY_Q; break;
									case MUCOSA_X11_XK_r: return MU_KEYBOARD_KEY_R; break;
									case MUCOSA_X11_XK_s: return MU_KEYBOARD_KEY_S; break;
									case MUCOSA_X11_XK_t: return MU_KEYBOARD_KEY_T; break;
									case MUCOSA_X11_XK_u: return MU_KEYBOARD_KEY_U; break;
									case MUCOSA_X11_XK_v: return MU_KEYBOARD_KEY_V; break;
									case MUCOSA_X11_XK_w: return MU_KEYBOARD_KEY_W; break;
									case MUCOSA_X11_XK_x: return MU_KEYBOARD_KEY_X; break;
									case MUCOSA_X11_XK_y: return MU_KEYBOARD_KEY_Y; break;
									case MUCOSA_X11_XK_z: return MU_KEYBOARD_KEY_Z; break;
									case MUCOSA_X11_XK_Super_L: return MU_KEYBOARD_KEY_LEFT_WINDOWS; break;
									case MUCOSA_X11_XK_Super_R: return MU_KEYBOARD_KEY_RIGHT_WINDOWS; break;
									case MUCOSA_X11_XK_KP_0: return MU_KEYBOARD_KEY_NUMPAD_0; break;
									case MUCOSA_X11_XK_KP_1: return MU_KEYBOARD_KEY_NUMPAD_1; break;
									case MUCOSA_X11_XK_KP_2: return MU_KEYBOARD_KEY_NUMPAD_2; break;
									case MUCOSA_X11_XK_KP_3: return MU_KEYBOARD_KEY_NUMPAD_3; break;
									case MUCOSA_X11_XK_KP_4: return MU_KEYBOARD_KEY_NUMPAD_4; break;
									case MUCOSA_X11_XK_KP_5: return MU_KEYBOARD_KEY_NUMPAD_5; break;
									case MUCOSA_X11_XK_KP_6: return MU_KEYBOARD_KEY_NUMPAD_6; break;
									case MUCOSA_X11_XK_KP_7: return MU_KEYBOARD_KEY_NUMPAD_7; break;
									case MUCOSA_X11_XK_KP_8: return MU_KEYBOARD_KEY_NUMPAD_8; break;
									case MUCOSA_X11_XK_KP_9: return MU_KEYBOARD_KEY_NUMPAD_9; break;
									case MUCOSA_X11_XK_KP_Multiply: return MU_KEYBOARD_KEY_MULTIPLY; break;
									case MUCOSA_X11_XK_KP_Add: return MU_KEYBOARD_KEY_ADD; break;
									case MUCOSA_X11_XK_KP_Separator: return MU_KEYBOARD_KEY_SEPARATOR; break;
									case MUCOSA_X11_XK_KP_Subtract: return MU_KEYBOARD_KEY_SUBTRACT; break;
									case MUCOSA_X11_XK_KP_Decimal: return MU_KEYBOARD_KEY_DECIMAL; break;
									case MUCOSA_X11_XK_KP_Divide: return MU_KEYBOARD_KEY_DIVIDE; break;
									case MUCOSA_X11_XK_F1: return MU_KEYBOARD_KEY_F1; break;
									case MUCOSA_X11_XK_F2: return MU_KEYBOARD_KEY_F2; break;
									case MUCOSA_X11_XK_F3: return MU_KEYBOARD_KEY_F3; break;
									case MUCOSA_X11_XK_F4: return MU_KEYBOARD_KEY_F4; break;
									case MUCOSA_X11_XK_F5: return MU_KEYBOARD_KEY_F5; break;
									case MUCOSA_X11_XK_F6: return MU_KEYBOARD_KEY_F6; break;
									case MUCOSA_X11_XK_F7: return MU_KEYBOARD_KEY_F7; break;
									case MUCOSA_X11_XK_F8: return MU_KEYBOARD_KEY_F8; break;
									case MUCOSA_X11_XK_F9: return MU_KEYBOARD_KEY_F9; break;
									case MUCOSA_X11_XK_F10: return MU_KEYBOARD_KEY_F10; break;
									case MUCOSA_X11_XK_F11: return MU_KEYBOARD_KEY_F11; break;
									case MUCOSA_X11_XK_F12: return MU_KEYBOARD_KEY_F12; break;
									case MUCOSA_X11_XK_F13: return MU_KEYBOARD_KEY_F13; break;
									case MUCOSA_X11_XK_F14: return MU_KEYBOARD_KEY_F14; break;
									case MUCOSA_X11_XK_F15: return MU_KEYBOARD_KEY_F15; break;
									case MUCOSA_X11_XK_F16: return MU_KEYBOARD_KEY_F16; break;
									case MUCOSA_X11_XK_F17: return MU_KEYBOARD_KEY_F17; break;
									case MUCOSA_X11_XK_F18: return MU_KEYBOARD_KEY_F18; break;
									case MUCOSA_X11_XK_F19: return MU_KEYBOARD_KEY_F19; break;
									case MUCOSA_X11_XK_F20: return MU_KEYBOARD_KEY_F20; break;
									case MUCOSA_X11_XK_F21: return MU_KEYBOARD_KEY_F21; break;
									case MUCOSA_X11_XK_F22: return MU_KEYBOARD_KEY_F22; break;
									case MUCOSA_X11_XK_F23: return MU_KEYBOARD_KEY_F23; break;
									case MUCOSA_X11_XK_F24: return MU_KEYBOARD_KEY_F24; break;
									case MUCOSA_X11_XK_Num_Lock: return MU_KEYBOARD_KEY_NUMLOCK; break;
									case MUCOSA_X11_XK_Scroll_Lock: return MU_KEYBOARD_KEY_SCROLL; break;
									case MUCOSA_X11_XK_Shift_L: return MU_KEYBOARD_KEY_LEFT_SHIFT; break;
									case MUCOSA_X11_XK_Shift_R: return MU_KEYBOARD_KEY_RIGHT_SHIFT; break;
									case MUCOSA_X11_XK_Control_L: return MU_KEYBOARD_KEY_LEFT_CONTROL; break;
									case MUCOSA_X11_XK_Control_R: return MU_KEYBOARD_KEY_RIGHT_CONTROL; break;
									case MUCOSA_X11_XK_3270_Attn: return MU_KEYBOARD_KEY_ATTN; break;
									case MUCOSA_X11_XK_3270_CursorSelect: return MU_KEYBOARD_KEY_CRSEL; break;
									case MUCOSA_X11_XK_3270_ExSelect: return MU_KEYBOARD_KEY_EXSEL; break;
									case MUCOSA_X11_XK_3270_EraseEOF: return MU_KEYBOARD_KEY_EREOF; break;
									case MUCOSA_X11_XK_3270_Play: return MU_KEYBOARD_KEY_PLAY; break;
									case MUCOSA_X11_XK_3270_PA1: return MU_KEYBOARD_KEY_PA1; break;
								}
							}

						/* Event handling */

							void muCOSA_X11_input_handle_keyboard_key(muCOSA_X11_window* win, XKeyEvent* p_kev, unsigned int keycode, muByte down) {
								muKeyboardKey key = muCOSA_X11_XK_key_to_keyboard_key(XkbKeycodeToKeysym(win->display, keycode, 0, 0));
								if (key != MU_KEYBOARD_KEY_UNKNOWN) {
									win->input.keyboard_key_states[key-MU_KEYBOARD_KEY_FIRST] = down;

									if (win->keyboard_key_callback != MU_NULL_PTR) {
										win->keyboard_key_callback(win, key, win->input.keyboard_key_states[key-MU_KEYBOARD_KEY_FIRST]);
									}
								}

								if (win->ic.active && win->ic.callback) {
									muCOSA_X11_handle_text_input_event(win, p_kev);
								}
							}

						/* Get */

							muButtonState muCOSA_X11_window_get_keyboard_key_state(muCOSA_X11_window* win, muKeyboardKey key) {
								return win->input.keyboard_key_states[key-MU_KEYBOARD_KEY_FIRST];
							}

					/* Keyboard state */

						/* Event handling */

							void muCOSA_X11_input_check_state(muCOSA_X11_window* win, unsigned int n, const char* name, muKeyboardState state) {
								muBool check = (n & (XInternAtom(win->display, name, False) - 1)) != 0;

								if (check != win->input.keyboard_state_states[state-MU_KEYBOARD_STATE_FIRST]) {
									win->input.keyboard_state_states[state-MU_KEYBOARD_STATE_FIRST] = check;

									if (win->keyboard_state_callback != 0) {
										win->keyboard_state_callback(win, state, check);
									}
								}
							}

							void muCOSA_X11_input_handle_states(muCOSA_X11_window* win) {
								// http://levonp.blogspot.com/2010/08/retrieving-caps-lock-info-using-xlib.html
								unsigned int n;
								XkbGetIndicatorState(win->display, XkbUseCoreKbd, &n);

								muCOSA_X11_input_check_state(win, n, "Caps Lock", MU_KEYBOARD_STATE_CAPS_LOCK);
								muCOSA_X11_input_check_state(win, n, "Num Lock", MU_KEYBOARD_STATE_NUM_LOCK);
								// Been having issues with this... being triggered by Caps Lock and Num Lock
								// sometimes... :L
								muCOSA_X11_input_check_state(win, n, "Scroll Lock", MU_KEYBOARD_STATE_SCROLL_LOCK);
							}

						/* Get */

							muState muCOSA_X11_window_get_keyboard_state_state(muCOSA_X11_window* win, muKeyboardState state) {
								return win->input.keyboard_state_states[state-MU_KEYBOARD_STATE_FIRST];
							}

					/* Cursor */

						/* Useful functions */

							int muCOSA_X11_cursor_get_style(muCursorStyle style) {
								switch (style) {
									default: return 2; break;
									case MU_CURSOR_STYLE_IBEAM: return 152; break;
									case MU_CURSOR_STYLE_WAIT: case MU_CURSOR_STYLE_WAIT_ARROW: return 150; break;
									case MU_CURSOR_STYLE_CROSSHAIR: return 34; break;
									case MU_CURSOR_STYLE_HAND: return 58; break;
									case MU_CURSOR_STYLE_SIZE_EAST_WEST: return 108; break;
									case MU_CURSOR_STYLE_SIZE_NORTH_SOUTH: return 116; break;
									case MU_CURSOR_STYLE_SIZE_NORTH_EAST_SOUTH_WEST: case MU_CURSOR_STYLE_SIZE_NORTH_WEST_SOUTH_EAST: return 120; break;
									case MU_CURSOR_STYLE_SIZE_ALL: return 52; break;
									case MU_CURSOR_STYLE_NO: return 0; break;
								}
							}

						/* Event handling */

							void muCOSA_X11_input_handle_cursor_button(muCOSA_X11_window* win, int button, muByte down) {
								switch (button) {
									default: break;

									case Button1: {
										win->input.mouse_button_states[MU_MOUSE_BUTTON_LEFT-MU_MOUSE_BUTTON_FIRST] = down;
										if (win->mouse_button_callback != 0) {
											win->mouse_button_callback(win, MU_MOUSE_BUTTON_LEFT, down);
										}
									} break;

									case Button2: {
										win->input.mouse_button_states[MU_MOUSE_BUTTON_MIDDLE-MU_MOUSE_BUTTON_FIRST] = down;
										if (win->mouse_button_callback != 0) {
											win->mouse_button_callback(win, MU_MOUSE_BUTTON_MIDDLE, down);
										}
									} break;

									case Button3: {
										win->input.mouse_button_states[MU_MOUSE_BUTTON_RIGHT-MU_MOUSE_BUTTON_FIRST] = down;
										if (win->mouse_button_callback != 0) {
											win->mouse_button_callback(win, MU_MOUSE_BUTTON_RIGHT, down);
										}
									} break;

									// Not 100% sure if 120 is the appropriate value here for matching with Win32's WM_MOUSEWHEEL.

									/* Scroll wheel up */
									case Button4: {
										if (!down) {
											win->scroll_level += 120;
										}
										if (win->scroll_callback != 0) {
											win->scroll_callback(win, 120);
										}
									} break;

									/* Scroll wheel down */
									case Button5: {
										if (!down) {
											win->scroll_level -= 120;
										}
										if (win->scroll_callback != 0) {
											win->scroll_callback(win, -120);
										}
									} break;
								}
							}

							void muCOSA_X11_input_handle_cursor_motion(muCOSA_X11_window* win, XMotionEvent motion_event) {
								if (win->cursor_position_callback != 0) {
									win->cursor_position_callback(win, (int32_m)motion_event.x, (int32_m)motion_event.y);
								}
							}

						/* Get / Set */

							void muCOSA_X11_window_get_position(muCOSA_X11_window* win, int32_m* wx, int32_m* wy);
							void muCOSA_X11_window_get_cursor_position(muCOSAResult* result, muCOSA_X11_window* win, int32_m* x, int32_m* y) {
								// https://stackoverflow.com/questions/3585871/how-can-i-get-the-current-mouse-pointer-position-co-ordinates-in-x
								int screen_count = XScreenCount(win->display);

								Bool oresult = False;
								int root_x, root_y;
								for (int i = 0; i < screen_count; i++) {
									Window root_window = XRootWindow(win->display, i);

									Window window_ret, window_chi;
									int win_x, win_y;
									unsigned int mask_return;
									oresult = XQueryPointer(win->display, root_window,  &window_ret, &window_chi, &root_x, &root_y, &win_x, &win_y, &mask_return);
									// "window_ret" is the window the cursor is located in
									// "root_x" and "root_y" are cursor pos relative to "window_ret"
									// "win_x" and "win_y" are cursor pos relative to "root_window" (which I'm pretty sure is the whole display)

									if (oresult == True) {
										int32_m wx, wy;
										muCOSA_X11_window_get_position(win, &wx, &wy);

										// I really don't get how this works, lol...
										MU_SET_RESULT(x, (int32_m)win_x - wx)
										MU_SET_RESULT(y, (int32_m)win_y - wy)
										return;
									}
								}

								MU_SET_RESULT(result, MUCOSA_NONEXISTENT_DEVICE)
							}

							void muCOSA_X11_window_set_cursor_position(muCOSA_X11_window* win, int32_m x, int32_m y) {
								XWarpPointer(win->display, win->window, win->window, 0, 0, 0, 0, x, y);
							}

							muCursorStyle muCOSA_X11_window_get_cursor_style(muCOSA_X11_window* win) {
								return win->cursor_style;
							}

							void muCOSA_X11_window_set_cursor_style(muCOSA_X11_window* win, muCursorStyle style) {
								XUndefineCursor(win->display, win->window);
								XFreeCursor(win->display, win->cursor);

								win->cursor_style = style;
								win->cursor = XCreateFontCursor(win->display, muCOSA_X11_cursor_get_style(style));
								XDefineCursor(win->display, win->window, win->cursor);
							}

							int32_m muCOSA_X11_window_get_scroll_level(muCOSA_X11_window* win) {
								return win->scroll_level;
							}

							void muCOSA_X11_window_set_scroll_level(muCOSA_X11_window* win, int32_m scroll_level) {
								win->scroll_level = scroll_level;
							}

						/* Get */

							muButtonState muCOSA_X11_window_get_mouse_button_state(muCOSA_X11_window* win, muMouseButton button) {
								return win->input.mouse_button_states[button-MU_MOUSE_BUTTON_FIRST];
							}

					/* Global handling */

						void muCOSA_X11_input_handle_event(muCOSA_X11_window* win, XEvent event) {
							switch (event.type) {
								default: return; break;

								/* Refreshing keyboard mapping */
								case KeymapNotify: XRefreshKeyboardMapping(&event.xmapping); break;

								/* Closing the window */
								case ClientMessage: {
									if (event.xclient.data.l[0] == (long int)win->delete_atom) {
										XUnmapWindow(win->display, win->window);
										win->closed = MU_TRUE;
									}
								} break;

								/* Mouse button press/release */
								case ButtonPress: muCOSA_X11_input_handle_cursor_button(win, event.xbutton.button, MU_TRUE); break;
								case ButtonRelease: muCOSA_X11_input_handle_cursor_button(win, event.xbutton.button, MU_FALSE); break;
								/* Mouse button movement */
								case MotionNotify: muCOSA_X11_input_handle_cursor_motion(win, event.xmotion); break;

								/* Keyboard key press/release */
								case KeyPress: muCOSA_X11_input_handle_keyboard_key(win, &event.xkey, event.xkey.keycode, MU_TRUE); break;
								case KeyRelease: muCOSA_X11_input_handle_keyboard_key(win, &event.xkey, event.xkey.keycode, MU_FALSE); break;

								/* Dimension changing */
								case Expose: {
									XWindowAttributes attribs;
									XGetWindowAttributes(win->display, win->window, &attribs);
									if (win->dimensions_callback != 0) {
										win->dimensions_callback(win, (uint32_m)attribs.width, (uint32_m)attribs.height);
									}
								} break;

								/* Position changing */
								case ConfigureNotify: {
									if (win->x != event.xconfigure.x || win->y != event.xconfigure.y) {
										win->x = (int32_m)event.xconfigure.x;
										win->y = (int32_m)event.xconfigure.y;
										if (win->position_callback != 0) {
											win->position_callback(win, win->x, win->y);
										}
									}
								} break;

								/* Focusing in/out */
								// @TODO Fix FocusIn/FocusOut getting called when window is being dragged
								case FocusIn: {
									if (win->ic.active) {
										XSetICFocus(win->ic.ic);
									}
									if (win->focus_callback != 0) {
										win->focus_callback(win, MU_TRUE);
									}
								} break;
								case FocusOut: {
									if (win->ic.active) {
										XUnsetICFocus(win->ic.ic);
									}
									if (win->focus_callback != 0) {
										win->focus_callback(win, MU_FALSE);
									}
								} break;
							}
						}

						void muCOSA_X11_input_handle_focus_flushing(muCOSA_X11_window* win) {
							Window focused_window;
							int revert_to;
							XGetInputFocus(win->display, &focused_window, &revert_to);

							if (focused_window != win->window) {
								win->input = MU_ZERO_STRUCT(muCOSA_X11_input);
							}
						}

						muBool muCOSA_X11_window_get_maximized(muCOSA_X11_window* win);
						muBool muCOSA_X11_window_get_minimized(muCOSA_X11_window* win);
						void muCOSA_X11_input_handle_min_max_events(muCOSA_X11_window* win) {
							muBool maximized = muCOSA_X11_window_get_maximized(win);
							if (win->maximized != maximized) {
								win->maximized = maximized;
								if (win->maximize_callback != 0) {
									win->maximize_callback(win, maximized);
								}
							}

							muBool minimized = muCOSA_X11_window_get_minimized(win);
							if (win->minimized != minimized) {
								win->minimized = minimized;
								if (win->minimize_callback != 0) {
									win->minimize_callback(win, minimized);
								}
							}
						}

				/* Creation / Destruction */

					int muCOSA_X11_cursor_get_style(muCursorStyle style);
					void muCOSA_X11_window_set_maximized(muCOSA_X11_window* win, muBool maximized);
					void muCOSA_X11_window_set_minimized(muCOSA_X11_window* win, muBool minimized);
					void muCOSA_X11_window_set_position(muCOSA_X11_window* win, int32_m x, int32_m y);
					void muCOSA_X11_window_create(muCOSA_X11_window* win, muCOSAResult* result, muGraphicsAPI api, muBool (*load_functions)(void), const char* name, uint16_m width, uint16_m height, muWindowCreateInfo create_info) {
						/* Size hints and display */

							win->size_hints = XAllocSizeHints();
							if (win->size_hints == 0) {
								MU_SET_RESULT(result, MUCOSA_ALLOCATION_FAILED)
								return;
							}

							win->display = XOpenDisplay(0);
							if (win->display == NULL) {
								MU_SET_RESULT(result, MUCOSA_FAILED_CONNECTION_TO_SERVER)
								XFree(win->size_hints);
								return;
							}

							// From here on out, calling XCloseDisplay will close all objects created with the
							// display, so don't worry about it.
							// https://www.x.org/releases/X11R7.5/doc/man/man3/XOpenDisplay.3.html

						/* Window */

							win->parent_window = DefaultRootWindow(win->display);

							/* Visual */

								XVisualInfo* vi = 0;
								MUCOSA_OPENGL_CALL(
									muCOSA_X11_gl_attributes gl_att = MU_ZERO_STRUCT(muCOSA_X11_gl_attributes); if (gl_att.att[2]) {}
									GLXFBConfig* fbcs = 0;
									GLXFBConfig fbc;
									int fbc_count = 0;
								)

								win->api = api;
								if (api >= MUCOSA_OPENGL_FIRST && api <= MUCOSA_OPENGL_LAST) {
									MUCOSA_OPENGL_CALL(
										gl_att = muCOSA_X11_get_gl_attributes(create_info.pixel_format);
										fbcs = glXChooseFBConfig(win->display, DefaultScreen(win->display), gl_att.att, &fbc_count);

										if (fbcs != 0) {
											// @TODO Choose best format
											fbc = fbcs[0];
											XFree(fbcs);
											vi = glXGetVisualFromFBConfig(win->display, fbc);
										}
									)
								}

							/* Attributes */

								// I'm pretty sure this isn't being done right... :L
								int depth = 8*3;
								Visual* v = 0;
								unsigned long valuemask = CWEventMask;
								XSetWindowAttributes win_attribs = MU_ZERO_STRUCT(XSetWindowAttributes);
								win_attribs.event_mask = ExposureMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | FocusChangeMask;

								if (vi != 0) {
									depth = vi->depth;
									v = vi->visual;

									win_attribs.colormap = XCreateColormap(win->display, win->parent_window, vi->visual, AllocNone);
									valuemask |= CWColormap;
								}

							/* Creation */

								// @TODO Figure out how to tell if this worked or not
								win->window = XCreateWindow(win->display, win->parent_window, 0, 0, (unsigned int)width, (unsigned int)height, 0, depth, InputOutput, v, valuemask, &win_attribs);

								if (vi != 0) {
									XFree(vi);
								}

						/* Cursor */

							win->cursor_style = create_info.cursor_style;
							win->cursor = XCreateFontCursor(win->display, muCOSA_X11_cursor_get_style(win->cursor_style));
							XDefineCursor(win->display, win->window, win->cursor);

						/* Graphics API */

							/* Initialization */

								if (api >= MUCOSA_OPENGL_FIRST && api <= MUCOSA_OPENGL_LAST) {
									MUCOSA_OPENGL_CALL(
										muCOSAResult res = muCOSA_X11_init_opengl(win->display, &win->gl_context, api, fbc);
										if (res != MUCOSA_SUCCESS) {
											MU_SET_RESULT(result, res)
											XFree(win->size_hints);
											XSync(win->display, False);
											XCloseDisplay(win->display);
										}
									)
								}

							/* Loading */

								if (api >= MUCOSA_OPENGL_FIRST && api <= MUCOSA_OPENGL_LAST) {
									MUCOSA_OPENGL_CALL(
										glXMakeCurrent(win->display, win->window, win->gl_context);
									)
								}

								if (load_functions != 0) {
									if (!load_functions()) {
										if (api >= MUCOSA_OPENGL_FIRST && api <= MUCOSA_OPENGL_LAST) {
											MUCOSA_OPENGL_CALL(glXDestroyContext(win->display, win->gl_context);)
										}
										XFree(win->size_hints);
										XSync(win->display, False);
										XCloseDisplay(win->display);
									}
								}

						/* Name */

							XChangeProperty(win->display, win->window, XInternAtom(win->display, "_NET_WM_NAME", False), XInternAtom(win->display, "UTF8_STRING", False), 8, PropModeReplace, (unsigned char*)name, mu_strlen(name));

						/* Delete message */

							win->delete_atom = XInternAtom(win->display, "WM_DELETE_WINDOW", False);
							XSetWMProtocols(win->display, win->window, &win->delete_atom, 1);

						/* Size hints */

							*win->size_hints = MU_ZERO_STRUCT(XSizeHints);
							win->size_hints->flags = PPosition | PMinSize | PMaxSize;
							win->size_hints->x = create_info.x;
							win->size_hints->y = create_info.y;
							win->size_hints->min_width = create_info.min_width;
							win->size_hints->min_height = create_info.min_height;
							win->size_hints->max_width = create_info.max_width;
							win->size_hints->max_height = create_info.max_height;
							if (!create_info.resizable) {
								win->size_hints->min_width = width;
								win->size_hints->min_height = height;
								win->size_hints->max_width = width;
								win->size_hints->max_height = height;
							}
							XSetWMNormalHints(win->display, win->window, win->size_hints);

						/* Map */

							if (create_info.visible) {
								XMapWindow(win->display, win->window);
							}
							XSync(win->display, False);

							muCOSA_X11_window_set_maximized(win, create_info.maximized);
							muCOSA_X11_window_set_minimized(win, create_info.minimized);

						/* Position */

							for (size_m i = 0; i < 10; i++) {
								muCOSA_X11_window_set_position(win, create_info.x, create_info.y);
							}

						/* Give */

							win->input = MU_ZERO_STRUCT(muCOSA_X11_input);
							win->ic = MU_ZERO_STRUCT(muCOSA_X11_ic);
							win->closed = MU_FALSE;
							win->visible = create_info.visible;
							win->resizable = create_info.resizable;
							win->min_width = create_info.min_width;
							win->min_height = create_info.min_height;
							win->max_width = create_info.max_width;
							win->max_height = create_info.max_height;
							win->scroll_level = 0;
							win->x = create_info.x;
							win->y = create_info.y;
							win->minimized = create_info.minimized;
							win->maximized = create_info.maximized;
							win->dimensions_callback = create_info.dimensions_callback;
							win->position_callback = create_info.position_callback;
							win->focus_callback = create_info.focus_callback;
							win->maximize_callback = create_info.maximize_callback;
							win->minimize_callback = create_info.minimize_callback;
							win->keyboard_key_callback = create_info.keyboard_key_callback;
							win->keyboard_state_callback = create_info.keyboard_state_callback;
							win->cursor_position_callback = create_info.cursor_position_callback;
							win->mouse_button_callback = create_info.mouse_button_callback;
							win->scroll_callback = create_info.scroll_callback;
					}

					void muCOSA_X11_window_destroy(muCOSA_X11_window* win) {
						XFree(win->size_hints);

						if (win->api >= MUCOSA_OPENGL_FIRST && win->api <= MUCOSA_OPENGL_LAST) {
							MUCOSA_OPENGL_CALL(glXDestroyContext(win->display, win->gl_context);)
						}

						XSync(win->display, False);
						XCloseDisplay(win->display);
					}

				/* Atom logic */

					Atom muCOSA_X11_window_get_atoms(muCOSA_X11_window* win, const char** names, size_m namelen) {
						Atom wm_state = XInternAtom(win->display, "_NET_WM_STATE", MU_TRUE);
						if (wm_state == None) {
							return None;
						}

						Atom type;
						int format;
						unsigned long n_item = 0, bytes_after;
						unsigned char* properties = 0;
						XGetWindowProperty(win->display, win->window, wm_state, 0, (~0L), False, AnyPropertyType, &type, &format, &n_item, &bytes_after, &properties);

						for (size_m i = 0; i < n_item; i++) {
							Atom prop = ((Atom*)properties)[0];
							char* prop_name = XGetAtomName(win->display, prop);
							if (prop_name != 0) {
								for (size_m i = 0; i < namelen; i++) {
									if (mu_strcmp(prop_name, names[i]) == 0) {
										XFree(properties);
										return prop;
									}
								}
							}
						}

						if (properties != 0) {
							XFree(properties);
						}
						return None;
					}

					Atom muCOSA_X11_window_get_atom(muCOSA_X11_window* win, const char* name) {
						const char* names[1] = { name };
						return muCOSA_X11_window_get_atoms(win, names, 1);
					}

			/* Main loop */

				muBool muCOSA_X11_window_get_closed(muCOSA_X11_window* win) {
					return win->closed;
				}

				void muCOSA_X11_window_close(muCOSA_X11_window* win) {
					XUnmapWindow(win->display, win->window);
					win->closed = MU_TRUE;
				}

				void muCOSA_X11_window_update(muCOSA_X11_window* win) {
					while (XPending(win->display)) {
						XEvent event;
						XNextEvent(win->display, &event);

						if (!XFilterEvent(&event, None)) {
							muCOSA_X11_input_handle_event(win, event);
						}
					}

					muCOSA_X11_input_handle_states(win);
					muCOSA_X11_input_handle_focus_flushing(win);
					muCOSA_X11_input_handle_min_max_events(win);
				}

				void muCOSA_X11_window_swap_buffers(muCOSA_X11_window* win) {
					if (win->api >= MUCOSA_OPENGL_FIRST && win->api <= MUCOSA_OPENGL_LAST) {
						MUCOSA_OPENGL_CALL(
							glXSwapBuffers(win->display, win->window);
						)
					}
				}

			/* Get / Set */

				muBool muCOSA_X11_window_get_focused(muCOSA_X11_window* win) {
					Window focused;
					int revert_to;
					XGetInputFocus(win->display, &focused, &revert_to);
					return focused == win->window;
				}

				muBool muCOSA_X11_window_get_minimized(muCOSA_X11_window* win);
				// This function, in my opinion, represents everything wrong with X11. Atom management
				// hell, message sending hell, nothing registering when you actually do it, etc.
				void muCOSA_X11_window_focus(muCOSAResult* result, muCOSA_X11_window* win) {
					if (muCOSA_X11_window_get_atom(win, "_NET_WM_STATE_HIDDEN") != None) {
						// https://stackoverflow.com/questions/30192347/how-to-restore-a-window-with-xlib
						XClientMessageEvent ev = MU_ZERO_STRUCT(XClientMessageEvent);

						ev.type = ClientMessage;
						ev.window = win->window;
						ev.message_type = XInternAtom(win->display, "_NET_ACTIVE_WINDOW", True);
						ev.format = 32;
						ev.data.l[0] = 1; ev.data.l[1] = CurrentTime;
						ev.data.l[2] = ev.data.l[3] = ev.data.l[4] = 0;

						XSendEvent(win->display, win->parent_window, False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent*)&ev);
						XFlush(win->display);
					}

					// Try blocking for a bit to register the deminimization
					// This is a hack and a half
					double time_beg = muCOSA_X11_get_system_time();
					while (muCOSA_X11_window_get_minimized(win) && (muCOSA_X11_get_system_time()-time_beg) < .01f) {}

					if (muCOSA_X11_window_get_minimized(win) || !win->visible) {
						MU_SET_RESULT(result, MUCOSA_INVALID_WINDOW_STATE)
						return;
					}

					XSetInputFocus(win->display, win->window, RevertToPointerRoot, CurrentTime);
					XRaiseWindow(win->display, win->window);
				}

				muBool muCOSA_X11_window_get_visible(muCOSA_X11_window* win) {
					return win->visible;
				}

				void muCOSA_X11_window_set_visible(muCOSA_X11_window* win, muBool visible) {
					if (visible && !win->visible) {
						XMapWindow(win->display, win->window);
						win->input = MU_ZERO_STRUCT(muCOSA_X11_input);
					} else if (!visible && win->visible) {
						XUnmapWindow(win->display, win->window);
						win->input = MU_ZERO_STRUCT(muCOSA_X11_input);
					}

					win->visible = visible;
				}

				void muCOSA_X11_window_get_position(muCOSA_X11_window* win, int32_m* x, int32_m* y) {
					int rx, ry;
					Window child;
					XTranslateCoordinates(win->display, win->window, win->parent_window, 0, 0, &rx, &ry, &child);
					MU_SET_RESULT(x, (int32_m)rx)
					MU_SET_RESULT(y, (int32_m)ry)
				}

				void muCOSA_X11_window_set_position(muCOSA_X11_window* win, int32_m x, int32_m y) {
					XWindowAttributes xwa;
					XGetWindowAttributes(win->display, win->window, &xwa);
					XMoveWindow(win->display, win->window, (x-(int32_m)xwa.x), (y-(int32_m)xwa.y));
				}

				void muCOSA_X11_window_get_dimensions(muCOSA_X11_window* win, uint32_m* w, uint32_m* h) {
					XWindowAttributes xwa;
					XGetWindowAttributes(win->display, win->window, &xwa);
					MU_SET_RESULT(w, (uint32_m)xwa.width)
					MU_SET_RESULT(h, (uint32_m)xwa.height)
				}

				void muCOSA_X11_window_set_dimensions(muCOSA_X11_window* win, uint32_m w, uint32_m h) {
					XResizeWindow(win->display, win->window, w, h);
				}

				muBool muCOSA_X11_window_get_maximized(muCOSA_X11_window* win) {
					static const char* names[2] = { "_NET_WM_STATE_MAXIMIZED_VERT", "_NET_WM_STATE_MAXIMIZED_HORZ" };
					return muCOSA_X11_window_get_atoms(win, names, 2) != None;
				}

				void muCOSA_X11_window_set_maximized(muCOSA_X11_window* win, muBool maximized) {
					// https://forum.juce.com/t/how-to-maximize-not-fullscreen/28346
					XClientMessageEvent ev = MU_ZERO_STRUCT(XClientMessageEvent);

					ev.type = ClientMessage;
					ev.window = win->window;
					ev.message_type = XInternAtom(win->display, "_NET_WM_STATE", False);
					ev.format = 32;
					ev.data.l[0] = maximized;
					ev.data.l[1] = XInternAtom(win->display, "_NET_WM_STATE_MAXIMIZED_VERT", False);
					ev.data.l[2] = XInternAtom(win->display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
					ev.data.l[3] = 1; ev.data.l[4] = 0;

					XSendEvent(win->display, win->parent_window, False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent*)&ev);
					XFlush(win->display);
				}

				muBool muCOSA_X11_window_get_minimized(muCOSA_X11_window* win) {
					return muCOSA_X11_window_get_atom(win, "_NET_WM_STATE_HIDDEN") != None;
				}

				void muCOSA_X11_window_set_minimized(muCOSA_X11_window* win, muBool minimized) {
					XClientMessageEvent ev = MU_ZERO_STRUCT(XClientMessageEvent);

					ev.type = ClientMessage;
					ev.window = win->window;
					if (minimized) {
						ev.message_type = XInternAtom(win->display, "WM_CHANGE_STATE", False);
						ev.format = 32;
						ev.data.l[0] = IconicState;
					} else {
						ev.message_type = XInternAtom(win->display, "_NET_ACTIVE_WINDOW", False);
						ev.format = 32;
						ev.data.l[0] = 1;
						ev.data.l[1] = CurrentTime;
						ev.data.l[2] = ev.data.l[3] = ev.data.l[4] = 0;
					}

					XSendEvent(win->display, win->parent_window, False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent*)&ev);
					XFlush(win->display);
				}

				void muCOSA_X11_window_get_minimum_dimensions(muCOSA_X11_window* win, uint32_m* min_w, uint32_m* min_h) {
					MU_SET_RESULT(min_w, win->min_width)
					MU_SET_RESULT(min_h, win->min_height)
				}

				void muCOSA_X11_window_set_minimum_dimensions(muCOSA_X11_window* win, uint32_m min_w, uint32_m min_h) {
					win->min_width = min_w;
					win->min_height = min_h;

					*win->size_hints = MU_ZERO_STRUCT(XSizeHints);
					win->size_hints->flags = PMinSize;
					win->size_hints->min_width = win->min_width;
					win->size_hints->min_height = win->min_height;
					XSetWMNormalHints(win->display, win->window, win->size_hints);
				}

				void muCOSA_X11_window_get_maximum_dimensions(muCOSA_X11_window* win, uint32_m* max_w, uint32_m* max_h) {
					MU_SET_RESULT(max_w, win->max_width)
					MU_SET_RESULT(max_h, win->max_height)
				}

				void muCOSA_X11_window_set_maximum_dimensions(muCOSA_X11_window* win, uint32_m max_w, uint32_m max_h) {
					win->max_width = max_w;
					win->max_height = max_h;

					*win->size_hints = MU_ZERO_STRUCT(XSizeHints);
					win->size_hints->flags = PMaxSize;
					win->size_hints->max_width = win->max_width;
					win->size_hints->max_height = win->max_height;
					XSetWMNormalHints(win->display, win->window, win->size_hints);
				}

			/* Get */

				void muCOSA_X11_window_get_frame_extents(muCOSA_X11_window* win, uint32_m* l, uint32_m* r, uint32_m* t, uint32_m* b) {
					Atom a = XInternAtom(win->display, "_NET_FRAME_EXTENTS", True);
					if (a == None) {
						MU_SET_RESULT(l, 0)
						MU_SET_RESULT(r, 0)
						MU_SET_RESULT(t, 0)
						MU_SET_RESULT(b, 0)
						return;
					}

					Atom _t;
					int f;
					unsigned long _n, _b;
					unsigned char* data = 0;
					int _r = XGetWindowProperty(win->display, win->window, a, 0, 4, False, AnyPropertyType, &_t, &f, &_n, &_b, &data);

					if (_r == Success && _n == 4 && _b == 0 && data) {
						long* extents = (long*)data;
						MU_SET_RESULT(l, (uint32_m)extents[0])
						MU_SET_RESULT(r, (uint32_m)extents[1])
						MU_SET_RESULT(t, (uint32_m)extents[2])
						MU_SET_RESULT(b, (uint32_m)extents[3])
					} else {
						MU_SET_RESULT(l, 0)
						MU_SET_RESULT(r, 0)
						MU_SET_RESULT(t, 0)
						MU_SET_RESULT(b, 0)
					}
				}

			/* Set */

				void muCOSA_X11_window_set_title(muCOSA_X11_window* win, const char* title) {
					XChangeProperty(win->display, win->window,
							XInternAtom(win->display, "_NET_WM_NAME", False),
							XInternAtom(win->display, "UTF8_STRING", False),
							8, PropModeReplace, (unsigned char*)title, (int)mu_strlen(title)
						);
				}

				void muCOSA_X11_window_set_dimensions_callback(muCOSA_X11_window* win, void (*callback)(muWindow window, uint32_m width, uint32_m height)) {
					win->dimensions_callback = callback;
				}

				void muCOSA_X11_window_set_position_callback(muCOSA_X11_window* win, void (*callback)(muWindow window, int32_m x, int32_m y)) {
					win->position_callback = callback;
				}

				void muCOSA_X11_window_set_focus_callback(muCOSA_X11_window* win, void (*callback)(muWindow window, muBool focused)) {
					win->focus_callback = callback;
				}

				void muCOSA_X11_window_set_maximize_callback(muCOSA_X11_window* win, void (*callback)(muWindow window, muBool maximized)) {
					win->maximize_callback = callback;
				}

				void muCOSA_X11_window_set_minimize_callback(muCOSA_X11_window* win, void (*callback)(muWindow window, muBool minimized)) {
					win->minimize_callback = callback;
				}

				void muCOSA_X11_window_set_keyboard_key_callback(muCOSA_X11_window* win, void (*callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state)) {
					win->keyboard_key_callback = callback;
				}

				void muCOSA_X11_window_set_keyboard_state_callback(muCOSA_X11_window* win, void (*callback)(muWindow window, muKeyboardState keyboard_state, muState state)) {
					win->keyboard_state_callback = callback;
				}

				void muCOSA_X11_window_set_cursor_position_callback(muCOSA_X11_window* win, void (*callback)(muWindow window, int32_m x, int32_m y)) {
					win->cursor_position_callback = callback;
				}

				void muCOSA_X11_window_set_mouse_button_callback(muCOSA_X11_window* win, void (*callback)(muWindow window, muMouseButton mouse_button, muButtonState state)) {
					win->mouse_button_callback = callback;
				}

				void muCOSA_X11_window_set_scroll_callback(muCOSA_X11_window* win, void (*callback)(muWindow window, int32_m scroll_level_add)) {
					win->scroll_callback = callback;
				}

			/* OS functions */

				void* muCOSA_X11_os_get_window_handle(muCOSAResult* result, muCOSA_X11_window* win, muWindowHandle handle) {
					switch (handle) {
						default: MU_SET_RESULT(result, MUCOSA_UNKNOWN_WINDOW_HANDLE) return 0; break;
						case MU_X11_DISPLAY: return (void*)&win->display; break;
						case MU_X11_WINDOW: return (void*)&win->window; break;
						case MU_X11_PARENT_WINDOW: return (void*)&win->parent_window; break;
					}
				}

			/* OpenGL */

				void muCOSA_X11_opengl_bind_window(muCOSAResult* result, muCOSA_X11_window* win) {
					#ifndef MUCOSA_OPENGL
						MU_SET_RESULT(result, MUCOSA_UNSUPPORTED_GRAPHICS_API)
						return; if (win) {}
					#else
						glXMakeCurrent(win->display, win->window, win->gl_context);
						return; if (result) {}
					#endif
				}

				void* muCOSA_X11_opengl_get_function_address(const char* name) {
					#ifndef MUCOSA_OPENGL
						return 0; if (name) {}
					#else
						void(*addr)() = glXGetProcAddress((const GLubyte*)name);
						void* ret = 0;
						mu_memcpy(&ret, &addr, sizeof(void*));
						return ret;
					#endif
				}

				void muCOSA_X11_opengl_window_swap_interval(muCOSAResult* result, muCOSA_X11_window* win, int interval) {
					#ifndef MUCOSA_OPENGL
						MU_SET_RESULT(result, MUCOSA_UNSUPPORTED_GRAPHICS_API)
						return; if (win) {} if (interval) {}
					#else
						muCOSA_X11_opengl_bind_window(0, win);

						void* ptr = muCOSA_X11_opengl_get_function_address("glXSwapIntervalEXT");
						if (ptr == 0) {
							MU_SET_RESULT(result, MUCOSA_UNSUPPORTED_OPENGL_FEATURE)
							return;
						}

						GLXDrawable drawable = glXGetCurrentDrawable();
						if (drawable) {
							PFNGLXSWAPINTERVALEXTPROC proc = 0;
							mu_memcpy(&proc, &ptr, sizeof(void*));
							proc(win->display, drawable, interval);
						} else {
							MU_SET_RESULT(result, MUCOSA_UNSUPPORTED_OPENGL_FEATURE)
						}
					#endif
				}

			/* Vulkan */

				const char** muCOSA_X11_vulkan_get_surface_instance_extensions(muCOSAResult* result, size_m* count) {
					#ifndef MUCOSA_VULKAN
						MU_SET_RESULT(result, MUCOSA_UNSUPPORTED_GRAPHICS_API)
						return 0; if (count) {}
					#else
						static const char* exts[] = { "VK_KHR_surface", "VK_KHR_xlib_surface" };
						MU_SET_RESULT(count, 2)
						return (const char**)exts; if (result) {}
					#endif
				}

				void muCOSA_X11_vulkan_create_window_surface(muCOSAResult* result, muCOSA_X11_window* win, void* vk_result, void* instance, void* allocator, void* surface) {
					#ifndef MUCOSA_VULKAN
						MU_SET_RESULT(result, MUCOSA_UNSUPPORTED_GRAPHICS_API)
						return; if (win) {} if (vk_result) {} if (instance) {} if (allocator) {} if (surface) {}
					#else
						VkXlibSurfaceCreateInfoKHR create_info = MU_ZERO_STRUCT(VkXlibSurfaceCreateInfoKHR);
						create_info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
						create_info.dpy = win->display;
						create_info.window = win->window;

						VkInstance r_instance = VK_NULL_HANDLE;
						if (instance != 0) {
							r_instance = *((VkInstance*)instance);
						}

						VkResult vkres = vkCreateXlibSurfaceKHR(r_instance, &create_info, (const VkAllocationCallbacks*)allocator, (VkSurfaceKHR*)surface);
						if (vk_result != MU_NULL_PTR) {
							*((VkResult*)vk_result) = vkres;
						}
					#endif
				}

	#else

		#define MUCOSA_X11_CALL(...)

	#endif /* MUCOSA_X11 */

	/* Inner */

		struct muCOSA_inner {
			muWindowSystem system;
			void* context;
		};
		typedef struct muCOSA_inner muCOSA_inner;

		muWindowSystem muCOSA_best_window_system(void) {
			#ifdef MU_WIN32
				return MU_WINDOW_SYSTEM_WIN32;
			#elif defined(MU_UNIX)
				return MU_WINDOW_SYSTEM_X11;
			#endif
		}

		void muCOSA_inner_create(muCOSAResult* result, muCOSA_inner* inner, muWindowSystem system) {
			if (system == MU_WINDOW_SYSTEM_AUTO) {
				system = muCOSA_best_window_system();
			}
			inner->system = system;

			switch (system) {
				default: *result = MUCOSA_UNKNOWN_WINDOW_SYSTEM; return; break;

				MUCOSA_WIN32_CALL(
					case MU_WINDOW_SYSTEM_WIN32: {
						inner->context = mu_malloc(sizeof(muCOSA_Win32_context));
						if (!inner->context) {
							*result = MUCOSA_ALLOCATION_FAILED;
							return;
						}
						muCOSA_Win32_context_init((muCOSA_Win32_context*)inner->context);
					} break;
				)

				MUCOSA_X11_CALL(
					case MU_WINDOW_SYSTEM_X11: {
						inner->context = mu_malloc(sizeof(muCOSA_X11_context));
						if (!inner->context) {
							*result = MUCOSA_ALLOCATION_FAILED;
							return;
						}
						muCOSA_X11_context_init((muCOSA_X11_context*)inner->context);
					} break;
				)
			}
		}

		void muCOSA_inner_destroy(muCOSA_inner* inner) {
			switch (inner->system) {
				default: return; break;

				MUCOSA_WIN32_CALL(
					case MU_WINDOW_SYSTEM_WIN32: {
						muCOSA_Win32_context_term((muCOSA_Win32_context*)inner->context);
					} break;
				)

				MUCOSA_X11_CALL(
					case MU_WINDOW_SYSTEM_X11: {
						muCOSA_X11_context_term((muCOSA_X11_context*)inner->context);
					} break;
				)
			}
		}

	/* API-level functionality */

		#ifdef MUCOSA_NAMES
			#define MUCOSA_NAME_CALL(...) __VA_ARGS__
		#else
			#define MUCOSA_NAME_CALL(...)
		#endif

		/* Context */

			MUDEF void muCOSA_context_create(muCOSAContext* context, muWindowSystem system, muBool set_context) {
				context->inner = mu_malloc(sizeof(muCOSA_inner));
				if (!context->inner) {
					context->result = MUCOSA_ALLOCATION_FAILED;
					return;
				}

				muCOSAResult res = MUCOSA_SUCCESS;
				muCOSA_inner_create(&res, (muCOSA_inner*)context->inner, system);
				if (res != MUCOSA_SUCCESS) {
					context->result = res;
					mu_free(context->inner);
					return;
				}

				if (set_context) {
					muCOSA_context_set(context);
				}
			}

			MUDEF void muCOSA_context_destroy(muCOSAContext* context) {
				if (context->inner) {
					muCOSA_inner_destroy((muCOSA_inner*)context->inner);
					mu_free(context->inner);
				}
			}

			muCOSAContext* muCOSA_global_context = 0;

			MUDEF void muCOSA_context_set(muCOSAContext* context) {
				muCOSA_global_context = context;
			}

			MUDEF muWindowSystem muCOSA_context_get_window_system(muCOSAContext* context) {
				muCOSA_inner* inner = (muCOSA_inner*)context->inner;
				return inner->system;
			}

		/* Window */

			/* Creation / Destruction */

				MUDEF muWindowCreateInfo mu_window_default_create_info(void) {
					muWindowCreateInfo ci;

					ci.pixel_format.red_bits = 8;
					ci.pixel_format.green_bits = 8;
					ci.pixel_format.blue_bits = 8;
					ci.pixel_format.alpha_bits = 0;
					ci.pixel_format.depth_bits = 0;
					ci.pixel_format.stencil_bits = 0;
					ci.pixel_format.samples = 1;

					ci.visible = MU_TRUE;
					ci.resizable = MU_TRUE;
					ci.minimized = MU_FALSE;
					ci.maximized = MU_FALSE;
					ci.x = 50;
					ci.y = 50;
					ci.min_width = 120;
					ci.min_height = 1;
					ci.max_width = 30720;
					ci.max_height = 17280;
					ci.cursor_style = MU_CURSOR_STYLE_DEFAULT;

					ci.dimensions_callback = 0;
					ci.position_callback = 0;
					ci.focus_callback = 0;
					ci.maximize_callback = 0;
					ci.minimize_callback = 0;
					ci.cursor_position_callback = 0;
					ci.keyboard_key_callback = 0;
					ci.keyboard_state_callback = 0;
					ci.scroll_callback = 0;
					ci.mouse_button_callback = 0;

					return ci;
				}

				MUDEF muWindow mu_window_create(muGraphicsAPI api, muBool (*load_functions)(void), const char* name, uint16_m width, uint16_m height, muWindowCreateInfo create_info) {
					return muCOSA_window_create(muCOSA_global_context, &muCOSA_global_context->result, api, load_functions, name, width, height, create_info);
				}
				MUDEF muWindow mu_window_create_(muCOSAResult* result, muGraphicsAPI api, muBool (*load_functions)(void), const char* name, uint16_m width, uint16_m height, muWindowCreateInfo create_info) {
					return muCOSA_window_create(muCOSA_global_context, result, api, load_functions, name, width, height, create_info);
				}
				MUDEF muWindow muCOSA_window_create(muCOSAContext* context, muCOSAResult* result, muGraphicsAPI api, muBool (*load_functions)(void), const char* name, uint16_m width, uint16_m height, muWindowCreateInfo create_info) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return 0; break;

						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: {
							muCOSA_Win32_window* win = (muCOSA_Win32_window*)mu_malloc(sizeof(muCOSA_Win32_window));
							if (!win) {
								MU_SET_RESULT(result, MUCOSA_ALLOCATION_FAILED)
								return 0;
							}

							muCOSAResult res = MUCOSA_SUCCESS;
							muCOSA_Win32_window_create(&res, api, load_functions, name, width, height, create_info, win);
							if (res != MUCOSA_SUCCESS) { // ?
								MU_SET_RESULT(result, res)
								mu_free(win);
								return 0;
							}
							return win;
						} break; )

						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: {
							muCOSA_X11_window* win = (muCOSA_X11_window*)mu_malloc(sizeof(muCOSA_X11_window));
							if (!win) {
								MU_SET_RESULT(result, MUCOSA_ALLOCATION_FAILED)
								return 0;
							}

							muCOSAResult res = MUCOSA_SUCCESS;
							muCOSA_X11_window_create(win, &res, api, load_functions, name, width, height, create_info);
							if (res != MUCOSA_SUCCESS) {
								MU_SET_RESULT(result, res)
								mu_free(win);
								return 0;
							}
							return win;
						} break; )
					}

					return 0; if (context) {} if (result) {} if (api) {} if (load_functions) {} if (name) {} if (width) {} if (height) {} if (create_info.min_width) {}
				}

				MUDEF muWindow mu_window_destroy(muWindow window) {
					return muCOSA_window_destroy(muCOSA_global_context, &muCOSA_global_context->result, window);
				}
				MUDEF muWindow mu_window_destroy_(muCOSAResult* result, muWindow window) {
					return muCOSA_window_destroy(muCOSA_global_context, result, window);
				}
				MUDEF muWindow muCOSA_window_destroy(muCOSAContext* context, muCOSAResult* result, muWindow window) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return window; break;

						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: {
							muCOSA_Win32_window_destroy((muCOSA_Win32_window*)window);
							mu_free(window);
							return 0;
						} break; )

						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: {
							muCOSA_X11_window_destroy((muCOSA_X11_window*)window);
							mu_free(window);
							return 0;
						} break; )
					}

					if (result) {} if (window) {}
				}

			/* Main loop */

				MUDEF muBool mu_window_get_closed(muWindow window) {
					return muCOSA_window_get_closed(muCOSA_global_context, &muCOSA_global_context->result, window);
				}
				MUDEF muBool mu_window_get_closed_(muCOSAResult* result, muWindow window) {
					return muCOSA_window_get_closed(muCOSA_global_context, result, window);
				}
				MUDEF muBool muCOSA_window_get_closed(muCOSAContext* context, muCOSAResult* result, muWindow window) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return MU_TRUE; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: return muCOSA_Win32_window_get_closed((muCOSA_Win32_window*)window); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: return muCOSA_X11_window_get_closed((muCOSA_X11_window*)window); break;)
					}

					if (result) {} if (window) {}
				}

				MUDEF void mu_window_close(muWindow window) {
					muCOSA_window_close(muCOSA_global_context, &muCOSA_global_context->result, window);
				}
				MUDEF void mu_window_close_(muCOSAResult* result, muWindow window) {
					muCOSA_window_close(muCOSA_global_context, result, window);
				}
				MUDEF void muCOSA_window_close(muCOSAContext* context, muCOSAResult* result, muWindow window) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_close((muCOSA_Win32_window*)window); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_close((muCOSA_X11_window*)window); break;)
					}

					return; if (result) {} if (window) {}
				}

				MUDEF void mu_window_update(muWindow window) {
					muCOSA_window_update(muCOSA_global_context, &muCOSA_global_context->result, window);
				}
				MUDEF void mu_window_update_(muCOSAResult* result, muWindow window) {
					muCOSA_window_update(muCOSA_global_context, result, window);
				}
				MUDEF void muCOSA_window_update(muCOSAContext* context, muCOSAResult* result, muWindow window) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_update((muCOSA_Win32_window*)window); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_update((muCOSA_X11_window*)window); break;)
					}

					return; if (result) {} if (window) {}
				}

				MUDEF void mu_window_swap_buffers(muWindow window) {
					muCOSA_window_swap_buffers(muCOSA_global_context, &muCOSA_global_context->result, window);
				}
				MUDEF void mu_window_swap_buffers_(muCOSAResult* result, muWindow window) {
					muCOSA_window_swap_buffers(muCOSA_global_context, result, window);
				}
				MUDEF void muCOSA_window_swap_buffers(muCOSAContext* context, muCOSAResult* result, muWindow window) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_swap_buffers((muCOSA_Win32_window*)window); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_swap_buffers((muCOSA_X11_window*)window); break;)
					}

					return; if (result) {} if (window) {}
				}

			/* Get / Set */

				MUDEF muBool mu_window_get_focused(muWindow window) {
					return muCOSA_window_get_focused(muCOSA_global_context, &muCOSA_global_context->result, window);
				}
				MUDEF muBool mu_window_get_focused_(muCOSAResult* result, muWindow window) {
					return muCOSA_window_get_focused(muCOSA_global_context, result, window);
				}
				MUDEF muBool muCOSA_window_get_focused(muCOSAContext* context, muCOSAResult* result, muWindow window) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return MU_FALSE; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: return muCOSA_Win32_window_get_focused((muCOSA_Win32_window*)window); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: return muCOSA_X11_window_get_focused((muCOSA_X11_window*)window); break;)
					}

					if (result) {} if (window) {}
				}

				MUDEF void mu_window_focus(muWindow window) {
					muCOSA_window_focus(muCOSA_global_context, &muCOSA_global_context->result, window);
				}
				MUDEF void mu_window_focus_(muCOSAResult* result, muWindow window) {
					muCOSA_window_focus(muCOSA_global_context, result, window);
				}
				MUDEF void muCOSA_window_focus(muCOSAContext* context, muCOSAResult* result, muWindow window) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_focus((muCOSA_Win32_window*)window); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_focus(result, (muCOSA_X11_window*)window); break;)
					}

					return; if (result) {} if (window) {}
				}

				MUDEF muBool mu_window_get_visible(muWindow window) {
					return muCOSA_window_get_visible(muCOSA_global_context, &muCOSA_global_context->result, window);
				}
				MUDEF muBool mu_window_get_visible_(muCOSAResult* result, muWindow window) {
					return muCOSA_window_get_visible(muCOSA_global_context, result, window);
				}
				MUDEF muBool muCOSA_window_get_visible(muCOSAContext* context, muCOSAResult* result, muWindow window) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return MU_FALSE; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: return muCOSA_Win32_window_get_visible((muCOSA_Win32_window*)window); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: return muCOSA_X11_window_get_visible((muCOSA_X11_window*)window); break;)
					}

					if (result) {} if (window) {}
				}

				MUDEF void mu_window_set_visible(muWindow window, muBool visible) {
					muCOSA_window_set_visible(muCOSA_global_context, &muCOSA_global_context->result, window, visible);
				}
				MUDEF void mu_window_set_visible_(muCOSAResult* result, muWindow window, muBool visible) {
					muCOSA_window_set_visible(muCOSA_global_context, result, window, visible);
				}
				MUDEF void muCOSA_window_set_visible(muCOSAContext* context, muCOSAResult* result, muWindow window, muBool visible) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_visible((muCOSA_Win32_window*)window, visible); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_visible((muCOSA_X11_window*)window, visible); break;)
					}

					return; if (result) {} if (window) {} if (visible) {}
				}

				MUDEF void mu_window_get_position(muWindow window, int32_m* x, int32_m* y) {
					muCOSA_window_get_position(muCOSA_global_context, &muCOSA_global_context->result, window, x, y);
				}
				MUDEF void mu_window_get_position_(muCOSAResult* result, muWindow window, int32_m* x, int32_m* y) {
					muCOSA_window_get_position(muCOSA_global_context, result, window, x, y);
				}
				MUDEF void muCOSA_window_get_position(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m* x, int32_m* y) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_get_position(result, (muCOSA_Win32_window*)window, x, y); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_get_position((muCOSA_X11_window*)window, x, y); break;)
					}

					return; if (result) {} if (window) {} if (x) {} if (y) {}
				}

				MUDEF void mu_window_set_position(muWindow window, int32_m x, int32_m y) {
					muCOSA_window_set_position(muCOSA_global_context, &muCOSA_global_context->result, window, x, y);
				}
				MUDEF void mu_window_set_position_(muCOSAResult* result, muWindow window, int32_m x, int32_m y) {
					muCOSA_window_set_position(muCOSA_global_context, result, window, x, y);
				}
				MUDEF void muCOSA_window_set_position(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m x, int32_m y) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_position(result, (muCOSA_Win32_window*)window, x, y); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_position((muCOSA_X11_window*)window, x, y); break;)
					}

					return; if (result) {} if (window) {} if (x) {} if (y) {}
				}

				MUDEF void mu_window_get_dimensions(muWindow window, uint32_m* width, uint32_m* height) {
					muCOSA_window_get_dimensions(muCOSA_global_context, &muCOSA_global_context->result, window, width, height);
				}
				MUDEF void mu_window_get_dimensions_(muCOSAResult* result, muWindow window, uint32_m* width, uint32_m* height) {
					muCOSA_window_get_dimensions(muCOSA_global_context, result, window, width, height);
				}
				MUDEF void muCOSA_window_get_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m* width, uint32_m* height) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_get_dimensions(result, (muCOSA_Win32_window*)window, width, height); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_get_dimensions((muCOSA_X11_window*)window, width, height); break;)
					}

					return; if (result) {} if (window) {} if (width) {} if (height) {}
				}

				MUDEF void mu_window_set_dimensions(muWindow window, uint32_m width, uint32_m height) {
					muCOSA_window_set_dimensions(muCOSA_global_context, &muCOSA_global_context->result, window, width, height);
				}
				MUDEF void mu_window_set_dimensions_(muCOSAResult* result, muWindow window, uint32_m width, uint32_m height) {
					muCOSA_window_set_dimensions(muCOSA_global_context, result, window, width, height);
				}
				MUDEF void muCOSA_window_set_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m width, uint32_m height) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_dimensions(result, (muCOSA_Win32_window*)window, width, height); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_dimensions((muCOSA_X11_window*)window, width, height); break;)
					}

					return; if (result) {} if (window) {} if (width) {} if (height) {}
				}

				MUDEF muBool mu_window_get_maximized(muWindow window) {
					return muCOSA_window_get_maximized(muCOSA_global_context, &muCOSA_global_context->result, window);
				}
				MUDEF muBool mu_window_get_maximized_(muCOSAResult* result, muWindow window) {
					return muCOSA_window_get_maximized(muCOSA_global_context, result, window);
				}
				MUDEF muBool muCOSA_window_get_maximized(muCOSAContext* context, muCOSAResult* result, muWindow window) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return MU_FALSE; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: return muCOSA_Win32_window_get_maximized(result, (muCOSA_Win32_window*)window); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: return muCOSA_X11_window_get_maximized((muCOSA_X11_window*)window); break;)
					}

					if (result) {} if (window) {}
				}

				MUDEF void mu_window_set_maximized(muWindow window, muBool maximized) {
					muCOSA_window_set_maximized(muCOSA_global_context, &muCOSA_global_context->result, window, maximized);
				}
				MUDEF void mu_window_set_maximized_(muCOSAResult* result, muWindow window, muBool maximized) {
					muCOSA_window_set_maximized(muCOSA_global_context, result, window, maximized);
				}
				MUDEF void muCOSA_window_set_maximized(muCOSAContext* context, muCOSAResult* result, muWindow window, muBool maximized) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_maximized((muCOSA_Win32_window*)window, maximized); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_maximized((muCOSA_X11_window*)window, maximized); break;)
					}

					return; if (result) {} if (window) {} if (maximized) {}
				}

				MUDEF muBool mu_window_get_minimized(muWindow window) {
					return muCOSA_window_get_minimized(muCOSA_global_context, &muCOSA_global_context->result, window);
				}
				MUDEF muBool mu_window_get_minimized_(muCOSAResult* result, muWindow window) {
					return muCOSA_window_get_minimized(muCOSA_global_context, result, window);
				}
				MUDEF muBool muCOSA_window_get_minimized(muCOSAContext* context, muCOSAResult* result, muWindow window) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return MU_FALSE; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: return muCOSA_Win32_window_get_minimized((muCOSA_Win32_window*)window); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: return muCOSA_X11_window_get_minimized((muCOSA_X11_window*)window); break;)
					}

					if (result) {} if (window) {}
				}

				MUDEF void mu_window_set_minimized(muWindow window, muBool minimized) {
					muCOSA_window_set_minimized(muCOSA_global_context, &muCOSA_global_context->result, window, minimized);
				}
				MUDEF void mu_window_set_minimized_(muCOSAResult* result, muWindow window, muBool minimized) {
					muCOSA_window_set_minimized(muCOSA_global_context, result, window, minimized);
				}
				MUDEF void muCOSA_window_set_minimized(muCOSAContext* context, muCOSAResult* result, muWindow window, muBool minimized) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_minimized(result, (muCOSA_Win32_window*)window, minimized); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_minimized((muCOSA_X11_window*)window, minimized); break;)
					}

					return; if (result) {} if (window) {} if (minimized) {}
				}

				MUDEF void mu_window_get_minimum_dimensions(muWindow window, uint32_m* min_width, uint32_m* min_height) {
					muCOSA_window_get_minimum_dimensions(muCOSA_global_context, &muCOSA_global_context->result, window, min_width, min_height);
				}
				MUDEF void mu_window_get_minimum_dimensions_(muCOSAResult* result, muWindow window, uint32_m* min_width, uint32_m* min_height) {
					muCOSA_window_get_minimum_dimensions(muCOSA_global_context, result, window, min_width, min_height);
				}
				MUDEF void muCOSA_window_get_minimum_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m* min_width, uint32_m* min_height) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_get_minimum_dimensions((muCOSA_Win32_window*)window, min_width, min_height); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_get_minimum_dimensions((muCOSA_X11_window*)window, min_width, min_height); break;)
					}

					return; if (result) {} if (window) {} if (min_width) {} if (min_height) {}
				}

				MUDEF void mu_window_set_minimum_dimensions(muWindow window, uint32_m min_width, uint32_m min_height) {
					muCOSA_window_set_minimum_dimensions(muCOSA_global_context, &muCOSA_global_context->result, window, min_width, min_height);
				}
				MUDEF void mu_window_set_minimum_dimensions_(muCOSAResult* result, muWindow window, uint32_m min_width, uint32_m min_height) {
					muCOSA_window_set_minimum_dimensions(muCOSA_global_context, result, window, min_width, min_height);
				}
				MUDEF void muCOSA_window_set_minimum_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m min_width, uint32_m min_height) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_minimum_dimensions((muCOSA_Win32_window*)window, min_width, min_height); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_minimum_dimensions((muCOSA_X11_window*)window, min_width, min_height); break;)
					}

					return; if (result) {} if (window) {} if (min_width) {} if (min_height) {}
				}

				MUDEF void mu_window_get_maximum_dimensions(muWindow window, uint32_m* max_width, uint32_m* max_height) {
					muCOSA_window_get_maximum_dimensions(muCOSA_global_context, &muCOSA_global_context->result, window, max_width, max_height);
				}
				MUDEF void mu_window_get_maximum_dimensions_(muCOSAResult* result, muWindow window, uint32_m* max_width, uint32_m* max_height) {
					muCOSA_window_get_maximum_dimensions(muCOSA_global_context, result, window, max_width, max_height);
				}
				MUDEF void muCOSA_window_get_maximum_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m* max_width, uint32_m* max_height) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_get_maximum_dimensions((muCOSA_Win32_window*)window, max_width, max_height); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_get_maximum_dimensions((muCOSA_X11_window*)window, max_width, max_height); break;)
					}

					return; if (result) {} if (window) {} if (max_width) {} if (max_height) {}
				}

				MUDEF void mu_window_set_maximum_dimensions(muWindow window, uint32_m max_width, uint32_m max_height) {
					muCOSA_window_set_maximum_dimensions(muCOSA_global_context, &muCOSA_global_context->result, window, max_width, max_height);
				}
				MUDEF void mu_window_set_maximum_dimensions_(muCOSAResult* result, muWindow window, uint32_m max_width, uint32_m max_height) {
					muCOSA_window_set_maximum_dimensions(muCOSA_global_context, result, window, max_width, max_height);
				}
				MUDEF void muCOSA_window_set_maximum_dimensions(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m max_width, uint32_m max_height) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_maximum_dimensions((muCOSA_Win32_window*)window, max_width, max_height); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_maximum_dimensions((muCOSA_X11_window*)window, max_width, max_height); break;)
					}

					return; if (result) {} if (window) {} if (max_width) {} if (max_height) {}
				}

				MUDEF void mu_window_get_cursor_position(muWindow window, int32_m* x, int32_m* y) {
					muCOSA_window_get_cursor_position(muCOSA_global_context, &muCOSA_global_context->result, window, x, y);
				}
				MUDEF void mu_window_get_cursor_position_(muCOSAResult* result, muWindow window, int32_m* x, int32_m* y) {
					muCOSA_window_get_cursor_position(muCOSA_global_context, result, window, x, y);
				}
				MUDEF void muCOSA_window_get_cursor_position(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m* x, int32_m* y) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_get_cursor_position(result, (muCOSA_Win32_window*)window, x, y); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_get_cursor_position(result, (muCOSA_X11_window*)window, x, y); break;)
					}

					return; if (result) {} if (window) {} if (x) {} if (y) {}
				}

				MUDEF void mu_window_set_cursor_position(muWindow window, int32_m x, int32_m y) {
					muCOSA_window_set_cursor_position(muCOSA_global_context, &muCOSA_global_context->result, window, x, y);
				}
				MUDEF void mu_window_set_cursor_position_(muCOSAResult* result, muWindow window, int32_m x, int32_m y) {
					muCOSA_window_set_cursor_position(muCOSA_global_context, result, window, x, y);
				}
				MUDEF void muCOSA_window_set_cursor_position(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m x, int32_m y) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_cursor_position(result, (muCOSA_Win32_window*)window, x, y); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_cursor_position((muCOSA_X11_window*)window, x, y); break;)
					}

					return; if (result) {} if (window) {} if (x) {} if (y) {}
				}

				MUDEF muCursorStyle mu_window_get_cursor_style(muWindow window) {
					return muCOSA_window_get_cursor_style(muCOSA_global_context, &muCOSA_global_context->result, window);
				}
				MUDEF muCursorStyle mu_window_get_cursor_style_(muCOSAResult* result, muWindow window) {
					return muCOSA_window_get_cursor_style(muCOSA_global_context, result, window);
				}
				MUDEF muCursorStyle muCOSA_window_get_cursor_style(muCOSAContext* context, muCOSAResult* result, muWindow window) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return MU_CURSOR_STYLE_UNKNOWN; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: return muCOSA_Win32_window_get_cursor_style((muCOSA_Win32_window*)window); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: return muCOSA_X11_window_get_cursor_style((muCOSA_X11_window*)window); break;)
					}

					if (result) {} if (window) {}
				}

				MUDEF void mu_window_set_cursor_style(muWindow window, muCursorStyle style) {
					muCOSA_window_set_cursor_style(muCOSA_global_context, &muCOSA_global_context->result, window, style);
				}
				MUDEF void mu_window_set_cursor_style_(muCOSAResult* result, muWindow window, muCursorStyle style) {
					muCOSA_window_set_cursor_style(muCOSA_global_context, result, window, style);
				}
				MUDEF void muCOSA_window_set_cursor_style(muCOSAContext* context, muCOSAResult* result, muWindow window, muCursorStyle style) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_cursor_style((muCOSA_Win32_window*)window, style); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_cursor_style((muCOSA_X11_window*)window, style); break;)
					}

					return; if (result) {} if (window) {} if (style) {}
				}

				MUDEF int32_m mu_window_get_scroll_level(muWindow window) {
					return muCOSA_window_get_scroll_level(muCOSA_global_context, &muCOSA_global_context->result, window);
				}
				MUDEF int32_m mu_window_get_scroll_level_(muCOSAResult* result, muWindow window) {
					return muCOSA_window_get_scroll_level(muCOSA_global_context, result, window);
				}
				MUDEF int32_m muCOSA_window_get_scroll_level(muCOSAContext* context, muCOSAResult* result, muWindow window) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return 0; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: return muCOSA_Win32_window_get_scroll_level((muCOSA_Win32_window*)window); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: return muCOSA_X11_window_get_scroll_level((muCOSA_X11_window*)window); break;)
					}

					if (result) {} if (window) {}
				}

				MUDEF void mu_window_set_scroll_level(muWindow window, int32_m scroll_level) {
					muCOSA_window_set_scroll_level(muCOSA_global_context, &muCOSA_global_context->result, window, scroll_level);
				}
				MUDEF void mu_window_set_scroll_level_(muCOSAResult* result, muWindow window, int32_m scroll_level) {
					muCOSA_window_set_scroll_level(muCOSA_global_context, result, window, scroll_level);
				}
				MUDEF void muCOSA_window_set_scroll_level(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m scroll_level) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_scroll_level((muCOSA_Win32_window*)window, scroll_level); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_scroll_level((muCOSA_X11_window*)window, scroll_level); break;)
					}

					return; if (result) {} if (window) {} if (scroll_level) {}
				}

			/* Get / Let */

				MUDEF void mu_window_get_text_input_focus(muWindow window, int32_m text_cursor_x, int32_m text_cursor_y, void (*callback)(muWindow window, const char* input)) {
					muCOSA_window_get_text_input_focus(muCOSA_global_context, &muCOSA_global_context->result, window, text_cursor_x, text_cursor_y, callback);
				}
				MUDEF void mu_window_get_text_input_focus_(muCOSAResult* result, muWindow window, int32_m text_cursor_x, int32_m text_cursor_y, void (*callback)(muWindow window, const char* input)) {
					muCOSA_window_get_text_input_focus(muCOSA_global_context, result, window, text_cursor_x, text_cursor_y, callback);
				}
				MUDEF void muCOSA_window_get_text_input_focus(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m text_cursor_x, int32_m text_cursor_y, void (*callback)(muWindow window, const char* input)) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_get_text_input_focus(result, (muCOSA_Win32_window*)window, text_cursor_x, text_cursor_y, callback); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_get_text_input_focus(result, (muCOSA_X11_window*)window, text_cursor_x, text_cursor_y, callback); break;)
					}

					return; if (result) {} if (window) {} if (text_cursor_x) {} if (text_cursor_y) {} if (callback) {}
				}

				MUDEF void mu_window_update_text_cursor(muWindow window, int32_m x, int32_m y) {
					muCOSA_window_update_text_cursor(muCOSA_global_context, &muCOSA_global_context->result, window, x, y);
				}
				MUDEF void mu_window_update_text_cursor_(muCOSAResult* result, muWindow window, int32_m x, int32_m y) {
					muCOSA_window_update_text_cursor(muCOSA_global_context, result, window, x, y);
				}
				MUDEF void muCOSA_window_update_text_cursor(muCOSAContext* context, muCOSAResult* result, muWindow window, int32_m x, int32_m y) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_update_text_cursor((muCOSA_Win32_window*)window, x, y); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_update_text_cursor((muCOSA_X11_window*)window, x, y); break;)
					}

					return; if (result) {} if (window) {} if (x) {} if (y) {}
				}

				MUDEF void mu_window_let_text_input_focus(muWindow window) {
					muCOSA_window_let_text_input_focus(muCOSA_global_context, &muCOSA_global_context->result, window);
				}
				MUDEF void mu_window_let_text_input_focus_(muCOSAResult* result, muWindow window) {
					muCOSA_window_let_text_input_focus(muCOSA_global_context, result, window);
				}
				MUDEF void muCOSA_window_let_text_input_focus(muCOSAContext* context, muCOSAResult* result, muWindow window) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_let_text_input_focus((muCOSA_Win32_window*)window); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_let_text_input_focus((muCOSA_X11_window*)window); break;)
					}

					return; if (result) {} if (window) {}
				}

			/* Get */

				MUDEF void mu_window_get_frame_extents(muWindow window, uint32_m* left, uint32_m* right, uint32_m* top, uint32_m* bottom) {
					muCOSA_window_get_frame_extents(muCOSA_global_context, &muCOSA_global_context->result, window, left, right, top, bottom);
				}
				MUDEF void mu_window_get_frame_extents_(muCOSAResult* result, muWindow window, uint32_m* left, uint32_m* right, uint32_m* top, uint32_m* bottom) {
					muCOSA_window_get_frame_extents(muCOSA_global_context, result, window, left, right, top, bottom);
				}
				MUDEF void muCOSA_window_get_frame_extents(muCOSAContext* context, muCOSAResult* result, muWindow window, uint32_m* left, uint32_m* right, uint32_m* top, uint32_m* bottom) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_get_frame_extents(result, (muCOSA_Win32_window*)window, left, right, top, bottom); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_get_frame_extents((muCOSA_X11_window*)window, left, right, top, bottom); break;)
					}

					return; if (result) { } if (window) {} if (left) {} if (right) {} if (top) {} if (bottom) {}
				}

				MUDEF muButtonState mu_window_get_keyboard_key_state(muWindow window, muKeyboardKey key) {
					return muCOSA_window_get_keyboard_key_state(muCOSA_global_context, &muCOSA_global_context->result, window, key);
				}
				MUDEF muButtonState mu_window_get_keyboard_key_state_(muCOSAResult* result, muWindow window, muKeyboardKey key) {
					return muCOSA_window_get_keyboard_key_state(muCOSA_global_context, result, window, key);
				}
				MUDEF muButtonState muCOSA_window_get_keyboard_key_state(muCOSAContext* context, muCOSAResult* result, muWindow window, muKeyboardKey key) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return 0; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: return muCOSA_Win32_window_get_keyboard_key_state((muCOSA_Win32_window*)window, key); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: return muCOSA_X11_window_get_keyboard_key_state((muCOSA_X11_window*)window, key); break;)
					}

					if (result) { } if (window) {} if (key) {}
				}

				MUDEF muState mu_window_get_keyboard_state_state(muWindow window, muKeyboardState state) {
					return muCOSA_window_get_keyboard_state_state(muCOSA_global_context, &muCOSA_global_context->result, window, state);
				}
				MUDEF muState mu_window_get_keyboard_state_state_(muCOSAResult* result, muWindow window, muKeyboardState state) {
					return muCOSA_window_get_keyboard_state_state(muCOSA_global_context, result, window, state);
				}
				MUDEF muState muCOSA_window_get_keyboard_state_state(muCOSAContext* context, muCOSAResult* result, muWindow window, muKeyboardState state) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return 0; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: return muCOSA_Win32_window_get_keyboard_state_state((muCOSA_Win32_window*)window, state); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: return muCOSA_X11_window_get_keyboard_state_state((muCOSA_X11_window*)window, state); break;)
					}

					if (result) { } if (window) {} if (state) {}
				}

				MUDEF muButtonState mu_window_get_mouse_button_state(muWindow window, muMouseButton button) {
					return muCOSA_window_get_mouse_button_state(muCOSA_global_context, &muCOSA_global_context->result, window, button);
				}
				MUDEF muButtonState mu_window_get_mouse_button_state_(muCOSAResult* result, muWindow window, muMouseButton button) {
					return muCOSA_window_get_mouse_button_state(muCOSA_global_context, result, window, button);
				}
				MUDEF muButtonState muCOSA_window_get_mouse_button_state(muCOSAContext* context, muCOSAResult* result, muWindow window, muMouseButton button) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return 0; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: return muCOSA_Win32_window_get_mouse_button_state((muCOSA_Win32_window*)window, button); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: return muCOSA_X11_window_get_mouse_button_state((muCOSA_X11_window*)window, button); break;)
					}

					if (result) { } if (window) {} if (button) {}
				}

			/* Set */

				MUDEF void mu_window_set_title(muWindow window, const char* title) {
					muCOSA_window_set_title(muCOSA_global_context, &muCOSA_global_context->result, window, title);
				}
				MUDEF void mu_window_set_title_(muCOSAResult* result, muWindow window, const char* title) {
					muCOSA_window_set_title(muCOSA_global_context, result, window, title);
				}
				MUDEF void muCOSA_window_set_title(muCOSAContext* context, muCOSAResult* result, muWindow window, const char* title) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_title(result, (muCOSA_Win32_window*)window, title); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_title((muCOSA_X11_window*)window, title); break;)
					}

					return; if (result) { } if (window) {} if (title) {}
				}

				MUDEF void mu_window_set_dimensions_callback(muWindow window, void (*callback)(muWindow window, uint32_m width, uint32_m height)) {
					muCOSA_window_set_dimensions_callback(muCOSA_global_context, &muCOSA_global_context->result, window, callback);
				}
				MUDEF void mu_window_set_dimensions_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, uint32_m width, uint32_m height)) {
					muCOSA_window_set_dimensions_callback(muCOSA_global_context, result, window, callback);
				}
				MUDEF void muCOSA_window_set_dimensions_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, uint32_m width, uint32_m height)) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_dimensions_callback((muCOSA_Win32_window*)window, callback); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_dimensions_callback((muCOSA_X11_window*)window, callback); break;)
					}

					return; if (result) { } if (window) {} if (callback) {}
				}

				MUDEF void mu_window_set_position_callback(muWindow window, void (*callback)(muWindow window, int32_m x, int32_m y)) {
					muCOSA_window_set_position_callback(muCOSA_global_context, &muCOSA_global_context->result, window, callback);
				}
				MUDEF void mu_window_set_position_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m x, int32_m y)) {
					muCOSA_window_set_position_callback(muCOSA_global_context, result, window, callback);
				}
				MUDEF void muCOSA_window_set_position_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m x, int32_m y)) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_position_callback((muCOSA_Win32_window*)window, callback); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_position_callback((muCOSA_X11_window*)window, callback); break;)
					}

					return; if (result) { } if (window) {} if (callback) {}
				}

				MUDEF void mu_window_set_focus_callback(muWindow window, void (*callback)(muWindow window, muBool focused)) {
					muCOSA_window_set_focus_callback(muCOSA_global_context, &muCOSA_global_context->result, window, callback);
				}
				MUDEF void mu_window_set_focus_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool focused)) {
					muCOSA_window_set_focus_callback(muCOSA_global_context, result, window, callback);
				}
				MUDEF void muCOSA_window_set_focus_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool focused)) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_focus_callback((muCOSA_Win32_window*)window, callback); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_focus_callback((muCOSA_X11_window*)window, callback); break;)
					}

					return; if (result) { } if (window) {} if (callback) {}
				}

				MUDEF void mu_window_set_maximize_callback(muWindow window, void (*callback)(muWindow window, muBool maximized)) {
					muCOSA_window_set_maximize_callback(muCOSA_global_context, &muCOSA_global_context->result, window, callback);
				}
				MUDEF void mu_window_set_maximize_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool maximized)) {
					muCOSA_window_set_maximize_callback(muCOSA_global_context, result, window, callback);
				}
				MUDEF void muCOSA_window_set_maximize_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool maximized)) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_maximize_callback((muCOSA_Win32_window*)window, callback); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_maximize_callback((muCOSA_X11_window*)window, callback); break;)
					}

					return; if (result) { } if (window) {} if (callback) {}
				}

				MUDEF void mu_window_set_minimize_callback(muWindow window, void (*callback)(muWindow window, muBool minimized)) {
					muCOSA_window_set_minimize_callback(muCOSA_global_context, &muCOSA_global_context->result, window, callback);
				}
				MUDEF void mu_window_set_minimize_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool minimized)) {
					muCOSA_window_set_minimize_callback(muCOSA_global_context, result, window, callback);
				}
				MUDEF void muCOSA_window_set_minimize_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool minimized)) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_minimize_callback((muCOSA_Win32_window*)window, callback); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_minimize_callback((muCOSA_X11_window*)window, callback); break;)
					}

					return; if (result) { } if (window) {} if (callback) {}
				}

				MUDEF void mu_window_set_keyboard_key_callback(muWindow window, void (*callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state)) {
					muCOSA_window_set_keyboard_key_callback(muCOSA_global_context, &muCOSA_global_context->result, window, callback);
				}
				MUDEF void mu_window_set_keyboard_key_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state)) {
					muCOSA_window_set_keyboard_key_callback(muCOSA_global_context, result, window, callback);
				}
				MUDEF void muCOSA_window_set_keyboard_key_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state)) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_keyboard_key_callback((muCOSA_Win32_window*)window, callback); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_keyboard_key_callback((muCOSA_X11_window*)window, callback); break;)
					}

					return; if (result) { } if (window) {} if (callback) {}
				}

				MUDEF void mu_window_set_keyboard_state_callback(muWindow window, void (*callback)(muWindow window, muKeyboardState keyboard_state, muState state)) {
					muCOSA_window_set_keyboard_state_callback(muCOSA_global_context, &muCOSA_global_context->result, window, callback);
				}
				MUDEF void mu_window_set_keyboard_state_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardState keyboard_state, muState state)) {
					muCOSA_window_set_keyboard_state_callback(muCOSA_global_context, result, window, callback);
				}
				MUDEF void muCOSA_window_set_keyboard_state_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardState keyboard_state, muState state)) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_keyboard_state_callback((muCOSA_Win32_window*)window, callback); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_keyboard_state_callback((muCOSA_X11_window*)window, callback); break;)
					}

					return; if (result) { } if (window) {} if (callback) {}
				}

				MUDEF void mu_window_set_cursor_position_callback(muWindow window, void (*callback)(muWindow window, int32_m x, int32_m y)) {
					muCOSA_window_set_cursor_position_callback(muCOSA_global_context, &muCOSA_global_context->result, window, callback);
				}
				MUDEF void mu_window_set_cursor_position_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m x, int32_m y)) {
					muCOSA_window_set_cursor_position_callback(muCOSA_global_context, result, window, callback);
				}
				MUDEF void muCOSA_window_set_cursor_position_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m x, int32_m y)) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_cursor_position_callback((muCOSA_Win32_window*)window, callback); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_cursor_position_callback((muCOSA_X11_window*)window, callback); break;)
					}

					return; if (result) { } if (window) {} if (callback) {}
				}

				MUDEF void mu_window_set_mouse_button_callback(muWindow window, void (*callback)(muWindow window, muMouseButton mouse_button, muButtonState state)) {
					muCOSA_window_set_mouse_button_callback(muCOSA_global_context, &muCOSA_global_context->result, window, callback);
				}
				MUDEF void mu_window_set_mouse_button_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muMouseButton mouse_button, muButtonState state)) {
					muCOSA_window_set_mouse_button_callback(muCOSA_global_context, result, window, callback);
				}
				MUDEF void muCOSA_window_set_mouse_button_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muMouseButton mouse_button, muButtonState state)) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_mouse_button_callback((muCOSA_Win32_window*)window, callback); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_mouse_button_callback((muCOSA_X11_window*)window, callback); break;)
					}

					return; if (result) { } if (window) {} if (callback) {}
				}

				MUDEF void mu_window_set_scroll_callback(muWindow window, void (*callback)(muWindow window, int32_m scroll_level_add)) {
					muCOSA_window_set_scroll_callback(muCOSA_global_context, &muCOSA_global_context->result, window, callback);
				}
				MUDEF void mu_window_set_scroll_callback_(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m scroll_level_add)) {
					muCOSA_window_set_scroll_callback(muCOSA_global_context, result, window, callback);
				}
				MUDEF void muCOSA_window_set_scroll_callback(muCOSAContext* context, muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m scroll_level_add)) {
					muCOSA_inner* inner = (muCOSA_inner*)context->inner;

					switch (inner->system) {
						default: return; break;
						MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_window_set_scroll_callback((muCOSA_Win32_window*)window, callback); break;)
						MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_window_set_scroll_callback((muCOSA_X11_window*)window, callback); break;)
					}

					return; if (result) { } if (window) {} if (callback) {}
				}

		/* Time */

			MUDEF double muCOSA_time_get(muCOSAContext* context, muCOSAResult* result) {
				muCOSA_inner* inner = (muCOSA_inner*)context->inner;

				switch (inner->system) {
					default: return 0.f; break;
					MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: return muCOSA_Win32_time_get((muCOSA_Win32_context*)inner->context, result); break;)
					MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: return muCOSA_X11_time_get(&((muCOSA_X11_context*)inner->context)->time); break;)
				}

				if (result) {}
			}
			MUDEF double mu_time_get_(muCOSAResult* result) {
				return muCOSA_time_get(muCOSA_global_context, result);
			}
			MUDEF double mu_time_get(void) {
				return muCOSA_time_get(muCOSA_global_context, &muCOSA_global_context->result);
			}

			MUDEF void muCOSA_time_set(muCOSAContext* context, muCOSAResult* result, double time) {
				muCOSA_inner* inner = (muCOSA_inner*)context->inner;

				switch (inner->system) {
					default: return; break;
					MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_time_set((muCOSA_Win32_context*)inner->context, result, time); break;)
					MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_X11_time_set(&((muCOSA_X11_context*)inner->context)->time, time); break;)
				}

				return; if (result) {} if (time) {}
			}
			MUDEF void mu_time_set_(muCOSAResult* result, double time) {
				muCOSA_time_set(muCOSA_global_context, result, time);
			}
			MUDEF void mu_time_set(double time) {
				muCOSA_time_set(muCOSA_global_context, &muCOSA_global_context->result, time);
			}

			MUDEF void muCOSA_sleep(muCOSAContext* context, muCOSAResult* result, double time) {
				muCOSA_inner* inner = (muCOSA_inner*)context->inner;

				switch (inner->system) {
					default: return; break;
					MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_sleep((muCOSA_Win32_context*)inner->context, result, time); break;)
					MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_sleep(time); break;)
				}

				return; if (result) {} if (time) {}
			}
			MUDEF void mu_sleep_(muCOSAResult* result, double time) {
				muCOSA_sleep(muCOSA_global_context, result, time);
			}
			MUDEF void mu_sleep(double time) {
				muCOSA_sleep(muCOSA_global_context, &muCOSA_global_context->result, time);
			}

		/* Clipboard */

			MUDEF char* mu_clipboard_get(void) {
				return muCOSA_clipboard_get(muCOSA_global_context, &muCOSA_global_context->result);
			}
			MUDEF char* mu_clipboard_get_(muCOSAResult* result) {
				return muCOSA_clipboard_get(muCOSA_global_context, result);
			}
			MUDEF char* muCOSA_clipboard_get(muCOSAContext* context, muCOSAResult* result) {
				muCOSA_inner* inner = (muCOSA_inner*)context->inner;

				switch (inner->system) {
					default: return 0; break;
					MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: return muCOSA_Win32_clipboard_get(result); break;)
					MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: return (char*)muCOSA_X11_clipboard_get(result); break;)
				}

				if (result) { }
			}

			MUDEF void mu_clipboard_set(const char* text, size_m text_size) {
				muCOSA_clipboard_set(muCOSA_global_context, &muCOSA_global_context->result, text, text_size);
			}
			MUDEF void mu_clipboard_set_(muCOSAResult* result, const char* text, size_m text_size) {
				muCOSA_clipboard_set(muCOSA_global_context, result, text, text_size);
			}
			MUDEF void muCOSA_clipboard_set(muCOSAContext* context, muCOSAResult* result, const char* text, size_m text_size) {
				muCOSA_inner* inner = (muCOSA_inner*)context->inner;

				switch (inner->system) {
					default: return; break;
					MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_clipboard_set(result, text, text_size); break;)
					MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_clipboard_set(result, &((muCOSA_X11_context*)inner->context)->clipboard, text, text_size); break;)
				}

				return; if (result) { } if (text) {} if (text_size) {}
			}

		/* OS functions */

			MUDEF void* mu_os_get_window_handle(muWindow window, muWindowHandle handle) {
				return muCOSA_os_get_window_handle(muCOSA_global_context, &muCOSA_global_context->result, window, handle);
			}
			MUDEF void* mu_os_get_window_handle_(muCOSAResult* result, muWindow window, muWindowHandle handle) {
				return muCOSA_os_get_window_handle(muCOSA_global_context, result, window, handle);
			}
			MUDEF void* muCOSA_os_get_window_handle(muCOSAContext* context, muCOSAResult* result, muWindow window, muWindowHandle handle) {
				muCOSA_inner* inner = (muCOSA_inner*)context->inner;

				switch (inner->system) {
					default: return 0; break;
					MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: return muCOSA_Win32_os_get_window_handle(result, (muCOSA_Win32_window*)window, handle); break;)
					MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: return muCOSA_X11_os_get_window_handle(result, (muCOSA_X11_window*)window, handle); break;)
				}

				if (result) { } if (window) {} if (handle) {}
			}

		/* OpenGL */

			MUDEF void mu_opengl_bind_window(muWindow window) {
				muCOSA_opengl_bind_window(muCOSA_global_context, &muCOSA_global_context->result, window);
			}
			MUDEF void mu_opengl_bind_window_(muCOSAResult* result, muWindow window) {
				muCOSA_opengl_bind_window(muCOSA_global_context, result, window);
			}
			MUDEF void muCOSA_opengl_bind_window(muCOSAContext* context, muCOSAResult* result, muWindow window) {
				muCOSA_inner* inner = (muCOSA_inner*)context->inner;

				switch (inner->system) {
					default: return; break;
					MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_opengl_bind_window(result, (muCOSA_Win32_window*)window); break;)
					MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_opengl_bind_window(result, (muCOSA_X11_window*)window); break;)
				}

				return; if (result) { } if (window) {}
			}

			MUDEF void* mu_opengl_get_function_address(const char* name) {
				return muCOSA_opengl_get_function_address(muCOSA_global_context, name);
			}
			MUDEF void* muCOSA_opengl_get_function_address(muCOSAContext* context, const char* name) {
				muCOSA_inner* inner = (muCOSA_inner*)context->inner;

				switch (inner->system) {
					default: return 0; break;
					MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: return muCOSA_Win32_opengl_get_function_address(name); break;)
					MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: return muCOSA_X11_opengl_get_function_address(name); break;)
				}

				if (name) {}
			}

			MUDEF void mu_opengl_window_swap_interval(muWindow window, int interval) {
				muCOSA_opengl_window_swap_interval(muCOSA_global_context, &muCOSA_global_context->result, window, interval);
			}
			MUDEF void mu_opengl_window_swap_interval_(muCOSAResult* result, muWindow window, int interval) {
				muCOSA_opengl_window_swap_interval(muCOSA_global_context, result, window, interval);
			}
			MUDEF void muCOSA_opengl_window_swap_interval(muCOSAContext* context, muCOSAResult* result, muWindow window, int interval) {
				muCOSA_inner* inner = (muCOSA_inner*)context->inner;

				switch (inner->system) {
					default: return; break;
					MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_opengl_window_swap_interval(result, (muCOSA_Win32_window*)window, interval); break;)
					MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_opengl_window_swap_interval(result, (muCOSA_X11_window*)window, interval); break;)
				}

				return; if (result) { } if (window) {} if (interval) {}
			}

		/* Vulkan */

			// Note: also not necessarily UTF-8
			MUDEF const char** mu_vulkan_get_surface_instance_extensions(size_m* count) {
				return muCOSA_vulkan_get_surface_instance_extensions(muCOSA_global_context, &muCOSA_global_context->result, count);
			}
			MUDEF const char** mu_vulkan_get_surface_instance_extensions_(muCOSAResult* result, size_m* count) {
				return muCOSA_vulkan_get_surface_instance_extensions(muCOSA_global_context, result, count);
			}
			MUDEF const char** muCOSA_vulkan_get_surface_instance_extensions(muCOSAContext* context, muCOSAResult* result, size_m* count) {
				muCOSA_inner* inner = (muCOSA_inner*)context->inner;

				switch (inner->system) {
					default: return 0; break;
					MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: return muCOSA_Win32_vulkan_get_surface_instance_extensions(result, count); break;)
					MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: return muCOSA_X11_vulkan_get_surface_instance_extensions(result, count); break;)
				}

				if (result) { } if (count) {}
			}

			// vk_result interpreted as VkResult*, instance as VkInstance*, allocator as const VkAllocationCallbacks*, and surface as VkSurfaceKHR*
			MUDEF void mu_vulkan_create_window_surface(muWindow window, void* vk_result, void* instance, void* allocator, void* surface) {
				muCOSA_vulkan_create_window_surface(muCOSA_global_context, &muCOSA_global_context->result, window, vk_result, instance, allocator, surface);
			}
			MUDEF void mu_vulkan_create_window_surface_(muCOSAResult* result, muWindow window, void* vk_result, void* instance, void* allocator, void* surface) {
				muCOSA_vulkan_create_window_surface(muCOSA_global_context, result, window, vk_result, instance, allocator, surface);
			}
			MUDEF void muCOSA_vulkan_create_window_surface(muCOSAContext* context, muCOSAResult* result, muWindow window, void* vk_result, void* instance, void* allocator, void* surface) {
				muCOSA_inner* inner = (muCOSA_inner*)context->inner;

				switch (inner->system) {
					default: return; break;
					MUCOSA_WIN32_CALL(case MU_WINDOW_SYSTEM_WIN32: muCOSA_Win32_vulkan_create_window_surface(result, (muCOSA_Win32_window*)window, vk_result, instance, allocator, surface); break;)
					MUCOSA_X11_CALL(case MU_WINDOW_SYSTEM_X11: muCOSA_X11_vulkan_create_window_surface(result, (muCOSA_X11_window*)window, vk_result, instance, allocator, surface); break;)
				}

				return; if (result) { } if (window) {} if (vk_result) {} if (instance) {} if (allocator) {} if (surface) {}
			}

		/* Names */

			#ifdef MUCOSA_NAMES
				MUDEF const char* muCOSA_result_get_name(muCOSAResult result) {
					switch (result) {
						default: return "MUCOSA_UNKNOWN"; break;
						case MUCOSA_SUCCESS: return "MUCOSA_SUCCESS"; break;
						case MUCOSA_ALREADY_INITIALIZED: return "MUCOSA_ALREADY_INITIALIZED"; break;
						case MUCOSA_ALREADY_TERMINATED: return "MUCOSA_ALREADY_TERMINATED"; break;
						case MUCOSA_ALREADY_ACTIVE: return "MUCOSA_ALREADY_ACTIVE"; break;
						case MUCOSA_ALREADY_INACTIVE: return "MUCOSA_ALREADY_INACTIVE"; break;
						case MUCOSA_NOT_YET_INITIALIZED: return "MUCOSA_NOT_YET_INITIALIZED"; break;
						case MUCOSA_ALLOCATION_FAILED: return "MUCOSA_ALLOCATION_FAILED"; break;
						case MUCOSA_UNKNOWN_WINDOW_SYSTEM: return "MUCOSA_UNKNOWN_WINDOW_SYSTEM"; break;
						case MUCOSA_UNKNOWN_GRAPHICS_API: return "MUCOSA_UNKNOWN_GRAPHICS_API"; break;
						case MUCOSA_UNKNOWN_KEYBOARD_KEY: return "MUCOSA_UNKNOWN_KEYBOARD_KEY"; break;
						case MUCOSA_UNKNOWN_KEYBOARD_STATE: return "MUCOSA_UNKNOWN_KEYBOARD_STATE"; break;
						case MUCOSA_UNKNOWN_MOUSE_BUTTON: return "MUCOSA_UNKNOWN_MOUSE_BUTTON"; break;
						case MUCOSA_UNKNOWN_WINDOW_HANDLE: return "MUCOSA_UNKNOWN_WINDOW_HANDLE"; break;
						case MUCOSA_UNSUPPORTED_WINDOW_SYSTEM: return "MUCOSA_UNSUPPORTED_WINDOW_SYSTEM"; break;
						case MUCOSA_UNSUPPORTED_FEATURE: return "MUCOSA_UNSUPPORTED_FEATURE"; break;
						case MUCOSA_UNSUPPORTED_OPENGL_FEATURE: return "MUCOSA_UNSUPPORTED_OPENGL_FEATURE"; break;
						case MUCOSA_UNSUPPORTED_GRAPHICS_API: return "MUCOSA_UNSUPPORTED_GRAPHICS_API"; break;
						case MUCOSA_FAILED_CONNECTION_TO_SERVER: return "MUCOSA_FAILED_CONNECTION_TO_SERVER"; break;
						case MUCOSA_FAILED_CREATE_WINDOW: return "MUCOSA_FAILED_CREATE_WINDOW"; break;
						case MUCOSA_FAILED_LOAD_FUNCTIONS: return "MUCOSA_FAILED_LOAD_FUNCTIONS"; break;
						case MUCOSA_FAILED_FIND_COMPATIBLE_FRAMEBUFFER: return "MUCOSA_FAILED_FIND_COMPATIBLE_FRAMEBUFFER"; break;
						case MUCOSA_FAILED_CREATE_OPENGL_CONTEXT: return "MUCOSA_FAILED_CREATE_OPENGL_CONTEXT"; break;
						case MUCOSA_FAILED_LOAD_OPENGL_CONTEXT: return "MUCOSA_FAILED_LOAD_OPENGL_CONTEXT"; break;
						case MUCOSA_FAILED_USE_PIXEL_FORMAT: return "MUCOSA_FAILED_USE_PIXEL_FORMAT"; break;
						case MUCOSA_FAILED_JOIN_THREAD: return "MUCOSA_FAILED_JOIN_THREAD"; break;
						case MUCOSA_FAILED_CREATE_THREAD: return "MUCOSA_FAILED_CREATE_THREAD"; break;
						case MUCOSA_FAILED_CREATE_INPUT_METHOD: return "MUCOSA_FAILED_CREATE_INPUT_METHOD"; break;
						case MUCOSA_FAILED_GET_INPUT_STYLES: return "MUCOSA_FAILED_GET_INPUT_STYLES"; break;
						case MUCOSA_FAILED_FIND_COMPATIBLE_INPUT_STYLE: return "MUCOSA_FAILED_FIND_COMPATIBLE_INPUT_STYLE"; break;
						case MUCOSA_FAILED_CREATE_INPUT_CONTEXT: return "MUCOSA_FAILED_CREATE_INPUT_CONTEXT"; break;
						case MUCOSA_FAILED_REGISTER_WINDOW_CLASS: return "MUCOSA_FAILED_REGISTER_WINDOW_CLASS"; break;
						case MUCOSA_FAILED_CONVERT_UTF8_TO_WCHAR: return "MUCOSA_FAILED_CONVERT_UTF8_TO_WCHAR"; break;
						case MUCOSA_FAILED_CONVERT_WCHAR_TO_UTF8: return "MUCOSA_FAILED_CONVERT_WCHAR_TO_UTF8"; break;
						case MUCOSA_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS: return "MUCOSA_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS"; break;
						case MUCOSA_FAILED_CREATE_DUMMY_WGL_WINDOW: return "MUCOSA_FAILED_CREATE_DUMMY_WGL_WINDOW"; break;
						case MUCOSA_FAILED_FIND_COMPATIBLE_PIXEL_FORMAT: return "MUCOSA_FAILED_FIND_COMPATIBLE_PIXEL_FORMAT"; break;
						case MUCOSA_FAILED_DESCRIBE_PIXEL_FORMAT: return "MUCOSA_FAILED_DESCRIBE_PIXEL_FORMAT"; break;
						case MUCOSA_FAILED_SET_PIXEL_FORMAT: return "MUCOSA_FAILED_SET_PIXEL_FORMAT"; break;
						case MUCOSA_FAILED_QUERY_WINDOW_INFO: return "MUCOSA_FAILED_QUERY_WINDOW_INFO"; break;
						case MUCOSA_FAILED_SET_WINDOW_INFO: return "MUCOSA_FAILED_SET_WINDOW_INFO"; break;
						case MUCOSA_FAILED_ASSOCIATE_IME_CONTEXT: return "MUCOSA_FAILED_ASSOCIATE_IME_CONTEXT"; break;
						case MUCOSA_FAILED_LET_IMM_CONTEXT: return "MUCOSA_FAILED_LET_IMM_CONTEXT"; break;
						case MUCOSA_FAILED_GLOBAL_ALLOCATION: return "MUCOSA_FAILED_GLOBAL_ALLOCATION"; break;
						case MUCOSA_FAILED_GLOBAL_LOCK: return "MUCOSA_FAILED_GLOBAL_LOCK"; break;
						case MUCOSA_FAILED_HOLD_CLIPBOARD: return "MUCOSA_FAILED_HOLD_CLIPBOARD"; break;
						case MUCOSA_FAILED_SET_CLIPBOARD: return "MUCOSA_FAILED_SET_CLIPBOARD"; break;
						case MUCOSA_INVALID_MINIMUM_MAXIMUM_BOOLS: return "MUCOSA_INVALID_MINIMUM_MAXIMUM_BOOLS"; break;
						case MUCOSA_INVALID_MINIMUM_MAXIMUM_DIMENSIONS: return "MUCOSA_INVALID_MINIMUM_MAXIMUM_DIMENSIONS"; break;
						case MUCOSA_INVALID_ID: return "MUCOSA_INVALID_ID"; break;
						case MUCOSA_INVALID_SAMPLE_COUNT: return "MUCOSA_INVALID_SAMPLE_COUNT"; break;
						case MUCOSA_INVALID_DIMENSIONS: return "MUCOSA_INVALID_DIMENSIONS"; break;
						case MUCOSA_INVALID_POINTER: return "MUCOSA_INVALID_POINTER"; break;
						case MUCOSA_INVALID_WINDOW_STATE: return "MUCOSA_INVALID_WINDOW_STATE"; break;
						case MUCOSA_INVALID_TIME: return "MUCOSA_INVALID_TIME"; break;
						case MUCOSA_NONEXISTENT_DEVICE: return "MUCOSA_NONEXISTENT_DEVICE"; break;
						case MUCOSA_OVERSIZED_CLIPBOARD: return "MUCOSA_OVERSIZED_CLIPBOARD"; break;
						case MUCOSA_WINDOW_NON_RESIZABLE: return "MUCOSA_WINDOW_NON_RESIZABLE"; break;
					}
				}

				MUDEF const char* mu_window_system_get_name(muWindowSystem system) {
					switch (system) {
						default: return "MU_WINDOW_SYSTEM_UNKNOWN"; break;
						case MU_WINDOW_SYSTEM_AUTO: return "MU_WINDOW_SYSTEM_AUTO"; break;
						case MU_WINDOW_SYSTEM_X11: return "MU_WINDOW_SYSTEM_X11"; break;
						case MU_WINDOW_SYSTEM_WIN32: return "MU_WINDOW_SYSTEM_WIN32"; break;
					}
				}

				MUDEF const char* mu_window_system_get_nice_name(muWindowSystem system) {
					switch (system) {
						default: return "Unknown"; break;
						case MU_WINDOW_SYSTEM_AUTO: return "Auto"; break;
						case MU_WINDOW_SYSTEM_X11: return "X11"; break;
						case MU_WINDOW_SYSTEM_WIN32: return "Win32"; break;
					}
				}

				MUDEF const char* mu_window_handle_get_name(muWindowHandle handle) {
					switch (handle) {
						default: return "UNKNOWN"; break;
						case MU_WINDOWS_HWND: return "MU_WINDOWS_HWND"; break;
						case MU_X11_DISPLAY: return "MU_X11_DISPLAY"; break;
						case MU_X11_WINDOW: return "MU_X11_WINDOW"; break;
						case MU_X11_PARENT_WINDOW: return "MU_X11_PARENT_WINDOW"; break;
					}
				}

				MUDEF const char* mu_window_handle_get_nice_name(muWindowHandle handle) {
					switch (handle) {
						default: return "Unknown"; break;
						case MU_WINDOWS_HWND: return "HWND"; break;
						case MU_X11_DISPLAY: return "Display"; break;
						case MU_X11_WINDOW: return "Window"; break;
						case MU_X11_PARENT_WINDOW: return "Parent Window"; break;
					}
				}

				MUDEF const char* mu_graphics_api_get_name(muGraphicsAPI api) {
					switch (api) {
						default: return "UNKNOWN"; break;
						case MU_NO_GRAPHICS_API: return "MU_NO_GRAPHICS_API"; break;
						case MU_OPENGL_1_0: return "MU_OPENGL_1_0"; break;
						case MU_OPENGL_1_1: return "MU_OPENGL_1_1"; break;
						case MU_OPENGL_1_2: return "MU_OPENGL_1_2"; break;
						case MU_OPENGL_1_2_1: return "MU_OPENGL_1_2_1"; break;
						case MU_OPENGL_1_3: return "MU_OPENGL_1_3"; break;
						case MU_OPENGL_1_4: return "MU_OPENGL_1_4"; break;
						case MU_OPENGL_1_5: return "MU_OPENGL_1_5"; break;
						case MU_OPENGL_2_0: return "MU_OPENGL_2_0"; break;
						case MU_OPENGL_2_1: return "MU_OPENGL_2_1"; break;
						case MU_OPENGL_3_0: return "MU_OPENGL_3_0"; break;
						case MU_OPENGL_3_1: return "MU_OPENGL_3_1"; break;
						case MU_OPENGL_3_2_CORE:          return "MU_OPENGL_3_2_CORE"; break;
						case MU_OPENGL_3_2_COMPATIBILITY: return "MU_OPENGL_3_2_COMPATIBILITY"; break;
						case MU_OPENGL_3_3_CORE:          return "MU_OPENGL_3_3_CORE"; break;
						case MU_OPENGL_3_3_COMPATIBILITY: return "MU_OPENGL_3_3_COMPATIBILITY"; break;
						case MU_OPENGL_4_0_CORE:          return "MU_OPENGL_4_0_CORE"; break;
						case MU_OPENGL_4_0_COMPATIBILITY: return "MU_OPENGL_4_0_COMPATIBILITY"; break;
						case MU_OPENGL_4_1_CORE:          return "MU_OPENGL_4_1_CORE"; break;
						case MU_OPENGL_4_1_COMPATIBILITY: return "MU_OPENGL_4_1_COMPATIBILITY"; break;
						case MU_OPENGL_4_2_CORE:          return "MU_OPENGL_4_2_CORE"; break;
						case MU_OPENGL_4_2_COMPATIBILITY: return "MU_OPENGL_4_2_COMPATIBILITY"; break;
						case MU_OPENGL_4_3_CORE:          return "MU_OPENGL_4_3_CORE"; break;
						case MU_OPENGL_4_3_COMPATIBILITY: return "MU_OPENGL_4_3_COMPATIBILITY"; break;
						case MU_OPENGL_4_4_CORE:          return "MU_OPENGL_4_4_CORE"; break;
						case MU_OPENGL_4_4_COMPATIBILITY: return "MU_OPENGL_4_4_COMPATIBILITY"; break;
						case MU_OPENGL_4_5_CORE:          return "MU_OPENGL_4_5_CORE"; break;
						case MU_OPENGL_4_5_COMPATIBILITY: return "MU_OPENGL_4_5_COMPATIBILITY"; break;
						case MU_OPENGL_4_6_CORE:          return "MU_OPENGL_4_6_CORE"; break;
						case MU_OPENGL_4_6_COMPATIBILITY: return "MU_OPENGL_4_6_COMPATIBILITY"; break;
					}
				}

				MUDEF const char* mu_graphics_api_get_nice_name(muGraphicsAPI api) {
					switch (api) {
						default: return "Unknown"; break;
						case MU_NO_GRAPHICS_API: return "None"; break;
						case MU_OPENGL_1_0: return "OpenGL 1.0"; break;
						case MU_OPENGL_1_1: return "OpenGL 1.1"; break;
						case MU_OPENGL_1_2: return "OpenGL 1.2"; break;
						case MU_OPENGL_1_2_1: return "OpenGL 1.2.1"; break;
						case MU_OPENGL_1_3: return "OpenGL 1.3"; break;
						case MU_OPENGL_1_4: return "OpenGL 1.4"; break;
						case MU_OPENGL_1_5: return "OpenGL 1.5"; break;
						case MU_OPENGL_2_0: return "OpenGL 2.0"; break;
						case MU_OPENGL_2_1: return "OpenGL 2.1"; break;
						case MU_OPENGL_3_0: return "OpenGL 3.0"; break;
						case MU_OPENGL_3_1: return "OpenGL 3.1"; break;
						case MU_OPENGL_3_2_CORE:          return "OpenGL 3.2 Core"; break;
						case MU_OPENGL_3_2_COMPATIBILITY: return "OpenGL 3.2 Compatibility"; break;
						case MU_OPENGL_3_3_CORE:          return "OpenGL 3.3 Core"; break;
						case MU_OPENGL_3_3_COMPATIBILITY: return "OpenGL 3.3 Compatibility"; break;
						case MU_OPENGL_4_0_CORE:          return "OpenGL 4.0 Core"; break;
						case MU_OPENGL_4_0_COMPATIBILITY: return "OpenGL 4.0 Compatibility"; break;
						case MU_OPENGL_4_1_CORE:          return "OpenGL 4.1 Core"; break;
						case MU_OPENGL_4_1_COMPATIBILITY: return "OpenGL 4.1 Compatibility"; break;
						case MU_OPENGL_4_2_CORE:          return "OpenGL 4.2 Core"; break;
						case MU_OPENGL_4_2_COMPATIBILITY: return "OpenGL 4.2 Compatibility"; break;
						case MU_OPENGL_4_3_CORE:          return "OpenGL 4.3 Core"; break;
						case MU_OPENGL_4_3_COMPATIBILITY: return "OpenGL 4.3 Compatibility"; break;
						case MU_OPENGL_4_4_CORE:          return "OpenGL 4.4 Core"; break;
						case MU_OPENGL_4_4_COMPATIBILITY: return "OpenGL 4.4 Compatibility"; break;
						case MU_OPENGL_4_5_CORE:          return "OpenGL 4.5 Core"; break;
						case MU_OPENGL_4_5_COMPATIBILITY: return "OpenGL 4.5 Compatibility"; break;
						case MU_OPENGL_4_6_CORE:          return "OpenGL 4.6 Core"; break;
						case MU_OPENGL_4_6_COMPATIBILITY: return "OpenGL 4.6 Compatibility"; break;
					}
				}

				MUDEF const char* mu_cursor_style_get_name(muCursorStyle style) {
					switch (style) {
						default: return "MU_CURSOR_STYLE_UNKNOWN"; break;
						case MU_CURSOR_STYLE_DEFAULT: return "MU_CURSOR_STYLE_DEFAULT"; break;
						case MU_CURSOR_STYLE_ARROW: return "MU_CURSOR_STYLE_ARROW"; break;
						case MU_CURSOR_STYLE_IBEAM: return "MU_CURSOR_STYLE_IBEAM"; break;
						case MU_CURSOR_STYLE_WAIT: return "MU_CURSOR_STYLE_WAIT"; break;
						case MU_CURSOR_STYLE_WAIT_ARROW: return "MU_CURSOR_STYLE_WAIT_ARROW"; break;
						case MU_CURSOR_STYLE_CROSSHAIR: return "MU_CURSOR_STYLE_CROSSHAIR"; break;
						case MU_CURSOR_STYLE_HAND: return "MU_CURSOR_STYLE_HAND"; break;
						case MU_CURSOR_STYLE_SIZE_EAST_WEST: return "MU_CURSOR_STYLE_SIZE_EAST_WEST"; break;
						case MU_CURSOR_STYLE_SIZE_NORTH_SOUTH: return "MU_CURSOR_STYLE_SIZE_NORTH_SOUTH"; break;
						case MU_CURSOR_STYLE_SIZE_NORTH_EAST_SOUTH_WEST: return "MU_CURSOR_STYLE_SIZE_NORTH_EAST_SOUTH_WEST"; break;
						case MU_CURSOR_STYLE_SIZE_NORTH_WEST_SOUTH_EAST: return "MU_CURSOR_STYLE_SIZE_NORTH_WEST_SOUTH_EAST"; break;
						case MU_CURSOR_STYLE_SIZE_ALL: return "MU_CURSOR_STYLE_SIZE_ALL"; break;
						case MU_CURSOR_STYLE_NO: return "MU_CURSOR_STYLE_NO"; break;
					}
				}

				MUDEF const char* mu_cursor_style_get_nice_name(muCursorStyle style) {
					switch (style) {
						default: return "Unknown"; break;
						case MU_CURSOR_STYLE_DEFAULT: return "Default"; break;
						case MU_CURSOR_STYLE_ARROW: return "Arrow"; break;
						case MU_CURSOR_STYLE_IBEAM: return "I-Beam"; break;
						case MU_CURSOR_STYLE_WAIT: return "Wait"; break;
						case MU_CURSOR_STYLE_WAIT_ARROW: return "Wait Arrow"; break;
						case MU_CURSOR_STYLE_CROSSHAIR: return "Crosshair"; break;
						case MU_CURSOR_STYLE_HAND: return "Hand"; break;
						case MU_CURSOR_STYLE_SIZE_EAST_WEST: return "Size East-West"; break;
						case MU_CURSOR_STYLE_SIZE_NORTH_SOUTH: return "Size North-South"; break;
						case MU_CURSOR_STYLE_SIZE_NORTH_EAST_SOUTH_WEST: return "Size North-East South-West"; break;
						case MU_CURSOR_STYLE_SIZE_NORTH_WEST_SOUTH_EAST: return "Size North-West South-East"; break;
						case MU_CURSOR_STYLE_SIZE_ALL: return "Size All"; break;
						case MU_CURSOR_STYLE_NO: return "No"; break;
					}
				}

				MUDEF const char* mu_keyboard_key_get_name(muKeyboardKey key) {
					switch (key) {
						default: return "MU_KEYBOARD_KEY_UNKNOWN"; break;
						case MU_KEYBOARD_KEY_BACKSPACE: return "MU_KEYBOARD_KEY_BACKSPACE"; break;
						case MU_KEYBOARD_KEY_TAB: return "MU_KEYBOARD_KEY_TAB"; break;
						case MU_KEYBOARD_KEY_CLEAR: return "MU_KEYBOARD_KEY_CLEAR"; break;
						case MU_KEYBOARD_KEY_RETURN: return "MU_KEYBOARD_KEY_RETURN"; break;
						case MU_KEYBOARD_KEY_PAUSE: return "MU_KEYBOARD_KEY_PAUSE"; break;
						case MU_KEYBOARD_KEY_ESCAPE: return "MU_KEYBOARD_KEY_ESCAPE"; break;
						case MU_KEYBOARD_KEY_MODECHANGE: return "MU_KEYBOARD_KEY_MODECHANGE"; break;
						case MU_KEYBOARD_KEY_SPACE: return "MU_KEYBOARD_KEY_SPACE"; break;
						case MU_KEYBOARD_KEY_PRIOR: return "MU_KEYBOARD_KEY_PRIOR"; break;
						case MU_KEYBOARD_KEY_NEXT: return "MU_KEYBOARD_KEY_NEXT"; break;
						case MU_KEYBOARD_KEY_END: return "MU_KEYBOARD_KEY_END"; break;
						case MU_KEYBOARD_KEY_HOME: return "MU_KEYBOARD_KEY_HOME"; break;
						case MU_KEYBOARD_KEY_LEFT: return "MU_KEYBOARD_KEY_LEFT"; break;
						case MU_KEYBOARD_KEY_UP: return "MU_KEYBOARD_KEY_UP"; break;
						case MU_KEYBOARD_KEY_RIGHT: return "MU_KEYBOARD_KEY_RIGHT"; break;
						case MU_KEYBOARD_KEY_DOWN: return "MU_KEYBOARD_KEY_DOWN"; break;
						case MU_KEYBOARD_KEY_SELECT: return "MU_KEYBOARD_KEY_SELECT"; break;
						case MU_KEYBOARD_KEY_PRINT: return "MU_KEYBOARD_KEY_PRINT"; break;
						case MU_KEYBOARD_KEY_EXECUTE: return "MU_KEYBOARD_KEY_EXECUTE"; break;
						case MU_KEYBOARD_KEY_INSERT: return "MU_KEYBOARD_KEY_INSERT"; break;
						case MU_KEYBOARD_KEY_DELETE: return "MU_KEYBOARD_KEY_DELETE"; break;
						case MU_KEYBOARD_KEY_HELP: return "MU_KEYBOARD_KEY_HELP"; break;
						case MU_KEYBOARD_KEY_0: return "MU_KEYBOARD_KEY_0"; break;
						case MU_KEYBOARD_KEY_1: return "MU_KEYBOARD_KEY_1"; break;
						case MU_KEYBOARD_KEY_2: return "MU_KEYBOARD_KEY_2"; break;
						case MU_KEYBOARD_KEY_3: return "MU_KEYBOARD_KEY_3"; break;
						case MU_KEYBOARD_KEY_4: return "MU_KEYBOARD_KEY_4"; break;
						case MU_KEYBOARD_KEY_5: return "MU_KEYBOARD_KEY_5"; break;
						case MU_KEYBOARD_KEY_6: return "MU_KEYBOARD_KEY_6"; break;
						case MU_KEYBOARD_KEY_7: return "MU_KEYBOARD_KEY_7"; break;
						case MU_KEYBOARD_KEY_8: return "MU_KEYBOARD_KEY_8"; break;
						case MU_KEYBOARD_KEY_9: return "MU_KEYBOARD_KEY_9"; break;
						case MU_KEYBOARD_KEY_A: return "MU_KEYBOARD_KEY_A"; break;
						case MU_KEYBOARD_KEY_B: return "MU_KEYBOARD_KEY_B"; break;
						case MU_KEYBOARD_KEY_C: return "MU_KEYBOARD_KEY_C"; break;
						case MU_KEYBOARD_KEY_D: return "MU_KEYBOARD_KEY_D"; break;
						case MU_KEYBOARD_KEY_E: return "MU_KEYBOARD_KEY_E"; break;
						case MU_KEYBOARD_KEY_F: return "MU_KEYBOARD_KEY_F"; break;
						case MU_KEYBOARD_KEY_G: return "MU_KEYBOARD_KEY_G"; break;
						case MU_KEYBOARD_KEY_H: return "MU_KEYBOARD_KEY_H"; break;
						case MU_KEYBOARD_KEY_I: return "MU_KEYBOARD_KEY_I"; break;
						case MU_KEYBOARD_KEY_J: return "MU_KEYBOARD_KEY_J"; break;
						case MU_KEYBOARD_KEY_K: return "MU_KEYBOARD_KEY_K"; break;
						case MU_KEYBOARD_KEY_L: return "MU_KEYBOARD_KEY_L"; break;
						case MU_KEYBOARD_KEY_M: return "MU_KEYBOARD_KEY_M"; break;
						case MU_KEYBOARD_KEY_N: return "MU_KEYBOARD_KEY_N"; break;
						case MU_KEYBOARD_KEY_O: return "MU_KEYBOARD_KEY_O"; break;
						case MU_KEYBOARD_KEY_P: return "MU_KEYBOARD_KEY_P"; break;
						case MU_KEYBOARD_KEY_Q: return "MU_KEYBOARD_KEY_Q"; break;
						case MU_KEYBOARD_KEY_R: return "MU_KEYBOARD_KEY_R"; break;
						case MU_KEYBOARD_KEY_S: return "MU_KEYBOARD_KEY_S"; break;
						case MU_KEYBOARD_KEY_T: return "MU_KEYBOARD_KEY_T"; break;
						case MU_KEYBOARD_KEY_U: return "MU_KEYBOARD_KEY_U"; break;
						case MU_KEYBOARD_KEY_V: return "MU_KEYBOARD_KEY_V"; break;
						case MU_KEYBOARD_KEY_W: return "MU_KEYBOARD_KEY_W"; break;
						case MU_KEYBOARD_KEY_X: return "MU_KEYBOARD_KEY_X"; break;
						case MU_KEYBOARD_KEY_Y: return "MU_KEYBOARD_KEY_Y"; break;
						case MU_KEYBOARD_KEY_Z: return "MU_KEYBOARD_KEY_Z"; break;
						case MU_KEYBOARD_KEY_LEFT_WINDOWS: return "MU_KEYBOARD_KEY_LEFT_WINDOWS"; break;
						case MU_KEYBOARD_KEY_RIGHT_WINDOWS: return "MU_KEYBOARD_KEY_RIGHT_WINDOWS"; break;
						case MU_KEYBOARD_KEY_NUMPAD_0: return "MU_KEYBOARD_KEY_NUMPAD_0"; break;
						case MU_KEYBOARD_KEY_NUMPAD_1: return "MU_KEYBOARD_KEY_NUMPAD_1"; break;
						case MU_KEYBOARD_KEY_NUMPAD_2: return "MU_KEYBOARD_KEY_NUMPAD_2"; break;
						case MU_KEYBOARD_KEY_NUMPAD_3: return "MU_KEYBOARD_KEY_NUMPAD_3"; break;
						case MU_KEYBOARD_KEY_NUMPAD_4: return "MU_KEYBOARD_KEY_NUMPAD_4"; break;
						case MU_KEYBOARD_KEY_NUMPAD_5: return "MU_KEYBOARD_KEY_NUMPAD_5"; break;
						case MU_KEYBOARD_KEY_NUMPAD_6: return "MU_KEYBOARD_KEY_NUMPAD_6"; break;
						case MU_KEYBOARD_KEY_NUMPAD_7: return "MU_KEYBOARD_KEY_NUMPAD_7"; break;
						case MU_KEYBOARD_KEY_NUMPAD_8: return "MU_KEYBOARD_KEY_NUMPAD_8"; break;
						case MU_KEYBOARD_KEY_NUMPAD_9: return "MU_KEYBOARD_KEY_NUMPAD_9"; break;
						case MU_KEYBOARD_KEY_MULTIPLY: return "MU_KEYBOARD_KEY_MULTIPLY"; break;
						case MU_KEYBOARD_KEY_ADD: return "MU_KEYBOARD_KEY_ADD"; break;
						case MU_KEYBOARD_KEY_SEPARATOR: return "MU_KEYBOARD_KEY_SEPARATOR"; break;
						case MU_KEYBOARD_KEY_SUBTRACT: return "MU_KEYBOARD_KEY_SUBTRACT"; break;
						case MU_KEYBOARD_KEY_DECIMAL: return "MU_KEYBOARD_KEY_DECIMAL"; break;
						case MU_KEYBOARD_KEY_DIVIDE: return "MU_KEYBOARD_KEY_DIVIDE"; break;
						case MU_KEYBOARD_KEY_F1: return "MU_KEYBOARD_KEY_F1"; break;
						case MU_KEYBOARD_KEY_F2: return "MU_KEYBOARD_KEY_F2"; break;
						case MU_KEYBOARD_KEY_F3: return "MU_KEYBOARD_KEY_F3"; break;
						case MU_KEYBOARD_KEY_F4: return "MU_KEYBOARD_KEY_F4"; break;
						case MU_KEYBOARD_KEY_F5: return "MU_KEYBOARD_KEY_F5"; break;
						case MU_KEYBOARD_KEY_F6: return "MU_KEYBOARD_KEY_F6"; break;
						case MU_KEYBOARD_KEY_F7: return "MU_KEYBOARD_KEY_F7"; break;
						case MU_KEYBOARD_KEY_F8: return "MU_KEYBOARD_KEY_F8"; break;
						case MU_KEYBOARD_KEY_F9: return "MU_KEYBOARD_KEY_F9"; break;
						case MU_KEYBOARD_KEY_F10: return "MU_KEYBOARD_KEY_F10"; break;
						case MU_KEYBOARD_KEY_F11: return "MU_KEYBOARD_KEY_F11"; break;
						case MU_KEYBOARD_KEY_F12: return "MU_KEYBOARD_KEY_F12"; break;
						case MU_KEYBOARD_KEY_F13: return "MU_KEYBOARD_KEY_F13"; break;
						case MU_KEYBOARD_KEY_F14: return "MU_KEYBOARD_KEY_F14"; break;
						case MU_KEYBOARD_KEY_F15: return "MU_KEYBOARD_KEY_F15"; break;
						case MU_KEYBOARD_KEY_F16: return "MU_KEYBOARD_KEY_F16"; break;
						case MU_KEYBOARD_KEY_F17: return "MU_KEYBOARD_KEY_F17"; break;
						case MU_KEYBOARD_KEY_F18: return "MU_KEYBOARD_KEY_F18"; break;
						case MU_KEYBOARD_KEY_F19: return "MU_KEYBOARD_KEY_F19"; break;
						case MU_KEYBOARD_KEY_F20: return "MU_KEYBOARD_KEY_F20"; break;
						case MU_KEYBOARD_KEY_F21: return "MU_KEYBOARD_KEY_F21"; break;
						case MU_KEYBOARD_KEY_F22: return "MU_KEYBOARD_KEY_F22"; break;
						case MU_KEYBOARD_KEY_F23: return "MU_KEYBOARD_KEY_F23"; break;
						case MU_KEYBOARD_KEY_F24: return "MU_KEYBOARD_KEY_F24"; break;
						case MU_KEYBOARD_KEY_NUMLOCK: return "MU_KEYBOARD_KEY_NUMLOCK"; break;
						case MU_KEYBOARD_KEY_SCROLL: return "MU_KEYBOARD_KEY_SCROLL"; break;
						case MU_KEYBOARD_KEY_LEFT_SHIFT: return "MU_KEYBOARD_KEY_LEFT_SHIFT"; break;
						case MU_KEYBOARD_KEY_RIGHT_SHIFT: return "MU_KEYBOARD_KEY_RIGHT_SHIFT"; break;
						case MU_KEYBOARD_KEY_LEFT_CONTROL: return "MU_KEYBOARD_KEY_LEFT_CONTROL"; break;
						case MU_KEYBOARD_KEY_RIGHT_CONTROL: return "MU_KEYBOARD_KEY_RIGHT_CONTROL"; break;
						case MU_KEYBOARD_KEY_LEFT_MENU: return "MU_KEYBOARD_KEY_LEFT_MENU"; break;
						case MU_KEYBOARD_KEY_RIGHT_MENU: return "MU_KEYBOARD_KEY_RIGHT_MENU"; break;
						case MU_KEYBOARD_KEY_ATTN: return "MU_KEYBOARD_KEY_ATTN"; break;
						case MU_KEYBOARD_KEY_CRSEL: return "MU_KEYBOARD_KEY_CRSEL"; break;
						case MU_KEYBOARD_KEY_EXSEL: return "MU_KEYBOARD_KEY_EXSEL"; break;
						case MU_KEYBOARD_KEY_EREOF: return "MU_KEYBOARD_KEY_EREOF"; break;
						case MU_KEYBOARD_KEY_PLAY: return "MU_KEYBOARD_KEY_PLAY"; break;
						case MU_KEYBOARD_KEY_PA1: return "MU_KEYBOARD_KEY_PA1"; break;
					}
				}

				MUDEF const char* mu_keyboard_key_get_nice_name(muKeyboardKey key) {
					switch (key) {
						default: return "Unknown"; break;
						case MU_KEYBOARD_KEY_BACKSPACE: return "Backspace"; break;
						case MU_KEYBOARD_KEY_TAB: return "Tab"; break;
						case MU_KEYBOARD_KEY_CLEAR: return "Clear"; break;
						case MU_KEYBOARD_KEY_RETURN: return "Return"; break;
						case MU_KEYBOARD_KEY_PAUSE: return "Pause"; break;
						case MU_KEYBOARD_KEY_ESCAPE: return "Escape"; break;
						case MU_KEYBOARD_KEY_MODECHANGE: return "Mode Change"; break;
						case MU_KEYBOARD_KEY_SPACE: return "Space"; break;
						case MU_KEYBOARD_KEY_PRIOR: return "Prior"; break;
						case MU_KEYBOARD_KEY_NEXT: return "Next"; break;
						case MU_KEYBOARD_KEY_END: return "End"; break;
						case MU_KEYBOARD_KEY_HOME: return "Home"; break;
						case MU_KEYBOARD_KEY_LEFT: return "Left"; break;
						case MU_KEYBOARD_KEY_UP: return "Up"; break;
						case MU_KEYBOARD_KEY_RIGHT: return "Right"; break;
						case MU_KEYBOARD_KEY_DOWN: return "Down"; break;
						case MU_KEYBOARD_KEY_SELECT: return "Select"; break;
						case MU_KEYBOARD_KEY_PRINT: return "Print"; break;
						case MU_KEYBOARD_KEY_EXECUTE: return "Execute"; break;
						case MU_KEYBOARD_KEY_INSERT: return "Insert"; break;
						case MU_KEYBOARD_KEY_DELETE: return "Delete"; break;
						case MU_KEYBOARD_KEY_HELP: return "Help"; break;
						case MU_KEYBOARD_KEY_0: return "0"; break;
						case MU_KEYBOARD_KEY_1: return "1"; break;
						case MU_KEYBOARD_KEY_2: return "2"; break;
						case MU_KEYBOARD_KEY_3: return "3"; break;
						case MU_KEYBOARD_KEY_4: return "4"; break;
						case MU_KEYBOARD_KEY_5: return "5"; break;
						case MU_KEYBOARD_KEY_6: return "6"; break;
						case MU_KEYBOARD_KEY_7: return "7"; break;
						case MU_KEYBOARD_KEY_8: return "8"; break;
						case MU_KEYBOARD_KEY_9: return "9"; break;
						case MU_KEYBOARD_KEY_A: return "A"; break;
						case MU_KEYBOARD_KEY_B: return "B"; break;
						case MU_KEYBOARD_KEY_C: return "C"; break;
						case MU_KEYBOARD_KEY_D: return "D"; break;
						case MU_KEYBOARD_KEY_E: return "E"; break;
						case MU_KEYBOARD_KEY_F: return "F"; break;
						case MU_KEYBOARD_KEY_G: return "G"; break;
						case MU_KEYBOARD_KEY_H: return "H"; break;
						case MU_KEYBOARD_KEY_I: return "I"; break;
						case MU_KEYBOARD_KEY_J: return "J"; break;
						case MU_KEYBOARD_KEY_K: return "K"; break;
						case MU_KEYBOARD_KEY_L: return "L"; break;
						case MU_KEYBOARD_KEY_M: return "M"; break;
						case MU_KEYBOARD_KEY_N: return "N"; break;
						case MU_KEYBOARD_KEY_O: return "O"; break;
						case MU_KEYBOARD_KEY_P: return "P"; break;
						case MU_KEYBOARD_KEY_Q: return "Q"; break;
						case MU_KEYBOARD_KEY_R: return "R"; break;
						case MU_KEYBOARD_KEY_S: return "S"; break;
						case MU_KEYBOARD_KEY_T: return "T"; break;
						case MU_KEYBOARD_KEY_U: return "U"; break;
						case MU_KEYBOARD_KEY_V: return "V"; break;
						case MU_KEYBOARD_KEY_W: return "W"; break;
						case MU_KEYBOARD_KEY_X: return "X"; break;
						case MU_KEYBOARD_KEY_Y: return "Y"; break;
						case MU_KEYBOARD_KEY_Z: return "Z"; break;
						case MU_KEYBOARD_KEY_LEFT_WINDOWS: return "Left Windows"; break;
						case MU_KEYBOARD_KEY_RIGHT_WINDOWS: return "Right Windows"; break;
						case MU_KEYBOARD_KEY_NUMPAD_0: return "Numpad 0"; break;
						case MU_KEYBOARD_KEY_NUMPAD_1: return "Numpad 1"; break;
						case MU_KEYBOARD_KEY_NUMPAD_2: return "Numpad 2"; break;
						case MU_KEYBOARD_KEY_NUMPAD_3: return "Numpad 3"; break;
						case MU_KEYBOARD_KEY_NUMPAD_4: return "Numpad 4"; break;
						case MU_KEYBOARD_KEY_NUMPAD_5: return "Numpad 5"; break;
						case MU_KEYBOARD_KEY_NUMPAD_6: return "Numpad 6"; break;
						case MU_KEYBOARD_KEY_NUMPAD_7: return "Numpad 7"; break;
						case MU_KEYBOARD_KEY_NUMPAD_8: return "Numpad 8"; break;
						case MU_KEYBOARD_KEY_NUMPAD_9: return "Numpad 9"; break;
						case MU_KEYBOARD_KEY_MULTIPLY: return "Multiply"; break;
						case MU_KEYBOARD_KEY_ADD: return "Add"; break;
						case MU_KEYBOARD_KEY_SEPARATOR: return "Separator"; break;
						case MU_KEYBOARD_KEY_SUBTRACT: return "Subtract"; break;
						case MU_KEYBOARD_KEY_DECIMAL: return "Decimal"; break;
						case MU_KEYBOARD_KEY_DIVIDE: return "Divide"; break;
						case MU_KEYBOARD_KEY_F1: return "F1"; break;
						case MU_KEYBOARD_KEY_F2: return "F2"; break;
						case MU_KEYBOARD_KEY_F3: return "F3"; break;
						case MU_KEYBOARD_KEY_F4: return "F4"; break;
						case MU_KEYBOARD_KEY_F5: return "F5"; break;
						case MU_KEYBOARD_KEY_F6: return "F6"; break;
						case MU_KEYBOARD_KEY_F7: return "F7"; break;
						case MU_KEYBOARD_KEY_F8: return "F8"; break;
						case MU_KEYBOARD_KEY_F9: return "F9"; break;
						case MU_KEYBOARD_KEY_F10: return "F10"; break;
						case MU_KEYBOARD_KEY_F11: return "F11"; break;
						case MU_KEYBOARD_KEY_F12: return "F12"; break;
						case MU_KEYBOARD_KEY_F13: return "F13"; break;
						case MU_KEYBOARD_KEY_F14: return "F14"; break;
						case MU_KEYBOARD_KEY_F15: return "F15"; break;
						case MU_KEYBOARD_KEY_F16: return "F16"; break;
						case MU_KEYBOARD_KEY_F17: return "F17"; break;
						case MU_KEYBOARD_KEY_F18: return "F18"; break;
						case MU_KEYBOARD_KEY_F19: return "F19"; break;
						case MU_KEYBOARD_KEY_F20: return "F20"; break;
						case MU_KEYBOARD_KEY_F21: return "F21"; break;
						case MU_KEYBOARD_KEY_F22: return "F22"; break;
						case MU_KEYBOARD_KEY_F23: return "F23"; break;
						case MU_KEYBOARD_KEY_F24: return "F24"; break;
						case MU_KEYBOARD_KEY_NUMLOCK: return "Num Lock"; break;
						case MU_KEYBOARD_KEY_SCROLL: return "Key Scroll"; break;
						case MU_KEYBOARD_KEY_LEFT_SHIFT: return "Left Shift"; break;
						case MU_KEYBOARD_KEY_RIGHT_SHIFT: return "Right Shift"; break;
						case MU_KEYBOARD_KEY_LEFT_CONTROL: return "Left Control"; break;
						case MU_KEYBOARD_KEY_RIGHT_CONTROL: return "Right Control"; break;
						case MU_KEYBOARD_KEY_LEFT_MENU: return "Left Menu"; break;
						case MU_KEYBOARD_KEY_RIGHT_MENU: return "Right Menu"; break;
						case MU_KEYBOARD_KEY_ATTN: return "Attention"; break;
						case MU_KEYBOARD_KEY_CRSEL: return "CrSel"; break;
						case MU_KEYBOARD_KEY_EXSEL: return "ExSel"; break;
						case MU_KEYBOARD_KEY_EREOF: return "Erase End Of File (EREOF)"; break;
						case MU_KEYBOARD_KEY_PLAY: return "Play"; break;
						case MU_KEYBOARD_KEY_PA1: return "PA1"; break;
					}
				}

				MUDEF const char* mu_keyboard_state_get_name(muKeyboardState state) {
					switch (state) {
						default: return "MU_KEYBOARD_STATE_UNKNOWN"; break;
						case MU_KEYBOARD_STATE_CAPS_LOCK: return "MU_KEYBOARD_STATE_CAPS_LOCK"; break;
						case MU_KEYBOARD_STATE_SCROLL_LOCK: return "MU_KEYBOARD_STATE_SCROLL_LOCK"; break;
						case MU_KEYBOARD_STATE_NUM_LOCK: return "MU_KEYBOARD_STATE_NUM_LOCK"; break;
					}
				}

				MUDEF const char* mu_keyboard_state_get_nice_name(muKeyboardState state) {
					switch (state) {
						default: return "Unknown"; break;
						case MU_KEYBOARD_STATE_CAPS_LOCK: return "Caps Lock"; break;
						case MU_KEYBOARD_STATE_SCROLL_LOCK: return "Scroll Lock"; break;
						case MU_KEYBOARD_STATE_NUM_LOCK: return "Num Lock"; break;
					}
				}

				MUDEF const char* mu_mouse_button_get_name(muMouseButton button) {
					switch (button) {
						default: return "MU_MOUSE_BUTTON_UNKNOWN"; break;
						case MU_MOUSE_BUTTON_LEFT: return "MU_MOUSE_BUTTON_LEFT"; break;
						case MU_MOUSE_BUTTON_RIGHT: return "MU_MOUSE_BUTTON_RIGHT"; break;
						case MU_MOUSE_BUTTON_MIDDLE: return "MU_MOUSE_BUTTON_MIDDLE"; break;
					}
				}

				MUDEF const char* mu_mouse_button_get_nice_name(muMouseButton button) {
					switch (button) {
						default: return "Unknown"; break;
						case MU_MOUSE_BUTTON_LEFT: return "Left"; break;
						case MU_MOUSE_BUTTON_RIGHT: return "Right"; break;
						case MU_MOUSE_BUTTON_MIDDLE: return "Middle"; break;
					}
				}
			#endif

	#ifdef __cplusplus
	}
	#endif
#endif /* MUCOSA_IMPLEMENTATION */

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

