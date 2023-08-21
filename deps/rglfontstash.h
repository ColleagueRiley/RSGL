//
// Copyright (c) 2009-2013 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//	claim that you wrote the original software. If you use this software
//	in a product, an acknowledgment in the product documentation would be
//	appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//	misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

/*
this is a heavily debloated version of
glfontstash that supports rlgl
made for RSGL
- ColleagueRiley
*/

typedef struct FONSRenderContext {
	GLuint tex;
	int width, height;
	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint tcoordBuffer;
	GLuint colorBuffer;
} FONSRenderContext;

#define FONTSTASH_IMPLEMENTATION
#include "fontstash.h"

#define glfonsRGBA(r, g, b, a) ((r) | (g << 8) | (b << 16) | (a << 24));

inline void fonsDeleteBuffer(GLuint* buffer);
inline void fonsCreateBuffer(GLuint* buffer);

void fonsDeleteBuffer(GLuint* buffer) {
	#ifndef GRAPHICS_API_OPENGL_11
	if (!*buffer) return;
	glDeleteTextures(1, buffer);
	buffer = 0;
	#endif
}
void fonsCreateBuffer(GLuint* buffer) {
	#ifndef GRAPHICS_API_OPENGL_11
	glGenBuffers(1, buffer);
	#endif
}

int fons_renderCreate(FONSRenderContext* gl, int width, int height) {
	// Create may be called multiple times, delete existing texture.
	
	#ifndef GRAPHICS_API_OPENGL_11
	fonsDeleteBuffer(&gl->tex);
	#endif

	glGenTextures(1, &gl->tex);
	if (!gl->tex)
		return 0;

	#ifndef GRAPHICS_API_OPENGL_11
	if (!gl->vertexArray)
		glGenVertexArrays(1, &gl->vertexArray);

	glBindVertexArray(gl->vertexArray);
	#endif

	#ifndef GRAPHICS_API_OPENGL_11
	fonsCreateBuffer(&gl->vertexBuffer);
	fonsCreateBuffer(&gl->tcoordBuffer);
	fonsCreateBuffer(&gl->colorBuffer);

	if (!gl->tex || !gl->vertexArray || !gl->colorBuffer || !gl->tcoordBuffer || !gl->vertexArray || !gl->vertexBuffer)
		return 0;
	#endif

	gl->width = width;
	gl->height = height;
	glBindTexture(GL_TEXTURE_2D, gl->tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, gl->width, gl->height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	static GLint swizzleRgbaParams[4] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRgbaParams);

	return 1;
}


inline void fons_pushPixelValues(GLint alignment, GLint rowLength, GLint skipPixels, GLint skipRows);
void fons_pushPixelValues(GLint alignment, GLint rowLength, GLint skipPixels, GLint skipRows) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, rowLength);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, skipPixels);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, skipRows);
}

void fons_renderUpdate(FONSRenderContext* gl, int *rect, const unsigned char *data) {
	int w = rect[2] - rect[0];
	int h = rect[3] - rect[1];

	if (gl->tex == 0)
		return;

	// Push old values
	GLint alignment, rowLength, skipPixels, skipRows;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowLength);
	glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skipPixels);
	glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skipRows);

	glBindTexture(GL_TEXTURE_2D, gl->tex);

	fons_pushPixelValues(1, gl->width, rect[0], rect[1]);

	glTexSubImage2D(GL_TEXTURE_2D, 0, rect[0], rect[1], w, h, GL_RED, GL_UNSIGNED_BYTE, data);

	// Pop old values
	fons_pushPixelValues(alignment, rowLength, skipPixels, skipRows);
}

void fons_renderDraw(FONSRenderContext* gl, const float *verts, const float *tcoords, const unsigned int *colors, int nverts) {
	if (gl->tex == 0)
		return;

	rlSetTexture(gl->tex);

	rlPushMatrix();
	rlBegin(GL_QUADS);

	int i, j = 0;
	for (i = 0; i < nverts * 6; i += 2) {
		rlTexCoord2f(tcoords[i], tcoords[i + 1]);

		if (j++ && j == 2 && (j -= 3))
			rlVertex2f(verts[i], verts[i + 1]);

		rlVertex2f(verts[i], verts[i + 1]);
	}
	rlEnd();
	rlPopMatrix();

	rlSetTexture(0);
}

void fons_renderDelete(FONSRenderContext* gl) {
	#ifndef GRAPHICS_API_OPENGL_11
	fonsDeleteBuffer(&gl->tex);
	fonsDeleteBuffer(&gl->vertexBuffer);
	fonsDeleteBuffer(&gl->tcoordBuffer);
	fonsDeleteBuffer(&gl->colorBuffer);
	fonsDeleteBuffer(&gl->vertexArray);
	#endif

	#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_43)
	glBindVertexArray(0);
	#endif
	free(gl);
}