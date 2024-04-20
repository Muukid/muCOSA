/*
muCOSA.h - Muukid
Public domain single-file C library for interacting with operating systems with a cross-platform API.
No warranty implied; use at your own risk.

Licensed under MIT License or public domain, whichever you prefer.
More explicit license information at the end of file.

@MENTION muCOSA works in null-terminated UTF-8.
@MENTION For now, muCOSA is targeted at operating systems that work around a mouse/keyboard/display
interface.
@MENTION muCOSA is largely untested on multi-display setups.
@MENTION Key callback repeats on hold. This is inteded, as to allow hold times to be handled by the
OS.
@MENTION Setting attributes about windows may invoke 2 identical callbacks. In general, non-
identical callbacks or duplicate callbacks are not guaranteed to not occur.
@MENTION Callbacks do NOT only occur on calls to mu_window_update, although they usually do.

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

https://forums.gentoo.org/viewtopic-t-757913-start-0.html
*/

#ifndef MUCOSA_H
	#define MUCOSA_H
	
	/* muString commit 6678f59 header */
	
		#if !defined(MU_CHECK_VERSION_MISMATCHING) && defined(MUS_H) && \
			(MUS_VERSION_MAJOR != 1 || MUS_VERSION_MINOR != 0 || MUS_VERSION_PATCH != 0)

			#pragma message("[MUCOSA] muString's header has already been defined, but version doesn't match the version that this library is built for. This may lead to errors, warnings, or unexpected behavior. Define MU_CHECK_VERSION_MISMATCHING before this to turn off this message.")

		#endif

		#ifndef MUS_H
			#define MUS_H
			
			/* muMemoryAllocator version 1.0.0 header */
			
				#if !defined(MU_CHECK_VERSION_MISMATCHING) && defined(MUMA_H) && \
					(MUMA_VERSION_MAJOR != 1 || MUMA_VERSION_MINOR != 0 || MUMA_VERSION_PATCH != 0)

					#pragma message("[MUS] muMemoryAllocator's header has already been defined, but version doesn't match the version that this library is built for. This may lead to errors, warnings, or unexpected behavior. Define MU_CHECK_VERSION_MISMATCHING before this to turn off this message.")

				#endif

				#ifndef MUMA_H
					#define MUMA_H
					
					/* muUtility version 1.0.0 header */
					
						#if !defined(MU_CHECK_VERSION_MISMATCHING) && defined(MUU_H) && \
							(MUU_VERSION_MAJOR != 1 || MUU_VERSION_MINOR != 0 || MUU_VERSION_PATCH != 0)

							#pragma message("[MUMA] muUtility's header has already been defined, but version doesn't match the version that this library is built for. This may lead to errors, warnings, or unexpected behavior. Define MU_CHECK_VERSION_MISMATCHING before this to turn off this message.")

						#endif

						#ifndef MUU_H
							#define MUU_H
							
							#if !defined(MU_SECURE_WARNINGS) && !defined(_CRT_SECURE_NO_WARNINGS)
								#define _CRT_SECURE_NO_WARNINGS
							#endif

							#ifdef __cplusplus
							extern "C" { // }
							#endif

							#define MUU_VERSION_MAJOR 1
							#define MUU_VERSION_MINOR 0
							#define MUU_VERSION_PATCH 0

							/* C standard library dependencies */

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

									#ifndef int8_m
										#ifdef INT8_MAX
											#define int8_m int8_t
										#else
											#define int8_m char
										#endif
									#endif

									#ifndef uint8_m
										#ifdef UINT8_MAX
											#define uint8_m uint8_t
										#else
											#define uint8_m unsigned char
										#endif
									#endif

									#ifndef int16_m
										#ifdef INT16_MAX
											#define int16_m int16_t
										#else
											#define int16_m short
										#endif
									#endif

									#ifndef uint16_m
										#ifdef UINT16_MAX
											#define uint16_m uint16_t
										#else
											#define uint16_m unsigned short
										#endif
									#endif

									#ifndef int32_m
										#ifdef INT32_MAX
											#define int32_m int32_t
										#else
											#define int32_m long
										#endif
									#endif

									#ifndef uint32_m
										#ifdef UINT32_MAX
											#define uint32_m uint32_t
										#else
											#define uint32_m unsigned long
										#endif
									#endif

									#ifndef int64_m
										#ifdef INT64_MAX
											#define int64_m int64_t
										#else
											#define int64_m long long
										#endif
									#endif

									#ifndef uint64_m
										#ifdef UINT64_MAX
											#define uint64_m uint64_t
										#else
											#define uint64_m unsigned long long
										#endif
									#endif

									#ifndef MU_SIZE_MAX
										#define MU_SIZE_MAX SIZE_MAX
									#endif

								#endif

								#if !defined(size_m)

									#include <stddef.h>

									#ifndef size_m
										#define size_m size_t
									#endif

								#endif

								#if !defined(muBool)   || \
									!defined(MU_TRUE)  || \
									!defined(MU_FALSE)

									#include <stdbool.h>

									#ifndef muBool
										#define muBool bool
									#endif

									#ifndef MU_TRUE
										#define MU_TRUE true
									#endif

									#ifndef MU_FALSE
										#define MU_FALSE false
									#endif

								#endif

							/* Useful macros */

								#ifndef MUDEF
									#ifdef MU_STATIC
										#define MUDEF static
									#else
										#define MUDEF extern
									#endif
								#endif

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

								#ifndef muByte
									#define muByte uint8_m
								#endif

								#ifndef mu_rle_uint8
									#define mu_rle_uint8(b) ((uint8_m)b[0] << 0)
								#endif
								#ifndef mu_rbe_uint8
									#define mu_rbe_uint8(b) ((uint8_m)b[0] << 0)
								#endif

								#ifndef mu_rle_uint16
									#define mu_rle_uint16(b) ((uint16_m)b[0] << 0 | (uint16_m)b[1] << 8)
								#endif
								#ifndef mu_rbe_uint16
									#define mu_rbe_uint16(b) ((uint16_m)b[1] << 0 | (uint16_m)b[0] << 8)
								#endif

								#ifndef mu_rle_uint32
									#define mu_rle_uint32(b) ((uint32_m)b[0] << 0 | (uint32_m)b[1] << 8 | (uint32_m)b[2] << 16 | (uint32_m)b[3] << 24)
								#endif
								#ifndef mu_rbe_uint32
									#define mu_rbe_uint32(b) ((uint32_m)b[3] << 0 | (uint32_m)b[2] << 8 | (uint32_m)b[1] << 16 | (uint32_m)b[0] << 24)
								#endif

								#ifndef mu_rle_uint64
									#define mu_rle_uint64(b) ((uint64_m)b[0] << 0 | (uint64_m)b[1] << 8 | (uint64_m)b[2] << 16 | (uint64_m)b[3] << 24 | (uint64_m)b[4] << 32 | (uint64_m)b[5] << 40 | (uint64_m)b[6] << 48 | (uint64_m)b[7] << 56)
								#endif
								#ifndef mu_rbe_uint64
									#define mu_rbe_uint64(b) ((uint64_m)b[7] << 0 | (uint64_m)b[6] << 8 | (uint64_m)b[5] << 16 | (uint64_m)b[4] << 24 | (uint64_m)b[3] << 32 | (uint64_m)b[2] << 40 | (uint64_m)b[1] << 48 | (uint64_m)b[0] << 56)
								#endif

								#ifndef mu_wle_uint8
									#define mu_wle_uint8(b, i) b[0] = (uint8_m)(i >> 0);
								#endif
								#ifndef mu_wbe_uint8
									#define mu_wbe_uint8(b, i) b[0] = (uint8_m)(i >> 0);
								#endif

								#ifndef mu_wle_uint16
									#define mu_wle_uint16(b, i) b[0] = (uint8_m)(i >> 0); b[1] = (uint8_m)(i >> 8);
								#endif
								#ifndef mu_wbe_uint16
									#define mu_wbe_uint16(b, i) b[1] = (uint8_m)(i >> 0); b[0] = (uint8_m)(i >> 8);
								#endif

								#ifndef mu_wle_uint32
									#define mu_wle_uint32(b, i) b[0] = (uint8_m)(i >> 0); b[1] = (uint8_m)(i >> 8); b[2] = (uint8_m)(i >> 16); b[3] = (uint8_m)(i >> 24);
								#endif
								#ifndef mu_wbe_uint32
									#define mu_wbe_uint32(b, i) b[3] = (uint8_m)(i >> 0); b[2] = (uint8_m)(i >> 8); b[1] = (uint8_m)(i >> 16); b[0] = (uint8_m)(i >> 24);
								#endif

								#ifndef mu_wle_uint64
									#define mu_wle_uint64(b, i) b[0] = (uint8_m)(i >> 0); b[1] = (uint8_m)(i >> 8); b[2] = (uint8_m)(i >> 16); b[3] = (uint8_m)(i >> 24); b[4] = (uint8_m)(i >> 32); b[5] = (uint8_m)(i >> 40); b[6] = (uint8_m)(i >> 48); b[7] = (uint8_m)(i >> 56);
								#endif
								#ifndef mu_wbe_uint64
									#define mu_wbe_uint64(b, i) b[7] = (uint8_m)(i >> 0); b[6] = (uint8_m)(i >> 8); b[5] = (uint8_m)(i >> 16); b[4] = (uint8_m)(i >> 24); b[3] = (uint8_m)(i >> 32); b[2] = (uint8_m)(i >> 40); b[1] = (uint8_m)(i >> 48); b[0] = (uint8_m)(i >> 56);
								#endif

								#ifndef MU_NULL_PTR
									#define MU_NULL_PTR 0
								#endif

								#ifndef MU_NULL
									#define MU_NULL 0
								#endif

								#ifndef MU_NONE
									#define MU_NONE MU_SIZE_MAX
								#endif

								#ifndef MU_SET_RESULT
									#define MU_SET_RESULT(res, val) if(res!=MU_NULL_PTR){*res=val;}
								#endif

								#ifndef MU_ASSERT
									#define MU_ASSERT(cond, res, val, after) if(!(cond)){MU_SET_RESULT(res, val) after}
								#endif

								#define MU_ENUM(name, ...) enum _##name{__VA_ARGS__};typedef enum _##name _##name; typedef size_m name;

								#if !defined(MU_WIN32) && !defined(MU_UNIX)
									#if defined(WIN32) || defined(_WIN32)
										#define MU_WIN32
									#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
										#define MU_UNIX
									#endif
								#endif

								#define MU_HRARRAY_DEFAULT_FUNC(name) \
									muBool name##_comp(name t0, name t1) { \
										return t0.active == t1.active; \
									} \
									\
									void name##_on_creation(name* p) { \
										if (p != MU_NULL_PTR) { \
											MU_LOCK_CREATE(p->lock, p->lock_active) \
										} \
									} \
									void name##_on_destruction(name* p) { \
										if (p != MU_NULL_PTR) { \
											MU_LOCK_DESTROY(p->lock, p->lock_active) \
										} \
									} \
									void name##_on_hold(name* p) { \
										if (p != MU_NULL_PTR) { \
											MU_LOCK_LOCK(p->lock, p->lock_active) \
										} \
									} \
									void name##_on_release(name* p) { \
										if (p != MU_NULL_PTR) { \
											MU_LOCK_UNLOCK(p->lock, p->lock_active) \
										} \
									} \
									\
									mu_dynamic_hrarray_declaration( \
										name##_array, name, name##_, name##_comp, \
										name##_on_creation, name##_on_destruction, name##_on_hold, name##_on_release \
									)

								#define MU_SAFEFUNC(result, lib_prefix, context, fail_return) \
									MU_SET_RESULT(result, lib_prefix##SUCCESS) \
									MU_ASSERT(context != MU_NULL_PTR, result, lib_prefix##NOT_YET_INITIALIZED, fail_return) \

								#define MU_HOLD(result, item, da, context, lib_prefix, fail_return, da_prefix) \
									MU_ASSERT(item < da.length, result, lib_prefix##INVALID_ID, fail_return) \
									da_prefix##hold_element(0, &da, item); \
									MU_ASSERT(da.data[item].active, result, lib_prefix##INVALID_ID, da_prefix##release_element(0, &da, item); fail_return)

								#define MU_RELEASE(da, item, da_prefix) \
									da_prefix##release_element(0, &da, item);

							#ifdef __cplusplus
							}
							#endif

						#endif /* MUU_H */
					
					#ifdef __cplusplus
						extern "C" {
					#endif

					#define MUMA_VERSION_MAJOR 1
					#define MUMA_VERSION_MINOR 0
					#define MUMA_VERSION_PATCH 0

					/* C standard library dependencies */

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

						#if !defined(mu_memset)  || \
							!defined(mu_memcpy)  || \
							!defined(mu_memmove)

							#include <string.h>

							#ifndef mu_memset
								#define mu_memset memset
							#endif

							#ifndef mu_memcpy
								#define mu_memcpy memcpy
							#endif

							#ifndef mu_memmove
								#define mu_memmove memmove
							#endif

						#endif

					/* Enums */

						MU_ENUM(mumaResult, 
							MUMA_SUCCESS,

							MUMA_FAILED_TO_ALLOCATE,
							MUMA_INVALID_INDEX,
							MUMA_INVALID_SHIFT_AMOUNT,
							MUMA_NOT_FOUND
						)

					/* Functions */

						#ifdef MUMA_NAMES
							MUDEF const char* muma_result_get_name(mumaResult result);
						#endif

					/* API macro functionality */

						#define mu_dynamic_array_declaration(struct_name, type, function_name_prefix, type_comparison_func) \
							\
							struct struct_name { \
								type* data; \
								size_m allocated_length; \
								size_m length; \
							}; typedef struct struct_name struct_name; \
							\
							struct_name function_name_prefix##create(mumaResult* result, size_m length) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								struct_name s = MU_ZERO_STRUCT(struct_name); \
								s.data = MU_NULL_PTR; \
								s.allocated_length = length; \
								s.length = length; \
								\
								if (s.length == 0) { \
									return s; \
								} \
								\
								s.data = (type*)mu_malloc(sizeof(type)*s.allocated_length); \
								if (s.data == 0) { \
									MU_SET_RESULT(result, MUMA_FAILED_TO_ALLOCATE) \
									s.allocated_length = 0; \
									s.length = 0; \
									return s; \
								} \
								\
								mu_memset(s.data, 0, sizeof(type)*s.allocated_length);\
								return s;\
							} \
							\
							struct_name function_name_prefix##destroy(mumaResult* result, struct_name s) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								if (s.data != MU_NULL_PTR) { \
									mu_free(s.data); \
									s.data = MU_NULL_PTR; \
								} \
								\
								s.allocated_length = 0; \
								s.length = 0; \
								return s; \
							} \
							\
							struct_name function_name_prefix##resize(mumaResult* result, struct_name s, size_m length) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								if (s.length == length) { \
									return s; \
								} \
								\
								if (s.data == MU_NULL_PTR) { \
									if (length == 0) { \
										return s; \
									} \
									\
									mumaResult res = MUMA_SUCCESS; \
									s = function_name_prefix##create(&res, length); \
									if (res != MUMA_SUCCESS) { \
										MU_SET_RESULT(result, res) \
										return s; \
									} \
								} \
								\
								if (length == 0) { \
									s.length = 0; \
									mu_memset(s.data, 0, sizeof(type)*s.allocated_length); \
									return s; \
								} \
								\
								size_m old_length = s.length; \
								size_m old_allocated_length = s.allocated_length; \
								s.length = length; \
								\
								if ((s.length > s.allocated_length) || (s.length < s.allocated_length/2)) { \
									while (s.length > s.allocated_length) { \
										s.allocated_length *= 2; \
									} \
									while (s.length < s.allocated_length/2) { \
										s.allocated_length /= 2; \
									} \
									\
									type* new_data = (type*)mu_realloc(s.data, sizeof(type) * s.allocated_length); \
									if (new_data == 0) { \
										s.length = old_length; \
										s.allocated_length = old_allocated_length; \
										MU_SET_RESULT(result, MUMA_FAILED_TO_ALLOCATE) \
										return s; \
									} \
									\
									s.data = new_data; \
								} \
								\
								if (old_length < s.length) { \
									mu_memset(&s.data[old_length], 0, sizeof(type)*(s.allocated_length-old_length)); \
								} \
								\
								return s; \
							} \
							\
							struct_name function_name_prefix##lshift(mumaResult* result, struct_name s, size_m index, size_m amount) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								if (index >= s.length) { \
									MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
									return s; \
								} \
								\
								if (amount == 0) { \
									return s; \
								} \
								\
								if (amount > index) { \
									MU_SET_RESULT(result, MUMA_INVALID_SHIFT_AMOUNT) \
									return s; \
								} \
								\
								mu_memmove(&s.data[index-amount], &s.data[index], sizeof(type)*(s.length-index)); \
								\
								mumaResult res = MUMA_SUCCESS; \
								s = function_name_prefix##resize(&res, s, s.length-amount); \
								if (res != MUMA_SUCCESS) { \
									MU_SET_RESULT(result, res) \
									return s; \
								} \
								\
								return s; \
							} \
							\
							struct_name function_name_prefix##rshift(mumaResult* result, struct_name s, size_m index, size_m amount) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								if (index >= s.length) { \
									MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
									return s; \
								} \
								\
								if (amount == 0) { \
									return s; \
								} \
								\
								mumaResult res = MUMA_SUCCESS; \
								s = function_name_prefix##resize(&res, s, s.length+amount); \
								if (res != MUMA_SUCCESS) { \
									MU_SET_RESULT(result, res) \
									return s; \
								} \
								\
								/* I have genuinely no idea why it needs to have a -1. It crashes if you don't. */\
								/* And lshift doesn't need one. Why?? */ \
								mu_memmove(&s.data[index+amount], &s.data[index], sizeof(type)*((s.length-index)-1)); \
								mu_memset(&s.data[index], 0, sizeof(type)*(amount)); \
								\
								return s; \
							} \
							\
							struct_name function_name_prefix##multiinsert(mumaResult* result, struct_name s, size_m index, type* insert, size_m count) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								mumaResult res = MUMA_SUCCESS; \
								s = function_name_prefix##rshift(&res, s, index, count); \
								if (res != MUMA_SUCCESS) { \
									MU_SET_RESULT(result, res) \
									return s; \
								} \
								\
								mu_memcpy(&s.data[index], insert, sizeof(type)*count); \
								return s; \
							} \
							\
							struct_name function_name_prefix##insert(mumaResult* result, struct_name s, size_m index, type insert) { \
								return function_name_prefix##multiinsert(result, s, index, &insert, 1); \
							} \
							\
							struct_name function_name_prefix##multierase(mumaResult* result, struct_name s, size_m index, size_m count) { \
								return function_name_prefix##lshift(result, s, index+count, count); \
							} \
							\
							struct_name function_name_prefix##erase(mumaResult* result, struct_name s, size_m index) { \
								return function_name_prefix##multierase(result, s, index, 1); \
							} \
							struct_name function_name_prefix##clear(mumaResult* result, struct_name s) { \
								return function_name_prefix##destroy(result, s); \
							} \
							\
							struct_name function_name_prefix##multipush(mumaResult* result, struct_name s, type* push, size_m count) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								mumaResult res = MUMA_SUCCESS; \
								s = function_name_prefix##resize(&res, s, s.length+count); \
								if (res != MUMA_SUCCESS) { \
									MU_SET_RESULT(result, res) \
									return s; \
								} \
								\
								mu_memcpy(&s.data[s.length-count], push, sizeof(type)*count); \
								return s; \
							} \
							\
							struct_name function_name_prefix##push(mumaResult* result, struct_name s, type push) { \
								return function_name_prefix##multipush(result, s, &push, 1); \
							} \
							\
							struct_name function_name_prefix##multipop(mumaResult* result, struct_name s, size_m count) { \
								return function_name_prefix##resize(result, s, s.length-count); \
							} \
							\
							struct_name function_name_prefix##pop(mumaResult* result, struct_name s) { \
								return function_name_prefix##multipop(result, s, 1); \
							} \
							\
							size_m function_name_prefix##find(mumaResult* result, struct_name s, type find) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								for (size_m i = 0; i < s.length; i++) { \
									if (type_comparison_func(find, s.data[i])) { \
										return i; \
									} \
								} \
								\
								MU_SET_RESULT(result, MUMA_NOT_FOUND) \
								return MU_NONE; \
							} \
							\
							struct_name function_name_prefix##find_push(mumaResult* result, struct_name s, type find, size_m* p_index) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								mumaResult res = MUMA_SUCCESS; \
								size_m index = function_name_prefix##find(&res, s, find); \
								if (index != MU_NONE) { \
									if (p_index != MU_NULL_PTR) { \
										*p_index = index; \
									} \
									return s; \
								} \
								if (res != MUMA_SUCCESS && res != MUMA_NOT_FOUND) { \
									if (p_index != MU_NULL_PTR) { \
										*p_index = MU_NONE; \
									} \
									MU_SET_RESULT(result, res) \
									return s; \
								} \
								\
								s = function_name_prefix##push(&res, s, find); \
								if (res != MUMA_SUCCESS) { \
									if (p_index != MU_NULL_PTR) { \
										*p_index = MU_NONE; \
									} \
									MU_SET_RESULT(result, res) \
									return s; \
								} \
								\
								if (p_index != MU_NULL_PTR) { \
									*p_index = s.length-1; \
								} \
								return s; \
							}

						#define mu_dynamic_hrarray_declaration( \
							struct_name, type, function_name_prefix, type_comparison_func, \
							on_creation, on_destruction, on_hold, on_release \
						) \
							\
							struct struct_name { \
								type* data; \
								size_m allocated_length; \
								size_m length; \
							}; typedef struct struct_name struct_name; \
							\
							struct_name function_name_prefix##create(mumaResult* result, size_m length) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								struct_name s = MU_ZERO_STRUCT(struct_name); \
								s.data = MU_NULL_PTR; \
								s.allocated_length = length; \
								s.length = length; \
								\
								if (s.length == 0) { \
									return s; \
								} \
								\
								s.data = (type*)mu_malloc(sizeof(type)*s.allocated_length); \
								if (s.data == 0) { \
									MU_SET_RESULT(result, MUMA_FAILED_TO_ALLOCATE) \
									s.allocated_length = 0; \
									s.length = 0; \
									return s; \
								} \
								\
								mu_memset(s.data, 0, sizeof(type)*s.allocated_length); \
								\
								for (size_m i = 0; i < s.length; i++) { \
									on_creation(&s.data[i]); \
								} \
								return s;\
							} \
							\
							void function_name_prefix##hold_element(mumaResult* result, struct_name* s, size_m index) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								if (index >= s->length) { \
									MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
									return; \
								} \
								\
								on_hold(&s->data[index]); \
							} \
							\
							void function_name_prefix##release_element(mumaResult* result, struct_name* s, size_m index) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								if (index >= s->length) { \
									MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
									return; \
								} \
								\
								on_release(&s->data[index]); \
							} \
							\
							void function_name_prefix##destroy(mumaResult* result, struct_name* s) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								for (size_m i = 0; i < s->length; i++) { \
									function_name_prefix##hold_element(0, s, i); \
								} \
								\
								for (size_m i = 0; i < s->length; i++) { \
									on_destruction(&s->data[i]); \
								} \
								\
								if (s->data != MU_NULL_PTR) { \
									mu_free(s->data); \
									s->data = MU_NULL_PTR; \
								} \
								\
								s->allocated_length = 0; \
								s->length = 0; \
							} \
							\
							void function_name_prefix##inner_resize(mumaResult* result, struct_name* s, size_m length, muBool cd) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								if (s->length == length) { \
									return; \
								} \
								\
								if (s->data == MU_NULL_PTR) { \
									if (length == 0) { \
										return; \
									} \
									\
									mumaResult res = MUMA_SUCCESS; \
									*s = function_name_prefix##create(&res, length); \
									if (res != MUMA_SUCCESS) { \
										MU_SET_RESULT(result, res) \
										return; \
									} \
								} \
								\
								if (length == 0) { \
									for (size_m i = 0; i < s->length; i++) { \
										on_destruction(&s->data[i]); \
									} \
									s->length = 0; \
									mu_memset(s->data, 0, sizeof(type)*s->allocated_length); \
									return; \
								} \
								\
								if (cd) { \
									for (size_m i = 0; i < s->length; i++) { \
										function_name_prefix##hold_element(0, s, i); \
									} \
								} \
								\
								size_m old_length = s->length; \
								size_m old_allocated_length = s->allocated_length; \
								s->length = length; \
								\
								/* Note: this is really dangerous, because it's not guaranteed that the  */ \
								/* reallocation will follow through. If it doesn't, we've now called the */ \
								/* destroy function on a bunch of elements that still exist. I can't     */ \
								/* really think of a better way of doing it than this right now, though. */ \
								if (cd && old_length > s->length) { \
									for (size_m i = s->length; i < old_length; i++) { \
										on_destruction(&s->data[i]); \
									} \
								} \
								\
								if ((s->length > s->allocated_length) || (s->length < s->allocated_length/2)) { \
									while (s->length > s->allocated_length) { \
										s->allocated_length *= 2; \
									} \
									while (s->length < s->allocated_length/2) { \
										s->allocated_length /= 2; \
									} \
									\
									type* new_data = (type*)mu_realloc(s->data, sizeof(type) * s->allocated_length); \
									if (new_data == 0) { \
										s->length = old_length; \
										s->allocated_length = old_allocated_length; \
										if (cd) { \
											for (size_m i = 0; i < s->length; i++) { \
												function_name_prefix##release_element(0, s, i); \
											} \
										} \
										MU_SET_RESULT(result, MUMA_FAILED_TO_ALLOCATE) \
										return; \
									} \
									\
									s->data = new_data; \
								} \
								\
								if (old_length < s->length) { \
									mu_memset(&s->data[old_length], 0, sizeof(type)*(s->allocated_length-old_length)); \
									\
									if (cd) { \
										for (size_m i = old_length; i < s->length; i++) { \
											on_creation(&s->data[i]); \
										} \
									} \
								} \
								\
								if (cd) { \
									for (size_m i = 0; i < s->length && i < old_length; i++) { \
										function_name_prefix##release_element(0, s, i); \
									} \
								} \
							} \
							\
							void function_name_prefix##resize(mumaResult* result, struct_name* s, size_m length) { \
								function_name_prefix##inner_resize(result, s, length, MU_TRUE); \
							} \
							\
							void function_name_prefix##inner_lshift(mumaResult* result, struct_name* s, size_m index, size_m amount, muBool cd) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								if (index >= s->length) { \
									MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
									return; \
								} \
								\
								if (amount == 0) { \
									return; \
								} \
								\
								if (amount > index) { \
									MU_SET_RESULT(result, MUMA_INVALID_SHIFT_AMOUNT) \
									return; \
								} \
								\
								if (cd) { \
									for (size_m i = 0; i < s->length; i++) { \
										function_name_prefix##hold_element(0, s, i); \
									} \
								} \
								\
								/* Dangerous; resize may fail after this */ \
								if (cd) { \
									for (size_m i = index-amount; i < index; i++) { \
										on_destruction(&s->data[i]); \
									} \
								} \
								\
								mu_memmove(&s->data[index-amount], &s->data[index], sizeof(type)*(s->length-index)); \
								\
								mumaResult res = MUMA_SUCCESS; \
								function_name_prefix##inner_resize(&res, s, s->length-amount, MU_FALSE); \
								\
								if (cd) { \
									for (size_m i = 0; i < s->length; i++) { \
										function_name_prefix##release_element(0, s, i); \
									} \
								} \
								\
								if (res != MUMA_SUCCESS) { \
									MU_SET_RESULT(result, res) \
									return; \
								} \
							} \
							\
							void function_name_prefix##lshift(mumaResult* result, struct_name* s, size_m index, size_m amount) { \
								function_name_prefix##inner_lshift(result, s, index, amount, MU_TRUE); \
							} \
							\
							void function_name_prefix##inner_rshift(mumaResult* result, struct_name* s, size_m index, size_m amount, muBool cd) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								if (index >= s->length) { \
									MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
									return; \
								} \
								\
								if (amount == 0) { \
									return; \
								} \
								\
								if (cd) { \
									for (size_m i = 0; i < s->length; i++) { \
										function_name_prefix##hold_element(0, s, i); \
									} \
								} \
								\
								mumaResult res = MUMA_SUCCESS; \
								function_name_prefix##inner_resize(&res, s, s->length+amount, MU_FALSE); \
								if (res != MUMA_SUCCESS) { \
									if (cd) { \
										for (size_m i = 0; i < s->length; i++) { \
											function_name_prefix##release_element(0, s, i); \
										} \
									} \
									MU_SET_RESULT(result, res) \
									return; \
								} \
								\
								mu_memmove(&s->data[index+amount], &s->data[index], sizeof(type)*((s->length-index)-1)); \
								\
								if (cd) { \
									for (size_m i = 0; i < index; i++) { \
										function_name_prefix##release_element(0, s, i); \
									} \
									for (size_m i = index+amount; i < s->length; i++) { \
										function_name_prefix##release_element(0, s, i); \
									} \
								} \
								\
								mu_memset(&s->data[index], 0, sizeof(type)*(amount)); \
								\
								if (cd) { \
									for (size_m i = index; i < index+amount; i++) { \
										on_creation(&s->data[i]); \
									} \
								} \
							} \
							\
							void function_name_prefix##rshift(mumaResult* result, struct_name* s, size_m index, size_m amount) { \
								function_name_prefix##inner_rshift(result, s, index, amount, MU_TRUE); \
							} \
							\
							void function_name_prefix##multiinsert(mumaResult* result, struct_name* s, size_m index, type* insert, size_m count) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								for (size_m i = 0; i < s->length; i++) { \
									function_name_prefix##hold_element(0, s, i); \
								} \
								\
								mumaResult res = MUMA_SUCCESS; \
								function_name_prefix##inner_rshift(&res, s, index, count, MU_FALSE); \
								if (res != MUMA_SUCCESS) { \
									for (size_m i = 0; i < s->length; i++) { \
										function_name_prefix##release_element(0, s, i); \
									} \
									MU_SET_RESULT(result, res) \
									return; \
								} \
								\
								mu_memcpy(&s->data[index], insert, sizeof(type)*count); \
								\
								for (size_m i = 0; i < index; i++) { \
									function_name_prefix##release_element(0, s, i); \
								} \
								for (size_m i = index+count; i < s->length; i++) { \
									function_name_prefix##release_element(0, s, i); \
								} \
								\
								for (size_m i = index; i < index+count; i++) { \
									on_creation(&s->data[i]); \
								} \
							} \
							\
							void function_name_prefix##insert(mumaResult* result, struct_name* s, size_m index, type insert) { \
								function_name_prefix##multiinsert(result, s, index, &insert, 1); \
							} \
							\
							void function_name_prefix##multierase(mumaResult* result, struct_name* s, size_m index, size_m count) { \
								function_name_prefix##lshift(result, s, index+count, count); \
							} \
							\
							void function_name_prefix##erase(mumaResult* result, struct_name* s, size_m index) { \
								function_name_prefix##multierase(result, s, index, 1); \
							} \
							void function_name_prefix##clear(mumaResult* result, struct_name* s) { \
								function_name_prefix##destroy(result, s); \
							} \
							\
							void function_name_prefix##multipush(mumaResult* result, struct_name* s, type* push, size_m count) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								size_m old_length = s->length; \
								for (size_m i = 0; i < s->length; i++) { \
									function_name_prefix##hold_element(0, s, i); \
								} \
								\
								mumaResult res = MUMA_SUCCESS; \
								function_name_prefix##inner_resize(&res, s, s->length+count, MU_FALSE); \
								if (res != MUMA_SUCCESS) { \
									for (size_m i = 0; i < s->length; i++) { \
										function_name_prefix##release_element(0, s, i); \
									} \
									MU_SET_RESULT(result, res) \
									return; \
								} \
								\
								mu_memcpy(&s->data[s->length-count], push, sizeof(type)*count); \
								\
								for (size_m i = 0; i < old_length; i++) { \
									function_name_prefix##release_element(0, s, i); \
								} \
								\
								for (size_m i = s->length-count; i < s->length; i++) { \
									on_creation(&s->data[i]); \
								} \
							} \
							\
							void function_name_prefix##push(mumaResult* result, struct_name* s, type push) { \
								function_name_prefix##multipush(result, s, &push, 1); \
							} \
							\
							void function_name_prefix##multipop(mumaResult* result, struct_name* s, size_m count) { \
								function_name_prefix##resize(result, s, s->length-count); \
							} \
							\
							void function_name_prefix##pop(mumaResult* result, struct_name* s) { \
								function_name_prefix##multipop(result, s, 1); \
							} \
							\
							size_m function_name_prefix##find(mumaResult* result, struct_name* s, type find) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								for (size_m i = 0; i < s->length; i++) { \
									function_name_prefix##hold_element(0, s, i); \
									if (type_comparison_func(find, s->data[i])) { \
										function_name_prefix##release_element(0, s, i); \
										return i; \
									} \
									function_name_prefix##release_element(0, s, i); \
								} \
								\
								MU_SET_RESULT(result, MUMA_NOT_FOUND) \
								return MU_NONE; \
							} \
							\
							void function_name_prefix##find_push(mumaResult* result, struct_name* s, type find, size_m* p_index) { \
								MU_SET_RESULT(result, MUMA_SUCCESS) \
								\
								mumaResult res = MUMA_SUCCESS; \
								size_m index = function_name_prefix##find(&res, s, find); \
								if (index != MU_NONE) { \
									if (p_index != MU_NULL_PTR) { \
										*p_index = index; \
									} \
									return; \
								} \
								if (res != MUMA_SUCCESS && res != MUMA_NOT_FOUND) { \
									if (p_index != MU_NULL_PTR) { \
										*p_index = MU_NONE; \
									} \
									MU_SET_RESULT(result, res) \
									return; \
								} \
								\
								function_name_prefix##push(&res, s, find); \
								if (res != MUMA_SUCCESS) { \
									if (p_index != MU_NULL_PTR) { \
										*p_index = MU_NONE; \
									} \
									MU_SET_RESULT(result, res) \
									return; \
								} \
								\
								if (p_index != MU_NULL_PTR) { \
									*p_index = s->length-1; \
								} \
							}

					#ifdef __cplusplus
						}
					#endif

				#endif /* MUMA_H */

			/* muMultithreading version 1.0.0 header (only if thread safe) */
			// CHANGES:
			// * Removed muma header inclusion because this library already has it.
			#ifdef MU_THREADSAFE
				#if !defined(MU_CHECK_VERSION_MISMATCHING) && defined(MUM_H) && \
					(MUM_VERSION_MAJOR != 1 || MUM_VERSION_MINOR != 0 || MUM_VERSION_PATCH != 0)

					#pragma message("[MUS] muMultithreading's header has already been defined, but version doesn't match the version that this library is built for. This may lead to errors, warnings, or unexpected behavior. Define MU_CHECK_VERSION_MISMATCHING before this to turn off this message.")

				#endif

				#ifndef MUM_H
					#define MUM_H

					#ifdef __cplusplus
					extern "C" { // }
					#endif

					#define MUM_VERSION_MAJOR 1
					#define MUM_VERSION_MINOR 0
					#define MUM_VERSION_PATCH 0

					/* Enums */

						MU_ENUM(mumResult,
							MUM_SUCCESS,

							MUM_MUMA_SUCCESS,
							MUM_MUMA_FAILED_TO_ALLOCATE,
							MUM_MUMA_INVALID_INDEX,
							MUM_MUMA_INVALID_SHIFT_AMOUNT,
							MUM_MUMA_NOT_FOUND,

							MUM_ALLOCATION_FAILED,

							MUM_ALREADY_INITIALIZED,
							MUM_ALREADY_TERMINATED,
							MUM_NOT_YET_INITIALIZED,

							MUM_CREATE_CALL_FAILED,
							MUM_DESTROY_CALL_FAILED,
							MUM_WAIT_CALL_FAILED,
							MUM_LOCK_CALL_FAILED,
							MUM_UNLOCK_CALL_FAILED,
							MUM_GET_RETURN_VALUE_CALL_FAILED,

							MUM_INVALID_ID,

							MUM_THREAD_TIMED_OUT,
							MUM_PREVIOUS_THREAD_CLOSED_BEFORE_LOCK
						)

					/* Macros */

						#define muThread size_m
						#define muMutex size_m
						#define muSpinlock size_m

						#ifdef MU_SPINLOCK
							#define muLock muSpinlock
							#define mu_lock_create mu_spinlock_create
							#define mu_lock_destroy mu_spinlock_destroy
							#define mu_lock_lock mu_spinlock_lock
							#define mu_lock_unlock mu_spinlock_unlock
						#else
							#define muLock muMutex
							#define mu_lock_create mu_mutex_create
							#define mu_lock_destroy mu_mutex_destroy
							#define mu_lock_lock mu_mutex_lock
							#define mu_lock_unlock mu_mutex_unlock
						#endif

					/* Incomplete types */

						typedef struct mumContext mumContext;

					/* Global variables */

						MUDEF mumContext* mum_global_context;

					/* Functions */

						/* Names */

							#ifdef MUM_NAMES
								MUDEF const char* mum_result_get_name(mumResult result);
							#endif

						/* Initiation/Termination */

							MUDEF void mum_init(mumResult* result);
							MUDEF void mum_term(mumResult* result);

						/* Thread */

							MUDEF muThread mu_thread_create(mumResult* result, void (*start)(void* args), void* args);
							MUDEF muThread mu_thread_destroy(mumResult* result, muThread thread);

							MUDEF void mu_thread_exit(void* ret);
							MUDEF void mu_thread_wait(mumResult* result, muThread thread);
							MUDEF void* mu_thread_get_return_value(mumResult* result, muThread thread);

						/* Mutex */

							MUDEF muMutex mu_mutex_create(mumResult* result);
							MUDEF muMutex mu_mutex_destroy(mumResult* result, muMutex mutex);

							MUDEF void mu_mutex_lock(mumResult* result, muMutex mutex);
							MUDEF void mu_mutex_unlock(mumResult* result, muMutex mutex);

						/* Spinlock */

							MUDEF muSpinlock mu_spinlock_create(mumResult* result);
							MUDEF muSpinlock mu_spinlock_destroy(mumResult* result, muSpinlock spinlock);

							MUDEF void mu_spinlock_lock(mumResult* result, muSpinlock spinlock);
							MUDEF void mu_spinlock_unlock(mumResult* result, muSpinlock spinlock);

					#ifdef __cplusplus
					}
					#endif

				#endif
			#endif

			#ifdef __cplusplus
			extern "C" { // }
			#endif

			#define MUS_VERSION_MAJOR 1
			#define MUS_VERSION_MINOR 0
			#define MUS_VERSION_PATCH 0

			/* C standard library dependencies */

				#if !defined(mu_log10) || \
					!defined(mu_floor) || \
					!defined(mu_pow)   || \
					!defined(mu_fmod)

					#include <math.h>

					#ifndef mu_log10
						#define mu_log10 log10
					#endif

					#ifndef mu_floor
						#define mu_floor floor
					#endif

					#ifndef mu_pow
						#define mu_pow pow
					#endif

					#ifndef mu_fmod
						#define mu_fmod fmod
					#endif

				#endif

			/* Incomplete types */

				typedef struct musContext musContext;

			/* Global variables */

				MUDEF musContext* mus_global_context;

			/* Macros */

				#define muCodePoint size_m
				#define muString size_m

			/* Enums */

				MU_ENUM(musResult,
					MUS_SUCCESS,

					MUS_ALREADY_INITIALIZED,
					MUS_ALLOCATION_FAILED,
					MUS_ALREADY_TERMINATED,
					MUS_NOT_YET_INITIALIZED,

					MUS_UNKNOWN_CHARACTER_ENCODING,

					MUS_CHARACTER_CANNOT_BE_ENCODED,

					MUS_INVALID_CHARACTER_ENCODING,
					MUS_INVALID_CODE_POINT_SIZE,
					MUS_INVALID_DATA_SIZE,
					MUS_INVALID_DATA_POINTER,
					MUS_INVALID_OFFSET,
					MUS_INVALID_ID,
					MUS_INVALID_DATA,

					MUS_INSUFFICIENT_DATA_SIZE,

					MUS_NONMATCHING_CHARACTER_ENCODING,

					MUS_OVERFLOW,

					MUS_MUMA_SUCCESS,
					MUS_MUMA_FAILED_TO_ALLOCATE,
					MUS_MUMA_INVALID_INDEX,
					MUS_MUMA_INVALID_SHIFT_AMOUNT,
					MUS_MUMA_NOT_FOUND
				)

				MU_ENUM(muCharacterEncoding,
					MU_UNKNOWN_CHARACTER_ENCODING,
					MU_ASCII,
					MU_UTF8,
				)

			/* Functions */

				/* Names */

					#ifdef MUS_NAMES
						MUDEF const char* mus_result_get_name(musResult result);
					#endif

				/* Initiation / Termination */

					MUDEF void mus_init(musResult* result);
					MUDEF void mus_term(musResult* result);

				/* String functions */

					/* Creation / Destruction */

						MUDEF muString mu_string_create(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size);
						MUDEF muString mu_string_destroy(musResult* result, muString string);
						MUDEF muString mu_string_duplicate(musResult* result, muString string);

					/* Data */

						MUDEF muByte* mu_string_get_raw_data(musResult* result, muString string, size_m* size);

					/* Character encoding */

						MUDEF muCharacterEncoding mu_string_get_character_encoding(musResult* result, muString string);
						MUDEF void mu_string_convert_character_encoding(musResult* result, muString string, muCharacterEncoding desired_encoding);

					/* Code point */

						MUDEF size_m mu_string_get_code_point_length(musResult* result, muString string);

						MUDEF size_m mu_string_get_code_point_offset(musResult* result, muString string, size_m offset, size_m index);

						MUDEF muCodePoint mu_string_get_code_point(musResult* result, muString string, size_m offset);
						MUDEF void mu_string_set_code_point(musResult* result, muString string, muCodePoint code_point, size_m offset);

					/* Insert */

						MUDEF void mu_string_insert_raw_string(musResult* result, muString string, muCharacterEncoding encoding, muByte* data, size_m data_size, size_m offset);

						MUDEF void mu_string_insert_integer(musResult* result, muString string, int64_m i, size_m offset);
						MUDEF void mu_string_insert_float(musResult* result, muString string, float d, size_m decimal_places, size_m offset);
						MUDEF void mu_string_insert_double(musResult* result, muString string, double d, size_m decimal_places, size_m offset);

				/* Raw string functions */

					/* Conversion */

						MUDEF size_m mu_raw_string_get_character_encoding_conversion_size(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, muCharacterEncoding desired_encoding);
						MUDEF void mu_raw_string_convert_character_encoding(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, muCharacterEncoding output_encoding, muByte* output_data, size_m output_data_size);

					/* Integer/Float/Double conversion */

						MUDEF size_m mu_raw_string_get_integer_conversion_size(musResult* result, muCharacterEncoding encoding, int64_m i);
						MUDEF void mu_raw_string_convert_to_integer(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, int64_m i);
						MUDEF int64_m mu_raw_string_get_integer(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size);

						MUDEF size_m mu_raw_string_get_float_conversion_size(musResult* result, muCharacterEncoding encoding, float d, size_m decimal_places);
						MUDEF void mu_raw_string_convert_to_float(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, float d, size_m decimal_places);
						MUDEF float mu_raw_string_get_float(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size);

						MUDEF size_m mu_raw_string_get_double_conversion_size(musResult* result, muCharacterEncoding encoding, double d, size_m decimal_places);
						MUDEF void mu_raw_string_convert_to_double(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, double d, size_m decimal_places);
						MUDEF double mu_raw_string_get_double(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size);

				/* Cross-encoding functions */

					MUDEF size_m mu_character_encoding_get_code_point_size(musResult* result, muCharacterEncoding encoding, muCodePoint code_point);
					MUDEF muCodePoint mu_character_encoding_get_code_point(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size);
					MUDEF void mu_character_encoding_set_code_point(musResult* result, muCharacterEncoding encoding, muCodePoint code_point, muByte* data, size_m data_size);

					MUDEF muBool mu_character_encoding_verify_raw_string(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size);
					MUDEF size_m mu_character_encoding_get_raw_string_code_point_length(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size);
					MUDEF size_m mu_character_encoding_get_raw_string_code_point_offset(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, size_m offset, size_m index);

				/* Encoding-specific functions */

					/* ASCII */

						MUDEF size_m mu_ASCII_get_code_point_size(musResult* result, muCodePoint code_point);
						MUDEF muCodePoint mu_ASCII_get_code_point(musResult* result, muByte* data, size_m data_size);
						MUDEF void mu_ASCII_set_code_point(musResult* result, muCodePoint code_point, muByte* data, size_m data_size);

						MUDEF muBool mu_ASCII_verify_raw_string(musResult* result, muByte* data, size_m data_size);
						MUDEF size_m mu_ASCII_get_raw_string_first_code_point_index(musResult* result, muByte* data, size_m data_size);
						MUDEF size_m mu_ASCII_get_raw_string_code_point_length(musResult* result, muByte* data, size_m data_size);
						MUDEF size_m mu_ASCII_get_raw_string_code_point_offset(musResult* result, muByte* data, size_m data_size, size_m offset, size_m index);

					/* UTF-8 */

						MUDEF size_m mu_UTF8_get_code_point_size(musResult* result, muCodePoint code_point);
						MUDEF muCodePoint mu_UTF8_get_code_point(musResult* result, muByte* data, size_m data_size);
						MUDEF void mu_UTF8_set_code_point(musResult* result, muCodePoint code_point, muByte* data, size_m data_size);

						MUDEF muBool mu_UTF8_verify_raw_string(musResult* result, muByte* data, size_m data_size);
						MUDEF size_m mu_UTF8_get_raw_string_code_point_length(musResult* result, muByte* data, size_m data_size);
						MUDEF size_m mu_UTF8_get_raw_string_code_point_offset(musResult* result, muByte* data, size_m data_size, size_m offset, size_m index);

				/* Code point */

					/* Uppercase/Lowercase */

						MUDEF muCodePoint mu_code_point_lowercase(musResult* result, muCharacterEncoding encoding, muCodePoint code_point);
						MUDEF muCodePoint mu_code_point_uppercase(musResult* result, muCharacterEncoding encoding, muCodePoint code_point);

			#ifdef __cplusplus
			}
			#endif
		#endif /* MUS_H */

	#ifdef __cplusplus
	extern "C" { // }
	#endif

	#define MUCOSA_VERSION_MAJOR 1
	#define MUCOSA_VERSION_MINOR 0
	#define MUCOSA_VERSION_PATCH 0

	/* C standard library dependencies */

		#if !defined(mu_strlen) || \
			!defined(mu_strcmp)

			#include <string.h>

			#ifndef mu_strlen
				#define mu_strlen strlen
			#endif

			#ifndef mu_strcmp
				#define mu_strcmp strcmp
			#endif

		#endif

	/* Incomplete types */

		typedef struct muCOSAContext muCOSAContext;

	/* Global variables */

		MUDEF muCOSAContext* muCOSA_global_context;

	/* Macros */

		/* Objects */

			#define muWindow size_m

		/* API choice */

			#ifndef MUCOSA_NO_AUTOMATIC_API

				#ifdef MU_UNIX
					#define MUCOSA_X11
				#endif

			#endif

			#ifdef MUCOSA_OPENGL
				#define MUCOSA_OPENGL_CALL(...) __VA_ARGS__
			#else
				#define MUCOSA_OPENGL_CALL(...)
			#endif

		/* States */

			#define muButtonState muBool
			#define MU_BUTTON_STATE_RELEASED 0
			#define MU_BUTTON_STATE_HELD 1

			#define muState muBool
			#define MU_ON 1
			#define MU_OFF 0

	/* Enums */

		MU_ENUM(muCOSAResult,
			MUCOSA_SUCCESS,

			MUCOSA_ALREADY_INITIALIZED,
			MUCOSA_ALREADY_TERMINATED,
			MUCOSA_NOT_YET_INITIALIZED,

			MUCOSA_ALLOCATION_FAILED,

			MUCOSA_UNKNOWN_WINDOW_SYSTEM,
			MUCOSA_UNKNOWN_GRAPHICS_API,
			MUCOSA_UNKNOWN_KEYBOARD_KEY,
			MUCOSA_UNKNOWN_KEYBOARD_STATE,
			MUCOSA_UNKNOWN_MOUSE_BUTTON,

			MUCOSA_UNSUPPORTED_WINDOW_SYSTEM,
			MUCOSA_UNSUPPORTED_FEATURE, // Could mean that it rather can't be (or hasn't been) implemented, or that
			// the specific thing you're trying to do won't work on this device/OS for some reason (for example, on X11,
			// an Atom needed for that task can't be found).

			MUCOSA_FAILED_CONNECTION_TO_SERVER,
			MUCOSA_FAILED_CREATE_WINDOW,
			MUCOSA_FAILED_LOAD_FUNCTIONS,
			MUCOSA_FAILED_FIND_COMPATIBLE_FRAMEBUFFER, // Not fatal
			MUCOSA_FAILED_CREATE_OPENGL_CONTEXT,
			MUCOSA_FAILED_USE_PIXEL_FORMAT, // Not fatal

			MUCOSA_INVALID_MINIMUM_MAXIMUM_BOOLS,
			MUCOSA_INVALID_MINIMUM_MAXIMUM_DIMENSIONS,
			MUCOSA_INVALID_ID,
			MUCOSA_INVALID_SAMPLE_COUNT,
			MUCOSA_INVALID_DIMENSIONS, // Often caused by width/height being outside of min/max width/height

			MUCOSA_NONEXISTENT_DEVICE, // For example, you requested the position of the cursor,
			// but there is no cursor. For whatever reason, lmao.

			MUCOSA_MUMA_SUCCESS,
			MUCOSA_MUMA_FAILED_TO_ALLOCATE,
			MUCOSA_MUMA_INVALID_INDEX,
			MUCOSA_MUMA_INVALID_SHIFT_AMOUNT,
			MUCOSA_MUMA_NOT_FOUND,
		)

		MU_ENUM(muWindowSystem,
			MU_AUTO_WINDOW_SYSTEM,
			MU_X11
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
			MU_CURSOR_STYLE_UNKNOWN,

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

	/* Struct */

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

			// @TODO Make some of these actually do it on X11
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

			void (*mouse_button_callback)(muWindow window, muMouseButton mouse_button, muButtonState state);
		};
		typedef struct muWindowCreateInfo muWindowCreateInfo;

	/* Functions */

		/* Names */

			#ifdef MUCOSA_NAMES
				MUDEF const char* muCOSA_result_get_name(muCOSAResult result);

				MUDEF const char* muCOSA_keyboard_key_get_name(muKeyboardKey key);
				MUDEF const char* muCOSA_keyboard_key_get_nice_name(muKeyboardKey key);
			#endif

		/* Initiation / Termination */

			MUDEF void muCOSA_init(muCOSAResult* result, muWindowSystem window_system);
			MUDEF void muCOSA_term(muCOSAResult* result);

		/* Window */

			/* Creation / Destruction */

				MUDEF muWindowCreateInfo mu_window_default_create_info();

				MUDEF muWindow mu_window_create(muCOSAResult* result, 
					muGraphicsAPI api, muBool (*load_functions)(void),
					muByte* name, uint16_m width, uint16_m height,
					muWindowCreateInfo create_info
				);

				MUDEF muWindow mu_window_destroy(muCOSAResult* result, muWindow window);

			/* Main loop */

				MUDEF muBool mu_window_get_closed(muCOSAResult* result, muWindow window);
				MUDEF void mu_window_close(muCOSAResult* result, muWindow window);

				// "Update" gathers input and such, should be called first and THEN frame logic
				MUDEF void mu_window_update(muCOSAResult* result, muWindow window);
				// "Swap buffers" simply puts what's been drawing on frames
				MUDEF void mu_window_swap_buffers(muCOSAResult* result, muWindow window);

			/* Get / Set */

				MUDEF muBool mu_window_get_focused(muCOSAResult* result, muWindow window);
				MUDEF void mu_window_focus(muCOSAResult* result, muWindow window);

				MUDEF muBool mu_window_get_visible(muCOSAResult* result, muWindow window);
				MUDEF void mu_window_set_visible(muCOSAResult* result, muWindow window, muBool visible);

				MUDEF void mu_window_get_position(muCOSAResult* result, muWindow window, int32_m* x, int32_m* y);
				MUDEF void mu_window_set_position(muCOSAResult* result, muWindow window, int32_m x, int32_m y);

				MUDEF void mu_window_get_dimensions(muCOSAResult* result, muWindow window, uint32_m* width, uint32_m* height);
				MUDEF void mu_window_set_dimensions(muCOSAResult* result, muWindow window, uint32_m width, uint32_m height);

				MUDEF muBool mu_window_get_maximized(muCOSAResult* result, muWindow window);
				MUDEF void mu_window_set_maximized(muCOSAResult* result, muWindow window, muBool maximized);

				MUDEF muBool mu_window_get_minimized(muCOSAResult* result, muWindow window);
				MUDEF void mu_window_set_minimized(muCOSAResult* result, muWindow window, muBool minimized);

				MUDEF void mu_window_get_minimum_dimensions(muCOSAResult* result, muWindow window, uint32_m* min_width, uint32_m* min_height);
				MUDEF void mu_window_set_minimum_dimensions(muCOSAResult* result, muWindow window, uint32_m min_width, uint32_m min_height);
				MUDEF void mu_window_get_maximum_dimensions(muCOSAResult* result, muWindow window, uint32_m* max_width, uint32_m* max_height);
				MUDEF void mu_window_set_maximum_dimensions(muCOSAResult* result, muWindow window, uint32_m max_width, uint32_m max_height);

				MUDEF void mu_window_get_cursor_position(muCOSAResult* result, muWindow window, int32_m* x, int32_m* y);
				MUDEF void mu_window_set_cursor_position(muCOSAResult* result, muWindow window, int32_m x, int32_m y);

				MUDEF muCursorStyle mu_window_get_cursor_style(muCOSAResult* result, muWindow window);
				MUDEF void mu_window_set_cursor_style(muCOSAResult* result, muWindow window, muCursorStyle style);

				MUDEF int32_m mu_window_get_scroll_level(muCOSAResult* result, muWindow window);
				MUDEF void mu_window_set_scroll_level(muCOSAResult* result, muWindow window, int32_m scroll_level);

			/* Get */

				MUDEF void mu_window_get_frame_extents(muCOSAResult* result, muWindow window, uint32_m* left, uint32_m* right, uint32_m* top, uint32_m* bottom);

				MUDEF muButtonState mu_window_get_keyboard_key_state(muCOSAResult* result, muWindow window, muKeyboardKey key);
				MUDEF muState mu_window_get_keyboard_state_state(muCOSAResult* result, muWindow window, muKeyboardState state);

				MUDEF muButtonState mu_window_get_mouse_button_state(muCOSAResult* result, muWindow window, muMouseButton button);

			/* Set */

				MUDEF void mu_window_set_title(muCOSAResult* result, muWindow window, muByte* title);

				MUDEF void mu_window_set_dimensions_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, uint32_m width, uint32_m height));
				MUDEF void mu_window_set_position_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m x, int32_m y));
				MUDEF void mu_window_set_focus_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool focused));
				MUDEF void mu_window_set_maximize_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool maximized));
				MUDEF void mu_window_set_minimize_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool minimized));

				MUDEF void mu_window_set_keyboard_key_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state));
				MUDEF void mu_window_set_keyboard_state_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardState keyboard_state, muState state));

				MUDEF void mu_window_set_mouse_button_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muMouseButton mouse_button, muButtonState state));

		/* OpenGL */

			MUDEF void mu_opengl_bind_window(muCOSAResult* result, muWindow window);

			// Note: not necessarily UTF-8, dependent on get proc address, maybe ASCII?
			MUDEF void* mu_opengl_get_function_address(const muByte* name);

	#ifdef __cplusplus
	}
	#endif
#endif /* MUCOSA_H */


#ifdef MUCOSA_IMPLEMENTATION

	/* muString commit 6678f59 implementation */

		#ifndef MUS_IMPLEMENTATION
			#define MUS_IMPLEMENTATION

			#ifdef MUS_IMPLEMENTATION

				/* muMemoryAllocator version 1.0.0 implementation */

					#ifndef MUMA_IMPLEMENTATION
						#define MUMA_IMPLEMENTATION

						#ifdef MUMA_IMPLEMENTATION

							/* muUtility version 1.0.0 implementation */

								#ifndef MUU_IMPLEMENTATION
									#define MUU_IMPLEMENTATION

									#ifdef MUU_IMPLEMENTATION
							
										// ...

									#endif /* MUU_IMPLEMENTATION */
								#endif

							#ifdef __cplusplus
								extern "C" {
							#endif

							#ifdef MUMA_NAMES
								MUDEF const char* muma_result_get_name(mumaResult result) {
									switch (result) {
										default: return "MUMA_UNKNOWN"; break;
										case MUMA_SUCCESS: return "MUMA_SUCCESS"; break;
										case MUMA_FAILED_TO_ALLOCATE: return "MUMA_FAILED_TO_ALLOCATE"; break;
										case MUMA_INVALID_INDEX: return "MUMA_INVALID_INDEX"; break;
										case MUMA_INVALID_SHIFT_AMOUNT: return "MUMA_INVALID_SHIFT_AMOUNT"; break;
										case MUMA_NOT_FOUND: return "MUMA_NOT_FOUND"; break;
									}
								}
							#endif

							#ifdef __cplusplus
								}
							#endif

						#endif /* MUMA_IMPLEMENTATION */
					#endif

				/* muMultithreading version 1.0.0 implementation (only if thread safe) */
				// CHANGES:
				// * Removed muma implementation inclusion because this library already has it.
				// * Added alt MU_LOCK definitions for non-thread safe code.

				#ifdef MU_THREADSAFE

					#ifndef MUM_IMPLEMENTATION
						#define MUM_IMPLEMENTATION

						#ifdef MUM_IMPLEMENTATION

							#ifdef __cplusplus
							extern "C" { // }
							#endif

							/* Shared functions */

								/* Names */

									#ifdef MUM_NAMES
										MUDEF const char* mum_result_get_name(mumResult result) {
											switch (result) {
												default: return "MUM_UNKNOWN"; break;
												case MUM_SUCCESS: return "MUM_SUCCESS"; break;
												case MUM_MUMA_SUCCESS: return "MUM_MUMA_SUCCESS"; break;
												case MUM_MUMA_FAILED_TO_ALLOCATE: return "MUM_MUMA_FAILED_TO_ALLOCATE"; break;
												case MUM_MUMA_INVALID_INDEX: return "MUM_MUMA_INVALID_INDEX"; break;
												case MUM_MUMA_INVALID_SHIFT_AMOUNT: return "MUM_MUMA_INVALID_SHIFT_AMOUNT"; break;
												case MUM_MUMA_NOT_FOUND: return "MUM_MUMA_NOT_FOUND"; break;
												case MUM_ALLOCATION_FAILED: return "MUM_ALLOCATION_FAILED"; break;
												case MUM_ALREADY_INITIALIZED: return "MUM_ALREADY_INITIALIZED"; break;
												case MUM_ALREADY_TERMINATED: return "MUM_ALREADY_TERMINATED"; break;
												case MUM_NOT_YET_INITIALIZED: return "MUM_NOT_YET_INITIALIZED"; break;
												case MUM_CREATE_CALL_FAILED: return "MUM_CREATE_CALL_FAILED"; break;
												case MUM_DESTROY_CALL_FAILED: return "MUM_DESTROY_CALL_FAILED"; break;
												case MUM_WAIT_CALL_FAILED: return "MUM_WAIT_CALL_FAILED"; break;
												case MUM_LOCK_CALL_FAILED: return "MUM_LOCK_CALL_FAILED"; break;
												case MUM_UNLOCK_CALL_FAILED: return "MUM_UNLOCK_CALL_FAILED"; break;
												case MUM_GET_RETURN_VALUE_CALL_FAILED: return "MUM_GET_RETURN_VALUE_CALL_FAILED"; break;
												case MUM_INVALID_ID: return "MUM_INVALID_ID"; break;
												case MUM_THREAD_TIMED_OUT: return "MUM_THREAD_TIMED_OUT"; break;
												case MUM_PREVIOUS_THREAD_CLOSED_BEFORE_LOCK: return "MUM_PREVIOUS_THREAD_CLOSED_BEFORE_LOCK"; break;
											}
										}
									#endif

								/* Get result */

									mumResult muma_result_to_mum_result(mumaResult res) {
										switch (res) {
											default: case MUMA_SUCCESS: return MUM_MUMA_SUCCESS; break;
											case MUMA_FAILED_TO_ALLOCATE: return MUM_MUMA_FAILED_TO_ALLOCATE; break;
											case MUMA_INVALID_INDEX: return MUM_MUMA_INVALID_INDEX; break;
											case MUMA_INVALID_SHIFT_AMOUNT: return MUM_MUMA_INVALID_SHIFT_AMOUNT; break;
											case MUMA_NOT_FOUND: return MUM_MUMA_NOT_FOUND; break;
										}
									}

							/* Unix */

							#ifdef MU_UNIX

								#include <pthread.h>

								/* Useful things */

									// These two functions are essentially stolen from:
									// https://github.com/stepancheg/no-mutex-c

									static inline muBool mum_atomic_compare_exchange(int* ptr, int compare, int exchange) {
										return __atomic_compare_exchange_n(ptr, &compare, exchange, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
									}

									static inline void mum_atomic_store(int* ptr, int value) {
										if (value) {}
										__atomic_store_n(ptr, 0, __ATOMIC_SEQ_CST);
									}

									#ifdef MU_THREADSAFE

										#define MU_LOCK pthread_mutex_t lock; muBool lock_active;

										#define MU_LOCK_CREATE(lock, lock_active) \
											if (pthread_mutex_init(&lock, 0) == 0) { \
												lock_active = MU_TRUE; \
											} else { \
												lock_active = MU_FALSE; \
											}

										#define MU_LOCK_DESTROY(lock, lock_active) \
											if (lock_active) { \
												if (pthread_mutex_destroy(&lock) == 0) { \
													lock_active = MU_FALSE; \
												} \
											}

										#define MU_LOCK_LOCK(lock, lock_active) \
											if (lock_active) { \
												pthread_mutex_lock(&lock); \
											}

										#define MU_LOCK_UNLOCK(lock, lock_active) \
											if (lock_active) { \
												pthread_mutex_unlock(&lock); \
											}

									#else

										#define MU_LOCK
										#define MU_LOCK_CREATE(lock, active)
										#define MU_LOCK_DESTROY(lock, active)
										#define MU_LOCK_LOCK(lock, active)
										#define MU_LOCK_UNLOCK(lock, active)

									#endif

								/* Arrays */

									/* Thread */

										struct mu_unix_thread {
											muBool active;
											pthread_t handle;
											void* ret;

											MU_LOCK
										};
										typedef struct mu_unix_thread mu_unix_thread;

										MU_HRARRAY_DEFAULT_FUNC(mu_unix_thread)

									/* Mutex */

										struct mu_unix_mutex {
											muBool active;
											pthread_mutex_t handle;

											MU_LOCK
										};
										typedef struct mu_unix_mutex mu_unix_mutex;

										MU_HRARRAY_DEFAULT_FUNC(mu_unix_mutex)

									/* Spinlock */

										struct mu_unix_spinlock {
											muBool active;
											int locked;

											MU_LOCK
										};
										typedef struct mu_unix_spinlock mu_unix_spinlock;

										MU_HRARRAY_DEFAULT_FUNC(mu_unix_spinlock)

								/* Initiation / Termination */

									/* Setup */

										struct mumContext {
											mu_unix_thread_array threads;
											#define MUM_GTHREADS mum_global_context->threads
											mu_unix_mutex_array mutexes;
											#define MUM_GMUTEXES mum_global_context->mutexes
											mu_unix_spinlock_array spinlocks;
											#define MUM_GSPINLOCKS mum_global_context->spinlocks
										};

									/* API-level */

										mumContext* mum_global_context = MU_NULL_PTR;

										MUDEF void mum_init(mumResult* result) {
											MU_SET_RESULT(result, MUM_SUCCESS)

											MU_ASSERT(mum_global_context == MU_NULL_PTR, result, MUM_ALREADY_INITIALIZED, return;)

											mum_global_context = (mumContext*)mu_malloc(sizeof(mumContext));
											MU_ASSERT(mum_global_context != 0, result, MUM_ALLOCATION_FAILED, return;)

											MUM_GTHREADS = MU_ZERO_STRUCT(mu_unix_thread_array);
											MUM_GMUTEXES = MU_ZERO_STRUCT(mu_unix_mutex_array);
											MUM_GSPINLOCKS = MU_ZERO_STRUCT(mu_unix_spinlock_array);
										}

										MUDEF void mum_term(mumResult* result) {
											MU_SET_RESULT(result, MUM_SUCCESS)

											MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_ALREADY_TERMINATED, return;)

											for (size_m i = 0; i < MUM_GTHREADS.length; i++) {
												mu_thread_destroy(0, i);
											}
											mu_unix_thread_destroy(0, &MUM_GTHREADS);

											for (size_m i = 0; i < MUM_GMUTEXES.length; i++) {
												mu_mutex_destroy(0, i);
											}
											mu_unix_mutex_destroy(0, &MUM_GMUTEXES);

											for (size_m i = 0; i < MUM_GSPINLOCKS.length; i++) {
												mu_spinlock_destroy(0, i);
											}
											mu_unix_spinlock_destroy(0, &MUM_GSPINLOCKS);

											mu_free(mum_global_context);
											mum_global_context = MU_NULL_PTR;
										}

								/* Threads */

									/* API-level */

										MUDEF muThread mu_thread_create(mumResult* result, void (*start)(void* args), void* args) {
											MU_SET_RESULT(result, MUM_SUCCESS)

											MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

											size_m thread = MU_NONE;
											mumaResult muma_res = MUMA_SUCCESS;
											mu_unix_thread_find_push(&muma_res, &MUM_GTHREADS, MU_ZERO_STRUCT(mu_unix_thread), &thread);
											MU_ASSERT(muma_res == MUMA_SUCCESS && thread != MU_NONE, result, muma_result_to_mum_result(muma_res), 
												return MU_NONE;
											)

											mu_unix_thread_hold_element(0, &MUM_GTHREADS, thread);
											void* (*func)(void*);
											mu_memcpy(&func, &start, sizeof(void*));
											if (pthread_create(&MUM_GTHREADS.data[thread].handle, 0, func, args) != 0) {
												MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_)
												MU_SET_RESULT(result, MUM_CREATE_CALL_FAILED)
												return MU_NONE;
											}

											MUM_GTHREADS.data[thread].active = MU_TRUE;
											MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_)
											return thread;
										}

										MUDEF muThread mu_thread_destroy(mumResult* result, muThread thread) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return thread;)
											MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return thread;, mu_unix_thread_)

											MU_ASSERT(pthread_cancel(MUM_GTHREADS.data[thread].handle) == 0, result, MUM_DESTROY_CALL_FAILED, 
												MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_) return thread;
											)
											MUM_GTHREADS.data[thread].active = MU_FALSE;

											MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_)
											return MU_NONE;
										}

										MUDEF void mu_thread_exit(void* ret) {
											pthread_exit(ret);
										}

										MUDEF void mu_thread_wait(mumResult* result, muThread thread) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
											MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return;, mu_unix_thread_)

											MU_ASSERT(pthread_join(MUM_GTHREADS.data[thread].handle, &MUM_GTHREADS.data[thread].ret) == 0, result, MUM_WAIT_CALL_FAILED,
												MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_) return;
											)
											MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_)
										}

										MUDEF void* mu_thread_get_return_value(mumResult* result, muThread thread) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return MU_NULL_PTR;)
											MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return MU_NULL_PTR;, mu_unix_thread_)

											void* ret = MUM_GTHREADS.data[thread].ret;
											MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_)
											return ret;
										}

								/* Mutexes */

									/* API-level */

										MUDEF muMutex mu_mutex_create(mumResult* result) {
											MU_SET_RESULT(result, MUM_SUCCESS)
											MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

											size_m mutex = MU_NONE;
											mumaResult muma_res = MUMA_SUCCESS;
											mu_unix_mutex_find_push(&muma_res, &MUM_GMUTEXES, MU_ZERO_STRUCT(mu_unix_mutex), &mutex);
											MU_ASSERT(muma_res == MUMA_SUCCESS && mutex != MU_NONE, result, muma_result_to_mum_result(muma_res),
												return MU_NONE;
											)

											mu_unix_mutex_hold_element(0, &MUM_GMUTEXES, mutex);
											if (pthread_mutex_init(&MUM_GMUTEXES.data[mutex].handle, 0) != 0) {
												MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_)
												MU_SET_RESULT(result, MUM_CREATE_CALL_FAILED)
												return MU_NONE;
											}

											MUM_GMUTEXES.data[mutex].active = MU_TRUE;
											MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_)
											return mutex;
										}

										MUDEF muMutex mu_mutex_destroy(mumResult* result, muMutex mutex) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return mutex;)
											MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return mutex;, mu_unix_mutex_)

											MU_ASSERT(pthread_mutex_destroy(&MUM_GMUTEXES.data[mutex].handle) == 0, result, MUM_DESTROY_CALL_FAILED, 
												MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_) return mutex;
											)
											MUM_GMUTEXES.data[mutex].active = MU_FALSE;
											MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_)
											return MU_NONE;
										}

										// This poses an odd structural challenge. If we make both the lock & unlock
										// function hold the mutex, you reach a deadlock pretty easily, ie:

										// * Thread 1 locks mutex, quickly holding and releasing it.
										// * Thread 2 attempts to lock mutex, holding it and waiting for it to be unlocked,
										// * Thread 1 is done, and attempts to unlock the mutex, so it tries to hold it,
										// but it's still being held by thread 2 who is attempting to lock it, so you reach
										// a deadlock.

										// This can be solved by:
										// A) Making the 'lock mutex' function release the function early, allowing it to
										// wait to lock it without holding it during that wait time.
										// B) Make the 'unlock mutex' function not hold/release the mutex.

										// I've chosen A because the only way it *doesn't* work well is if the user deletes
										// a mutex while a thread is running with it locked AND another one is waiting on
										// it, which is already a horrible situation to be in. Pretty sure deleting a mutex
										// with a thread having it locked is already undefined behavior anyway.

										// Note that this same logic applies to the other locks.

										MUDEF void mu_mutex_lock(mumResult* result, muMutex mutex) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
											MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return;, mu_unix_mutex_)

											MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_)
											MU_ASSERT(pthread_mutex_lock(&MUM_GMUTEXES.data[mutex].handle) == 0, result, MUM_LOCK_CALL_FAILED, 
												return;
											)
										}

										MUDEF void mu_mutex_unlock(mumResult* result, muMutex mutex) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
											MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return;, mu_unix_mutex_)

											MU_ASSERT(pthread_mutex_unlock(&MUM_GMUTEXES.data[mutex].handle) == 0, result, MUM_UNLOCK_CALL_FAILED, 
												MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_) return;
											)
											MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_)
										}

								/* Spinlocks */

									/* API-level */

										MUDEF muSpinlock mu_spinlock_create(mumResult* result) {
											MU_SET_RESULT(result, MUM_SUCCESS)
											MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

											size_m spinlock = MU_NONE;
											mumaResult muma_res = MUMA_SUCCESS;
											mu_unix_spinlock_find_push(&muma_res, &MUM_GSPINLOCKS, MU_ZERO_STRUCT(mu_unix_spinlock), &spinlock);
											MU_ASSERT(muma_res == MUMA_SUCCESS && spinlock != MU_NONE, result, muma_result_to_mum_result(muma_res),
												return MU_NONE;
											)

											mu_unix_spinlock_hold_element(0, &MUM_GSPINLOCKS, spinlock);
											MUM_GSPINLOCKS.data[spinlock].locked = 0;

											MUM_GSPINLOCKS.data[spinlock].active = MU_TRUE;
											MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_unix_spinlock_)
											return spinlock;
										}

										MUDEF muSpinlock mu_spinlock_destroy(mumResult* result, muSpinlock spinlock) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return spinlock;)
											MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return spinlock;, mu_unix_spinlock_)

											MUM_GSPINLOCKS.data[spinlock].locked = 0;
											MUM_GSPINLOCKS.data[spinlock].active = MU_FALSE;
											MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_unix_spinlock_)
											return MU_NONE;
										}

										MUDEF void mu_spinlock_lock(mumResult* result, muSpinlock spinlock) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
											MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return;, mu_unix_spinlock_)

											MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_unix_spinlock_)
											// Note that this is a bit less safe than mutexes, but there's little I can do
											// in this position.
											while (!mum_atomic_compare_exchange(&MUM_GSPINLOCKS.data[spinlock].locked, 0, 1)) {}
										}

										MUDEF void mu_spinlock_unlock(mumResult* result, muSpinlock spinlock) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
											MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return;, mu_unix_spinlock_)

											mum_atomic_store(&MUM_GSPINLOCKS.data[spinlock].locked, 0);
											MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_unix_spinlock_)
										}

							#endif /* MU_UNIX */

							#ifdef MU_WIN32

								#include <windows.h>

								/* Useful things */

									static inline muBool mum_atomic_compare_exchange(LONG volatile* ptr, LONG compare, LONG exchange) {
										return InterlockedCompareExchange(ptr, exchange, compare) != exchange;
									}

									// Kind of a hack
									static inline void mum_atomic_store(LONG volatile* ptr, long value) {
										if (value == 0) {
											_interlockedbittestandreset(ptr, 0);
										} else {
											_interlockedbittestandset(ptr, 0);
										}
									}

									#ifdef MU_THREADSAFE

										#define MU_LOCK HANDLE lock; muBool lock_active;

										#define MU_LOCK_CREATE(lock, lock_active) \
											lock = CreateMutex(0, MU_FALSE, 0); \
											if (lock != 0) { \
												lock_active = MU_TRUE; \
											} else { \
												lock_active = MU_FALSE; \
											}

										#define MU_LOCK_DESTROY(lock, lock_active) \
											if (lock_active) { \
												if (CloseHandle(lock) != 0) { \
													lock_active = MU_FALSE; \
												} \
											}

										#define MU_LOCK_LOCK(lock, lock_active) \
											if (lock_active) { \
												WaitForSingleObject(lock, INFINITE); \
											}

										#define MU_LOCK_UNLOCK(lock, lock_active) \
											if (lock_active) { \
												ReleaseMutex(lock); \
											}

									#else

										#define MU_LOCK
										#define MU_LOCK_CREATE(lock, active)
										#define MU_LOCK_DESTROY(lock, active)
										#define MU_LOCK_LOCK(lock, active)
										#define MU_LOCK_UNLOCK(lock, active)

									#endif

								/* Arrays */

									/* Thread */

										struct mu_win32_thread {
											muBool active;
											HANDLE handle;

											MU_LOCK
										};
										typedef struct mu_win32_thread mu_win32_thread;

										MU_HRARRAY_DEFAULT_FUNC(mu_win32_thread)

									/* Mutex */

										struct mu_win32_mutex {
											muBool active;
											HANDLE handle;

											MU_LOCK
										};
										typedef struct mu_win32_mutex mu_win32_mutex;

										MU_HRARRAY_DEFAULT_FUNC(mu_win32_mutex)

									/* Spinlock */

										struct mu_win32_spinlock {
											muBool active;
											LONG volatile locked;

											MU_LOCK
										};
										typedef struct mu_win32_spinlock mu_win32_spinlock;

										MU_HRARRAY_DEFAULT_FUNC(mu_win32_spinlock)

								/* Initiation / Termination */

									/* Setup */

										struct mumContext {
											mu_win32_thread_array threads;
											#define MUM_GTHREADS mum_global_context->threads
											mu_win32_mutex_array mutexes;
											#define MUM_GMUTEXES mum_global_context->mutexes
											mu_win32_spinlock_array spinlocks;
											#define MUM_GSPINLOCKS mum_global_context->spinlocks
										};

									/* API-level */

										mumContext* mum_global_context = MU_NULL_PTR;

										MUDEF void mum_init(mumResult* result) {
											MU_SET_RESULT(result, MUM_SUCCESS)

											MU_ASSERT(mum_global_context == MU_NULL_PTR, result, MUM_ALREADY_INITIALIZED, return;)

											mum_global_context = (mumContext*)mu_malloc(sizeof(mumContext));
											MU_ASSERT(mum_global_context != 0, result, MUM_ALLOCATION_FAILED, return;)

											MUM_GTHREADS = MU_ZERO_STRUCT(mu_win32_thread_array);
											MUM_GMUTEXES = MU_ZERO_STRUCT(mu_win32_mutex_array);
											MUM_GSPINLOCKS = MU_ZERO_STRUCT(mu_win32_spinlock_array);
										}

										MUDEF void mum_term(mumResult* result) {
											MU_SET_RESULT(result, MUM_SUCCESS)

											MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_ALREADY_TERMINATED, return;)

											for (size_m i = 0; i < MUM_GTHREADS.length; i++) {
												mu_thread_destroy(0, i);
											}
											mu_win32_thread_destroy(0, &MUM_GTHREADS);

											for (size_m i = 0; i < MUM_GMUTEXES.length; i++) {
												mu_mutex_destroy(0, i);
											}
											mu_win32_mutex_destroy(0, &MUM_GMUTEXES);

											for (size_m i = 0; i < MUM_GSPINLOCKS.length; i++) {
												mu_spinlock_destroy(0, i);
											}
											mu_win32_spinlock_destroy(0, &MUM_GSPINLOCKS);

											mu_free(mum_global_context);
											mum_global_context = MU_NULL_PTR;
										}

								/* Threads */

									/* API-level */

										MUDEF muThread mu_thread_create(mumResult* result, void (*start)(void* args), void* args) {
											MU_SET_RESULT(result, MUM_SUCCESS)

											MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

											size_m thread = MU_NONE;
											mumaResult muma_res = MUMA_SUCCESS;
											mu_win32_thread_find_push(&muma_res, &MUM_GTHREADS, MU_ZERO_STRUCT(mu_win32_thread), &thread);
											MU_ASSERT(muma_res == MUMA_SUCCESS && thread != MU_NONE, result, muma_result_to_mum_result(muma_res), 
												return MU_NONE;
											)

											mu_win32_thread_hold_element(0, &MUM_GTHREADS, thread);
											LPTHREAD_START_ROUTINE lp_start;
											mu_memcpy(&lp_start, &start, sizeof(void*));
											DWORD id;
											MUM_GTHREADS.data[thread].handle = CreateThread(0, 0, lp_start, args, 0, &id);
											if (MUM_GTHREADS.data[thread].handle == 0) {
												MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_);
												MU_SET_RESULT(result, MUM_CREATE_CALL_FAILED)
												return MU_NONE;
											}

											MUM_GTHREADS.data[thread].active = MU_TRUE;
											MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_)
											return thread;
										}

										MUDEF muThread mu_thread_destroy(mumResult* result, muThread thread) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return thread;)
											MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return thread;, mu_win32_thread_)

											MU_ASSERT(CloseHandle(MUM_GTHREADS.data[thread].handle) == 0, result, MUM_DESTROY_CALL_FAILED, 
												MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_) return thread;
											)
											MUM_GTHREADS.data[thread].active = MU_FALSE;

											MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_)
											return MU_NONE;
										}

										MUDEF void mu_thread_exit(void* ret) {
											DWORD d;
											mu_memcpy(&d, &ret, sizeof(DWORD));
											ExitThread(d);
										}

										MUDEF void mu_thread_wait(mumResult* result, muThread thread) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
											MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return;, mu_win32_thread_)

											DWORD wait_result = WaitForSingleObject(MUM_GTHREADS.data[thread].handle, INFINITE);

											switch (wait_result) {
												case WAIT_TIMEOUT: {
													MU_SET_RESULT(result, MUM_THREAD_TIMED_OUT)
												} break;

												case WAIT_FAILED: {
													MU_SET_RESULT(result, MUM_WAIT_CALL_FAILED)
												} break;
											}

											MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_)
										}

										MUDEF void* mu_thread_get_return_value(mumResult* result, muThread thread) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return MU_NULL_PTR;)
											MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return MU_NULL_PTR;, mu_win32_thread_)

											DWORD exit_code = 0;
											MU_ASSERT(GetExitCodeThread(MUM_GTHREADS.data[thread].handle, &exit_code) != 0, result, MUM_GET_RETURN_VALUE_CALL_FAILED, 
												MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_) return MU_NULL_PTR;
											)

											void* p;
											mu_memcpy(&p, &exit_code, sizeof(DWORD));

											MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_)
											return p;
										}

								/* Mutexes */

									/* API-level */

										MUDEF muMutex mu_mutex_create(mumResult* result) {
											MU_SET_RESULT(result, MUM_SUCCESS)
											MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

											size_m mutex = MU_NONE;
											mumaResult muma_res = MUMA_SUCCESS;
											mu_win32_mutex_find_push(&muma_res, &MUM_GMUTEXES, MU_ZERO_STRUCT(mu_win32_mutex), &mutex);
											MU_ASSERT(muma_res == MUMA_SUCCESS && mutex != MU_NONE, result, muma_result_to_mum_result(muma_res),
												return MU_NONE;
											)

											mu_win32_mutex_hold_element(0, &MUM_GMUTEXES, mutex);
											MUM_GMUTEXES.data[mutex].handle = CreateMutex(0, MU_FALSE, 0);
											if (MUM_GMUTEXES.data[mutex].handle == 0) {
												MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_)
												MU_SET_RESULT(result, MUM_CREATE_CALL_FAILED)
												return MU_NONE;
											}

											MUM_GMUTEXES.data[mutex].active = MU_TRUE;
											MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_)
											return mutex;
										}

										MUDEF muMutex mu_mutex_destroy(mumResult* result, muMutex mutex) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return mutex;)
											MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return mutex;, mu_win32_mutex_)

											MU_ASSERT(CloseHandle(MUM_GMUTEXES.data[mutex].handle) == 0, result, MUM_DESTROY_CALL_FAILED, 
												MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_) return mutex;
											)
											MUM_GMUTEXES.data[mutex].active = MU_FALSE;
											MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_)
											return MU_NONE;
										}

										MUDEF void mu_mutex_lock(mumResult* result, muMutex mutex) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
											MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return;, mu_win32_mutex_)

											MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_)
											DWORD wait_result = WaitForSingleObject(MUM_GMUTEXES.data[mutex].handle, INFINITE);

											switch (wait_result) {
												// The mutex has most likely been closed. This should pretty much never happen with
												// the way mum is set up, but if it has, that's really bad. Most likely, rather mum
												// is not working as intended at all, or the user has modified things that they
												// shouldn't.
												case WAIT_FAILED: {
													MU_SET_RESULT(result, MUM_INVALID_ID)
												} break;

												// The thread holding the mutex has died. This can be due to a few things:
												// * The thread crashed or otherwise imploded in on itself.
												// * I forgot to put an unlock call on an error return case.
												// * The user has fiddled around with values they shouldn't.
												// Either way, this is REALLY, REALLY bad, and will lead to sporadic random bugs &
												// crashes.
												// Note: we still have ownership due to this, but ehhhhh.
												// https://devblogs.microsoft.com/oldnewthing/20050912-14/?p=34253
												// (Raymond Chen is awesome btw)
												case WAIT_ABANDONED: {
													MU_SET_RESULT(result, MUM_PREVIOUS_THREAD_CLOSED_BEFORE_LOCK)
												} break;
											}
										}

										MUDEF void mu_mutex_unlock(mumResult* result, muMutex mutex) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
											MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return;, mu_win32_mutex_)

											MU_ASSERT(ReleaseMutex(MUM_GMUTEXES.data[mutex].handle), result, MUM_UNLOCK_CALL_FAILED, 
												MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_) return;
											)
											MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_)
										}

								/* Spinlocks */

									/* API-level */

										MUDEF muSpinlock mu_spinlock_create(mumResult* result) {
											MU_SET_RESULT(result, MUM_SUCCESS)
											MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

											size_m spinlock = MU_NONE;
											mumaResult muma_res = MUMA_SUCCESS;
											mu_win32_spinlock_find_push(&muma_res, &MUM_GSPINLOCKS, MU_ZERO_STRUCT(mu_win32_spinlock), &spinlock);
											MU_ASSERT(muma_res == MUMA_SUCCESS && spinlock != MU_NONE, result, muma_result_to_mum_result(muma_res),
												return MU_NONE;
											)

											mu_win32_spinlock_hold_element(0, &MUM_GSPINLOCKS, spinlock);
											MUM_GSPINLOCKS.data[spinlock].locked = 0;

											MUM_GSPINLOCKS.data[spinlock].active = MU_TRUE;
											MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_win32_spinlock_)
											return spinlock;
										}

										MUDEF muSpinlock mu_spinlock_destroy(mumResult* result, muSpinlock spinlock) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return spinlock;)
											MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return spinlock;, mu_win32_spinlock_)

											MUM_GSPINLOCKS.data[spinlock].locked = 0;
											MUM_GSPINLOCKS.data[spinlock].active = MU_FALSE;
											MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_win32_spinlock_)
											return MU_NONE;
										}

										MUDEF void mu_spinlock_lock(mumResult* result, muSpinlock spinlock) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
											MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return;, mu_win32_spinlock_)

											MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_win32_spinlock_)
											while (!mum_atomic_compare_exchange(&MUM_GSPINLOCKS.data[spinlock].locked, 0, 1)) {}
										}

										MUDEF void mu_spinlock_unlock(mumResult* result, muSpinlock spinlock) {
											MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
											MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return;, mu_win32_spinlock_)

											mum_atomic_store(&MUM_GSPINLOCKS.data[spinlock].locked, 0);
											MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_win32_spinlock_)
										}

							#endif /* MU_WIN32 */

							#ifdef __cplusplus
							}
							#endif

						#endif /* MUM_IMPLEMENTATION */
					#endif

				#else

					#define MU_LOCK
					#define MU_LOCK_CREATE(...)
					#define MU_LOCK_DESTROY(...)
					#define MU_LOCK_LOCK(...)
					#define MU_LOCK_UNLOCK(...)

				#endif

				#ifdef __cplusplus
				extern "C" { // }
				#endif

				/* Result conversion */

					musResult muma_result_to_mus_result(mumaResult res) {
						switch (res) {
							default: case MUMA_SUCCESS: return MUS_MUMA_SUCCESS; break;
							case MUMA_FAILED_TO_ALLOCATE: return MUS_MUMA_FAILED_TO_ALLOCATE; break;
							case MUMA_INVALID_INDEX: return MUS_MUMA_INVALID_INDEX; break;
							case MUMA_INVALID_SHIFT_AMOUNT: return MUS_MUMA_INVALID_SHIFT_AMOUNT; break;
							case MUMA_NOT_FOUND: return MUS_MUMA_NOT_FOUND; break;
						}
					}

				/* Functions */

					/* Names */

						#ifdef MUS_NAMES
							MUDEF const char* mus_result_get_name(musResult result) {
								switch (result) {
									default: return "MUS_UNKNOWN"; break;
									case MUS_SUCCESS: return "MUS_SUCCESS"; break;
									case MUS_ALREADY_INITIALIZED: return "MUS_ALREADY_INITIALIZED"; break;
									case MUS_ALLOCATION_FAILED: return "MUS_ALLOCATION_FAILED"; break;
									case MUS_ALREADY_TERMINATED: return "MUS_ALREADY_TERMINATED"; break;
									case MUS_NOT_YET_INITIALIZED: return "MUS_NOT_YET_INITIALIZED"; break;
									case MUS_UNKNOWN_CHARACTER_ENCODING: return "MUS_UNKNOWN_CHARACTER_ENCODING"; break;
									case MUS_CHARACTER_CANNOT_BE_ENCODED: return "MUS_CHARACTER_CANNOT_BE_ENCODED"; break;
									case MUS_INVALID_CHARACTER_ENCODING: return "MUS_INVALID_CHARACTER_ENCODING"; break;
									case MUS_INVALID_CODE_POINT_SIZE: return "MUS_INVALID_CODE_POINT_SIZE"; break;
									case MUS_INVALID_DATA_SIZE: return "MUS_INVALID_DATA_SIZE"; break;
									case MUS_INVALID_DATA_POINTER: return "MUS_INVALID_DATA_POINTER"; break;
									case MUS_INVALID_OFFSET: return "MUS_INVALID_OFFSET"; break;
									case MUS_INVALID_ID: return "MUS_INVALID_ID"; break;
									case MUS_INVALID_DATA: return "MUS_INVALID_DATA"; break;
									case MUS_INSUFFICIENT_DATA_SIZE: return "MUS_INSUFFICIENT_DATA_SIZE"; break;
									case MUS_NONMATCHING_CHARACTER_ENCODING: return "MUS_NONMATCHING_CHARACTER_ENCODING"; break;
									case MUS_MUMA_FAILED_TO_ALLOCATE: return "MUS_MUMA_FAILED_TO_ALLOCATE"; break;
									case MUS_MUMA_INVALID_INDEX: return "MUS_MUMA_INVALID_INDEX"; break;
									case MUS_MUMA_INVALID_SHIFT_AMOUNT: return "MUS_MUMA_INVALID_SHIFT_AMOUNT"; break;
									case MUS_MUMA_NOT_FOUND: return "MUS_MUMA_NOT_FOUND"; break;
								}
							}
						#endif

					/* Structs */

						muBool mu_bstring_comp(muByte b0, muByte b1) {
							return b0 == b1;
						}

						mu_dynamic_array_declaration(mus_byte_string, muByte, mus_bstring_, mu_bstring_comp)

						struct mus_string {
							muBool active;

							muCharacterEncoding encoding;
							mus_byte_string bytes;
							size_m code_point_length;

							MU_LOCK
						};
						typedef struct mus_string mus_string;

						MU_HRARRAY_DEFAULT_FUNC(mus_string)

					/* Initiation / Termination */

						struct musContext {
							mus_string_array strings;
							#define MUS_GSTRINGS mus_global_context->strings
						};

						musContext* mus_global_context = MU_NULL_PTR;

						MUDEF void mus_init(musResult* result) {
							MU_SET_RESULT(result, MUS_SUCCESS)

							MU_ASSERT(mus_global_context == MU_NULL_PTR, result, MUS_ALREADY_INITIALIZED, return;)

							mus_global_context = (musContext*)mu_malloc(sizeof(musContext));
							MU_ASSERT(mus_global_context != 0, result, MUS_ALLOCATION_FAILED, return;)

							MUS_GSTRINGS = MU_ZERO_STRUCT(mus_string_array);
						}

						MUDEF void mus_term(musResult* result) {
							MU_SET_RESULT(result, MUS_SUCCESS)
							MU_ASSERT(mus_global_context != MU_NULL_PTR, result, MUS_ALREADY_TERMINATED, return;)

							for (size_m i = 0; i < MUS_GSTRINGS.length; i++) {
								mu_string_destroy(0, i);
							}
							mus_string_destroy(0, &MUS_GSTRINGS);

							mu_free(mus_global_context);
							mus_global_context = MU_NULL_PTR;
						}

					/* String functions */

						/* Creation/Destruction */

							MUDEF muString mu_string_create(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size) {
								MU_SAFEFUNC(result, MUS_, mus_global_context, return MU_NONE;)
								mumaResult muma_res = MUMA_SUCCESS;
								musResult res = MUS_SUCCESS;
								
								MU_ASSERT(mu_character_encoding_verify_raw_string(&res, encoding, data, data_size) == MU_TRUE, result, MUS_INVALID_CHARACTER_ENCODING, return MU_NONE;)
								MU_ASSERT(res == MUS_SUCCESS, result, res, return MU_NONE;)

								mus_string s_string = MU_ZERO_STRUCT(mus_string);
								s_string.active = MU_TRUE;

								s_string.encoding = encoding;
								s_string.code_point_length = mu_character_encoding_get_raw_string_code_point_length(&res, encoding, data, data_size);
								MU_ASSERT(res == MUS_SUCCESS, result, res, return MU_NONE;)
								
								s_string.bytes = mus_bstring_create(&muma_res, data_size);
								MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), return MU_NONE;)
								mu_memcpy(s_string.bytes.data, data, data_size);

								size_m string = MU_NONE;
								mus_string_find_push(&muma_res, &MUS_GSTRINGS, MU_ZERO_STRUCT(mus_string), &string);
								MU_ASSERT(muma_res == MUMA_SUCCESS && string != MU_NONE, result, muma_result_to_mus_result(muma_res), 
									mus_bstring_destroy(0, s_string.bytes);
								)

								mus_string_hold_element(0, &MUS_GSTRINGS, string);
								MUS_GSTRINGS.data[string] = s_string;
								MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
								return string;
							}

							MUDEF muString mu_string_destroy(musResult* result, muString string) {
								MU_SAFEFUNC(result, MUS_, mus_global_context, return string;)
								MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return string;, mus_string_)
								mumaResult muma_res = MUMA_SUCCESS;

								mus_bstring_destroy(&muma_res, MUS_GSTRINGS.data[string].bytes);
								MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return string;)
								MUS_GSTRINGS.data[string].active = MU_FALSE;

								MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
								return MU_NONE;
							}

							MUDEF muString mu_string_duplicate(musResult* result, muString string) {
								MU_SAFEFUNC(result, MUS_, mus_global_context, return MU_NONE;)
								MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return MU_NONE;, mus_string_)
								musResult mus_res = MUS_SUCCESS;

								muString dupe = mu_string_create(&mus_res, 
									MUS_GSTRINGS.data[string].encoding,
									MUS_GSTRINGS.data[string].bytes.data,
									MUS_GSTRINGS.data[string].bytes.length
								);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return MU_NONE;)

								MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
								return dupe;
							}

						/* Data */

							MUDEF muByte* mu_string_get_raw_data(musResult* result, muString string, size_m* size) {
								MU_SAFEFUNC(result, MUS_, mus_global_context, return MU_NULL_PTR;)
								MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return MU_NULL_PTR;, mus_string_)

								muByte* data = MUS_GSTRINGS.data[string].bytes.data;
								MU_SET_RESULT(size, MUS_GSTRINGS.data[string].bytes.length)

								MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
								return data;
							}

						/* Character encoding */

							MUDEF muCharacterEncoding mu_string_get_character_encoding(musResult* result, muString string) {
								MU_SAFEFUNC(result, MUS_, mus_global_context, return MU_UNKNOWN_CHARACTER_ENCODING;)
								MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return MU_UNKNOWN_CHARACTER_ENCODING;, mus_string_)

								muCharacterEncoding encoding = MUS_GSTRINGS.data[string].encoding;

								MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
								return encoding;
							}

							MUDEF void mu_string_convert_character_encoding(musResult* result, muString string, muCharacterEncoding desired_encoding) {
								MU_SAFEFUNC(result, MUS_, mus_global_context, return;)
								MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return;, mus_string_)
								musResult mus_res = MUS_SUCCESS;
								mumaResult muma_res = MUMA_SUCCESS;

								size_m new_size = mu_raw_string_get_character_encoding_conversion_size(&mus_res, 
									MUS_GSTRINGS.data[string].encoding,
									MUS_GSTRINGS.data[string].bytes.data,
									MUS_GSTRINGS.data[string].bytes.length,
									desired_encoding
								);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)
								if (MUS_GSTRINGS.data[string].encoding == desired_encoding) {
									return;
								}

								mus_byte_string new_bytes = mus_bstring_create(&muma_res, new_size);
								MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								mu_raw_string_convert_character_encoding(&mus_res, 
									MUS_GSTRINGS.data[string].encoding,
									MUS_GSTRINGS.data[string].bytes.data,
									MUS_GSTRINGS.data[string].bytes.length,
									desired_encoding,
									new_bytes.data,
									new_bytes.length
								);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, mus_bstring_destroy(0, new_bytes); MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								size_m new_code_point_length = mu_character_encoding_get_raw_string_code_point_length(&mus_res,
									desired_encoding, new_bytes.data, new_bytes.length
								);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, mus_bstring_destroy(0, new_bytes); MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								mus_bstring_destroy(0, MUS_GSTRINGS.data[string].bytes);
								MUS_GSTRINGS.data[string].bytes = new_bytes;
								MUS_GSTRINGS.data[string].encoding = desired_encoding;
								MUS_GSTRINGS.data[string].code_point_length = new_code_point_length;

								MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
							}

						/* Code point */

							MUDEF size_m mu_string_get_code_point_length(musResult* result, muString string) {
								MU_SAFEFUNC(result, MUS_, mus_global_context, return 0;)
								MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return 0;, mus_string_)

								size_m code_point_length = MUS_GSTRINGS.data[string].code_point_length;

								MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
								return code_point_length;
							}

							MUDEF size_m mu_string_get_code_point_offset(musResult* result, muString string, size_m offset, size_m index) {
								MU_SAFEFUNC(result, MUS_, mus_global_context, return 0;)
								MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return 0;, mus_string_)
								musResult mus_res = MUS_SUCCESS;

								size_m ret_offset = mu_character_encoding_get_raw_string_code_point_offset(&mus_res,
									MUS_GSTRINGS.data[string].encoding,
									MUS_GSTRINGS.data[string].bytes.data,
									MUS_GSTRINGS.data[string].bytes.length,
									offset, index
								);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return 0;)

								MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
								return ret_offset;
							}

							MUDEF muCodePoint mu_string_get_code_point(musResult* result, muString string, size_m offset) {
								MU_SAFEFUNC(result, MUS_, mus_global_context, return 0;)
								MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return 0;, mus_string_)
								musResult mus_res = MUS_SUCCESS;

								MU_ASSERT(offset < MUS_GSTRINGS.data[string].bytes.length, result, MUS_INVALID_OFFSET, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return 0;)

								muCodePoint cp = mu_character_encoding_get_code_point(&mus_res, 
									MUS_GSTRINGS.data[string].encoding,
									&MUS_GSTRINGS.data[string].bytes.data[offset],
									MUS_GSTRINGS.data[string].bytes.length-offset
								);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return 0;)

								MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
								return cp;
							}

							MUDEF void mu_string_set_code_point(musResult* result, muString string, muCodePoint code_point, size_m offset) {
								MU_SAFEFUNC(result, MUS_, mus_global_context, return;)
								MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return;, mus_string_)
								musResult mus_res = MUS_SUCCESS;
								mumaResult muma_res = MUMA_SUCCESS;

								MU_ASSERT(offset < MUS_GSTRINGS.data[string].bytes.length, result, MUS_INVALID_OFFSET, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								// Get sizes of current code point and the code point to be set

								muCodePoint prev_cp = mu_character_encoding_get_code_point(&mus_res, 
									MUS_GSTRINGS.data[string].encoding,
									&MUS_GSTRINGS.data[string].bytes.data[offset],
									MUS_GSTRINGS.data[string].bytes.length-offset
								);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								size_m prev_cp_size = mu_character_encoding_get_code_point_size(&mus_res, MUS_GSTRINGS.data[string].encoding, prev_cp);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)
								size_m cp_size = mu_character_encoding_get_code_point_size(&mus_res, MUS_GSTRINGS.data[string].encoding, code_point);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								// Compare sizes and shift data accordingly

								if (prev_cp_size > cp_size) {
									MUS_GSTRINGS.data[string].bytes = mus_bstring_lshift(&muma_res, 
										MUS_GSTRINGS.data[string].bytes,
										offset + prev_cp_size,
										prev_cp_size - cp_size
									);
									MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)
								} else if (prev_cp_size < cp_size) {
									MUS_GSTRINGS.data[string].bytes = mus_bstring_rshift(&muma_res,
										MUS_GSTRINGS.data[string].bytes,
										offset + prev_cp_size,
										cp_size - prev_cp_size
									);
									MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)
								}

								// Set code point

								mu_character_encoding_set_code_point(&mus_res, 
									MUS_GSTRINGS.data[string].encoding,
									code_point,
									&MUS_GSTRINGS.data[string].bytes.data[offset],
									MUS_GSTRINGS.data[string].bytes.length-offset
								);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
							}

						/* Insert */

							MUDEF void mu_string_insert_raw_string(musResult* result, muString string, muCharacterEncoding encoding, muByte* data, size_m data_size, size_m offset) {
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)
								MU_SAFEFUNC(result, MUS_, mus_global_context, return;)
								MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return;, mus_string_)

								mumaResult muma_res = MUMA_SUCCESS;
								musResult mus_res = MUS_SUCCESS;

								MU_ASSERT(encoding == MUS_GSTRINGS.data[string].encoding, result, MUS_NONMATCHING_CHARACTER_ENCODING, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								size_m first_offset = mu_character_encoding_get_raw_string_code_point_offset(&mus_res, encoding, data, data_size, 0, 0);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								MUS_GSTRINGS.data[string].bytes = mus_bstring_multiinsert(&muma_res, 
									MUS_GSTRINGS.data[string].bytes, offset,
									&data[first_offset], data_size-first_offset
								);
								MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
							}

							MUDEF void mu_string_insert_integer(musResult* result, muString string, int64_m i, size_m offset) {
								MU_SAFEFUNC(result, MUS_, mus_global_context, return;)
								MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return;, mus_string_)

								mumaResult muma_res = MUMA_SUCCESS;
								musResult mus_res = MUS_SUCCESS;

								size_m int_size = mu_raw_string_get_integer_conversion_size(&mus_res, MUS_GSTRINGS.data[string].encoding, i);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								MUS_GSTRINGS.data[string].bytes = mus_bstring_rshift(&muma_res,
									MUS_GSTRINGS.data[string].bytes, offset, int_size
								);
								MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								mu_raw_string_convert_to_integer(&mus_res, 
									MUS_GSTRINGS.data[string].encoding,
									&MUS_GSTRINGS.data[string].bytes.data[offset],
									MUS_GSTRINGS.data[string].bytes.length-offset,
									i
								);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res,
									MUS_GSTRINGS.data[string].bytes = mus_bstring_lshift(0, MUS_GSTRINGS.data[string].bytes, offset+int_size, int_size);
									MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
									return;
								)

								MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
							}

							MUDEF void mu_string_insert_float(musResult* result, muString string, float d, size_m decimal_places, size_m offset) {
								MU_SAFEFUNC(result, MUS_, mus_global_context, return;)
								MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return;, mus_string_)

								mumaResult muma_res = MUMA_SUCCESS;
								musResult mus_res = MUS_SUCCESS;

								size_m int_size = mu_raw_string_get_float_conversion_size(&mus_res, MUS_GSTRINGS.data[string].encoding, d, decimal_places);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								MUS_GSTRINGS.data[string].bytes = mus_bstring_rshift(&muma_res,
									MUS_GSTRINGS.data[string].bytes, offset, int_size
								);
								MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								mu_raw_string_convert_to_float(&mus_res, 
									MUS_GSTRINGS.data[string].encoding,
									&MUS_GSTRINGS.data[string].bytes.data[offset],
									MUS_GSTRINGS.data[string].bytes.length-offset,
									d, decimal_places
								);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res,
									MUS_GSTRINGS.data[string].bytes = mus_bstring_lshift(0, MUS_GSTRINGS.data[string].bytes, offset+int_size, int_size);
									MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
									return;
								)

								MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
							}

							MUDEF void mu_string_insert_double(musResult* result, muString string, double d, size_m decimal_places, size_m offset) {
								MU_SAFEFUNC(result, MUS_, mus_global_context, return;)
								MU_HOLD(result, string, MUS_GSTRINGS, mus_global_context, MUS_, return;, mus_string_)

								mumaResult muma_res = MUMA_SUCCESS;
								musResult mus_res = MUS_SUCCESS;

								size_m int_size = mu_raw_string_get_double_conversion_size(&mus_res, MUS_GSTRINGS.data[string].encoding, d, decimal_places);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								MUS_GSTRINGS.data[string].bytes = mus_bstring_rshift(&muma_res,
									MUS_GSTRINGS.data[string].bytes, offset, int_size
								);
								MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_mus_result(muma_res), MU_RELEASE(MUS_GSTRINGS, string, mus_string_) return;)

								mu_raw_string_convert_to_double(&mus_res, 
									MUS_GSTRINGS.data[string].encoding,
									&MUS_GSTRINGS.data[string].bytes.data[offset],
									MUS_GSTRINGS.data[string].bytes.length-offset,
									d, decimal_places
								);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res,
									MUS_GSTRINGS.data[string].bytes = mus_bstring_lshift(0, MUS_GSTRINGS.data[string].bytes, offset+int_size, int_size);
									MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
									return;
								)

								MU_RELEASE(MUS_GSTRINGS, string, mus_string_)
							}

					/* Raw string functions */

						/* Conversion */

							MUDEF size_m mu_raw_string_get_character_encoding_conversion_size(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, muCharacterEncoding desired_encoding) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)
								musResult res = MUS_SUCCESS;

								size_m i = mu_character_encoding_get_raw_string_code_point_offset(&res, encoding, data, data_size, 0, 0);
								MU_ASSERT(res == MUS_SUCCESS, result, res, return 0;)

								size_m output_size = 0;

								for (; i < data_size;) {
									muCodePoint cp = mu_character_encoding_get_code_point(&res, encoding, &data[i], data_size-i);
									MU_ASSERT(res == MUS_SUCCESS, result, res, return 0;)

									size_m cp_size = mu_character_encoding_get_code_point_size(&res, desired_encoding, cp);
									MU_ASSERT(res == MUS_SUCCESS, result, res, return 0;)
									output_size += cp_size;

									cp_size = mu_character_encoding_get_code_point_size(&res, encoding, cp);
									MU_ASSERT(res == MUS_SUCCESS, result, res, return 0;)

									i += cp_size;
								}

								return output_size;
							}

							MUDEF void mu_raw_string_convert_character_encoding(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, muCharacterEncoding output_encoding, muByte* output_data, size_m output_data_size) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
								MU_ASSERT(output_data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)
								MU_ASSERT(output_data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)
								musResult res = MUS_SUCCESS;

								size_m input_i = mu_character_encoding_get_raw_string_code_point_offset(&res, encoding, data, data_size, 0, 0);
								MU_ASSERT(res == MUS_SUCCESS, result, res, return;)

								size_m output_i = 0;

								for (; input_i < data_size;) {
									muCodePoint input_cp = mu_character_encoding_get_code_point(&res, encoding, &data[input_i], data_size-input_i);
									MU_ASSERT(res == MUS_SUCCESS, result, res, return;)

									size_m input_cp_size = mu_character_encoding_get_code_point_size(&res, encoding, input_cp);
									MU_ASSERT(res == MUS_SUCCESS, result, res, return;)

									size_m output_cp_size = mu_character_encoding_get_code_point_size(&res, output_encoding, input_cp);
									MU_ASSERT(res == MUS_SUCCESS, result, res, return;)

									input_i += input_cp_size;
									MU_ASSERT((output_data_size - output_i) >= output_cp_size, result, MUS_INSUFFICIENT_DATA_SIZE, return;)

									mu_character_encoding_set_code_point(&res, output_encoding, input_cp, &output_data[output_i], output_data_size-output_i);
									MU_ASSERT(res == MUS_SUCCESS, result, res, return;)
									output_i += output_cp_size;
								}
							}

						/* Integer/Float/Double conversion */

							// For these functions, ASCII/UTF-8 is assumed. As of the time of writing this,
							// these are the only encoding formats supported by muString, and their
							// implementation for the characters concerned (0-9 and .) are the same, storing 1
							// byte per character.

							#define MUS_MAX_FLOAT_DECIMAL_POINTS 7
							#define MUS_MAX_DOUBLE_DECIMAL_POINTS 16

							MUDEF size_m mu_raw_string_get_integer_conversion_size(musResult* result, muCharacterEncoding encoding, int64_m i) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								if (encoding) {}

								uint8_m add = 0;
								uint64_m ui;
								if (i >= 0) {
									ui = (uint64_m)i;
								} else {
									add = 1;
									ui = (uint64_m)((int64_m)(-i));
								}

								// https://stackoverflow.com/a/14564888
								uint8_m digits = ((uint8_m)(mu_log10((double)ui))) + 1;
								return (size_m)(digits + add);
							}

							MUDEF void mu_raw_string_convert_to_integer(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, int64_m i) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)

								musResult mus_res = MUS_SUCCESS;
								size_m isize = mu_raw_string_get_integer_conversion_size(&mus_res, encoding, i);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, return;)
								MU_ASSERT(data_size >= isize, result, MUS_INSUFFICIENT_DATA_SIZE, return;)

								uint8_m neg = 0;
								if (i < 0) {
									neg = 1;
								}

								// https://stackoverflow.com/a/14564888
								for (int8_m isizemod = isize-1; isizemod >= 0; isizemod -= 1, i /= 10) {
									if (i < 0) {
										data[isizemod] = ((-i) % 10) + 48;
									} else {
										data[isizemod] = (i % 10) + 48;
									}
								}

								if (neg > 0) {
									data[0] = 45;
								}
							}

							MUDEF int64_m mu_raw_string_get_integer(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)
								if (encoding) {}

								size_m beg_i = 0;
								if (data[0] == 45) {
									beg_i = 1;
								}

								int64_m ret = 0;
								for (size_m i = data_size; i != beg_i;) {
									i -= 1;
									int64_m bef_ret = ret;

									MU_ASSERT(data[i] >= 48 && data[i] <= 57, result, MUS_INVALID_DATA, return 0;)
									ret += mu_pow(10, (data_size-i)-1) * (data[i]-48);
									if (ret < bef_ret) {
										MU_SET_RESULT(result, MUS_OVERFLOW)
										return 0;
									}
								}

								if (beg_i == 1) {
									return -ret;
								}
								return ret;
							}

							MUDEF size_m mu_raw_string_get_float_conversion_size(musResult* result, muCharacterEncoding encoding, float d, size_m decimal_places) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								if (encoding) {}
								if (decimal_places > MUS_MAX_FLOAT_DECIMAL_POINTS) {
									decimal_places = MUS_MAX_FLOAT_DECIMAL_POINTS;
								}

								uint8_m add = 0;
								float ud;
								if (d >= 0.f) {
									ud = d;
								} else {
									add = 1;
									ud = -d;
								}

								uint8_m digits = ((uint8_m)(mu_log10((double)ud))) + 1;
								if (decimal_places == 0) {
									return (size_m)(digits + add);
								}
								return ((size_m)(digits + add)) + decimal_places + 1;
							}

							void mus_write_double_int_only(size_m intsize, double floored_d, muByte* data) {
								uint8_m neg = 0;
								if (floored_d < 0.f) {
									neg = 1;
								}

								size_m count = 0;

								for (int64_m isizemod = intsize-1; isizemod >= 0; isizemod -= 1, count += 1) {
									double i = floored_d / mu_pow(10.f, count);
									if (i < 0) {
										data[isizemod] = (muByte)mu_fmod(-i, 10.f) + 48;
									} else {
										data[isizemod] = (muByte)mu_fmod(i, 10.f) + 48;
									}
								}

								if (neg > 0) {
									data[0] = 45;
								}
							}

							MUDEF void mu_raw_string_convert_to_float(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, float d, size_m decimal_places) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)
								if (decimal_places > MUS_MAX_FLOAT_DECIMAL_POINTS) {
									decimal_places = MUS_MAX_FLOAT_DECIMAL_POINTS;
								}

								musResult mus_res = MUS_SUCCESS;
								size_m dsize = mu_raw_string_get_float_conversion_size(&mus_res, encoding, d, decimal_places);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, return;)
								MU_ASSERT(data_size >= dsize, result, MUS_INSUFFICIENT_DATA_SIZE, return;)

								size_m intsize = (dsize - decimal_places);
								if (decimal_places != 0) {
									intsize -= 1;
								}

								// Write int digits

								double floored_d;
								if (d >= 0.f) {
									floored_d = mu_floor((double)d);
								} else {
									floored_d = -mu_floor((double)-d);
								}

								mus_write_double_int_only(intsize, floored_d, data);

								// Write decimal digits

								if (decimal_places > 0) {
									data[intsize] = 46;

									if (d < 0.f) { d = -d; }
									if (floored_d < 0.f) { floored_d = -floored_d; }

									double d_decimals = (double)(d)-floored_d;
									if (d_decimals != 0.f) {
										d_decimals = (((double)(d_decimals)) * (mu_pow(10.f, (double)(decimal_places))));
									} else {
										d_decimals = (((double)(floored_d)) * (mu_pow(10.f, (double)(decimal_places))));
									}

									mus_write_double_int_only(decimal_places, d_decimals, &data[intsize+1]);
								}
							}

							MUDEF float mu_raw_string_get_float(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0.f;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0.f;)
								if (encoding) {}

								size_m beg_i = 0;
								if (data[0] == 45) {
									beg_i = 1;
								}

								size_m decimal = MU_NONE;
								for (size_m i = 0; i < data_size; i++) {
									if (data[i] == 46) {
										decimal = i;
										break;
									}
								}
								MU_ASSERT(decimal != MU_NONE, result, MUS_INVALID_DATA, return 0.f;)

								float ret = 0.f;
								for (size_m i = beg_i; i < data_size; i++) {
									if (i == decimal) {
										continue;
									}
									MU_ASSERT(data[i] >= 48 && data[i] <= 57, result, MUS_INVALID_DATA, return 0;)

									if (i < decimal) {
										ret += (float)(mu_pow(10.f, (decimal-i)-1) * (double)(data[i]-48));
									} else {
										ret += (float)(mu_pow(10.f, -(double)((i-decimal))) * (double)(data[i]-48));
									}
								}

								if (beg_i == 1) {
									return -ret;
								}
								return ret;
							}

							MUDEF size_m mu_raw_string_get_double_conversion_size(musResult* result, muCharacterEncoding encoding, double d, size_m decimal_places) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								if (encoding) {}
								if (decimal_places > MUS_MAX_DOUBLE_DECIMAL_POINTS) {
									decimal_places = MUS_MAX_DOUBLE_DECIMAL_POINTS;
								}

								uint8_m add = 0;
								double ud;
								if (d >= 0.f) {
									ud = d;
								} else {
									add = 1;
									ud = -d;
								}

								uint8_m digits = ((uint8_m)(mu_log10(ud))) + 1;
								if (decimal_places == 0) {
									return (size_m)(digits + add);
								}
								return ((size_m)(digits + add)) + decimal_places + 1;
							}

							MUDEF void mu_raw_string_convert_to_double(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, double d, size_m decimal_places) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)
								if (decimal_places > MUS_MAX_DOUBLE_DECIMAL_POINTS) {
									decimal_places = MUS_MAX_DOUBLE_DECIMAL_POINTS;
								}

								musResult mus_res = MUS_SUCCESS;
								size_m dsize = mu_raw_string_get_double_conversion_size(&mus_res, encoding, d, decimal_places);
								MU_ASSERT(mus_res == MUS_SUCCESS, result, mus_res, return;)
								MU_ASSERT(data_size >= dsize, result, MUS_INSUFFICIENT_DATA_SIZE, return;)

								size_m intsize = (dsize - decimal_places);
								if (decimal_places != 0) {
									intsize -= 1;
								}

								// Write int digits

								double floored_d;
								if (d >= 0.f) {
									floored_d = mu_floor((double)d);
								} else {
									floored_d = -mu_floor((double)-d);
								}

								mus_write_double_int_only(intsize, floored_d, data);

								// Write decimal digits

								if (decimal_places > 0) {
									data[intsize] = 46;

									if (d < 0.f) { d = -d; }
									if (floored_d < 0.f) { floored_d = -floored_d; }

									double d_decimals = (double)(d)-floored_d;
									if (d_decimals != 0.f) {
										d_decimals = (((double)(d_decimals)) * (mu_pow(10.f, (double)(decimal_places))));
									} else {
										d_decimals = (((double)(floored_d)) * (mu_pow(10.f, (double)(decimal_places))));
									}

									mus_write_double_int_only(decimal_places, d_decimals, &data[intsize+1]);
								}
							}

							MUDEF double mu_raw_string_get_double(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0.f;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0.f;)
								if (encoding) {}

								size_m beg_i = 0;
								if (data[0] == 45) {
									beg_i = 1;
								}

								size_m decimal = MU_NONE;
								for (size_m i = 0; i < data_size; i++) {
									if (data[i] == 46) {
										decimal = i;
										break;
									}
								}
								MU_ASSERT(decimal != MU_NONE, result, MUS_INVALID_DATA, return 0.f;)

								double ret = 0.f;
								for (size_m i = beg_i; i < data_size; i++) {
									if (i == decimal) {
										continue;
									}
									MU_ASSERT(data[i] >= 48 && data[i] <= 57, result, MUS_INVALID_DATA, return 0;)

									if (i < decimal) {
										ret += mu_pow(10.f, (decimal-i)-1) * (double)(data[i]-48);
									} else {
										ret += mu_pow(10.f, -(double)((i-decimal))) * (double)(data[i]-48);
									}
								}

								if (beg_i == 1) {
									return -ret;
								}
								return ret;
							}

					/* Cross-encoding functions */

						MUDEF size_m mu_character_encoding_get_code_point_size(musResult* result, muCharacterEncoding encoding, muCodePoint code_point) {
							switch (encoding) { default: MU_SET_RESULT(result, MUS_UNKNOWN_CHARACTER_ENCODING) return 0; break;
								case MU_ASCII: return mu_ASCII_get_code_point_size(result, code_point); break;
								case MU_UTF8: return mu_UTF8_get_code_point_size(result, code_point); break;
							}
						}

						MUDEF muCodePoint mu_character_encoding_get_code_point(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size) {
							switch (encoding) { default: MU_SET_RESULT(result, MUS_UNKNOWN_CHARACTER_ENCODING) return 0; break;
								case MU_ASCII: return mu_ASCII_get_code_point(result, data, data_size); break;
								case MU_UTF8: return mu_UTF8_get_code_point(result, data, data_size); break;
							}
						}

						MUDEF void mu_character_encoding_set_code_point(musResult* result, muCharacterEncoding encoding, muCodePoint code_point, muByte* data, size_m data_size) {
							switch (encoding) { default: MU_SET_RESULT(result, MUS_UNKNOWN_CHARACTER_ENCODING) return; break;
								case MU_ASCII: mu_ASCII_set_code_point(result, code_point, data, data_size); break;
								case MU_UTF8: mu_UTF8_set_code_point(result, code_point, data, data_size); break;
							}
						}

						MUDEF muBool mu_character_encoding_verify_raw_string(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size) {
							switch (encoding) { default: MU_SET_RESULT(result, MUS_UNKNOWN_CHARACTER_ENCODING) return MU_FALSE; break;
								case MU_ASCII: return mu_ASCII_verify_raw_string(result, data, data_size); break;
								case MU_UTF8: return mu_UTF8_verify_raw_string(result, data, data_size); break;
							}
						}

						MUDEF size_m mu_character_encoding_get_raw_string_code_point_length(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size) {
							switch (encoding) { default: MU_SET_RESULT(result, MUS_UNKNOWN_CHARACTER_ENCODING) return 0; break;
								case MU_ASCII: return mu_ASCII_get_raw_string_code_point_length(result, data, data_size); break;
								case MU_UTF8: return mu_UTF8_get_raw_string_code_point_length(result, data, data_size); break;
							}
						}

						MUDEF size_m mu_character_encoding_get_raw_string_code_point_offset(musResult* result, muCharacterEncoding encoding, muByte* data, size_m data_size, size_m offset, size_m index) {
							switch (encoding) { default: MU_SET_RESULT(result, MUS_UNKNOWN_CHARACTER_ENCODING) return 0; break;
								case MU_ASCII: return mu_ASCII_get_raw_string_code_point_offset(result, data, data_size, offset, index); break;
								case MU_UTF8: return mu_UTF8_get_raw_string_code_point_offset(result, data, data_size, offset, index); break;
							}
						}

					/* Encoding-specific functions */

						/* ASCII */

							MUDEF size_m mu_ASCII_get_code_point_size(musResult* result, muCodePoint code_point) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(code_point <= 0x7F, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)

								return 1;
							}

							MUDEF muCodePoint mu_ASCII_get_code_point(musResult* result, muByte* data, size_m data_size) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)
								MU_ASSERT(data[0] <= 0x7F, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)

								return data[0];
							}

							MUDEF void mu_ASCII_set_code_point(musResult* result, muCodePoint code_point, muByte* data, size_m data_size) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)
								MU_ASSERT(code_point <= 0x7F, result, MUS_CHARACTER_CANNOT_BE_ENCODED, return;)

								data[0] = code_point;
							}

							MUDEF muBool mu_ASCII_verify_raw_string(musResult* result, muByte* data, size_m data_size) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)

								for (size_m i = 0; i < data_size; i++) {
									if (data[i] > 0x7F) {
										return MU_FALSE;
									}
								}

								return MU_TRUE;
							}

							MUDEF size_m mu_ASCII_get_raw_string_code_point_length(musResult* result, muByte* data, size_m data_size) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)

								return data_size;
							}

							MUDEF size_m mu_ASCII_get_raw_string_code_point_offset(musResult* result, muByte* data, size_m data_size, size_m offset, size_m index) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return offset;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return offset;)
								MU_ASSERT(offset+index < data_size, result, MUS_INVALID_OFFSET, return offset;)

								return offset+index;
							}

						/* UTF-8 */

							muByte mus_UTF8_get_code_point_size_from_byte(muByte byte) {
								// 00000000 <= n <= 01111111
								if (/*byte >= 0 && */byte <= 127) {
									return 1;
								// 11000000 <= n < 11100000
								} else if (byte >= 192 && byte < 224) {
									return 2;
								// 11100000 <= n < 11110000
								} else if (byte >= 224 && byte < 240) {
									return 3;
								// 11110000 <= n <= 11110111
								} else if (byte >= 240 && byte <= 247) {
									return 4;
								} else {
									// Value is 10xxxxxx or 11111xxx; invalid.
									return 0;
								}
							}

							MUDEF size_m mu_UTF8_get_code_point_size(musResult* result, muCodePoint code_point) {
								MU_SET_RESULT(result, MUS_SUCCESS)

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
								// Storable in 21 bits = 4 bytes
								if (code_point < 2097152) {
									return 4;
								}
								// Anything larger cannot be stored.
								MU_SET_RESULT(result, MUS_CHARACTER_CANNOT_BE_ENCODED)
								return 0;
							}

							MUDEF muCodePoint mu_UTF8_get_code_point(musResult* result, muByte* data, size_m data_size) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)

								muByte code_point_size = mus_UTF8_get_code_point_size_from_byte(data[0]);
								MU_ASSERT(code_point_size != 0, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)

								for (size_m i = 1; i < code_point_size; i++) {
									// 10000000 <= n <= 10111111
									if (!(data[i] >= 128 && data[i] <= 191)) {
										MU_SET_RESULT(result, MUS_INVALID_CHARACTER_ENCODING)
										return 0;
									}
								}

								switch (code_point_size) { default: MU_SET_RESULT(result, MUS_INVALID_CHARACTER_ENCODING) return 0; break;
									case 1: {
										return data[0];
									} break;

									case 2: {
										MU_ASSERT(data_size >= 2, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)
										return
										// 110xxxxx
										(muCodePoint)(data[0] & 31) << 6 |
										// 10xxxxxx
										(muCodePoint)(data[1] & 63);
									} break;

									case 3: {
										MU_ASSERT(data_size >= 3, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)
										return
										// 1110xxxx
										(muCodePoint)(data[0] & 15) << 12 |
										// 10xxxxxx
										(muCodePoint)(data[1] & 63) << 6 |
										// 10xxxxxx
										(muCodePoint)(data[2] & 63);
									} break;

									case 4: {
										MU_ASSERT(data_size >= 4, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)
										return
										// 1110xxxx
										(muCodePoint)(data[0] & 7) << 18 |
										// 10xxxxxx
										(muCodePoint)(data[1] & 63) << 12 |
										// 10xxxxxx
										(muCodePoint)(data[2] & 63) << 6 |
										// 10xxxxxx
										(muCodePoint)(data[3] & 63);
									} break;
								}
							}

							MUDEF void mu_UTF8_set_code_point(musResult* result, muCodePoint code_point, muByte* data, size_m data_size) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return;)

								musResult res = MUS_SUCCESS;
								size_m code_point_size = mu_UTF8_get_code_point_size(&res, code_point);
								MU_ASSERT(res == MUS_SUCCESS, result, res, return;)

								MU_ASSERT(code_point_size <= data_size, result, MUS_INVALID_DATA_SIZE, return;)

								switch (code_point_size) { default: MU_SET_RESULT(result, MUS_CHARACTER_CANNOT_BE_ENCODED) return; break;
									case 1: {
										data[0] = code_point;
									} break;

									// mask = Get what you want
									// add  = Add bits
									case 2: {
										// 110xxxxx                           mask    add
										data[0] = (muByte)(((code_point >> 6) & 31) | 192);
										// 10xxxxxx                      mask
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

							MUDEF muBool mu_UTF8_verify_raw_string(musResult* result, muByte* data, size_m data_size) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)

								musResult res = MUS_SUCCESS;
								size_m i = 0;
								// BOM
								if (data_size >= 3) {
									if (data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF) {
										i = 3;
									}
								}

								for (; i < data_size;) {
									muByte code_point_size = mus_UTF8_get_code_point_size_from_byte(data[i]);
									if (code_point_size == 0) {
										return MU_FALSE;
									}
									if (!((i+code_point_size < data_size) || (i+code_point_size == data_size))) {
										return MU_FALSE;
									}

									mu_UTF8_get_code_point(&res, &data[i], data_size-i);
									if (res != MUS_SUCCESS) {
										return MU_FALSE;
									}

									i += code_point_size;
								}
								return MU_TRUE;
							}

							MUDEF size_m mu_UTF8_get_raw_string_code_point_length(musResult* result, muByte* data, size_m data_size) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)

								size_m i = 0;
								// BOM
								if (data_size >= 3) {
									if (data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF) {
										i = 3;
									}
								}

								size_m code_point_length = 0;

								for (; i < data_size;) {
									muByte code_point_size = mus_UTF8_get_code_point_size_from_byte(data[i]);
									MU_ASSERT(code_point_size != 0, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)
									MU_ASSERT((i+code_point_size < data_size) || (i+code_point_size == data_size), result, MUS_INVALID_CHARACTER_ENCODING, return 0;)
									code_point_length += 1;

									i += code_point_size;
								}
								return code_point_length;
							}

							MUDEF size_m mu_UTF8_get_raw_string_code_point_offset(musResult* result, muByte* data, size_m data_size, size_m offset, size_m index) {
								MU_SET_RESULT(result, MUS_SUCCESS)
								MU_ASSERT(data != MU_NULL_PTR, result, MUS_INVALID_DATA_POINTER, return 0;)
								MU_ASSERT(data_size > 0, result, MUS_INVALID_DATA_SIZE, return 0;)

								size_m i = 0;
								if (offset == 0) {
									// BOM
									if (data_size >= 3) {
										if (data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF) {
											i = 3;
										}
									}
								}

								size_m count_index = 0;

								for (; i < data_size && count_index != index;) {
									muByte code_point_size = mus_UTF8_get_code_point_size_from_byte(data[0]);
									MU_ASSERT(code_point_size != 0, result, MUS_INVALID_CHARACTER_ENCODING, return 0;)
									MU_ASSERT((i+code_point_size < data_size) || (i+code_point_size == data_size), result, MUS_INVALID_CHARACTER_ENCODING, return 0;)
									count_index += 1;

									i += code_point_size;
								}

								MU_ASSERT(count_index == index, result, MUS_INVALID_OFFSET, return 0;)
								return i;
							}

					/* Codepoint */

						/* Uppercase/Lowercase */

							// Last updated 16 September 2023, covers 0 -> 65533
							// https://en.wikipedia.org/wiki/List_of_Unicode_characters
							// https://www.ssec.wisc.edu/~tomw/java/unicode.html

							MUDEF muCodePoint mu_code_point_lowercase(musResult* result, muCharacterEncoding encoding, muCodePoint code_point) {
								if (encoding == MU_ASCII) {
									MU_ASSERT(code_point <= 0x7F, result, MUS_INVALID_CHARACTER_ENCODING, return code_point;)
									if (code_point >= 65 && code_point <= 90) {
										return code_point + 32;
									}
									return code_point;
								}

								if (
								// Latin alphabet
									(code_point >= 65 && code_point <= 90) ||
								// Latin-1 supplement
									(code_point >= 192 && code_point <= 222 && code_point != 215) ||
								// Greek and Coptic
									(code_point >= 913 && code_point <= 939) ||
								// Cyrillic
									(code_point >= 0x0410 && code_point <= 0x042F) ||
								// Halfwidth and fullwidth forms
									(code_point >= 65313 && code_point <= 65338)
								) {
									return code_point + 32;
								} else if (
								// Latin extended-a
									(
										(code_point >= 256 && code_point <= 310 && code_point % 2 == 0) || 
										(code_point >= 313 && code_point <= 327 && code_point % 2 != 0) ||
										(code_point >= 330 && code_point <= 376 && code_point % 2 == 0) ||
										(code_point >= 377 && code_point <= 381 && code_point % 2 != 0)
									) ||
								// Latin extended-b
									(
										(code_point >= 461 && code_point <= 475 && code_point % 2 != 0) ||
										(code_point >= 478 && code_point <= 494 && code_point % 2 == 0) ||
										(code_point >= 504 && code_point <= 542 && code_point % 2 == 0) ||
										(code_point >= 546 && code_point <= 562 && code_point % 2 == 0) ||
										(code_point >= 582 && code_point <= 590 && code_point % 2 == 0)
									) ||
								// Greek and Coptic
									(
										(code_point >= 984 && code_point <= 1006 && code_point % 2 == 0)
									) ||
								// Cyrillic
									(
										(code_point >= 0x0460 && code_point <= 0x0480 && code_point % 2 == 0) ||
										(code_point >= 0x048A && code_point <= 0x04BE && code_point % 2 == 0) ||
										(code_point >= 0x04C1 && code_point <= 0x04CD && code_point % 2 != 0) ||
										(code_point >= 0x04D0 && code_point <= 0x04FE && code_point % 2 == 0) ||
										// Cyrillic supplement
										(code_point >= 0x0500 && code_point <= 0x052E && code_point % 2 == 0)
									) ||
								// Latin extended additional
									(
										(code_point >= 7680 && code_point <= 7828 && code_point % 2 == 0) ||
										(code_point >= 7840 && code_point <= 7928 && code_point % 2 == 0)
									)
								) {
									return code_point + 1;
								} else if (
								// Cyrillic
									(
										(code_point >= 0x0400 && code_point <= 0x040F)
									)
								) {
									return code_point + 80;
								} else if (
								// Armenian
									(
										(code_point >= 0x0530 && code_point <= 0x0558)
									) ||
								// Georgian
									(
										(code_point >= 0x10A0 && code_point <= 0x10CF)
									)
								) {
									return code_point + 48;
								} else if (
								// Greek extended
									(
										(code_point >= 7944 && code_point <= 7951) || (code_point >= 7960 && code_point <= 7965) || (code_point >= 7976 && code_point <= 7983) || (code_point >= 7992 && code_point <= 7999) ||
										(code_point >= 8008 && code_point <= 8013) || (code_point >= 8025 && code_point <= 8031) || (code_point >= 8040 && code_point <= 8047) || (code_point >= 8072 && code_point <= 8079) ||
										(code_point >= 8088 && code_point <= 8095) || (code_point >= 8104 && code_point <= 8111) || (code_point >= 8120 && code_point <= 8124)
									)
								) {
									return code_point - 8;
								} else if (
								// Enclosed alphanumerics
									(
										(code_point >= 9398 && code_point <= 9423)
									)
								) {
									return code_point + 26;
								}
								switch (code_point) {
								default: break;
								// Odd Latin extended-b / IPA extensions
								case 386: case 388: case 391: case 395: case 401: case 408: case 416: case 418: case 420: case 423: case 428: case 431: 
								case 435: case 437: case 440: case 444: case 453: case 456: case 459: case 498: case 500: case 571: case 577: return code_point+1; break;
								case 384: return 579; break;
								case 385: return 595; break;
								case 390: return 596; break;
								case 393: return 598; break;
								case 394: return 599; break;
								case 398: return 600; break;
								case 399: return 601; break;
								case 400: return 603; break;
								case 403: return 608; break;
								case 404: return 611; break;
								case 406: return 617; break;
								case 407: return 616; break;
								case 412: return 623; break;
								case 413: return 626; break;
								case 425: return 643; break;
								case 430: return 648; break;
								case 433: return 650; break;
								case 434: return 641; break;
								case 439: return 658; break;
								case 452: return 454; break;
								case 455: return 457; break;
								case 458: return 460; break;
								case 497: return 499; break;
								case 544: return 414; break;
								case 573: return 410; break;
								case 579: return 384; break;
								case 580: return 649; break;
								case 581: return 652; break;

								// Odd greek and coptic
								case 880: case 882: case 886: case 1015: case 1018: return code_point+1; break;
								case 895: return 1011; break;
								case 904: case 905: case 906: return code_point+37; break;
								case 908: case 910: case 911: return code_point+64; break;
								case 975: return 983; break;
								case 1012: return 977; break;
								case 1017: return 1010; break;
								case 1021: case 1022: case 1023: return code_point-130; break;

								// Odd greek extended
								case 8136: case 8137: case 8138: case 8139: return code_point-86; break;
								case 8140: return 8131; break;
								case 8152: return 8144; break;
								case 8153: return 8145; break;
								case 8154: case 8155: return code_point-100; break;
								case 8168: return 8160; break;
								case 8169: return 8161; break;
								case 8170: return 8058; break;
								case 8171: return 8059; break;
								case 8172: return 8165; break;
								case 8184: case 8185: return code_point-128; break;
								case 8187: return 8061; break;
								}
								return code_point;
							}

							MUDEF muCodePoint mu_code_point_uppercase(musResult* result, muCharacterEncoding encoding, muCodePoint code_point) {
								if (encoding == MU_ASCII) {
									MU_ASSERT(code_point <= 0x7F, result, MUS_INVALID_CHARACTER_ENCODING, return code_point;)
									if (code_point >= 97 && code_point <= 122) {
										return code_point - 32;
									}
									return code_point;
								}

								if (
								// Latin alphabet
									(code_point >= 97 && code_point <= 122) ||
								// Latin-1 supplement
									(code_point >= 224 && code_point <= 255 && code_point != 247) ||
								// Greek and Coptic
									(code_point >= 945 && code_point <= 971) ||
								// Cyrillic
									(code_point >= (0x0410 + 32) && code_point <= (0x042F + 32)) ||
								// Halfwidth and fullwidth forms
									(code_point >= (65313 + 32) && code_point <= (65338 + 32))
								) {
									return code_point - 32;
								} else if (
								// Latin extended-a
									(
										(code_point >= 257 && code_point <= 311 && code_point % 2 != 0) || 
										(code_point >= 312 && code_point <= 328 && code_point % 2 == 0) ||
										(code_point >= 329 && code_point <= 375 && code_point % 2 != 0) ||
										(code_point >= 378 && code_point <= 382 && code_point % 2 == 0)
									) ||
								// Latin extended-b
									(
										(code_point >= 462 && code_point <= 476 && code_point % 2 == 0) ||
										(code_point >= 479 && code_point <= 495 && code_point % 2 != 0) ||
										(code_point >= 505 && code_point <= 543 && code_point % 2 != 0) ||
										(code_point >= 547 && code_point <= 563 && code_point % 2 != 0) ||
										(code_point >= 583 && code_point <= 591 && code_point % 2 != 0)
									) ||
								// Greek and Coptic
									(
										(code_point >= 985 && code_point <= 1007 && code_point % 2 != 0)
									) ||
								// Cyrillic
									(
										(code_point >= 0x0461 && code_point <= 0x0481 && code_point % 2 != 0) ||
										(code_point >= 0x048B && code_point <= 0x04BF && code_point % 2 != 0) ||
										(code_point >= 0x04C2 && code_point <= 0x04CE && code_point % 2 == 0) ||
										(code_point >= 0x04D1 && code_point <= 0x04FF && code_point % 2 != 0) ||
										// Cyrillic supplement
										(code_point >= 0x0501 && code_point <= 0x052F && code_point % 2 != 0)
									) ||
								// Latin extended additional
									(
										(code_point >= 7681 && code_point <= 7829 && code_point % 2 != 0) ||
										(code_point >= 7841 && code_point <= 7929 && code_point % 2 != 0)
									)
								) {
									return code_point - 1;
								} else if (
								// Cyrillic
									(
										(code_point >= (0x0400+80) && code_point <= (0x040F+80))
									)
								) {
									return code_point - 80;
								} else if (
								// Armenian
									(
										(code_point >= (0x0530+48) && code_point <= (0x0558+48))
									) ||
								// Georgian
									(
										(code_point >= (0x10A0+48) && code_point <= (0x10CF+48))
									)
								) {
									return code_point - 48;
								} else if (
								// Greek extended
									(
										(code_point >= (7944-8) && code_point <= (7951-8)) || (code_point >= (7960-8) && code_point <= (7965-8)) || (code_point >= (7976-8) && code_point <= (7983-8)) || (code_point >= (7992-8) && code_point <= (7999-8)) ||
										(code_point >= (8008-8) && code_point <= (8013-8)) || (code_point >= (8025-8) && code_point <= (8031-8)) || (code_point >= (8040-8) && code_point <= (8047-8)) || (code_point >= (8072-8) && code_point <= (8079-8)) ||
										(code_point >= (8088-8) && code_point <= (8095-8)) || (code_point >= (8104-8) && code_point <= (8111-8)) || (code_point >= (8120-8) && code_point <= (8124-8))
									)
								) {
									return code_point + 8;
								} else if (
								// Enclosed alphanumerics
									(
										(code_point >= (9398+26) && code_point <= (9423+26))
									)
								) {
									return code_point - 26;
								}
								switch (code_point) {
								default: break;
								// Odd Latin extended-b / IPA extensions
								case 387: case 389: case 392: case 396: case 402: case 409: case 417: case 419: case 421: case 424: case 429: case 432: 
								case 436: case 438: case 441: case 445: case 453: case 456: case 459: case 498: case 501: case 572: case 578: return code_point-1; break;
								case 579: return 384; break;
								case 595: return 385; break;
								case 596: return 390; break;
								case 598: return 393; break;
								case 599: return 394; break;
								case 600: return 398; break;
								case 601: return 399; break;
								case 603: return 400; break;
								case 608: return 403; break;
								case 611: return 404; break;
								case 617: return 406; break;
								case 616: return 407; break;
								case 623: return 412; break;
								case 626: return 413; break;
								case 643: return 425; break;
								case 648: return 430; break;
								case 650: return 433; break;
								case 641: return 434; break;
								case 658: return 439; break;
								case 454: return 452; break;
								case 457: return 455; break;
								case 460: return 458; break;
								case 499: return 497; break;
								case 414: return 544; break;
								case 410: return 573; break;
								case 384: return 579; break;
								case 649: return 580; break;
								case 652: return 581; break;

								// Odd Greek and Coptic
								case 881: case 883: case 887: case 1016: case 1019: return code_point-1; break;
								case 1011: return 895; break;
								case 941: case 942: case 943: return code_point-37; break;
								case 972: case 974: case 975: return code_point-64; break;
								case 983: return 975; break;
								case 977: return 1012; break;
								case 1010: return 1017; break;
								case 891: case 892: case 893: return code_point+130; break;

								// Odd Greek extended
								case 8050: case 8051: case 8052: case 8053: return code_point+86; break;
								case 8131: return 8140; break;
								case 8144: return 8152; break;
								case 8145: return 8153; break;
								case 8054: case 8055: return code_point+100; break;
								case 8160: return 8168; break;
								case 8161: return 8169; break;
								case 8058: return 8170; break;
								case 8059: return 8171; break;
								case 8165: return 8172; break;
								case 8056: case 8057: return code_point+128; break;
								case 8061: return 8187; break;
								}
								return code_point;
							}

				#ifdef __cplusplus
				}
				#endif

			#endif /* MUS_IMPLEMENTATION */
		#endif

	#ifdef __cplusplus
	extern "C" { // }
	#endif

	/* Shared functions */

		muCOSAResult muma_result_to_muCOSA_result(mumaResult result) {
			switch (result) {
				default: case MUMA_SUCCESS: return MUCOSA_MUMA_SUCCESS; break;
				case MUMA_FAILED_TO_ALLOCATE: return MUCOSA_MUMA_FAILED_TO_ALLOCATE; break;
				case MUMA_INVALID_INDEX: return MUCOSA_MUMA_INVALID_INDEX; break;
				case MUMA_INVALID_SHIFT_AMOUNT: return MUCOSA_MUMA_INVALID_SHIFT_AMOUNT; break;
				case MUMA_NOT_FOUND: return MUCOSA_MUMA_NOT_FOUND; break;
			}
		}

		muCOSAResult muCOSA_verify_pixel_format(muPixelFormat pf) {
			if (pf.samples != 1 && pf.samples != 2 && pf.samples != 4 && pf.samples != 8 && pf.samples != 16) {
				return MUCOSA_INVALID_SAMPLE_COUNT;
			}
			return MUCOSA_SUCCESS;
		}

		muCOSAResult muCOSA_verify_window_create_info(muWindowCreateInfo ci) {
			if (ci.minimized && ci.maximized) {
				return MUCOSA_INVALID_MINIMUM_MAXIMUM_BOOLS;
			}
			if (ci.min_width > ci.max_width || ci.min_height > ci.max_height) {
				return MUCOSA_INVALID_MINIMUM_MAXIMUM_DIMENSIONS;
			}
			return muCOSA_verify_pixel_format(ci.pixel_format);
		}

	/* X11 implementation */

	#ifdef MUCOSA_X11
		#define MUCOSA_X11_CALL(...) __VA_ARGS__

		#include <X11/Xlib.h> // Includes <X11/X.h>
		#include <X11/Xutil.h> // Must be included AFTER <X11/Xlib.h>
		#include <X11/XKBlib.h> // For XkbKeycodeToKeysym

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

		/* Structs */

			struct muCOSA_X11_input {
				muButtonState keyboard_key_states[MU_KEYBOARD_KEY_LAST-MU_KEYBOARD_KEY_FIRST+1];
				muState keyboard_state_states[MU_KEYBOARD_STATE_LAST-MU_KEYBOARD_STATE_FIRST+1];
				muButtonState mouse_button_states[MU_MOUSE_BUTTON_LAST-MU_MOUSE_BUTTON_FIRST+1];
			};
			typedef struct muCOSA_X11_input muCOSA_X11_input;

			struct muCOSA_X11Window {
				muBool active;

				muBool closed;
				muBool visible;

				Display* display;
				Window parent_window;
				Window window;

				MU_LOCK

				Atom delete_atom;

				muGraphicsAPI api;
				MUCOSA_OPENGL_CALL(
					GLXContext gl_context;
				)

				uint32_m min_width; uint32_m min_height;
				uint32_m max_width; uint32_m max_height;
				XSizeHints* size_hints;

				muCursorStyle cursor_style;
				Cursor cursor;
				int32_m scroll_level;

				muCOSA_X11_input input;

				int32_m x;
				int32_m y;
				muBool minimized;
				muBool maximized;

				void (*dimensions_callback)(muWindow window, uint32_m width, uint32_m height);
				void (*position_callback)(muWindow window, int32_m x, int32_m y);
				void (*focus_callback)(muWindow window, muBool focused);
				void (*maximize_callback)(muWindow window, muBool maximized);
				void (*minimize_callback)(muWindow window, muBool minimized);

				void (*keyboard_key_callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state);
				void (*keyboard_state_callback)(muWindow window, muKeyboardState keyboard_state, muState state);

				void (*mouse_button_callback)(muWindow window, muMouseButton mouse_button, muButtonState state);

			};
			typedef struct muCOSA_X11Window muCOSA_X11Window;

			MU_HRARRAY_DEFAULT_FUNC(muCOSA_X11Window)

		/* Useful funcs */

			/* Keyboard key input */

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

				int muCOSA_X11_keyboard_key_get_XK_key(muKeyboardKey key) {
					switch (key) {
						default: return MUCOSA_X11_XK_VoidSymbol; break;
						case MU_KEYBOARD_KEY_BACKSPACE: return MUCOSA_X11_XK_BackSpace; break;
						case MU_KEYBOARD_KEY_TAB: return MUCOSA_X11_XK_Tab; break;
						case MU_KEYBOARD_KEY_CLEAR: return MUCOSA_X11_XK_Clear; break;
						case MU_KEYBOARD_KEY_RETURN: return MUCOSA_X11_XK_Return; break;
						case MU_KEYBOARD_KEY_PAUSE: return MUCOSA_X11_XK_Pause; break;
						case MU_KEYBOARD_KEY_ESCAPE: return MUCOSA_X11_XK_Escape; break;
						case MU_KEYBOARD_KEY_MODECHANGE: return MUCOSA_X11_XK_Mode_switch; break;
						case MU_KEYBOARD_KEY_SPACE: return MUCOSA_X11_XK_space; break;
						case MU_KEYBOARD_KEY_PRIOR: return MUCOSA_X11_XK_Prior; break;
						case MU_KEYBOARD_KEY_NEXT: return MUCOSA_X11_XK_Next; break;
						case MU_KEYBOARD_KEY_END: return MUCOSA_X11_XK_End; break;
						case MU_KEYBOARD_KEY_HOME: return MUCOSA_X11_XK_Home; break;
						case MU_KEYBOARD_KEY_LEFT: return MUCOSA_X11_XK_Left; break;
						case MU_KEYBOARD_KEY_UP: return MUCOSA_X11_XK_Up; break;
						case MU_KEYBOARD_KEY_RIGHT: return MUCOSA_X11_XK_Right; break;
						case MU_KEYBOARD_KEY_DOWN: return MUCOSA_X11_XK_Down; break;
						case MU_KEYBOARD_KEY_SELECT: return MUCOSA_X11_XK_Select; break;
						case MU_KEYBOARD_KEY_PRINT: return MUCOSA_X11_XK_Print; break;
						case MU_KEYBOARD_KEY_EXECUTE: return MUCOSA_X11_XK_Execute; break;
						case MU_KEYBOARD_KEY_INSERT: return MUCOSA_X11_XK_Insert; break;
						case MU_KEYBOARD_KEY_DELETE: return MUCOSA_X11_XK_Delete; break;
						case MU_KEYBOARD_KEY_HELP: return MUCOSA_X11_XK_Help; break;
						case MU_KEYBOARD_KEY_0: return MUCOSA_X11_XK_0; break;
						case MU_KEYBOARD_KEY_1: return MUCOSA_X11_XK_1; break;
						case MU_KEYBOARD_KEY_2: return MUCOSA_X11_XK_2; break;
						case MU_KEYBOARD_KEY_3: return MUCOSA_X11_XK_3; break;
						case MU_KEYBOARD_KEY_4: return MUCOSA_X11_XK_4; break;
						case MU_KEYBOARD_KEY_5: return MUCOSA_X11_XK_5; break;
						case MU_KEYBOARD_KEY_6: return MUCOSA_X11_XK_6; break;
						case MU_KEYBOARD_KEY_7: return MUCOSA_X11_XK_7; break;
						case MU_KEYBOARD_KEY_8: return MUCOSA_X11_XK_8; break;
						case MU_KEYBOARD_KEY_9: return MUCOSA_X11_XK_9; break;
						case MU_KEYBOARD_KEY_A: return MUCOSA_X11_XK_a; break;
						case MU_KEYBOARD_KEY_B: return MUCOSA_X11_XK_b; break;
						case MU_KEYBOARD_KEY_C: return MUCOSA_X11_XK_c; break;
						case MU_KEYBOARD_KEY_D: return MUCOSA_X11_XK_d; break;
						case MU_KEYBOARD_KEY_E: return MUCOSA_X11_XK_e; break;
						case MU_KEYBOARD_KEY_F: return MUCOSA_X11_XK_f; break;
						case MU_KEYBOARD_KEY_G: return MUCOSA_X11_XK_g; break;
						case MU_KEYBOARD_KEY_H: return MUCOSA_X11_XK_h; break;
						case MU_KEYBOARD_KEY_I: return MUCOSA_X11_XK_i; break;
						case MU_KEYBOARD_KEY_J: return MUCOSA_X11_XK_j; break;
						case MU_KEYBOARD_KEY_K: return MUCOSA_X11_XK_k; break;
						case MU_KEYBOARD_KEY_L: return MUCOSA_X11_XK_l; break;
						case MU_KEYBOARD_KEY_M: return MUCOSA_X11_XK_m; break;
						case MU_KEYBOARD_KEY_N: return MUCOSA_X11_XK_n; break;
						case MU_KEYBOARD_KEY_O: return MUCOSA_X11_XK_o; break;
						case MU_KEYBOARD_KEY_P: return MUCOSA_X11_XK_p; break;
						case MU_KEYBOARD_KEY_Q: return MUCOSA_X11_XK_q; break;
						case MU_KEYBOARD_KEY_R: return MUCOSA_X11_XK_r; break;
						case MU_KEYBOARD_KEY_S: return MUCOSA_X11_XK_s; break;
						case MU_KEYBOARD_KEY_T: return MUCOSA_X11_XK_t; break;
						case MU_KEYBOARD_KEY_U: return MUCOSA_X11_XK_u; break;
						case MU_KEYBOARD_KEY_V: return MUCOSA_X11_XK_v; break;
						case MU_KEYBOARD_KEY_W: return MUCOSA_X11_XK_w; break;
						case MU_KEYBOARD_KEY_X: return MUCOSA_X11_XK_x; break;
						case MU_KEYBOARD_KEY_Y: return MUCOSA_X11_XK_y; break;
						case MU_KEYBOARD_KEY_Z: return MUCOSA_X11_XK_z; break;
						case MU_KEYBOARD_KEY_LEFT_WINDOWS: return MUCOSA_X11_XK_Super_L; break;
						case MU_KEYBOARD_KEY_RIGHT_WINDOWS: return MUCOSA_X11_XK_Super_R; break;
						case MU_KEYBOARD_KEY_NUMPAD_0: return MUCOSA_X11_XK_KP_0; break;
						case MU_KEYBOARD_KEY_NUMPAD_1: return MUCOSA_X11_XK_KP_1; break;
						case MU_KEYBOARD_KEY_NUMPAD_2: return MUCOSA_X11_XK_KP_2; break;
						case MU_KEYBOARD_KEY_NUMPAD_3: return MUCOSA_X11_XK_KP_3; break;
						case MU_KEYBOARD_KEY_NUMPAD_4: return MUCOSA_X11_XK_KP_4; break;
						case MU_KEYBOARD_KEY_NUMPAD_5: return MUCOSA_X11_XK_KP_5; break;
						case MU_KEYBOARD_KEY_NUMPAD_6: return MUCOSA_X11_XK_KP_6; break;
						case MU_KEYBOARD_KEY_NUMPAD_7: return MUCOSA_X11_XK_KP_7; break;
						case MU_KEYBOARD_KEY_NUMPAD_8: return MUCOSA_X11_XK_KP_8; break;
						case MU_KEYBOARD_KEY_NUMPAD_9: return MUCOSA_X11_XK_KP_9; break;
						case MU_KEYBOARD_KEY_MULTIPLY: return MUCOSA_X11_XK_KP_Multiply; break;
						case MU_KEYBOARD_KEY_ADD: return MUCOSA_X11_XK_KP_Add; break;
						case MU_KEYBOARD_KEY_SEPARATOR: return MUCOSA_X11_XK_KP_Separator; break;
						case MU_KEYBOARD_KEY_SUBTRACT: return MUCOSA_X11_XK_KP_Subtract; break;
						case MU_KEYBOARD_KEY_DECIMAL: return MUCOSA_X11_XK_KP_Decimal; break;
						case MU_KEYBOARD_KEY_DIVIDE: return MUCOSA_X11_XK_KP_Divide; break;
						case MU_KEYBOARD_KEY_F1: return MUCOSA_X11_XK_F1; break;
						case MU_KEYBOARD_KEY_F2: return MUCOSA_X11_XK_F2; break;
						case MU_KEYBOARD_KEY_F3: return MUCOSA_X11_XK_F3; break;
						case MU_KEYBOARD_KEY_F4: return MUCOSA_X11_XK_F4; break;
						case MU_KEYBOARD_KEY_F5: return MUCOSA_X11_XK_F5; break;
						case MU_KEYBOARD_KEY_F6: return MUCOSA_X11_XK_F6; break;
						case MU_KEYBOARD_KEY_F7: return MUCOSA_X11_XK_F7; break;
						case MU_KEYBOARD_KEY_F8: return MUCOSA_X11_XK_F8; break;
						case MU_KEYBOARD_KEY_F9: return MUCOSA_X11_XK_F9; break;
						case MU_KEYBOARD_KEY_F10: return MUCOSA_X11_XK_F10;break;
						case MU_KEYBOARD_KEY_F11: return MUCOSA_X11_XK_F11;break;
						case MU_KEYBOARD_KEY_F12: return MUCOSA_X11_XK_F12; break;
						case MU_KEYBOARD_KEY_F13: return MUCOSA_X11_XK_F13; break;
						case MU_KEYBOARD_KEY_F14: return MUCOSA_X11_XK_F14; break;
						case MU_KEYBOARD_KEY_F15: return MUCOSA_X11_XK_F15; break;
						case MU_KEYBOARD_KEY_F16: return MUCOSA_X11_XK_F16; break;
						case MU_KEYBOARD_KEY_F17: return MUCOSA_X11_XK_F17; break;
						case MU_KEYBOARD_KEY_F18: return MUCOSA_X11_XK_F18; break;
						case MU_KEYBOARD_KEY_F19: return MUCOSA_X11_XK_F19; break;
						case MU_KEYBOARD_KEY_F20: return MUCOSA_X11_XK_F20; break;
						case MU_KEYBOARD_KEY_F21: return MUCOSA_X11_XK_F21; break;
						case MU_KEYBOARD_KEY_F22: return MUCOSA_X11_XK_F22; break;
						case MU_KEYBOARD_KEY_F23: return MUCOSA_X11_XK_F23; break;
						case MU_KEYBOARD_KEY_F24: return MUCOSA_X11_XK_F24; break;
						case MU_KEYBOARD_KEY_NUMLOCK: return MUCOSA_X11_XK_Num_Lock; break;
						case MU_KEYBOARD_KEY_SCROLL: return MUCOSA_X11_XK_Scroll_Lock; break;
						case MU_KEYBOARD_KEY_LEFT_SHIFT: return MUCOSA_X11_XK_Shift_L; break;
						case MU_KEYBOARD_KEY_RIGHT_SHIFT: return MUCOSA_X11_XK_Shift_R; break;
						case MU_KEYBOARD_KEY_LEFT_CONTROL: return MUCOSA_X11_XK_Control_L; break;
						case MU_KEYBOARD_KEY_RIGHT_CONTROL: return MUCOSA_X11_XK_Control_R; break;
						case MU_KEYBOARD_KEY_LEFT_MENU: return MUCOSA_X11_XK_Menu; break;
						case MU_KEYBOARD_KEY_RIGHT_MENU: return MUCOSA_X11_XK_Menu; break;
						case MU_KEYBOARD_KEY_ATTN: return MUCOSA_X11_XK_3270_Attn; break;
						case MU_KEYBOARD_KEY_CRSEL: return MUCOSA_X11_XK_3270_CursorSelect; break;
						case MU_KEYBOARD_KEY_EXSEL: return MUCOSA_X11_XK_3270_ExSelect; break;
						case MU_KEYBOARD_KEY_EREOF: return MUCOSA_X11_XK_3270_EraseEOF; break;
						case MU_KEYBOARD_KEY_PLAY: return MUCOSA_X11_XK_3270_Play; break;
						case MU_KEYBOARD_KEY_PA1: return MUCOSA_X11_XK_3270_PA1; break;
					}
				}

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

				void muCOSA_X11_keyboard_key_handle_event(muWindow window, muCOSA_X11Window* p_win, unsigned int keycode, muByte down) {
					muKeyboardKey key = muCOSA_X11_XK_key_to_keyboard_key(XkbKeycodeToKeysym(p_win->display, keycode, 0, 0));
					if (key == MU_KEYBOARD_KEY_UNKNOWN) {
						return;
					}

					p_win->input.keyboard_key_states[key-MU_KEYBOARD_KEY_FIRST] = down;

					if (p_win->keyboard_key_callback != MU_NULL_PTR) {
						p_win->keyboard_key_callback(window, key, p_win->input.keyboard_key_states[key-MU_KEYBOARD_KEY_FIRST]);
					}
				}

			/* Cursor */

				// https://tronche.com/gui/x/xlib/appendix/b/
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

				void muCOSA_X11_cursor_handle_event(muWindow window, muCOSA_X11Window* p_win, int button, muByte down) {
					// down = true -> button being pressed down
					// down = false -> button being released
					switch (button) {
						default: break;

						case Button1: {
							p_win->input.mouse_button_states[MU_MOUSE_BUTTON_LEFT-MU_MOUSE_BUTTON_FIRST] = down;
							if (p_win->mouse_button_callback != MU_NULL_PTR) {
								p_win->mouse_button_callback(window, MU_MOUSE_BUTTON_LEFT, down);
							}
						} break;

						case Button2: {
							p_win->input.mouse_button_states[MU_MOUSE_BUTTON_MIDDLE-MU_MOUSE_BUTTON_FIRST] = down;
							if (p_win->mouse_button_callback != MU_NULL_PTR) {
								p_win->mouse_button_callback(window, MU_MOUSE_BUTTON_MIDDLE, down);
							}
						} break;

						case Button3: {
							p_win->input.mouse_button_states[MU_MOUSE_BUTTON_RIGHT-MU_MOUSE_BUTTON_FIRST] = down;
							if (p_win->mouse_button_callback != MU_NULL_PTR) {
								p_win->mouse_button_callback(window, MU_MOUSE_BUTTON_RIGHT, down);
							}
						} break;

						// Not 100% sure if 120 is the appropriate value here for matching with window's WM_MOUSEWHEEL.

						/* Scroll wheel up */
						case Button4: {
							if (!down) {
								p_win->scroll_level += 120;
							}
						} break;

						/* Scroll wheel down */
						case Button5: {
							if (!down) {
								p_win->scroll_level -= 120;
							}
						} break;
					}
				}

			/* Window */

				// https://www.linuxquestions.org/questions/programming-9/how-to-read-the-state-by-using-_net_wm_state-in-xlib-836879/
				Atom muCOSA_X11Window_get_atom_plural(muCOSA_X11Window* p_win, const char** names, size_m namelen) {
					Atom wm_state = XInternAtom(p_win->display, "_NET_WM_STATE", MU_TRUE);
					if (wm_state == None) {
						return None;
					}
					Atom type;
					int format;
					unsigned long n_item = 0, bytes_after;
					unsigned char* properties = 0;
					XGetWindowProperty(
						p_win->display, p_win->window,
						wm_state, 0, (~0L), False, AnyPropertyType,
						&type, &format, &n_item, &bytes_after, &properties
					);

					for (size_m i = 0; i < n_item; i++) {
						Atom prop = ((Atom*)properties)[0];
						char* prop_name = XGetAtomName(p_win->display, prop);
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

				Atom muCOSA_X11Window_get_atom(muCOSA_X11Window* p_win, const char* name) {
					const char* names[1] = { name };
					return muCOSA_X11Window_get_atom_plural(p_win, names, 1);
				}

				// https://stackoverflow.com/questions/36188154/get-x11-window-caption-height
				void muCOSA_X11_inner_window_get_extents(Display* display, Window window, long* left, long* right, long* top, long* bottom) {
					MU_SET_RESULT(left, 0) MU_SET_RESULT(right, 0) MU_SET_RESULT(top, 0) MU_SET_RESULT(bottom, 0)

					Atom a = XInternAtom(display, "_NET_FRAME_EXTENTS", True);
					if (a == None) {
						return;
					}
					Atom t;
					int f;
					unsigned long _n, b;
					unsigned char* data = 0;
					int r = XGetWindowProperty(display, window, 
						a, 0, 4, False, AnyPropertyType, 
						&t, &f, &_n, &b, &data
					);

					if (r == Success && _n == 4 && b == 0) {
						long* extents = (long*)data;
						MU_SET_RESULT(left, extents[0])
						MU_SET_RESULT(right, extents[1])
						MU_SET_RESULT(top, extents[2])
						MU_SET_RESULT(bottom, extents[3])
					}
				}

				void muCOSA_X11Window_get_extents(muCOSA_X11Window* p_win, long* left, long* right, long* top, long* bottom) {
					muCOSA_X11_inner_window_get_extents(p_win->display, p_win->window, left, right, top, bottom);
				}

				void muCOSA_X11Window_update_min_max_dim(muCOSA_X11Window* p_win) {
					*p_win->size_hints = MU_ZERO_STRUCT(XSizeHints);
					p_win->size_hints->flags = PMinSize | PMaxSize;
					p_win->size_hints->min_width = p_win->min_width;
					p_win->size_hints->min_height = p_win->min_height;
					p_win->size_hints->max_width = p_win->min_width;
					p_win->size_hints->max_height = p_win->min_height;
					XSetWMNormalHints(p_win->display, p_win->window, p_win->size_hints);
				}

				void muCOSA_X11_inner_window_get_position(Display* display, Window window, Window par, int* x, int* y) {
					// https://stackoverflow.com/questions/3806872/window-position-in-xlib
					int rx, ry;
					Window child;
					XTranslateCoordinates(display, window, par, 0, 0, &rx, &ry, &child);

					MU_SET_RESULT(x, rx)
					MU_SET_RESULT(y, ry)
				}

				void muCOSA_X11_inner_window_set_position(Display* display, Window window, Window par, int x, int y) {
					if (par) {}
					// https://stackoverflow.com/questions/3806872/window-position-in-xlib
					XWindowAttributes xwa = MU_ZERO_STRUCT(XWindowAttributes);
					XGetWindowAttributes(display, window, &xwa);

					XMoveWindow(display, window, (x-xwa.x), (y-xwa.y));
				}

				muBool muCOSA_X11_inner_get_maximized(muCOSA_X11Window* p_win) {
					static const char* names[2] = { "_NET_WM_STATE_MAXIMIZED_VERT", "_NET_WM_STATE_MAXIMIZED_HORZ" };
					if (muCOSA_X11Window_get_atom_plural(p_win, names, 2) != None) {
						return MU_TRUE;
					}
					return MU_FALSE;
				}

				muBool muCOSA_X11_inner_get_minimized(muCOSA_X11Window* p_win) {
					if (muCOSA_X11Window_get_atom(p_win, "_NET_WM_STATE_HIDDEN") != None) {
						return MU_TRUE;
					}
					return MU_FALSE;
				}

			/* Event */

				muCOSAResult muCOSA_X11Window_handle_event(muWindow window, muCOSA_X11Window* p_win, XEvent event) {
					switch (event.type) {
						default: return MUCOSA_SUCCESS; break;

						/* Closing the window */
						case ClientMessage: {
							if (event.xclient.data.l[0] == (long int)p_win->delete_atom) {
								XUnmapWindow(p_win->display, p_win->window);
								p_win->closed = MU_TRUE;
							}
						} break;

						/* Mouse button press */
						case ButtonPress: {
							muCOSA_X11_cursor_handle_event(window, p_win, event.xbutton.button, MU_TRUE);
						} break;
						/* Mouse button release */
						case ButtonRelease: {
							muCOSA_X11_cursor_handle_event(window, p_win, event.xbutton.button, MU_FALSE);
						} break;

						/* Keyboard key press */
						case KeyPress: {
							muCOSA_X11_keyboard_key_handle_event(window, p_win, event.xkey.keycode, MU_TRUE);
						} break;
						/* Keyboard key release */
						case KeyRelease: {
							muCOSA_X11_keyboard_key_handle_event(window, p_win, event.xkey.keycode, MU_FALSE);
						} break;

						/* Dimension changing */
						case Expose: {
							XWindowAttributes attribs;
							XGetWindowAttributes(p_win->display, p_win->window, &attribs);
							if (p_win->dimensions_callback != MU_NULL_PTR) {
								p_win->dimensions_callback(window, (uint32_m)attribs.width, (uint32_m)attribs.height);
							}
						} break;

						/* Position changing */
						case ConfigureNotify: {
							if (p_win->x != event.xconfigure.x || p_win->y != event.xconfigure.y) {
								p_win->x = (int32_m)event.xconfigure.x;
								p_win->y = (int32_m)event.xconfigure.y;
								if (p_win->position_callback != MU_NULL_PTR) {
									p_win->position_callback(window, p_win->x, p_win->y);
								}
							}
						} break;

						/* Focusing in/out */
						// @TODO Fix FocusIn/FocusOut getting called when window is being dragged
						case FocusIn: {
							if (p_win->focus_callback != MU_NULL_PTR) {
								p_win->focus_callback(window, MU_TRUE);
							}
						} break;
						case FocusOut: {
							if (p_win->focus_callback != MU_NULL_PTR) {
								p_win->focus_callback(window, MU_FALSE);
							}
						} break;
					}

					return MUCOSA_SUCCESS;
				}

				void muCOSA_X11Window_handle_focus_input_flushing(muCOSA_X11Window* p_win) {
					Window focused_window;
					int revert_to;
					XGetInputFocus(p_win->display, &focused_window, &revert_to);

					if (focused_window != p_win->window) {
						p_win->input = MU_ZERO_STRUCT(muCOSA_X11_input);
					}
				}

				void muCOSA_X11Window_handle_min_max_events(muWindow window, muCOSA_X11Window* p_win) {
					muBool maximized = muCOSA_X11_inner_get_maximized(p_win);
					if (p_win->maximized != maximized) {
						p_win->maximized = maximized;
						if (p_win->maximize_callback != MU_NULL_PTR) {
							p_win->maximize_callback(window, maximized);
						}
					}

					muBool minimized = muCOSA_X11_inner_get_minimized(p_win);
					if (p_win->minimized != minimized) {
						p_win->minimized = minimized;
						if (p_win->minimize_callback != MU_NULL_PTR) {
							p_win->minimize_callback(window, minimized);
						}
					}
				}

			/* Keyboard state input */

				void muCOSA_X11_check_state_input(muWindow window, muCOSA_X11Window* p_win, unsigned int n, const char* name, muKeyboardState state) {
					muBool check = (n & (XInternAtom(p_win->display, name, False) - 1)) != 0;
					if (check != p_win->input.keyboard_state_states[state-MU_KEYBOARD_STATE_FIRST]) {
						p_win->input.keyboard_state_states[state-MU_KEYBOARD_STATE_FIRST] = check;

						if (p_win->keyboard_state_callback != MU_NULL_PTR) {
							p_win->keyboard_state_callback(window, state, check);
						}
					}
				}

				void muCOSA_X11_handle_state_input(muWindow window, muCOSA_X11Window* p_win) {
					// http://levonp.blogspot.com/2010/08/retrieving-caps-lock-info-using-xlib.html
					unsigned int n;
					XkbGetIndicatorState(p_win->display, XkbUseCoreKbd, &n);

					muCOSA_X11_check_state_input(window, p_win, n, "Caps Lock", MU_KEYBOARD_STATE_CAPS_LOCK);
					muCOSA_X11_check_state_input(window, p_win, n, "Num Lock", MU_KEYBOARD_STATE_NUM_LOCK);
					// Been having issues with this... being triggered by Caps Lock and Num Lock
					// sometimes... :L
					muCOSA_X11_check_state_input(window, p_win, n, "Scroll Lock", MU_KEYBOARD_STATE_SCROLL_LOCK);
				}

		/* Functions */

			/* Is supported */

				muBool muCOSA_X11_is_supported() {
					return MU_TRUE;
				}

			/* Initiation / Termination */

				struct muCOSA_X11Context {
					muCOSA_X11Window_array windows;
				};
				typedef struct muCOSA_X11Context muCOSA_X11Context;

				void muCOSA_X11_init(muCOSAResult* result, muCOSA_X11Context* c) {
					MU_SET_RESULT(result, MUCOSA_SUCCESS)

					c->windows = MU_ZERO_STRUCT(muCOSA_X11Window_array);
				}

				muWindow muCOSA_X11_window_destroy(muCOSAResult* result, muCOSA_X11Context* c, muWindow window);
				void muCOSA_X11_term(muCOSAResult* result, muCOSA_X11Context* c) {
					MU_SET_RESULT(result, MUCOSA_SUCCESS)

					for (size_m i = 0; i < c->windows.length; i++) {
						muCOSA_X11_window_destroy(0, c, i);
					}
				}

			/* Window */

				/* Creation / Destruction */

					muWindow muCOSA_X11_window_create(muCOSAResult* result, muCOSA_X11Context* c,
						muGraphicsAPI api, muBool (*load_functions)(void),
						muByte* name, uint16_m width, uint16_m height,
						muWindowCreateInfo create_info
					) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						mumaResult muma_res = MUMA_SUCCESS;
						muCOSAResult mu_res = MUCOSA_SUCCESS;

						mu_res = muCOSA_verify_window_create_info(create_info);
						MU_ASSERT(mu_res == MUCOSA_SUCCESS, result, mu_res, return MU_NONE;)

						size_m win = MU_NONE;
						muCOSA_X11Window_find_push(&muma_res, &c->windows, MU_ZERO_STRUCT(muCOSA_X11Window), &win);
						if (muma_res != MUMA_SUCCESS) {
							MU_SET_RESULT(result, muma_result_to_muCOSA_result(muma_res))
							return MU_NONE;
						}
						muCOSA_X11Window_hold_element(&muma_res, &c->windows, win);
						MU_ASSERT(muma_res == MUMA_SUCCESS, result, muma_result_to_muCOSA_result(muma_res), return MU_NONE;)

						c->windows.data[win].size_hints = XAllocSizeHints();
						MU_ASSERT(c->windows.data[win].size_hints != 0, result, MUCOSA_ALLOCATION_FAILED,
							MU_RELEASE(c->windows, win, muCOSA_X11Window_) return MU_NONE;)

						c->windows.data[win].active = MU_FALSE;

						/* Create display */

						c->windows.data[win].display = XOpenDisplay(MU_NULL_PTR);
						MU_ASSERT(c->windows.data[win].display != NULL, result, MUCOSA_FAILED_CONNECTION_TO_SERVER, 
							// Note: OK to just release here, since it's still marked as inactive
							XFree(c->windows.data[win].size_hints);
							MU_RELEASE(c->windows, win, muCOSA_X11Window_) return MU_NONE;)

						/*
						The XCloseDisplay function closes the connection to the X server for the
						display specified in the Display structure and destroys all windows,
						resource IDs (Window, Font, Pixmap, Colormap, Cursor, and GContext), or
						other resources that the client has created on this display, unless the
						close-down mode of the resource has been changed (see XSetCloseDownMode).
						Therefore, these windows, resource IDs, and other resources should never be
						referenced again or an error will be generated. Before exiting, you should
						call XCloseDisplay explicitly so that any pending errors are reported as
						XCloseDisplay performs a final XSync operation. 
						*/
						// https://www.x.org/releases/X11R7.5/doc/man/man3/XOpenDisplay.3.html

						// In other words, just call XCloseDisplay, and everything will be OK.

						/* Create window */

						c->windows.data[win].parent_window = DefaultRootWindow(c->windows.data[win].display);

						MUCOSA_OPENGL_CALL(
							muCOSA_X11_gl_attributes gl_att = MU_ZERO_STRUCT(muCOSA_X11_gl_attributes); if (gl_att.att[2]) {}
							GLXFBConfig* fbcs = 0;
							GLXFBConfig fbc;
							int fbc_count = 0;
						)
						XVisualInfo* vi = 0;
						if (api >= MUCOSA_OPENGL_FIRST && api <= MUCOSA_OPENGL_LAST) {
							MUCOSA_OPENGL_CALL(
								gl_att = muCOSA_X11_get_gl_attributes(create_info.pixel_format);
								fbcs = glXChooseFBConfig(
									c->windows.data[win].display, 
									DefaultScreen(c->windows.data[win].display),
									gl_att.att, &fbc_count
								);
								if (fbcs == 0) {
									MU_SET_RESULT(result, MUCOSA_FAILED_FIND_COMPATIBLE_FRAMEBUFFER)
								} else {
									// @TODO Choose best format
									fbc = fbcs[0];
									XFree(fbcs);

									vi = glXGetVisualFromFBConfig(c->windows.data[win].display, fbc);
									if (vi == 0) {
										MU_SET_RESULT(result, MUCOSA_FAILED_USE_PIXEL_FORMAT)
									}
								}
							)
						}

						// This probably isn't being done right :L
						int depth = 8*3;
						Visual* v = 0;
						unsigned long valuemask = CWEventMask;
						XSetWindowAttributes win_attribs = MU_ZERO_STRUCT(XSetWindowAttributes);
						win_attribs.event_mask = ExposureMask | StructureNotifyMask
						| KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask
						| PointerMotionMask
						;
						if (vi != 0) {
							depth = vi->depth;
							v = vi->visual;

							win_attribs.colormap = XCreateColormap(
								c->windows.data[win].display, c->windows.data[win].parent_window,
								vi->visual, AllocNone
							);
							valuemask |= CWColormap;
						}

						// @TODO Figure out how to tell if this worked or not
						c->windows.data[win].window = XCreateWindow(c->windows.data[win].display,
							c->windows.data[win].parent_window,
							0, 0,
							(unsigned int)width, (unsigned int)height,
							0, // (Border width)
							// (Bits per pixel / "depth")
							depth,
							InputOutput,
							v,
							valuemask, &win_attribs
						);

						XFree(vi);

						/* Create cursor */

						c->windows.data[win].cursor_style = create_info.cursor_style;
						c->windows.data[win].cursor = XCreateFontCursor(c->windows.data[win].display, muCOSA_X11_cursor_get_style(c->windows.data[win].cursor_style));
						XDefineCursor(c->windows.data[win].display, c->windows.data[win].window, c->windows.data[win].cursor);

						/* API initialization */

						if (api >= MUCOSA_OPENGL_FIRST && api <= MUCOSA_OPENGL_LAST) {
							MUCOSA_OPENGL_CALL(
								mu_res = muCOSA_X11_init_opengl(
									c->windows.data[win].display, &c->windows.data[win].gl_context,
									api, fbc
								);
								MU_ASSERT(mu_res == MUCOSA_SUCCESS, result, mu_res, 
									XCloseDisplay(c->windows.data[win].display); MU_RELEASE(c->windows, win, muCOSA_X11Window_) return MU_NONE;)
							)
						} else if (api != MU_NO_GRAPHICS_API) {
							MU_SET_RESULT(result, MUCOSA_UNKNOWN_GRAPHICS_API)
							XFree(c->windows.data[win].size_hints);
							XCloseDisplay(c->windows.data[win].display);
							MU_RELEASE(c->windows, win, muCOSA_X11Window_)
							return MU_NONE;
						}

						/* API loading */

						if (api >= MUCOSA_OPENGL_FIRST && api <= MUCOSA_OPENGL_LAST) {
							MUCOSA_OPENGL_CALL(
								glXMakeCurrent(c->windows.data[win].display, c->windows.data[win].window, c->windows.data[win].gl_context);
							)
						}

						if (load_functions != MU_NULL_PTR) {
							MU_ASSERT(load_functions() == MU_TRUE, result, MUCOSA_FAILED_LOAD_FUNCTIONS, 
								if (api >= MUCOSA_OPENGL_FIRST && api <= MUCOSA_OPENGL_LAST) {
									MUCOSA_OPENGL_CALL(glXDestroyContext(c->windows.data[win].display, c->windows.data[win].gl_context);)
								}
								XFree(c->windows.data[win].size_hints);
								XCloseDisplay(c->windows.data[win].display); MU_RELEASE(c->windows, win, muCOSA_X11Window_) return MU_NONE;)
						}

						/* Name */

						XChangeProperty(
							c->windows.data[win].display, c->windows.data[win].window,
							XInternAtom(c->windows.data[win].display, "_NET_WM_NAME", MU_FALSE),
							XInternAtom(c->windows.data[win].display, "UTF8_STRING", MU_FALSE),
							8, PropModeReplace, (unsigned char*)name, mu_strlen((const char*)name) // <--- strlen or actual UTF-8 char count?
						);

						/* Delete message */

						c->windows.data[win].delete_atom = XInternAtom(c->windows.data[win].display, "WM_DELETE_WINDOW", MU_FALSE);
						XSetWMProtocols(c->windows.data[win].display, c->windows.data[win].window, &c->windows.data[win].delete_atom, 1);

						/* Map */

						if (create_info.visible) {
							XMapWindow(c->windows.data[win].display, c->windows.data[win].window);
						}
						XSync(c->windows.data[win].display, False);

						/* Position correctly */

						for (size_m i = 0; i < 10; i++) {
							muCOSA_X11_inner_window_set_position(c->windows.data[win].display, c->windows.data[win].window, c->windows.data[win].parent_window,
								(int)create_info.x, (int)create_info.y
							);
						}

						/* G i v e */

						c->windows.data[win].active = MU_TRUE;
						c->windows.data[win].api = api;
						c->windows.data[win].closed = MU_FALSE;
						c->windows.data[win].visible = create_info.visible;
						c->windows.data[win].min_width = create_info.min_width;
						c->windows.data[win].min_height = create_info.min_height;
						c->windows.data[win].max_width = create_info.max_width;
						c->windows.data[win].max_height = create_info.max_height;
						c->windows.data[win].scroll_level = 0;
						c->windows.data[win].input = MU_ZERO_STRUCT(muCOSA_X11_input);
						c->windows.data[win].dimensions_callback = create_info.dimensions_callback;
						c->windows.data[win].position_callback = create_info.position_callback;
						c->windows.data[win].focus_callback = create_info.focus_callback;
						c->windows.data[win].maximize_callback = create_info.maximize_callback;
						c->windows.data[win].minimize_callback = create_info.minimize_callback;
						c->windows.data[win].keyboard_key_callback = create_info.keyboard_key_callback;
						c->windows.data[win].keyboard_state_callback = create_info.keyboard_state_callback;
						c->windows.data[win].mouse_button_callback = create_info.mouse_button_callback;
						c->windows.data[win].x = create_info.x;
						c->windows.data[win].y = create_info.y;
						c->windows.data[win].minimized = create_info.minimized;
						c->windows.data[win].maximized = create_info.maximized;
						MU_RELEASE(c->windows, win, muCOSA_X11Window_)
						return win;
					}

					muWindow muCOSA_X11_window_destroy(muCOSAResult* result, muCOSA_X11Context* c, muWindow window) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return window;, muCOSA_X11Window_)

						if (c->windows.data[window].api >= MUCOSA_OPENGL_FIRST && c->windows.data[window].api <= MUCOSA_OPENGL_LAST) {
							MUCOSA_OPENGL_CALL(
								glXDestroyContext(c->windows.data[window].display, c->windows.data[window].gl_context);
							)
						}

						XFree(c->windows.data[window].size_hints);

						XCloseDisplay(c->windows.data[window].display);

						c->windows.data[window].active = MU_FALSE;
						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
						return MU_NONE;
					}

				/* Main loop */

					muBool muCOSA_X11_window_get_closed(muCOSAResult* result, muCOSA_X11Context* c, muWindow window) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return MU_TRUE;, muCOSA_X11Window_)

						muBool closed = c->windows.data[window].closed;

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
						return closed;
					}

					void muCOSA_X11_window_close(muCOSAResult* result, muCOSA_X11Context* c, muWindow window) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						XUnmapWindow(c->windows.data[window].display, c->windows.data[window].window);
						c->windows.data[window].closed = MU_TRUE;

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_update(muCOSAResult* result, muCOSA_X11Context* c, muWindow window) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)
						muCOSAResult mu_res = MUCOSA_SUCCESS;

						while (XPending(c->windows.data[window].display)) {
							XEvent event;
							XNextEvent(c->windows.data[window].display, &event);

							mu_res = muCOSA_X11Window_handle_event(window, &c->windows.data[window], event);
							MU_ASSERT(mu_res == MUCOSA_SUCCESS, result, mu_res, MU_RELEASE(c->windows, window, muCOSA_X11Window_) return;)
						}

						muCOSA_X11_handle_state_input(window, &c->windows.data[window]);
						muCOSA_X11Window_handle_focus_input_flushing(&c->windows.data[window]);
						muCOSA_X11Window_handle_min_max_events(window, &c->windows.data[window]);

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_swap_buffers(muCOSAResult* result, muCOSA_X11Context* c, muWindow window) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						if (c->windows.data[window].api >= MUCOSA_OPENGL_FIRST && c->windows.data[window].api <= MUCOSA_OPENGL_LAST) {
							MUCOSA_OPENGL_CALL(
								glXSwapBuffers(c->windows.data[window].display, c->windows.data[window].window);
							)
						}

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

				/* Get / Set */

					muBool muCOSA_X11_window_get_focused(muCOSAResult* result, muCOSA_X11Context* c, muWindow window) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return MU_FALSE;, muCOSA_X11Window_)

						Window focused;
						int revert_to;
						XGetInputFocus(c->windows.data[window].display, &focused, &revert_to);

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)

						return focused == c->windows.data[window].window;
					}

					void muCOSA_X11_window_focus(muCOSAResult* result, muCOSA_X11Context* c, muWindow window) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						if (muCOSA_X11Window_get_atom(&c->windows.data[window], "_NET_WM_STATE_HIDDEN") != None) {
							// https://stackoverflow.com/questions/30192347/how-to-restore-a-window-with-xlib
							XClientMessageEvent ev = MU_ZERO_STRUCT(XClientMessageEvent); ev.type = ClientMessage; ev.window = c->windows.data[window].window;
							ev.message_type = XInternAtom(c->windows.data[window].display, "_NET_ACTIVE_WINDOW", True);
							MU_ASSERT(ev.message_type != None, result, MUCOSA_UNSUPPORTED_FEATURE, MU_RELEASE(c->windows, window, muCOSA_X11Window_) return;)

							ev.format = 32;
							ev.data.l[0] = 1; ev.data.l[1] = CurrentTime;
							ev.data.l[2] = ev.data.l[3] = ev.data.l[4] = 0;
							XSendEvent(c->windows.data[window].display, c->windows.data[window].parent_window, False, 
								SubstructureRedirectMask | SubstructureNotifyMask, (XEvent*)&ev
							);
							XFlush(c->windows.data[window].display);
						}

						XSetInputFocus(c->windows.data[window].display, c->windows.data[window].window, RevertToPointerRoot, CurrentTime);
						XRaiseWindow(c->windows.data[window].display, c->windows.data[window].window);
						if (c->windows.data[window].focus_callback != MU_NULL_PTR) {
							c->windows.data[window].focus_callback(window, MU_TRUE);
						}

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					muBool muCOSA_X11_window_get_visible(muCOSAResult* result, muCOSA_X11Context* c, muWindow window) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return MU_FALSE;, muCOSA_X11Window_)

						muBool visible = c->windows.data[window].visible;

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)

						return visible;
					}

					void muCOSA_X11_window_set_visible(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, muBool visible) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						if (visible && !c->windows.data[window].visible) {
							XMapWindow(c->windows.data[window].display, c->windows.data[window].window);
							c->windows.data[window].input = MU_ZERO_STRUCT(muCOSA_X11_input);
						} else if (!visible && c->windows.data[window].visible) {
							XUnmapWindow(c->windows.data[window].display, c->windows.data[window].window);
							c->windows.data[window].input = MU_ZERO_STRUCT(muCOSA_X11_input);
						}

						c->windows.data[window].visible = visible;

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_get_position(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, int32_m* x, int32_m* y) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						int rx = 0, ry = 0;
						muCOSA_X11_inner_window_get_position(c->windows.data[window].display, c->windows.data[window].window, c->windows.data[window].parent_window, &rx, &ry);

						MU_SET_RESULT(x, rx) MU_SET_RESULT(y, ry)

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_set_position(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, int32_m x, int32_m y) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						muCOSA_X11_inner_window_set_position(c->windows.data[window].display, c->windows.data[window].window, c->windows.data[window].parent_window, x, y);
						c->windows.data[window].x = x;
						c->windows.data[window].y = y;
						if (c->windows.data[window].position_callback != MU_NULL_PTR) {
							c->windows.data[window].position_callback(window, x, y);
						}

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_get_dimensions(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, uint32_m* width, uint32_m* height) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						XWindowAttributes xwa = MU_ZERO_STRUCT(XWindowAttributes);
						XGetWindowAttributes(c->windows.data[window].display, c->windows.data[window].window, &xwa);

						MU_SET_RESULT(width, xwa.width) MU_SET_RESULT(height, xwa.height)

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_set_dimensions(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, uint32_m width, uint32_m height) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						MU_ASSERT(width >= c->windows.data[window].min_width && width <= c->windows.data[window].max_width &&
							height >= c->windows.data[window].min_height && height <= c->windows.data[window].max_height,
							result, MUCOSA_INVALID_DIMENSIONS, MU_RELEASE(c->windows, window, muCOSA_X11Window_) return;
						)

						XResizeWindow(c->windows.data[window].display, c->windows.data[window].window, width, height);
						if (c->windows.data[window].dimensions_callback != MU_NULL_PTR) {
							c->windows.data[window].dimensions_callback(window, width, height);
						}

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					muBool muCOSA_X11_window_get_maximized(muCOSAResult* result, muCOSA_X11Context* c, muWindow window) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return MU_FALSE;, muCOSA_X11Window_)

						muBool maximized = muCOSA_X11_inner_get_maximized(&c->windows.data[window]);

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)

						return maximized;
					}

					void muCOSA_X11_window_set_maximized(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, muBool maximized) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						// https://forum.juce.com/t/how-to-maximize-not-fullscreen/28346
						XClientMessageEvent ev = MU_ZERO_STRUCT(XClientMessageEvent); ev.type = ClientMessage; ev.window = c->windows.data[window].window;
						ev.message_type = XInternAtom(c->windows.data[window].display, "_NET_WM_STATE", True);
						MU_ASSERT(ev.message_type != None, result, MUCOSA_UNSUPPORTED_FEATURE, MU_RELEASE(c->windows, window, muCOSA_X11Window_) return;)
						ev.format = 32;
						ev.data.l[0] = maximized;
						ev.data.l[1] = XInternAtom(c->windows.data[window].display, "_NET_WM_STATE_MAXIMIZED_VERT", False);
						ev.data.l[2] = XInternAtom(c->windows.data[window].display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
						ev.data.l[3] = 1; ev.data.l[4] = 0;
						XSendEvent(c->windows.data[window].display, c->windows.data[window].parent_window,
							False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent*)&ev
						);
						XFlush(c->windows.data[window].display);

						if (c->windows.data[window].maximize_callback != MU_NULL_PTR) {
							c->windows.data[window].maximize_callback(window, maximized);
						}

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					muBool muCOSA_X11_window_get_minimized(muCOSAResult* result, muCOSA_X11Context* c, muWindow window) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return MU_FALSE;, muCOSA_X11Window_)

						muBool minimized = muCOSA_X11_inner_get_minimized(&c->windows.data[window]);

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)

						return minimized;
					}

					void muCOSA_X11_window_set_minimized(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, muBool minimized) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						XClientMessageEvent ev = MU_ZERO_STRUCT(XClientMessageEvent); ev.type = ClientMessage; ev.window = c->windows.data[window].window;
						if (minimized) {
							ev.message_type = XInternAtom(c->windows.data[window].display, "WM_CHANGE_STATE", False);
							ev.format = 32;
							ev.data.l[0] = IconicState;
						} else {
							ev.message_type = XInternAtom(c->windows.data[window].display, "_NET_ACTIVE_WINDOW", False);
							ev.format = 32;
							ev.data.l[0] = 1;
							ev.data.l[1] = CurrentTime;
							ev.data.l[2] = ev.data.l[3] = ev.data.l[4] = 0;
						}
						XSendEvent(c->windows.data[window].display, c->windows.data[window].parent_window,
							False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent*)&ev
						);
						XFlush(c->windows.data[window].display);

						if (c->windows.data[window].minimize_callback != MU_NULL_PTR) {
							c->windows.data[window].minimize_callback(window, minimized);
						}

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_get_minimum_dimensions(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, uint32_m* min_width, uint32_m* min_height) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						MU_SET_RESULT(min_width, c->windows.data[window].min_width)
						MU_SET_RESULT(min_height, c->windows.data[window].min_height)

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_set_minimum_dimensions(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, uint32_m min_width, uint32_m min_height) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						c->windows.data[window].min_width = min_width;
						c->windows.data[window].min_height = min_height;
						muCOSA_X11Window_update_min_max_dim(&c->windows.data[window]);

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_get_maximum_dimensions(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, uint32_m* max_width, uint32_m* max_height) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						MU_SET_RESULT(max_width, c->windows.data[window].max_width)
						MU_SET_RESULT(max_height, c->windows.data[window].max_height)

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_set_maximum_dimensions(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, uint32_m max_width, uint32_m max_height) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						c->windows.data[window].max_width = max_width;
						c->windows.data[window].max_height = max_height;
						muCOSA_X11Window_update_min_max_dim(&c->windows.data[window]);

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_get_cursor_position(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, int32_m* x, int32_m* y) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						// https://stackoverflow.com/questions/3585871/how-can-i-get-the-current-mouse-pointer-position-co-ordinates-in-x
						int screen_count = XScreenCount(c->windows.data[window].display);

						Bool oresult = False;
						int root_x, root_y;
						for (int i = 0; i < screen_count; i++) {
							Window root_window = XRootWindow(c->windows.data[window].display, i);

							Window window_ret, window_chi;
							int win_x, win_y;
							unsigned int mask_return;
							oresult = XQueryPointer(c->windows.data[window].display, root_window, 
								&window_ret, &window_chi, &root_x, &root_y, &win_x, &win_y, &mask_return
							);
							// "window_ret" is the window the cursor is located in
							// "root_x" and "root_y" are cursor pos relative to "window_ret"
							// "win_x" and "win_y" are cursor pos relative to "root_window" (which I'm pretty sure is the whole display)

							if (oresult == True) {
								int wx, wy;
								muCOSA_X11_inner_window_get_position(c->windows.data[window].display, c->windows.data[window].window, c->windows.data[window].parent_window, &wx, &wy);

								// I really don't get how this works, lol...
								MU_SET_RESULT(x, win_x - wx)
								MU_SET_RESULT(y, win_y - wy)

								MU_RELEASE(c->windows, window, muCOSA_X11Window_)
								return;
							}
						}

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)

						MU_SET_RESULT(result, MUCOSA_NONEXISTENT_DEVICE)
					}

					void muCOSA_X11_window_set_cursor_position(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, int32_m x, int32_m y) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						XWarpPointer(c->windows.data[window].display, c->windows.data[window].window, c->windows.data[window].window, 0, 0, 0, 0, x, y);

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					muCursorStyle muCOSA_X11_window_get_cursor_style(muCOSAResult* result, muCOSA_X11Context* c, muWindow window) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return MU_CURSOR_STYLE_UNKNOWN;, muCOSA_X11Window_)

						muCursorStyle style = c->windows.data[window].cursor_style;

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)

						return style;
					}

					void muCOSA_X11_window_set_cursor_style(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, muCursorStyle style) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						XUndefineCursor(c->windows.data[window].display, c->windows.data[window].window);
						XFreeCursor(c->windows.data[window].display, c->windows.data[window].cursor);

						c->windows.data[window].cursor_style = muCOSA_X11_cursor_get_style(style);
						c->windows.data[window].cursor = XCreateFontCursor(c->windows.data[window].display, c->windows.data[window].cursor_style);
						XDefineCursor(c->windows.data[window].display, c->windows.data[window].window, c->windows.data[window].cursor);

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					int32_m muCOSA_X11_window_get_scroll_level(muCOSAResult* result, muCOSA_X11Context* c, muWindow window) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return 0;, muCOSA_X11Window_)

						int32_m scroll_level = c->windows.data[window].scroll_level;

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)

						return scroll_level;
					}

					void muCOSA_X11_window_set_scroll_level(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, int32_m scroll_level) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						c->windows.data[window].scroll_level = scroll_level;

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

				/* Get */

					void muCOSA_X11_window_get_frame_extents(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, uint32_m* left, uint32_m* right, uint32_m* top, uint32_m* bottom) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						long l_left=0, l_right=0, l_top=0, l_bottom=0;
						muCOSA_X11Window_get_extents(&c->windows.data[window], &l_left, &l_right, &l_top, &l_bottom);
						MU_SET_RESULT(left, (uint32_m)l_left)
						MU_SET_RESULT(right, (uint32_m)l_right)
						MU_SET_RESULT(top, (uint32_m)l_top)
						MU_SET_RESULT(bottom, (uint32_m)l_bottom)

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					muButtonState muCOSA_X11_window_get_keyboard_key_state(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, muKeyboardKey key) {
						MU_ASSERT(key >= MU_KEYBOARD_KEY_FIRST && key <= MU_KEYBOARD_KEY_LAST, result, MUCOSA_UNKNOWN_KEYBOARD_KEY, return 0;)

						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return 0;, muCOSA_X11Window_)

						muButtonState state = c->windows.data[window].input.keyboard_key_states[key-MU_KEYBOARD_KEY_FIRST];

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)

						return state;
					}

					muState muCOSA_X11_window_get_keyboard_state_state(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, muKeyboardState state) {
						MU_ASSERT(state >= MU_KEYBOARD_STATE_FIRST && state <= MU_KEYBOARD_STATE_LAST, result, MUCOSA_UNKNOWN_KEYBOARD_STATE, return 0;)

						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return 0;, muCOSA_X11Window_)

						muState ret_state = c->windows.data[window].input.keyboard_state_states[state-MU_KEYBOARD_STATE_FIRST];

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)

						return ret_state;
					}

					muButtonState muCOSA_X11_window_get_mouse_button_state(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, muMouseButton button) {
						MU_ASSERT(button >= MU_MOUSE_BUTTON_FIRST && button <= MU_MOUSE_BUTTON_LAST, result, MUCOSA_UNKNOWN_MOUSE_BUTTON, return 0;)

						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return 0;, muCOSA_X11Window_)

						muButtonState state = c->windows.data[window].input.mouse_button_states[button-MU_MOUSE_BUTTON_FIRST];

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)

						return state;
					}

				/* Set */

					void muCOSA_X11_window_set_title(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, muByte* title) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						XChangeProperty(c->windows.data[window].display, c->windows.data[window].window,
							XInternAtom(c->windows.data[window].display, "_NET_WM_NAME", False),
							XInternAtom(c->windows.data[window].display, "UTF8_STRING", False),
							8, PropModeReplace, (unsigned char*)title, (int)mu_strlen((const char*)title)
						);

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_set_dimensions_callback(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, void (*callback)(muWindow window, uint32_m width, uint32_m height)) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						c->windows.data[window].dimensions_callback = callback;

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_set_position_callback(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, void (*callback)(muWindow window, int32_m x, int32_m y)) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						c->windows.data[window].position_callback = callback;

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_set_focus_callback(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, void (*callback)(muWindow window, muBool focused)) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						c->windows.data[window].focus_callback = callback;

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_set_maximize_callback(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, void (*callback)(muWindow window, muBool maximized)) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						c->windows.data[window].maximize_callback = callback;

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_set_minimize_callback(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, void (*callback)(muWindow window, muBool minimized)) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						c->windows.data[window].minimize_callback = callback;

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_set_keyboard_key_callback(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, void (*callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state)) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						c->windows.data[window].keyboard_key_callback = callback;

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_set_keyboard_state_callback(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, void (*callback)(muWindow window, muKeyboardState keyboard_state, muState state)) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						c->windows.data[window].keyboard_state_callback = callback;

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

					void muCOSA_X11_window_set_mouse_button_callback(muCOSAResult* result, muCOSA_X11Context* c, muWindow window, void (*callback)(muWindow window, muMouseButton mouse_button, muButtonState state)) {
						MU_SET_RESULT(result, MUCOSA_SUCCESS)
						MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

						c->windows.data[window].mouse_button_callback = callback;

						MU_RELEASE(c->windows, window, muCOSA_X11Window_)
					}

			/* OpenGL */

				void muCOSA_X11_opengl_bind_window(muCOSAResult* result, muCOSA_X11Context* c, muWindow window) {
					MU_SET_RESULT(result, MUCOSA_SUCCESS)
					MU_HOLD(result, window, c->windows, muCOSA_global_context, MUCOSA_, return;, muCOSA_X11Window_)

					if (c->windows.data[window].api >= MUCOSA_OPENGL_FIRST && c->windows.data[window].api <= MUCOSA_OPENGL_LAST) {
						MUCOSA_OPENGL_CALL(
							glXMakeCurrent(c->windows.data[window].display, c->windows.data[window].window, c->windows.data[window].gl_context);
						)
					}

					MU_RELEASE(c->windows, window, muCOSA_X11Window_)
				}

				void* muCOSA_X11_opengl_get_function_address(const muByte* name) {
					MUCOSA_OPENGL_CALL(
						void(*addr)() = glXGetProcAddress((const GLubyte*)name);
						void* ret = 0;
						mu_memcpy(&ret, &addr, sizeof(void*));
						return ret;
					)
					if (name) {}
					return 0;
				}

	#else
		#define MUCOSA_X11_CALL(...)
	#endif  /* MUCOSA_X11 */

	/* API-level functions */

		/* Names */

			#ifdef MUCOSA_NAMES
				MUDEF const char* muCOSA_result_get_name(muCOSAResult result) {
					switch (result) {
						default: return "MUCOSA_UNKNOWN"; break;
						case MUCOSA_SUCCESS: return "MUCOSA_SUCCESS"; break;
						case MUCOSA_ALREADY_INITIALIZED: return "MUCOSA_ALREADY_INITIALIZED"; break;
						case MUCOSA_ALREADY_TERMINATED: return "MUCOSA_ALREADY_TERMINATED"; break;
						case MUCOSA_NOT_YET_INITIALIZED: return "MUCOSA_NOT_YET_INITIALIZED"; break;
						case MUCOSA_ALLOCATION_FAILED: return "MUCOSA_ALLOCATION_FAILED"; break;
						case MUCOSA_UNKNOWN_WINDOW_SYSTEM: return "MUCOSA_UNKNOWN_WINDOW_SYSTEM"; break;
						case MUCOSA_UNKNOWN_GRAPHICS_API: return "MUCOSA_UNKNOWN_GRAPHICS_API"; break;
						case MUCOSA_UNKNOWN_KEYBOARD_KEY: return "MUCOSA_UNKNOWN_KEYBOARD_KEY"; break;
						case MUCOSA_UNKNOWN_KEYBOARD_STATE: return "MUCOSA_UNKNOWN_KEYBOARD_STATE"; break;
						case MUCOSA_UNKNOWN_MOUSE_BUTTON: return "MUCOSA_UNKNOWN_MOUSE_BUTTON"; break;
						case MUCOSA_UNSUPPORTED_WINDOW_SYSTEM: return "MUCOSA_UNSUPPORTED_WINDOW_SYSTEM"; break;
						case MUCOSA_UNSUPPORTED_FEATURE: return "MUCOSA_UNSUPPORTED_FEATURE"; break;
						case MUCOSA_FAILED_CONNECTION_TO_SERVER: return "MUCOSA_FAILED_CONNECTION_TO_SERVER"; break;
						case MUCOSA_FAILED_CREATE_WINDOW: return "MUCOSA_FAILED_CREATE_WINDOW"; break;
						case MUCOSA_FAILED_LOAD_FUNCTIONS: return "MUCOSA_FAILED_LOAD_FUNCTIONS"; break;
						case MUCOSA_FAILED_FIND_COMPATIBLE_FRAMEBUFFER: return "MUCOSA_FAILED_FIND_COMPATIBLE_FRAMEBUFFER"; break;
						case MUCOSA_FAILED_CREATE_OPENGL_CONTEXT: return "MUCOSA_FAILED_CREATE_OPENGL_CONTEXT"; break;
						case MUCOSA_FAILED_USE_PIXEL_FORMAT: return "MUCOSA_FAILED_USE_PIXEL_FORMAT"; break;
						case MUCOSA_INVALID_MINIMUM_MAXIMUM_BOOLS: return "MUCOSA_INVALID_MINIMUM_MAXIMUM_BOOLS"; break;
						case MUCOSA_INVALID_MINIMUM_MAXIMUM_DIMENSIONS: return "MUCOSA_INVALID_MINIMUM_MAXIMUM_DIMENSIONS"; break;
						case MUCOSA_INVALID_ID: return "MUCOSA_INVALID_ID"; break;
						case MUCOSA_INVALID_SAMPLE_COUNT: return "MUCOSA_INVALID_SAMPLE_COUNT"; break;
						case MUCOSA_INVALID_DIMENSIONS: return "MUCOSA_INVALID_DIMENSIONS"; break;
						case MUCOSA_MUMA_SUCCESS: return "MUCOSA_MUMA_SUCCESS"; break;
						case MUCOSA_MUMA_FAILED_TO_ALLOCATE: return "MUCOSA_MUMA_FAILED_TO_ALLOCATE"; break;
						case MUCOSA_MUMA_INVALID_INDEX: return "MUCOSA_MUMA_INVALID_INDEX"; break;
						case MUCOSA_MUMA_INVALID_SHIFT_AMOUNT: return "MUCOSA_MUMA_INVALID_SHIFT_AMOUNT"; break;
						case MUCOSA_MUMA_NOT_FOUND: return "MUCOSA_MUMA_NOT_FOUND"; break;
					}
				}

				MUDEF const char* muCOSA_keyboard_key_get_name(muKeyboardKey key) {
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

				MUDEF const char* muCOSA_keyboard_key_get_nice_name(muKeyboardKey key) {
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
			#endif

		/* Initiation / Termination */

			struct muCOSAContext {
				muWindowSystem window_system;
				#define MUCOSA_GWINSYS muCOSA_global_context->window_system
				MUCOSA_X11_CALL(muCOSA_X11Context X11;)
				#define MUCOSA_GX11 muCOSA_global_context->X11
			};

			muCOSAContext* muCOSA_global_context = MU_NULL_PTR;

			MUDEF void muCOSA_init(muCOSAResult* result, muWindowSystem window_system) {
				MU_SET_RESULT(result, MUCOSA_SUCCESS)

				MU_ASSERT(muCOSA_global_context == MU_NULL_PTR, result, MUCOSA_ALREADY_INITIALIZED, return;)

				muCOSA_global_context = (muCOSAContext*)mu_malloc(sizeof(muCOSAContext));
				MU_ASSERT(muCOSA_global_context != 0, result, MUCOSA_ALLOCATION_FAILED, return;)

				MUCOSA_X11_CALL(MUCOSA_GX11 = MU_ZERO_STRUCT(muCOSA_X11Context);)

				if (window_system == MU_AUTO_WINDOW_SYSTEM) {
					#ifdef MU_UNIX
						MUCOSA_X11_CALL(
							if (muCOSA_X11_is_supported()) {
								window_system = MU_X11;
							}
						)
					#endif
				} else {
					switch (window_system) {
						default: MU_SET_RESULT(result, MUCOSA_UNKNOWN_WINDOW_SYSTEM) mu_free(muCOSA_global_context); return; break;
						MUCOSA_X11_CALL(case MU_X11: MU_ASSERT(muCOSA_X11_is_supported(), result, MUCOSA_UNSUPPORTED_WINDOW_SYSTEM, mu_free(muCOSA_global_context); return;) break;)
					}
				}

				switch (window_system) {
					default: MU_SET_RESULT(result, MUCOSA_UNSUPPORTED_WINDOW_SYSTEM) mu_free(muCOSA_global_context); return; break;

					MUCOSA_X11_CALL(case MU_X11: {
						muCOSAResult res = MUCOSA_SUCCESS;
						muCOSA_X11_init(&res, &MUCOSA_GX11);
						MU_ASSERT(res == MUCOSA_SUCCESS, result, res, mu_free(muCOSA_global_context); return;)
						MUCOSA_GWINSYS = MU_X11;
						return;
					} break;)
				}
			}

			MUDEF void muCOSA_term(muCOSAResult* result) {
				MU_SET_RESULT(result, MUCOSA_SUCCESS)
				MU_ASSERT(muCOSA_global_context != MU_NULL_PTR, result, MUCOSA_ALREADY_TERMINATED, return;)

				switch (MUCOSA_GWINSYS) {
					default: return; break;

					MUCOSA_X11_CALL(case MU_X11: {
						muCOSAResult res = MUCOSA_SUCCESS;
						muCOSA_X11_term(&res, &MUCOSA_GX11);
						MU_ASSERT(res == MUCOSA_SUCCESS, result, res, return;)
					} break;)
				}

				mu_free(muCOSA_global_context);
				muCOSA_global_context = MU_NULL_PTR;
			}

		/* Window */

			/* Creation / Destruction */

				MUDEF muWindowCreateInfo mu_window_default_create_info() {
					muWindowCreateInfo ci = MU_ZERO_STRUCT(muWindowCreateInfo);

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
					ci.min_width = 120; // ? Does it work on Windows?
					ci.min_height = 1; // ? Does it work on Windows?
					ci.max_width = 30720;
					ci.max_height = 17280;
					ci.cursor_style = MU_CURSOR_STYLE_DEFAULT;

					ci.dimensions_callback = MU_NULL_PTR;
					ci.position_callback = MU_NULL_PTR;
					ci.focus_callback = MU_NULL_PTR;
					ci.maximize_callback = MU_NULL_PTR;
					ci.minimize_callback = MU_NULL_PTR;

					ci.keyboard_key_callback = MU_NULL_PTR;
					ci.keyboard_state_callback = MU_NULL_PTR;

					return ci;
				}

				MUDEF muWindow mu_window_create(muCOSAResult* result, 
					muGraphicsAPI api, muBool (*load_functions)(void),
					muByte* name, uint16_m width, uint16_m height,
					muWindowCreateInfo create_info
				) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return MU_NONE;)

					MU_ASSERT(width >= create_info.min_width && width <= create_info.max_width &&
						height >= create_info.min_height && height <= create_info.max_height,
						result, MUCOSA_INVALID_DIMENSIONS, return MU_NONE;
					)

					switch (MUCOSA_GWINSYS) {
						default: return MU_NONE; break;

						MUCOSA_X11_CALL(case MU_X11: {
							return muCOSA_X11_window_create(result, &MUCOSA_GX11, api, load_functions, name, width, height, create_info);
						} break;)
					}
				}

				MUDEF muWindow mu_window_destroy(muCOSAResult* result, muWindow window) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return window;)

					switch (MUCOSA_GWINSYS) {
						default: return window; break;

						MUCOSA_X11_CALL(case MU_X11: {
							return muCOSA_X11_window_destroy(result, &MUCOSA_GX11, window);
						} break;)
					}
				}

			/* Main loop */

				MUDEF muBool mu_window_get_closed(muCOSAResult* result, muWindow window) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return MU_TRUE;)

					switch (MUCOSA_GWINSYS) {
						default: return window; break;

						MUCOSA_X11_CALL(case MU_X11: {
							return muCOSA_X11_window_get_closed(result, &MUCOSA_GX11, window);
						} break;)
					}
				}

				MUDEF void mu_window_close(muCOSAResult* result, muWindow window) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_close(result, &MUCOSA_GX11, window);
						} break;)
					}
				}

				MUDEF void mu_window_update(muCOSAResult* result, muWindow window) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_update(result, &MUCOSA_GX11, window);
						} break;)
					}
				}

				MUDEF void mu_window_swap_buffers(muCOSAResult* result, muWindow window) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_swap_buffers(result, &MUCOSA_GX11, window);
						} break;)
					}
				}

			/* Get / Set */

				MUDEF muBool mu_window_get_focused(muCOSAResult* result, muWindow window) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return MU_FALSE;)

					switch (MUCOSA_GWINSYS) {
						default: return MU_FALSE; break;

						MUCOSA_X11_CALL(case MU_X11: {
							return muCOSA_X11_window_get_focused(result, &MUCOSA_GX11, window);
						} break;)
					}
				}

				MUDEF void mu_window_focus(muCOSAResult* result, muWindow window) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_focus(result, &MUCOSA_GX11, window);
						} break;)
					}
				}

				MUDEF muBool mu_window_get_visible(muCOSAResult* result, muWindow window) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return MU_FALSE;)

					switch (MUCOSA_GWINSYS) {
						default: return MU_FALSE; break;

						MUCOSA_X11_CALL(case MU_X11: {
							return muCOSA_X11_window_get_visible(result, &MUCOSA_GX11, window);
						} break;)
					}
				}

				MUDEF void mu_window_set_visible(muCOSAResult* result, muWindow window, muBool visible) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_visible(result, &MUCOSA_GX11, window, visible);
						} break;)
					}
				}

				MUDEF void mu_window_get_position(muCOSAResult* result, muWindow window, int32_m* x, int32_m* y) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_get_position(result, &MUCOSA_GX11, window, x, y);
						} break;)
					}
				}

				MUDEF void mu_window_set_position(muCOSAResult* result, muWindow window, int32_m x, int32_m y) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_position(result, &MUCOSA_GX11, window, x, y);
						} break;)
					}
				}

				MUDEF void mu_window_get_dimensions(muCOSAResult* result, muWindow window, uint32_m* width, uint32_m* height) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_get_dimensions(result, &MUCOSA_GX11, window, width, height);
						} break;)
					}
				}

				MUDEF void mu_window_set_dimensions(muCOSAResult* result, muWindow window, uint32_m width, uint32_m height) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_dimensions(result, &MUCOSA_GX11, window, width, height);
						} break;)
					}
				}

				MUDEF muBool mu_window_get_maximized(muCOSAResult* result, muWindow window) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return MU_FALSE;)

					switch (MUCOSA_GWINSYS) {
						default: return MU_FALSE; break;

						MUCOSA_X11_CALL(case MU_X11: {
							return muCOSA_X11_window_get_maximized(result, &MUCOSA_GX11, window);
						} break;)
					}
				}

				MUDEF void mu_window_set_maximized(muCOSAResult* result, muWindow window, muBool maximized) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_maximized(result, &MUCOSA_GX11, window, maximized);
						} break;)
					}
				}

				MUDEF muBool mu_window_get_minimized(muCOSAResult* result, muWindow window) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return MU_FALSE;)

					switch (MUCOSA_GWINSYS) {
						default: return MU_FALSE; break;

						MUCOSA_X11_CALL(case MU_X11: {
							return muCOSA_X11_window_get_minimized(result, &MUCOSA_GX11, window);
						} break;)
					}
				}

				MUDEF void mu_window_set_minimized(muCOSAResult* result, muWindow window, muBool minimized) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_minimized(result, &MUCOSA_GX11, window, minimized);
						} break;)
					}
				}

				MUDEF void mu_window_get_minimum_dimensions(muCOSAResult* result, muWindow window, uint32_m* min_width, uint32_m* min_height) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_get_minimum_dimensions(result, &MUCOSA_GX11, window, min_width, min_height);
						} break;)
					}
				}

				MUDEF void mu_window_set_minimum_dimensions(muCOSAResult* result, muWindow window, uint32_m min_width, uint32_m min_height) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_minimum_dimensions(result, &MUCOSA_GX11, window, min_width, min_height);
						} break;)
					}
				}

				MUDEF void mu_window_get_maximum_dimensions(muCOSAResult* result, muWindow window, uint32_m* max_width, uint32_m* max_height) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_get_maximum_dimensions(result, &MUCOSA_GX11, window, max_width, max_height);
						} break;)
					}
				}

				MUDEF void mu_window_set_maximum_dimensions(muCOSAResult* result, muWindow window, uint32_m max_width, uint32_m max_height) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_maximum_dimensions(result, &MUCOSA_GX11, window, max_width, max_height);
						} break;)
					}
				}

				MUDEF void mu_window_get_cursor_position(muCOSAResult* result, muWindow window, int32_m* x, int32_m* y) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_get_cursor_position(result, &MUCOSA_GX11, window, x, y);
						} break;)
					}
				}

				MUDEF void mu_window_set_cursor_position(muCOSAResult* result, muWindow window, int32_m x, int32_m y) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_cursor_position(result, &MUCOSA_GX11, window, x, y);
						} break;)
					}
				}

				MUDEF muCursorStyle mu_window_get_cursor_style(muCOSAResult* result, muWindow window) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return MU_CURSOR_STYLE_UNKNOWN;)

					switch (MUCOSA_GWINSYS) {
						default: return MU_CURSOR_STYLE_UNKNOWN; break;

						MUCOSA_X11_CALL(case MU_X11: {
							return muCOSA_X11_window_get_cursor_style(result, &MUCOSA_GX11, window);
						} break;)
					}
				}

				MUDEF void mu_window_set_cursor_style(muCOSAResult* result, muWindow window, muCursorStyle style) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_cursor_style(result, &MUCOSA_GX11, window, style);
						} break;)
					}
				}

				MUDEF int32_m mu_window_get_scroll_level(muCOSAResult* result, muWindow window) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return 0;)

					switch (MUCOSA_GWINSYS) {
						default: return 0; break;

						MUCOSA_X11_CALL(case MU_X11: {
							return muCOSA_X11_window_get_scroll_level(result, &MUCOSA_GX11, window);
						} break;)
					}
				}

				MUDEF void mu_window_set_scroll_level(muCOSAResult* result, muWindow window, int32_m scroll_level) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_scroll_level(result, &MUCOSA_GX11, window, scroll_level);
						} break;)
					}
				}

			/* Get */

				MUDEF void mu_window_get_frame_extents(muCOSAResult* result, muWindow window, uint32_m* left, uint32_m* right, uint32_m* top, uint32_m* bottom) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_get_frame_extents(result, &MUCOSA_GX11, window, left, right, top, bottom);
						} break;)
					}
				}

				MUDEF muButtonState mu_window_get_keyboard_key_state(muCOSAResult* result, muWindow window, muKeyboardKey key) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return 0;)

					switch (MUCOSA_GWINSYS) {
						default: return 0; break;

						MUCOSA_X11_CALL(case MU_X11: {
							return muCOSA_X11_window_get_keyboard_key_state(result, &MUCOSA_GX11, window, key);
						} break;)
					}
				}

				MUDEF muState mu_window_get_keyboard_state_state(muCOSAResult* result, muWindow window, muKeyboardState state) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return 0;)

					switch (MUCOSA_GWINSYS) {
						default: return 0; break;

						MUCOSA_X11_CALL(case MU_X11: {
							return muCOSA_X11_window_get_keyboard_state_state(result, &MUCOSA_GX11, window, state);
						} break;)
					}
				}

				MUDEF muButtonState mu_window_get_mouse_button_state(muCOSAResult* result, muWindow window, muMouseButton button) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return 0;)

					switch (MUCOSA_GWINSYS) {
						default: return 0; break;

						MUCOSA_X11_CALL(case MU_X11: {
							return muCOSA_X11_window_get_mouse_button_state(result, &MUCOSA_GX11, window, button);
						} break;)
					}
				}

			/* Set */

				MUDEF void mu_window_set_title(muCOSAResult* result, muWindow window, muByte* title) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_title(result, &MUCOSA_GX11, window, title);
						} break;)
					}
				}

				MUDEF void mu_window_set_dimensions_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, uint32_m width, uint32_m height)) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_dimensions_callback(result, &MUCOSA_GX11, window, callback);
						} break;)
					}
				}

				MUDEF void mu_window_set_position_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m x, int32_m y)) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_position_callback(result, &MUCOSA_GX11, window, callback);
						} break;)
					}
				}

				MUDEF void mu_window_set_focus_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool focused)) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_focus_callback(result, &MUCOSA_GX11, window, callback);
						} break;)
					}
				}

				MUDEF void mu_window_set_maximize_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool maximized)) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_maximize_callback(result, &MUCOSA_GX11, window, callback);
						} break;)
					}
				}

				MUDEF void mu_window_set_minimize_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool minimized)) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_minimize_callback(result, &MUCOSA_GX11, window, callback);
						} break;)
					}
				}

				MUDEF void mu_window_set_keyboard_key_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state)) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_keyboard_key_callback(result, &MUCOSA_GX11, window, callback);
						} break;)
					}
				}

				MUDEF void mu_window_set_keyboard_state_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardState keyboard_state, muState state)) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_keyboard_state_callback(result, &MUCOSA_GX11, window, callback);
						} break;)
					}
				}

				MUDEF void mu_window_set_mouse_button_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muMouseButton mouse_button, muButtonState state)) {
					MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

					switch (MUCOSA_GWINSYS) {
						default: return; break;

						MUCOSA_X11_CALL(case MU_X11: {
							muCOSA_X11_window_set_mouse_button_callback(result, &MUCOSA_GX11, window, callback);
						} break;)
					}
				}

		/* OpenGL */

			MUDEF void mu_opengl_bind_window(muCOSAResult* result, muWindow window) {
				MU_SAFEFUNC(result, MUCOSA_, muCOSA_global_context, return;)

				switch (MUCOSA_GWINSYS) {
					default: return; break;

					MUCOSA_X11_CALL(case MU_X11: {
						muCOSA_X11_opengl_bind_window(result, &MUCOSA_GX11, window);
					} break;)
				}
			}

			MUDEF void* mu_opengl_get_function_address(const muByte* name) {
				if (muCOSA_global_context == 0) {
					return 0;
				}

				switch (MUCOSA_GWINSYS) {
					default: return 0; break;

					MUCOSA_X11_CALL(case MU_X11: {
						return muCOSA_X11_opengl_get_function_address(name);
					} break;)
				}
			}

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

