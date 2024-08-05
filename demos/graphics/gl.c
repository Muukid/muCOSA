/*
============================================================
                        DEMO INFO

DEMO NAME:          gl.c
DEMO WRITTEN BY:    Muukid
CREATION DATE:      2024-08-05
LAST UPDATED:       2024-08-05

============================================================
                        DEMO PURPOSE

This demo shows how loading and using OpenGL in muCOSA
works.

Program should display a spinning cube with multiple colors
on each side.

============================================================
                        LICENSE INFO

This software is licensed under:
(MIT license OR public domain) AND Apache 2.0.
More explicit license information at the end of file.

============================================================
*/

/* Inclusion */
	
	// Inlcude glad for OpenGL loading
	#define GLAD_GL_IMPLEMENTATION
	#include "glad.h"

	// Include muCOSA
	#define MUCOSA_NAMES // For name functions
	#define MU_SUPPORT_OPENGL
	#define MUCOSA_IMPLEMENTATION // For source code
	#include "muCOSA.h"
	
	// Include stdio for print functions
	#include <stdio.h>
	
	// Include math for math functions
	#include <math.h>
	
	// Include string for mem functions
	#include <string.h>

/* Variables */
	
	// Global context
	muCOSAContext muCOSA;

	// The window system
	muWindowSystem window_system = MU_WINDOW_NULL; // (Auto)

	// Window handle
	muWindow win;

	// OpenGL context
	muGLContext gl;

	// Pixel format
	muPixelFormat format = {
		// RGBA bits
		8, 8, 8, 8,
		// Depth bits
		24,
		// Stencil bits
		0,
		// Samples
		1,
	};

	// Window information
	muWindowInfo wininfo = {
		// Title
		(char*)"OpenGL 3.3 Core",
		// Resolution (width & height)
		800, 600,
		// Coordinates (x and y)
		50, 50,
		// Pixel format
		&format
	};

	// Window keyboard map
	muBool* keyboard;

/* OpenGL functions */

	// Function to get an OpenGL address
	void* load_gl_func(const char* name) {
		return mu_gl_get_proc_address(name);
	}

/* OpenGL code */

	// Vertex shader
	const char* vert_code = 
		"#version 330 core\n"

		// Input: vec3 pos, vec3 col
		"layout (location=0) in vec3 vPos;"
		"layout (location=1) in vec3 vCol;"

		// Output: vec3 col
		"out vec3 fCol;"

		// Matrixes
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"

		"void main() {"
			// Position based on matrixes:
			"gl_Position = projection * view * model * vec4(vPos, 1.0);"
			// Send color to fragment:
			"fCol = vCol;"
		"}"
	;

	// Fragment shader
	const char* frag_code =
		"#version 330 core\n"

		// Input: vec3 col
		"in vec3 fCol;"

		// Output color:
		"out vec4 oCol;"

		"void main() {"
			// Output color we were given
			"oCol = vec4(fCol, 1.0);"
		"}"
	;

	// Shader program variable
	GLuint shader;

	// Compiles shaders
	void compile_shaders(void) {
		// To store success and error logs
		int success;
		char err_log[512];

		// Compile vertex shader
		GLuint vert = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert, 1, &vert_code, 0);
		glCompileShader(vert);
		// - Print possible error
		glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vert, 512, 0, err_log);
			printf("WARNING: Vertex shader failed to compile; error message: \n%s\n", err_log);
		}

		// Compile fragment shader
		GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag, 1, &frag_code, 0);
		glCompileShader(frag);
		// - Print possible error
		glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(frag, 512, 0, err_log);
			printf("WARNING: Fragment shader failed to compile; error message: \n%s\n", err_log);
		}

		// Merge shaders in program
		shader = glCreateProgram();
		glAttachShader(shader, vert);
		glAttachShader(shader, frag);
		glLinkProgram(shader);
		// - Print possible error
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 512, 0, err_log);
			printf("WARNING: Shader program failed to merge; error message: \n%s\n", err_log);
		}

		// Delete shaders (they're now in the program)
		if (vert) {
			glDeleteShader(vert);
		}
		if (frag) {
			glDeleteShader(frag);
		}

		// Use shader
		glUseProgram(shader);
	}

	// Vertex data macros
	#define COL0 1.f, 0.f, 0.f
	#define COL1 0.f, 1.f, 0.f
	#define COL2 0.f, 0.f, 1.f
	#define COL3 0.f, 1.f, 1.f
	#define COL4 1.f, 0.f, 1.f
	#define COL5 1.f, 1.f, 0.f

	// Vertex data
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
			-.5f, .5f, .5f, COL5
	};

	// Create vertex objects
	void load_vertexes(void) {
		// Vertex array object (vao) and vertex buffer object (vbo)
		GLuint vao, vbo;
		// - Generate
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		// - Bind
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Buffer data
		glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
		// Describe data
		// - vec3 pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
		glEnableVertexAttribArray(0);
		// - vec3 col
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(1);

		// Unbind vertex buffer object
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// The VAO stays binded here, and we don't need to reference it after this
	}

/* Matrix logic */

	// Note: largely "based off" (AKA stolen) off cglm

	void matrix_set(float mat[4][4], float v) {
		mat[0][0] = v; mat[0][1] = mat[0][2] = mat[0][3] = 0.f;
		mat[1][1] = v; mat[1][0] = mat[1][2] = mat[1][3] = 0.f;
		mat[2][2] = v; mat[2][0] = mat[2][1] = mat[2][3] = 0.f;
		mat[3][3] = v; mat[3][0] = mat[3][1] = mat[3][2] = 0.f;
	}

	void vec3_scale(float vector[3], float scale, float dest[3]) {
		dest[0] = vector[0] * scale;
		dest[1] = vector[1] * scale;
		dest[2] = vector[2] * scale;
	}

	void matrix_rotate(float mat[4][4], float angle, float axis[3]) {
		float rot[4][4];
		memset(rot, 0.f, sizeof(rot));
		float c = cosf(angle);

		float norm_axis[3];
		vec3_scale(
			axis,
			1.f / sqrtf(axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2]),
			norm_axis
		);

		float vec[3], vecs[3];
		vec3_scale(norm_axis, 1.f-c, vec);
		vec3_scale(norm_axis, sinf(angle), vecs);

		vec3_scale(norm_axis, vec[0], rot[0]);
		vec3_scale(norm_axis, vec[1], rot[1]);
		vec3_scale(norm_axis, vec[2], rot[2]);

		rot[0][0] += c;       rot[1][0] -= vecs[2]; rot[2][0] += vecs[1];
		rot[0][1] += vecs[2]; rot[1][1] += c;       rot[2][1] -= vecs[0];
		rot[0][2] -= vecs[1]; rot[1][2] += vecs[0]; rot[2][2] += c;
		rot[0][3] = rot[1][3] = rot[2][3] = rot[3][0] = rot[3][1] = rot[3][2] = 0.f;
		rot[3][3] = 1.f;

		float a00 = mat[0][0], a01 = mat[0][1], a02 = mat[0][2], a03 = mat[0][3],
		      a10 = mat[1][0], a11 = mat[1][1], a12 = mat[1][2], a13 = mat[1][3],
		      a20 = mat[2][0], a21 = mat[2][1], a22 = mat[2][2], a23 = mat[2][3],
		      a30 = mat[3][0], a31 = mat[3][1], a32 = mat[3][2], a33 = mat[3][3],
		      b00 = rot[0][0], b01 = rot[0][1], b02 = rot[0][2],
		      b10 = rot[1][0], b11 = rot[1][1], b12 = rot[1][2],
		      b20 = rot[2][0], b21 = rot[2][1], b22 = rot[2][2];

		mat[0][0] = a00*b00 + a10*b01 + a20*b02;
		mat[0][1] = a01*b00 + a11*b01 + a21*b02;
		mat[0][2] = a02*b00 + a12*b01 + a22*b02;
		mat[0][3] = a03*b00 + a13*b01 + a23*b02;
		mat[1][0] = a00*b10 + a10*b11 + a20*b12;
		mat[1][1] = a01*b10 + a11*b11 + a21*b12;
		mat[1][2] = a02*b10 + a12*b11 + a22*b12;
		mat[1][3] = a03*b10 + a13*b11 + a23*b12;
		mat[2][0] = a00*b20 + a10*b21 + a20*b22;
		mat[2][1] = a01*b20 + a11*b21 + a21*b22;
		mat[2][2] = a02*b20 + a12*b21 + a22*b22;
		mat[2][3] = a03*b20 + a13*b21 + a23*b22; 
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

/* Frame rendering logic */

	// Handles keyboard input
	void key_input(void) {
		// Do wireframe if W is being held down
		if (keyboard[MU_KEYBOARD_W]) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	// Calculates the matrix data and sends it to the shader
	void calc_matrixes(void) {
		// Variables
		float model      [4][4]; matrix_set(model,      1.f);
		float view       [4][4]; matrix_set(view,       1.f);
		float projection [4][4]; matrix_set(projection, 1.f);
		// + Temp vec3 for calculations
		float temp_vec[3];

		// Rotate cube over time unevenly on angles
		temp_vec[0] = 1.f;
		temp_vec[1] = .5f;
		temp_vec[2] = 0.f;
		matrix_rotate(model, mu_time_get(), temp_vec);

		// Move cube to a viewable position
		temp_vec[0] = 0.f;
		temp_vec[1] = 0.f;
		temp_vec[2] = -2.f;
		matrix_translate(view, temp_vec);

		// Calculate perspective based on dimensions
		uint32_m dim[2];
		mu_window_get(win, MU_WINDOW_DIMENSIONS, dim);
		matrix_perspective(
			// FOV
			45.f * (3.141592653539f / 100.f),
			// Aspect
			(float)(dim[0]) / (float)(dim[1]),
			// Near-Z
			.1f,
			// Far-Z
			100.f,
			// Projection matrix to be filled
			projection
		);

		// Set matrix values
		glUniformMatrix4fv(glGetUniformLocation(shader, "model"     ), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"      ), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection[0][0]);
	}

	// Renders the scene
	void render(void) {
		// Clear background
		glClearColor(191.f / 255.f, 174.f / 255.f, 212.f / 255.f, 1.f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		// Draw triangles
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vert) / (sizeof(float)*6));

		// Swap buffers to present image
		mu_gl_swap_buffers(win);
	}

int main(void)
{

/* Initiation */

	// Initiate muCOSA
	muCOSA_context_create(&muCOSA, window_system, MU_TRUE);

	// Print currently running window system
	printf("Running window system \"%s\"\n",
		mu_window_system_get_nice_name(muCOSA_context_get_window_system(&muCOSA))
	);

	// Create window
	win = mu_window_create(&wininfo);

	// Create and bind OpenGL context
	gl = mu_gl_context_create(win, MU_OPENGL_3_3_CORE);
	mu_gl_bind(win, gl);

	// Get keyboard map
	mu_window_get(win, MU_WINDOW_KEYBOARD_MAP, &keyboard);

/* Print explanation */

	// ...

/* OpenGL setup */

	// Load OpenGL functions
	if (!gladLoadGL((GLADloadfunc)load_gl_func)) {
		printf("[WARNING] Failed to load OpenGL functions!\n");
	}

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Compile the shaders
	compile_shaders();

	// Create and bind a buffer for the vertex data
	load_vertexes();

/* Main loop */

	// Loop while window isn't closed:
	while (!mu_window_get_closed(win))
	{
		// Do matrix math
		calc_matrixes();

		// Handle keyboard input
		key_input();

		// Render scene
		render();

		// Update window (which refreshes input and such)
		mu_window_update(win);
	}

/* Termination */

	// Destroy window (required)
	win = mu_window_destroy(win);

	// Terminate muCOSA (required)
	muCOSA_context_destroy(&muCOSA);

	// Print possible error
	if (muCOSA.result != MUCOSA_SUCCESS) {
		printf("Something went wrong during the program's lifespan; result: %s\n",
			muCOSA_result_get_name(muCOSA.result)
		);
	} else {
		printf("Successful\n");
	}

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

