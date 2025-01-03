/*
muCOSA.h - Muukid
Public domain single-file C library for interacting with operating systems using a cross-platform API.
Currently only supports Windows.
No warranty implied; use at your own risk.

Licensed under MIT license or public domain, whichever you prefer.
More explicit license information at the end of file.

@TODO A bunch of name functions.
*/

/* @DOCBEGIN

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

@DOCEND */

#ifndef MUCOSA_H
	#define MUCOSA_H
	
	// @DOCLINE # Other library dependencies
	// @DOCLINE muCOSA has a dependency on:

	// @DOCLINE * [muUtility v2.0.1](https://github.com/Muukid/muUtility/releases/tag/v2.0.1).
	// @IGNORE
		#if !defined(MU_CHECK_VERSION_MISMATCHING) && defined(MUU_H) && \
			(MUU_VERSION_MAJOR != 2 || MUU_VERSION_MINOR != 0 || MUU_VERSION_PATCH != 1)

			#pragma message("[MUCOSA] muUtility's header has already been defined, but version doesn't match the version that this library is built for. This may lead to errors, warnings, or unexpected behavior. Define MU_CHECK_VERSION_MISMATCHING before this to turn off this message.")

		#endif

		#ifndef MUU_H
			#define MUU_H

			// @DOCLINE # Version
				// @DOCLINE The macros `MUU_VERSION_MAJOR`, `MUU_VERSION_MINOR`, and `MUU_VERSION_PATCH` are defined to match its respective release version, following the formatting of `MAJOR.MINOR.PATCH`.
				
				#define MUU_VERSION_MAJOR 2
				#define MUU_VERSION_MINOR 0
				#define MUU_VERSION_PATCH 1

			// @DOCLINE # `MUDEF`
				// @DOCLINE The `MUDEF` macro is used by virtually all mu libraries, and is generally added before a header-defined variable or function. Its default value is `extern`, but can be changed to `static` by defining `MU_STATIC` before the header section of muUtility is defined. Its value can also be overwritten entirely to anything else by directly defining `MUDEF`.
				
				#ifndef MUDEF
					#ifdef MU_STATIC
						#define MUDEF static
					#else
						#define MUDEF extern
					#endif
				#endif
			
			// @DOCLINE # Secure warnings
				// @DOCLINE mu libraries often use non-secure functions that will trigger warnings on certain compilers. These warnings are, to put it lightly, dumb, so muUtility defines `_CRT_SECURE_NO_WARNINGS`. However, it is not guaranteed that this definition will actually turn the warnings off, which, at that point, they have to be manually turned off by the user. This functionality can be turned off by defining `MU_SECURE_WARNINGS`.
				#if !defined(MU_SECURE_WARNINGS) && !defined(_CRT_SECURE_NO_WARNINGS)
					#define _CRT_SECURE_NO_WARNINGS
				#endif
			
			// @DOCLINE # C++ extern
				// @DOCLINE Every mu library's primary code externs "C" within the main chunks of their code; macros `MU_CPP_EXTERN_START` and `MU_CPP_EXTERN_END` are defined to make this process easier, and would read like this:
				/* @DOCBEGIN
				```
				MU_CPP_EXTERN_START

				// Library code goes here...

				MU_CPP_EXTERN_END
				```
				@DOCEND */
				#ifdef __cplusplus
					#define MU_CPP_EXTERN_START extern "C" {
					#define MU_CPP_EXTERN_END   }
				#else
					#define MU_CPP_EXTERN_START
					#define MU_CPP_EXTERN_END
				#endif
			
			MU_CPP_EXTERN_START

			// @DOCLINE # C standard library dependencies

				// @DOCLINE muUtility has several C standard library dependencies, all of which are overridable by defining them before the inclusion of the file. The following is a list of those dependencies.

				// @DOCLINE ## `stdint.h` dependencies
				#if !defined(int8_m) || \
					!defined(uint8_m) || \
					!defined(int16_m) || \
					!defined(uint16_m) || \
					!defined(int32_m) || \
					!defined(uint32_m) || \
					!defined(int64_m) || \
					!defined(uint64_m) || \
					!defined(MU_SIZE_MAX)

					#define __STDC_LIMIT_MACROS
					#define __STDC_CONSTANT_MACROS
					#include <stdint.h>
					
					// @DOCLINE * `int8_m` - equivalent to `int8_t` if `INT8_MAX` is defined; `char` if otherwise.
					#ifndef int8_m
						#ifdef INT8_MAX
							#define int8_m int8_t
						#else
							#define int8_m char
						#endif
					#endif

					// @DOCLINE * `uint8_m` - equivalent to `uint8_t` if `UINT8_MAX` is defined; `unsigned char` if otherwise.
					#ifndef uint8_m
						#ifdef UINT8_MAX
							#define uint8_m uint8_t
						#else
							#define uint8_m unsigned char
						#endif
					#endif

					// @DOCLINE * `int16_m` - equivalent to `int16_t` if `INT16_MAX` is defined; `short` if otherwise.
					#ifndef int16_m
						#ifdef INT16_MAX
							#define int16_m int16_t
						#else
							#define int16_m short
						#endif
					#endif

					// @DOCLINE * `uint16_m` - equivalent to `uint16_t` if `UINT16_MAX` is defined; `unsigned short` if otherwise.
					#ifndef uint16_m
						#ifdef UINT16_MAX
							#define uint16_m uint16_t
						#else
							#define uint16_m unsigned short
						#endif
					#endif

					// @DOCLINE * `int32_m` - equivalent to `int32_t` if `INT32_MAX` is defined; `long` if otherwise.
					#ifndef int32_m
						#ifdef INT32_MAX
							#define int32_m int32_t
						#else
							#define int32_m long
						#endif
					#endif

					// @DOCLINE * `uint32_m` - equivalent to `uint32_t` if `UINT32_MAX` is defined; `unsigned long` if otherwise.
					#ifndef uint32_m
						#ifdef UINT32_MAX
							#define uint32_m uint32_t
						#else
							#define uint32_m unsigned long
						#endif
					#endif

					// @DOCLINE * `int64_m` - equivalent to `int64_t` if `INT64_MAX` is defined; `long long` if otherwise.
					#ifndef int64_m
						#ifdef INT64_MAX
							#define int64_m int64_t
						#else
							#define int64_m long long
						#endif
					#endif

					// @DOCLINE * `uint64_m` - equivalent to `uint64_t` if `UINT64_MAX` is defined; `unsigned long long` if otherwise.
					#ifndef uint64_m
						#ifdef UINT64_MAX
							#define uint64_m uint64_t
						#else
							#define uint64_m unsigned long long
						#endif
					#endif

					// @DOCLINE * `MU_SIZE_MAX` - equivalent to `SIZE_MAX`.
					#ifndef MU_SIZE_MAX
						#define MU_SIZE_MAX SIZE_MAX
					#endif

				#endif /* stdint.h */

				// @DOCLINE ## `stddef.h` dependencies
				#if !defined(size_m)

					#include <stddef.h>

					// @DOCLINE * `size_m` - equivalent to `size_t`.
					#ifndef size_m
						#define size_m size_t
					#endif

				#endif /* stddef.h */

				// @DOCLINE ## `stdbool.h` dependencies
				#if !defined(muBool) || \
					!defined(MU_TRUE) || \
					!defined(MU_FALSE)

					#include <stdbool.h>

					// @DOCLINE * `muBool` - equivalent to `bool`.
					#ifndef muBool
						#define muBool bool
					#endif

					// @DOCLINE * `MU_TRUE` - equivalent to `true`.
					#ifndef MU_TRUE
						#define MU_TRUE true
					#endif

					// @DOCLINE * `MU_FALSE` - equivalent to `false`.
					#ifndef MU_FALSE
						#define MU_FALSE false
					#endif

				#endif /* stdbool.h */

			// @DOCLINE # Zero struct

				// @DOCLINE There are two macros, `MU_ZERO_STRUCT` and `MU_ZERO_STRUCT_CONST`, which are functions used to zero-out a struct's contents, with their only parameter being the struct type. The reason this needs to be defined is because the way C and C++ syntax handles an empty struct are different, and need to be adjusted for. These macros are overridable by defining them before `muUtility.h` is included.

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

			// @DOCLINE # Byte manipulation

				// @DOCLINE muUtility defines several inline functions that read a value from a given array of bytes. Internally, they're all defined with the prefix `muu_...`, and then a macro is defined for them as `MU_...` (with change in capitalization after the prefix as well). The macros for these functions can be overridden, and, in such case, the original function will go undefined. For example, the function `muu_rleu8` is primarily referenced via the macro `MU_RLEU8`, and if `MU_RLEU8` is overridden, `muu_rleu8` is never defined and is not referenceable.

				// @DOCLINE All reading functions take in a pointer of bytes as their only parameter and have a return type of the fixed-width size of bits in question; for example, `muu_rleu8` is defined as:

				/* @DOCBEGIN
				```
				MUDEF inline uint8_m muu_rleu8(muByte* b);
				```
				@DOCEND */

				// @DOCLINE All writing functions take in a pointer of bytes as their first parameter, the number to be written as the second parameter, and have a return type of void; for example, `muu_wleu8` is defined as:

				/* @DOCBEGIN
				```
				MUDEF inline void muu_wleu8(muByte* b, uint8_m n);
				```
				@DOCEND */

				// @DOCLINE The exception to this is 24-bit, in which case, the fixed-width types are 32-bit (`uint32_m` / `int32_m`).

				// @DOCLINE ## Byte type

					// @DOCLINE muUtility defines the type `muByte` to refer to a byte. It is defined as `uint8_m`, and is overridable.
					#ifndef muByte
						#define muByte uint8_m
					#endif

				// @DOCLINE ## 8-bit

					// @DOCLINE The following macros exist for byte manipulation regarding 8-bit integers:

					// @DOCLINE * `MU_RLEU8` - reads an unsigned 8-bit integer from little-endian byte data; overridable macro to `muu_rleu8`.
					#ifndef MU_RLEU8
						MUDEF inline uint8_m muu_rleu8(muByte* b) {
							return b[0];
						}
						#define MU_RLEU8 muu_rleu8
					#endif

					// @DOCLINE * `MU_WLEU8` - writes an unsigned 8-bit integer to little-endian byte data; overridable macro to `muu_wleu8`.
					#ifndef MU_WLEU8
						MUDEF inline void muu_wleu8(muByte* b, uint8_m n) {
							b[0] = n;
						}
						#define MU_WLEU8 muu_wleu8
					#endif

					// @DOCLINE * `MU_RLES8` - reads a signed 8-bit integer from little-endian byte data; overridable macro to `muu_rles8`.
					#ifndef MU_RLES8
						MUDEF inline int8_m muu_rles8(muByte* b) {
							// I'm PRETTY sure this is okay...
							return *(int8_m*)b;
						}
						#define MU_RLES8 muu_rles8
					#endif

					// @DOCLINE * `MU_WLES8` - writes a signed 8-bit integer to little-endian byte data; overridable macro to `muu_wles8`.
					#ifndef MU_WLES8
						MUDEF inline void muu_wles8(muByte* b, int8_m n) {
							((int8_m*)(b))[0] = n;
						}
						#define MU_WLES8 muu_wles8
					#endif

					// @DOCLINE * `MU_RBEU8` - reads an unsigned 8-bit integer from big-endian byte data; overridable macro to `muu_rbeu8`.
					#ifndef MU_RBEU8
						MUDEF inline uint8_m muu_rbeu8(muByte* b) {
							return b[0];
						}
						#define MU_RBEU8 muu_rbeu8
					#endif

					// @DOCLINE * `MU_WBEU8` - writes an unsigned 8-bit integer to big-endian byte data; overridable macro to `muu_wbeu8`.
					#ifndef MU_WBEU8
						MUDEF inline void muu_wbeu8(muByte* b, uint8_m n) {
							b[0] = n;
						}
						#define MU_WBEU8 muu_wbeu8
					#endif

					// @DOCLINE * `MU_RBES8` - reads a signed 8-bit integer from big-endian byte data; overridable macro to `muu_rbes8`.
					#ifndef MU_RBES8
						MUDEF inline int8_m muu_rbes8(muByte* b) {
							return *(int8_m*)b;
						}
						#define MU_RBES8 muu_rbes8
					#endif

					// @DOCLINE * `MU_WBES8` - writes a signed 8-bit integer to big-endian byte data; overridable macro to `muu_wbes8`.
					#ifndef MU_WBES8
						MUDEF inline void muu_wbes8(muByte* b, int8_m n) {
							((int8_m*)(b))[0] = n;
						}
						#define MU_WBES8 muu_wbes8
					#endif

				// @DOCLINE ## 16-bit

					// @DOCLINE The following macros exist for byte manipulation regarding 16-bit integers:

					// @DOCLINE * `MU_RLEU16` - reads an unsigned 16-bit integer from little-endian byte data; overridable macro to `muu_rleu16`.
					#ifndef MU_RLEU16
						MUDEF inline uint16_m muu_rleu16(muByte* b) {
							return (
								((uint16_m)(b[0]) << 0) |
								((uint16_m)(b[1]) << 8)
							);
						}
						#define MU_RLEU16 muu_rleu16
					#endif

					// @DOCLINE * `MU_WLEU16` - writes an unsigned 16-bit integer to little-endian byte data; overridable macro to `muu_wleu16`.
					#ifndef MU_WLEU16
						MUDEF inline void muu_wleu16(muByte* b, uint16_m n) {
							b[0] = (uint8_m)(n >> 0);
							b[1] = (uint8_m)(n >> 8);
						}
						#define MU_WLEU16 muu_wleu16
					#endif

					// @DOCLINE * `MU_RLES16` - reads a signed 16-bit integer from little-endian byte data; overridable macro to `muu_rles16`.
					#ifndef MU_RLES16
						MUDEF inline int16_m muu_rles16(muByte* b) {
							uint16_m u16 = muu_rleu16(b);
							return *(int16_m*)&u16;
						}
						#define MU_RLES16 muu_rles16
					#endif

					// @DOCLINE * `MU_WLES16` - writes a signed 16-bit integer to little-endian byte data; overridable macro to `muu_wles16`.
					#ifndef MU_WLES16
						MUDEF inline void muu_wles16(muByte* b, int16_m n) {
							uint16_m un = *(uint16_m*)&n;
							b[0] = (uint8_m)(un >> 0);
							b[1] = (uint8_m)(un >> 8);
						}
						#define MU_WLES16 muu_wles16
					#endif

					// @DOCLINE * `MU_RBEU16` - reads an unsigned 16-bit integer from big-endian byte data; overridable macro to `muu_rbeu16`.
					#ifndef MU_RBEU16
						MUDEF inline uint16_m muu_rbeu16(muByte* b) {
							return (
								((uint16_m)(b[1]) << 0) |
								((uint16_m)(b[0]) << 8)
							);
						}
						#define MU_RBEU16 muu_rbeu16
					#endif

					// @DOCLINE * `MU_WBEU16` - writes an unsigned 16-bit integer to big-endian byte data; overridable macro to `muu_wbeu16`.
					#ifndef MU_WBEU16
						MUDEF inline void muu_wbeu16(muByte* b, uint16_m n) {
							b[1] = (uint8_m)(n >> 0);
							b[0] = (uint8_m)(n >> 8);
						}
						#define MU_WBEU16 muu_wbeu16
					#endif

					// @DOCLINE * `MU_RBES16` - reads a signed 16-bit integer from big-endian byte data; overridable macro to `muu_rbes16`.
					#ifndef MU_RBES16
						MUDEF inline int16_m muu_rbes16(muByte* b) {
							uint16_m u16 = muu_rbeu16(b);
							return *(int16_m*)&u16;
						}
						#define MU_RBES16 muu_rbes16
					#endif

					// @DOCLINE * `MU_WBES16` - writes a signed 16-bit integer to big-endian byte data; overridable macro to `muu_wbes16`.
					#ifndef MU_WBES16
						MUDEF inline void muu_wbes16(muByte* b, int16_m n) {
							uint16_m un = *(uint16_m*)&n;
							b[1] = (uint8_m)(un >> 0);
							b[0] = (uint8_m)(un >> 8);
						}
						#define MU_WBES16 muu_wbes16
					#endif

				// @DOCLINE ## 24-bit

					// @DOCLINE The following macros exist for byte manipulation regarding 24-bit integers:

					// @DOCLINE * `MU_RLEU24` - reads an unsigned 24-bit integer from little-endian byte data; overridable macro to `muu_rleu24`.
					#ifndef MU_RLEU24
						MUDEF inline uint32_m muu_rleu24(muByte* b) {
							return (
								((uint32_m)(b[0]) << 0) |
								((uint32_m)(b[1]) << 8) |
								((uint32_m)(b[2]) << 16)
							);
						}
						#define MU_RLEU24 muu_rleu24
					#endif

					// @DOCLINE * `MU_WLEU24` - writes an unsigned 24-bit integer to little-endian byte data; overridable macro to `muu_wleu24`.
					#ifndef MU_WLEU24
						MUDEF inline void muu_wleu24(muByte* b, uint32_m n) {
							b[0] = (uint8_m)(n >> 0);
							b[1] = (uint8_m)(n >> 8);
							b[2] = (uint8_m)(n >> 16);
						}
						#define MU_WLEU24 muu_wleu24
					#endif

					// @DOCLINE * `MU_RLES24` - reads a signed 24-bit integer from little-endian byte data; overridable macro to `muu_rles24`.
					#ifndef MU_RLES24
						MUDEF inline int32_m muu_rles24(muByte* b) {
							uint32_m u24 = muu_rleu24(b);
							return *(int32_m*)&u24;
						}
						#define MU_RLES24 muu_rles24
					#endif

					// @DOCLINE * `MU_WLES24` - writes a signed 24-bit integer to little-endian byte data; overridable macro to `muu_wles24`.
					#ifndef MU_WLES24
						MUDEF inline void muu_wles24(muByte* b, int32_m n) {
							// Probably definitely doesn't work with signed integers; fix later
							uint32_m un = *(uint32_m*)&n;
							b[0] = (uint8_m)(un >> 0);
							b[1] = (uint8_m)(un >> 8);
							b[2] = (uint8_m)(un >> 16);
						}
						#define MU_WLES24 muu_wles24
					#endif

					// @DOCLINE * `MU_RBEU24` - reads an unsigned 24-bit integer from big-endian byte data; overridable macro to `muu_rbeu24`.
					#ifndef MU_RBEU24
						MUDEF inline uint32_m muu_rbeu24(muByte* b) {
							return (
								((uint32_m)(b[2]) << 0) |
								((uint32_m)(b[1]) << 8) |
								((uint32_m)(b[0]) << 16)
							);
						}
						#define MU_RBEU24 muu_rbeu24
					#endif

					// @DOCLINE * `MU_WBEU24` - writes an unsigned 24-bit integer to big-endian byte data; overridable macro to `muu_wbeu24`.
					#ifndef MU_WBEU24
						MUDEF inline void muu_wbeu24(muByte* b, uint32_m n) {
							b[2] = (uint8_m)(n >> 0);
							b[1] = (uint8_m)(n >> 8);
							b[0] = (uint8_m)(n >> 16);
						}
						#define MU_WBEU24 muu_wbeu24
					#endif

					// @DOCLINE * `MU_RBES24` - reads a signed 24-bit integer from big-endian byte data; overridable macro to `muu_rbes24`.
					#ifndef MU_RBES24
						MUDEF inline int32_m muu_rbes24(muByte* b) {
							uint32_m u24 = muu_rbeu24(b);
							return *(int32_m*)&u24;
						}
						#define MU_RBES24 muu_rbes24
					#endif

					// @DOCLINE * `MU_WBES24` - writes a signed 24-bit integer to big-endian byte data; overridable macro to `muu_wbes24`.
					#ifndef MU_WBES24
						MUDEF inline void muu_wbes24(muByte* b, int32_m n) {
							uint32_m un = *(uint32_m*)&n;
							b[2] = (uint8_m)(un >> 0);
							b[1] = (uint8_m)(un >> 8);
							b[0] = (uint8_m)(un >> 16);
						}
						#define MU_WBES24 muu_wbes24
					#endif

				// @DOCLINE ## 32-bit

					// @DOCLINE The following macros exist for byte manipulation regarding 32-bit integers:

					// @DOCLINE * `MU_RLEU32` - reads an unsigned 32-bit integer from little-endian byte data; overridable macro to `muu_rleu32`.
					#ifndef MU_RLEU32
						MUDEF inline uint32_m muu_rleu32(muByte* b) {
							return (
								((uint32_m)(b[0]) << 0)  |
								((uint32_m)(b[1]) << 8)  |
								((uint32_m)(b[2]) << 16) |
								((uint32_m)(b[3]) << 24)
							);
						}
						#define MU_RLEU32 muu_rleu32
					#endif

					// @DOCLINE * `MU_WLEU32` - writes an unsigned 32-bit integer to little-endian byte data; overridable macro to `muu_wleu32`.
					#ifndef MU_WLEU32
						MUDEF inline void muu_wleu32(muByte* b, uint32_m n) {
							b[0] = (uint8_m)(n >> 0);
							b[1] = (uint8_m)(n >> 8);
							b[2] = (uint8_m)(n >> 16);
							b[3] = (uint8_m)(n >> 24);
						}
						#define MU_WLEU32 muu_wleu32
					#endif

					// @DOCLINE * `MU_RLES32` - reads a signed 32-bit integer from little-endian byte data; overridable macro to `muu_rles32`.
					#ifndef MU_RLES32
						MUDEF inline int32_m muu_rles32(muByte* b) {
							uint32_m u32 = muu_rleu32(b);
							return *(int32_m*)&u32;
						}
						#define MU_RLES32 muu_rles32
					#endif

					// @DOCLINE * `MU_WLES32` - writes a signed 32-bit integer to little-endian byte data; overridable macro to `muu_wles32`.
					#ifndef MU_WLES32
						MUDEF inline void muu_wles32(muByte* b, int32_m n) {
							uint32_m un = *(uint32_m*)&n;
							b[0] = (uint8_m)(un >> 0);
							b[1] = (uint8_m)(un >> 8);
							b[2] = (uint8_m)(un >> 16);
							b[3] = (uint8_m)(un >> 24);
						}
						#define MU_WLES32 muu_wles32
					#endif

					// @DOCLINE * `MU_RBEU32` - reads an unsigned 32-bit integer from big-endian byte data; overridable macro to `muu_rbeu32`.
					#ifndef MU_RBEU32
						MUDEF inline uint32_m muu_rbeu32(muByte* b) {
							return (
								((uint32_m)(b[3]) << 0)  |
								((uint32_m)(b[2]) << 8)  |
								((uint32_m)(b[1]) << 16) |
								((uint32_m)(b[0]) << 24)
							);
						}
						#define MU_RBEU32 muu_rbeu32
					#endif

					// @DOCLINE * `MU_WBEU32` - writes an unsigned 32-bit integer to big-endian byte data; overridable macro to `muu_wbeu32`.
					#ifndef MU_WBEU32
						MUDEF inline void muu_wbeu32(muByte* b, uint32_m n) {
							b[3] = (uint8_m)(n >> 0);
							b[2] = (uint8_m)(n >> 8);
							b[1] = (uint8_m)(n >> 16);
							b[0] = (uint8_m)(n >> 24);
						}
						#define MU_WBEU32 muu_wbeu32
					#endif

					// @DOCLINE * `MU_RBES32` - reads a signed 32-bit integer from big-endian byte data; overridable macro to `muu_rbes32`.
					#ifndef MU_RBES32
						MUDEF inline int32_m muu_rbes32(muByte* b) {
							uint32_m u32 = muu_rbeu32(b);
							return *(int32_m*)&u32;
						}
						#define MU_RBES32 muu_rbes32
					#endif

					// @DOCLINE * `MU_WBES32` - writes a signed 32-bit integer to big-endian byte data; overridable macro to `muu_wbes32`.
					#ifndef MU_WBES32
						MUDEF inline void muu_wbes32(muByte* b, int32_m n) {
							uint32_m un = *(uint32_m*)&n;
							b[3] = (uint8_m)(un >> 0);
							b[2] = (uint8_m)(un >> 8);
							b[1] = (uint8_m)(un >> 16);
							b[0] = (uint8_m)(un >> 24);
						}
						#define MU_WBES32 muu_wbes32
					#endif

				// @DOCLINE ## 64-bit

					// @DOCLINE The following macros exist for byte manipulation regarding 64-bit integers:

					// @DOCLINE * `MU_RLEU64` - reads an unsigned 64-bit integer from little-endian byte data; overridable macro to `muu_rleu64`.
					#ifndef MU_RLEU64
						MUDEF inline uint64_m muu_rleu64(muByte* b) {
							return (
								((uint64_m)(b[0]) << 0)  |
								((uint64_m)(b[1]) << 8)  |
								((uint64_m)(b[2]) << 16) |
								((uint64_m)(b[3]) << 24) |
								((uint64_m)(b[4]) << 32) |
								((uint64_m)(b[5]) << 40) |
								((uint64_m)(b[6]) << 48) |
								((uint64_m)(b[7]) << 56)
							);
						}
						#define MU_RLEU64 muu_rleu64
					#endif

					// @DOCLINE * `MU_WLEU64` - writes an unsigned 64-bit integer to little-endian byte data; overridable macro to `muu_wleu64`.
					#ifndef MU_WLEU64
						MUDEF inline void muu_wleu64(muByte* b, uint64_m n) {
							b[0] = (uint8_m)(n >> 0);
							b[1] = (uint8_m)(n >> 8);
							b[2] = (uint8_m)(n >> 16);
							b[3] = (uint8_m)(n >> 24);
							b[4] = (uint8_m)(n >> 32);
							b[5] = (uint8_m)(n >> 40);
							b[6] = (uint8_m)(n >> 48);
							b[7] = (uint8_m)(n >> 56);
						}
						#define MU_WLEU64 muu_wleu64
					#endif

					// @DOCLINE * `MU_RLES64` - reads a signed 64-bit integer from little-endian byte data; overridable macro to `muu_rles64`.
					#ifndef MU_RLES64
						MUDEF inline int64_m muu_rles64(muByte* b) {
							uint64_m u64 = muu_rleu64(b);
							return *(int64_m*)&u64;
						}
						#define MU_RLES64 muu_rles64
					#endif

					// @DOCLINE * `MU_WLES64` - writes a signed 64-bit integer to little-endian byte data; overridable macro to `muu_wles64`.
					#ifndef MU_WLES64
						MUDEF inline void muu_wles64(muByte* b, int64_m n) {
							uint64_m un = *(uint64_m*)&n;
							b[0] = (uint8_m)(un >> 0);
							b[1] = (uint8_m)(un >> 8);
							b[2] = (uint8_m)(un >> 16);
							b[3] = (uint8_m)(un >> 24);
							b[4] = (uint8_m)(un >> 32);
							b[5] = (uint8_m)(un >> 40);
							b[6] = (uint8_m)(un >> 48);
							b[7] = (uint8_m)(un >> 56);
						}
						#define MU_WLES64 muu_wles64
					#endif

					// @DOCLINE * `MU_RBEU64` - reads an unsigned 64-bit integer from big-endian byte data; overridable macro to `muu_rbeu64`.
					#ifndef MU_RBEU64
						MUDEF inline uint64_m muu_rbeu64(muByte* b) {
							return (
								((uint64_m)(b[7]) << 0)  |
								((uint64_m)(b[6]) << 8)  |
								((uint64_m)(b[5]) << 16) |
								((uint64_m)(b[4]) << 24) |
								((uint64_m)(b[3]) << 32) |
								((uint64_m)(b[2]) << 40) |
								((uint64_m)(b[1]) << 48) |
								((uint64_m)(b[0]) << 56)
							);
						}
						#define MU_RBEU64 muu_rbeu64
					#endif

					// @DOCLINE * `MU_WBEU64` - writes an unsigned 64-bit integer to big-endian byte data; overridable macro to `muu_wbeu64`.
					#ifndef MU_WBEU64
						MUDEF inline void muu_wbeu64(muByte* b, uint64_m n) {
							b[7] = (uint8_m)(n >> 0);
							b[6] = (uint8_m)(n >> 8);
							b[5] = (uint8_m)(n >> 16);
							b[4] = (uint8_m)(n >> 24);
							b[3] = (uint8_m)(n >> 32);
							b[2] = (uint8_m)(n >> 40);
							b[1] = (uint8_m)(n >> 48);
							b[0] = (uint8_m)(n >> 56);
						}
						#define MU_WBEU64 muu_wbeu64
					#endif

					// @DOCLINE * `MU_RBES64` - reads a signed 64-bit integer from big-endian byte data; overridable macro to `muu_rbes64`.
					#ifndef MU_RBES64
						MUDEF inline int64_m muu_rbes64(muByte* b) {
							uint64_m u64 = muu_rbeu64(b);
							return *(int64_m*)&u64;
						}
						#define MU_RBES64 muu_rbes64
					#endif

					// @DOCLINE * `MU_WBES64` - writes a signed 64-bit integer to big-endian byte data; overridable macro to `muu_wbes64`.
					#ifndef MU_WBES64
						MUDEF inline void muu_wbes64(muByte* b, int64_m n) {
							uint64_m un = *(uint64_m*)&n;
							b[7] = (uint8_m)(un >> 0);
							b[6] = (uint8_m)(un >> 8);
							b[5] = (uint8_m)(un >> 16);
							b[4] = (uint8_m)(un >> 24);
							b[3] = (uint8_m)(un >> 32);
							b[2] = (uint8_m)(un >> 40);
							b[1] = (uint8_m)(un >> 48);
							b[0] = (uint8_m)(un >> 56);
						}
						#define MU_WBES64 muu_wbes64
					#endif

			// @DOCLINE # Set result

				/* @DOCBEGIN

				The `MU_SET_RESULT(res, val)` macro is an overridable function that checks if the given parameter `res` is a null pointer. If it is, it does nothing, but if it isn't, it dereferences the pointer and sets the value to `val`. This macro saves a lot of code, shrinking down what would be this:

				```c
				if (result) {
					*result = ...;
				}
				```

				into this:

				```c
				MU_SET_RESULT(result, ...)
				```

				@DOCEND */

				#ifndef MU_SET_RESULT
					#define MU_SET_RESULT(res, val) if(res){*res=val;}
				#endif

			// @DOCLINE # Enum

				/* @DOCBEGIN

				The `MU_ENUM(name, ...)` macro is an overridable function used to declare an enumerator. `name` is the name of the enumerator type, and `...` are all of the values. The reason why one would prefer this over the traditional way of declaring enumerators is because it actually makes it a `size_m`, which can avoid errors on certain compilers (looking at you, Microsoft) in regards to treating enumerators like values. It expands like this:

				```c
				enum _##name {
					__VA_ARGS__
				};
				typedef enum _##name _##name;
				typedef size_m name;
				```

				@DOCEND */

				#define MU_ENUM(name, ...) enum _##name{__VA_ARGS__};typedef enum _##name _##name;typedef size_m name;

			// @DOCLINE # Operating system recognition

				/* @DOCBEGIN

				The macros `MU_WIN32` or `MU_LINUX` are defined (if neither were defined before) in order to allow mu libraries to easily check if they're running on a Windows or Linux system.

				`MU_WIN32` will be defined if `WIN32` or `_WIN32` are defined, one of which is usually pre-defined on Windows systems.

				`MU_LINUX` will be defined if `__linux__` is defined.

				@DOCEND */

				#if !defined(MU_WIN32) && !defined(MU_LINUX)
					#if defined(WIN32) || defined(_WIN32)
						#define MU_WIN32
					#endif
					#if defined(__linux__)
						#define MU_LINUX
					#endif
				#endif

			MU_CPP_EXTERN_END

		#endif /* MUU_H */
	// @ATTENTION

	// @DOCLINE > Note that mu libraries store their dependencies within their files, so you don't need to import these dependencies yourself; this section is purely to give more information about the contents that this file defines. The libraries listed may also have other dependencies that they also include that aren't explicitly listed here.

	// Types elaborated later on
	typedef uint16_m muCOSAResult; // (65,536 error results including success)
	typedef uint16_m muKeyboardKey;
	typedef uint8_m muKeyboardState;
	typedef uint16_m muMouseKey;

	// @DOCLINE # Version

		// @DOCLINE The macros `MUCOSA_VERSION_MAJOR`, `MUCOSA_VERSION_MINOR`, and `MUCOSA_VERSION_PATCH` are defined to match its respective release version, following the formatting of `MAJOR.MINOR.PATCH`.

		#define MUCOSA_VERSION_MAJOR 2
		#define MUCOSA_VERSION_MINOR 0
		#define MUCOSA_VERSION_PATCH 0

	MU_CPP_EXTERN_START

	// @DOCLINE # Window systems

		// @DOCLINE The type `muWindowSystem` (typedef for `uint8_m`) is used to define all of the currently supported window systems. It has the following defined values:
		typedef uint8_m muWindowSystem;

		// @DOCLINE * `MU_WINDOW_NULL` - Unknown/Null window system; real value 0. This value can also act as an "auto" window system, such as when creating a muCOSA context with it, automatically picking the best currently available window system.
		#define MU_WINDOW_NULL 0

		// @DOCLINE * `MU_WINDOW_WIN32` - Win32; real value 1.
		#define MU_WINDOW_WIN32 1

		// @DOCLINE > Note that although on most operating systems, only one window system can exist (such as macOS or Windows), some operating systems can have more than one window system, such as Linux with X11 or Wayland. Just in case, muCOSA allows more than one window system to be defined at once in its API, tying each muCOSA context to a particular window system, theoretically allowing for multiple muCOSA contexts to exist at once with different window systems in one program.

		// @DOCLINE ## Window system names
		#ifdef MUCOSA_NAMES

		// @DOCLINE The name function `mu_window_system_get_name` returns a `const char*` representation of the given window sytem (for example, `MU_WINDOW_NULL` returns "MU_WINDOW_NULL"), defined below: @NLNT
		MUDEF const char* mu_window_system_get_name(muWindowSystem system);

		// @DOCLINE It will return "MU_UNKNOWN" in the case that `system` is an invalid window system value.

		// @DOCLINE The name function `mu_window_system_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_WINDOW_NULL` returns "Unknown/Auto"), defined below: @NLNT
		MUDEF const char* mu_window_system_get_nice_name(muWindowSystem system);

		// @DOCLINE It will return "Unknown" in the case that `system` is an invalid window system value.

		// @DOCLINE > These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.

		#endif

		// @DOCLINE ## Manual window system support

		// @DOCLINE Not all window systems are supported upfront, so it is automatically guessed based on the current operating system that the code is compiling for using macros provided by muUtility.

		// @DOCLINE The explicit functionality is this:

		// @DOCLINE * `MUCOSA_WIN32` is defined if `MU_WIN32` is defined (automatically provided by muUtility), which toggles support for Win32.

		// @DOCLINE All of this functionality can be overrided by defining the macro `MUCOSA_MANUAL_OS_SUPPORT`, in which case, none of this is performed, and it is up to the user to manually define that operating systems are supported by defining their respective muCOSA macros (ie, if you're compiling on Windows and disabled automatic operating system recognition, you would need to define `MUCOSA_WIN32` yourself).

	// @DOCLINE # Global context

		// @DOCLINE muCOSA operates in a context, encapsulated by the type `muCOSAContext`, which has the following members:

		struct muCOSAContext {
			// @DOCLINE * `@NLFT* inner` - pointer to internally-used manually-allocated memory for running the current operating system context. The user should never interact with this member.
			void* inner;
			// @DOCLINE * `@NLFT result` - the result of the latest non-successful non-result-checking function call regarding the context; starting value upon context creation is `MUCOSA_SUCCESS`, and is set to another value if a function whose result is not set manually by the user doesn't return a success result value.
			muCOSAResult result;
		};
		typedef struct muCOSAContext muCOSAContext;

		// @DOCLINE ## Creation and destruction

		// @DOCLINE To create the context, the function `muCOSA_context_create` is used, defined below: @NLNT
		MUDEF void muCOSA_context_create(muCOSAContext* context, muWindowSystem system, muBool set_context);

		// @DOCLINE The result of this function is stored within `context->result`. Upon success, this function automatically calls `muCOSA_context_set` on the created context unless `set_context` is equal to `MU_FALSE`.

		// > It is valid for `system` to be `MU_WINDOW_NULL` for this function, in which case, the best currently available window system will be automatically chosen by muCOSA.

		// @DOCLINE For every successfully created context, it must be destroyed, which is done with the function `muCOSA_context_destroy`, defined below: @NLNT
		MUDEF void muCOSA_context_destroy(muCOSAContext* context);
		// @DOCLINE This function cannot fail if given a valid pointer to an active context (otherwise, a crash is likely), so no result value is ever indicated by this function via any means.

		// @DOCLINE ## Global context

		MUDEF muCOSAContext* muCOSA_global_context;

		// @DOCLINE muCOSA uses a global variable to reference the currently "set" context whenever a function is called that assumes a context (ie it doesn't take a parameter for context). This global variable can be changed to reference a certain context via the function `muCOSA_context_set`, defined below: @NLNT
		MUDEF void muCOSA_context_set(muCOSAContext* context);

		// @DOCLINE ## Get context information

		// @DOCLINE The only information that can be retrieved about an active context is its window system, using the function `muCOSA_context_get_window_system`, defined below: @NLNT
		MUDEF muWindowSystem muCOSA_context_get_window_system(muCOSAContext* context);

		// @DOCLINE ## Non-result/context checking functions

		/* @DOCBEGIN
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

		@DOCEND */

	// @DOCLINE # The window

		// @DOCLINE muCOSA works in a desktop OS environment, and therefore, its functionality primarily centers around the window (respective type `muWindow`; macro for `void*`).
		#define muWindow void*

		// @DOCLINE The documentation of this library will use the term "surface" in regards to the window's rendering space, and is simply the part of the window that is rendered to, excluding the "frame extents", which is comprised of things like the side bars, title bar, close button, etc.

		// @DOCLINE ## Window information

			// @DOCLINE The struct `muWindowInfo` represents information about a window. It has the following members:

			typedef struct muPixelFormat muPixelFormat;
			typedef struct muWindowCallbacks muWindowCallbacks;
			struct muWindowInfo {
				// @DOCLINE * `@NLFT* title` - the title of the window shown to the user in most interfaces (primarily the title bar).
				char* title;
				// @DOCLINE * `@NLFT width` - the width of the window's surface, in pixels.
				uint32_m width;
				// @DOCLINE * `@NLFT height` - the height of the window's surface, in pixels.
				uint32_m height;
				// @DOCLINE * `@NLFT min_width` - the minimum width of the window's surface, in pixels; a value of 0 implies no minimum.
				uint32_m min_width;
				// @DOCLINE * `@NLFT min_height` - the minimum height of the window's surface, in pixels; a value of 0 implies no minimum.
				uint32_m min_height;
				// @DOCLINE * `@NLFT max_width` - the maximum width of the window's surface, in pixels; a value of 0 implies no maximum.
				uint32_m max_width;
				// @DOCLINE * `@NLFT max_height` - the maximum height of the window's surface, in pixels; a value of 0 implies no maximum.
				uint32_m max_height;
				// @DOCLINE * `@NLFT x` - the x-coordinate of the top-leftest pixel in the window's surface relative to the entire window space of the window system.
				int32_m x;
				// @DOCLINE * `@NLFT y` - the y-coordinate of the top-leftest pixel in the window's surface relative to the entire window space of the window system.
				int32_m y;
				// @DOCLINE * `@NLFT* pixel_format` - the pixel format for the window's surface. If the value of this member is equal to 0, no pixel format is specified, and a default compatible one is chosen. If the pixel format is specified, muCOSA attempts to choose it, and if unsupported, muCOSA will throw a non-fatal error and default on a compatible pixel format.
				muPixelFormat* pixel_format;
				// @DOCLINE * `@NLFT* callbacks` - the [callback functions](#window-callbacks) for various attributes of the window. If this member is equal to 0, no callbacks are specified. If this member is not equal to 0, it should be a valid pointer to a `muWindowCallbacks` struct specifying callbacks for the window.
				muWindowCallbacks* callbacks;
			};
			typedef struct muWindowInfo muWindowInfo;

			// @DOCLINE > Due to restrictions on certain operating systems, the minimum width that will work on all operating systems is 120 pixels, and the minimum height that will surely work is 1 pixel. Additionally, negative coordiantes may not function properly for a given window in regards to setting them to that value, and should not be relied upon for functionality.

		// @DOCLINE ## Window creation and destruction

			// @DOCLINE In order to create a window, the function `muCOSA_window_create` must be called, defined below: @NLNT
			MUDEF muWindow muCOSA_window_create(muCOSAContext* context, muCOSAResult* result, muWindowInfo* info);

			// @DOCLINE Upon failure (marked by the value of `result`), the creation function returns 0.

			// @DOCLINE > The macro `mu_window_create` is the non-result-checking equivalent, and the macro `mu_window_create_` is the result-checking equivalent.
			#define mu_window_create(...) muCOSA_window_create(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
			#define mu_window_create_(result, ...) muCOSA_window_create(muCOSA_global_context, result, __VA_ARGS__)

			// @DOCLINE Every successfully created window must be destroyed at some point during the relevant context's lifetime using the function `muCOSA_window_destroy`, defined below: @NLNT
			MUDEF muWindow muCOSA_window_destroy(muCOSAContext* context, muWindow win);

			// @DOCLINE The destruction function cannot fail if given a proper context and window, and thus, there is no `result` parameter.

			// @DOCLINE > The macro `mu_window_destroy` is the non-result-checking equivalent.
			#define mu_window_destroy(...) muCOSA_window_destroy(muCOSA_global_context, __VA_ARGS__)

		// @DOCLINE ## The main loop

			// @DOCLINE The "main loop" of a window consists of two functions that are called each "frame": `muCOSA_window_get_closed` and `muCOSA_window_update`. A main loop with these functions generally looks like this:

			/* @DOCBEGIN
			```c
			// Initialization / Window creation here...

			while (!mu_window_get_closed(window)) {

				// ... (This is where the frame-by-frame logic would go) ...

				mu_window_update(window);
			}

			// Termination here...
			```
			@DOCEND */

			// @DOCLINE OpenGL contexts slightly change this; an OpenGL context renders directly to the window's surface, meaning that swapping the buffers of a window needs to occur each frame with a valid OpenGL context binded. For example, if `window` had an OpenGL context created from it named `gl`, a main loop for it would look like this:

			/* @DOCBEGIN
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
			@DOCEND */

			// @DOCLINE More information is provided in the [OpenGL context](#opengl-context) section.

			// @DOCLINE ### Get closed

			// @DOCLINE The function `muCOSA_window_get_closed` returns whether or not a given window has been closed, defined below: @NLNT
			MUDEF muBool muCOSA_window_get_closed(muCOSAContext* context, muCOSAResult* result, muWindow win);

			// @DOCLINE Once this function returns `MU_FALSE`, it is no longer usable in all circumstances other than destroying it with `muCOSA_window_destroy`; a window cannot be revived once closed.

			// @DOCLINE > The macro `mu_window_get_closed` is the non-result-checking equivalent, and the macro `mu_window_get_closed_` is the result-checking equivalent.
			#define mu_window_get_closed(...) muCOSA_window_get_closed(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
			#define mu_window_get_closed_(result, ...) muCOSA_window_get_closed(muCOSA_global_context, result, __VA_ARGS__)

			// @DOCLINE ### Close window

			// @DOCLINE The function `muCOSA_window_close` closes a given window, defined below: @NLNT
			MUDEF void muCOSA_window_close(muCOSAContext* context, muWindow win);

			// @DOCLINE This function cannot if given a valid unclosed window and a valid context corresponding to the window, and thus, has no result parameter.

			// @DOCLINE > The macro `mu_window_close` is the non-result-checking equivalent.
			#define mu_window_close(...) muCOSA_window_close(muCOSA_global_context, __VA_ARGS__)

			// @DOCLINE ### Update

			// @DOCLINE The function `muCOSA_window_update` updates/refreshes a window and triggers all relevant callbacks, presenting the contents of the surface, defined below: @NLNT
			MUDEF void muCOSA_window_update(muCOSAContext* context, muCOSAResult* result, muWindow win);

			// @DOCLINE > The macro `mu_window_update` is the non-result-checking equivalent, and the macro `mu_window_update_` is the result-checking equivalent.
			#define mu_window_update(...) muCOSA_window_update(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
			#define mu_window_update_(result, ...) muCOSA_window_update(muCOSA_global_context, result, __VA_ARGS__)

		// @DOCLINE ## Window attributes

			typedef uint16_m muWindowAttrib;
			// @DOCLINE The window is described by several attributes, with each attribute represented by the type `muWindowAttrib` (typedef for `uint16_m`). It has the following values:

			// @DOCLINE * `MU_WINDOW_TITLE` - the title of the window, represented by a `char*` UTF-8 string. This cannot be "get", but can be "set".
			#define MU_WINDOW_TITLE 0

			// @DOCLINE * `MU_WINDOW_DIMENSIONS` - the width and height of the window's surface, in pixels, represented by an array of two `uint32_m`s, where the first element is the width, and the second element is the height. This can be "get" and "set".
			#define MU_WINDOW_DIMENSIONS 1

			// @DOCLINE * `MU_WINDOW_POSITION` - the x- and y-coordinates of the top-leftest pixel of the window's surface relative to the entire window space of the window system, represented by an array of two `int32_m`s, where the first element is the x-coordinate, and the second element is the y-coordinate. This can be "get" and "set".
			#define MU_WINDOW_POSITION 2

			// @DOCLINE * `MU_WINDOW_KEYBOARD_MAP` - the [keyboard keymap](#keyboard-keymap), represented by a pointer to an array of booleans (type `muBool`) representing the state of each readable keyboard key. This can be "get", but not "set".
			#define MU_WINDOW_KEYBOARD_MAP 3
			// @DOCLINE    > Note that when being read, the data is not expected to be the actual array, but instead a pointer that will be set to the internally-used keymap array, which remains consistent for an entire window's lifespan. More information about the keyboard keymap can be found in the [keymap](#keymaps) section.

			// @DOCLINE * `MU_WINDOW_KEYSTATE_MAP` - the [keystate keymap](#keystate-keymap), represented by a pointer to an array of booleans (type `muBool`) representing the state of all keyboard states (such as caps lock, for example). This can be "get" but not "set".
			#define MU_WINDOW_KEYSTATE_MAP 4

			// @DOCLINE * `MU_WINDOW_MOUSE_MAP` - the [mouse keymap](#mouse-keymap), represented by a pointer to an array of booleans (type `muBool`) representing the state of each readable mouse key. This can be "get", but not "set".
			#define MU_WINDOW_MOUSE_MAP 5

			// @DOCLINE * `MU_WINDOW_SCROLL_LEVEL` - the scroll level of the cursor associated with the window, represented by a single `int32_m` value representing how far it is scrolled up (positive) or down (negative). This can be "get" and "set".
			#define MU_WINDOW_SCROLL_LEVEL 6
			// @DOCLINE > One full scroll up/down on a mouse wheel is worth 120 units.

			// @DOCLINE * `MU_WINDOW_CURSOR` - the x- and y-coordinates of the visual cursor relative to the position of the window's surface, represented by an array of two `int32_m`s, where the first element is the x-coordinate, and the second element is the y-coordinate. This can be "get" and "set".
			#define MU_WINDOW_CURSOR 7

			// @DOCLINE * `MU_WINDOW_CURSOR_STYLE` - the [style of the cursor](#cursor-style), represented by a single value `muCursorStyle`. This can be "get" and "set".
			#define MU_WINDOW_CURSOR_STYLE 8

			// @DOCLINE * `MU_WINDOW_..._CALLBACK` - the callback attributes. These all cannot be "get", but can be "set".
			// @DOCLINE    * `MU_WINDOW_DIMENSIONS_CALLBACK` - the dimensions callback.
			#define MU_WINDOW_DIMENSIONS_CALLBACK 9
			// @DOCLINE    * `MU_WINDOW_POSITION_CALLBACK` - the position callback.
			#define MU_WINDOW_POSITION_CALLBACK 10
			// @DOCLINE    * `MU_WINDOW_KEYBOARD_CALLBACK` - the keyboard callback.
			#define MU_WINDOW_KEYBOARD_CALLBACK 11
			// @DOCLINE    * `MU_WINDOW_KEYSTATE_CALLBACK` - the keystate callback.
			#define MU_WINDOW_KEYSTATE_CALLBACK 12
			// @DOCLINE    * `MU_WINDOW_MOUSE_KEY_CALLBACK` - the mouse key callback.
			#define MU_WINDOW_MOUSE_KEY_CALLBACK 13
			// @DOCLINE    * `MU_WINDOW_CURSOR_CALLBACK` - the cursor callback.
			#define MU_WINDOW_CURSOR_CALLBACK 14
			// @DOCLINE    * `MU_WINDOW_SCROLL_CALLBACK` - the scroll callback.
			#define MU_WINDOW_SCROLL_CALLBACK 15
			// @DOCLINE    > When callbacks are being set via `muCOSA_window_set`, note that they are pointers *to* the function pointers; function "`fun`" would be set via `muCOSA_window_set(..., &fun)`. For more information about the callbacks, see the (callbacks section)(#window-callbacks). The types listed in the `muWindowCallbacks` struct match the types expected for the callback window attributes.

			// @DOCLINE A value is "get" if calling `muCOSA_window_get` with it is valid, and a value is "set" if calling `muCOSA_window_set` with it is valid.

			// @DOCLINE ### Names
			#ifdef MUCOSA_NAMES

			// @DOCLINE The name function for `muWindowAttrib` is `mu_window_attrib_get_name`, defined below: @NLNT
			MUDEF const char* mu_window_attrib_get_name(muWindowAttrib attrib);

			// @DOCLINE > This function returns "MU_UNKNOWN" if the value of `attrib` is unrecognized.

			// @DOCLINE The nice name function for `muWindowAttrib` is `mu_window_attrib_get_nice_name`, defined below: @NLNT
			MUDEF const char* mu_window_attrib_get_nice_name(muWindowAttrib attrib);

			// @DOCLINE > This function returns "Unknown" if the value of `attrib` is unrecognized.
			#endif

			// @DOCLINE ### Get and set window attributes

			// @DOCLINE The function `muCOSA_window_get` retrieves an attribute of a window, defined below: @NLNT
			MUDEF void muCOSA_window_get(muCOSAContext* context, muCOSAResult* result, muWindow win, muWindowAttrib attrib, void* data);

			// @DOCLINE > The macro `mu_window_get` is the non-result-checking equivalent, and the macro `mu_window_get_` is the result-checking equivalent.
			#define mu_window_get(...) muCOSA_window_get(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
			#define mu_window_get_(result, ...) muCOSA_window_get(muCOSA_global_context, result, __VA_ARGS__)

			// @DOCLINE The function `muCOSA_window_set` modifies an attribute of a window, defined below: @NLNT
			MUDEF void muCOSA_window_set(muCOSAContext* context, muCOSAResult* result, muWindow win, muWindowAttrib attrib, void* data);

			// @DOCLINE > The macro `mu_window_set` is the non-result-checking equivalent, and the macro `mu_window_set_` is the result-checking equivalent.
			#define mu_window_set(...) muCOSA_window_set(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
			#define mu_window_set_(result, ...) muCOSA_window_set(muCOSA_global_context, result, __VA_ARGS__)

			// @DOCLINE For both functions, `data` is a pointer to data dictated by the value of `attrib`. In the case of `muCOOSA_window_get`, the data is derefenced and filled in corresponding to the window's requested attribute (if successful); in the case of `muCOSA_window_set`, the data is dereferenced and read, and the requested window attribute is changed to the given value(s) (if successful).

			// @DOCLINE > `mu_window_set` will only read from `data` and never modify it. Likewise, `mu_window_get` will only dereference `data` and never read from it.

		// @DOCLINE ## Window callbacks

			// @DOCLINE A window callback is a function that is called whenever the window registers that a certain attribute has changed. Every callback function is called while the window is being updated via the function `muCOSA_window_update`.

			// @DOCLINE Window callbacks are specified in the struct `muWindowCallbacks`, which has the following members:

			struct muWindowCallbacks {
				// @DOCLINE * `void (*dimensions)` - the dimensions callback, called every time that the window's dimensions are modified, defined below: @NLNT
				void (*dimensions)(muWindow win, uint32_m width, uint32_m height);
				// @DOCLINE * `void (*position)` - the position callback, called every time that the window's position is modified, defined below: @NLNT
				void (*position)(muWindow win, int32_m x, int32_m y);
				// @DOCLINE * `void (*keyboard)` - the keyboard callback, called every time that the status of a keyboard key on the [keyboard keymap](#keyboard-keymap) changes, defined below: @NLNT
				void (*keyboard)(muWindow win, muKeyboardKey key, muBool status);
				// @DOCLINE * `void (*keystate)` - the keystate callback, called every time that the status of a keystate on the [keystate keymap](#keystate-keymap) changes, defined below: @NLNT
				void (*keystate)(muWindow win, muKeyboardState state, muBool status);
				// @DOCLINE * `void (*mouse_key)` - the mouse key callback, called every time that the status of a mouse key on the [mouse keymap](#mouse-keymap) changes, defined below: @NLNT
				void (*mouse_key)(muWindow win, muMouseKey key, muBool status);
				// @DOCLINE * `void (*cursor)` - the cursor position callback, called every time that the cursor position changes, defined below: @NLNT
				void (*cursor)(muWindow win, int32_m x, int32_m y);
				// @DOCLINE * `void (*scroll)` - the scroll callback, called every time that the scroll level changes, defined below: @NLNT
				void (*scroll)(muWindow win, int32_m add);
			};

			// @DOCLINE Setting the value of any member to 0 dictates no callback function. Changes to an attribute made via the program (such as a `mu_window_set` call) are not guaranteed to generate corresponding callbacks.

			// @DOCLINE Initial callbacks, AKA callbacks involving the window's attributes being set upon creation, are not guaranteed to be called. Additionally, duplicate callbacks (ie callbacks issuing the same value, such a keyboard callback issuing a key status that already matched the previous key status, AKA a key being pressed/released twice) can occur; in fact, duplicate key presses are used to represent a key being held down for a long time, and the rate of repetition varies between window systems and their settings.

			// @DOCLINE Callbacks involving queryable attributes of a window are called after their attribute has been updated. For example, if a keyboard callback is triggered, the keyboard keymap for the corresponding window has already been updated for the key in question.

			// @DOCLINE Users should also avoid possible callback loops, such as a position callback that changes the position, which can theoretically trigger an infinite loop and cause a nasty crash.

		// @DOCLINE ## Keymaps

			// @DOCLINE In order to make input require as minimal overhead as possible, muCOSA allows the user to read key input using "keymaps". A keymap is an array of booleans (type `muBool`) that dictate the state of each key. Therefore, if a user wanted to check a particular key's state, they would retrieve the keymap, and index into it based on what key they want to check. This array is stored internally somewhere in the API, and, when retrieved (via a "get" function call), a pointer to this array is given. Since the keymap is stored as a pointer to inner memory used by muCOSA, it is automatically updated every call to `muCOSA_window_update`.

			// @DOCLINE The big advantage of keymaps is that the array remains at the same point in memory for the entire window's lifespan; a user could grab the pointer to the keymap once at the beginning of the program, and instantly know the state of any key via indexing into it, without any need for refreshing or extra function calls as long as the window remains alive.

			// @DOCLINE > Note that when the keymap is retrieved, a *pointer* to it is retrieved, not the array itself. This means that when calling a "get" function with a keymap, a pointer to a pointer should be given, to which muCOSA will dereference the initial pointer and set the pointer to the address of the keymap array.

			// @DOCLINE > Note that keymaps are only meant to be read, not modified. Changing any value within a keymap array will result in undefined behavior.

			// @DOCLINE ### Keyboard keymap

			// @DOCLINE The keyboard keymap represents keys on the keyboard readable by muCOSA, using type `muKeyboardKey` (typedef for `uint16_m`) as index. The length of the keymap is `MU_KEYBOARD_LENGTH`. It has the following indexes:

			// @DOCLINE * `MU_KEYBOARD_UNKNOWN` - unknown key.
			#define MU_KEYBOARD_UNKNOWN 0
			// @DOCLINE * `MU_KEYBOARD_BACKSPACE` - the [backspace key](https://en.wikipedia.org/wiki/Backspace).
			#define MU_KEYBOARD_BACKSPACE 1
			// @DOCLINE * `MU_KEYBOARD_TAB` - the [tab key](https://en.wikipedia.org/wiki/Tab_key).
			#define MU_KEYBOARD_TAB 2
			// @DOCLINE * `MU_KEYBOARD_CLEAR` - the [clear key](https://en.wikipedia.org/wiki/Clear_key).
			#define MU_KEYBOARD_CLEAR 3
			// @DOCLINE * `MU_KEYBOARD_RETURN` - the [return key](https://en.wikipedia.org/wiki/Return_key).
			#define MU_KEYBOARD_RETURN 4
			// @DOCLINE * `MU_KEYBOARD_PAUSE` - the [pause key](https://en.wikipedia.org/wiki/Pause_key).
			#define MU_KEYBOARD_PAUSE 5
			// @DOCLINE * `MU_KEYBOARD_ESCAPE` - the [escape key](https://en.wikipedia.org/wiki/Escape_key).
			#define MU_KEYBOARD_ESCAPE 6
			// @DOCLINE * `MU_KEYBOARD_MODECHANGE` - the [modechange key](https://en.wikipedia.org/wiki/Language_input_keys).
			#define MU_KEYBOARD_MODECHANGE 7
			// @DOCLINE * `MU_KEYBOARD_SPACE` - the [space key](https://en.wikipedia.org/wiki/Space_bar).
			#define MU_KEYBOARD_SPACE 8
			// @DOCLINE * `MU_KEYBOARD_PRIOR` - the [page up key](https://en.wikipedia.org/wiki/Page_Up_and_Page_Down_keys).
			#define MU_KEYBOARD_PRIOR 9
			// @DOCLINE * `MU_KEYBOARD_NEXT` - the [page down key](https://en.wikipedia.org/wiki/Page_Up_and_Page_Down_keys).
			#define MU_KEYBOARD_NEXT 10
			// @DOCLINE * `MU_KEYBOARD_END` - the [end key](https://en.wikipedia.org/wiki/End_key).
			#define MU_KEYBOARD_END 11
			// @DOCLINE * `MU_KEYBOARD_HOME` - the [home key](https://en.wikipedia.org/wiki/Home_key).
			#define MU_KEYBOARD_HOME 12
			// @DOCLINE * `MU_KEYBOARD_(LEFT/UP/RIGHT/DOWN)` - the left, up, right, and down (arrow keys)[https://en.wikipedia.org/wiki/Arrow_keys].
			#define MU_KEYBOARD_LEFT 13
			#define MU_KEYBOARD_UP 14
			#define MU_KEYBOARD_RIGHT 15
			#define MU_KEYBOARD_DOWN 16
			// @DOCLINE * `MU_KEYBOARD_SELECT` - the [select key](https://stackoverflow.com/questions/23995537/what-is-the-select-key).
			#define MU_KEYBOARD_SELECT 17
			// @DOCLINE * `MU_KEYBOARD_PRINT` - the [print key](https://en.wikipedia.org/wiki/Print_Screen).
			#define MU_KEYBOARD_PRINT 18
			// @DOCLINE * `MU_KEYBOARD_EXECUTE` - the execute key.
			#define MU_KEYBOARD_EXECUTE 19
			// @DOCLINE * `MU_KEYBOARD_INSERT` - the [insert key](https://en.wikipedia.org/wiki/Insert_key).
			#define MU_KEYBOARD_INSERT 20
			// @DOCLINE * `MU_KEYBOARD_DELETE` - the [delete key](https://en.wikipedia.org/wiki/Delete_key).
			#define MU_KEYBOARD_DELETE 21
			// @DOCLINE * `MU_KEYBOARD_HELP` - the [help key](https://en.wikipedia.org/wiki/Help_key).
			#define MU_KEYBOARD_HELP 22
			// @DOCLINE * `MU_KEYBOARD_(0...9)` - [the number keys](https://en.wikipedia.org/wiki/Numerical_digit) (0-9).
			#define MU_KEYBOARD_0 23
			#define MU_KEYBOARD_1 24
			#define MU_KEYBOARD_2 25
			#define MU_KEYBOARD_3 26
			#define MU_KEYBOARD_4 27
			#define MU_KEYBOARD_5 28
			#define MU_KEYBOARD_6 29
			#define MU_KEYBOARD_7 30
			#define MU_KEYBOARD_8 31
			#define MU_KEYBOARD_9 32
			// @DOCLINE * `MU_KEYBOARD_(A...Z)` - the [alphabet keys](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys) (A-Z).
			#define MU_KEYBOARD_A 33
			#define MU_KEYBOARD_B 34
			#define MU_KEYBOARD_C 35
			#define MU_KEYBOARD_D 36
			#define MU_KEYBOARD_E 37
			#define MU_KEYBOARD_F 38
			#define MU_KEYBOARD_G 39
			#define MU_KEYBOARD_H 40
			#define MU_KEYBOARD_I 41
			#define MU_KEYBOARD_J 42
			#define MU_KEYBOARD_K 43
			#define MU_KEYBOARD_L 44
			#define MU_KEYBOARD_M 45
			#define MU_KEYBOARD_N 46
			#define MU_KEYBOARD_O 47
			#define MU_KEYBOARD_P 48
			#define MU_KEYBOARD_Q 49
			#define MU_KEYBOARD_R 50
			#define MU_KEYBOARD_S 51
			#define MU_KEYBOARD_T 52
			#define MU_KEYBOARD_U 53
			#define MU_KEYBOARD_V 54
			#define MU_KEYBOARD_W 55
			#define MU_KEYBOARD_X 56
			#define MU_KEYBOARD_Y 57
			#define MU_KEYBOARD_Z 58
			// @DOCLINE * `MU_KEYBOARD_(LEFT/RIGHT)_WINDOWS` - the left and right [Windows](https://en.wikipedia.org/wiki/Windows_key)/[super](https://en.wikipedia.org/wiki/Super_key_(keyboard_button))/[command](https://en.wikipedia.org/wiki/Command_key) keys.
			#define MU_KEYBOARD_LEFT_WINDOWS 59
			#define MU_KEYBOARD_RIGHT_WINDOWS 60
			// @DOCLINE * `MU_KEYBOARD_NUMPAD_(0...9)` - the [numpad number keys](https://en.wikipedia.org/wiki/Numeric_keypad) (0-9).
			#define MU_KEYBOARD_NUMPAD_0 61
			#define MU_KEYBOARD_NUMPAD_1 62
			#define MU_KEYBOARD_NUMPAD_2 63
			#define MU_KEYBOARD_NUMPAD_3 64
			#define MU_KEYBOARD_NUMPAD_4 65
			#define MU_KEYBOARD_NUMPAD_5 66
			#define MU_KEYBOARD_NUMPAD_6 67
			#define MU_KEYBOARD_NUMPAD_7 68
			#define MU_KEYBOARD_NUMPAD_8 69
			#define MU_KEYBOARD_NUMPAD_9 70
			// @DOCLINE * `MU_KEYBOARD_(ADD/SUBTRACT/MULTIPLY/DIVIDE)` - the addition, subtraction, multiplication, and division [numpad keys](https://en.wikipedia.org/wiki/Numeric_keypad).
			#define MU_KEYBOARD_ADD      71
			#define MU_KEYBOARD_SUBTRACT 72
			#define MU_KEYBOARD_MULTIPLY 73
			#define MU_KEYBOARD_DIVIDE   74
			// @DOCLINE * `MU_KEYBOARD_SEPARATOR` - the [separator key](https://stackoverflow.com/questions/67916923/what-physical-key-maps-to-keycode-108-vk-separator).
			#define MU_KEYBOARD_SEPARATOR 75
			// @DOCLINE * `MU_KEYBOARD_DECIMAL` - the [decimal/period/dot key](https://en.wikipedia.org/wiki/Numeric_keypad).
			#define MU_KEYBOARD_DECIMAL 76
			// @DOCLINE * `MU_KEYBOARD_F(1...24)` - the [function keys](https://en.wikipedia.org/wiki/Function_key) (1-24).
			#define MU_KEYBOARD_F1  77
			#define MU_KEYBOARD_F2  78
			#define MU_KEYBOARD_F3  79
			#define MU_KEYBOARD_F4  80
			#define MU_KEYBOARD_F5  81
			#define MU_KEYBOARD_F6  82
			#define MU_KEYBOARD_F7  83
			#define MU_KEYBOARD_F8  84
			#define MU_KEYBOARD_F9  85
			#define MU_KEYBOARD_F10 86
			#define MU_KEYBOARD_F11 87
			#define MU_KEYBOARD_F12 88
			#define MU_KEYBOARD_F13 89
			#define MU_KEYBOARD_F14 90
			#define MU_KEYBOARD_F15 91
			#define MU_KEYBOARD_F16 92
			#define MU_KEYBOARD_F17 93
			#define MU_KEYBOARD_F18 94
			#define MU_KEYBOARD_F19 95
			#define MU_KEYBOARD_F20 96
			#define MU_KEYBOARD_F21 97
			#define MU_KEYBOARD_F22 98
			#define MU_KEYBOARD_F23 99
			#define MU_KEYBOARD_F24 100
			// @DOCLINE * `MU_KEYBOARD_NUMLOCK` - the [Num Lock key](https://en.wikipedia.org/wiki/Num_Lock).
			#define MU_KEYBOARD_NUMLOCK 101
			// @DOCLINE * `MU_KEYBOARD_SCROLL` - the [Scroll Lock key](https://en.wikipedia.org/wiki/Scroll_Lock).
			#define MU_KEYBOARD_SCROLL 102
			// @DOCLINE * `MU_KEYBOARD_(LEFT/RIGHT)_SHIFT` - the left/right [shift keys](https://en.wikipedia.org/wiki/Shift_key).
			#define MU_KEYBOARD_LEFT_SHIFT 103
			#define MU_KEYBOARD_RIGHT_SHIFT 104
			// @DOCLINE * `MU_KEYBOARD_(LEFT/RIGHT)_CONTROL` - the left/right [control keys](https://en.wikipedia.org/wiki/Control_key).
			#define MU_KEYBOARD_LEFT_CONTROL 105
			#define MU_KEYBOARD_RIGHT_CONTROL 106
			// @DOCLINE * `MU_KEYBOARD_(LEFT/RIGHT)_MENU` - the left/right [menu keys](https://en.wikipedia.org/wiki/Menu_key).
			#define MU_KEYBOARD_LEFT_MENU 107
			#define MU_KEYBOARD_RIGHT_MENU 108
			// @DOCLINE * `MU_KEYBOARD_ATTN` - the [ATTN key](https://www.ibm.com/support/pages/apar/II04992).
			#define MU_KEYBOARD_ATTN 109
			// @DOCLINE * `MU_KEYBOARD_CRSEL` - the [CRSEL key](https://learn.microsoft.com/en-us/dotnet/api/system.windows.forms.keys?view=windowsdesktop-8.0).
			#define MU_KEYBOARD_CRSEL 110
			// @DOCLINE * `MU_KEYBOARD_EXSEL` - the [EXSEL key](https://learn.microsoft.com/en-us/dotnet/api/system.windows.forms.keys?view=windowsdesktop-8.0).
			#define MU_KEYBOARD_EXSEL 111
			// @DOCLINE * `MU_KEYBOARD_EREOF` - the [EREOF key](https://www.ibm.com/docs/en/wsfz-and-o/1.1?topic=descriptions-ereof-erase-end-field-key-statement).
			#define MU_KEYBOARD_EREOF 112
			// @DOCLINE * `MU_KEYBOARD_PLAY` - the play key.
			#define MU_KEYBOARD_PLAY 113
			// @DOCLINE * `MU_KEYBOARD_PA1` - the [PA1 key](https://www.ibm.com/docs/en/zos-basic-skills?topic=ispf-keyboard-keys-functions).
			#define MU_KEYBOARD_PA1 114

			#define MU_KEYBOARD_LENGTH 115

			// @DOCLINE Once the pointer to the keyboard keymap array has been retrieved via `muCOSA_window_get`, these values can be used as indexes to see the status of any keyboard key, in which `MU_TRUE` indicates that the key is being pressed down, and `MU_FALSE` indicates that the key is released.

			#ifdef MUCOSA_NAMES
			// @DOCLINE #### Keyboard names

			// @DOCLINE The name function `mu_keyboard_key_get_name` returns a `const char*` representation of the given keyboard key (for example, `MU_KEYBOARD_W` returns "MU_KEYBOARD_W"), defined below: @NLNT
			MUDEF const char* mu_keyboard_key_get_name(muKeyboardKey key);
			// @DOCLINE It will return "MU_UNKNOWN" in the case that `key` is an invalid keyboard key value.

			// @DOCLINE The name function `mu_keyboard_key_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_KEYBOARD_W` returns "W"), defined below: @NLNT
			MUDEF const char* mu_keyboard_key_get_nice_name(muKeyboardKey key);
			// @DOCLINE It will return "Unknown" in the case that `key` is an invalid keyboard key value.

			// @DOCLINE > These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.

			#endif

			// @DOCLINE ### Keystate keymap

			// @DOCLINE The keystate keymap represents the state of certain modifiers on the keyboard readable by muCOSA, using type `muKeyboardState` (typedef for `uint8_m`) as index. The length of the keymap is `MU_KEYSTATE_LENGTH`. It has the following indexes:

			// @DOCLINE * `MU_KEYSTATE_UNKNOWN` - unknown keystate.
			#define MU_KEYSTATE_UNKNOWN 0
			// @DOCLINE * `MU_KEYSTATE_CAPS_LOCK` - the [caps lock](https://en.wikipedia.org/wiki/Caps_Lock) state.
			#define MU_KEYSTATE_CAPS_LOCK 1
			// @DOCLINE * `MU_KEYSTATE_SCROLL_LOCK` - the [scroll lock](https://en.wikipedia.org/wiki/Scroll_Lock) state.
			#define MU_KEYSTATE_SCROLL_LOCK 2
			// @DOCLINE * `MU_KEYSTATE_NUM_LOCK` - the [num lock](https://en.wikipedia.org/wiki/Num_Lock) state.
			#define MU_KEYSTATE_NUM_LOCK 3

			#define MU_KEYSTATE_LENGTH 4

			// @DOCLINE Once the pointer to the keystate keymap array has been retrieved via `muCOSA_window_get`, these values can be used as indexes to see the status of any mouse key, in which `MU_TRUE` indicates that the keystate is active, and `MU_FALSE` indicates that the keystate is inactive.

			#ifdef MUCOSA_NAMES
			// @DOCLINE #### Keystate names

			// @DOCLINE The name function `mu_keystate_get_name` returns a `const char*` representation of the given keystate (for example, `MU_KEYSTATE_CAPS_LOCK` returns "MU_KEYSTATE_CAPS_LOCK"), defined below: @NLNT
			MUDEF const char* mu_keystate_get_name(muKeyboardState state);
			// @DOCLINE It will return "MU_UNKNOWN" in the case that `state` is an invalid keystate value.

			// @DOCLINE The name function `mu_keystate_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_KEYSTATE_CAPS_LOCK` returns "Caps Lock"), defined below: @NLNT
			MUDEF const char* mu_keystate_get_nice_name(muKeyboardState state);
			// @DOCLINE It will return "Unknown" in the case that `state` is an invalid keystate value.

			// @DOCLINE > These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.

			#endif

			// @DOCLINE ### Mouse keymap

			// @DOCLINE The mouse keymap represents the keys on a common computer mouse readable by muCOSA, using type `muMouseKey` (typedef for `uint16_m`) as index. The length of the keymap is `MU_MOUSE_LENGTH`. It has the following indexes:

			// @DOCLINE * `MU_MOUSE_UNKNOWN` - unknown mouse key.
			#define MU_MOUSE_UNKNOWN 0
			// @DOCLINE * `MU_MOUSE_LEFT` - the left mouse key.
			#define MU_MOUSE_LEFT 1
			// @DOCLINE * `MU_MOUSE_RIGHT` - the right mouse key.
			#define MU_MOUSE_RIGHT 2
			// @DOCLINE * `MU_MOUSE_MIDDLE` - the middle mouse key; this indicates whether or not the middle mouse key (usually the scroll wheel) is being clicked, not scrolled.
			#define MU_MOUSE_MIDDLE 3

			#define MU_MOUSE_LENGTH 4

			// @DOCLINE Once the pointer to the mouse keymap array has been retrieved via `muCOSA_window_get`, these values can be used as indexes to see the status of any mouse key, in which `MU_TRUE` indicates that the key is being pressed down, and `MU_FALSE` indicates that the key is released.

			#ifdef MUCOSA_NAMES
			// @DOCLINE #### Mouse key names

			// @DOCLINE The name function `mu_mouse_key_get_name` returns a `const char*` representation of the given mouse key (for example, `MU_MOUSE_LEFT` returns "MU_MOUSE_LEFT"), defined below: @NLNT
			MUDEF const char* mu_mouse_key_get_name(muMouseKey key);
			// @DOCLINE It will return "MU_UNKNOWN" in the case that `key` is an invalid mouse key value.

			// @DOCLINE The name function `mu_mouse_key_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_MOUSE_LEFT` returns "Left"), defined below: @NLNT
			MUDEF const char* mu_mouse_key_get_nice_name(muMouseKey key);
			// @DOCLINE It will return "Unknown" in the case that `key` is an invalid mouse key value.

			// @DOCLINE > These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.

			#endif

		// @DOCLINE ## Cursor style

			typedef uint16_m muCursorStyle;

			// @DOCLINE The style of a cursor determines how it visually appears based on a number of presets for its look that the window system provides. Its type is `muCursorStyle` (typedef for `uint16_m`), and has the following values:

			// @DOCLINE * `MU_CURSOR_UNKNOWN` - unknown cursor style.
			#define MU_CURSOR_UNKNOWN 0
			// @DOCLINE * `MU_CURSOR_ARROW` - the normal arrow-looking cursor style; equivalent to `IDC_ARROW` in Win32.
			#define MU_CURSOR_ARROW 1
			// @DOCLINE * `MU_CURSOR_IBEAM` - the text-select cursor style, appearing as a vertical beam; equivalent to `IDC_IBEAM` in Win32.
			#define MU_CURSOR_IBEAM 2
			// @DOCLINE * `MU_CURSOR_WAIT` - the waiting/busy/loading cursor style; equivalent to `IDC_WAIT` in Win32.
			#define MU_CURSOR_WAIT 3
			// @DOCLINE * `MU_CURSOR_WAIT_ARROW` - the waiting/busy/loading cursor style, but also with the normal arrow visible; equivalent to `IDC_APPSTARTING` in Win32.
			#define MU_CURSOR_WAIT_ARROW 4
			// @DOCLINE * `MU_CURSOR_CROSSHAIR` - the crosshair cursor style; equivalent to `IDC_CROSS` in Win32.
			#define MU_CURSOR_CROSSHAIR 5
			// @DOCLINE * `MU_CURSOR_HAND` - the finger-pointing/link-select cursor style; equivalent to `IDC_HAND` in Win32.
			#define MU_CURSOR_HAND 6
			// @DOCLINE * `MU_CURSOR_SIZE_LR` - the resize cursor style, pointing left-to-right horizontally; equivalent to `IDC_SIZEWE` in Win32.
			#define MU_CURSOR_SIZE_LR 7
			// @DOCLINE * `MU_CURSOR_SIZE_TB` - the resize cursor style, pointing up-to-down vertically; equivalent to `IDC_SIZENS` in Win32.
			#define MU_CURSOR_SIZE_TB 8
			// @DOCLINE * `MU_CURSOR_SIZE_TL_BR` - the resize cursor style, pointing from top-left to bottom-right sideways; equivalent to `IDC_NWSE` in Win32.
			#define MU_CURSOR_SIZE_TL_BR 9
			// @DOCLINE * `MU_CURSOR_SIZE_TR_BL` - the resize cursor style, pointing from top-right to bottom-left sideways; equivalent to `IDC_NESW` in Win32.
			#define MU_CURSOR_SIZE_TR_BL 10
			// @DOCLINE * `MU_CURSOR_SIZE_ALL` - the move/resize-all cursor style, pointing outwards in all directions; equivalent to `IDC_SIZEALL` in Win32.
			#define MU_CURSOR_SIZE_ALL 11
			// @DOCLINE * `MU_CURSOR_NO` - the disallowing/error/not-allowed cursor style; equivalent to `IDC_NO` in WIn32.
			#define MU_CURSOR_NO 12

			#ifdef MUCOSA_NAMES
			// @DOCLINE ### Cursor style names

			// @DOCLINE The name function `mu_cursor_style_get_name` returns a `const char*` representation of the given cursor style (for example, `MU_CURSOR_HAND` returns "MU_CURSOR_HAND"), defined below: @NLNT
			MUDEF const char* mu_cursor_style_get_name(muCursorStyle style);
			// @DOCLINE It will return "MU_UNKNOWN" in the case that `style` is an invalid cursor style value.

			// @DOCLINE The name function `mu_cursor_style_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_CURSOR_HAND` returns "Hand"), defined below: @NLNT
			MUDEF const char* mu_cursor_style_get_nice_name(muCursorStyle style);
			// @DOCLINE It will return "Unknown" in the case that `style` is an invalid cursor style value.

			// @DOCLINE > These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.
			#endif

		// @DOCLINE ## Text input

			// @DOCLINE muCOSA is able to get text input from a window, which uses a callback to give Unicode character codepoints based on what the user is typing. This system is recommended over trying to emulate text input via keyboard callbacks, as it automatically handles variables such as key states and input method managers.

			// @DOCLINE muCOSA needs to know where the text input is being taken visually on the window's surface; for this, the "text cursor" exists, whose position dictates where features like virtual keyboards render their preview characters, and is relative to the top-leftest pixel on the window's surface, and whose coordinates should always be within the window's surface (AKA less than the window's dimensions).

			// @DOCLINE ### Get text input focus

			// @DOCLINE The function `muCOSA_window_get_text_input` gets text input from a window, defined below: @NLNT
			MUDEF void muCOSA_window_get_text_input(muCOSAContext* context, muCOSAResult* result, muWindow win, uint32_m text_cursor_x, uint32_m text_cursor_y, void (*callback)(muWindow window, uint8_m* data));

			// @DOCLINE The callback will be called with a UTF-8-encoded character representing what character has been input by the user. This callback will only be called while the given window is updating (AKA while `muCOSA_window_update` is being called on it), just like all other window callbacks.

			// @DOCLINE Once text input is successfully retrieved for a window, it should be manually let go of by the user at some point before the window is destroyed and before this function is called on another window. Text input stops being sent to the window while it's unfocused, but text input focus is still retained, and does not need to be called again one the window is refocused.

			// @DOCLINE > The macro `mu_window_get_text_input` is the non-result-checking equivalent, and the macro `mu_window_get_text_input_focus_` is the result-checking equivalent.
			#define mu_window_get_text_input(...) muCOSA_window_get_text_input(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
			#define mu_window_get_text_input_(result, ...) muCOSA_window_get_text_input(muCOSA_global_context, result, __VA_ARGS__)

			// @DOCLINE ### Let go of text input focus

			// @DOCLINE The function `muCOSA_window_let_text_input` lets go of text input focus for the given window that has text input focus, defined below: @NLNT
			MUDEF void muCOSA_window_let_text_input(muCOSAContext* context, muWindow win);

			// @DOCLINE The given window must have text input focus before this function is called. If the parameters are valid, this function cannot fail, and thus, has no `result` parameter.

			// @DOCLINE > The macro `mu_window_destroy` is the non-result-checking equivalent.
			#define mu_window_let_text_input(...) muCOSA_window_let_text_input(muCOSA_global_context, __VA_ARGS__)

			// @DOCLINE ### Update text cursor position

			// @DOCLINE The function `muCOSA_window_update_text_cursor` updates the position of a text cursor for a window that has text input focus, defined below: @NLNT
			MUDEF void muCOSA_window_update_text_cursor(muCOSAContext* context, muCOSAResult* result, muWindow win, uint32_m x, uint32_m y);

			// @DOCLINE The given window must have text input focus before this function is called.

			// @DOCLINE > The macro `mu_window_update_text_cursor` is the non-result-checking equivalent, and the macro `mu_window_update_text_cursor_` is the result-checking equivalent.
			#define mu_window_update_text_cursor(...) muCOSA_window_update_text_cursor(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
			#define mu_window_update_text_cursor_(result, ...) muCOSA_window_update_text_cursor(muCOSA_global_context, result, __VA_ARGS__)

		// @DOCLINE ## Pixel format

			// @DOCLINE A window's pixel format is used to define what data will be used when representing the window's surface. Its respective type is `muPixelFormat`, and has the following members:

			struct muPixelFormat {
				// @DOCLINE * `@NLFT red_bits` - the amount of bits used for the red channel.
				uint16_m red_bits;
				// @DOCLINE * `@NLFT green_bits` - the amount of bits used for the green channel.
				uint16_m green_bits;
				// @DOCLINE * `@NLFT blue_bits` - the amount of bits used for the blue channel.
				uint16_m blue_bits;
				// @DOCLINE * `@NLFT alpha_bits` - the amount of bits used for the alpha channel.
				uint16_m alpha_bits;

				// @DOCLINE * `@NLFT depth_bits` - the amount of bits used for the depth channel.
				uint16_m depth_bits;
				// @DOCLINE * `@NLFT stencil_bits` - the amount of bits used for the stencil channel.
				uint16_m stencil_bits;

				// @DOCLINE * `@NLFT samples` - the amount of samples used for each pixel. A value of 1 means no multi-sampling is performed. Any value other than 1 indicates multi-sampling, and must be a power of 2.
				uint8_m samples;
			};

			// @DOCLINE 0 bits means that the data does not include it; for example, if `depth_bits` is equal to 0, then no depth data is defined in the pixel format.

		// @DOCLINE ## Graphics APIs

			typedef uint16_m muGraphicsAPI;

			// @DOCLINE muCOSA supports windows creating graphics APIs associated with them (respective type `muGraphicsAPI`, typedef for `uint16_m`). These are the currently defined and supported graphics APIs:

			// @DOCLINE * `MU_NULL_GRAPHICS_API` - unknown/no graphics API. This value can be given when the user does not want a window to be associated with a particular graphics API, and will attempt to load no graphics API.
			#define MU_NULL_GRAPHICS_API 0

			// == OpenGL (1-256) ==

			// @DOCLINE * `MU_OPENGL_1_0` - [OpenGL v1.0](https://registry.khronos.org/OpenGL/specs/gl/glspec10.pdf).
			#define MU_OPENGL_1_0 1
			// @DOCLINE * `MU_OPENGL_1_1` - [OpenGL v1.1](https://registry.khronos.org/OpenGL/specs/gl/glspec11.pdf).
			#define MU_OPENGL_1_1 2
			// @DOCLINE * `MU_OPENGL_1_2` - [OpenGL v1.2](https://registry.khronos.org/OpenGL/specs/gl/glspec121.pdf).
			#define MU_OPENGL_1_2 3
			// @DOCLINE * `MU_OPENGL_1_2_1` - [OpenGL v1.2.1](https://registry.khronos.org/OpenGL/specs/gl/glspec121.pdf).
			#define MU_OPENGL_1_2_1 4
			// @DOCLINE * `MU_OPENGL_1_3` - [OpenGL v1.3](https://registry.khronos.org/OpenGL/specs/gl/glspec13.pdf).
			#define MU_OPENGL_1_3 5
			// @DOCLINE * `MU_OPENGL_1_4` - [OpenGL v1.4](https://registry.khronos.org/OpenGL/specs/gl/glspec14.pdf).
			#define MU_OPENGL_1_4 6
			// @DOCLINE * `MU_OPENGL_1_5` - [OpenGL v1.5](https://registry.khronos.org/OpenGL/specs/gl/glspec15.pdf).
			#define MU_OPENGL_1_5 7
			// @DOCLINE * `MU_OPENGL_2_0` - [OpenGL v2.0](https://registry.khronos.org/OpenGL/specs/gl/glspec20.pdf).
			#define MU_OPENGL_2_0 8
			// @DOCLINE * `MU_OPENGL_2_1` - [OpenGL v2.1](https://registry.khronos.org/OpenGL/specs/gl/glspec21.pdf).
			#define MU_OPENGL_2_1 9
			// @DOCLINE * `MU_OPENGL_3_0` - [OpenGL v3.0](https://registry.khronos.org/OpenGL/specs/gl/glspec30.pdf).
			#define MU_OPENGL_3_0 10
			// @DOCLINE * `MU_OPENGL_3_1` - [OpenGL v3.1](https://registry.khronos.org/OpenGL/specs/gl/glspec31.pdf).
			#define MU_OPENGL_3_1 11
			// @DOCLINE * `MU_OPENGL_3_2_CORE` - [OpenGL v3.2 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec32.core.pdf).
			#define MU_OPENGL_3_2_CORE 12
			// @DOCLINE * `MU_OPENGL_3_2_COMPATIBILITY` - [OpenGL v3.2 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec32.compatibility.pdf).
			#define MU_OPENGL_3_2_COMPATIBILITY 13
			// @DOCLINE * `MU_OPENGL_3_3_CORE` - [OpenGL v3.3 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec33.core.pdf).
			#define MU_OPENGL_3_3_CORE 14
			// @DOCLINE * `MU_OPENGL_3_3_COMPATIBILITY` - [OpenGL v3.3 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec33.compatibility.pdf).
			#define MU_OPENGL_3_3_COMPATIBILITY 15
			// @DOCLINE * `MU_OPENGL_4_0_CORE` - [OpenGL v4.0 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec40.core.pdf).
			#define MU_OPENGL_4_0_CORE 16
			// @DOCLINE * `MU_OPENGL_4_0_COMPATIBILITY` - [OpenGL v4.0 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec40.compatibility.pdf).
			#define MU_OPENGL_4_0_COMPATIBILITY 17
			// @DOCLINE * `MU_OPENGL_4_1_CORE` - [OpenGL v4.1 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec41.core.pdf).
			#define MU_OPENGL_4_1_CORE 18
			// @DOCLINE * `MU_OPENGL_4_1_COMPATIBILITY` - [OpenGL v4.1 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec41.compatibility.pdf).
			#define MU_OPENGL_4_1_COMPATIBILITY 19
			// @DOCLINE * `MU_OPENGL_4_2_CORE` - [OpenGL v4.2 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec42.core.pdf).
			#define MU_OPENGL_4_2_CORE 20
			// @DOCLINE * `MU_OPENGL_4_2_COMPATIBILITY` - [OpenGL v4.2 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec42.compatibility.pdf).
			#define MU_OPENGL_4_2_COMPATIBILITY 21
			// @DOCLINE * `MU_OPENGL_4_3_CORE` - [OpenGL v4.3 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec43.core.pdf).
			#define MU_OPENGL_4_3_CORE 22
			// @DOCLINE * `MU_OPENGL_4_3_COMPATIBILITY` - [OpenGL v4.3 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec43.compatibility.pdf).
			#define MU_OPENGL_4_3_COMPATIBILITY 23
			// @DOCLINE * `MU_OPENGL_4_4_CORE` - [OpenGL v4.4 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec44.core.pdf).
			#define MU_OPENGL_4_4_CORE 24
			// @DOCLINE * `MU_OPENGL_4_4_COMPATIBILITY` - [OpenGL v4.4 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec44.compatibility.pdf).
			#define MU_OPENGL_4_4_COMPATIBILITY 25
			// @DOCLINE * `MU_OPENGL_4_5_CORE` - [OpenGL v4.5 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec45.core.pdf).
			#define MU_OPENGL_4_5_CORE 26
			// @DOCLINE * `MU_OPENGL_4_5_COMPATIBILITY` - [OpenGL v4.5 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec45.compatibility.pdf).
			#define MU_OPENGL_4_5_COMPATIBILITY 27
			// @DOCLINE * `MU_OPENGL_4_6_CORE` - [OpenGL v4.6 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec46.core.pdf).
			#define MU_OPENGL_4_6_CORE 28
			// @DOCLINE * `MU_OPENGL_4_6_COMPATIBILITY` - [OpenGL v4.6 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec46.compatibility.pdf).
			#define MU_OPENGL_4_6_COMPATIBILITY 29

			// @DOCLINE Note that OpenGL will only work if `MU_SUPPORT_OPENGL` is defined before `muCOSA.h` is first included.

			// @DOCLINE ### Graphics API macro customization

			// @DOCLINE Files necessary to define OpenGL features (such as `gl/gh.`/`gl/glu.h` on Win32) are automatically included if `MU_SUPPORT_OPENGL` is defined; the inclusion of these files can be manually turned off (in case they have already been included) via defining `MUCOSA_NO_INCLUDE_OPENGL`.

			#ifdef MUCOSA_NAMES
			// @DOCLINE ### Graphics API names

			// @DOCLINE The name function `mu_graphics_api_get_name` returns a `const char*` representation of the given graphics API (for example, `MU_OPENGL_3_3_CORE` returns "MU_OPENGL_3_3_CORE"), defined below: @NLNT
			MUDEF const char* mu_graphics_api_get_name(muGraphicsAPI api);
			// @DOCLINE It will return "MU_UNKNOWN" in the case that `api` is an invalid graphics API value.

			// @DOCLINE The name function `mu_graphics_api_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_OPENGL_3_3_CORE` returns "OpenGL 3.3 (Core Profile)"), defined below: @NLNT
			MUDEF const char* mu_graphics_api_get_nice_name(muGraphicsAPI api);
			// @DOCLINE It will return "Unknown" in the case that `api` is an invalid graphics API value.

			// @DOCLINE > These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.
			#endif

		// @DOCLINE ## OpenGL context

			#define muGLContext void*

			// @DOCLINE A window can be used to create a valid OpenGL context that is linked to the window (respective type `muGLContext`; macro for `void*`). Any operations to an OpenGL context are regarding to the currently bound OpenGL context on the current thread (besides the binding function itself), so some OpenGL context-related functions do not take the actual OpenGL context as a parameter.

			// @DOCLINE ### Create / Destroy OpenGL context

			// @DOCLINE The function `muCOSA_gl_context_create` creates an OpenGL context, defined below: @NLNT
			MUDEF muGLContext muCOSA_gl_context_create(muCOSAContext* context, muCOSAResult* result, muWindow win, muGraphicsAPI api);

			// @DOCLINE Every successfully created OpenGL context must be destroyed before the window that created it is destroyed. This function does not modify any previously binded OpenGL context.

			// @DOCLINE > The macro `mu_gl_context_create` is the non-result-checking equivalent, and the macro `mu_gl_context_create_` is the result-checking equivalent.
			#define mu_gl_context_create(...) muCOSA_gl_context_create(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
			#define mu_gl_context_create_(result, ...) muCOSA_gl_context_create(muCOSA_global_context, result, __VA_ARGS__)

			// @DOCLINE The function `muCOSA_gl_context_destroy` destroys an OpenGL context, defined below: @NLNT
			MUDEF muGLContext muCOSA_gl_context_destroy(muCOSAContext* context, muWindow win, muGLContext gl_context);

			// @DOCLINE The destruction function cannot fail if given a proper context and window, and thus, there is no `result` parameter.

			// @DOCLINE > The macro `mu_gl_context_destroy` is the non-result-checking equivalent.
			#define mu_gl_context_destroy(...) muCOSA_gl_context_destroy(muCOSA_global_context, __VA_ARGS__)

			// @DOCLINE ### Bind OpenGL context

			// @DOCLINE The function `muCOSA_gl_bind` binds an OpenGL context to the current thread, defined below: @NLNT
			MUDEF void muCOSA_gl_bind(muCOSAContext* context, muCOSAResult* result, muWindow win, muGLContext gl_context);

			// @DOCLINE If `gl_context` is not 0, it is expected to be a valid OpenGL context. If `gl_context` is 0, this function unbinds any OpenGL context currently binded on the calling thread; doing this without any OpenGL context previously binded is undefined behavior.

			// @DOCLINE This function is, unfortunately, one of the few functions whose defined functionality can differ based on the currently running window system. If this function gives a fatal return value on Win32, the previously binded OpenGL context is unbinded; on X11, the previously binded OpenGL context stays binded.

			// @DOCLINE > The macro `mu_gl_bind` is the non-result-checking equivalent, and the macro `mu_gl_bind_` is the result-checking equivalent.
			#define mu_gl_bind(...) muCOSA_gl_bind(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
			#define mu_gl_bind_(result, ...) muCOSA_gl_bind(muCOSA_global_context, result, __VA_ARGS__)

			// @DOCLINE ### Swap buffers for OpenGL

			// @DOCLINE The function `muCOSA_gl_swap_buffers` swaps the buffers of a window associated with at least one OpenGL context, defined below: @NLNT
			MUDEF void muCOSA_gl_swap_buffers(muCOSAContext* context, muCOSAResult* result, muWindow win);

			// @DOCLINE Because this function acts purely on the window, and not directly with any associated OpenGL context, this should only be called if an OpenGL context exists that was created from the given window, and should be called every frame before `muCOSA_window_update` for as long as there is an OpenGL context associated with the window.

			// @DOCLINE > The macro `mu_gl_swap_buffers` is the non-result-checking equivalent, and the macro `mu_gl_swap_buffers_` is the result-checking equivalent.
			#define mu_gl_swap_buffers(...) muCOSA_gl_swap_buffers(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
			#define mu_gl_swap_buffers_(result, ...) muCOSA_gl_swap_buffers(muCOSA_global_context, result, __VA_ARGS__)

			// @DOCLINE ### Get procedure address

			// @DOCLINE The function `muCOSA_gl_get_proc_address` returns the address of a requested OpenGL function by name, defined below: @NLNT
			MUDEF void* muCOSA_gl_get_proc_address(muCOSAContext* context, const char* name);

			// @DOCLINE This function must be called with a valid OpenGL context binded. On failure, this function returns 0, and on success, returns a non-zero value. A valid address given by this function can only be guaranteed to be valid in relation to the currently binded OpenGL context.

			// @DOCLINE > The macro `mu_gl_get_proc_address` is the non-result-checking equivalent.
			#define mu_gl_get_proc_address(...) muCOSA_gl_get_proc_address(muCOSA_global_context, __VA_ARGS__)

			// @DOCLINE ### Swap interval

			// @DOCLINE The function `muCOSA_gl_swap_interval` acts as a call to `wglSwapIntervalEXT`, defined below: @NLNT
			MUDEF muBool muCOSA_gl_swap_interval(muCOSAContext* context, muCOSAResult* result, int interval);

			// @DOCLINE On Win32, this function returns the return value of `wglSwapIntervalEXT` if `result` is set to a non-fatal value, and 0 if otherwise.

			// @DOCLINE > The macro `mu_gl_swap_interval` is the non-result-checking equivalent, and the macro `mu_gl_swap_interval_` is the result-checking equivalent.
			#define mu_gl_swap_interval(...) muCOSA_gl_swap_interval(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
			#define mu_gl_swap_interval_(result, ...) muCOSA_gl_swap_interval(muCOSA_global_context, result, __VA_ARGS__)

	// @DOCLINE # Time

		// @DOCLINE Every muCOSA context has a "fixed time", which refers to the amount of seconds it has been since the context was first created, stored internally as a double. The "fixed time" is different than the "time", which is usually equal to the fixed time, unless it is manually overwritten by the user, which is available in the muCOSA API.

		// @DOCLINE ## Get fixed time

		// @DOCLINE The function `muCOSA_fixed_time_get` retrieves the current amount of fixed time for a muCOSA context, defined below: @NLNT
		MUDEF double muCOSA_fixed_time_get(muCOSAContext* context);

		// @DOCLINE This function cannot fail if the parameter(s) are valid.

		// @DOCLINE > The macro `mu_fixed_time_get` is the non-result-checking equivalent.
		#define mu_fixed_time_get() muCOSA_fixed_time_get(muCOSA_global_context)

		// @DOCLINE ## Get/Set time

		// @DOCLINE The function `muCOSA_time_get` retrieves the current amount of time for a muCOSA context, defined below: @NLNT
		MUDEF double muCOSA_time_get(muCOSAContext* context);

		// @DOCLINE > The macro `mu_time_get` is the non-result-checking equivalent.
		#define mu_time_get() muCOSA_time_get(muCOSA_global_context)

		// @DOCLINE The function `muCOSA_time_set` overwrites the current time for a muCOSA context, defined below: @NLNT
		MUDEF void muCOSA_time_set(muCOSAContext* context, double time);

		// @DOCLINE This function is valid to call with negative values.

		// @DOCLINE > The macro `mu_time_set` is the non-result-checking equivalent.
		#define mu_time_set(...) muCOSA_time_set(muCOSA_global_context, __VA_ARGS__)

		// @DOCLINE These functions cannot fail if the parameter(s) are valid.

	// @DOCLINE # Sleep

		// @DOCLINE The sleep function `muCOSA_sleep` is used to sleep for a given amount of seconds, defined below: @NLNT
		MUDEF void muCOSA_sleep(muCOSAContext* context, double time);

		// @DOCLINE This function cannot fail if the parameter(s) are valid.

		// @DOCLINE > The macro `mu_sleep` is the non-result-checking equivalent.
		#define mu_sleep(...) muCOSA_sleep(muCOSA_global_context, __VA_ARGS__)

	// @DOCLINE # Clipboard

		// @DOCLINE muCOSA offers functionality for getting and setting the current text clipboard.

		// @DOCLINE ## Get clipboard

		// @DOCLINE The function `muCOSA_clipboard_get` retrieves the current text clipboard, defined below: @NLNT
		MUDEF uint8_m* muCOSA_clipboard_get(muCOSAContext* context, muCOSAResult* result);

		// @DOCLINE On success, this function rather returns 0 (implying that there is no text clipboard set), or a pointer to data manually allocated by muCOSA; in the latter circumstance, it must be freed by the user manually when they are finished using the data.
		// @DOCLINE On failure, this function returns 0, and `result` is set to the failure value.

		// @DOCLINE > The macro `mu_clipboard_get` is the non-result-checking equivalent, and the macro `mu_clipboard_get_` is the result-checking equivalent.
		#define mu_clipboard_get() muCOSA_clipboard_get(muCOSA_global_context, &muCOSA_global_context->result)
		#define mu_clipboard_get_(result) muCOSA_clipboard_get(muCOSA_global_context, result)

		// @DOCLINE ## Set clipboard

		// @DOCLINE The function `muCOSA_clipboard_set` sets the current text clipboard, defined below: @NLNT
		MUDEF void muCOSA_clipboard_set(muCOSAContext* context, muCOSAResult* result, uint8_m* data, size_m datalen);

		// @DOCLINE On success, this function sets the current text clipboard to the given UTF-8 text data, of length `datalen` (including null-terminating character).

		// @DOCLINE > The macro `mu_clipboard_set` is the non-result-checking equivalent, and the macro `mu_clipboard_set_` is the result-checking equivalent.
		#define mu_clipboard_set(...) muCOSA_clipboard_set(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
		#define mu_clipboard_set_(result, ...) muCOSA_clipboard_set(muCOSA_global_context, result, __VA_ARGS__)

	// @DOCLINE # Result

		// @DOCLINE The type `muCOSAResult` (typedef for `uint16_m`) is used to represent how a task in muCOSA went. It has the following defined values:

		// == MUCOSA_... 0-4095 ==

		// @DOCLINE * `MUCOSA_SUCCESS` - the task succeeded; real value 0.
		#define MUCOSA_SUCCESS 0

		// @DOCLINE * `MUCOSA_FAILED_NULL_WINDOW_SYSTEM` - rather an invalid window system value was given by the user, the window system value given by the user was unsupported, or no supported window system could be found.
		#define MUCOSA_FAILED_NULL_WINDOW_SYSTEM 1

		// @DOCLINE * `MUCOSA_FAILED_MALLOC` - a call to `mu_malloc` failed, meaning that there is insufficient memory available to perform the task.
		#define MUCOSA_FAILED_MALLOC 2

		// @DOCLINE * `MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB` - an invalid `muWindowAttrib` value was given by the user.
		#define MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB 3

		// @DOCLINE * `MUCOSA_FAILED_REALLOC` - a call to `mu_realloc` failed, meaning that there is insufficient memory available to perform the task.
		#define MUCOSA_FAILED_REALLOC 4

		// @DOCLINE * `MUCOSA_FAILED_UNKNOWN_GRAPHICS_API` - an invalid `muGraphicsAPI` value was given by the user.
		#define MUCOSA_FAILED_UNKNOWN_GRAPHICS_API 5

		// @DOCLINE * `MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API` - a function relating to a graphics API was called despite the fact that support for the graphics API was not enabled.
		#define MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API 6

		// == MUCOSA_WIN32_... 4096-8191 ==

		// @DOCLINE * `MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR` - a conversion from a UTF-8 string to a wide character string failed, rather due to the conversion itself failing or the allocation of memory required for the conversion; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR 4096

		// @DOCLINE * `MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS` - a call to `RegisterClassExW` failed, meaning that the window class needed to create the window could not be created; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS 4097

		// @DOCLINE * `MUCOSA_WIN32_FAILED_CREATE_WINDOW` - a call to `CreateWindowExW` failed, meaning that the window could not be created; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_CREATE_WINDOW 4098

		// @DOCLINE * `MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB` - whatever function needed to retrieve the requested window attribute returned a non-success value; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB 4099
		// @DOCLINE    * In the case of dimensions, `GetClientRect` failed.
		// @DOCLINE    * In the case of position, `GetWindowRect` failed.
		// @DOCLINE    * In the case of cursor, rather `GetCursorPos` or `muCOSA_window_get(...MU_WINDOW_POSITION)` failed.

		// @DOCLINE * `MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB` - whatever function needed to modify the requested window attribute returned a non-success value; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB 4100
		// @DOCLINE    * In the case of title, `SetWindowTextW` failed.
		// @DOCLINE    * In the case of dimensions, rather `GetWindowInfo`, `AdjustWindowRect`, or `SetWindowPos` failed.
		// @DOCLINE    * In the case of position, `SetWindowPos` failed.
		// @DOCLINE    * In the case of cursor, rather `SetCursorPos` or `muCOSA_window_get(...MU_WINDOW_POSITION)` failed.

		// @DOCLINE * `MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS` - a call to `RegisterClassA` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
		#define MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS 4101

		// @DOCLINE * `MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW` - a call to `CreateWindowExA` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
		#define MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW 4102

		// @DOCLINE * `MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT` - a call to `GetPixelFormat` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
		#define MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT 4103

		// @DOCLINE * `MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT` - a call to `DescribePixelFormat` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
		#define MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT 4104

		// @DOCLINE * `MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT` - a call to `SetPixelFormat` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
		#define MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT 4105

		// @DOCLINE * `MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT` - a call to `wglCreateContext` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
		#define MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT 4106

		// @DOCLINE * `MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT` - a call to `wglMakeCurrent` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
		#define MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT 4107

		// @DOCLINE * `MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS` - the function `wglCreateContextAttribsARB` could not be found, which is necessary to creating OpenGL functions; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
		#define MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS 4108

		// @DOCLINE * `MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT` - the function `wglChoosePixelFormatARB` could not be found, which is necessary to creating OpenGL functions; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
		#define MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT 4109

		// @DOCLINE * `MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT` - the function `wglChoosePixelFormatARB` returned a failure value when creating an OpenGL context; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but the pixel format will likely not be what the user requested.
		#define MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT 4110

		// @DOCLINE * `MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT` - the function `GetPixelFormat` returned a failure value when creating an OpenGL context; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT 4111

		// @DOCLINE * `MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT` - the function `DescribePixelFormat` returned a failure value when creating an OpenGL context; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT 4112

		// @DOCLINE * `MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT` - the function `SetPixelFormat` returned a failure value when creating an OpenGL context; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT 4113

		// @DOCLINE * `MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT` - the function `wglCreateContextAttribsARB` returned a failure value when creating an OpenGL context; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT 4114

		// @DOCLINE * `MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT` - the function `wglMakeCurrent` returned a failure value when binding the OpenGL context; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT 4115

		// @DOCLINE * `MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS` - the function `SwapBuffers` returned a failure value when swapping the buffers; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS 4116

		// @DOCLINE * `MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION` - the corresponding OpenGL function could not be located; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION 4117

		// @DOCLINE * `MUCOSA_WIN32_FAILED_ASSOCIATE_IMM` - the function `ImmAssociateContextEx` returned a failure value when getting text input focus; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_ASSOCIATE_IMM 4118

		// @DOCLINE * `MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION` - the function `ImmSetCompositionWindow` returned a failure value when attempting to move it to the current text cursor position; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION 4119

		// @DOCLINE * `MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD` - the function `OpenClipboard` returned a failure value when attempting to retrieve the clipboard data (`muCOSA_clipboard_get`) or overwrite it (`muCOSA_clipboard_set`); this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD 4120

		// @DOCLINE * `MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA` - the function `GlobalLock` returned a failure value when attempting to retrieve a pointer to the clipboard data when attempting to retrieve the clipboard data (`muCOSA_clipboard_get`); this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA 4121

		// @DOCLINE * `MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT` - the conversion between UTF-16 wide-character data and UTF-8 `uint8_m*` data (rather converting from UTF-8 to UTF-16 when setting the clipboard data (`muCOSA_clipboard_set`), or converting from UTF-16 to UTF-8 when getting the clipboard data (`muCOSA_clipboard_get`)) failed, rather due to allocation or to the data itself being invalid; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT 4122

		// @DOCLINE * `MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA` - rather the function `GlobalAlloc` or `GlobalLock` failed when attempting to allocate and get a pointer to the global data for the clipboard when setting the clipboard (`muCOSA_clipboard_set`); this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA 4123

		// @DOCLINE * `MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA` - the function `SetClipboardData` failed when attempting to set the clipboard data; this is exclusive to Win32.
		#define MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA 4124

		// @DOCLINE All non-success values (unless explicitly stated otherwise) mean that the function fully failed, AKA it was "fatal", and the library continues as if the function had never been called; so, for example, if something was supposed to be allocated, but the function fatally failed, nothing was allocated.

		// @DOCLINE There are non-fatal failure values, which mean that the function still executed, but not fully to the extent that the user would expect from the function. The function `muCOSA_result_is_fatal` returns whether or not a given result function value is fatal, defined below: @NLNT
		MUDEF muBool muCOSA_result_is_fatal(muCOSAResult result);

		// @DOCLINE > This function returns `MU_TRUE` if the value of `result` is invalid. It returns `MU_FALSE` if `result` is equal to `MUCOSA_SUCCESS`.

		// @DOCLINE ## Result names
		#ifdef MUCOSA_NAMES

		// @DOCLINE The name function `muCOSA_result_get_name` returns a `const char*` representation of the given result value (for example, `MUCOSA_SUCCESS` returns "MUCOSA_SUCCESS"), defined below: @NLNT
		MUDEF const char* muCOSA_result_get_name(muCOSAResult result);

		// @DOCLINE It will return "MU_UNKNOWN" in the case that `result` is an invalid result value.

		// @DOCLINE > This function is a "name" function, and therefore is only defined if `MUCOSA_NAMES` is also defined by the user.

		#endif

	// @DOCLINE # C standard library dependencies

		// @DOCLINE muCOSA has several C standard library dependencies, all of which are overridable by defining them before the inclusion of its header. The following is a list of those dependencies.

		// @DOCLINE ## `string.h` dependencies
		#if !defined(mu_memcpy) || \
			!defined(mu_memset)

			#include <string.h>

			// @DOCLINE * `mu_memcpy` - equivalent to `memcpy`.
			#ifndef mu_memcpy
				#define mu_memcpy memcpy
			#endif

			// @DOCLINE * `mu_memset` - equivalent to `memset`.
			#ifndef mu_memset
				#define mu_memset memset
			#endif

		#endif /* string.h */

		// @DOCLINE ## `stdlib.h` dependencies
		#if !defined(mu_malloc) || \
			!defined (mu_free) || \
			!defined(mu_realloc)

			#include <stdlib.h>

			// @DOCLINE * `mu_malloc` - equivalent to `malloc`.
			#ifndef mu_malloc
				#define mu_malloc malloc
			#endif

			// @DOCLINE * `mu_free` - equivalent to `free`.
			#ifndef mu_free
				#define mu_free free
			#endif

			// @DOCLINE * `mu_realloc` - equivalent to `realloc`.
			#ifndef mu_realloc
				#define mu_realloc realloc
			#endif

		#endif /* stdlib.h */

	MU_CPP_EXTERN_END

#endif /* MUCOSA_H */

#ifdef MUCOSA_IMPLEMENTATION

	MU_CPP_EXTERN_START

	/* Macros */

		// OS recognition

		#ifndef MUCOSA_MANUAL_OS_SUPPORT

			// Win32
			#ifdef MU_WIN32
				#define MUCOSA_WIN32
				#define MUCOSA_WIN32_CALL(...) __VA_ARGS__
			#else
				#define MUCOSA_WIN32_CALL(...)
			#endif

		#endif /* MUCOSA_MANUAL_OS_SUPPORT */

		// Graphics API support

		#ifdef MU_SUPPORT_OPENGL
			#define MUCOSA_OPENGL_CALL(...) __VA_ARGS__
		#else
			#define MUCOSA_OPENGL_CALL(...)
		#endif

	/* Win32 */

	#ifdef MUCOSA_WIN32

		// For all of the stuff that can't be included directly (CURSE YOU BILL)
		#include <windows.h>

		/* Useful functions */

			// Hinstance retriever (thank you Raymond Chen)
			EXTERN_C IMAGE_DOS_HEADER __ImageBase;
			HINSTANCE muCOSAW32_get_hinstance(void) {
				return ((HINSTANCE)&__ImageBase);
			}

			// UTF-8 -> wchar_t (UTF-16)
			wchar_t* muCOSAW32_utf8_to_wchar(char* str) {
				// Get length needed for conversion
				// Note: we can use -1 here, since it's expected for it to be null-terminated
				int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
				if (len == 0) {
					return 0;
				}

				// Allocate data
				wchar_t* wstr = (wchar_t*)mu_malloc(len * sizeof(wchar_t));
				if (wstr == 0) {
					return 0;
				}

				// Perform conversion
				if (MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)wstr, len) == 0) {
					mu_free(wstr);
					return 0;
				}

				// Return converted data
				return wstr;
			}

			// wchar_t (UTF-16) -> UTF-8
			uint8_m* muCOSAW32_wchar_to_utf8(wchar_t* wstr) {
				// Get length needed
				int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
				if (!len) {
					return 0;
				}

				// Allocate data needed for conversion
				uint8_m* str = (uint8_m*)mu_malloc(len);
				if (!str) {
					return 0;
				}

				// Perform conversion
				if (!WideCharToMultiByte(CP_UTF8, 0, wstr, -1, (LPSTR)str, len, NULL, NULL)) {
					mu_free(str);
					return 0;
				}

				return str;
			}

			// Win32 virtual key code to muCOSA key code
			muKeyboardKey muCOSAW32_VK_to_muCOSA(WPARAM vk) {
				switch (vk) {
					default: return MU_KEYBOARD_UNKNOWN; break;
					case VK_BACK: return MU_KEYBOARD_BACKSPACE; break;
					case VK_TAB: return MU_KEYBOARD_TAB; break;
					case VK_CLEAR: return MU_KEYBOARD_CLEAR; break;
					case VK_RETURN: return MU_KEYBOARD_RETURN; break;
					case VK_PAUSE: return MU_KEYBOARD_PAUSE; break;
					case VK_ESCAPE: return MU_KEYBOARD_ESCAPE; break;
					case VK_MODECHANGE: return MU_KEYBOARD_MODECHANGE; break;
					case VK_SPACE: return MU_KEYBOARD_SPACE; break;
					case VK_PRIOR: return MU_KEYBOARD_PRIOR; break;
					case VK_NEXT: return MU_KEYBOARD_NEXT; break;
					case VK_END: return MU_KEYBOARD_END; break;
					case VK_HOME: return MU_KEYBOARD_HOME; break;
					case VK_LEFT: return MU_KEYBOARD_LEFT; break;
					case VK_UP: return MU_KEYBOARD_UP; break;
					case VK_RIGHT: return MU_KEYBOARD_RIGHT; break;
					case VK_DOWN: return MU_KEYBOARD_DOWN; break;
					case VK_SELECT: return MU_KEYBOARD_SELECT; break;
					case VK_PRINT: return MU_KEYBOARD_PRINT; break;
					case VK_EXECUTE: return MU_KEYBOARD_EXECUTE; break;
					case VK_INSERT: return MU_KEYBOARD_INSERT; break;
					case VK_DELETE: return MU_KEYBOARD_DELETE; break;
					case VK_HELP: return MU_KEYBOARD_HELP; break;
					case 0x30: return MU_KEYBOARD_0; break;
					case 0x31: return MU_KEYBOARD_1; break;
					case 0x32: return MU_KEYBOARD_2; break;
					case 0x33: return MU_KEYBOARD_3; break;
					case 0x34: return MU_KEYBOARD_4; break;
					case 0x35: return MU_KEYBOARD_5; break;
					case 0x36: return MU_KEYBOARD_6; break;
					case 0x37: return MU_KEYBOARD_7; break;
					case 0x38: return MU_KEYBOARD_8; break;
					case 0x39: return MU_KEYBOARD_9; break;
					case 0x41: return MU_KEYBOARD_A; break;
					case 0x42: return MU_KEYBOARD_B; break;
					case 0x43: return MU_KEYBOARD_C; break;
					case 0x44: return MU_KEYBOARD_D; break;
					case 0x45: return MU_KEYBOARD_E; break;
					case 0x46: return MU_KEYBOARD_F; break;
					case 0x47: return MU_KEYBOARD_G; break;
					case 0x48: return MU_KEYBOARD_H; break;
					case 0x49: return MU_KEYBOARD_I; break;
					case 0x4A: return MU_KEYBOARD_J; break;
					case 0x4B: return MU_KEYBOARD_K; break;
					case 0x4C: return MU_KEYBOARD_L; break;
					case 0x4D: return MU_KEYBOARD_M; break;
					case 0x4E: return MU_KEYBOARD_N; break;
					case 0x4F: return MU_KEYBOARD_O; break;
					case 0x50: return MU_KEYBOARD_P; break;
					case 0x51: return MU_KEYBOARD_Q; break;
					case 0x52: return MU_KEYBOARD_R; break;
					case 0x53: return MU_KEYBOARD_S; break;
					case 0x54: return MU_KEYBOARD_T; break;
					case 0x55: return MU_KEYBOARD_U; break;
					case 0x56: return MU_KEYBOARD_V; break;
					case 0x57: return MU_KEYBOARD_W; break;
					case 0x58: return MU_KEYBOARD_X; break;
					case 0x59: return MU_KEYBOARD_Y; break;
					case 0x5A: return MU_KEYBOARD_Z; break;
					case VK_LWIN: return MU_KEYBOARD_LEFT_WINDOWS; break;
					case VK_RWIN: return MU_KEYBOARD_RIGHT_WINDOWS; break;
					case VK_NUMPAD0: return MU_KEYBOARD_NUMPAD_0; break;
					case VK_NUMPAD1: return MU_KEYBOARD_NUMPAD_1; break;
					case VK_NUMPAD2: return MU_KEYBOARD_NUMPAD_2; break;
					case VK_NUMPAD3: return MU_KEYBOARD_NUMPAD_3; break;
					case VK_NUMPAD4: return MU_KEYBOARD_NUMPAD_4; break;
					case VK_NUMPAD5: return MU_KEYBOARD_NUMPAD_5; break;
					case VK_NUMPAD6: return MU_KEYBOARD_NUMPAD_6; break;
					case VK_NUMPAD7: return MU_KEYBOARD_NUMPAD_7; break;
					case VK_NUMPAD8: return MU_KEYBOARD_NUMPAD_8; break;
					case VK_NUMPAD9: return MU_KEYBOARD_NUMPAD_9; break;
					case VK_MULTIPLY: return MU_KEYBOARD_MULTIPLY; break;
					case VK_ADD: return MU_KEYBOARD_ADD; break;
					case VK_SEPARATOR: return MU_KEYBOARD_SEPARATOR; break;
					case VK_SUBTRACT: return MU_KEYBOARD_SUBTRACT; break;
					case VK_DECIMAL: return MU_KEYBOARD_DECIMAL; break;
					case VK_DIVIDE: return MU_KEYBOARD_DIVIDE; break;
					case VK_F1: return MU_KEYBOARD_F1; break;
					case VK_F2: return MU_KEYBOARD_F2; break;
					case VK_F3: return MU_KEYBOARD_F3; break;
					case VK_F4: return MU_KEYBOARD_F4; break;
					case VK_F5: return MU_KEYBOARD_F5; break;
					case VK_F6: return MU_KEYBOARD_F6; break;
					case VK_F7: return MU_KEYBOARD_F7; break;
					case VK_F8: return MU_KEYBOARD_F8; break;
					case VK_F9: return MU_KEYBOARD_F9; break;
					case VK_F10: return MU_KEYBOARD_F10; break;
					case VK_F11: return MU_KEYBOARD_F11; break;
					case VK_F12: return MU_KEYBOARD_F12; break;
					case VK_F13: return MU_KEYBOARD_F13; break;
					case VK_F14: return MU_KEYBOARD_F14; break;
					case VK_F15: return MU_KEYBOARD_F15; break;
					case VK_F16: return MU_KEYBOARD_F16; break;
					case VK_F17: return MU_KEYBOARD_F17; break;
					case VK_F18: return MU_KEYBOARD_F18; break;
					case VK_F19: return MU_KEYBOARD_F19; break;
					case VK_F20: return MU_KEYBOARD_F20; break;
					case VK_F21: return MU_KEYBOARD_F21; break;
					case VK_F22: return MU_KEYBOARD_F22; break;
					case VK_F23: return MU_KEYBOARD_F23; break;
					case VK_F24: return MU_KEYBOARD_F24; break;
					case VK_NUMLOCK: return MU_KEYBOARD_NUMLOCK; break;
					case VK_SCROLL: return MU_KEYBOARD_SCROLL; break;
					case VK_LSHIFT: return MU_KEYBOARD_LEFT_SHIFT; break;
					case VK_RSHIFT: return MU_KEYBOARD_RIGHT_SHIFT; break;
					case VK_LCONTROL: return MU_KEYBOARD_LEFT_CONTROL; break;
					case VK_RCONTROL: return MU_KEYBOARD_RIGHT_CONTROL; break;
					case VK_LMENU: return MU_KEYBOARD_LEFT_MENU; break;
					case VK_RMENU: return MU_KEYBOARD_RIGHT_MENU; break;
					case VK_ATTN: return MU_KEYBOARD_ATTN; break;
					case VK_CRSEL: return MU_KEYBOARD_CRSEL; break;
					case VK_EXSEL: return MU_KEYBOARD_EXSEL; break;
					case VK_EREOF: return MU_KEYBOARD_EREOF; break;
					case VK_PLAY: return MU_KEYBOARD_PLAY; break;
					case VK_PA1: return MU_KEYBOARD_PA1; break;
				}
			}

			// muCOSA cursor style to Win32 cursor
			void* muCOSAW32_muCOSA_cursor_to_W32(muCursorStyle style) {
				switch (style) {
					default: return IDC_ARROW; break;
					case MU_CURSOR_ARROW: return IDC_ARROW; break;
					case MU_CURSOR_IBEAM: return IDC_IBEAM; break;
					case MU_CURSOR_WAIT: return IDC_WAIT; break;
					case MU_CURSOR_WAIT_ARROW: return IDC_APPSTARTING; break;
					case MU_CURSOR_CROSSHAIR: return IDC_CROSS; break;
					case MU_CURSOR_HAND: return IDC_HAND; break;
					case MU_CURSOR_SIZE_LR: return IDC_SIZEWE; break;
					case MU_CURSOR_SIZE_TB: return IDC_SIZENS; break;
					case MU_CURSOR_SIZE_TL_BR: return IDC_SIZENWSE; break;
					case MU_CURSOR_SIZE_TR_BL: return IDC_SIZENESW; break;
					case MU_CURSOR_SIZE_ALL: return IDC_SIZEALL; break;
					case MU_CURSOR_NO: return IDC_NO; break;
				}
			}

			// muCOSA keystate to Win32
			int muCOSAW32_keystate_to_W32(muKeyboardState state) {
				switch (state) {
					default: return VK_NONAME; break;
					case MU_KEYSTATE_CAPS_LOCK: return VK_CAPITAL; break;
					case MU_KEYSTATE_SCROLL_LOCK: return VK_SCROLL; break;
					case MU_KEYSTATE_NUM_LOCK: return VK_NUMLOCK; break;
				}
			}

		/* OpenGL */

		#ifdef MU_SUPPORT_OPENGL

			/* Inclusion and common definition */

				// Common OpenGL definitions
				#ifndef MUCOSA_NO_INCLUDE_OPENGL
					// Only include if "glClearColor" isn't defined; this is to avoid
					// accidental double definition in many circumstances.
					#ifndef glClearColor
						#include <gl/gl.h>
						#include <gl/glu.h>
					#endif
				#endif

				// Device context -> valid OpenGL context
				#include <wingdi.h>

				// Typedefs for functions we need
				// - wglCreateContextAttribs; needed for specifying exact version when creating context
				typedef HGLRC WINAPI muCOSAW32_wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int* attribList);
				// - wglChoosePixelFormat; needed for choosing a more thorough pixel format when creating context
				typedef BOOL WINAPI muCOSAW32_wglChoosePixelFormatARB_type(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT uMaxFormats, int* piFormats, UINT* nNumFormats);
				// - wglSwapInterval; not needed in context creation, but used for swap interval later
				typedef BOOL WINAPI muCOSAW32_wglSwapIntervalEXT_type(int interval);

				// Struct to hold WGL functions needed for context creation
				struct muCOSAW32_WGL {
					muCOSAW32_wglCreateContextAttribsARB_type* CreateContextAttribs;
					muCOSAW32_wglChoosePixelFormatARB_type* ChoosePixelFormat;
					muCOSAW32_wglSwapIntervalEXT_type* SwapInterval;
				};
				typedef struct muCOSAW32_WGL muCOSAW32_WGL;

			/* WGL tokens */

				// https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_create_context.txt

				#define MUCOSAWGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
				#define MUCOSAWGL_CONTEXT_MINOR_VERSION_ARB             0x2092
				#define MUCOSAWGL_CONTEXT_LAYER_PLANE_ARB               0x2093
				#define MUCOSAWGL_CONTEXT_FLAGS_ARB                     0x2094
				#define MUCOSAWGL_CONTEXT_PROFILE_MASK_ARB              0x9126

				#define MUCOSAWGL_CONTEXT_DEBUG_BIT_ARB                 0x0001
				#define MUCOSAWGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB    0x0002

				#define MUCOSAWGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
				#define MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

				#define MUCOSAWGL_ERROR_INVALID_VERSION_ARB                 0x2095
				#define MUCOSAWGL_ERROR_INVALID_PROFILE_ARB                 0x2096

				// https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt

				#define MUCOSAWGL_NUMBER_PIXEL_FORMATS_ARB    0x2000
				#define MUCOSAWGL_DRAW_TO_WINDOW_ARB          0x2001
				#define MUCOSAWGL_DRAW_TO_BITMAP_ARB          0x2002
				#define MUCOSAWGL_ACCELERATION_ARB            0x2003
				#define MUCOSAWGL_NEED_PALETTE_ARB            0x2004
				#define MUCOSAWGL_NEED_SYSTEM_PALETTE_ARB     0x2005
				#define MUCOSAWGL_SWAP_LAYER_BUFFERS_ARB      0x2006
				#define MUCOSAWGL_SWAP_METHOD_ARB             0x2007
				#define MUCOSAWGL_NUMBER_OVERLAYS_ARB         0x2008
				#define MUCOSAWGL_NUMBER_UNDERLAYS_ARB        0x2009
				#define MUCOSAWGL_TRANSPARENT_ARB             0x200A
				#define MUCOSAWGL_TRANSPARENT_RED_VALUE_ARB   0x2037
				#define MUCOSAWGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
				#define MUCOSAWGL_TRANSPARENT_BLUE_VALUE_ARB  0x2039
				#define MUCOSAWGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
				#define MUCOSAWGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
				#define MUCOSAWGL_SHARE_DEPTH_ARB             0x200C
				#define MUCOSAWGL_SHARE_STENCIL_ARB           0x200D
				#define MUCOSAWGL_SHARE_ACCUM_ARB             0x200E
				#define MUCOSAWGL_SUPPORT_GDI_ARB             0x200F
				#define MUCOSAWGL_SUPPORT_OPENGL_ARB          0x2010
				#define MUCOSAWGL_DOUBLE_BUFFER_ARB           0x2011
				#define MUCOSAWGL_STEREO_ARB                  0x2012
				#define MUCOSAWGL_PIXEL_TYPE_ARB              0x2013
				#define MUCOSAWGL_COLOR_BITS_ARB              0x2014
				#define MUCOSAWGL_RED_BITS_ARB                0x2015
				#define MUCOSAWGL_RED_SHIFT_ARB               0x2016
				#define MUCOSAWGL_GREEN_BITS_ARB              0x2017
				#define MUCOSAWGL_GREEN_SHIFT_ARB             0x2018
				#define MUCOSAWGL_BLUE_BITS_ARB               0x2019
				#define MUCOSAWGL_BLUE_SHIFT_ARB              0x201A
				#define MUCOSAWGL_ALPHA_BITS_ARB              0x201B
				#define MUCOSAWGL_ALPHA_SHIFT_ARB             0x201C
				#define MUCOSAWGL_ACCUM_BITS_ARB              0x201D
				#define MUCOSAWGL_ACCUM_RED_BITS_ARB          0x201E
				#define MUCOSAWGL_ACCUM_GREEN_BITS_ARB        0x201F
				#define MUCOSAWGL_ACCUM_BLUE_BITS_ARB         0x2020
				#define MUCOSAWGL_ACCUM_ALPHA_BITS_ARB        0x2021
				#define MUCOSAWGL_DEPTH_BITS_ARB              0x2022
				#define MUCOSAWGL_STENCIL_BITS_ARB            0x2023
				#define MUCOSAWGL_AUX_BUFFERS_ARB             0x2024

				#define MUCOSAWGL_NO_ACCELERATION_ARB         0x2025
				#define MUCOSAWGL_GENERIC_ACCELERATION_ARB    0x2026
				#define MUCOSAWGL_FULL_ACCELERATION_ARB       0x2027

				#define MUCOSAWGL_SWAP_EXCHANGE_ARB           0x2028
				#define MUCOSAWGL_SWAP_COPY_ARB               0x2029
				#define MUCOSAWGL_SWAP_UNDEFINED_ARB          0x202A

				#define MUCOSAWGL_TYPE_RGBA_ARB               0x202B
				#define MUCOSAWGL_TYPE_COLORINDEX_ARB         0x202C

				// https://nehe.gamedev.net/tutorial/fullscreen_antialiasing/16008/

				#define MUCOSAWGL_SAMPLE_BUFFERS_ARB  0x2041
				#define MUCOSAWGL_SAMPLES_ARB     0x2042

			/* Functions */

				// Retrieves OpenGL extensions needed for context creation
				muCOSAResult muCOSAW32_get_opengl_extensions(muCOSAW32_WGL* wgl) {
					// Create temp window class
					WNDCLASSA wclass = MU_ZERO_STRUCT(WNDCLASSA);
					wclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
					wclass.lpfnWndProc = DefWindowProcA;
					wclass.hInstance = muCOSAW32_get_hinstance();
					wclass.lpszClassName = "Dummy WGL window class";
					if (!RegisterClassA(&wclass)) {
						return MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS;
					}

					// Create temp window
					HWND win = CreateWindowExA(
						0, wclass.lpszClassName, "Dummy WGL window", 0,
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0,
						wclass.hInstance, 0
					);
					if (win == NULL) {
						UnregisterClassA(wclass.lpszClassName, wclass.hInstance);
						return MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW;
					}

					// Get device context from this window
					HDC dc = GetDC(win);

					// Use pixel format number 1
					// This is a HACK and a half, but works
					int pixel_format = 1;
					PIXELFORMATDESCRIPTOR format_desc;
					if (!DescribePixelFormat(dc, pixel_format, sizeof(format_desc), &format_desc)) {
						ReleaseDC(win, dc);
						DestroyWindow(win);
						UnregisterClassA(wclass.lpszClassName, wclass.hInstance);
						return MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT;
					}

					// Set pixel format. This doesn't make much sense, this is already definitionally
					// the pixel format of the device, but it doesn't work if I don't do this, so, :L
					// Note: this might not be okay, and we may need to specify SOME pixel format,
					// but I don't know.
					if (!SetPixelFormat(dc, pixel_format, &format_desc)) {
						ReleaseDC(win, dc);
						DestroyWindow(win);
						UnregisterClassA(wclass.lpszClassName, wclass.hInstance);
						return MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT;
					}

					// Create dummy context
					HGLRC context = wglCreateContext(dc);
					if (!context) {
						ReleaseDC(win, dc);
						DestroyWindow(win);
						UnregisterClassA(wclass.lpszClassName, wclass.hInstance);
						return MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT;
					}
					// Bind dummy context
					if (!wglMakeCurrent(dc, context)) {
						wglDeleteContext(context);
						ReleaseDC(win, dc);
						DestroyWindow(win);
						UnregisterClassA(wclass.lpszClassName, wclass.hInstance);
						return MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT;
					}

					// Find necessary functions
					PROC wglCreateContextAttribsARB_proc = wglGetProcAddress("wglCreateContextAttribsARB");
					PROC wglChoosePixelFormatARB_proc    = wglGetProcAddress("wglChoosePixelFormatARB");
					// Find helpful functions
					PROC wglSwapIntervalEXT_proc         = wglGetProcAddress("wglSwapIntervalEXT");

					// Destroy resources
					wglMakeCurrent(dc, 0);
					wglDeleteContext(context);
					ReleaseDC(win, dc);
					DestroyWindow(win);
					UnregisterClassA(wclass.lpszClassName, wclass.hInstance);

					// Detect if any required functions went unfound
					if (!wglCreateContextAttribsARB_proc) {
						return MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS;
					}
					if (!wglChoosePixelFormatARB_proc) {
						return MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT;
					}

					// Copy over functions
					mu_memcpy(&wgl->CreateContextAttribs, &wglCreateContextAttribsARB_proc, sizeof(PROC));
					mu_memcpy(&wgl->ChoosePixelFormat,    &wglChoosePixelFormatARB_proc,    sizeof(PROC));
					mu_memcpy(&wgl->SwapInterval,         &wglSwapIntervalEXT_proc,         sizeof(PROC));
					return MUCOSA_SUCCESS;
				}

				// Creates an OpenGL context
				muCOSAResult muCOSAW32_create_opengl_context(HDC dc, int win_pixel_format, muCOSAW32_WGL* wgl, muPixelFormat* format, HGLRC* context, muGraphicsAPI api, muBool* set) {
					muCOSAResult res = MUCOSA_SUCCESS;
					// Choose pixel format
					int pixel_format;

					// - If a pixel format is supplied by the user:
					if (format) {
						// Set up an attribute field based on pixel format
						int pixel_format_attributes[] = {
							MUCOSAWGL_DRAW_TO_WINDOW_ARB, MU_TRUE,
							MUCOSAWGL_SUPPORT_OPENGL_ARB, MU_TRUE,
							MUCOSAWGL_DOUBLE_BUFFER_ARB,  MU_TRUE,
							MUCOSAWGL_SAMPLE_BUFFERS_ARB, MU_TRUE, // (?)
							MUCOSAWGL_ACCELERATION_ARB,   MUCOSAWGL_FULL_ACCELERATION_ARB, // (?)
							MUCOSAWGL_PIXEL_TYPE_ARB,     MUCOSAWGL_TYPE_RGBA_ARB, // (?)
							MUCOSAWGL_RED_BITS_ARB,       format->red_bits,
							MUCOSAWGL_GREEN_BITS_ARB,     format->green_bits,
							MUCOSAWGL_BLUE_BITS_ARB,      format->blue_bits,
							MUCOSAWGL_ALPHA_BITS_ARB,     format->alpha_bits,
							MUCOSAWGL_DEPTH_BITS_ARB,     format->depth_bits,
							MUCOSAWGL_STENCIL_BITS_ARB,   format->stencil_bits,
							MUCOSAWGL_SAMPLES_ARB,        format->samples,
							0
						};
						// Find corresponding format using WGL
						UINT format_count = 0;
						// - Does the return value of this indicate error? Can't find documentation
						wgl->ChoosePixelFormat(dc, pixel_format_attributes, 0, 1, &pixel_format, &format_count);
						if (!format_count) {
							res = MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT;
						}
					}

					// - If a pixel format isn't supplied by the user (or it failed)
					if ((!format) || (res != MUCOSA_SUCCESS)) {
						// Get default pixel format from window
						pixel_format = win_pixel_format;
					}

					// Describe pixel format
					PIXELFORMATDESCRIPTOR format_desc;
					if (!DescribePixelFormat(dc, pixel_format, sizeof(format_desc), &format_desc)) {
						return MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT;
					}
					// Set pixel format @TODO Figure this out in window creation
					if (!*set) {
						if (!SetPixelFormat(dc, pixel_format, &format_desc)) {
							return MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT;
						}
						*set = MU_TRUE;
					}

					// OpenGL version info
					int opengl_attributes[] = {
						MUCOSAWGL_CONTEXT_MAJOR_VERSION_ARB, 0,
						MUCOSAWGL_CONTEXT_MINOR_VERSION_ARB, 0,
						MUCOSAWGL_CONTEXT_PROFILE_MASK_ARB, MUCOSAWGL_CONTEXT_CORE_PROFILE_BIT_ARB,
						0
					};

					// - Fill in OpenGL version info based on graphics API value
					switch (api) {
						default: return MUCOSA_FAILED_UNKNOWN_GRAPHICS_API; break;
						case MU_OPENGL_1_0: {
							opengl_attributes[1] = 1;
							opengl_attributes[3] = 0;
							opengl_attributes[4] = 0;
						} break;
						case MU_OPENGL_1_1: {
							opengl_attributes[1] = 1;
							opengl_attributes[3] = 1;
							opengl_attributes[4] = 0;
						} break;
						case MU_OPENGL_1_2: {
							opengl_attributes[1] = 1;
							opengl_attributes[3] = 2;
							opengl_attributes[4] = 0;
						} break;
						// Don't know what to do, setting to 1.3
						case MU_OPENGL_1_2_1: {
							opengl_attributes[1] = 1;
							opengl_attributes[3] = 3;
							opengl_attributes[4] = 0;
						} break;
						case MU_OPENGL_1_3: {
							opengl_attributes[1] = 1;
							opengl_attributes[3] = 3;
							opengl_attributes[4] = 0;
						} break;
						case MU_OPENGL_1_4: {
							opengl_attributes[1] = 1;
							opengl_attributes[3] = 4;
							opengl_attributes[4] = 0;
						} break;
						case MU_OPENGL_1_5: {
							opengl_attributes[1] = 1;
							opengl_attributes[3] = 5;
							opengl_attributes[4] = 0;
						} break;
						case MU_OPENGL_2_0: {
							opengl_attributes[1] = 2;
							opengl_attributes[3] = 0;
							opengl_attributes[4] = 0;
						} break;
						case MU_OPENGL_2_1: {
							opengl_attributes[1] = 2;
							opengl_attributes[3] = 1;
							opengl_attributes[4] = 0;
						} break;
						case MU_OPENGL_3_0: {
							opengl_attributes[1] = 3;
							opengl_attributes[3] = 0;
							opengl_attributes[4] = 0;
						} break;
						case MU_OPENGL_3_1: {
							opengl_attributes[1] = 3;
							opengl_attributes[3] = 1;
							opengl_attributes[4] = 0;
						} break;
						case MU_OPENGL_3_2_CORE: {
							opengl_attributes[1] = 3;
							opengl_attributes[3] = 2;
						} break;
						case MU_OPENGL_3_2_COMPATIBILITY: {
							opengl_attributes[1] = 3;
							opengl_attributes[3] = 2;
							opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
						} break;
						case MU_OPENGL_3_3_CORE: {
							opengl_attributes[1] = 3;
							opengl_attributes[3] = 3;
						} break;
						case MU_OPENGL_3_3_COMPATIBILITY: {
							opengl_attributes[1] = 3;
							opengl_attributes[3] = 3;
							opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
						} break;
						case MU_OPENGL_4_0_CORE: {
							opengl_attributes[1] = 4;
							opengl_attributes[3] = 0;
						} break;
						case MU_OPENGL_4_0_COMPATIBILITY: {
							opengl_attributes[1] = 4;
							opengl_attributes[3] = 0;
							opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
						} break;
						case MU_OPENGL_4_1_CORE: {
							opengl_attributes[1] = 4;
							opengl_attributes[3] = 1;
						} break;
						case MU_OPENGL_4_1_COMPATIBILITY: {
							opengl_attributes[1] = 4;
							opengl_attributes[3] = 1;
							opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
						} break;
						case MU_OPENGL_4_2_CORE: {
							opengl_attributes[1] = 4;
							opengl_attributes[3] = 2;
						} break;
						case MU_OPENGL_4_2_COMPATIBILITY: {
							opengl_attributes[1] = 4;
							opengl_attributes[3] = 2;
							opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
						} break;
						case MU_OPENGL_4_3_CORE: {
							opengl_attributes[1] = 4;
							opengl_attributes[3] = 3;
						} break;
						case MU_OPENGL_4_3_COMPATIBILITY: {
							opengl_attributes[1] = 4;
							opengl_attributes[3] = 3;
							opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
						} break;
						case MU_OPENGL_4_4_CORE: {
							opengl_attributes[1] = 4;
							opengl_attributes[3] = 4;
						} break;
						case MU_OPENGL_4_4_COMPATIBILITY: {
							opengl_attributes[1] = 4;
							opengl_attributes[3] = 4;
							opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
						} break;
						case MU_OPENGL_4_5_CORE: {
							opengl_attributes[1] = 4;
							opengl_attributes[3] = 5;
						} break;
						case MU_OPENGL_4_5_COMPATIBILITY: {
							opengl_attributes[1] = 4;
							opengl_attributes[3] = 5;
							opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
						} break;
						case MU_OPENGL_4_6_CORE: {
							opengl_attributes[1] = 4;
							opengl_attributes[3] = 6;
						} break;
						case MU_OPENGL_4_6_COMPATIBILITY: {
							opengl_attributes[1] = 4;
							opengl_attributes[3] = 6;
							opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
						} break;
					}

					// Create context
					*context = wgl->CreateContextAttribs(dc, 0, opengl_attributes);
					if (!*context) {
						return MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT;
					}

					return res;
				}

		#endif /* MU_SUPPORT_OPENGL */

		/* Time */

			struct muCOSAW32_Time {
				// @DOCLINE The original time the context was created.
				double orig_time;
				// @DOCLINE The non-overwritable time.
				double fixed_time;
			};
			typedef struct muCOSAW32_Time muCOSAW32_Time;

			// https://stackoverflow.com/questions/1695288/getting-the-current-time-in-milliseconds-from-the-system-clock-in-windows
			double muCOSAW32_get_current_time(void) {
				// Get system time
				FILETIME file_time;
				GetSystemTimeAsFileTime(&file_time);

				// Format low + high time
				LONGLONG ll_now = 
					(LONGLONG)(file_time.dwLowDateTime) + ((LONGLONG)(file_time.dwHighDateTime) << 32LL)
				;
				// Return time properly divided
				return (double)(ll_now) / (double)(1.0e7);
			}

			// Initiates the time struct
			void muCOSAW32_time_init(muCOSAW32_Time* time) {
				// Set time to current time
				time->orig_time = time->fixed_time = muCOSAW32_get_current_time();
			}

		/* Clipboard */

			uint8_m* muCOSAW32_clipboard_get(muCOSAResult* result) {
				// Hold clipboard
				if (!OpenClipboard(NULL)) {
					MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD)
					return 0;
				}

				// Get handle to clipboard data (UTF-16)
				HANDLE data = GetClipboardData(CF_UNICODETEXT);
				if (!data) {
					// I'm pretty sure this can happen if no clipboard is available, so I'm not
					// throwing an error here
					CloseClipboard();
					return 0;
				}

				// Get clipboard data (UTF-16)
				wchar_t* utf16 = (wchar_t*)GlobalLock(data);
				if (!utf16) {
					MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA)
					CloseClipboard();
					return 0;
				}

				// Convert UTF-16 to UTF-8
				uint8_m* utf8 = muCOSAW32_wchar_to_utf8(utf16);
				if (!utf8) {
					MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT)
					GlobalUnlock(data);
					CloseClipboard();
					return 0;
				}

				// Close and return data
				GlobalUnlock(data);
				CloseClipboard();
				return utf8;
			}

			void muCOSAW32_clipboard_set(muCOSAResult* result, uint8_m* data, size_m datalen) {
				// Get length of UTF-16 equivalent
				int wlen = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)data, datalen, NULL, 0);
				if (!wlen) {
					MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT)
					return;
				}

				// Allocate global memory for UTF-16 string
				HGLOBAL g_mem = GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)(wlen)*sizeof(wchar_t));
				if (!g_mem) {
					MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA)
					return;
				}

				// Get memory pointer
				LPVOID p_mem = GlobalLock(g_mem);
				if (!p_mem) {
					MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA)
					GlobalFree(g_mem);
					return;
				}

				// Perform conversion
				if (!MultiByteToWideChar(CP_UTF8, 0, (LPCCH)data, datalen, (LPWSTR)p_mem, wlen)) {
					MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT)
					GlobalUnlock(g_mem);
					GlobalFree(g_mem);
					return;
				}

				// Release memory pointer
				GlobalUnlock(g_mem);

				// Hold clipboard
				if (!OpenClipboard(NULL)) {
					MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD)
					GlobalFree(g_mem);
					return;
				}

				// Empty pre-existing clipboard data
				if (!EmptyClipboard()) {
					// Assuming this can get triggered if the clipboard was already empty,
					// so no error thrown here.
				}

				// Set clipboard data
				if (!SetClipboardData(CF_UNICODETEXT, g_mem)) {
					MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA)
					GlobalFree(g_mem);
					CloseClipboard();
					return;
				}

				// Close clipboard and we're done
				CloseClipboard();
			}

		/* Context */

			struct muCOSAW32_Context {
				muCOSAW32_Time time;
				MUCOSA_OPENGL_CALL(muCOSAW32_WGL wgl;)
			};
			typedef struct muCOSAW32_Context muCOSAW32_Context;

			// Accessing/Deaccessing pointer map functions needed for contexts
			void muCOSAW32_window_pmap_access(void);
			void muCOSAW32_window_pmap_deaccess(void);

			muCOSAResult muCOSAW32_context_init(muCOSAW32_Context* context) {
				muCOSAResult res = MUCOSA_SUCCESS;
				// Mark newly created context for pmap
				muCOSAW32_window_pmap_access();
				// Initiate context time
				muCOSAW32_time_init(&context->time);
				// Load OpenGL extensions
				MUCOSA_OPENGL_CALL(
					mu_memset(&context->wgl, 0, sizeof(context->wgl));
					res = muCOSAW32_get_opengl_extensions(&context->wgl);
				)

				return res;
			}

			void muCOSAW32_context_term(muCOSAW32_Context* context) {
				// Remove context from pmap
				muCOSAW32_window_pmap_deaccess();
				return; if (context) {}
			}

		/* Context time */

			double muCOSAW32_fixed_time_get(muCOSAW32_Context* context) {
				// Return the difference between now and when the context was created
				return muCOSAW32_get_current_time() - context->time.fixed_time;
			}

			double muCOSAW32_time_get(muCOSAW32_Context* context) {
				// Return the difference between now and the overridable original time
				return muCOSAW32_get_current_time() - context->time.orig_time;
			}

			void muCOSAW32_time_set(muCOSAW32_Context* context, double time) {
				// Set time to current time minus the given time
				context->time.orig_time = muCOSAW32_get_current_time() - time;
			}

		/* Sleep */

			void muCOSAW32_sleep(double time) {
				// Sleep for the approx. amount of milliseconds
				Sleep((DWORD)(time*1000.0));
			}

		/* Window structs */

			struct muCOSAW32_Keymaps {
				// Keyboard keys
				muBool keyboard[MU_KEYBOARD_LENGTH];
				// Keystates
				muBool keystates[MU_KEYSTATE_LENGTH];
				// Mouse keys
				muBool mouse[MU_MOUSE_LENGTH];
			};
			typedef struct muCOSAW32_Keymaps muCOSAW32_Keymaps;

			struct muCOSAW32_WindowHandles {
				// Window class name
				wchar_t wclass_name[2];
				// Hinstance
				HINSTANCE hinstance;
				// Window handle
				HWND hwnd;
				// Device context
				HDC dc;
				// Cursor handle
				HCURSOR hcursor;
				// IMM context handle
				HIMC imc;
			};
			typedef struct muCOSAW32_WindowHandles muCOSAW32_WindowHandles;

			// States possibly held by the window that are updated via proc
			struct muCOSAW32_WindowStates {
				// Closed or not
				muBool closed;
				// Cursor style
				muCursorStyle cursor_style;
			};
			typedef struct muCOSAW32_WindowStates muCOSAW32_WindowStates;

			struct muCOSAW32_WindowProperties {
				// Pixel format
				muPixelFormat format;
				muBool use_format;
				muBool format_set;
				// Default pixel format value
				int pixel_format;

				// Min/Max dimensions
				uint32_m min_width;
				uint32_m min_height;
				uint32_m max_width;
				uint32_m max_height;

				// Scroll level
				int32_m scroll_level;

				// Cursor position
				int32_m cursor_x;
				int32_m cursor_y;

				// Text input
				void (*text_input_callback)(muWindow window, uint8_m* data);
				uint32_m text_cursor_x;
				uint32_m text_cursor_y;
			};
			typedef struct muCOSAW32_WindowProperties muCOSAW32_WindowProperties;

			struct muCOSAW32_WindowTemp {
				// High surrogate for cross-WM_CHAR messages
				WCHAR high_surrogate;
			};
			typedef struct muCOSAW32_WindowTemp muCOSAW32_WindowTemp;

			struct muCOSAW32_Window {
				muCOSAW32_WindowHandles handles;
				muCOSAW32_Keymaps keymaps;
				muCOSAW32_WindowStates states;
				muCOSAW32_WindowProperties props;
				muWindowCallbacks callbacks;
				muCOSAW32_WindowTemp temp;
			};
			typedef struct muCOSAW32_Window muCOSAW32_Window;

		/* Pmap */

			// This entire section describes an atom-locked array of muCOSAW32_Window's.
			// This is needed for when we need to find a muCOSAW32_Window based on just
			// the Win32 handle (HWND), which happens in the proc function. Really sucks
			// that I have to do this, but it works!

			// The way the array works is by using a pointer array that doubles in memory
			// to keep up with allocation every time new memory is needed. Each element
			// in the array is a pointer to a valid muCOSA window or 0; when a window is
			// removed, its slot is set to 0, marking it for overwriting, which will be
			// done if a new window needs to be added and there is an empty slot.

			// The limitation to this is that the array cannot "de-expand"; for the
			// entire time that at least one muCOSA context exists, the amount of windows
			// alllocated can only go up. This is never likely to be a large issue, as
			// the VAST majority of programs will never allocate enough windows at once
			// to ever make this considerable in terms of memory allocated, but it is a
			// limitation nonetheless.

			// Pointer map struct
			struct muCOSAW32_WindowPMAP {
				// Pointer to each window's memory
				muCOSAW32_Window** windows;
				// Amount of windows
				size_m wincount;
				// Amount of window array memory allocated; unit is amount of windows
				size_m winlen;
				// Locked state
				LONG volatile locked;
				// Amount of active muCOSA contexts;
				// used to destroy all memory once all muCOSA contexts deactivate
				size_m access_count;
			};
			typedef struct muCOSAW32_WindowPMAP muCOSAW32_WindowPMAP;

			// Global window pmap
			muCOSAW32_WindowPMAP muCOSAW32_GlobalWindowPMAP = MU_ZERO_STRUCT_CONST(muCOSAW32_WindowPMAP);

			// Locks the window pmap to this thread
			void muCOSAW32_window_pmap_lock(void) {
				// Wait until we have it locked for this thread
				while (InterlockedCompareExchange(&muCOSAW32_GlobalWindowPMAP.locked, 1, 0) == 1) {
					// ...
				}
			}

			// Unlocks the window pmap (make sure you have it locked under the current thread!)
			void muCOSAW32_window_pmap_unlock(void) {
				// Set locked bit to 0 atomically
				_interlockedbittestandreset(&muCOSAW32_GlobalWindowPMAP.locked, 0);
			}

			// Adds a window to the window pmap
			muCOSAResult muCOSAW32_window_pmap_add(muCOSAW32_Window* win) {
				// Lock pmap to this thread
				muCOSAW32_window_pmap_lock();

				// Try and find an empty slot to fill
				for (size_m s = 0; s < muCOSAW32_GlobalWindowPMAP.wincount; ++s) {
					if (!muCOSAW32_GlobalWindowPMAP.windows[s]) {
						// If an empty slot is found, fill it and leave
						muCOSAW32_GlobalWindowPMAP.windows[s] = win;
						muCOSAW32_window_pmap_unlock();
						return MUCOSA_SUCCESS;
					}
				}

				// Increment window count by 1
				++muCOSAW32_GlobalWindowPMAP.wincount;

				// If array is non-existent currently:
				if (!muCOSAW32_GlobalWindowPMAP.windows) {
					// Allocate 1 window worth
					muCOSAW32_GlobalWindowPMAP.winlen = 1;
					muCOSAW32_GlobalWindowPMAP.windows = (muCOSAW32_Window**)mu_malloc(sizeof(muCOSAW32_Window*));
					// If unable to allocate:
					if (!muCOSAW32_GlobalWindowPMAP.windows) {
						// Set values to 0
						muCOSAW32_GlobalWindowPMAP.wincount = 0;
						muCOSAW32_GlobalWindowPMAP.winlen = 0;
						// Unlock pmap
						muCOSAW32_window_pmap_unlock();
						// Return bad value
						return MUCOSA_FAILED_MALLOC;
					}
				}

				// If an array already exists, but we don't have enough memory allocated
				else if (muCOSAW32_GlobalWindowPMAP.wincount >= muCOSAW32_GlobalWindowPMAP.winlen) {
					// Multiply window length by 2
					muCOSAW32_GlobalWindowPMAP.winlen *= 2;
					// Reallocate this new length
					muCOSAW32_Window** new_windows = (muCOSAW32_Window**)mu_realloc(
						muCOSAW32_GlobalWindowPMAP.windows,
						sizeof(muCOSAW32_Window*) * muCOSAW32_GlobalWindowPMAP.winlen
					);

					// If we failed to reallocate:
					if (!new_windows) {
						// Deincrement window count
						--muCOSAW32_GlobalWindowPMAP.wincount;
						// Undo winlen multiplication
						muCOSAW32_GlobalWindowPMAP.winlen /= 2;
						// Unlock pmap
						muCOSAW32_window_pmap_unlock();
						// Return bad value
						return MUCOSA_FAILED_REALLOC;
					}

					// If we didn't fail, we point to the newly allocated memory
					muCOSAW32_GlobalWindowPMAP.windows = new_windows;
				}

				// Set newly allocated window spot to the new window
				muCOSAW32_GlobalWindowPMAP.windows[muCOSAW32_GlobalWindowPMAP.wincount-1] = win;

				// Unlock pmap
				muCOSAW32_window_pmap_unlock();
				// Return success
				return MUCOSA_SUCCESS;
			}

			// Removes a window from the pmap
			void muCOSAW32_window_pmap_remove(muCOSAW32_Window* win) {
				// Lock pmap to this thread
				muCOSAW32_window_pmap_lock();

				// Find window pointer within pmap
				for (size_m p = 0; p < muCOSAW32_GlobalWindowPMAP.wincount; ++p) {
					if (muCOSAW32_GlobalWindowPMAP.windows[p] == win) {
						muCOSAW32_GlobalWindowPMAP.windows[p] = 0;
						break;
					}
				}

				// Unlock pmap
				muCOSAW32_window_pmap_unlock();
			}

			// Cleans up all of the memory within the pmap
			// Note that this does NOT lock/unlock; this should only be called with
			// a thread that already has the global pmap locked
			void muCOSAW32_window_pmap_cleanup(void) {
				// Free the memory if it's allocated
				if (muCOSAW32_GlobalWindowPMAP.windows) {
					mu_free(muCOSAW32_GlobalWindowPMAP.windows);
				}
				// Zero-out the global pmap struct
				muCOSAW32_GlobalWindowPMAP = MU_ZERO_STRUCT(muCOSAW32_WindowPMAP);
			}

			// "Accesses" the pmap, marking a context as currently active
			void muCOSAW32_window_pmap_access(void) {
				// Lock the pmap
				muCOSAW32_window_pmap_lock();

				// Increment access count
				++muCOSAW32_GlobalWindowPMAP.access_count;

				// Unlock the pmap
				muCOSAW32_window_pmap_unlock();
			}

			// "De-accesses" the pmap, marking a context as no longer active
			void muCOSAW32_window_pmap_deaccess(void) {
				// Lock the pmap
				muCOSAW32_window_pmap_lock();

				// Deincrement access count
				--muCOSAW32_GlobalWindowPMAP.access_count;

				// If access count is 0, no more contexts are active, which
				// means that we should clean everything up
				if (muCOSAW32_GlobalWindowPMAP.access_count == 0) {
					muCOSAW32_window_pmap_cleanup();
				}

				// Unlock the pmap
				muCOSAW32_window_pmap_unlock();
			}

			// Finds a pointer to a muCOSA window based on the HWND handle
			muCOSAW32_Window* muCOSAW32_window_pmap_find(HWND hwnd) {
				// Lock the pmap
				muCOSAW32_window_pmap_lock();

				// Default pointer
				muCOSAW32_Window* wp = 0;

				// Find pointer in array
				for (size_m p = 0; p < muCOSAW32_GlobalWindowPMAP.wincount; ++p) {
					if (muCOSAW32_GlobalWindowPMAP.windows[p] != 0 && 
						muCOSAW32_GlobalWindowPMAP.windows[p]->handles.hwnd == hwnd
					) {
						wp = muCOSAW32_GlobalWindowPMAP.windows[p];
						break;
					}
				}

				// Unlock the pmap
				muCOSAW32_window_pmap_unlock();
				// Return pointer
				return wp;
			}

		/* Proc */

			// Info about a proc message
			struct muCOSAW32_ProcMsg {
				// Proc stuff
				UINT uMsg;
				WPARAM wParam;
				LPARAM lParam;

				// muCOSA window
				muCOSAW32_Window* win;
			};
			typedef struct muCOSAW32_ProcMsg muCOSAW32_ProcMsg;

			// Handling for WM_DESTROY; when a window is closed
			LRESULT CALLBACK muCOSAW32_DESTROY(muCOSAW32_ProcMsg msg) {
				PostQuitMessage(0);
				msg.win->states.closed = MU_TRUE;
				return 0;
			}

			// Separates left/right keys
			WPARAM muCOSAW32_map_lr_keys(WPARAM vk, LPARAM lParam) {
				// Separate scancode and left/right bit
				UINT scancode = (lParam & 0x00ff0000) >> 16;
				int  extended = (lParam & 0x01000000) != 0;

				// Separate into left/right virtual key codes if need be
				switch (vk) {
					default: return vk; break;
					case VK_SHIFT:   return MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX); break;
					case VK_CONTROL: return extended ? VK_RCONTROL : VK_LCONTROL; break;
					case VK_MENU:    return extended ? VK_RMENU    : VK_LMENU; break;
				}
			}

			// Handling for WM_KEYDOWN and WM_KEYUP
			LRESULT CALLBACK muCOSAW32_KEY(muCOSAW32_ProcMsg msg, muBool up) {
				// Handle separation of keys based on left/right
				msg.wParam = muCOSAW32_map_lr_keys(msg.wParam, msg.lParam);
				// Convert Win32 key value to muCOSA key value
				muKeyboardKey key = muCOSAW32_VK_to_muCOSA(msg.wParam);
				// Return if key is unknown or out of range
				if (key == MU_KEYBOARD_UNKNOWN || key >= MU_KEYBOARD_LENGTH) {
					return 0;
				}

				// Set key state
				msg.win->keymaps.keyboard[key] = up;
				// Call keyboard callback
				if (msg.win->callbacks.keyboard) {
					msg.win->callbacks.keyboard(msg.win, key, up);
				}

				// Exit
				return 0;
			}

			// Handling for WM_LBUTTONDOWN, WM_RBUTTONDOWN, WM_LBUTTONUP, and WM_RBUTTONDOWN
			LRESULT CALLBACK muCOSAW32_MBUTTON(muCOSAW32_ProcMsg msg, muMouseKey key, muBool up) {
				// Update keymap
				msg.win->keymaps.mouse[key] = up;
				// + Callback
				if (msg.win->callbacks.mouse_key) {
					msg.win->callbacks.mouse_key(msg.win, key, up);
				}
				return 0;
			}

			muCOSAResult muCOSAW32_window_get_dimensions(muCOSAW32_Window* win, uint32_m* data);
			muCOSAResult muCOSAW32_window_get_cursor_pos(muCOSAW32_Window* win, int32_m* data);
			// Handling for WM_SETCURSOR
			LRESULT CALLBACK muCOSAW32_SETCURSOR(muCOSAW32_ProcMsg msg) {
				// Only allow the cursor to change if it's outside the window surface
				// This is a hack and a half, but largely works
				int32_m cur[2]; muCOSAW32_window_get_cursor_pos(msg.win, cur);
				uint32_m dim[2]; muCOSAW32_window_get_dimensions(msg.win, dim);
				int32_m idim[2] = { (int32_m)dim[0], (int32_m)dim[1] };
				if (cur[0] >= 0 && cur[1] >= 0 && cur[0] < idim[0] && cur[1] < idim[1]) {
					SetCursor(msg.win->handles.hcursor);
				} else {
					return DefWindowProcW(msg.win->handles.hwnd, msg.uMsg, msg.wParam, msg.lParam);
				}
				return 0;
			}

			// Handling for WM_GETMINMAXINFO
			LRESULT CALLBACK muCOSAW32_GETMINMAXINFO(muCOSAW32_ProcMsg msg) {
				LPMINMAXINFO lp = (LPMINMAXINFO)msg.lParam;
				// I have no idea why 16 and 39 need to be added here. Can't be borders
				// because these numbers don't even nearly match the border values.
				// If it works, it works :P
				lp->ptMinTrackSize.x = msg.win->props.min_width + 16;
				lp->ptMinTrackSize.y = msg.win->props.min_height + 39;
				lp->ptMaxTrackSize.x = msg.win->props.max_width + 16;
				lp->ptMaxTrackSize.y = msg.win->props.max_height + 39;
				return 0;
			}

			// Handling for WM_MOUSEWHEEL
			LRESULT CALLBACK muCOSAW32_MOUSEWHEEL(muCOSAW32_ProcMsg msg) {
				// Add scroll level from wParam
				int32_m add = GET_WHEEL_DELTA_WPARAM(msg.wParam);
				msg.win->props.scroll_level += add;
				// + Callback
				if (msg.win->callbacks.scroll) {
					msg.win->callbacks.scroll(msg.win, add);
				}

				return 0;
			}

			// Handling for WM_SIZE
			LRESULT CALLBACK muCOSAW32_SIZE(muCOSAW32_ProcMsg msg) {
				// Call dimensions callback
				if (msg.win->callbacks.dimensions) {
					msg.win->callbacks.dimensions(msg.win,
						(uint32_m)(LOWORD(msg.lParam)),
						(uint32_m)(HIWORD(msg.lParam))
					);
				}

				// Not sure why we're calling this
				// I think I vaguely remember this fixing some issue, but idk
				PostMessage(msg.win->handles.hwnd, WM_PAINT, 0, 0);
				return 0;
			}

			// Handling for WM_MOVE
			LRESULT CALLBACK muCOSAW32_MOVE(muCOSAW32_ProcMsg msg) {
				// Call position callback
				if (msg.win->callbacks.position) {
					// https://www.autohotkey.com/boards/viewtopic.php?t=27857
					// I would KISS Bill on the mouth if I ever met him...
					msg.win->callbacks.position(
						msg.win,
						(int32_m)( msg.lParam      & 0x8000 ? - ((~msg.lParam    ) & 0x7FFF)+1 : msg.lParam       & 0x7FFF),
						(int32_m)((msg.lParam>>16) & 0x8000 ? - ((~msg.lParam>>16) & 0X7FFF)+1 : (msg.lParam>>16) & 0x7FFF)
					);
				}

				return 0;
			}

			uint8_m muCOSAW32_UTF8_codepoint_size(uint32_m codepoint) {
				// Storable in 7 bits = 1 byte
				if (codepoint < 128) {
					return 1;
				}
				// Storable in 11 bits = 2 bytes
				if (codepoint < 2048) {
					return 2;
				}
				// Storable in 16 bits = 3 bytes
				if (codepoint < 65536) {
					return 3;
				}
				return 4;
			}

			// Handling for WM_CHAR
			LRESULT CALLBACK muCOSAW32_CHAR(muCOSAW32_ProcMsg msg) {
				// Hold onto high surrogate if it is one
				if (IS_HIGH_SURROGATE(msg.wParam)) {
					msg.win->temp.high_surrogate = (WCHAR)msg.wParam;
					return 0;
				}

				// Return if we aren't taking text input
				if (!msg.win->props.text_input_callback) {
					return DefWindowProcW(msg.win->handles.hwnd, msg.uMsg, msg.wParam, msg.lParam);
				}

				// Convert UTF-16 wParam to wchar_t* UTF-16 string
				WCHAR wstr[3];
				// - Surrogate pair
				if (msg.win->temp.high_surrogate) {
					wstr[0] = msg.win->temp.high_surrogate;
					wstr[1] = (WCHAR)msg.wParam;
					wstr[2] = 0;
					// (Reset high surrogate)
					msg.win->temp.high_surrogate = 0;
				}
				// - Non-surrogate pair
				else {
					wstr[0] = (WCHAR)msg.wParam;
					wstr[1] = 0;
				}

				// Convert wchar_t* UTF-16 string to UTF-8 string
				uint8_m buf[5];
				if (!WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wstr, -1, (LPSTR)buf, sizeof(buf), NULL, NULL)) {
					return 0;
				}

				// Callback
				msg.win->props.text_input_callback(msg.win, buf);
				return 0;
			}

			// Handling for WM_IME_STARTCOMPOSITION
			LRESULT CALLBACK muCOSAW32_IME_STARTCOMPOSITION(muCOSAW32_ProcMsg msg) {
				// Get IMM context handle
				HIMC imc = ImmGetContext(msg.win->handles.hwnd);

				// Fill out composition form
				COMPOSITIONFORM cf = MU_ZERO_STRUCT(COMPOSITIONFORM);
				cf.dwStyle = CFS_FORCE_POSITION;
				cf.ptCurrentPos.x = msg.win->props.text_cursor_x;
				cf.ptCurrentPos.y = msg.win->props.text_cursor_y;

				// Send composition form
				if (!ImmSetCompositionWindow(imc, &cf)) {
					return 0;
				}

				// Release IMM context handle
				ImmReleaseContext(msg.win->handles.hwnd, imc);
				return 0;
			}

			// Handles a proc message
			LRESULT muCOSAW32_procmsg(muCOSAW32_ProcMsg msg) {
				// Do things based on the message code
				switch (msg.uMsg) {
					// Message code we're not processing; return default handling at end of switch
					default: break;
					// Window closing
					case WM_DESTROY: return muCOSAW32_DESTROY(msg); break;
					// Key down
					case WM_KEYDOWN: return muCOSAW32_KEY(msg, MU_TRUE); break;
					// Key up
					case WM_KEYUP: return muCOSAW32_KEY(msg, MU_FALSE); break;
					// Left mouse up
					case WM_LBUTTONUP: return muCOSAW32_MBUTTON(msg, MU_MOUSE_LEFT, MU_FALSE); break;
					// Left mouse down
					case WM_LBUTTONDOWN: return muCOSAW32_MBUTTON(msg, MU_MOUSE_LEFT, MU_TRUE); break;
					// Right mouse up
					case WM_RBUTTONUP: return muCOSAW32_MBUTTON(msg, MU_MOUSE_RIGHT, MU_FALSE); break;
					// Right mouse down
					case WM_RBUTTONDOWN: return muCOSAW32_MBUTTON(msg, MU_MOUSE_RIGHT, MU_TRUE); break;
					// Middle mouse up
					case WM_MBUTTONUP: return muCOSAW32_MBUTTON(msg, MU_MOUSE_MIDDLE, MU_FALSE); break;
					// Middle mouse down
					case WM_MBUTTONDOWN: return muCOSAW32_MBUTTON(msg, MU_MOUSE_MIDDLE, MU_TRUE); break;
					// Cursor style changing
					case WM_SETCURSOR: return muCOSAW32_SETCURSOR(msg); break;
					// Windows asking for min/max dimensions
					case WM_GETMINMAXINFO: return muCOSAW32_GETMINMAXINFO(msg); break;
					// Scrolling
					case WM_MOUSEWHEEL: return muCOSAW32_MOUSEWHEEL(msg); break;
					// Resizing
					case WM_SIZE: return muCOSAW32_SIZE(msg); break;
					// Movement
					case WM_MOVE: return muCOSAW32_MOVE(msg); break;
					// Character input
					case WM_CHAR: return muCOSAW32_CHAR(msg); break;
					// IME composition position
					case WM_IME_STARTCOMPOSITION: return muCOSAW32_IME_STARTCOMPOSITION(msg); break;
				}

				// Default handling
				return DefWindowProcW(msg.win->handles.hwnd, msg.uMsg, msg.wParam, msg.lParam);
			}

			// Proc function for Win32
			LRESULT CALLBACK muCOSAW32_winproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
				// Return default if hwnd is NULL
				if (!hwnd) {
					return DefWindowProcW(hwnd, uMsg, wParam, lParam);
				}

				// Start getting info about the proc
				muCOSAW32_ProcMsg msg;

				// Find window based on HWND
				msg.win = muCOSAW32_window_pmap_find(hwnd);
				if (!msg.win) {
					return DefWindowProcW(hwnd, uMsg, wParam, lParam);
				}

				// Copy over other info
				msg.uMsg = uMsg;
				msg.wParam = wParam;
				msg.lParam = lParam;

				// Return proc message handling
				return muCOSAW32_procmsg(msg);
			}

		/* Creation / Destruction */

			muCOSAResult muCOSAW32_window_set_position(muCOSAW32_Window* win, int32_m* data);

			muCOSAResult muCOSAW32_window_create(muWindowInfo* info, muCOSAW32_Window* win) {
				/* Default attributes */

					// Zero-ing-out
					mu_memset(&win->handles, 0, sizeof(win->handles));
					mu_memset(&win->keymaps, 0, sizeof(win->keymaps));

					// Closed
					win->states.closed = MU_FALSE;

					// Cursor style
					win->states.cursor_style = MU_CURSOR_ARROW;

					// Pixel format
					if (info->pixel_format) {
						win->props.use_format = MU_TRUE;
						win->props.format = *info->pixel_format;
					} else {
						win->props.use_format = MU_FALSE;
					}
					win->props.format_set = MU_FALSE;

					// Min/Max dimensions
					win->props.min_width = info->min_width;
					win->props.min_height = info->min_height;
					win->props.max_width = info->max_width;
					win->props.max_height = info->max_height;
					// - (Fix 0 defaults)
					if (win->props.min_width == 0) {
						win->props.min_width = 120;
					}
					if (win->props.min_height == 0) {
						win->props.min_height = 1;
					}
					if (win->props.max_width == 0) {
						win->props.max_width = 0x0FFFFFFF;
					}
					if (win->props.max_height == 0) {
						win->props.max_height = 0x0FFFFFFF;
					}

					// Scroll level
					win->props.scroll_level = 0;

					// Cursor position
					win->props.cursor_x = win->props.cursor_y = 0;

					// Callbacks
					// - Zero-out if no callbacks specified
					if (!info->callbacks) {
						mu_memset(&win->callbacks, 0, sizeof(win->callbacks));
					}
					// - Set all callbacks if specified
					else {
						win->callbacks = *info->callbacks;
					}

					// Text focus
					win->props.text_input_callback = 0;
					win->props.text_cursor_x = win->props.text_cursor_y = 0;

					// High surrogate for WM_CHAR messages
					win->temp.high_surrogate = 0;

				/* Class */

					// Window title
					wchar_t* wname = muCOSAW32_utf8_to_wchar(info->title);
					if (!wname) {
						return MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR;
					}

					// This is some of the jankiest code I've written!
					// To generate a unique window class, I hamfistedly
					// attempt to generate one by adding the actual pointer
					// value of 'win' to a character, creating an often
					// unreadable class title. This needs to be improved
					// at some point, but works for me :P

					win->handles.wclass_name[0] = (wchar_t)'!';

					wchar_t add; // (Purposely left uninitialized for more randomness)
					if (sizeof(wchar_t) < sizeof(win)) {
						mu_memcpy(&add, &win, sizeof(wchar_t));
					} else {
						mu_memcpy(&add, &win, sizeof(win));
					}

					win->handles.wclass_name[0] += add;
					win->handles.wclass_name[1] = (wchar_t)'\0';

					// Hinstance
					win->handles.hinstance = muCOSAW32_get_hinstance();
					// Hcursor
					win->handles.hcursor = LoadCursor(NULL, IDC_ARROW);

					// Create class struct
					WNDCLASSEXW wclass = MU_ZERO_STRUCT(WNDCLASSEXW);
					wclass.cbSize = sizeof(wclass);
					wclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
					wclass.lpfnWndProc = muCOSAW32_winproc;
					wclass.hInstance = win->handles.hinstance;
					wclass.hCursor = win->handles.hcursor;
					wclass.lpszMenuName = wname;
					wclass.lpszClassName = win->handles.wclass_name;

					// Register
					if(!RegisterClassExW(&wclass)) {
						mu_free(wname);
						return MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS;
					}

				/* Window */

					// Style
					DWORD style = WS_OVERLAPPEDWINDOW;

					// Rect for the window's dimensions
					RECT r;
					r.left = r.top = 0;
					r.right = (LONG)info->width;
					r.bottom = (LONG)info->height;
					AdjustWindowRect(&r, style, FALSE);

					// Create window
					win->handles.hwnd = CreateWindowExW(
						// dwExStyle
						0,
						// lpClassName
						wclass.lpszClassName,
						// lpWindowName
						wclass.lpszMenuName,
						// dwStyle
						style,
						// X, Y,
						(int)info->x, (int)info->y,
						// nWidth, nHeight
						r.right-r.left, r.bottom-r.top,
						// hWndParent
						NULL,
						// hMenu
						NULL,
						// hInstance
						win->handles.hinstance,
						// lpParam
						NULL
					);

					// - Handle fail case
					mu_free(wname);
					if (win->handles.hwnd == NULL) {
						UnregisterClassW(win->handles.wclass_name, win->handles.hinstance);
						return MUCOSA_WIN32_FAILED_CREATE_WINDOW;
					}

				/* Pixel format */

					// Get device context
					win->handles.dc = GetDC(win->handles.hwnd);

					// Handle default pixel format:
					if (!info->pixel_format) {
						// Generic pixel format descriptor
						// Based on https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
						PIXELFORMATDESCRIPTOR pfd = {
							sizeof(PIXELFORMATDESCRIPTOR),
							1,
							PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
							PFD_TYPE_RGBA,
							32,
							0, 0, 0, 0, 0, 0,
							0, 0, 0,
							0, 0, 0, 0,
							24, 8, 8,
							PFD_MAIN_PLANE,
							0,
							0, 0, 0
						};

						// Find pixel format; fallback on 1
						win->props.pixel_format = ChoosePixelFormat(win->handles.dc, &pfd);
						if (!win->props.pixel_format) {
							win->props.pixel_format = 1;
						}
					}

					// Show window
					ShowWindow(win->handles.hwnd, SW_NORMAL);

				/* Auto-disable IME */

					if (ImmAssociateContextEx(win->handles.hwnd, NULL, 0) == FALSE) {
						// ?
					}

				/* Set position manually */

					// A bit of a hack considering that it should work on the first try,
					// but frame extents on Win32 will be frame extents on Win32.

					int32_m pos[2] = { info->x, info->y };
					muCOSAW32_window_set_position(win, pos);

				/* Add window to pmap */

					muCOSAResult res = muCOSAW32_window_pmap_add(win);
					if (muCOSA_result_is_fatal(res)) {
						return res;
					}

				return res;
			}

			void muCOSAW32_window_destroy(muCOSAW32_Window* win) {
				// Release device context
				ReleaseDC(win->handles.hwnd, win->handles.dc);
				// Destroy window
				DestroyWindow(win->handles.hwnd);
				// Unregister window class
				UnregisterClassW(win->handles.wclass_name, win->handles.hinstance);
				// Remove window from pmap
				muCOSAW32_window_pmap_remove(win);
			}

		/* Main loop */

			muBool muCOSAW32_window_get_closed(muCOSAW32_Window* win) {
				return win->states.closed;
			}

			void muCOSAW32_window_close(muCOSAW32_Window* win) {
				// Set closed flag
				win->states.closed = MU_TRUE;
			}

			// Checks all keystates and updates accordingly
			void muCOSAW32_update_keystate(muCOSAW32_Window* win) {
				// Loop through each possible keystate
				for (muKeyboardState s = 1; s < MU_KEYSTATE_LENGTH; ++s) {
					// Assume not on at first
					muBool b = MU_FALSE;
					// Convert keystate to Win32
					int s_w32 = muCOSAW32_keystate_to_W32(s);
					// If not recognized, continue:
					if (s_w32 == VK_NONAME) {
						continue;
					}

					// Set boolean to if it's on or off
					b = (GetKeyState(s_w32) & 0x0001) != 0;
					// Change value if different
					if (b != win->keymaps.keystates[s]) {
						win->keymaps.keystates[s] = b;

						// + Callback
						if (win->callbacks.keystate) {
							win->callbacks.keystate(win, s, b);
						}
					}
				}
			}

			// Handles the cursor changing position every frame;
			// no corresponding Win32 proc for this as far as I'm aware :L
			muCOSAResult muCOSAW32_window_get_cursor_pos(muCOSAW32_Window* win, int32_m* data);
			void muCOSAW32_update_cursor(muCOSAW32_Window* win) {
				// Get cursor position
				int32_m c[2];
				if (muCOSA_result_is_fatal(muCOSAW32_window_get_cursor_pos(win, c))) {
					return;
				}

				// Update cursor position if changed
				if (win->props.cursor_x != c[0] || win->props.cursor_y != c[1]) {
					win->props.cursor_x = c[0];
					win->props.cursor_y = c[1];
					// + Callback
					if (win->callbacks.cursor) {
						win->callbacks.cursor(win, c[0], c[1]);
					}
				}
			}

			void muCOSAW32_window_update(muCOSAW32_Window* win) {
				// Process messages
				MSG msg = MU_ZERO_STRUCT(MSG);
				while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				// Update keystates
				muCOSAW32_update_keystate(win);
				// Update cursor position
				muCOSAW32_update_cursor(win);
			}

		/* Title */

			// Set title
			muCOSAResult muCOSAW32_window_set_title(muCOSAW32_Window* win, char* data) {
				// Convert UTF-8 data to wchar_t* data
				wchar_t* wtitle = muCOSAW32_utf8_to_wchar(data);
				if (!wtitle) {
					return MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR;
				}

				// Set window title
				if (!SetWindowTextW(win->handles.hwnd, wtitle)) {
					mu_free(wtitle);
					return MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB;
				}

				mu_free(wtitle);
				return MUCOSA_SUCCESS;
			}

		/* Frame extents */

			// Default frame extents
			// I love you Bill...
			void muCOSAW32_def_window_frame_extents(uint32_m* data) {
				data[0] = data[1] = data[3] = (uint32_m)GetSystemMetrics(SM_CXSIZEFRAME);
				data[2] = (uint32_m)(GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(92));
			}

			// Get frame extents for a window
			// I LOVE YOU BILL GATES!!!!!
			void muCOSAW32_window_get_frame_extents(muCOSAW32_Window* win, uint32_m* data) {
				// Fallback
				muCOSAW32_def_window_frame_extents(data);

				// Get window and client rect
				RECT wr, cr;
				if (!GetWindowRect(win->handles.hwnd, &wr)) {
					// (Not returning error since default window frame extents are filled)
					return;
				}
				if (!GetClientRect(win->handles.hwnd, &cr)) {
					return;
				}

				// Map client rect to window points
				if (!MapWindowPoints(win->handles.hwnd, NULL, (LPPOINT)&cr, 2)) {
					return;
				}

				// Set values based on window and client rect
				data[0] = (uint32_m)(cr.left-wr.left);
				data[1] = (uint32_m)(wr.right-cr.right);
				data[2] = (uint32_m)(cr.top-wr.top);
				data[3] = (uint32_m)(wr.bottom-cr.bottom);
			}

		/* Dimensions */

			muCOSAResult muCOSAW32_window_get_dimensions(muCOSAW32_Window* win, uint32_m* data) {
				// Get client rect
				RECT r;
				if (!GetClientRect(win->handles.hwnd, &r)) {
					return MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB;
				}

				// Set values
				data[0] = (uint32_m)(r.right-r.left);
				data[1] = (uint32_m)(r.bottom-r.top);
				return MUCOSA_SUCCESS;
			}

			muCOSAResult muCOSAW32_window_set_dimensions(muCOSAW32_Window* win, uint32_m* data) {
				// Get general window info for style
				WINDOWINFO wi;
				if (!GetWindowInfo(win->handles.hwnd, &wi)) {
					return MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB;
				}

				// Calculate rect
				RECT r;
				r.left = r.top = 0;
				r.right = data[0];
				r.bottom = data[1];

				// Calculate appropriate window rect for dimensions
				if (!AdjustWindowRect(&r, wi.dwStyle, FALSE)) {
					return MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB;
				}

				// Set window rect based on this
				if (!SetWindowPos(win->handles.hwnd, HWND_TOP, 0, 0, r.right-r.left, r.bottom-r.top, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE)) {
					return MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB;
				}
				return MUCOSA_SUCCESS;
			}

		/* Position */

			muCOSAResult muCOSAW32_window_get_position(muCOSAW32_Window* win, int32_m* data) {
				// Get window rect
				RECT r;
				if (!GetWindowRect(win->handles.hwnd, &r)) {
					return MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB;
				}

				// Get frame extents
				uint32_m fe[4];
				muCOSAW32_window_get_frame_extents(win, fe);

				// Set position based on rect and relative frame extents
				data[0] = (int32_m)(r.left) + (int32_m)(fe[0]);
				data[1] = (int32_m)(r.top) + (int32_m)(fe[2]);
				return MUCOSA_SUCCESS;
			}

			muCOSAResult muCOSAW32_window_set_position(muCOSAW32_Window* win, int32_m* data) {
				// Get frame extents
				uint32_m fe[4];
				muCOSAW32_window_get_frame_extents(win, fe);

				// Translate x and y based on extents
				int32_m x = data[0] - (int32_m)(fe[0]);
				int32_m y = data[1] - (int32_m)(fe[2]);

				// Set window position
				if (!SetWindowPos(win->handles.hwnd, HWND_TOP, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE)) {
					return MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB;
				}
				return MUCOSA_SUCCESS;
			}

		/* Keymaps */

			muCOSAResult muCOSAW32_window_get_keyboard_map(muCOSAW32_Window* win, muBool** data) {
				// Point to keyboard keymap
				*data = win->keymaps.keyboard;
				return MUCOSA_SUCCESS;
			}

			muCOSAResult muCOSAW32_window_get_keystate_map(muCOSAW32_Window* win, muBool** data) {
				// Point to keystate keymap
				*data = win->keymaps.keystates;
				return MUCOSA_SUCCESS;
			}

			muCOSAResult muCOSAW32_window_get_mouse_map(muCOSAW32_Window* win, muBool** data) {
				// Point to mouse keymap
				*data = win->keymaps.mouse;
				return MUCOSA_SUCCESS;
			}

		/* Cursor */

			muCOSAResult muCOSAW32_window_get_cursor_pos(muCOSAW32_Window* win, int32_m* data) {
				// Get cursor position
				POINT p;
				if (!GetCursorPos(&p)) {
					return MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB;
				}

				// Get window position
				int32_m wpos[2];
				muCOSAResult res = muCOSAW32_window_get_position(win, wpos);
				if (muCOSA_result_is_fatal(res)) {
					return MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB;
				}

				// Retrieve cursor position relative to window
				data[0] = p.x-wpos[0];
				data[1] = p.y-wpos[1];
				return res;
			}

			muCOSAResult muCOSAW32_window_set_cursor_pos(muCOSAW32_Window* win, int32_m* data) {
				// Get window position
				int32_m wpos[2];
				muCOSAResult res = muCOSAW32_window_get_position(win, wpos);
				if (muCOSA_result_is_fatal(res)) {
					return MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB;
				}

				// Set cursor position relative to window
				if (!SetCursorPos(wpos[0]+data[0], wpos[1]+data[1])) {
					return MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB;
				}
				return res;
			}

			muCOSAResult muCOSAW32_window_get_cursor_style(muCOSAW32_Window* win, muCursorStyle* data) {
				// Set data to cursor style
				*data = win->states.cursor_style;
				return MUCOSA_SUCCESS;
			}

			muCOSAResult muCOSAW32_window_set_cursor_style(muCOSAW32_Window* win, muCursorStyle* data) {
				// Set internal storage to cursor style
				win->states.cursor_style = *data;
				// Destroy cursor
				DestroyCursor(win->handles.hcursor);
				// Load cursor
				win->handles.hcursor = LoadCursor(0, (LPCSTR)muCOSAW32_muCOSA_cursor_to_W32(*data));
				// Set cursor
				SetCursor(win->handles.hcursor);
				return MUCOSA_SUCCESS;
			}

		/* Scroll */

			void muCOSAW32_window_get_scroll(muCOSAW32_Window* win, int32_m* data) {
				// Give scroll level
				*data = win->props.scroll_level;
			}

			void muCOSAW32_window_set_scroll(muCOSAW32_Window* win, int32_m* data) {
				// Overwrite scroll level
				win->props.scroll_level = *data;
			}

		/* Callbacks */

			void muCOSAW32_window_set_callback(muCOSAW32_Window* win, muWindowAttrib attrib, void* fun) {
				switch (attrib) {
					case MU_WINDOW_DIMENSIONS_CALLBACK: {
						mu_memcpy(&win->callbacks.dimensions, fun, sizeof(win->callbacks.dimensions));
					} break;
					case MU_WINDOW_POSITION_CALLBACK: {
						mu_memcpy(&win->callbacks.position, fun, sizeof(win->callbacks.position));
					} break;
					case MU_WINDOW_KEYBOARD_CALLBACK: {
						mu_memcpy(&win->callbacks.keyboard, fun, sizeof(win->callbacks.keyboard));
					} break;
					case MU_WINDOW_KEYSTATE_CALLBACK: {
						mu_memcpy(&win->callbacks.keystate, fun, sizeof(win->callbacks.keystate));
					} break;
					case MU_WINDOW_MOUSE_KEY_CALLBACK: {
						mu_memcpy(&win->callbacks.mouse_key, fun, sizeof(win->callbacks.mouse_key));
					} break;
					case MU_WINDOW_CURSOR_CALLBACK: {
						mu_memcpy(&win->callbacks.cursor, fun, sizeof(win->callbacks.cursor));
					} break;
					case MU_WINDOW_SCROLL_CALLBACK: {
						mu_memcpy(&win->callbacks.scroll, fun, sizeof(win->callbacks.scroll));
					} break;
				}
			}

		/* Text input */

			void muCOSAW32_window_update_text_cursor(muCOSAResult* result, muCOSAW32_Window* win, uint32_m cx, uint32_m cy);
			void muCOSAW32_window_get_text_input(muCOSAResult* result, muCOSAW32_Window* win, uint32_m cx, uint32_m cy, void (*callback)(muWindow, uint8_m*)) {
				// Associate IMM context with this window
				if (!ImmAssociateContextEx(win->handles.hwnd, NULL, IACE_DEFAULT)) {
					MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_ASSOCIATE_IMM)
					return;
				}

				// Set callback
				win->props.text_input_callback = callback;
				// Update text cursor position
				muCOSAW32_window_update_text_cursor(0, win, cx, cy);
			}

			void muCOSAW32_window_let_text_input(muCOSAW32_Window* win) {
				// Set callback to nothing
				win->props.text_input_callback = 0;
				// Deassociate IMM context
				ImmAssociateContextEx(win->handles.hwnd, NULL, 0);
			}

			void muCOSAW32_window_update_text_cursor(muCOSAResult* result, muCOSAW32_Window* win, uint32_m cx, uint32_m cy) {
				// Do nothing if text input isn't being taken
				if (!win->props.text_input_callback) {
					return;
				}

				// Might not be a good idea to keep grabbing an releasing IMM context,
				// but the entire IMM is so fragile that the smallest change makes it
				// not function at all. So, I'm keeping it like this :L

				// Get IMM context handle
				win->handles.imc = ImmGetContext(win->handles.hwnd);

				// Fill out composition form
				COMPOSITIONFORM cf = MU_ZERO_STRUCT(COMPOSITIONFORM);
				cf.dwStyle = CFS_FORCE_POSITION;
				cf.ptCurrentPos.x = cx;
				cf.ptCurrentPos.y = cy;

				// Send composition form
				if (!ImmSetCompositionWindow(win->handles.imc, &cf)) {
					MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION)
				}

				// Release IMM context handle
				ImmReleaseContext(win->handles.hwnd, win->handles.imc);
			}

		/* OpenGL */

		#ifdef MU_SUPPORT_OPENGL

			struct muCOSAW32_GL {
				HDC dc;
				HGLRC hgl;
			};
			typedef struct muCOSAW32_GL muCOSAW32_GL;

			void* muCOSAW32_gl_context_create(muCOSAW32_Context* context, muCOSAResult* result, muCOSAW32_Window* win, muGraphicsAPI api) {
				// Allocate memory for context
				muCOSAW32_GL* gl = (muCOSAW32_GL*)mu_malloc(sizeof(muCOSAW32_GL));
				if (!gl) {
					MU_SET_RESULT(result, MUCOSA_FAILED_MALLOC)
					return 0;
				}
				gl->dc = win->handles.dc;

				// Create OpenGL context
				muPixelFormat* format = 0;
				if (win->props.use_format) {
					format = &win->props.format;
				}
				muCOSAResult res = muCOSAW32_create_opengl_context(win->handles.dc, win->props.pixel_format, &context->wgl, format, &gl->hgl, api, &win->props.format_set);
				if (res != MUCOSA_SUCCESS) {
					MU_SET_RESULT(result, res)
					if (muCOSA_result_is_fatal(res)) {
						mu_free(gl);
						gl = 0;
					}
				}
				return gl;
			}

			void muCOSAW32_gl_context_destroy(muCOSAW32_GL* gl) {
				wglDeleteContext(gl->hgl);
				mu_free(gl);
			}

			muCOSAResult muCOSAW32_gl_bind(muCOSAW32_GL* gl) {
				if (!wglMakeCurrent(gl->dc, gl->hgl)) {
					return MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT;
				}
				return MUCOSA_SUCCESS;
			}

			muCOSAResult muCOSAW32_gl_swap_buffers(muCOSAW32_Window* win) {
				if (!SwapBuffers(win->handles.dc)) {
					return MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS;
				}
				return MUCOSA_SUCCESS;
			}

			void* muCOSAW32_gl_get_proc_address(const char* name) {
				// Get proc address based on name
				PROC p = (PROC)wglGetProcAddress(name);

				// If failed:
				if (p == 0
					|| (p == (PROC)1)
					|| (p == (PROC)2)
					|| (p == (PROC)3)
					|| (p == (PROC)-1)
				) {
					// Try loading from DLL itself
					HMODULE module = LoadLibraryA("opengl32.dll");
					p = (PROC)GetProcAddress(module, name);
				}

				// Convert to void* in a way that avoids warnings
				void* vptr = 0;
				mu_memcpy(&vptr, &p, sizeof(void*));
				return vptr;
			}

			muBool muCOSAW32_gl_swap_interval(muCOSAW32_Context* context, muCOSAResult* result, int interval) {
				// If the swap interval function was not found when loading, quit
				if (!context->wgl.SwapInterval) {
					MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION)
					return 0;
				}

				// Return a call to it if we've found it
				return context->wgl.SwapInterval(interval);
			}

		#endif /* MU_SUPPORT_OPENGL */

	#endif /* MUCOSA_WIN32 */

	/* Inner */

		// Inner struct
		struct muCOSA_Inner {
			muWindowSystem system;
			void* context;
		};
		typedef struct muCOSA_Inner muCOSA_Inner;

		// Determines the best window system
		muWindowSystem muCOSA_best_window_system(void) {
			#ifdef MUCOSA_WIN32
				return MU_WINDOW_WIN32;
			#else
				return 0;
			#endif
		}

		// Creates a valid inner struct based on the requested system
		muCOSAResult muCOSA_inner_create(muCOSA_Inner* inner, muWindowSystem system) {
			// Find best window system
			if (system == 0) {
				system = muCOSA_best_window_system();
			}
			inner->system = system;

			// Create context based on system
			switch (system) {
				// Unknown/Unsupported/None supported:
				default: return MUCOSA_FAILED_NULL_WINDOW_SYSTEM; break;

				// Win32
				MUCOSA_WIN32_CALL(
					case MU_WINDOW_WIN32: {
						// Allocate context
						inner->context = mu_malloc(sizeof(muCOSAW32_Context));
						if (!inner->context) {
							return MUCOSA_FAILED_MALLOC;
						}

						// Create context
						muCOSAResult res = muCOSAW32_context_init((muCOSAW32_Context*)inner->context);
						if (muCOSA_result_is_fatal(res)) {
							mu_free(inner->context);
						}
						return res;
					} break;
				)
			}
		}

		// Destroys inner struct based on system
		void muCOSA_inner_destroy(muCOSA_Inner* inner) {
			switch (inner->system) {
				default: break;

				// Win32
				MUCOSA_WIN32_CALL(
					case MU_WINDOW_WIN32: {
						// Destroy context
						muCOSAW32_context_term((muCOSAW32_Context*)inner->context);
					} break;
				)
			}
		}

	// Name call macro
	#ifdef MUCOSA_NAMES
		#define MUCOSA_NAME_CALL(...) __VA_ARGS__
	#else
		#define MUCOSA_NAME_CALL(...)
	#endif

	/* Context */

		MUDEF void muCOSA_context_create(muCOSAContext* context, muWindowSystem system, muBool set_context) {
			// Allocate context
			context->inner = mu_malloc(sizeof(muCOSA_Inner));
			if (!context->inner) {
				context->result = MUCOSA_FAILED_MALLOC;
				return;
			}

			// Create context
			context->result = muCOSA_inner_create((muCOSA_Inner*)context->inner, system);
			if (muCOSA_result_is_fatal(context->result)) {
				mu_free(context->inner);
				return;
			}

			// Set context if necessary
			if (set_context) {
				muCOSA_context_set(context);
			}
		}

		MUDEF void muCOSA_context_destroy(muCOSAContext* context) {
			// Free and destroy if inner contents exist
			if (context->inner) {
				muCOSA_inner_destroy((muCOSA_Inner*)context->inner);
				mu_free(context->inner);
			}
		}

		// Global context
		muCOSAContext* muCOSA_global_context = 0;

		// Set context
		MUDEF void muCOSA_context_set(muCOSAContext* context) {
			muCOSA_global_context = context;
		}

		// Get window system
		MUDEF muWindowSystem muCOSA_context_get_window_system(muCOSAContext* context) {
			return ((muCOSA_Inner*)context->inner)->system;
		}

	/* Window */

		/* Creation / Destruction */

			MUDEF muWindow muCOSA_window_create(muCOSAContext* context, muCOSAResult* result, muWindowInfo* info) {
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do thing based on window system
				switch (inner->system) {
					default: return 0; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32 : {
						// Allocate window memory
						muCOSAW32_Window* win = (muCOSAW32_Window*)mu_malloc(sizeof(muCOSAW32_Window));
						if (!win) {
							MU_SET_RESULT(result, MUCOSA_FAILED_MALLOC)
							return 0;
						}

						// Create window
						muCOSAResult res = muCOSAW32_window_create(info, win);
						if (res != MUCOSA_SUCCESS) {
							MU_SET_RESULT(result, res)
							if (muCOSA_result_is_fatal(res)) {
								mu_free(win);
								MU_SET_RESULT(result, res)
								return 0;
							}
						}
						return win;
					} break;)
				}

				// To avoid unused parameter warnings in some cases
				if (result) {} if (info) {}
			}

			MUDEF muWindow muCOSA_window_destroy(muCOSAContext* context, muWindow win) {
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do things based on window system
				switch (inner->system) {
					default: return 0; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
						// Destroy window
						muCOSAW32_window_destroy((muCOSAW32_Window*)win);
						// Free memory
						mu_free(win);
						return 0;
					} break;)
				}

				// To avoid unused parameter warnings in some cases
				if (win) {}
			}

		/* Main loop */

			MUDEF muBool muCOSA_window_get_closed(muCOSAContext* context, muCOSAResult* result, muWindow win) {
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do things based on window system
				switch (inner->system) {
					default: return 0; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
						return muCOSAW32_window_get_closed((muCOSAW32_Window*)win);
					} break;)
				}

				// To avoid unused parameter warnings in some cases
				if (result) {} if (win) {}
			}

			MUDEF void muCOSA_window_close(muCOSAContext* context, muWindow win) {
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do things based on window system
				switch (inner->system) {
					default: return; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
						muCOSAW32_window_close((muCOSAW32_Window*)win);
						return;
					} break;)
				}

				// To avoid unused parameter warnings in some cases
				if (win) {}
			}

			MUDEF void muCOSA_window_update(muCOSAContext* context, muCOSAResult* result, muWindow win) {
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do things based on window system
				switch (inner->system) {
					default: return; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
						muCOSAW32_window_update((muCOSAW32_Window*)win);
						return;
					} break;)
				}

				// To avoid unused parameter warnings in some cases
				if (result) {} if (win) {}
			}

		/* Get / Set */

			MUDEF void muCOSA_window_get(muCOSAContext* context, muCOSAResult* result, muWindow win, muWindowAttrib attrib, void* data) {
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do things based on window system
				switch (inner->system) {
					default: return; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
						muCOSAResult res = MUCOSA_SUCCESS;
						muCOSAW32_Window* w32_win = (muCOSAW32_Window*)win;

						// Do things based on attribute
						switch (attrib) {
							default: MU_SET_RESULT(result, MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB) return; break;

							// Dimensions
							case MU_WINDOW_DIMENSIONS: res = muCOSAW32_window_get_dimensions(w32_win, (uint32_m*)data); break;
							// Position
							case MU_WINDOW_POSITION: res = muCOSAW32_window_get_position(w32_win, (int32_m*)data); break;
							// Keyboard keymap
							case MU_WINDOW_KEYBOARD_MAP: res = muCOSAW32_window_get_keyboard_map(w32_win, (muBool**)data); break;
							// Keystate keymap
							case MU_WINDOW_KEYSTATE_MAP: res = muCOSAW32_window_get_keystate_map(w32_win, (muBool**)data); break;
							// Mouse keymap
							case MU_WINDOW_MOUSE_MAP: res = muCOSAW32_window_get_mouse_map(w32_win, (muBool**)data); break;
							// Scroll
							case MU_WINDOW_SCROLL_LEVEL: muCOSAW32_window_get_scroll(w32_win, (int32_m*)data); return; break;
							// Cursor
							case MU_WINDOW_CURSOR: res = muCOSAW32_window_get_cursor_pos(w32_win, (int32_m*)data); break;
							// Cursor style
							case MU_WINDOW_CURSOR_STYLE: res = muCOSAW32_window_get_cursor_style(w32_win, (muCursorStyle*)data); break;
						}

						if (res != MUCOSA_SUCCESS) {
							MU_SET_RESULT(result, res)
						}
						return;
					} break;)
				}

				// To avoid unused parameter warnings in some cases
				if (result) {} if (win) {} if (attrib) {} if (data) {}
			}

			MUDEF void muCOSA_window_set(muCOSAContext* context, muCOSAResult* result, muWindow win, muWindowAttrib attrib, void* data) {
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do things based on window system
				switch (inner->system) {
					default: return; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
						muCOSAResult res = MUCOSA_SUCCESS;
						muCOSAW32_Window* w32_win = (muCOSAW32_Window*)win;

						// Do things based on attribute
						switch (attrib) {
							default: MU_SET_RESULT(result, MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB) return; break;

							// Title
							case MU_WINDOW_TITLE: res = muCOSAW32_window_set_title(w32_win, (char*)data); break;
							// Dimensions
							case MU_WINDOW_DIMENSIONS: res = muCOSAW32_window_set_dimensions(w32_win, (uint32_m*)data); break;
							// Position
							case MU_WINDOW_POSITION: res = muCOSAW32_window_set_position(w32_win, (int32_m*)data); break;
							// Scroll
							case MU_WINDOW_SCROLL_LEVEL: muCOSAW32_window_set_scroll(w32_win, (int32_m*)data); return; break;
							// Cursor
							case MU_WINDOW_CURSOR: res = muCOSAW32_window_set_cursor_pos(w32_win, (int32_m*)data); break;
							// Cursor style
							case MU_WINDOW_CURSOR_STYLE: res = muCOSAW32_window_set_cursor_style(w32_win, (muCursorStyle*)data); break;
							// Callbacks
							case MU_WINDOW_DIMENSIONS_CALLBACK: case MU_WINDOW_POSITION_CALLBACK:
							case MU_WINDOW_KEYBOARD_CALLBACK: case MU_WINDOW_KEYSTATE_CALLBACK:
							case MU_WINDOW_MOUSE_KEY_CALLBACK: case MU_WINDOW_CURSOR_CALLBACK:
							case MU_WINDOW_SCROLL_CALLBACK: muCOSAW32_window_set_callback(w32_win, attrib, data); return; break;
						}

						if (res != MUCOSA_SUCCESS) {
							MU_SET_RESULT(result, res)
						}
						return;
					} break;)
				}

				// To avoid unused parameter warnings in some cases
				if (result) {} if (win) {} if (attrib) {} if (data) {}
			}

		/* Text input */

			MUDEF void muCOSA_window_get_text_input(muCOSAContext* context, muCOSAResult* result, muWindow win, uint32_m text_cursor_x, uint32_m text_cursor_y, void (*callback)(muWindow window, uint8_m* data)) {
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do things based on window system
				switch (inner->system) {
					default: return; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
						muCOSAW32_window_get_text_input(result, (muCOSAW32_Window*)win, text_cursor_x, text_cursor_y, callback);
						return;
					} break;)
				}

				// To avoid unused parameter warnings in some cases
				if (result) {} if (win) {} if (text_cursor_x) {} if (text_cursor_y) {} if (callback) {}
			}

			MUDEF void muCOSA_window_let_text_input(muCOSAContext* context, muWindow win) {
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do things based on window system
				switch (inner->system) {
					default: return; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
						muCOSAW32_window_let_text_input((muCOSAW32_Window*)win);
						return;
					} break;)
				}

				// To avoid unused parameter warnings in some cases
				if (win) {}
			}

			MUDEF void muCOSA_window_update_text_cursor(muCOSAContext* context, muCOSAResult* result, muWindow win, uint32_m x, uint32_m y) {
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do things based on window system
				switch (inner->system) {
					default: return; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
						muCOSAW32_window_update_text_cursor(result, (muCOSAW32_Window*)win, x, y);
						return;
					} break;)
				}

				// To avoid unused parameter warnings in some cases
				if (result) {} if (win) {} if (x) {} if (y) {}
			}

		/* OpenGL */

			MUDEF muGLContext muCOSA_gl_context_create(muCOSAContext* context, muCOSAResult* result, muWindow win, muGraphicsAPI api) {
				#ifdef MU_SUPPORT_OPENGL
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do things based on window system
				switch (inner->system) {
					default: return 0; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
						return muCOSAW32_gl_context_create(
							(muCOSAW32_Context*)inner->context, result,
							(muCOSAW32_Window*)win, api
						);
					} break;)
				}

				// To avoid parameter warnings in certain circumstances
				if (result) {} if (win) {} if (api) {}

				// Fallback for non-OpenGL support:
				#else
				MU_SET_RESULT(result, MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API)
				return 0;
				if (context) {} if (result) {} if (win) {} if (api) {}
				#endif
			}

			MUDEF muGLContext muCOSA_gl_context_destroy(muCOSAContext* context, muWindow win, muGLContext gl_context) {
				#ifdef MU_SUPPORT_OPENGL
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do things based on window system
				switch (inner->system) {
					default: return 0; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
						muCOSAW32_gl_context_destroy((muCOSAW32_GL*)gl_context);
						return 0;
					} break;)
				}

				// To avoid parameter warnings in certain circumstances
				if (win) {} if (gl_context) {}

				// Fallback for non-OpenGL support:
				#else
				return 0;
				if (context) {} if (win) {} if (gl_context) {}
				#endif
			}

			MUDEF void muCOSA_gl_bind(muCOSAContext* context, muCOSAResult* result, muWindow win, muGLContext gl_context) {
				#ifdef MU_SUPPORT_OPENGL
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do things based on window system
				switch (inner->system) {
					default: return; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
						muCOSAResult res = muCOSAW32_gl_bind((muCOSAW32_GL*)gl_context);
						if (res != MUCOSA_SUCCESS) {
							MU_SET_RESULT(result, res)
						}
						return;
					} break;)
				}

				// To avoid parameter warnings in certain circumstances
				if (result) {} if (win) {} if (gl_context) {}

				// Fallback for non-OpenGL support:
				#else
				MU_SET_RESULT(result, MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API)
				return;
				if (context) {} if (result) {} if (win) {} if (gl_context) {}
				#endif
			}

			MUDEF void muCOSA_gl_swap_buffers(muCOSAContext* context, muCOSAResult* result, muWindow win) {
				#ifdef MU_SUPPORT_OPENGL
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do things based on window system
				switch (inner->system) {
					default: return; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
						muCOSAResult res = muCOSAW32_gl_swap_buffers((muCOSAW32_Window*)win);
						if (res != MUCOSA_SUCCESS) {
							MU_SET_RESULT(result, res)
						}
						return;
					} break;)
				}

				// To avoid parameter warnings in certain circumstances
				if (result) {} if (win) {}

				// Fallback for non-OpenGL support:
				#else
				MU_SET_RESULT(result, MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API)
				return;
				if (context) {} if (result) {} if (win) {}
				#endif
			}

			MUDEF void* muCOSA_gl_get_proc_address(muCOSAContext* context, const char* name) {
				#ifdef MU_SUPPORT_OPENGL
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do things based on window system
				switch (inner->system) {
					default: return 0; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
						return muCOSAW32_gl_get_proc_address(name);
					} break;)
				}

				// To avoid parameter warnings in certain circumstances
				if (name) {}

				// Fallback for non-OpenGL support:
				#else
				return 0;
				if (context) {} if (name) {}
				#endif
			}

			MUDEF muBool muCOSA_gl_swap_interval(muCOSAContext* context, muCOSAResult* result, int interval) {
				#ifdef MU_SUPPORT_OPENGL
				// Get inner from context
				muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

				// Do things based on window system
				switch (inner->system) {
					default: return 0; break;

					// Win32
					MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
						return muCOSAW32_gl_swap_interval((muCOSAW32_Context*)inner->context, result, interval);
					} break;)
				}

				// Fallback for non-OpenGL support
				#else
				MU_SET_RESULT(result, MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API)
				return 0;
				if (context) {} if (result) {} if (interval) {}
				#endif
			}

	/* Time */

		MUDEF double muCOSA_fixed_time_get(muCOSAContext* context) {
			// Get inner from context
			muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

			// Do things based on window system
			switch (inner->system) {
				default: return 0.0; break;

				// Win32
				MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
					return muCOSAW32_fixed_time_get((muCOSAW32_Context*)inner->context);
				} break;)
			}
		}

		MUDEF double muCOSA_time_get(muCOSAContext* context) {
			// Get inner from context
			muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

			// Do things based on window system
			switch (inner->system) {
				default: return 0.0; break;

				// Win32
				MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
					return muCOSAW32_time_get((muCOSAW32_Context*)inner->context);
				} break;)
			}
		}

		MUDEF void muCOSA_time_set(muCOSAContext* context, double time) {
			// Get inner from context
			muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

			// Do things based on window system
			switch (inner->system) {
				default: return; break;

				// Win32
				MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
					muCOSAW32_time_set((muCOSAW32_Context*)inner->context, time);
					return;
				} break;)
			}

			// To avoid parameter warnings in certain cirumstances
			if (time) {}
		}

	/* Sleep */

		MUDEF void muCOSA_sleep(muCOSAContext* context, double time) {
			// Get inner from context
			muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

			// Do things based on window system
			switch (inner->system) {
				default: return; break;

				// Win32
				MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
					muCOSAW32_sleep(time);
					return;
				} break;)
			}

			// To avoid parameter warnings in certain circumstances
			if (time) {}
		}

	/* Clipboard */

		MUDEF uint8_m* muCOSA_clipboard_get(muCOSAContext* context, muCOSAResult* result) {
			// Get inner from context
			muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

			// Do things based on window system
			switch (inner->system) {
				default: return 0; break;

				// Win32
				MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
					return muCOSAW32_clipboard_get(result);
				} break;)
			}

			// To avoid unused parameter warnings in some circumstances
			if (result) {}
		}

		MUDEF void muCOSA_clipboard_set(muCOSAContext* context, muCOSAResult* result, uint8_m* data, size_m datalen) {
			// Get inner from context
			muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

			// Do things based on window system
			switch (inner->system) {
				default: return; break;

				// Win32
				MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
					muCOSAW32_clipboard_set(result, data, datalen);
					return;
				} break;)
			}

			// To avoid unused parameter warnings in some circumstances
			if (result) {} if (data) {} if (datalen) {}
		}

	/* Result */

		MUDEF muBool muCOSA_result_is_fatal(muCOSAResult result) {
			switch (result) {
				// Fatal errors (assumed fatal):
				default: return MU_TRUE; break;

				// All non-fatal errors:
				case MUCOSA_SUCCESS:
				case MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS:
				case MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW:
				case MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT:
				case MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT:
				case MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT:
				case MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT:
				case MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT:
				case MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS:
				case MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT:
				case MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT:
					return MU_FALSE;
				break;
			}
		}

	/* Names */

	MUCOSA_NAME_CALL(
		MUDEF const char* muCOSA_result_get_name(muCOSAResult result) {
			switch (result) {
				default: return "MU_UNKNOWN"; break;

				case MUCOSA_SUCCESS: return "MUCOSA_SUCCESS"; break;
				case MUCOSA_FAILED_NULL_WINDOW_SYSTEM: return "MUCOSA_FAILED_NULL_WINDOW_SYSTEM"; break;
				case MUCOSA_FAILED_MALLOC: return "MUCOSA_FAILED_MALLOC"; break;
				case MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB: return "MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB"; break;
				case MUCOSA_FAILED_REALLOC: return "MUCOSA_FAILED_REALLOC"; break;
				case MUCOSA_FAILED_UNKNOWN_GRAPHICS_API: return "MUCOSA_FAILED_UNKNOWN_GRAPHICS_API"; break;
				case MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API: return "MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API"; break;

				case MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR: return "MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR"; break;
				case MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS: return "MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS"; break;
				case MUCOSA_WIN32_FAILED_CREATE_WINDOW: return "MUCOSA_WIN32_FAILED_CREATE_WINDOW"; break;
				case MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB: return "MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB"; break;
				case MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB: return "MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB"; break;
				case MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS: return "MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS"; break;
				case MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW: return "MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW"; break;
				case MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT"; break;
				case MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT"; break;
				case MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT"; break;
				case MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT: return "MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT"; break;
				case MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT: return "MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT"; break;
				case MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS: return "MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS"; break;
				case MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT"; break;
				case MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT"; break;
				case MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT"; break;
				case MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT"; break;
				case MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT"; break;
				case MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT: return "MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT"; break;
				case MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT: return "MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT"; break;
				case MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS: return "MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS"; break;
				case MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION: return "MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION"; break;
				case MUCOSA_WIN32_FAILED_ASSOCIATE_IMM: return "MUCOSA_WIN32_FAILED_ASSOCIATE_IMM"; break;
				case MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION: return "MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION"; break;
				case MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD: return "MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD"; break;
				case MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA: return "MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA"; break;
				case MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT: return "MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT"; break;
				case MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA: return "MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA"; break;
				case MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA: return "MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA"; break;
			}
		}

		MUDEF const char* mu_window_system_get_name(muWindowSystem system) {
			switch (system) {
				default: return "MU_UNKNOWN"; break;

				case MU_WINDOW_NULL: return "MU_WINDOW_NULL"; break;
				case MU_WINDOW_WIN32: return "MU_WINDOW_WIN32"; break;
			}
		}

		MUDEF const char* mu_window_system_get_nice_name(muWindowSystem system) {
			switch (system) {
				default: return "Unknown"; break;

				case MU_WINDOW_NULL: return "Unknown/Auto"; break;
				case MU_WINDOW_WIN32: return "Win32"; break;
			}
		}

		MUDEF const char* mu_window_attrib_get_name(muWindowAttrib attrib) {
			switch (attrib) {
				default: return "MU_UNKNOWN"; break;
				case MU_WINDOW_TITLE: return "MU_WINDOW_TITLE"; break;
				case MU_WINDOW_DIMENSIONS: return "MU_WINDOW_DIMENSIONS"; break;
				case MU_WINDOW_POSITION: return "MU_WINDOW_POSITION"; break;
				case MU_WINDOW_KEYBOARD_MAP: return "MU_WINDOW_KEYBOARD_MAP"; break;
				case MU_WINDOW_KEYSTATE_MAP: return "MU_WINDOW_KEYSTATE_MAP"; break;
				case MU_WINDOW_MOUSE_MAP: return "MU_WINDOW_MOUSE_MAP"; break;
				case MU_WINDOW_SCROLL_LEVEL: return "MU_WINDOW_SCROLL_LEVEL"; break;
				case MU_WINDOW_CURSOR: return "MU_WINDOW_CURSOR"; break;
				case MU_WINDOW_CURSOR_STYLE: return "MU_WINDOW_CURSOR_STYLE"; break;
				case MU_WINDOW_DIMENSIONS_CALLBACK: return "MU_WINDOW_DIMENSIONS_CALLBACK"; break;
				case MU_WINDOW_POSITION_CALLBACK: return "MU_WINDOW_POSITION_CALLBACK"; break;
				case MU_WINDOW_KEYBOARD_CALLBACK: return "MU_WINDOW_KEYBOARD_CALLBACK"; break;
				case MU_WINDOW_KEYSTATE_CALLBACK: return "MU_WINDOW_KEYSTATE_CALLBACK"; break;
				case MU_WINDOW_MOUSE_KEY_CALLBACK: return "MU_WINDOW_MOUSE_KEY_CALLBACK"; break;
				case MU_WINDOW_CURSOR_CALLBACK: return "MU_WINDOW_CURSOR_CALLBACK"; break;
				case MU_WINDOW_SCROLL_CALLBACK: return "MU_WINDOW_SCROLL_CALLBACK"; break;
			}
		}

		MUDEF const char* mu_window_attrib_get_nice_name(muWindowAttrib attrib) {
			switch (attrib) {
				default: return "Unknown"; break;
				case MU_WINDOW_TITLE: return "Title"; break;
				case MU_WINDOW_DIMENSIONS: return "Dimensions"; break;
				case MU_WINDOW_POSITION: return "Position"; break;
				case MU_WINDOW_KEYBOARD_MAP: return "Keyboard map"; break;
				case MU_WINDOW_KEYSTATE_MAP: return "Keystate map"; break;
				case MU_WINDOW_MOUSE_MAP: return "Mouse map"; break;
				case MU_WINDOW_SCROLL_LEVEL: return "Scroll level"; break;
				case MU_WINDOW_CURSOR: return "Cursor"; break;
				case MU_WINDOW_CURSOR_STYLE: return "Cursor style"; break;
				case MU_WINDOW_DIMENSIONS_CALLBACK: return "Dimensions callback"; break;
				case MU_WINDOW_POSITION_CALLBACK: return "Position callback"; break;
				case MU_WINDOW_KEYBOARD_CALLBACK: return "Keyboard callback"; break;
				case MU_WINDOW_KEYSTATE_CALLBACK: return "Keystate callback"; break;
				case MU_WINDOW_MOUSE_KEY_CALLBACK: return "Mouse key callback"; break;
				case MU_WINDOW_CURSOR_CALLBACK: return "Cursor callback"; break;
				case MU_WINDOW_SCROLL_CALLBACK: return "Scroll callback"; break;
			}
		}

		MUDEF const char* mu_keyboard_key_get_name(muKeyboardKey key) {
			switch (key) {
				default: return "MU_UNKNOWN"; break;
				case MU_KEYBOARD_UNKNOWN: return "MU_KEYBOARD_UNKNOWN"; break;
				case MU_KEYBOARD_BACKSPACE: return "MU_KEYBOARD_BACKSPACE"; break;
				case MU_KEYBOARD_TAB: return "MU_KEYBOARD_TAB"; break;
				case MU_KEYBOARD_CLEAR: return "MU_KEYBOARD_CLEAR"; break;
				case MU_KEYBOARD_RETURN: return "MU_KEYBOARD_RETURN"; break;
				case MU_KEYBOARD_PAUSE: return "MU_KEYBOARD_PAUSE"; break;
				case MU_KEYBOARD_ESCAPE: return "MU_KEYBOARD_ESCAPE"; break;
				case MU_KEYBOARD_MODECHANGE: return "MU_KEYBOARD_MODECHANGE"; break;
				case MU_KEYBOARD_SPACE: return "MU_KEYBOARD_SPACE"; break;
				case MU_KEYBOARD_PRIOR: return "MU_KEYBOARD_PRIOR"; break;
				case MU_KEYBOARD_NEXT: return "MU_KEYBOARD_NEXT"; break;
				case MU_KEYBOARD_END: return "MU_KEYBOARD_END"; break;
				case MU_KEYBOARD_HOME: return "MU_KEYBOARD_HOME"; break;
				case MU_KEYBOARD_LEFT: return "MU_KEYBOARD_LEFT"; break;
				case MU_KEYBOARD_UP: return "MU_KEYBOARD_UP"; break;
				case MU_KEYBOARD_RIGHT: return "MU_KEYBOARD_RIGHT"; break;
				case MU_KEYBOARD_DOWN: return "MU_KEYBOARD_DOWN"; break;
				case MU_KEYBOARD_SELECT: return "MU_KEYBOARD_SELECT"; break;
				case MU_KEYBOARD_PRINT: return "MU_KEYBOARD_PRINT"; break;
				case MU_KEYBOARD_EXECUTE: return "MU_KEYBOARD_EXECUTE"; break;
				case MU_KEYBOARD_INSERT: return "MU_KEYBOARD_INSERT"; break;
				case MU_KEYBOARD_DELETE: return "MU_KEYBOARD_DELETE"; break;
				case MU_KEYBOARD_HELP: return "MU_KEYBOARD_HELP"; break;
				case MU_KEYBOARD_0: return "MU_KEYBOARD_0"; break;
				case MU_KEYBOARD_1: return "MU_KEYBOARD_1"; break;
				case MU_KEYBOARD_2: return "MU_KEYBOARD_2"; break;
				case MU_KEYBOARD_3: return "MU_KEYBOARD_3"; break;
				case MU_KEYBOARD_4: return "MU_KEYBOARD_4"; break;
				case MU_KEYBOARD_5: return "MU_KEYBOARD_5"; break;
				case MU_KEYBOARD_6: return "MU_KEYBOARD_6"; break;
				case MU_KEYBOARD_7: return "MU_KEYBOARD_7"; break;
				case MU_KEYBOARD_8: return "MU_KEYBOARD_8"; break;
				case MU_KEYBOARD_9: return "MU_KEYBOARD_9"; break;
				case MU_KEYBOARD_A: return "MU_KEYBOARD_A"; break;
				case MU_KEYBOARD_B: return "MU_KEYBOARD_B"; break;
				case MU_KEYBOARD_C: return "MU_KEYBOARD_C"; break;
				case MU_KEYBOARD_D: return "MU_KEYBOARD_D"; break;
				case MU_KEYBOARD_E: return "MU_KEYBOARD_E"; break;
				case MU_KEYBOARD_F: return "MU_KEYBOARD_F"; break;
				case MU_KEYBOARD_G: return "MU_KEYBOARD_G"; break;
				case MU_KEYBOARD_H: return "MU_KEYBOARD_H"; break;
				case MU_KEYBOARD_I: return "MU_KEYBOARD_I"; break;
				case MU_KEYBOARD_J: return "MU_KEYBOARD_J"; break;
				case MU_KEYBOARD_K: return "MU_KEYBOARD_K"; break;
				case MU_KEYBOARD_L: return "MU_KEYBOARD_L"; break;
				case MU_KEYBOARD_M: return "MU_KEYBOARD_M"; break;
				case MU_KEYBOARD_N: return "MU_KEYBOARD_N"; break;
				case MU_KEYBOARD_O: return "MU_KEYBOARD_O"; break;
				case MU_KEYBOARD_P: return "MU_KEYBOARD_P"; break;
				case MU_KEYBOARD_Q: return "MU_KEYBOARD_Q"; break;
				case MU_KEYBOARD_R: return "MU_KEYBOARD_R"; break;
				case MU_KEYBOARD_S: return "MU_KEYBOARD_S"; break;
				case MU_KEYBOARD_T: return "MU_KEYBOARD_T"; break;
				case MU_KEYBOARD_U: return "MU_KEYBOARD_U"; break;
				case MU_KEYBOARD_V: return "MU_KEYBOARD_V"; break;
				case MU_KEYBOARD_W: return "MU_KEYBOARD_W"; break;
				case MU_KEYBOARD_X: return "MU_KEYBOARD_X"; break;
				case MU_KEYBOARD_Y: return "MU_KEYBOARD_Y"; break;
				case MU_KEYBOARD_Z: return "MU_KEYBOARD_Z"; break;
				case MU_KEYBOARD_LEFT_WINDOWS: return "MU_KEYBOARD_LEFT_WINDOWS"; break;
				case MU_KEYBOARD_RIGHT_WINDOWS: return "MU_KEYBOARD_RIGHT_WINDOWS"; break;
				case MU_KEYBOARD_NUMPAD_0: return "MU_KEYBOARD_NUMPAD_0"; break;
				case MU_KEYBOARD_NUMPAD_1: return "MU_KEYBOARD_NUMPAD_1"; break;
				case MU_KEYBOARD_NUMPAD_2: return "MU_KEYBOARD_NUMPAD_2"; break;
				case MU_KEYBOARD_NUMPAD_3: return "MU_KEYBOARD_NUMPAD_3"; break;
				case MU_KEYBOARD_NUMPAD_4: return "MU_KEYBOARD_NUMPAD_4"; break;
				case MU_KEYBOARD_NUMPAD_5: return "MU_KEYBOARD_NUMPAD_5"; break;
				case MU_KEYBOARD_NUMPAD_6: return "MU_KEYBOARD_NUMPAD_6"; break;
				case MU_KEYBOARD_NUMPAD_7: return "MU_KEYBOARD_NUMPAD_7"; break;
				case MU_KEYBOARD_NUMPAD_8: return "MU_KEYBOARD_NUMPAD_8"; break;
				case MU_KEYBOARD_NUMPAD_9: return "MU_KEYBOARD_NUMPAD_9"; break;
				case MU_KEYBOARD_ADD: return "MU_KEYBOARD_ADD"; break;
				case MU_KEYBOARD_SUBTRACT: return "MU_KEYBOARD_SUBTRACT"; break;
				case MU_KEYBOARD_MULTIPLY: return "MU_KEYBOARD_MULTIPLY"; break;
				case MU_KEYBOARD_DIVIDE: return "MU_KEYBOARD_DIVIDE"; break;
				case MU_KEYBOARD_SEPARATOR: return "MU_KEYBOARD_SEPARATOR"; break;
				case MU_KEYBOARD_DECIMAL: return "MU_KEYBOARD_DECIMAL"; break;
				case MU_KEYBOARD_F1: return "MU_KEYBOARD_F1"; break;
				case MU_KEYBOARD_F2: return "MU_KEYBOARD_F2"; break;
				case MU_KEYBOARD_F3: return "MU_KEYBOARD_F3"; break;
				case MU_KEYBOARD_F4: return "MU_KEYBOARD_F4"; break;
				case MU_KEYBOARD_F5: return "MU_KEYBOARD_F5"; break;
				case MU_KEYBOARD_F6: return "MU_KEYBOARD_F6"; break;
				case MU_KEYBOARD_F7: return "MU_KEYBOARD_F7"; break;
				case MU_KEYBOARD_F8: return "MU_KEYBOARD_F8"; break;
				case MU_KEYBOARD_F9: return "MU_KEYBOARD_F9"; break;
				case MU_KEYBOARD_F10: return "MU_KEYBOARD_F10"; break;
				case MU_KEYBOARD_F11: return "MU_KEYBOARD_F11"; break;
				case MU_KEYBOARD_F12: return "MU_KEYBOARD_F12"; break;
				case MU_KEYBOARD_F13: return "MU_KEYBOARD_F13"; break;
				case MU_KEYBOARD_F14: return "MU_KEYBOARD_F14"; break;
				case MU_KEYBOARD_F15: return "MU_KEYBOARD_F15"; break;
				case MU_KEYBOARD_F16: return "MU_KEYBOARD_F16"; break;
				case MU_KEYBOARD_F17: return "MU_KEYBOARD_F17"; break;
				case MU_KEYBOARD_F18: return "MU_KEYBOARD_F18"; break;
				case MU_KEYBOARD_F19: return "MU_KEYBOARD_F19"; break;
				case MU_KEYBOARD_F20: return "MU_KEYBOARD_F20"; break;
				case MU_KEYBOARD_F21: return "MU_KEYBOARD_F21"; break;
				case MU_KEYBOARD_F22: return "MU_KEYBOARD_F22"; break;
				case MU_KEYBOARD_F23: return "MU_KEYBOARD_F23"; break;
				case MU_KEYBOARD_F24: return "MU_KEYBOARD_F24"; break;
				case MU_KEYBOARD_NUMLOCK: return "MU_KEYBOARD_NUMLOCK"; break;
				case MU_KEYBOARD_SCROLL: return "MU_KEYBOARD_SCROLL"; break;
				case MU_KEYBOARD_LEFT_SHIFT: return "MU_KEYBOARD_LEFT_SHIFT"; break;
				case MU_KEYBOARD_RIGHT_SHIFT: return "MU_KEYBOARD_RIGHT_SHIFT"; break;
				case MU_KEYBOARD_LEFT_CONTROL: return "MU_KEYBOARD_LEFT_CONTROL"; break;
				case MU_KEYBOARD_RIGHT_CONTROL: return "MU_KEYBOARD_RIGHT_CONTROL"; break;
				case MU_KEYBOARD_LEFT_MENU: return "MU_KEYBOARD_LEFT_MENU"; break;
				case MU_KEYBOARD_RIGHT_MENU: return "MU_KEYBOARD_RIGHT_MENU"; break;
				case MU_KEYBOARD_ATTN: return "MU_KEYBOARD_ATTN"; break;
				case MU_KEYBOARD_CRSEL: return "MU_KEYBOARD_CRSEL"; break;
				case MU_KEYBOARD_EXSEL: return "MU_KEYBOARD_EXSEL"; break;
				case MU_KEYBOARD_EREOF: return "MU_KEYBOARD_EREOF"; break;
				case MU_KEYBOARD_PLAY: return "MU_KEYBOARD_PLAY"; break;
				case MU_KEYBOARD_PA1: return "MU_KEYBOARD_PA1"; break;
			}
		}

		MUDEF const char* mu_keyboard_key_get_nice_name(muKeyboardKey key) {
			switch (key) {
				default: return "Unknown"; break;
				case MU_KEYBOARD_UNKNOWN: return "Unknown"; break;
				case MU_KEYBOARD_BACKSPACE: return "Baskspace"; break;
				case MU_KEYBOARD_TAB: return "Tab"; break;
				case MU_KEYBOARD_CLEAR: return "Clear"; break;
				case MU_KEYBOARD_RETURN: return "Return"; break;
				case MU_KEYBOARD_PAUSE: return "Pause"; break;
				case MU_KEYBOARD_ESCAPE: return "Escape"; break;
				case MU_KEYBOARD_MODECHANGE: return "Mode change"; break;
				case MU_KEYBOARD_SPACE: return "Space"; break;
				case MU_KEYBOARD_PRIOR: return "Prior"; break;
				case MU_KEYBOARD_NEXT: return "Next"; break;
				case MU_KEYBOARD_END: return "End"; break;
				case MU_KEYBOARD_HOME: return "Home"; break;
				case MU_KEYBOARD_LEFT: return "Left"; break;
				case MU_KEYBOARD_UP: return "Up"; break;
				case MU_KEYBOARD_RIGHT: return "Right"; break;
				case MU_KEYBOARD_DOWN: return "Down"; break;
				case MU_KEYBOARD_SELECT: return "Select"; break;
				case MU_KEYBOARD_PRINT: return "Print"; break;
				case MU_KEYBOARD_EXECUTE: return "Execute"; break;
				case MU_KEYBOARD_INSERT: return "Insert"; break;
				case MU_KEYBOARD_DELETE: return "Delete"; break;
				case MU_KEYBOARD_HELP: return "Help"; break;
				case MU_KEYBOARD_0: return "0"; break;
				case MU_KEYBOARD_1: return "1"; break;
				case MU_KEYBOARD_2: return "2"; break;
				case MU_KEYBOARD_3: return "3"; break;
				case MU_KEYBOARD_4: return "4"; break;
				case MU_KEYBOARD_5: return "5"; break;
				case MU_KEYBOARD_6: return "6"; break;
				case MU_KEYBOARD_7: return "7"; break;
				case MU_KEYBOARD_8: return "8"; break;
				case MU_KEYBOARD_9: return "9"; break;
				case MU_KEYBOARD_A: return "A"; break;
				case MU_KEYBOARD_B: return "B"; break;
				case MU_KEYBOARD_C: return "C"; break;
				case MU_KEYBOARD_D: return "D"; break;
				case MU_KEYBOARD_E: return "E"; break;
				case MU_KEYBOARD_F: return "F"; break;
				case MU_KEYBOARD_G: return "G"; break;
				case MU_KEYBOARD_H: return "H"; break;
				case MU_KEYBOARD_I: return "I"; break;
				case MU_KEYBOARD_J: return "J"; break;
				case MU_KEYBOARD_K: return "K"; break;
				case MU_KEYBOARD_L: return "L"; break;
				case MU_KEYBOARD_M: return "M"; break;
				case MU_KEYBOARD_N: return "N"; break;
				case MU_KEYBOARD_O: return "O"; break;
				case MU_KEYBOARD_P: return "P"; break;
				case MU_KEYBOARD_Q: return "Q"; break;
				case MU_KEYBOARD_R: return "R"; break;
				case MU_KEYBOARD_S: return "S"; break;
				case MU_KEYBOARD_T: return "T"; break;
				case MU_KEYBOARD_U: return "U"; break;
				case MU_KEYBOARD_V: return "V"; break;
				case MU_KEYBOARD_W: return "W"; break;
				case MU_KEYBOARD_X: return "X"; break;
				case MU_KEYBOARD_Y: return "Y"; break;
				case MU_KEYBOARD_Z: return "Z"; break;
				case MU_KEYBOARD_LEFT_WINDOWS: return "Left Windows/Super/Command"; break;
				case MU_KEYBOARD_RIGHT_WINDOWS: return "Right Windows/Super/Command"; break;
				case MU_KEYBOARD_NUMPAD_0: return "Numpad 0"; break;
				case MU_KEYBOARD_NUMPAD_1: return "Numpad 1"; break;
				case MU_KEYBOARD_NUMPAD_2: return "Numpad 2"; break;
				case MU_KEYBOARD_NUMPAD_3: return "Numpad 3"; break;
				case MU_KEYBOARD_NUMPAD_4: return "Numpad 4"; break;
				case MU_KEYBOARD_NUMPAD_5: return "Numpad 5"; break;
				case MU_KEYBOARD_NUMPAD_6: return "Numpad 6"; break;
				case MU_KEYBOARD_NUMPAD_7: return "Numpad 7"; break;
				case MU_KEYBOARD_NUMPAD_8: return "Numpad 8"; break;
				case MU_KEYBOARD_NUMPAD_9: return "Numpad 9"; break;
				case MU_KEYBOARD_ADD: return "Add"; break;
				case MU_KEYBOARD_SUBTRACT: return "Subtract"; break;
				case MU_KEYBOARD_MULTIPLY: return "Multiply"; break;
				case MU_KEYBOARD_DIVIDE: return "Divide"; break;
				case MU_KEYBOARD_SEPARATOR: return "Separator"; break;
				case MU_KEYBOARD_DECIMAL: return "Decimal"; break;
				case MU_KEYBOARD_F1: return "F1"; break;
				case MU_KEYBOARD_F2: return "F2"; break;
				case MU_KEYBOARD_F3: return "F3"; break;
				case MU_KEYBOARD_F4: return "F4"; break;
				case MU_KEYBOARD_F5: return "F5"; break;
				case MU_KEYBOARD_F6: return "F6"; break;
				case MU_KEYBOARD_F7: return "F7"; break;
				case MU_KEYBOARD_F8: return "F8"; break;
				case MU_KEYBOARD_F9: return "F9"; break;
				case MU_KEYBOARD_F10: return "F10"; break;
				case MU_KEYBOARD_F11: return "F11"; break;
				case MU_KEYBOARD_F12: return "F12"; break;
				case MU_KEYBOARD_F13: return "F13"; break;
				case MU_KEYBOARD_F14: return "F14"; break;
				case MU_KEYBOARD_F15: return "F15"; break;
				case MU_KEYBOARD_F16: return "F16"; break;
				case MU_KEYBOARD_F17: return "F17"; break;
				case MU_KEYBOARD_F18: return "F18"; break;
				case MU_KEYBOARD_F19: return "F19"; break;
				case MU_KEYBOARD_F20: return "F20"; break;
				case MU_KEYBOARD_F21: return "F21"; break;
				case MU_KEYBOARD_F22: return "F22"; break;
				case MU_KEYBOARD_F23: return "F23"; break;
				case MU_KEYBOARD_F24: return "F24"; break;
				case MU_KEYBOARD_NUMLOCK: return "Num Lock"; break;
				case MU_KEYBOARD_SCROLL: return "Scroll"; break;
				case MU_KEYBOARD_LEFT_SHIFT: return "Left shift"; break;
				case MU_KEYBOARD_RIGHT_SHIFT: return "Right shift"; break;
				case MU_KEYBOARD_LEFT_CONTROL: return "Left control"; break;
				case MU_KEYBOARD_RIGHT_CONTROL: return "Right control"; break;
				case MU_KEYBOARD_LEFT_MENU: return "Left menu"; break;
				case MU_KEYBOARD_RIGHT_MENU: return "Right menu"; break;
				case MU_KEYBOARD_ATTN: return "Attn"; break;
				case MU_KEYBOARD_CRSEL: return "CrSel"; break;
				case MU_KEYBOARD_EXSEL: return "ExSel"; break;
				case MU_KEYBOARD_EREOF: return "EreOf"; break;
				case MU_KEYBOARD_PLAY: return "Play"; break;
				case MU_KEYBOARD_PA1: return "PA1"; break;
			}
		}

		MUDEF const char* mu_keystate_get_name(muKeyboardState state) {
			switch (state) {
				default: return "MU_UNKNOWN"; break;
				case MU_KEYSTATE_UNKNOWN: return "MU_KEYSTATE_UNKNOWN"; break;
				case MU_KEYSTATE_CAPS_LOCK: return "MU_KEYSTATE_CAPS_LOCK"; break;
				case MU_KEYSTATE_SCROLL_LOCK: return "MU_KEYSTATE_SCROLL_LOCK"; break;
				case MU_KEYSTATE_NUM_LOCK: return "MU_KEYSTATE_NUM_LOCK"; break;
			}
		}

		MUDEF const char* mu_keystate_get_nice_name(muKeyboardState state) {
			switch (state) {
				default: return "Unknown"; break;
				case MU_KEYSTATE_UNKNOWN: return "Unknown"; break;
				case MU_KEYSTATE_CAPS_LOCK: return "Caps Lock"; break;
				case MU_KEYSTATE_SCROLL_LOCK: return "Scroll Lock"; break;
				case MU_KEYSTATE_NUM_LOCK: return "Num Lock"; break;
			}
		}

		MUDEF const char* mu_mouse_key_get_name(muMouseKey key) {
			switch (key) {
				default: return "MU_UNKNOWN"; break;
				case MU_MOUSE_UNKNOWN: return "MU_MOUSE_UNKNOWN"; break;
				case MU_MOUSE_LEFT: return "MU_MOUSE_LEFT"; break;
				case MU_MOUSE_RIGHT: return "MU_MOUSE_RIGHT"; break;
				case MU_MOUSE_MIDDLE: return "MU_MOUSE_MIDDLE"; break;
			}
		}

		MUDEF const char* mu_mouse_key_get_nice_name(muMouseKey key) {
			switch (key) {
				default: return "Unknown"; break;
				case MU_MOUSE_UNKNOWN: return "Unknown"; break;
				case MU_MOUSE_LEFT: return "Left"; break;
				case MU_MOUSE_RIGHT: return "Right"; break;
				case MU_MOUSE_MIDDLE: return "Middle"; break;
			}
		}

		MUDEF const char* mu_cursor_style_get_name(muCursorStyle style) {
			switch (style) {
				default: return "MU_UNKNOWN"; break;
				case MU_CURSOR_UNKNOWN: return "MU_CURSOR_UNKNOWN"; break;
				case MU_CURSOR_ARROW: return "MU_CURSOR_ARROW"; break;
				case MU_CURSOR_IBEAM: return "MU_CURSOR_IBEAM"; break;
				case MU_CURSOR_WAIT: return "MU_CURSOR_WAIT"; break;
				case MU_CURSOR_WAIT_ARROW: return "MU_CURSOR_WAIT_ARROW"; break;
				case MU_CURSOR_CROSSHAIR: return "MU_CURSOR_CROSSHAIR"; break;
				case MU_CURSOR_HAND: return "MU_CURSOR_HAND"; break;
				case MU_CURSOR_SIZE_LR: return "MU_CURSOR_SIZE_LR"; break;
				case MU_CURSOR_SIZE_TB: return "MU_CURSOR_SIZE_TB"; break;
				case MU_CURSOR_SIZE_TL_BR: return "MU_CURSOR_SIZE_TL_BR"; break;
				case MU_CURSOR_SIZE_TR_BL: return "MU_CURSOR_SIZE_TR_BL"; break;
				case MU_CURSOR_SIZE_ALL: return "MU_CURSOR_SIZE_ALL"; break;
				case MU_CURSOR_NO: return "MU_CURSOR_NO"; break;
			}
		}

		MUDEF const char* mu_cursor_style_get_nice_name(muCursorStyle style) {
			switch (style) {
				default: return "Unknown"; break;
				case MU_CURSOR_UNKNOWN: return "Unknown"; break;
				case MU_CURSOR_ARROW: return "Arrow"; break;
				case MU_CURSOR_IBEAM: return "I-Beam"; break;
				case MU_CURSOR_WAIT: return "Wait"; break;
				case MU_CURSOR_WAIT_ARROW: return "Wait (Arrow)"; break;
				case MU_CURSOR_CROSSHAIR: return "Crosshair"; break;
				case MU_CURSOR_HAND: return "Hand"; break;
				case MU_CURSOR_SIZE_LR: return "Resize (Left to right)"; break;
				case MU_CURSOR_SIZE_TB: return "Resize (Top to bottom)"; break;
				case MU_CURSOR_SIZE_TL_BR: return "Resize (Top-left to bottom-right)"; break;
				case MU_CURSOR_SIZE_TR_BL: return "Resize (Top-right to bottom-left)"; break;
				case MU_CURSOR_SIZE_ALL: return "Resize (All 4 directions)"; break;
				case MU_CURSOR_NO: return "No/Error"; break;
			}
		}

		MUDEF const char* mu_graphics_api_get_name(muGraphicsAPI api) {
			switch (api) {
				default: return "MU_UNKNOWN"; break;
				case MU_NULL_GRAPHICS_API: return "MU_NULL_GRAPHICS_API"; break;
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
				case MU_OPENGL_3_2_CORE: return "MU_OPENGL_3_2_CORE"; break;
				case MU_OPENGL_3_2_COMPATIBILITY: return "MU_OPENGL_3_2_COMPATIBILITY"; break;
				case MU_OPENGL_3_3_CORE: return "MU_OPENGL_3_3_CORE"; break;
				case MU_OPENGL_3_3_COMPATIBILITY: return "MU_OPENGL_3_3_COMPATIBILITY"; break;
				case MU_OPENGL_4_0_CORE: return "MU_OPENGL_4_0_CORE"; break;
				case MU_OPENGL_4_0_COMPATIBILITY: return "MU_OPENGL_4_0_COMPATIBILITY"; break;
				case MU_OPENGL_4_1_CORE: return "MU_OPENGL_4_1_CORE"; break;
				case MU_OPENGL_4_1_COMPATIBILITY: return "MU_OPENGL_4_1_COMPATIBILITY"; break;
				case MU_OPENGL_4_2_CORE: return "MU_OPENGL_4_2_CORE"; break;
				case MU_OPENGL_4_2_COMPATIBILITY: return "MU_OPENGL_4_2_COMPATIBILITY"; break;
				case MU_OPENGL_4_3_CORE: return "MU_OPENGL_4_3_CORE"; break;
				case MU_OPENGL_4_3_COMPATIBILITY: return "MU_OPENGL_4_3_COMPATIBILITY"; break;
				case MU_OPENGL_4_4_CORE: return "MU_OPENGL_4_4_CORE"; break;
				case MU_OPENGL_4_4_COMPATIBILITY: return "MU_OPENGL_4_4_COMPATIBILITY"; break;
				case MU_OPENGL_4_5_CORE: return "MU_OPENGL_4_5_CORE"; break;
				case MU_OPENGL_4_5_COMPATIBILITY: return "MU_OPENGL_4_5_COMPATIBILITY"; break;
				case MU_OPENGL_4_6_CORE: return "MU_OPENGL_4_6_CORE"; break;
				case MU_OPENGL_4_6_COMPATIBILITY: return "MU_OPENGL_4_6_COMPATIBILITY"; break;
			}
		}

		MUDEF const char* mu_graphics_api_get_nice_name(muGraphicsAPI api) {
			switch (api) {
				default: return "Unknown"; break;
				case MU_NULL_GRAPHICS_API: return "Unknown"; break;
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
				case MU_OPENGL_3_2_CORE: return "OpenGL 3.2 (Core Profile)"; break;
				case MU_OPENGL_3_2_COMPATIBILITY: return "OpenGL 3.2 (Compatibility Profile)"; break;
				case MU_OPENGL_3_3_CORE: return "OpenGL 3.3 (Core Profile)"; break;
				case MU_OPENGL_3_3_COMPATIBILITY: return "OpenGL 3.3 (Compatibility Profile)"; break;
				case MU_OPENGL_4_0_CORE: return "OpenGL 4.0 (Core Profile)"; break;
				case MU_OPENGL_4_0_COMPATIBILITY: return "OpenGL 4.0 (Compatibility Profile)"; break;
				case MU_OPENGL_4_1_CORE: return "OpenGL 4.1 (Core Profile)"; break;
				case MU_OPENGL_4_1_COMPATIBILITY: return "OpenGL 4.1 (Compatibility Profile)"; break;
				case MU_OPENGL_4_2_CORE: return "OpenGL 4.2 (Core Profile)"; break;
				case MU_OPENGL_4_2_COMPATIBILITY: return "OpenGL 4.2 (Compatibility Profile)"; break;
				case MU_OPENGL_4_3_CORE: return "OpenGL 4.3 (Core Profile)"; break;
				case MU_OPENGL_4_3_COMPATIBILITY: return "OpenGL 4.3 (Compatibility Profile)"; break;
				case MU_OPENGL_4_4_CORE: return "OpenGL 4.4 (Core Profile)"; break;
				case MU_OPENGL_4_4_COMPATIBILITY: return "OpenGL 4.4 (Compatibility Profile)"; break;
				case MU_OPENGL_4_5_CORE: return "OpenGL 4.5 (Core Profile)"; break;
				case MU_OPENGL_4_5_COMPATIBILITY: return "OpenGL 4.5 (Compatibility Profile)"; break;
				case MU_OPENGL_4_6_CORE: return "OpenGL 4.6 (Core Profile)"; break;
				case MU_OPENGL_4_6_COMPATIBILITY: return "OpenGL 4.6 (Compatibility Profile)"; break;
			}
		}
	)

	MU_CPP_EXTERN_END

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

