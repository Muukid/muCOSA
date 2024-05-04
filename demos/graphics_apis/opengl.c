/*
============================================================
                        DEMO INFO

DEMO NAME:          opengl.c
DEMO WRITTEN BY:    Muukid
CREATION DATE:      2024-04-21
LAST UPDATED:       2024-05-04

============================================================
                        DEMO PURPOSE

This demo shows how loading OpenGL in muCOSA works.

============================================================
                        LICENSE INFO

This software is licensed under:
(MIT license OR public domain) AND Apache 2.0.
More explicit license information at the end of file.

============================================================
                       COMPILER NOTES

glad does violate extension usage, so it needs to be turned
off when compiling this demo when relevant.

============================================================
*/

// Include muCOSA

#define MUCOSA_NAMES // For name functions
#define MUCOSA_OPENGL // For 'mu_opengl_get_function_address'
#define MUCOSA_NO_INCLUDE_OPENGL // Don't include OpenGL, we'll include it later with glad
#define MUCOSA_IMPLEMENTATION
#include <muCOSA.h>

// Include glad for OpenGL loading

#define GLAD_GL_IMPLEMENTATION
#include <glad.h>

// C standard library

#include <stdio.h> // For printf

/* Variables */

	// Used to store the result of functions
	muCOSAResult result = MUCOSA_SUCCESS;
	// Macro which is used to print if the result is bad, meaning a function went wrong.
	#define scall(function_name) if (result != MUCOSA_SUCCESS) {printf("WARNING: '" #function_name "' returned %s\n", muCOSA_result_get_name(result));}

	// The window system (like Win32, X11, etc.)
	muWindowSystem window_system = MU_WINDOW_SYSTEM_AUTO;

	// The graphics API
	muGraphicsAPI graphics_api = MU_OPENGL_3_3_CORE;

/* Graphics API loading function */

	// Function that will be passed to the window creation function that will be called when it's
	// time for OpenGL to be loaded.
	muBool load_gl_funcs(void) {
		return gladLoadGL((GLADloadfunc)mu_opengl_get_function_address);
	}

/* Dimensions callback */

	void dimensions_callback(muWindow window, uint32_m width, uint32_m height) {
		if (window) {} // (to avoid unused parameter warnings)
		glViewport(0, 0, width, height);
	}

/* Matrix logic */

	// Note: largely based off cglm

	void matrix_set(float mat[4][4], float v) {
		mat[0][0] = v; mat[0][1] = 0.f; mat[0][2] = 0.f; mat[0][3] = 0.f;
		mat[1][0] = 0.f; mat[1][1] = v; mat[1][2] = 0.f; mat[1][3] = 0.f;
		mat[2][0] = 0.f; mat[2][1] = 0.f; mat[2][2] = v; mat[2][3] = 0.f;
		mat[3][0] = 0.f; mat[3][1] = 0.f; mat[3][2] = 0.f; mat[3][3] = v;
	}

	void vec3_scale(float vector[3], float scale, float dest[3]) {
		dest[0] = vector[0] * scale;
		dest[1] = vector[1] * scale;
		dest[2] = vector[2] * scale;
	}

	void matrix_rotate(float mat[4][4], float angle, float axis[3]) {
		float rotation[4][4];
		mu_memset(rotation, 0.f, sizeof(rotation));
		float c = cosf(angle);

		float normalized_axis[3];
		vec3_scale(axis, 1.f / sqrtf(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]), normalized_axis);

		float vec[3], vecs[3];
		vec3_scale(normalized_axis, 1.f-c, vec);
		vec3_scale(normalized_axis, sinf(angle), vecs);

		vec3_scale(normalized_axis, vec[0], rotation[0]);
		vec3_scale(normalized_axis, vec[1], rotation[1]);
		vec3_scale(normalized_axis, vec[2], rotation[2]);

		rotation[0][0] += c; rotation[1][0] -= vecs[2]; rotation[2][0] += vecs[1];
		rotation[0][1] += vecs[2]; rotation[1][1] += c; rotation[2][1] -= vecs[0];
		rotation[0][2] -= vecs[1]; rotation[1][2] += vecs[0]; rotation[2][2] += c;
		rotation[0][3] = rotation[1][3] = rotation[2][3] = rotation[3][0] = rotation[3][1] = rotation[3][2] = 0.f;
		rotation[3][3] = 1.f;

		float a00 = mat[0][0], a01 = mat[0][1], a02 = mat[0][2], a03 = mat[0][3],
		a10 = mat[1][0], a11 = mat[1][1], a12 = mat[1][2], a13 = mat[1][3],
		a20 = mat[2][0], a21 = mat[2][1], a22 = mat[2][2], a23 = mat[2][3],
		a30 = mat[3][0], a31 = mat[3][1], a32 = mat[3][2], a33 = mat[3][3],
		b00 = rotation[0][0], b01 = rotation[0][1], b02 = rotation[0][2],
		b10 = rotation[1][0], b11 = rotation[1][1], b12 = rotation[1][2],
		b20 = rotation[2][0], b21 = rotation[2][1], b22 = rotation[2][2];
		mat[0][0] = a00 * b00 + a10 * b01 + a20 * b02;
		mat[0][1] = a01 * b00 + a11 * b01 + a21 * b02;
		mat[0][2] = a02 * b00 + a12 * b01 + a22 * b02;
		mat[0][3] = a03 * b00 + a13 * b01 + a23 * b02;
		mat[1][0] = a00 * b10 + a10 * b11 + a20 * b12;
		mat[1][1] = a01 * b10 + a11 * b11 + a21 * b12;
		mat[1][2] = a02 * b10 + a12 * b11 + a22 * b12;
		mat[1][3] = a03 * b10 + a13 * b11 + a23 * b12;
		mat[2][0] = a00 * b20 + a10 * b21 + a20 * b22;
		mat[2][1] = a01 * b20 + a11 * b21 + a21 * b22;
		mat[2][2] = a02 * b20 + a12 * b21 + a22 * b22;
		mat[2][3] = a03 * b20 + a13 * b21 + a23 * b22;
		mat[3][0] = a30; mat[3][1] = a31; mat[3][2] = a32; mat[3][3] = a33;
	}

	void vec4_muladds(float inp[4], float s, float dest[4]) {
		dest[0] += inp[0] * s;
		dest[1] += inp[1] * s;
		dest[2] += inp[2] * s;
		dest[3] += inp[3] * s;
	}

	void matrix_translate(float mat[4][4], float vec[3]) {
		vec4_muladds(mat[0], vec[0], mat[3]);
		vec4_muladds(mat[1], vec[1], mat[3]);
		vec4_muladds(mat[2], vec[2], mat[3]);
	}

	void matrix_perspective(float fovy, float aspect, float near_z, float far_z, float mat[4][4]) {
		matrix_set(mat, 0.f);
		float f = 1.f / tanf(fovy * .5f), fn = 1.f / (near_z - far_z);
		mat[0][0] = f / aspect;
		mat[1][1] = f;
		mat[2][2] = (near_z + far_z) * fn;
		mat[2][3] = -1.f;
		mat[3][2] = 2.f * near_z * far_z * fn;
	}

int main(void) {
/* Initiation */

	// Initiate muCOSA

	muCOSA_init(&result, window_system); scall(muCOSA_init)

	// Print currently running window system

	printf("Running window system \"%s\"\n", mu_window_system_get_nice_name(muCOSA_get_current_window_system(0)));

	// Create window

	muWindowCreateInfo create_info = mu_window_default_create_info();
	create_info.pixel_format.depth_bits = 24; // For depth in OpenGL
	create_info.dimensions_callback = dimensions_callback; // For glViewport

	muWindow window = mu_window_create(
		&result,
		graphics_api, load_gl_funcs, // <--- Note our loading function is being passed in here
		(muByte*)"OpenGL 3.3 Core", 800, 600,
		create_info
	);
	scall(mu_window_create)

	// Print current graphics API

	printf("Running graphics API \"%s\"\n", mu_graphics_api_get_nice_name(graphics_api));

/* OpenGL code */

	// Note: largely based off LearnOpenGL

	// Bind up and configure OpenGL

	mu_opengl_bind_window(&result, window); scall(mu_opengl_bind_window)

	glEnable(GL_DEPTH_TEST);

	// Setup shader code

	const char* vert_code = 
	"#version 330 core\n"
	"layout (location=0) in vec3 vPos;\n"
	"layout (location=1) in vec3 vCol;\n"

	"out vec3 fCol;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"

	"void main() {\n"
	"	gl_Position = projection * view * model * vec4(vPos, 1.0);\n"
	"	fCol = vCol;\n"
	"}\n"
	;

	const char* frag_code = 
	"#version 330 core\n"
	"out vec4 oCol;\n"
	"in vec3 fCol;\n"

	"void main() {\n"
	"	oCol = vec4(fCol, 1.0);\n"
	"}\n"
	;

	int success;
	char info_log[512];

	unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vert_code, 0);
	glCompileShader(vert_shader);
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vert_shader, 512, 0, info_log);
		printf("WARNING: vertex shader failed to compile, error message: \n%s\n", info_log);
	}

	unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &frag_code, 0);
	glCompileShader(frag_shader);
	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(frag_shader, 512, 0, info_log);
		printf("WARNING: fragment shader failed to compile, error message: \n%s\n", info_log);
	}

	unsigned int shader = glCreateProgram();
	glAttachShader(shader, vert_shader);
	glAttachShader(shader, frag_shader);
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader, 512, 0, info_log);
		printf("WARNING: shader linking failed to compile, error message: \n%s\n", info_log);
	}

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	// Setup vertex/index data

	#define COL0 1.f, 0.f, 0.f
	#define COL1 0.f, 1.f, 0.f
	#define COL2 0.f, 0.f, 1.f
	#define COL3 0.f, 1.f, 1.f
	#define COL4 1.f, 0.f, 1.f
	#define COL5 1.f, 1.f, 0.f

	float vert[] = {
		// Front face
			// t0
			.5f, .5f, .5f, COL0,
			.5f, -.5f, .5f, COL0,
			-.5f, .5f, .5f, COL0,
			// t1
			.5f, -.5f, .5f, COL0,
			-.5f, -.5f, .5f, COL0,
			-.5f, .5f, .5f, COL0,
		// Back face
			// t0
			.5f, .5f, -.5f, COL1,
			.5f, -.5f, -.5f, COL1,
			-.5f, .5f, -.5f, COL1,
			// t1
			.5f, -.5f, -.5f, COL1,
			-.5f, -.5f, -.5f, COL1,
			-.5f, .5f, -.5f, COL1,
		// Right face
			// t0
			.5f, .5f, .5f, COL2,
			.5f, -.5f, .5f, COL2,
			.5f, -.5f, -.5f, COL2,
			// t1
			.5f, .5f, .5f, COL2,
			.5f, .5f, -.5f, COL2,
			.5f, -.5f, -.5f, COL2,
		// Left face
			// t0
			-.5f, .5f, .5f, COL3,
			-.5f, -.5f, .5f, COL3,
			-.5f, -.5f, -.5f, COL3,
			// t1
			-.5f, .5f, .5f, COL3,
			-.5f, .5f, -.5f, COL3,
			-.5f, -.5f, -.5f, COL3,
		// Top face
			// t0
			-.5f, -.5f, .5f, COL4,
			.5f, -.5f, .5f, COL4,
			.5f, -.5f, -.5f, COL4,
			// t1
			.5f, -.5f, -.5f, COL4,
			-.5f, -.5f, -.5f, COL4,
			-.5f, -.5f, .5f, COL4,
		// Bottom face
			// t0
			-.5f, .5f, .5f, COL5,
			.5f, .5f, .5f, COL5,
			.5f, .5f, -.5f, COL5,
			// t1
			.5f, .5f, -.5f, COL5,
			-.5f, .5f, -.5f, COL5,
			-.5f, .5f, .5f, COL5,
	};

	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

/* Main loop */

	// Set up a loop that continues as long as the window isn't closed

	while (!mu_window_get_closed(&result, window)) {
		scall(mu_window_get_closed)

		// Fallback for if window has closed now
		if (mu_window_get_closed(&result, window)) {
			break;
		}

		// Call the close window function if the escape key is pressed

		if (mu_window_get_keyboard_key_state(&result, window, MU_KEYBOARD_KEY_ESCAPE)) {
			mu_window_close(&result, window); scall(mu_window_close)
		}

		// Toggle wireframe if 'W' is down

		if (mu_window_get_keyboard_key_state(&result, window, MU_KEYBOARD_KEY_W)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// Calculate matrices

		float model[4][4]; matrix_set(model, 1.f);
		float view[4][4]; matrix_set(view, 1.f);
		float projection[4][4]; matrix_set(projection, 1.f);
		float temp_vec3[3];

		temp_vec3[0] = 1.f; temp_vec3[1] = .5f; temp_vec3[2] = 0.f;
		matrix_rotate(model, mu_time_get(0), temp_vec3);

		temp_vec3[0] = 0.f; temp_vec3[1] = 0.f; temp_vec3[2] = -3.f;
		matrix_translate(view, temp_vec3);

		uint32_m width=800, height=600;
		mu_window_get_dimensions(&result, window, &width, &height);
		scall(mu_window_get_dimensions)

		matrix_perspective(45.f * (3.14159265359f / 180.f), (float)width / (float)height, .1f, 100.f, projection);

		glUseProgram(shader);
		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection[0][0]);

		// Render scene

		glClearColor(191.f / 255.f, 174.f / 255.f, 212.f / 255.f, 1.f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vert) / (sizeof(float) * 6));

		// Swap buffers (which renders the screen)

		mu_window_swap_buffers(&result, window); scall(mu_window_swap_buffers)

		// Update window (which refreshes input and such)

		mu_window_update(&result, window); scall(mu_window_update)
	}

/* Termination */

	// Destroy window (optional)

	window = mu_window_destroy(&result, window); scall(mu_window_destroy)

	// Terminate muCOSA
	
	muCOSA_term(&result); scall(muCOSA_term)

	// Program should make a window that displays a 3D cube spinning in OpenGL, and you can hold
	// down the 'W' key to turn on or off wireframe mode.
	return 0;
}

/*
For all source code:

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

For Khronos specifications:
	
	Copyright (c) 2013-2020 The Khronos Group Inc.

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.

*/

