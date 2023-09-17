/*
* Copyright (c) 2021-23 ColleagueRiley ColleagueRiley@gmail.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following r estrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*
*
*/

/* 
MACRO #DEFINE ARGUMENTS

***
#define RGL_IMPLEMENTATION - include the RGL defines, this line must be included in at least one file or argument (-D), in your project
***

#define RGL_MODERN_OPENGL - Use the modern opengl backend (this is enabled by default)
#define RGL_LEGACY_OPENGL - Use the legacy opengl backend
#define RGL_NO_X64_OPTIMIZATIONS - Use x64 optimizations (x64 only), eg. SIMD
#define RGL_ALLOC_BATCHES - Allocate room for batches instead of using a stack-based c array
#define RGL_ALLOC_MATRIX_STACK - Allocate room for the matrix stack instead of using a stack-based c array
#define RGL_EBO - (modern opengl) use EBO for RGL_QUADS, this is off by default because it is buggy currently

Values

#define RGL_MAX_BATCHES <x> - Set the max amount of batches (a batch is created when a different mode or texture is used), 
							By default this value is set to 256
#define RGL_MAX_BUFFER_ELEMENTS <x> - Set the max number of elements the renderer uses (verticies, colors, texture coords)
								 By Default this value is set to 8192
#define RGL_MAX_MATRIX_STACK_SIZE <x> - Set the max number of matrix stack layers that can be active at a time
										 By default this value is set to 32
#define RGL_MALLOC <x> - set the malloc function for RGL and GLAD to use, by default this is the c standard malloc
#define RGL_FREE <x> - set the free function for RGL and GLAD to use, by default this is the c standard free
#define PI <x> - set the default value for PI used, by default this value is 3.14159265358979323846f
#define DEG2RAD - set the default value for deg2rad used, by default this value is (PI / 180.0f)
*/

/*
Credits :

RLGL (raylib / Raysay5) - 	{UPDATE : Most of this code was actually based on or built off RLGL!}
								A simular project, that this project's design is based on.
									RLGL was also used as a reference for some of this code.
								
*/


#ifndef RGL_H
#define RGL_H

#ifndef RGL_OPENGL_LEGACY
#define RGL_MODERN_OPENGL
#endif

#if !defined(u8)
#include <stdint.h>

typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
typedef u8 b8;
#endif
#ifndef RGL_MALLOC
    #include <stdlib.h>
    #define RGL_MALLOC(sz)     malloc(sz)
    #define RGL_FREE(p)        free(p)
#endif

#ifndef RGL_MAX_MATRIX_STACK_SIZE
#define RGL_MAX_MATRIX_STACK_SIZE 32
#endif

#ifndef RGL_MAX_BATCHES
#define RGL_MAX_BATCHES 256
#endif

#ifndef RGL_MAX_BUFFER_ELEMENTS
#define RGL_MAX_BUFFER_ELEMENTS 8192
#endif

#ifndef RGL_PROJECTION 
#define RGL_MODELVIEW                            0x1700      /* GL_MODELVIEW */
#define RGL_PROJECTION                           0x1701      /* GL_PROJECTION */
#endif

#ifndef RGL_QUADS
#define RGL_LINES                                0x0001      /* GL_LINES */
#define RGL_TRIANGLES                            0x0004      /* GL_TRIANGLES */
#define RGL_QUADS                                0x0007      /* GL_QUADS */
#endif

#if !defined(RGL_MATRIX_TYPE)
typedef struct RGL_MATRIX {
    float m[16];
} RGL_MATRIX;
#define RGL_MATRIX_TYPE
#endif

typedef struct RGL_BATCH {
    i32 mode;                   /* Drawing mode: LINES, TRIANGLES, QUADS */
    i32 vertexCount;            /* Number of vertex of the draw */
    i32 vertexAlignment;        /* Number of vertex required for index alignment (LINES, TRIANGLES) */
    u32 tex;     /* Texture id to be used on the draw -> Use to create new draw call if changes */
} RGL_BATCH;

#if defined(__cplusplus)
extern "C" {            /* Prevents name mangling of functions */
#endif


inline void rglInit(int width,  i32 height, void* loader);             /* Initialize RGLinfo (buffers, shaders, textures, states) */
inline void rglClose(void);                             /* De-initialize RGLinfo (buffers, shaders, textures) */
inline void rglSetFramebufferSize(int width,    i32 height);            /* Set current framebuffer size */

inline void rglRenderBatch(void);                         /* Draw render batch data (Update->Draw->Reset) */
inline void rglRenderBatchWithShader(u32 program, u32 vertexLocation, u32 texCoordLocation, u32 colorLocation);

inline void rglSetTexture(u32 id);               /* Set current texture for render batch and check buffers limits */
inline u32 rglCreateTexture(u8* bitmap, u32 width, u32 height, u8 channels); /* create texture */

#if defined(RGL_OPENGL_LEGACY)
#define rglBegin glBegin
#define rglColor3f glColor3f
#define rglColor3ub glColor4ub
#define rglColor4f glColor4f
#define rglColor4ub glColor4ub
#define rglTexCoord2f glTexCoord2f
#define rglTranslatef glTranslatef
#define rglScalef glScalef
#define rglVertex2f glVertex2f
#define rglVertex3f glVertex3f
#define rglPushMatrix glPushMatrix
#define rglPopMatrix glPopMatrix
#define rglEnd glEnd
#define rglMatrixMode glMatrixMode
#define rglLoadIdentity glLoadIdentity
#define rglRotatef glRotatef
#define rglOrtho glOrtho
#define rglFrustum glFrustum
#define rglMultMatrixf glMultMatrixf
#define rglViewport glViewport
#define rglLineWidth glLineWidt
#else

inline void rglBegin(int mode);
inline void rglEnd(void);
inline void rglTexCoord2f(float x, float y); 

inline void rglColor3ub(u8 r, u8 g, u8 b);
inline void rglColor4ub(u8 r, u8 g, u8 b, u8 a);

inline void rglColor3f(float r, float g, float b);
inline void rglColor4f(float r, float g, float b, float a);

inline void rglVertex2f(float x, float y);
inline void rglVertex3f(float x, float y, float z);

#define rglViewport glViewport
#define rglLineWidth glLineWidth

inline void rglMatrixMode(int mode);
inline void rglPushMatrix(void);
inline void rglPopMatrix(void);
inline void rglLoadIdentity(void);
inline void rglTranslatef(float x, float y, float z);
inline void rglRotatef(float angle, float x, float y, float z);
inline void rglMultMatrixf(const float *matf);
inline void rglOrtho(double left, double right, double bottom, double top, double znear, double zfar);

static RGL_MATRIX rglMatrixIdentity(void);                       /* Get identity matrix */
static RGL_MATRIX rglMatrixMultiply(float left[16], float right[16]);  /* Multiply two matrices */

inline i32 rglCheckRenderBatchLimit(int vCount);                             /* Check internal buffer overflow for a given number of vertex */

#endif
#if defined(__cplusplus)
}
#endif

#endif /* RGL_H */

#ifdef RGL_IMPLEMENTATION

#if defined(RGL_OPENGL_LEGACY)
    #if defined(__APPLE__)
        #include <OpenGL/gl.h>          /* OpenGL 1.1 library for OSX */
        #include <OpenGL/glext.h>       /* OpenGL extensions library */
    #else
        #include <GL/gl.h>
    #endif
#endif

#if defined(RGL_MODERN_OPENGL)
    #define GLAD_MALLOC RGL_MALLOC
    #define GLAD_FREE RGL_FREE

    #define GLAD_GL_IMPLEMENTATION
    #include "glad.h"      /* GLAD extensions loading library, includes OpenGL headers */
#endif

#include <math.h>                       /* Required for: sqrtf(), sinf(), cosf(), floor(), log() */

/* Defines and Macros */
#ifndef PI
    #define PI 3.14159265358979323846f
#endif
#ifndef DEG2RAD
    #define DEG2RAD (PI/180.0f)
#endif
#ifndef RAD2DEG
    #define RAD2DEG (180.0f/PI)
#endif

#ifdef RGL_MODERN_OPENGL
typedef struct RGL_INFO {
    i32 vertexCounter;                  /* Current active render batch vertex counter (generic, used for all batches) */
    float tcoord[3];
    float color[4];

    i32 matrixMode;              /* Current matrix mode */
    RGL_MATRIX* matrix;              /* Current matrix pointer */
    RGL_MATRIX modelview;                   /* Default modelview matrix */
    RGL_MATRIX projection;                  /* Default projection matrix */
    
	RGL_MATRIX transform; /* transformation matrix*/
	u8 transformRequired;

    #ifdef RGL_ALLOC_MATRIX_STACK 
    RGL_MATRIX* stack
    #else
    RGL_MATRIX stack[RGL_MAX_MATRIX_STACK_SIZE]; /* RGL_MATRIX stack for push/pop */
    #endif

    i32 stackCounter;                   /* RGL_MATRIX stack counter */

    u32 tex;      /* Default texture used on shapes/poly drawing (required by shader)*/
    u32 vShader;      /* Default vertex shader id (used by default shader program)*/
    u32 fShader;      /* Default fragment shader id (used by default shader program)*/
    u32 program;       /* Default shader program id, supports vertex color and diffuse texture*/
    u32 mvp;
    u32 defaultTex;

    i32 width;               /* Current framebuffer width*/
    i32 height;              /* Current framebuffer height*/

	float* vertices;
	float* colors;
    float* tcoords;

    u16* indices;
    u32 elementCount;

    i32 bufferCount;            /* Number of vertex buffers (multi-buffering support) */
    i32 currentBuffer;          /* Current buffer tracking in case of multi-buffering */
    i32 drawCounter;            /* Draw calls counter */

    #ifdef RGL_ALLOC_BATCHES
    RGL_BATCH* batches;          /* Draw calls array, depends on tex */
    #else
    RGL_BATCH batches[RGL_MAX_BATCHES];
    #endif

    u32 vao, vbo, tbo, cbo, ebo; /* array object and array buffers */
} RGL_INFO;

static RGL_INFO RGLinfo;
#endif /* RGL_MODERN_OPENGL */

void rglSetTexture(u32 id) {
#if defined(RGL_OPENGL_LEGACY)
    glBindTexture(GL_TEXTURE_2D, id);
#else
    RGLinfo.tex = id;

    if (id == 0)
        RGLinfo.tex = RGLinfo.defaultTex;

    RGLinfo.vertexCounter += RGLinfo.batches[RGLinfo.drawCounter - 1].vertexAlignment;
    RGLinfo.drawCounter++;
    RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount = 0;
#endif
}

u32 rglCreateTexture(u8* bitmap, u32 width, u32 height, u8 channels) {
    unsigned int id = 0;

    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
    
    unsigned int c;

    switch (channels) {
        case 1: c = GL_RED; break;
        case 2: c = GL_RG; break;
        case 3: c = GL_RGB; break;
        case 4: c = GL_RGBA; break;
        default: break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, c, width, height, 0, c, GL_UNSIGNED_BYTE, bitmap);

    glBindTexture(GL_TEXTURE_2D, 0);

    return id;
}

#ifdef RGL_DEBUG

void RGL_opengl_getError() {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		switch (err) {
		case GL_INVALID_ENUM:
			printf("OpenGL error: GL_INVALID_ENUM\n");
			break;
		case GL_INVALID_VALUE:
			printf("OpenGL error: GL_INVALID_VALUE\n");
			break;
		case GL_INVALID_OPERATION:
			printf("OpenGL error: GL_INVALID_OPERATION\n");
			break;
		case GL_STACK_OVERFLOW:
			printf("OpenGL error: GL_STACK_OVERFLOW\n");
			break;
		case GL_STACK_UNDERFLOW:
			printf("OpenGL error: GL_STACK_UNDERFLOW\n");
			break;
		default:
			printf("OpenGL error: Unknown error code 0x%x\n", err);
			break;
		}
	}
}

void RGL_debug_shader(u32 src, const char *shader, const char *action) {
	GLint status;
	if (action[0] == 'l')
		glGetProgramiv(src, GL_LINK_STATUS, &status);
	else
		glGetShaderiv(src, GL_COMPILE_STATUS, &status);

	if (status == GL_TRUE)
		printf("%s Shader %s successfully.\n", shader, action);
	else {
		printf("%s Shader failed to %s.\n", shader, action);

		if (action[0] == 'c') {
			GLint infoLogLength;
			glGetShaderiv(src, GL_INFO_LOG_LENGTH, &infoLogLength);

			if (infoLogLength > 0) {
				GLchar *infoLog = (GLchar *)RGL_MALLOC(infoLogLength);
				glGetShaderInfoLog(src, infoLogLength, NULL, infoLog);
				printf("%s Shader info log:\n%s\n", shader, infoLog);
				free(infoLog);
			}
		}

		RGL_opengl_getError();
		exit(1);
	}
}
#endif /* RGL_DEBUG */

#define RGL_MULTILINE_STR(...) #__VA_ARGS__

/* Initialize RGLinfo: OpenGL extensions, default buffers/shaders/textures, OpenGL states*/
void rglInit(int width, i32 height, void *loader) {
#if defined(RGL_MODERN_OPENGL)
    gladLoadGL((GLADloadfunc)loader);

    static const char *defaultVShaderCode = RGL_MULTILINE_STR(
        \x23version 330                    \n
        in vec3 vertexPosition;
        in vec2 vertexTexCoord;
        in vec4 vertexColor;
        out vec2 fragTexCoord;
        out vec4 fragColor;
        uniform mat4 mvp;
        void main() {
            fragTexCoord = vertexTexCoord;
            fragColor = vertexColor;
            gl_Position = mvp*vec4(vertexPosition, 1.0);
        }
    );

    static const char *defaultFShaderCode = RGL_MULTILINE_STR(
        \x23version 330       \n
        in vec2 fragTexCoord;              
        in vec4 fragColor;                 
        out vec4 finalColor;     
        uniform sampler2D texture0;        
        void main() { 
            finalColor = texture(texture0, fragTexCoord) * fragColor;
        }                               
    );

	glGenVertexArrays(1, &RGLinfo.vao);
	glBindVertexArray(RGLinfo.vao);

	glGenBuffers(1, &RGLinfo.vbo);
	glGenBuffers(1, &RGLinfo.tbo);
	glGenBuffers(1, &RGLinfo.cbo);
	glGenBuffers(1, &RGLinfo.ebo);

	/* compile vertex shader */
	RGLinfo.vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(RGLinfo.vShader, 1, &defaultVShaderCode, NULL);
	glCompileShader(RGLinfo.vShader);

    #ifdef RGL_DEBUG
        RGL_debug_shader(RGLinfo.vShader, "Vertex", "compile");
    #endif

	/* compile fragment shader */
	RGLinfo.fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(RGLinfo.fShader, 1, &defaultFShaderCode, NULL);
	glCompileShader(RGLinfo.fShader);

    #ifdef RGL_DEBUG
        RGL_debug_shader(RGLinfo.fShader, "Fragment", "compile");
    #endif

	/* create program and link vertex and fragment shaders */
	RGLinfo.program = glCreateProgram();

	glAttachShader(RGLinfo.program, RGLinfo.vShader);
	glAttachShader(RGLinfo.program, RGLinfo.fShader);

    glBindAttribLocation(RGLinfo.vShader, 0, "vertexPosition");
    glBindAttribLocation(RGLinfo.vShader, 1, "vertexTexCoord");
    glBindAttribLocation(RGLinfo.vShader, 2, "vertexColor");

	glLinkProgram(RGLinfo.program);

    /* Set default shader locations: uniform locations */
    RGLinfo.mvp  = glGetUniformLocation(RGLinfo.program, "mvp");

    /* Init default vertex arrays buffers */
    /* Initialize CPU (RAM) vertex buffers (position, texcoord, color data and indexes) */

    RGLinfo.elementCount = RGL_MAX_BUFFER_ELEMENTS;

    RGLinfo.vertices = (float *)RGL_MALLOC(RGL_MAX_BUFFER_ELEMENTS * 3 * 4 * sizeof(float));
    RGLinfo.tcoords = (float *)RGL_MALLOC(RGL_MAX_BUFFER_ELEMENTS * 2 * 4 * sizeof(float));
    RGLinfo.colors = (float*)RGL_MALLOC(RGL_MAX_BUFFER_ELEMENTS * 4 * 4 * sizeof(float));
    RGLinfo.indices = (u16*)RGL_MALLOC(RGL_MAX_BUFFER_ELEMENTS * 6 * sizeof(u16));

    i32 k = 0, j;

    /* Indices can be initialized right now */
    for (j = 0; j < (6 * RGL_MAX_BUFFER_ELEMENTS); j += 6) {
        RGLinfo.indices[j] = 4*k;
        RGLinfo.indices[j + 1] = 4*k + 1;
        RGLinfo.indices[j + 2] = 4*k + 2;
        RGLinfo.indices[j + 3] = 4*k;
        RGLinfo.indices[j + 4] = 4*k + 2;
        RGLinfo.indices[j + 5] = 4*k + 3;

        k++;
    }

    RGLinfo.vertexCounter = 0;

    glGenVertexArrays(1, &RGLinfo.vao);
    glBindVertexArray(RGLinfo.vao);

    /* Quads - Vertex buffers binding and attributes enable */
    /* Vertex position buffer (shader-location = 0) */
    glGenBuffers(1, &RGLinfo.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, RGLinfo.vbo);
    glBufferData(GL_ARRAY_BUFFER, RGL_MAX_BUFFER_ELEMENTS * 3 * 4 * sizeof(float), RGLinfo.vertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);

    /* Vertex texcoord buffer (shader-location = 1) */
    glGenBuffers(1, &RGLinfo.tbo);
    glBindBuffer(GL_ARRAY_BUFFER, RGLinfo.tbo);
    glBufferData(GL_ARRAY_BUFFER, RGL_MAX_BUFFER_ELEMENTS * 2 * 4 * sizeof(float), RGLinfo.tcoords, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);

    /* Vertex color buffer (shader-location = 3) */
    glGenBuffers(1, &RGLinfo.cbo);
    glBindBuffer(GL_ARRAY_BUFFER, RGLinfo.cbo);
    glBufferData(GL_ARRAY_BUFFER, RGL_MAX_BUFFER_ELEMENTS * 4 * 4 * sizeof(float), RGLinfo.colors, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, 0, 0);

    /* Fill index buffer */
    glGenBuffers(1, &RGLinfo.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RGLinfo.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, RGL_MAX_BUFFER_ELEMENTS * 6 * sizeof(u16), RGLinfo.indices, GL_STATIC_DRAW);

    /* Unbind the current VAO */
    if (RGLinfo.vao) 
        glBindVertexArray(0);

    /* load default texture */
    u8 white[4] = {255, 255, 255, 255};
    RGLinfo.defaultTex = rglCreateTexture(white, 1, 1, 4);
    RGLinfo.tex = RGLinfo.defaultTex;

    #ifdef RGL_ALLOC_BATCHES
    RGLinfo.batches = (RGL_BATCH *)RGL_MALLOC(RGL_MAX_BATCHES * sizeof(RGL_BATCH));
    #endif
    
    #ifdef RGL_ALLOC_MATRIX_STACK
    RGLinfo.statck = (RGL_MATRIX*)RGL_MALLOC(RGL_MAX_MATRIX_STACK_SIZE * sizeof(RGL_MATRIX));
    #endif

    u32 i;
    for (i = 0; i < RGL_MAX_BATCHES; i++) {
        RGLinfo.batches[i].mode = RGL_QUADS;
        RGLinfo.batches[i].vertexCount = 0;
        RGLinfo.batches[i].vertexAlignment = 0;
        RGLinfo.batches[i].tex = RGLinfo.tex;
    }

    RGLinfo.bufferCount = 1;    /* Record buffer count */
    RGLinfo.drawCounter = 1;             /* Reset draws counter */
    
    /* Init stack matrices (emulating OpenGL 1.1) */
    for (i = 0; i < RGL_MAX_MATRIX_STACK_SIZE; i++) RGLinfo.stack[i] = rglMatrixIdentity();

    /* Init internal matrices */
    /*RGLinfo.transform = rglMatrixIdentity(); */
    RGLinfo.projection = rglMatrixIdentity();
    RGLinfo.modelview = rglMatrixIdentity();
    RGLinfo.matrix = &RGLinfo.modelview;

    /* Store screen size into global variables */
    RGLinfo.width = width;
    RGLinfo.height = height;
#endif
}

/* Vertex Buffer Object deinitialization (memory free) */
void rglClose(void) {
#if defined(RGL_MODERN_OPENGL)
    /* Unbind everything */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Unload all vertex buffers data */
    glBindVertexArray(RGLinfo.vao);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glBindVertexArray(0);

    /* Delete VBOs from GPU (VRAM) */
    glDeleteBuffers(1, &RGLinfo.vbo);
    glDeleteBuffers(1, &RGLinfo.tbo);
    glDeleteBuffers(1, &RGLinfo.cbo);
    glDeleteBuffers(1, &RGLinfo.ebo);

    glDeleteVertexArrays(1, &RGLinfo.vao);

    /* Free vertex arrays memory from CPU (RAM) */
    RGL_FREE(RGLinfo.vertices);
    RGL_FREE(RGLinfo.tcoords);
    RGL_FREE(RGLinfo.colors);
    RGL_FREE(RGLinfo.indices);

    /* Unload arrays */
    #ifdef RGL_ALLOC_BATCHES
    RGL_FREE(RGLinfo.batches);
    #endif

    #ifdef RGL_ALLOC_MATRIX_STACK
    RGL_FREE(RGLinfo.stack);
    #endif

    glUseProgram(0);

    glDetachShader(RGLinfo.program, RGLinfo.vShader);
    glDetachShader(RGLinfo.program, RGLinfo.fShader);
    glDeleteShader(RGLinfo.vShader);
    glDeleteShader(RGLinfo.fShader);

    glDeleteProgram(RGLinfo.program);

    glDeleteTextures(1, &RGLinfo.tex); /* Unload default texture */
#endif
}

void rglSetFramebufferSize(int width,   i32 height) {
    #ifndef RGL_OPENGL_LEGACY
    RGLinfo.width = width;
    RGLinfo.height = height;
    #endif
    glViewport(0, 0, width, height);
}


void rglRenderBatch() {
    #if defined(RGL_MODERN_OPENGL)
    rglRenderBatchWithShader(RGLinfo.program, 0, 1, 2);
    #endif
}

void rglRenderBatchWithShader(u32 program, u32 vertexLocation, u32 texCoordLocation, u32 colorLocation) {
#if defined(RGL_MODERN_OPENGL)
    if (RGLinfo.vertexCounter > 0) {
        glBindVertexArray(RGLinfo.vao);

        /* Vertex positions buffer */
        glBindBuffer(GL_ARRAY_BUFFER, RGLinfo.vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, RGLinfo.vertexCounter * 3 * sizeof(float), RGLinfo.vertices);

        /* Texture coordinates buffer */
        glBindBuffer(GL_ARRAY_BUFFER, RGLinfo.tbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, RGLinfo.vertexCounter * 2 * sizeof(float), RGLinfo.tcoords);
        
        /* Colors buffer */
        glBindBuffer(GL_ARRAY_BUFFER, RGLinfo.cbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, RGLinfo.vertexCounter * 4 * sizeof(float), RGLinfo.colors); 
        
        glBindVertexArray(0);

        /* Set current shader and upload current MVP matrix */
        glUseProgram(program);

        RGL_MATRIX matMVP = rglMatrixMultiply(RGLinfo.modelview.m, RGLinfo.projection.m);
        glUniformMatrix4fv(RGLinfo.mvp, 1, 0, matMVP.m);

        glBindVertexArray(RGLinfo.vao);

        /* Bind vertex attrib: position (shader-location = 0) */
        glBindBuffer(GL_ARRAY_BUFFER, RGLinfo.vbo);
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, 0, 0, 0);
        glEnableVertexAttribArray(vertexLocation);

        /* Bind vertex attrib: texcoord (shader-location = 1) */
        glBindBuffer(GL_ARRAY_BUFFER, RGLinfo.tbo);
        glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, 0, 0, 0);
        glEnableVertexAttribArray(texCoordLocation);

        /* Bind vertex attrib: color (shader-location = 3) */
        glBindBuffer(GL_ARRAY_BUFFER, RGLinfo.cbo);
        glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_TRUE, 0, 0);
        glEnableVertexAttribArray(colorLocation);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RGLinfo.ebo);

        glActiveTexture(GL_TEXTURE0);

        u32 vertexOffset;
        u32 i;

        for (i = 0, vertexOffset = 0; i < RGLinfo.drawCounter; i++) {
            /* Bind current draw call texture, activated as GL_TEXTURE0 and Bound to sampler2D texture0 by default */
            glBindTexture(GL_TEXTURE_2D, RGLinfo.batches[i].tex);

            #ifdef RGL_EBO
            if ((RGLinfo.batches[i].mode == RGL_LINES) || (RGLinfo.batches[i].mode == RGL_TRIANGLES)) 
            #endif
                glDrawArrays(RGLinfo.batches[i].mode, vertexOffset, RGLinfo.batches[i].vertexCount);
            
            #ifdef RGL_EBO
            else
                glDrawElements(GL_TRIANGLES, RGLinfo.batches[i].vertexCount / 4 * 6, GL_UNSIGNED_SHORT, (GLvoid *)(vertexOffset / 4 * 6 * sizeof(GLushort)));
            #endif

            vertexOffset += (RGLinfo.batches[i].vertexCount + RGLinfo.batches[i].vertexAlignment);
        }

        if (!RGLinfo.vao) {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        glBindTexture(GL_TEXTURE_2D, 0);    /* Unbind textures */
    }

    if (RGLinfo.vao) 
        glBindVertexArray(0); /* Unbind VAO */

    glUseProgram(0);    /* Unbind shader program */

    /* Reset vertex counter for next frame */
    RGLinfo.vertexCounter = 0;

    /* Reset RGLinfo.batches array */
    i32 i;
    for (i = 0; i < RGL_MAX_BATCHES; i++) {
        RGLinfo.batches[i].vertexCount = 0;
        RGLinfo.batches[i].tex = RGLinfo.tex;
    }

    /* Reset draws counter to one draw for the batch */
    RGLinfo.drawCounter = 1;

    /* Change to next buffer in the list (in case of multi-buffering) */
    RGLinfo.currentBuffer++;
    if (RGLinfo.currentBuffer >= RGLinfo.bufferCount) 
        RGLinfo.currentBuffer = 0;
#endif
}

#if defined(RGL_MODERN_OPENGL)

/* Check internal buffer overflow for a given number of vertex */
/* and force a void draw call if required */
int rglCheckRenderBatchLimit(int vCount) {
    if ((RGLinfo.vertexCounter + vCount) < (RGLinfo.elementCount*4))
        return 0;

    /* Store current primitive drawing mode and texture id */
    i32 currentMode = RGLinfo.batches[RGLinfo.drawCounter - 1].mode;

    rglRenderBatch();    /* NOTE: Stereo rendering is checked inside */

    /* Restore state of last batch so we can continue adding vertices */
    RGLinfo.batches[RGLinfo.drawCounter - 1].mode = currentMode;
    RGLinfo.batches[RGLinfo.drawCounter - 1].tex = RGLinfo.tex;
    
    return 1; 
}

/* Initialize drawing mode (how to organize vertex) */
void rglBegin(int mode) {
    if (RGLinfo.batches[RGLinfo.drawCounter - 1].mode != mode && 
        RGLinfo.batches[RGLinfo.drawCounter - 1].tex != RGLinfo.tex &&
        RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount > 0) {
            if (RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_LINES) 
                RGLinfo.batches[RGLinfo.drawCounter - 1].vertexAlignment = ((RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount < 4)? RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount : RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount%4);
            else if (RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_TRIANGLES) 
                RGLinfo.batches[RGLinfo.drawCounter - 1].vertexAlignment = ((RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount < 4)? 1 : (4 - (RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount%4)));
            else 
                RGLinfo.batches[RGLinfo.drawCounter - 1].vertexAlignment = 0;

            if (!rglCheckRenderBatchLimit(RGLinfo.batches[RGLinfo.drawCounter - 1].vertexAlignment)) {
                RGLinfo.vertexCounter += RGLinfo.batches[RGLinfo.drawCounter - 1].vertexAlignment;
                RGLinfo.drawCounter++;
            }

            if (RGLinfo.drawCounter >= RGL_MAX_BATCHES) 
                rglRenderBatch();

            RGLinfo.batches[RGLinfo.drawCounter - 1].mode = mode;
            RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount = 0;
            RGLinfo.batches[RGLinfo.drawCounter - 1].tex = RGLinfo.tex;
        }
}

void rglEnd(void) {

}

/* Define one vertex (texture coordinate) */
void rglTexCoord2f(float x, float y) {
    RGLinfo.tcoord[0] = x;
    RGLinfo.tcoord[1] = y;
}

/* Define one vertex (color) */
void rglColor3ub(u8 r, u8 g, u8 b) {
    rglColor4ub(r, g, b, 255);
} 

void rglColor4ub(u8 r, u8 g, u8 b, u8 a) {
    rglColor4f(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void rglColor3f(float r, float g, float b) {
    rglColor4f(r, g, b, 1);
}

void rglColor4f(float r, float g, float b, float a) {
    RGLinfo.color[0] = r;
    RGLinfo.color[1] = g;
    RGLinfo.color[2] = b;
    RGLinfo.color[3] = a;
}

/* Define one vertex (position) */
void rglVertex2f(float x, float y) {
    rglVertex3f(x, y, 0.0f);
}

void rglVertex3f(float x, float y, float z) {
    float tx = x;
    float ty = y;
    float tz = z;

    if (RGLinfo.transformRequired){
        tx = RGLinfo.transform.m[0] * x + RGLinfo.transform.m[4] * y + RGLinfo.transform.m[8] * z + RGLinfo.transform.m[12];
        ty = RGLinfo.transform.m[1] * x + RGLinfo.transform.m[5] * y + RGLinfo.transform.m[9] * z + RGLinfo.transform.m[13];
        tz = RGLinfo.transform.m[2] * x + RGLinfo.transform.m[6] * y + RGLinfo.transform.m[10] * z + RGLinfo.transform.m[14];
    }

    if (RGLinfo.vertexCounter > (RGLinfo.elementCount * 4 - 4)) {
        if ((RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_LINES) &&
            (RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount%2 == 0))
                rglCheckRenderBatchLimit(2 + 1);
        else if ((RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_TRIANGLES) &&
            (RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount%3 == 0))
                rglCheckRenderBatchLimit(3 + 1);
        else if ((RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_QUADS) &&
            (RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount%4 == 0))
                rglCheckRenderBatchLimit(4 + 1);
    }

    /* Add vertices */
    RGLinfo.vertices[3*RGLinfo.vertexCounter] = tx;
    RGLinfo.vertices[3*RGLinfo.vertexCounter + 1] = ty;
    RGLinfo.vertices[3*RGLinfo.vertexCounter + 2] = tz;

    i8 i;
    for (i = 0; i < 2; i ++)
        RGLinfo.tcoords[2 * RGLinfo.vertexCounter + i] = RGLinfo.tcoord[i];

    for (i = 0; i < 4; i ++)
        RGLinfo.colors[4 * RGLinfo.vertexCounter + i] = RGLinfo.color[i];

    RGLinfo.vertexCounter++;
    RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount++;
    RGLinfo.batches[RGLinfo.drawCounter - 1].tex = RGLinfo.tex;
}


/* Multiply the current matrix by a translation matrix */
void rglTranslatef(float x, float y, float z) {
    RGL_MATRIX matTranslation = { 
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            x,    y,    z,    1.0f
        }
    };

    /* NOTE: We transpose matrix with multiplication order */
    *RGLinfo.matrix = rglMatrixMultiply(matTranslation.m, RGLinfo.matrix->m);
}

/* Multiply the current matrix by a rotation matrix */
void rglRotatef(float angle, float x, float y, float z) {
	/* Axis vector (x, y, z) normalization */
	float lengthSquared = x * x + y * y + z * z;
	if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f)) {
		float inverseLength = 1.0f / sqrtf(lengthSquared);
		x *= inverseLength;
		y *= inverseLength;
		z *= inverseLength;
	}

	/* Rotation matrix generation */
	float sinres = sinf(DEG2RAD * angle);
	float cosres = cosf(DEG2RAD * angle);
	float t = 1.0f - cosres;

	float matRotation[16] = 
				{
					x * x * t + cosres,   	  	y * x * t + z * sinres,   	z * x * t - y * sinres,   	0.0f,
					x * y * t - z * sinres,   	y * y * t + cosres,   		z * y * t + x * sinres,   	0.0f,
					x * z * t + y * sinres,   	y * z * t - x * sinres,  	z * z * t + cosres,   		0.0f,
					0.0f,   					0.0f,   					0.0f,   					1.0f
				};

	*RGLinfo.matrix = rglMatrixMultiply(matRotation, RGLinfo.matrix->m);
}

/* Multiply the current matrix by an orthographic matrix generated by parameters */
void rglOrtho(double left, double right, double bottom, double top, double znear, double zfar) {
	float rl = (float)(right - left);
	float tb = (float)(top - bottom);
	float fn = (float)(zfar - znear);

	float tx = -((float)(right + left) / rl);
	float ty = -((float)(top + bottom) / tb);
	float tz = -((float)(zfar + znear) / fn);

	float matOrtho[16] = 
			{
				2.0f / rl,   0.0f,          0.0f,    	  0.0f,
				0.0f,        2.0 / tb,      0.0f,         0.0f,   
				0.0f,        0.0f,          -2.0f / fn,   0.0f,
				tx,          ty,            tz,           1.0f
			};

    *RGLinfo.matrix = rglMatrixMultiply(RGLinfo.matrix->m, matOrtho);
}

/* Choose the current matrix to be transformed */
void rglMatrixMode(int mode) {
	RGLinfo.matrixMode = mode;

	if (mode == RGL_MODELVIEW)
		RGLinfo.matrix = &RGLinfo.modelview;
	else
		RGLinfo.matrix = &RGLinfo.projection;
}

/* Push the current matrix into RGLinfo.stack */
void rglPushMatrix(void) {
    RGLinfo.stack[RGLinfo.stackCounter] = *RGLinfo.matrix;

    if (RGLinfo.matrixMode == RGL_MODELVIEW) {
        RGLinfo.transformRequired = 1;
        RGLinfo.matrix = &RGLinfo.transform;
    }

    RGLinfo.stackCounter++;
}

/* Pop lattest inserted matrix from RGLinfo.stack */
void rglPopMatrix(void) {
    if (RGLinfo.stackCounter > 0) {
        RGL_MATRIX mat = RGLinfo.stack[RGLinfo.stackCounter - 1];
        *RGLinfo.matrix = mat;
        RGLinfo.stackCounter--;
    }

    if ((RGLinfo.stackCounter == 0) && (RGLinfo.matrixMode == RGL_MODELVIEW)) {
        RGLinfo.matrix = &RGLinfo.modelview;
		RGLinfo.transformRequired = 0;
	}
}

/* Reset current matrix to identity matrix */
void rglLoadIdentity(void) {
    *RGLinfo.matrix = rglMatrixIdentity();
}

/* Get identity matrix */
static RGL_MATRIX rglMatrixIdentity(void) {
    return (RGL_MATRIX) { 
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    };
}

void rglMultMatrixf(const float* m) {
	*RGLinfo.matrix = rglMatrixMultiply(RGLinfo.matrix->m, (float*)m);
}

static RGL_MATRIX rglMatrixMultiply(float left[16], float right[16]) {
    return (RGL_MATRIX) {
        {
            left[0] * right[0] + left[1] * right[4] + left[2] * right[8] + left[3] * right[12],
            left[0] * right[1] + left[1] * right[5] + left[2] * right[9] + left[3] * right[13],
            left[0] * right[2] + left[1] * right[6] + left[2] * right[10] + left[3] * right[14],
            left[0] * right[3] + left[1] * right[7] + left[2] * right[11] + left[3] * right[15],
            left[4] * right[0] + left[5] * right[4] + left[6] * right[8] + left[7] * right[12],
            left[4] * right[1] + left[5] * right[5] + left[6] * right[9] + left[7] * right[13],
            left[4] * right[2] + left[5] * right[6] + left[6] * right[10] + left[7] * right[14],
            left[4] * right[3] + left[5] * right[7] + left[6] * right[11] + left[7] * right[15],
            left[8] * right[0] + left[9] * right[4] + left[10] * right[8] + left[11] * right[12],
            left[8] * right[1] + left[9] * right[5] + left[10] * right[9] + left[11] * right[13],
            left[8] * right[2] + left[9] * right[6] + left[10] * right[10] + left[11] * right[14],
            left[8] * right[3] + left[9] * right[7] + left[10] * right[11] + left[11] * right[15],
            left[12] * right[0] + left[13] * right[4] + left[14] * right[8] + left[15] * right[12],
            left[12] * right[1] + left[13] * right[5] + left[14] * right[9] + left[15] * right[13],
            left[12] * right[2] + left[13] * right[6] + left[14] * right[10] + left[15] * right[14],
            left[12] * right[3] + left[13] * right[7] + left[14] * right[11] + left[15] * right[15]
        }
    };
}

#endif /* RGL_MODERN_OPENGL */

#endif /* RGL_IMPLEMENTATION */