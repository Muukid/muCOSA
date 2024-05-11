# muCOSA

muCOSA (COSA standing for cross-operating-system API) is a public domain header-only single-file C library for interacting with operating systems with a cross-platform API. To use it, download the `muCOSA.h` file, add it to your include path, and include it like so:

```c
#define MUCOSA_IMPLEMENTATION
#include <muCOSA.h>
```

More information about the general structure of a mu library is provided at [the mu library information GitHub repository.](https://github.com/Muukid/mu-library-information)

***WARNING!*** Because this library hasn't had an official release as of yet, it is still considered to be in a generally unsafe state.

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

while (!mu_window_get_closed(0, window)) {
	// ... (This is where the frame-by-frame logic would go) ...

	mu_window_swap_buffers(0, window);
	mu_window_update(0, window);
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

2. Non-Latin input; OS's usually have some way of inputting non-Latin characters, and the text input in muCOSA automatically takes it in so the user doesn't need to handle it themselves.

3. Holding down keys; the keyboard key callback's behavior in regards to what happens when a user holds down a key is generally undefined (usually just sends repetitive hold/release messages or just hold), but in text input, the exact key is sent at the exact rate designated by the OS.

Text input is sent via an array of bytes in UTF-8 representing the text input. It's not guaranteed to be just one codepoint, but it is guaranteed that the last byte will be a terminating 0 byte.

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
#include <glad.h>

#define MUCOSA_OPENGL
#define MUCOSA_IMPLEMENTATION
#include <muCOSA.h>

muBool load_gl_funcs(void) {
	// Point glad to the function 'mu_opengl_get_function_address'
	// provided by muCOSA, so that glad can find each function for
	// us.
	return gladLoadGL((GLADloadfunc)mu_opengl_get_function_address);
}

int main(void) {
	// ...

	muWindow window = mu_window_create(
		0, MU_OPENGL_3_3_CORE, load_gl_funcs,
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
#define MUCOSA_VULKAN_INCLUDE_PATH <vkbind.h>

#define MUCOSA_VULKAN
#define MUCOSA_IMPLEMENTATION
#include <muCOSA.h>

// ...

VkResult vkres = VK_SUCCESS;
VkInstance instance;
VkSurfaceKHR surface;

// ...

int main(void) {
	// ...

	muWindow window = mu_window_create(
		0, MU_NO_GRAPHICS_API, MU_NULL_PTR,
		// ...
	);

	// ...

	vk_init();

	// ...

	size_m surface_extension_count = 0;
	const char** surface_extensions = mu_vulkan_get_surface_instance_extensions(0, &surface_extension_count);

	// ...

	mu_vulkan_create_window_surface(0, window, &vkres, &instance, 0, &surface);

	// ...
}
```

This allows the user to associate the window with no graphics API, initiate Vulkan themselves, and then query information about and ultimately retrieve a handle to the window's surface, which can then be rendered to by the user as they please.

## Time

The 'time' in muCOSA is a reference to a timer, in seconds, that automatically starts when `muCOSA_init` is called. The time of this timer can be retrieved and set to a different value (to which, if a different value is set, when the time is retrieved, it will return that time that was set plus the additional time that has passed since).

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

[muString](https://github.com/Muukid/muString).

Note that mu libraries store their dependencies within their files, so you don't need to import these dependencies yourself.

Note that the libraries listed may also have other dependencies that they also include that aren't listed here.

## Demo library dependencies

The demos have an `include` folder for other files needed to compile the demos, which is usually for other library dependencies. These library dependencies are:

[glad](https://github.com/Dav1dde/glad); specifically generated with [this generator on these settings](http://glad.sh/#api=gl%3Acore%3D3.3&extensions=&generator=c&options=HEADER_ONLY%2CLOADER).

[vkbind](https://github.com/mackron/vkbind).

The files for these libraries are all self-contained in the `include` folder found in the `demos` folder.

# Thread safety

muCOSA, on default, has no thread safety, but can be made automatically thread safe by defining `MU_THREADSAFE` before the header of muCOSA is included. Once on, this protects global objects such as windows, time, and the clipboard from being accessed by multiple threads at the same time.

# Functions

## Initiation / Termination

The functions `muCOSA_init` and `muCOSA_term` initiate and terminate muCOSA respectively, defined below:

```c
MUDEF void muCOSA_init(muCOSAResult* result, muWindowSystem window_system);

MUDEF void muCOSA_term(muCOSAResult* result);
```

Note that the value `MU_WINDOW_SYSTEM_AUTO` can be used for `window_system` to automatically determine the best window system to be used.

## Current window system

The currently running window system can be queried by the function `muCOSA_get_current_window_system`, defined below:

```c
MUDEF muWindowSystem muCOSA_get_current_window_system(muCOSAResult* result);
```

## Window functions

### Creation / Destruction

The functions `mu_window_create` and `mu_window_destroy` create and destroy a window respectively, defined below:

```c
MUDEF muWindow mu_window_create(muCOSAResult* result, 
	muGraphicsAPI api, muBool (*load_functions)(void),
	muByte* name, uint16_m width, uint16_m height,
	muWindowCreateInfo create_info
);

MUDEF muWindow mu_window_destroy(muCOSAResult* result, muWindow window);
```

### Default create info

The default values for the `muWindowCreateInfo` struct passed into `mu_window_create` can be retrieved with the function `mu_window_default_create_info`, defined below:

```c
MUDEF muWindowCreateInfo mu_window_default_create_info(void);
```

The values this returns are:

```c
.pixel_format.red_bits =     8;
.pixel_format.green_bits =   8;
.pixel_format.blue_bits =    8;
.pixel_format.alpha_bits =   0;
.pixel_format.depth_bits =   0;
.pixel_format.stencil_bits = 0;
.pixel_format.samples =      1;

.visible =   MU_TRUE;
.resizable = MU_TRUE;
.minimized = MU_FALSE;
.maximized = MU_FALSE;

.x =          50;
.y =          50;
.min_width =  120;
.min_height = 1;
.max_width =  30720;
.max_height = 17280;

.cursor_style = MU_CURSOR_STYLE_DEFAULT;

.dimensions_callback = MU_NULL_PTR;
.position_callback =   MU_NULL_PTR;
.focus_callback =      MU_NULL_PTR;
.maximize_callback =   MU_NULL_PTR;
.minimize_callback =   MU_NULL_PTR;

.cursor_position_callback = MU_NULL_PTR;
.keyboard_key_callback =    MU_NULL_PTR;
.keyboard_state_callback =  MU_NULL_PTR;
```

### The main loop functions

There are four functions commonly used within the main loop: `mu_window_get_closed` which returns if the window is closed, `mu_window_close` which closes the window, `mu_window_update` which primarily does event handling for the window and refreshes input, and `mu_window_swap_buffers` which, if a given graphics API is associated with the window, swaps buffers, presenting the rendered image, defined below:

```c
MUDEF muBool mu_window_get_closed(muCOSAResult* result, muWindow window);

MUDEF void mu_window_close(muCOSAResult* result, muWindow window);

MUDEF void mu_window_update(muCOSAResult* result, muWindow window);

MUDEF void mu_window_swap_buffers(muCOSAResult* result, muWindow window);
```

Note that `mu_window_update` will, when called, trigger all relevant window callbacks.

### Get / Set functions

This section covers the window functions that have get/set pairs about various properties of the window.

Note that get-functions that pass their value through pointers can be safely used with the value 0 if you do not wish to retrieve a particular parameter.

#### Focused

The functions `mu_window_get_focused` and `mu_window_focus` get the focused state of a window and focus the window respectively, defined below:

```c
MUDEF muBool mu_window_get_focused(muCOSAResult* result, muWindow window);

MUDEF void mu_window_focus(muCOSAResult* result, muWindow window);
```

Note that due to the nature of how most window systems are built, `mu_window_focus` is not guaranteed to work at all times, but should at least alert the user to focus on the window.

#### Visibility

The functions `mu_window_get_visible` and `mu_window_set_visible` get/set the visibility state of the window, defined below:

```c
MUDEF muBool mu_window_get_visible(muCOSAResult* result, muWindow window);

MUDEF void mu_window_set_visible(muCOSAResult* result, muWindow window, muBool visible);
```

#### Position

The functions `mu_window_get_position` and `mu_window_set_position` get/set the position of the window, defined below:

```c
MUDEF void mu_window_get_position(muCOSAResult* result, muWindow window, int32_m* x, int32_m* y);

MUDEF void mu_window_set_position(muCOSAResult* result, muWindow window, int32_m x, int32_m y);
```

#### Dimensions

The functions `mu_window_get_dimensions` and `mu_window_set_dimensions` get/set the dimensions of the window, defined below:

```c
MUDEF void mu_window_get_dimensions(muCOSAResult* result, muWindow window, uint32_m* width, uint32_m* height);

MUDEF void mu_window_set_dimensions(muCOSAResult* result, muWindow window, uint32_m width, uint32_m height);
```

Note that for `mu_window_set_dimensions`, setting `width` or `height` to a value not covered by the window's min/max values will result in undefined behavior.

#### Maximized

The functions `mu_window_get_maximized` and `mu_window_set_maximized` get/set the maximized state of the window, defined below:

```c
MUDEF muBool mu_window_get_maximized(muCOSAResult* result, muWindow window);

MUDEF void mu_window_set_maximized(muCOSAResult* result, muWindow window, muBool maximized);
```

#### Minimized

The functions `mu_window_get_minimized` and `mu_window_set_minimized` get/set the minimized state of the window, defined below:

```c
MUDEF muBool mu_window_get_minimized(muCOSAResult* result, muWindow window);

MUDEF void mu_window_set_minimized(muCOSAResult* result, muWindow window, muBool minimized);
```

#### Minimum/Maximum dimensions

The functions `mu_window_get_minimum_dimensions`, `mu_window_set_minimum_dimensions`, `mu_window_get_maximum_dimensions`, and `mu_window_set_maximum_dimensions` get/set the minimum/maximum dimensions of the window, defined below:

```c
MUDEF void mu_window_get_minimum_dimensions(muCOSAResult* result, muWindow window, uint32_m* min_width, uint32_m* min_height);

MUDEF void mu_window_set_minimum_dimensions(muCOSAResult* result, muWindow window, uint32_m min_width, uint32_m min_height);

MUDEF void mu_window_get_maximum_dimensions(muCOSAResult* result, muWindow window, uint32_m* max_width, uint32_m* max_height);

MUDEF void mu_window_set_maximum_dimensions(muCOSAResult* result, muWindow window, uint32_m max_width, uint32_m max_height);
```

#### Cursor position

The functions `mu_window_get_cursor_position` and `mu_window_set_cursor_position` get/set the cursor position relative to the window, defined below:

```c
MUDEF void mu_window_get_cursor_position(muCOSAResult* result, muWindow window, int32_m* x, int32_m* y);

MUDEF void mu_window_set_cursor_position(muCOSAResult* result, muWindow window, int32_m x, int32_m y);
```

Note that this function is not guaranteed to work if the window is not focused or the cursor is not within the window's surface.

#### Cursor style

The functions `mu_window_get_cursor_style` and `mu_window_set_cursor_style` get/set the cursor style of the window, defined below:

```c
MUDEF muCursorStyle mu_window_get_cursor_style(muCOSAResult* result, muWindow window);

MUDEF void mu_window_set_cursor_style(muCOSAResult* result, muWindow window, muCursorStyle style);
```

#### Scroll level

The functions `mu_window_get_scroll_level` and `mu_window_set_scroll_level` get/set the scroll level of the window, defined below:

```c
MUDEF int32_m mu_window_get_scroll_level(muCOSAResult* result, muWindow window);

MUDEF void mu_window_set_scroll_level(muCOSAResult* result, muWindow window, int32_m scroll_level);
```

### Get / Let functions

This section covers the window functions that have get/let pairs in regards to the window.

#### Text input focus

The functions `mu_window_get_text_input_focus` and `mu_window_let_text_input_focus` get/let text input focus, and the function `mu_window_update_text_cursor` updates the position of the text cursor relative to the window's surface, defined below:

```c
MUDEF void mu_window_get_text_input_focus(muCOSAResult* result, muWindow window, int32_m text_cursor_x, int32_m text_cursor_y, void (*callback)(muWindow window, muByte* input));

MUDEF void mu_window_let_text_input_focus(muCOSAResult* result, muWindow window);

MUDEF void mu_window_update_text_cursor(muCOSAResult* result, muWindow window, int32_m x, int32_m y);
```

Note that `mu_window_update_text_cursor` is safe to call even when not taking text input.

### Get functions

This section covers the window functions that are used purely to retrieve information.

#### Frame extents

The function `mu_window_get_frame_extents` retrieves information about a window's current frame extents' dimensions, defined below:

```c
MUDEF void mu_window_get_frame_extents(muCOSAResult* result, muWindow window, uint32_m* left, uint32_m* right, uint32_m* top, uint32_m* bottom);
```

#### Keyboard state

The functions `mu_window_get_keyboard_key_state` and `mu_window_get_keyboard_state_state` retrieve the current state of a keyboard key and state, defined below:

```c
MUDEF muButtonState mu_window_get_keyboard_key_state(muCOSAResult* result, muWindow window, muKeyboardKey key);
MUDEF muState mu_window_get_keyboard_state_state(muCOSAResult* result, muWindow window, muKeyboardState state);
```

#### Mouse button state

The function `mu_window_get_mouse_button_state` retrieves the current state of a mouse button, defined below:

```c
MUDEF muButtonState mu_window_get_mouse_button_state(muCOSAResult* result, muWindow window, muMouseButton button);
```

### Set functions

This section covers the window functions that are used to only set information.

#### Title

The function `mu_window_set_title` sets the title of a given window, defined below:

```c
MUDEF void mu_window_set_title(muCOSAResult* result, muWindow window, muByte* title);
```

#### Callback functions

There is a function for each callback that the window usually takes, defined below:

```c
MUDEF void mu_window_set_dimensions_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, uint32_m width, uint32_m height));
MUDEF void mu_window_set_position_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m x, int32_m y));
MUDEF void mu_window_set_focus_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool focused));
MUDEF void mu_window_set_maximize_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool maximized));
MUDEF void mu_window_set_minimize_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muBool minimized));

MUDEF void mu_window_set_keyboard_key_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardKey keyboard_key, muButtonState state));
MUDEF void mu_window_set_keyboard_state_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muKeyboardState keyboard_state, muState state));

MUDEF void mu_window_set_cursor_position_callback(muCOSAResult* result, muWindow win, void (*callback)(muWindow window, int32_m x, int32_m y));
MUDEF void mu_window_set_mouse_button_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, muMouseButton mouse_button, muButtonState state));
MUDEF void mu_window_set_scroll_callback(muCOSAResult* result, muWindow window, void (*callback)(muWindow window, int32_m scroll_level_add));
```

Note that it is safe for the `callback` parameter to be 0, and it will disable the callback.

Note that it is not guaranteed that duplicate callbacks will not occur.

## Time functions

### Get / Set time

The functions `mu_time_get` and `mu_time_set` get/set the time, defined below:

```c
MUDEF double mu_time_get(muCOSAResult* result);

MUDEF void mu_time_set(muCOSAResult* result, double time);
```

### Sleep

The function `mu_sleep` hangs on the call to this function for a given amount of time, in seconds, defined below:

```c
MUDEF void mu_sleep(muCOSAResult* result, double time);
```

## Clipboard functions

### Get / Set clipboard

The functions `mu_clipboard_get` and `mu_clipboard_set` get/set the current global clipboard, defined below:

```c
MUDEF muByte* mu_clipboard_get(muCOSAResult* result);

MUDEF void mu_clipboard_set(muCOSAResult* result, muByte* text, size_m text_size);
```

Note that if there is no clipboard, `mu_clipboard_get` still sets result to `MUCOSA_SUCCESS` but returns 0.

## OS functions

This section covers functions regarding OS-specific behavior.

### Get window handle

The function `mu_os_get_window_handle` returns a pointer to a given handle of the window, defined below:

```c
MUDEF void* mu_os_get_window_handle(muCOSAResult* result, muWindow window, muWindowHandle handle);
```

The pointer returned is a pointer to the handle itself; ie, if `handle` is `MU_WINDOWS_HWND`, then the `HWND` can be retrieved via casting it to a pointer of the desired type and then dereferencing it like so:

```c
void* v_ptr = mu_os_get_window_handle(...);
HWND hwnd = *((HWND*)v_ptr);
```

## OpenGL functions

### Bind window

The function `mu_opengl_bind_window` sets the current OpenGL context to the OpenGL context of a given window, defined below:

```c
MUDEF void mu_opengl_bind_window(muCOSAResult* result, muWindow window);
```

### Get function address

The function `mu_opengl_get_function_address` returns the address of a requested OpenGL function, defined below:

```c
MUDEF void* mu_opengl_get_function_address(const muByte* name);
```

If the function could not be found, the return value is 0.

Note that `name` is not necessarily UTF-8 like most functions, but instead the encoding of the respective OS-dependent OpenGL function it calls to locate the address.

### Swap interval

The function `mu_opengl_window_swap_interval` sets the OpenGL swap interval, defined below:

```c
MUDEF void mu_opengl_window_swap_interval(muCOSAResult* result, muWindow window, int interval);
```

## Vulkan functions

### Surface extensions

The function `mu_vulkan_get_surface_instance_extensions` returns the required surface extensions necessary to create a surface for the current window system, defined below:

```c
MUDEF const char** mu_vulkan_get_surface_instance_extensions(muCOSAResult* result, size_m* count);
```

Note that this function is not necessarily UTF-8.

### Window surface

The function `mu_vulkan_create_window_surface` creates a Vulkan surface for a given window, defined below:

```c
MUDEF void mu_vulkan_create_window_surface(muCOSAResult* result, muWindow window, void* vk_result, void* instance, void* allocator, void* surface);
```

`vk_result` is interpreted as type `VkResult*`.

`instance` is interpreted as type `VkInstance*`.

`allocator` is interpreted as type `const VkAllocationCallbacks*`.

`surface` is interpreted as type `VkSurfaceKHR*`.

Note that the parameters are `void*` to allow this function to be at least declared if Vulkan types have not yet been defined.

## Name functions

There are functions used to get a `const char*` representation of an enumerator, all defined below:

```c
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
```

Note that these functions are only defined if `MUCOSA_NAMES` is defined before the header of muCOSA is defined.

Note that the 'nice' functions return names for the enumerators that are nicer to read. For example:

```c
mu_graphics_api_get_nice_name(MU_OPENGL_3_3_COMPATIBILITY)
```

returns

```c
"OpenGL 3.3 Compatibility"
```

, while:

```c
mu_graphics_api_get_name(MU_OPENGL_3_3_COMPATIBILITY)
```

returns

```c
"MU_OPENGL_3_3_COMPATIBILITY"
```

# Macros

## muWindow

The object `muWindow` is an ID reference represented by the type `size_m`. It represents a window.

## `MUCOSA_NAMES`

If `MUCOSA_NAMES` is defined before the header of muCOSA is defined, the following functions are defined:

`muCOSA_result_get_name`, `mu_window_system_get_name`, `mu_window_system_get_nice_name`, `mu_window_handle_get_name`, `mu_window_handle_get_nice_name`, `mu_graphics_api_get_name`, `mu_graphics_api_get_nice_name`, `mu_cursor_style_get_name`, `mu_cursor_style_get_nice_name`, `mu_keyboard_key_get_name`, `mu_keyboard_key_get_nice_name`, `mu_keyboard_state_get_name`, `mu_keyboard_state_get_nice_name`, `mu_mouse_button_get_name`, and `mu_mouse_button_get_nice_name`.

## Button state

The macro `muButtonState` is a macro for `muBool`, with it and its respective values defined below:

```c
#define muButtonState muBool
#define MU_BUTTON_STATE_RELEASED 0
#define MU_BUTTON_STATE_HELD 1
```

## State

The macro `muState` is a macro for `muBool`, with it and its respective values defined below:

```c
#define muState muBool
#define MU_ON 1
#define MU_OFF 0
```

## Locale modification

On X11, in order for text input to fully work, it is recommended to set the locale to "". This is automatically performed unless `MUCOSA_NO_LOCALE_MOD` is defined before the implementation of muCOSA is defined.

## Version macro

muCOSA defines three macros to define the version of muCOSA: `MUCOSA_VERSION_MAJOR`, `MUCOSA_VERSION_MINOR`, and `MUCOSA_VERSION_PATCH`, following the format of:

```
vMAJOR.MINOR.PATCH
```

# Enumerators

## Result enumerator

muCOSA uses the `muCOSAResult` enumerator to represent how a function went. It has these possible values:

`MUCOSA_SUCCESS`: the task succeeded.

`MUCOSA_ALREADY_INITIALIZED`: the task attempted to initialize muCOSA when it was already initialized.

`MUCOSA_ALREADY_TERMINATED`: the task attempted to terminate muCOSA when it was already terminated or hadn't yet been initialized.

`MUCOSA_ALREADY_ACTIVE`: the task attempted to set the state of something to active when it was already active.

`MUCOSA_ALREADY_INACTIVE`: the task attempted to set the state of something to inactive when it was already inactive.

`MUCOSA_NOT_YET_INITIALIZED` : the task requires muCOSA to be initialized, but it has not yet been initialized.

`MUCOSA_ALLOCATION_FAILED`: the task requires allocation of some kind, but there was insufficient memory to do so.

`MUCOSA_UNKNOWN_WINDOW_SYSTEM`: a given window system has an unknown enumerator value.

`MUCOSA_UNKNOWN_GRAPHICS_API`: a given graphics API has an unknown enumerator value.

`MUCOSA_UNKNOWN_KEYBOARD_KEY`: a given keyboard key has an unknown enumerator value.

`MUCOSA_UNKNOWN_KEYBOARD_STATE`: a given keyboard state has an unknown enumerator value.

`MUCOSA_UNKNOWN_MOUSE_BUTTON`: a given mouse button has an unknown enumerator value.

`MUCOSA_UNKNOWN_WINDOW_HANDLE`: a given window handle has an unknown enumerator value.

`MUCOSA_UNSUPPORTED_WINDOW_SYSTEM`: a given window system is not supported on this device.

`MUCOSA_UNSUPPORTED_FEATURE`: rather the task being requested has not yet been implemented or the task requires the device to have some functionality that isn't supported.

`MUCOSA_UNSUPPORTED_OPENGL_FEATURE`: a required OpenGL feature is not supported on the device at the moment.

`MUCOSA_UNSUPPORTED_GRAPHICS_API`: a graphics API-specific function was called when that given graphics API hasn't been defined yet (see **Basic library concepts** -> **Graphics APIs**).

`MUCOSA_FAILED_CONNECTION_TO_SERVER`: an X11 display could not be opened. This is X11 specific.

`MUCOSA_FAILED_CREATE_WINDOW`: the function used to create a window failed.

`MUCOSA_FAILED_LOAD_FUNCTIONS`: a given function used to load functions returned zero.

`MUCOSA_FAILED_FIND_COMPATIBLE_FRAMEBUFFER`: a compatible framebuffer for the specified pixel format could not be found. This is not fatal.

`MUCOSA_FAILED_CREATE_OPENGL_CONTEXT`: the context of a requested OpenGL version could not be created.

`MUCOSA_FAILED_LOAD_OPENGL_CONTEXT`: the context of a requested OpenGL version could not be loaded.

`MUCOSA_FAILED_USE_PIXEL_FORMAT`: the requested pixel format could not be used. This is not fatal.

`MUCOSA_FAILED_JOIN_THREAD`: a thread necessary for the task could not be joined.

`MUCOSA_FAILED_CREATE_THREAD`: a thread necessary for the task could not be created.

`MUCOSA_FAILED_CREATE_INPUT_METHOD`: an input method could not be created. This is X11 specific.

`MUCOSA_FAILED_GET_INPUT_STYLES`: an input style could not be found for the input method. This is X11 specific.

`MUCOSA_FAILED_FIND_COMPATIBLE_INPUT_STYLE`: an input style compatible with the way muCOSA handles input could not be found. This is X11 specific.

`MUCOSA_FAILED_CREATE_INPUT_CONTEXT`: an input context could not be created. This is X11 specific.

`MUCOSA_FAILED_REGISTER_WINDOW_CLASS`: the window class for a window could not be registered. This is Win32 specific.

`MUCOSA_FAILED_CONVERT_UTF8_TO_WCHAR`: the conversion from a UTF-8 string for use in Win32 with `wchar_t*` could not be performed successfully. This is Win32 specific.

`MUCOSA_FAILED_CONVERT_WCHAR_TO_UTF8`: the conversion from a Win32 `wchar_t*` string to a UTF-8 string could not be performed successfully. This is Win32 specific.

`MUCOSA_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS`: the dummy WGL window class necessary to properly load OpenGL could not be registered. This is Win32 specific.

`MUCOSA_FAILED_CREATE_DUMMY_WGL_WINDOW`: the dummy WGL window necessary to properly load OpenGL could not be created. This is Win32 specific.

`MUCOSA_FAILED_FIND_COMPATIBLE_PIXEL_FORMAT`: a pixel format compatible with the requested pixel format could not be found. This is Win32 specific.

`MUCOSA_FAILED_DESCRIBE_PIXEL_FORMAT`: the chosen pixel format could not be properly described. This is Win32 specific.

`MUCOSA_FAILED_SET_PIXEL_FORMAT`: the chosen pixel format could not be properly set. This is Win32 specific.

`MUCOSA_FAILED_QUERY_WINDOW_INFO`: the requested window information could not be retrieved. This is Win32 specific.

`MUCOSA_FAILED_SET_WINDOW_INFO`: the requested window property could not be modified. This is Win32 specific.

`MUCOSA_FAILED_GET_IMM_CONTEXT`: the input method manager context could not be retrieved. This is Win32 specific.

`MUCOSA_FAILED_LET_IMM_CONTEXT`: the input method manager context could not be let go. This is Win32 specific.

`MUCOSA_FAILED_GLOBAL_ALLOCATION`: the global memory necessary for the task could not be allocated. This is Win32 specific.

`MUCOSA_FAILED_GLOBAL_LOCK`: the global memory necessary for the task could not be locked. This is Win32 specific.

`MUCOSA_FAILED_HOLD_CLIPBOARD`: the clipboard could not be held. This is Win32 specific.

`MUCOSA_FAILED_SET_CLIPBOARD`: the clipboard could not be set. This is Win32 specific.

`MUCOSA_INVALID_MINIMUM_MAXIMUM_BOOLS`: the minimized and maximized boolean values describing a window are invalid/paradoxical (for example, if minimum and maximum are both set to true, this error is triggered, as a window cannot be both minimized and maximized).

`MUCOSA_INVALID_MINIMUM_MAXIMUM_DIMENSIONS`: the given minimum/maximum dimensions describing a window are invalid (for example, if the maximum dimension are under the minimum dimensions).

`MUCOSA_INVALID_ID`: a given object ID is invalid/non-existent.

`MUCOSA_INVALID_SAMPLE_COUNT`: a given sample count is invalid. It must be a power of 2 and be at most 16.

`MUCOSA_INVALID_DIMENSIONS`: the given dimensions are invalid (for example, if the given dimensions are not in range of the minimum/maximum dimensions).

`MUCOSA_INVALID_POINTER`: a pointer that is required for a task to perform is 0.

`MUCOSA_INVALID_WINDOW_STATE`: a requested task, if performed, would lead to an invalid window state (for example, if an invisible window is attempted to be focused).

`MUCOSA_INVALID_TIME`: the requested time is invalid (for example, if a negative value is passed to the sleep function).

`MUCOSA_NONEXISTENT_DEVICE`: properties about a non-existent device are attempting to be retrieved or set (for example, calling cursor-related functions when a cursor is not available).

`MUCOSA_OVERSIZED_CLIPBOARD`: the clipboard is too large to be set.

`MUCOSA_WINDOW_NON_RESIZABLE`: a non-resizeable window was attempted to be resized.

`MUCOSA_MUMA_SUCCESS`: a muma task succeeded.

`MUCOSA_MUMA_FAILED_TO_ALLOCATE`: a muma task failed to allocate.

`MUCOSA_MUMA_INVALID_INDEX`: the index of an element given to a muma function was invalid.

`MUCOSA_MUMA_INVALID_SHIFT_AMOUNT`: the shifting of an array in a muma task could not be performed.

`MUCOSA_MUMA_NOT_FOUND`: an element in an array could not be located by a muma function.

Unless specified otherwise above, if the result of a function does not equal `MUCOSA_SUCCESS`, whatever the function was supposed to do failed, and it will be as if the function was never called (meaning the function call had no permanent effect).

## Window system

The enum `muWindowSystem` defines a window system. It has these possible values:

`MU_WINDOW_SYSTEM_UNKNOWN`: an unknown window system.

`MU_WINDOW_SYSTEM_AUTO`: choose the window system automatically.

`MU_WINDOW_SYSTEM_X11`: the [X11 window system](https://en.wikipedia.org/wiki/X_Window_System).

`MU_WINDOW_SYSTEM_WIN32`: [Win32](https://en.wikipedia.org/wiki/Windows_API#Major_versions)'s window system.

## Window handle

The enum `muWindowHandle` defines a window handle. It has these possible values:

`MU_WINDOWS_HWND`: Win32's [HWND](https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types#HWND).

`MU_X11_DISPLAY`: X11's [Display](https://tronche.com/gui/x/xlib/display/opening.html#Display).

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

Note that not all of these styles are guaranteed to be available (in particular, all of the directional cursor styles), in which case a similar cursor style will be chosen.

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

Note that this list is by no means exhaustive, and is even missing some basic keys like the semicolon key (in which case, the only realistic way of getting input would be to use text input); I plan to add more keys later.

## Keyboard state

The enum `muKeyboardState` defines a keyboard state. It has these possible values:

`MU_KEYBOARD_STATE_UNKNOWN`: unknown keyboard state.

`MU_KEYBOARD_STATE_CAPS_LOCK`: [caps lock state](https://en.wikipedia.org/wiki/Caps_Lock).

`MU_KEYBOARD_STATE_SCROLL_LOCK`: [scroll lock state](https://en.wikipedia.org/wiki/Scroll_Lock).

`MU_KEYBOARD_STATE_NUM_LOCK`: [num lock state](https://en.wikipedia.org/wiki/Num_Lock).

## Mouse button

The enum `muMouseButton` defines a [mouse button](https://en.wikipedia.org/wiki/Mouse_button). It has these possible values:

`MU_MOUSE_BUTTON_UNKNOWN`: unknown mouse button.

`MU_MOUSE_BUTTON_LEFT`: left mouse button.

`MU_MOUSE_BUTTON_RIGHT`: right mouse button.

`MU_MOUSE_BUTTON_MIDDLE`: middle mouse button.

# Structs

## Pixel format

The struct `muPixelFormat` is used to represent a pixel format, defined below:

```c
struct muPixelFormat {
	uint16_m red_bits;
	uint16_m green_bits;
	uint16_m blue_bits;
	uint16_m alpha_bits;

	uint16_m depth_bits;
	uint16_m stencil_bits;

	uint8_m samples;
};
```

Note that samples must be a power of 2 and cannot be any larger than 16.

## Window create info

The struct `muWindowCreateInfo` is used to represent a window's state upon creation, defined below:

```c
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

	muCursorStyle cursor_style;

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
```

Note that the default values for these can be retrieved via the function `mu_window_default_create_info`.

# C standard library dependencies

muCOSA has several C standard library dependencies not provided by its other library dependencies, all of which are overridable by defining them before the inclusion of its header. This is a list of all of those dependencies:

## `string.h` dependencies

`mu_strlen`: equivalent to `strlen`

`mu_strcmp`: equivalent to `strcmp`

## `locale.h` dependencies

`mu_setlocale`: equivalent to `setlocale`

## `stdio.h` dependencies

`mu_fflush`: equivalent to `fflush`

`mu_stdout`: equivalent to `stdout`

# Global variables

## Context

The variable `muCOSA_global_context` is used to refer to the context of muCOSA globally, defined below:

```c
MUDEF muCOSAContext* muCOSA_global_context;
```

This context is 0 if muCOSA is not currently initiated. This variable can be checked by the user, but should not be written to.

# Incomplete types

## Context type

muCOSA has an incomplete type defined in its header called `muCOSAContext` that represents the context of muCOSA. This context is not meant to be used by the user, and is later defined in the implementation.
