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

(MAKE SURE RGL_IMPLEMENTATION is in exactly one header or you use -D RGL_IMPLEMENTATION)

***
#define RGL_IMPLEMENTATION - include the RGL defines, this line must be included in exactly one file or argument (-D), in your project
***

#define RGL_MODERN_OPENGL - Use the modern opengl backend (this is enabled by default)
#define RGL_OPENGL_LEGACY - Use the legacy opengl backend
#define RGL_NO_GL_LOADER  - Do not use the RGL OpenGL loader (you'll have to use something like GLAD instead)
#define RGL_NO_X64_OPTIMIZATIONS - Use x64 optimizations (x64 only), eg. SIMD
#define RGL_ALLOC_BATCHES - Allocate room for batches instead of using a stack-based c array
#define RGL_ALLOC_MATRIX_STACK - Allocate room for the matrix stack instead of using a stack-based c array
#define RGL_EBO - (modern opengl) use EBO for RGL_QUADS, this is off by default because it is buggy currently

#define RGL_OPENGL_21 - Load and use  opengl version 4.3 functionaries
#define RGL_OPENGL_33 - Load and use opengl version 4.3 functionaries
#define RGL_OPENGL_43 - Load and use  opengl version 4.3 functionaries
#define RGL_OPENGL_ES2 - Load and use  opengl ES version 2 functionaries
#define RGL_OPENGL_ES3 - Load and use  opengl ES version 3 functionaries

Values

#define RGL_MAX_BATCHES <x> - Set the max amount of batches (a batch is created when a different mode or texture is used), 
							By default this value is set to 256
#define RGL_MAX_BUFFER_ELEMENTS <x> - Set the max number of elements the renderer uses (verticies, colors, texture coords)
								 By Default this value is set to 8192
#define RGL_MAX_MATRIX_STACK_SIZE <x> - Set the max number of matrix stack layers that can be active at a time
										 By default this value is set to 32
#define RGL_MALLOC <x> - set the malloc function for RGL, by default this is the c standard malloc
#define RGL_FREE <x> - set the free function for RGL, by default this is the c standard free
#define PI <x> - set the default value for PI used, by default this value is 3.14159265358979323846f
#define DEG2RAD - set the default value for deg2rad used, by default this value is (PI / 180.0f)
*/

/*
Credits :

RLGL (raylib / Raysay5) - 	{UPDATE : Most of this code was actually based on or built off RLGL!}
								A simular project, that this project's design is based on.
									RLGL was also used as a reference for some of this code.
								
*/

#if defined(__APPLE__)
    #include <OpenGL/gl.h>          /* OpenGL 1.1 library for OSX */
    #include <OpenGL/glext.h>       /* OpenGL extensions library */
#else
    #include <GL/gl.h>
#endif

#ifdef RGL_DIRECTX
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#endif

#if defined(__WIN32) && !defined(__linux__) && !defined(GL_VERTEX_SHADER)
typedef char GLchar;
typedef int	 GLsizei;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

#define GL_VERTEX_SHADER   0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_ARRAY_BUFFER         0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW  0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_TEXTURE0 0x84C0
#endif

#ifdef __APPLE__
#define RGLDEF extern inline
#else
#define RGLDEF inline
#endif

#ifndef RGL_H
#define RGL_H

#ifndef RGL_OPENGL_LEGACY
#define RGL_MODERN_OPENGL
#if !defined(RGL_OPENGL_21) && !defined(RGL_OPENGL_33) && !defined(RGL_OPENGL_43)
#define RGL_OPENGL_33
#endif
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
#define RGL_MAX_BATCHES 2028
#endif

#ifndef RGL_MAX_BUFFER_ELEMENTS
#define RGL_MAX_BUFFER_ELEMENTS 8192
#endif

#ifndef RGL_PROJECTION 
#define RGL_MODELVIEW                            0x1700      /* GL_MODELVIEW */
#define RGL_PROJECTION                           0x1701      /* GL_PROJECTION */
#endif

#ifndef RGL_QUADS
#define RGL_POINTS                               0x0000
#define RGL_LINES                                0x0001      /* GL_LINES */
#define RGL_LINE_LOOP                            0x0002
#define RGL_LINE_STRIP                           0x0003
#define RGL_TRIANGLES                            0x0004      /* GL_TRIANGLES */  
#define RGL_TRIANGLE_STRIP                       0x0005
#define RGL_TRIANGLE_FAN                         0x0006      /* GL_TRIANGLE_FAN  */  
#define GL_QUADS                                 0x0007

/* these ensure GL_DEPTH_TEST is disabled when they're being rendered */
#define RGL_POINTS_2D                               0x0010
#define RGL_LINES_2D                                0x0011      /* GL_LINES */
#define RGL_LINE_LOOP_2D                            0x0012
#define RGL_LINE_STRIP_2D                           0x0013
#define RGL_TRIANGLES_2D                            0x0014      /* GL_TRIANGLES */  
#define RGL_TRIANGLE_STRIP_2D                       0x0015
#define RGL_TRIANGLE_FAN_2D                         0x0016      /* GL_TRIANGLE_FAN  */ 
#define RGL_QUADS                                 0x0007
#endif

#ifndef RGL_PERSPECTIVE_CORRECTION_HINT
#define RGL_PERSPECTIVE_CORRECTION_HINT		0x0C50
#define RGL_POINT_SMOOTH_HINT			0x0C51
#define RGL_LINE_SMOOTH_HINT			0x0C52
#define RGL_POLYGON_SMOOTH_HINT			0x0C53
#define RGL_FOG_HINT				0x0C54
#define RGL_DONT_CARE				0x1100
#define RGL_FASTEST				0x1101
#define RGL_NICEST				0x1102

#define RGL_DEPTH_TEST               0x0B71
#define RGL_LEQUAL                   0x0203
#define RGL_ONE_MINUS_SRC_ALPHA      0x0303
#endif

#ifndef RGL_BLEND
#define RGL_BLEND                    0x0BE2
#define RGL_BACK                     0x0405
#define RGL_CCW                      0x0901
#define RGL_CULL_FACE                0x0B44
#define RGL_COLOR_BUFFER_BIT         0x00004000
#define RGL_DEPTH_BUFFER_BIT         0x00000100
#define RGL_SRC_ALPHA                0x0302
#endif

#ifndef GL_RG
#define GL_RG 0x8227
#endif

#ifndef GL_SMOOTH
#define GL_LIGHTING				0x0B50
#define GL_LIGHT0				0x4000
#define GL_LIGHT1				0x4001
#define GL_LIGHT2				0x4002
#define GL_LIGHT3				0x4003
#define GL_LIGHT4				0x4004
#define GL_LIGHT5				0x4005
#define GL_LIGHT6				0x4006
#define GL_LIGHT7				0x4007
#define GL_SPOT_EXPONENT			0x1205
#define GL_SPOT_CUTOFF				0x1206
#define GL_CONSTANT_ATTENUATION			0x1207
#define GL_LINEAR_ATTENUATION			0x1208
#define GL_QUADRATIC_ATTENUATION		0x1209
#define GL_AMBIENT				0x1200
#define GL_DIFFUSE				0x1201
#define GL_SPECULAR				0x1202
#define GL_SHININESS				0x1601
#define GL_EMISSION				0x1600
#define GL_POSITION				0x1203
#define GL_SPOT_DIRECTION			0x1204
#define GL_AMBIENT_AND_DIFFUSE			0x1602
#define GL_COLOR_INDEXES			0x1603
#define GL_LIGHT_MODEL_TWO_SIDE			0x0B52
#define GL_LIGHT_MODEL_LOCAL_VIEWER		0x0B51
#define GL_LIGHT_MODEL_AMBIENT			0x0B53
#define GL_FRONT_AND_BACK			0x0408
#define GL_SHADE_MODEL				0x0B54
#define GL_FLAT					0x1D00
#define GL_SMOOTH				0x1D01
#define GL_COLOR_MATERIAL			0x0B57
#define GL_COLOR_MATERIAL_FACE			0x0B55
#define GL_COLOR_MATERIAL_PARAMETER		0x0B56
#define GL_NORMALIZE				0x0BA1
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
    float lineWidth;
} RGL_BATCH;

#if defined(__cplusplus)
extern "C" {            /* Prevents name mangling of functions */
#endif

RGLDEF void rglInit(void* loader);             /* Initialize RGLinfo (buffers, shaders, textures, states) */
RGLDEF void rglClose(void);                             /* De-initialize RGLinfo (buffers, shaders, textures) */

RGLDEF void rglRenderBatch(void);                         /* Draw render batch data (Update->Draw->Reset) */
RGLDEF void rglRenderBatchWithShader(u32 program, u32 vertexLocation, u32 texCoordLocation, u32 colorLocation);

RGLDEF void rglSetTexture(u32 id);               /* Set current texture for render batch and check buffers limits */
RGLDEF u32 rglCreateTexture(u8* bitmap, u32 width, u32 height, u8 channels); /* create texture */
RGLDEF void rglUpdateTexture(u32 texture, u8* bitmap, u32 width, u32 height, u8 channels); /* update texture */
RGLDEF void rglDeleteTextures(GLsizei n, const GLuint * textures);

RGLDEF void rglPushPixelValues(GLint alignment, GLint rowLength, GLint skipPixels, GLint skipRows);
RGLDEF void rglTextureSwizzleMask(u32 atlas, u32 param, i32 swizzleRgbaParams[4]);

RGLDEF void rglAtlasAddBitmap(u32 atlas, u8* bitmap, float x, float y, float width, float height);

/* RGL implementation of gluPerspective */
RGLDEF void rglPerspective(double fovY, double aspect, double zNear, double zFar); /* set up a perspective projection matrix */
/* Scale */
RGLDEF RGL_MATRIX rglMatrixScale(float x, float y, float z);

/* render with legacy (or turn of legacy rendering if you turned it on) */
RGLDEF void rglLegacy(u8 state);

#ifndef RGL_NO_STD
/* get opengl error */
RGLDEF void rglGetError(void);
#endif

RGLDEF void rglBegin(int mode);

RGLDEF void rglLineWidth(float width);
RGLDEF void rglViewport(GLint x, GLint y, GLsizei width, GLsizei height);
RGLDEF void rglClear(GLbitfield mask);
RGLDEF void rglClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
RGLDEF void rglDepthFunc(GLenum func);
RGLDEF void rglCullFace(GLenum mode);
RGLDEF void rglFrontFace(GLenum mode);
#ifdef RGL_OPENGL_43
RGLDEF void rglClearDepth(float depth);
RGLDEF void rglBlendFunc(GLenum sfactor, GLenum dfactor)
#endif

RGLDEF void rglHint(GLenum target, GLenum mode);
RGLDEF void rglEnable(GLenum cap);

#if defined(RGL_OPENGL_LEGACY)
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
#else

RGLDEF void rglEnd(void);
RGLDEF void rglTexCoord2f(float x, float y); 

RGLDEF void rglColor3ub(u8 r, u8 g, u8 b);
RGLDEF void rglColor4ub(u8 r, u8 g, u8 b, u8 a);

RGLDEF void rglColor3f(float r, float g, float b);
RGLDEF void rglColor4f(float r, float g, float b, float a);

RGLDEF void rglVertex2f(float x, float y);
RGLDEF void rglVertex3f(float x, float y, float z);

RGLDEF void rglMatrixMode(int mode);
RGLDEF void rglPushMatrix(void);
RGLDEF void rglPopMatrix(void);
RGLDEF void rglLoadIdentity(void);
RGLDEF void rglTranslatef(float x, float y, float z);
RGLDEF void rglRotatef(float angle, float x, float y, float z);
RGLDEF void rglMultMatrixf(const float *matf);
RGLDEF void rglOrtho(double left, double right, double bottom, double top, double znear, double zfar);

RGLDEF RGL_MATRIX rglMatrixIdentity(void);                       /* Get identity matrix */
RGLDEF RGL_MATRIX rglMatrixMultiply(float left[16], float right[16]);  /* Multiply two matrices */

RGLDEF i32 rglCheckRenderBatchLimit(int vCount);                             /* Check internal buffer overflow for a given number of vertex */

#ifndef RGL_NO_GL_LOADER
#define RGL_PROC_DEF(proc, name) name##SRC = (name##PROC)proc(#name)

typedef void (*RGLapiproc)(void);
typedef RGLapiproc (*RGLloadfunc)(const char *name);

typedef void (*glShaderSourcePROC) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef GLuint (*glCreateShaderPROC) (GLenum type);
typedef void (*glCompileShaderPROC) (GLuint shader);
typedef GLuint (*glCreateProgramPROC) (void);
typedef void (*glAttachShaderPROC) (GLuint program, GLuint shader);
typedef void (*glBindAttribLocationPROC) (GLuint program, GLuint index, const GLchar *name);
typedef void (*glLinkProgramPROC) (GLuint program);
typedef void (*glBindBufferPROC) (GLenum target, GLuint buffer);
typedef void (*glBufferDataPROC) (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void (*glEnableVertexAttribArrayPROC) (GLuint index);
typedef void (*glVertexAttribPointerPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void (*glDisableVertexAttribArrayPROC) (GLuint index);
typedef void (*glDeleteBuffersPROC) (GLsizei n, const GLuint *buffers);
typedef void (*glDeleteVertexArraysPROC) (GLsizei n, const GLuint *arrays);
typedef void (*glUseProgramPROC) (GLuint program);
typedef void (*glDetachShaderPROC) (GLuint program, GLuint shader);
typedef void (*glDeleteShaderPROC) (GLuint shader);
typedef void (*glDeleteProgramPROC) (GLuint program);
typedef void (*glBufferSubDataPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
typedef void (*glGetShaderivPROC)(GLuint shader, GLenum pname, GLint *params);
typedef void (*glGetShaderInfoLogPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (*glGetProgramivPROC)(GLuint program, GLenum pname, GLint *params);
typedef void (*glGetProgramInfoLogPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (*glGenVertexArraysPROC)(GLsizei n, GLuint *arrays);
typedef void (*glGenBuffersPROC)(GLsizei n, GLuint *buffers);
typedef void (*glBindVertexArrayPROC)(GLuint array);
typedef GLint (*glGetUniformLocationPROC)(GLuint program, const GLchar *name);
typedef void (*glUniformMatrix4fvPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*glTexImage2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (*glActiveTexturePROC) (GLenum texture);

glShaderSourcePROC glShaderSourceSRC = NULL;
glCreateShaderPROC glCreateShaderSRC = NULL;
glCompileShaderPROC glCompileShaderSRC = NULL;
glCreateProgramPROC glCreateProgramSRC = NULL;
glAttachShaderPROC glAttachShaderSRC = NULL;
glBindAttribLocationPROC glBindAttribLocationSRC = NULL;
glLinkProgramPROC glLinkProgramSRC = NULL;
glBindBufferPROC glBindBufferSRC = NULL;
glBufferDataPROC glBufferDataSRC = NULL;
glEnableVertexAttribArrayPROC glEnableVertexAttribArraySRC = NULL;
glVertexAttribPointerPROC glVertexAttribPointerSRC = NULL;
glDisableVertexAttribArrayPROC glDisableVertexAttribArraySRC = NULL;
glDeleteBuffersPROC glDeleteBuffersSRC = NULL;
glUseProgramPROC glUseProgramSRC = NULL;
glDetachShaderPROC glDetachShaderSRC = NULL;
glDeleteShaderPROC glDeleteShaderSRC = NULL;
glDeleteProgramPROC glDeleteProgramSRC = NULL;
glBufferSubDataPROC glBufferSubDataSRC = NULL;
glGetShaderivPROC glGetShaderivSRC = NULL;
glGetShaderInfoLogPROC glGetShaderInfoLogSRC = NULL;
glGetProgramivPROC glGetProgramivSRC = NULL;
glGetProgramInfoLogPROC glGetProgramInfoLogSRC = NULL;
glGenBuffersPROC glGenBuffersSRC = NULL;
glGetUniformLocationPROC glGetUniformLocationSRC = NULL;
glUniformMatrix4fvPROC glUniformMatrix4fvSRC = NULL;
glActiveTexturePROC glActiveTextureSRC = NULL;

#if defined(RGL_OPENGL_ES2) && !defined(RGL_OPENGL_ES3)
typedef void (* PFNGLGENVERTEXARRAYSOESPROC) (GLsizei n, GLuint *arrays);
typedef void (* PFNGLBINDVERTEXARRAYOESPROC) (GLuint array);
typedef void (* PFNGLDELETEVERTEXARRAYSOESPROC) (GLsizei n, const GLuint *arrays);

static PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysSRC = NULL;
static PFNGLBINDVERTEXARRAYOESPROC glBindVertexArraySRC = NULL;
static PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysSRC = NULL;
#else
glGenVertexArraysPROC glGenVertexArraysSRC = NULL;
glBindVertexArrayPROC glBindVertexArraySRC = NULL;
glDeleteVertexArraysPROC glDeleteVertexArraysSRC = NULL;
#endif

#define glActiveTexture glActiveTextureSRC
#define glShaderSource glShaderSourceSRC
#define glCreateShader glCreateShaderSRC
#define glCompileShader glCompileShaderSRC
#define glCreateProgram glCreateProgramSRC
#define glAttachShader glAttachShaderSRC
#define glBindAttribLocation glBindAttribLocationSRC
#define glLinkProgram glLinkProgramSRC
#define glBindBuffer glBindBufferSRC
#define glBufferData glBufferDataSRC
#define glEnableVertexAttribArray glEnableVertexAttribArraySRC
#define glVertexAttribPointer glVertexAttribPointerSRC
#define glDisableVertexAttribArray glDisableVertexAttribArraySRC
#define glDeleteBuffers glDeleteBuffersSRC
#define glDeleteVertexArrays glDeleteVertexArraysSRC
#define glUseProgram glUseProgramSRC
#define glDetachShader glDetachShaderSRC
#define glDeleteShader glDeleteShaderSRC
#define glDeleteProgram glDeleteProgramSRC
#define glBufferSubData glBufferSubDataSRC
#define glGetShaderiv glGetShaderivSRC
#define glGetShaderInfoLog glGetShaderInfoLogSRC
#define glGetProgramiv glGetProgramivSRC
#define glGetProgramInfoLog glGetProgramInfoLogSRC
#define glGenVertexArrays glGenVertexArraysSRC
#define glGenBuffers glGenBuffersSRC
#define glBindVertexArray glBindVertexArraySRC
#define glGetUniformLocation glGetUniformLocationSRC
#define glUniformMatrix4fv glUniformMatrix4fvSRC

extern int RGL_loadGLModern(RGLloadfunc proc);
#endif

#endif
#if defined(__cplusplus)
}
#endif

#endif /* RGL_H */

#ifdef RGL_IMPLEMENTATION
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

#ifndef RGL_OPENGL_LEGACY
typedef struct RGL_INFO {
	RGL_MATRIX transform; /* transformation matrix*/
    #ifdef RGL_ALLOC_MATRIX_STACK 
    RGL_MATRIX* stack
    #else
    RGL_MATRIX stack[RGL_MAX_MATRIX_STACK_SIZE]; /* RGL_MATRIX stack for push/pop */
    #endif

    RGL_MATRIX* matrix;              /* Current matrix pointer */
    RGL_MATRIX modelview;                   /* Default modelview matrix */
    RGL_MATRIX projection;                  /* Default projection matrix */
    RGL_BATCH* batches;          /* Draw calls array, depends on tex */

    u16* indices;
    
    float* vertices;
	float* colors;
    float* tcoords;

    i32 vertexCounter;                  /* Current active render batch vertex counter (generic, used for all batches) */
    
    i32 matrixMode;              /* Current matrix mode */

	u8 transformRequired;

    i32 stackCounter;                   /* RGL_MATRIX stack counter */

    u32 tex;      /* Default texture used on shapes/poly drawing (required by shader)*/

    #ifdef RGL_MODERN_OPENGL
    u32 vShader;      /* Default vertex shader id (used by default shader program)*/
    u32 fShader;      /* Default fragment shader id (used by default shader program)*/
    u32 mvp;
    #endif

    u32 program;       /* Default shader program id, supports vertex color and diffuse texture*/
    u32 defaultTex;
    u32 elementCount;

    i32 bufferCount;            /* Number of vertex buffers (multi-buffering support) */
    i32 currentBuffer;          /* Current buffer tracking in case of multi-buffering */
    i32 drawCounter;            /* Draw calls counter */

    float lineWidth;    /* Default lineWidth used on shapes/poly drawing (required by shader)*/
    float tcoord[3];
    float color[4];

    u32 vao, vbo, tbo, cbo, ebo; /* array object and array buffers */
    u8 legacy;
} RGL_INFO;

RGL_INFO RGLinfo;
#endif /* RGL_MODERN_OPENGL */

void rglLineWidth(float width) {
    #ifndef RGL_OPENGL_LEGACY
    if (RGLinfo.legacy) 
    #endif
    {
        glLineWidth(width);
        return;
    }
#if defined(RGL_MODERN_OPENGL)
    if (RGLinfo.lineWidth == width)
        return;

    RGLinfo.lineWidth = width;

    RGLinfo.vertexCounter += RGLinfo.batches[RGLinfo.drawCounter - 1].vertexAlignment;
    RGLinfo.drawCounter++;
    RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount = 0;
#endif    
}

void rglViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    #if !defined(RGL_MODERN_OPENGL) && !defined(RGL_OPENGL_LEGACY)
    if (RGLinfo.legacy)
    #endif
        glViewport(x, y, width, height);

    #ifdef RGL_DIRECTX
    D3D11_VIEWPORT viewport;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = win->r.w;
    viewport.Height = win->r.h;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    dxInfo.pDeviceContext->lpVtbl->RSSetViewports(RGL_dxInfo.pDeviceContext, 1, &viewport);
    #endif
}

void rglClear(GLbitfield mask) {
    #if !defined(RGL_MODERN_OPENGL) && !defined(RGL_OPENGL_LEGACY)
    if (RGLinfo.legacy)
    #endif
        glClear(mask);
}

void rglClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    #if !defined(RGL_MODERN_OPENGL) && !defined(RGL_OPENGL_LEGACY)
    if (RGLinfo.legacy)
    #endif
        glClearColor(red, green, blue, alpha);
}

void rglDeleteTextures(GLsizei n, const GLuint * textures) {
    #if !defined(RGL_MODERN_OPENGL) && !defined(RGL_OPENGL_LEGACY)
    if (RGLinfo.legacy)
    #endif
        glDeleteTextures(n, textures);
}

void rglPushPixelValues(i32 alignment, i32 rowLength, i32 skipPixels, i32 skipRows) {
    #if !defined(RGL_MODERN_OPENGL) && !defined(RGL_OPENGL_LEGACY)
    if (RGLinfo.legacy == 0) 
        return;
    #endif
	
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, rowLength);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, skipPixels);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, skipRows);
}


void rglTextureSwizzleMask(u32 atlas, u32 param, i32 swizzleRgbaParams[4]) {
    #if !defined(RGL_MODERN_OPENGL) && !defined(RGL_OPENGL_LEGACY)
    if (RGLinfo.legacy == 0) 
        return;
    #endif

    glBindTexture(GL_TEXTURE_2D, atlas);
    glTexParameteriv(GL_TEXTURE_2D, param, swizzleRgbaParams);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void rglAtlasAddBitmap(u32 atlas, u8* bitmap, float x, float y, float width, float height) {
    #if !defined(RGL_MODERN_OPENGL) && !defined(RGL_OPENGL_LEGACY)
    if (RGLinfo.legacy == 0) 
        return;
    #endif

    rglEnable(GL_TEXTURE_2D);

    i32 alignment, rowLength, skipPixels, skipRows;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
    glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowLength);
    glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skipPixels);
    glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skipRows);

    glBindTexture(GL_TEXTURE_2D, atlas);

    rglPushPixelValues(1, width, 0, 0);

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RED, GL_UNSIGNED_BYTE, bitmap);

    rglPushPixelValues(alignment, rowLength, skipPixels, skipRows);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void rglDepthFunc(GLenum func) {
    #if !defined(RGL_MODERN_OPENGL) && !defined(RGL_OPENGL_LEGACY)
    if (RGLinfo.legacy) 
    #endif

    return glDepthFunc(func);
}

void rglCullFace(GLenum mode) {
    #if !defined(RGL_MODERN_OPENGL) && !defined(RGL_OPENGL_LEGACY)
    if (RGLinfo.legacy) 
    #endif
        glCullFace(mode);
}

void rglFrontFace(GLenum mode) {
    #if !defined(RGL_MODERN_OPENGL) && !defined(RGL_OPENGL_LEGACY)
    if (RGLinfo.legacy) 
    #endif
        glFrontFace(mode);
}

#ifdef RGL_OPENGL_43
void rglClearDepth(float depth) {
    #if !defined(RGL_MODERN_OPENGL) && !defined(RGL_OPENGL_LEGACY)
    if (RGLinfo.legacy) 
    #endif
        glClearDepth(depth);
}

void rglBlendFunc(GLenum sfactor, GLenum dfactor) {
    #if !defined(RGL_MODERN_OPENGL) && !defined(RGL_OPENGL_LEGACY)
    if (RGLinfo.legacy) 
    #endif
        glBlendFunc(sfactor, dfactor);
}
#endif

void rglHint(GLenum target, GLenum mode) {
    #if !defined(RGL_MODERN_OPENGL) && !defined(RGL_OPENGL_LEGACY)
    if (RGLinfo.legacy) 
    #endif
        glHint(target, mode);
}

void rglEnable(GLenum cap) {
    #if !defined(RGL_MODERN_OPENGL) && !defined(RGL_OPENGL_LEGACY)
    if (RGLinfo.legacy) 
    #endif
        glEnable(cap);
}

void rglSetTexture(u32 id) {
    #ifndef RGL_OPENGL_LEGACY
    if (RGLinfo.legacy) 
    #endif
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, id);
        return;
    }
#ifndef RGL_OPENGL_LEGACY
    if (RGLinfo.tex == id)
        return;

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

    #if defined(RGL_MODERN_OPENGL) || defined(RGL_OPENGL_LEGACY)
        glBindTexture(GL_TEXTURE_2D, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
        
        unsigned int c = 0;

        switch (channels) {
            case 1: c = GL_RED; break;
            case 2: c = GL_RG; break;
            case 3: c = GL_RGB; break;
            case 4: c = GL_RGBA; break;
            default: break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, c, width, height, 0, c, GL_UNSIGNED_BYTE, bitmap);

        glBindTexture(GL_TEXTURE_2D, 0);
    #endif

    return id;
}

void rglUpdateTexture(u32 texture, u8* bitmap, u32 width, u32 height, u8 channels) {
    #if defined(RGL_MODERN_OPENGL) || defined(RGL_OPENGL_LEGACY)
        glBindTexture(GL_TEXTURE_2D, texture);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, width);

        u16 c = 0;
        switch (channels) {
            case 1: c = GL_RED; break;
            case 2: c = GL_RG; break;
            case 3: c = GL_RGB; break;
            case 4: c = GL_RGBA; break;
            default: break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, c, width,height, 0, c, GL_UNSIGNED_BYTE, bitmap);
        glBindTexture(GL_TEXTURE_2D, 0);
    #endif
}

#ifdef RGL_DEBUG
#include <stdio.h>

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

#ifdef RGL_MODERN_OPENGL

#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS 0x8B82
#define GL_COMPILE_STATUS 0x8B81
#define GL_INFO_LOG_LENGTH 0x8B84
#endif

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
#endif
#endif /* RGL_DEBUG */

#define RGL_MULTILINE_STR(...) #__VA_ARGS__

#ifdef RGL_OPENGL_LEGACY
void rglBegin(int mode) {
    if (mode > 0x0010)
        return glBegin(mode - 0x0010);
    return glBegin(mode);
}
#endif

#ifdef RGL_DIRECTX
typedef struct {
	IDXGIFactory* pFactory;
	IDXGIAdapter* pAdapter;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
} RGFW_directXinfo;

RGFW_directXinfo RGL_dxInfo;
#endif

/* Initialize RGLinfo: OpenGL extensions, default buffers/shaders/textures, OpenGL states*/
void rglInit(void *loader) {
#ifndef RGL_OPENGL_LEGACY
    #if !defined(RGL_NO_GL_LOADER) && defined(RGL_MODERN_OPENGL)
    if (RGL_loadGLModern((RGLloadfunc)loader)) {
        #ifdef RGL_DEBUG
        printf("Failed to load an OpenGL 3.3 Context, reverting to OpenGL Legacy\n");
        #endif

        RGLinfo.legacy = 2;   
        return;
    }
    #endif

    #ifdef RGL_DIRECTX
    RGL_dxInfo = (RGFW_directXinfo)loader;
    #endif

    RGLinfo.legacy = 0; 

    #ifdef RGL_MODERN_OPENGL
    
    static const char *defaultVShaderCode = RGL_MULTILINE_STR(
    #if defined(RGL_OPENGL_21)
        \x23version 120                       \n
        attribute vec3 vertexPosition;     \n
        attribute vec2 vertexTexCoord;     \n
        attribute vec4 vertexColor;        \n
        varying vec2 fragTexCoord;         \n
        varying vec4 fragColor;            \n
    #elif defined(RGL_OPENGL_33)
        \x23version 330                     \n
        in vec3 vertexPosition;            \n
        in vec2 vertexTexCoord;            \n
        in vec4 vertexColor;               \n
        out vec2 fragTexCoord;             \n
        out vec4 fragColor;                \n
    #endif
    #if defined(RGL_OPENGL_ES2)
        \x23version 100                     \n
        precision mediump float;           \n
        attribute vec3 vertexPosition;     \n
        attribute vec2 vertexTexCoord;     \n
        attribute vec4 vertexColor;        \n
        varying vec2 fragTexCoord;         \n
        varying vec4 fragColor;            \n
    #endif
        uniform mat4 mvp;
        void main() {
            fragTexCoord = vertexTexCoord;
            fragColor = vertexColor;
            gl_Position = mvp*vec4(vertexPosition, 1.0);
        }
    );

    static const char* defaultFShaderCode = RGL_MULTILINE_STR(
#if defined(RGL_OPENGL_21)
    \x23version 120 \n
    varying vec2 fragTexCoord;
    varying vec4 fragColor;
#elif defined(RGL_OPENGL_33)
    \x23version 330       \n
    in vec2 fragTexCoord;
    in vec4 fragColor;
    out vec4 finalColor;
#endif
#if defined(RGL_OPENGL_ES2)
        \x23version 100                    \n
        precision mediump float;           \n
        varying vec2 fragTexCoord;         \n
        varying vec4 fragColor;            \n
#endif
        uniform sampler2D texture0;        
        void main() { 
            #ifdef RGL_OPENGL_33
            finalColor = texture(texture0, fragTexCoord) * fragColor;
            #else
            gl_FragColor = texture2D(texture0, fragTexCoord) * fragColor;
            #endif
        }                               
    );

    #if !defined(RGL_OPENGL_21) && !defined(RGL_OPENGL_ES2)
	glGenVertexArrays(1, &RGLinfo.vao);
	glBindVertexArray(RGLinfo.vao);
    #endif

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

    glBindAttribLocation(RGLinfo.program, 0, "vertexPosition");
    glBindAttribLocation(RGLinfo.program, 1, "vertexTexCoord");
    glBindAttribLocation(RGLinfo.program, 2, "vertexColor");

	glLinkProgram(RGLinfo.program);

    /* Set default shader locations: uniform locations */
    RGLinfo.mvp  = glGetUniformLocation(RGLinfo.program, "mvp");

    #endif

    #ifdef RGL_DIRECTX
    const char* shaderString = MULTILINE_STR(
        typedef struct {float x, y, z;} Vertex;

        struct VOut
        {
            float4 position : SV_POSITION;
        };

        VOut VS(float4 position : POSITION)
        {
            VOut output;

            output.position = position;

            return output;
        }


        float4 PS(float4 position : SV_POSITION) : SV_TARGET
        {
            return float4(1.0, 0.0, 0.0, 1.0);
        }
    );

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(Vertex) * 3;
    bd.BindFlags = D3D11 _BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    RGL_dxInfo.pDevice->lpVtbl->CreateBuffer(RGL_dxInfo.pDevice, &bd, NULL, &pVertexBuffer);

    D3D11_MAPPED_SUBRESOURCE ms;
    RGL_dxInfo.pDeviceContext->lpVtbl->Map(RGL_dxInfo.pDeviceContext, (ID3D11Resource*)pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
    memcpy(ms.pData, vertices, sizeof(vertices));
    RGL_dxInfo.pDeviceContext->lpVtbl->Unmap(RGL_dxInfo.pDeviceContext, (ID3D11Resource*)pVertexBuffer, 0);   

    // Compile shaders
    ID3D10Blob* pVertexShaderBlob = NULL;
    ID3D10Blob* pPixelShaderBlob = NULL;
    ID3D10Blob* pErrorBlob = NULL;

    D3DCompile(shaderString, strlen(shaderString), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &pVertexShaderBlob, &pErrorBlob);
    D3DCompile(shaderString, strlen(shaderString), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &pPixelShaderBlob, &pErrorBlob);

    // Create shaders
    ID3D11VertexShader* pVertexShader;
    ID3D11PixelShader* pPixelShader;
    RGL_dxInfo.pDevice->lpVtbl->CreateVertexShader(RGL_dxInfo.pDevice, pVertexShaderBlob->lpVtbl->GetBufferPointer(pVertexShaderBlob), pVertexShaderBlob->lpVtbl->GetBufferSize(pVertexShaderBlob), NULL, &pVertexShader);
    RGL_dxInfo.pDevice->lpVtbl->CreatePixelShader(RGL_dxInfo.pDevice, pPixelShaderBlob->lpVtbl->GetBufferPointer(pPixelShaderBlob), pPixelShaderBlob->lpVtbl->GetBufferSize(pPixelShaderBlob), NULL, &pPixelShader);
    
    RGL_dxInfo.pDeviceContext->lpVtbl->VSSetShader(RGL_dxInfo.pDeviceContext, pVertexShader, 0, 0);
    RGL_dxInfo.pDeviceContext->lpVtbl->PSSetShader(RGL_dxInfo.pDeviceContext, pPixelShader, 0, 0);

    // Set input layout
    ID3D11InputLayout* pInputLayout;
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    
    RGL_dxInfo.pDevice->lpVtbl->CreateInputLayout(RGL_dxInfo.pDevice, layout, 1, pVertexShaderBlob->lpVtbl->GetBufferPointer(pVertexShaderBlob), pVertexShaderBlob->lpVtbl->GetBufferSize(pVertexShaderBlob), &pInputLayout);
    RGL_dxInfo.pDeviceContext->lpVtbl->IASetInputLayout(RGL_dxInfo.pDeviceContext, pInputLayout);
    #endif

    /* Init default vertex arrays buffers */
    /* Initialize CPU (RAM) vertex buffers (position, texcoord, color data and indexes) */

    RGLinfo.elementCount = RGL_MAX_BUFFER_ELEMENTS;

    RGLinfo.vertices = (float*)RGL_MALLOC(RGL_MAX_BUFFER_ELEMENTS * 3 * 4 * sizeof(float) * RGL_MAX_BATCHES);
    RGLinfo.tcoords = (float*)RGL_MALLOC(RGL_MAX_BUFFER_ELEMENTS * 2 * 4 * sizeof(float) * RGL_MAX_BATCHES);
    RGLinfo.colors = (float*)RGL_MALLOC(RGL_MAX_BUFFER_ELEMENTS * 4 * 4 * sizeof(float) * RGL_MAX_BATCHES);
    RGLinfo.indices = (u16*)RGL_MALLOC(RGL_MAX_BUFFER_ELEMENTS * 6 * sizeof(u16) * RGL_MAX_BATCHES);

    i32 k = 0, j;

    /* Indices can be initialized right now */
    for (j = 0; j < (6 * RGL_MAX_BUFFER_ELEMENTS); j += 6) {
		RGLinfo.indices[j + 0] = k + 0;
		RGLinfo.indices[j + 1] = k + 1;
		RGLinfo.indices[j + 2] = k + 2;

		RGLinfo.indices[j + 3] = k + 0;
		RGLinfo.indices[j + 4] = k + 2;
		RGLinfo.indices[j + 5] = k + 3;

		k += 4;
    }

    RGLinfo.vertexCounter = 0;

    #ifdef RGL_MODERN_OPENGL

    #if !defined(RGL_OPENGL_21) && !defined(RGL_OPENGL_ES2)
    glBindVertexArray(RGLinfo.vao);
    #endif

    #ifdef RGL_DEBUG
    rglGetError();
    #endif

    /* Quads - Vertex buffers binding and attributes enable */
    /* Vertex position buffer (shader-location = 0) */
    glBindBuffer(GL_ARRAY_BUFFER, RGLinfo.vbo);
    glBufferData(GL_ARRAY_BUFFER, RGL_MAX_BUFFER_ELEMENTS * 3 * 4 * sizeof(float), RGLinfo.vertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);

    /* Vertex texcoord buffer (shader-location = 1) */
    glBindBuffer(GL_ARRAY_BUFFER, RGLinfo.tbo);
    glBufferData(GL_ARRAY_BUFFER, RGL_MAX_BUFFER_ELEMENTS * 2 * 4 * sizeof(float), RGLinfo.tcoords, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);

    /* Vertex color buffer (shader-location = 3) */
    glBindBuffer(GL_ARRAY_BUFFER, RGLinfo.cbo);
    glBufferData(GL_ARRAY_BUFFER, RGL_MAX_BUFFER_ELEMENTS * 4 * 4 * sizeof(float), RGLinfo.colors, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, 0, 0);

    /* Fill index buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RGLinfo.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, RGL_MAX_BUFFER_ELEMENTS * 6 * sizeof(u16), RGLinfo.indices, GL_STATIC_DRAW);
    #endif

    RGL_FREE(RGLinfo.indices);

    #if !defined(RGL_OPENGL_21) && !defined(RGL_OPENGL_ES2)
    
    #ifdef RGL_MODERN_OPENGL
    /* Unbind the current VAO */
    if (RGLinfo.vao) 
        glBindVertexArray(0);
    #endif

    #ifdef RGL_DEBUG
    rglGetError();
    #endif

    #endif

    /* load default texture */
    u8 white[4] = {255, 255, 255, 255};
    RGLinfo.defaultTex = rglCreateTexture(white, 1, 1, 4);
    RGLinfo.tex = RGLinfo.defaultTex;
    RGLinfo.lineWidth = 1;

    RGLinfo.batches = (RGL_BATCH *)RGL_MALLOC(RGL_MAX_BATCHES * sizeof(RGL_BATCH));
    
    #ifdef RGL_ALLOC_MATRIX_STACK
    RGLinfo.statck = (RGL_MATRIX*)RGL_MALLOC(RGL_MAX_MATRIX_STACK_SIZE * sizeof(RGL_MATRIX));
    #endif

    u32 i;
    for (i = 0; i < RGL_MAX_BATCHES; i++) {
        RGLinfo.batches[i].mode = 0;
        RGLinfo.batches[i].vertexCount = 0;
        RGLinfo.batches[i].vertexAlignment = 0;
        RGLinfo.batches[i].tex = RGLinfo.tex;
        RGLinfo.batches[i].lineWidth = RGLinfo.lineWidth;
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
    
    #ifdef RGL_DEBUG
    rglGetError();
    #endif
#endif
}

/* Vertex Buffer Object deinitialization (memory free) */
void rglClose(void) {
#ifndef RGL_OPENGL_LEGACY
    #if defined(RGL_MODERN_OPENGL)
        if (RGLinfo.legacy)
            return;
        
        /* Unbind everything */
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        /* Unload all vertex buffers data */
        #if !defined(RGL_OPENGL_21) && !defined(RGL_OPENGL_ES2)
        glBindVertexArray(RGLinfo.vao);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glBindVertexArray(0);
        #endif

        /* Delete VBOs from GPU (VRAM) */
        glDeleteBuffers(1, &RGLinfo.vbo);
        glDeleteBuffers(1, &RGLinfo.tbo);
        glDeleteBuffers(1, &RGLinfo.cbo);
        glDeleteBuffers(1, &RGLinfo.ebo);

        #if !defined(RGL_OPENGL_21) && !defined(RGL_OPENGL_ES2)
        glDeleteVertexArrays(1, &RGLinfo.vao);
        #endif
    #endif

    /* Free vertex arrays memory from CPU (RAM) */
    RGL_FREE(RGLinfo.vertices);
    RGL_FREE(RGLinfo.tcoords);
    RGL_FREE(RGLinfo.colors);

    /* Unload arrays */
    #ifdef RGL_ALLOC_BATCHES
    RGL_FREE(RGLinfo.batches);
    #endif

    #ifdef RGL_ALLOC_MATRIX_STACK
    RGL_FREE(RGLinfo.stack);
    #endif
#endif

#ifdef RGL_MODERN_OPENGL
    glUseProgram(0);

    glDetachShader(RGLinfo.program, RGLinfo.vShader);
    glDetachShader(RGLinfo.program, RGLinfo.fShader);
    glDeleteShader(RGLinfo.vShader);
    glDeleteShader(RGLinfo.fShader);

    glDeleteProgram(RGLinfo.program);

    glDeleteTextures(1, &RGLinfo.tex); /* Unload default texture */
#endif
}

void rglRenderBatch() {
    #ifndef RGL_OPENGL_LEGACY
    if (RGLinfo.legacy)
        return;
    
    rglRenderBatchWithShader(RGLinfo.program, 0, 1, 2);
    #endif
}

void rglRenderBatchWithShader(u32 program, u32 vertexLocation, u32 texCoordLocation, u32 colorLocation) {
#ifndef RGL_OPENGL_LEGACY
    if (RGLinfo.legacy)
        return;

#ifdef RGL_MODERN_OPENGL
    if (RGLinfo.vertexCounter > 0) {
        #if !defined(RGL_OPENGL_21) && !defined(RGL_OPENGL_ES2)
        glBindVertexArray(RGLinfo.vao);
        #endif

        /* Vertex positions buffer */
        glBindBuffer(GL_ARRAY_BUFFER, RGLinfo.vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, RGLinfo.vertexCounter * 3 * sizeof(float), RGLinfo.vertices);

        /* Texture coordinates buffer */
        glBindBuffer(GL_ARRAY_BUFFER, RGLinfo.tbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, RGLinfo.vertexCounter * 2 * sizeof(float), RGLinfo.tcoords);
        
        /* Colors buffer */
        glBindBuffer(GL_ARRAY_BUFFER, RGLinfo.cbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, RGLinfo.vertexCounter * 4 * sizeof(float), RGLinfo.colors); 
        
        #if !defined(RGL_OPENGL_21) && !defined(RGL_OPENGL_ES2)
        glBindVertexArray(0);
        #endif

        /* Set current shader and upload current MVP matrix */
        glUseProgram(program);

        RGL_MATRIX matMVP = rglMatrixMultiply(RGLinfo.modelview.m, RGLinfo.projection.m);
        glUniformMatrix4fv(RGLinfo.mvp, 1, 0, matMVP.m);

        #if !defined(RGL_OPENGL_21) && !defined(RGL_OPENGL_ES2)
        glBindVertexArray(RGLinfo.vao);
        #endif

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
        
        #ifdef RGL_DEBUG
        rglGetError();
        #endif

        u32 vertexOffset;
        u32 i;
        for (i = 1, vertexOffset = 0; i < RGLinfo.drawCounter; i++) {
            GLenum mode = RGLinfo.batches[i].mode;
            
            if (mode > 0x0010) {
                mode -= 0x0010;
                glDisable(GL_DEPTH_TEST);
                glDepthMask(GL_FALSE);
            }
            else {
                glEnable(GL_DEPTH_TEST);
                glDepthMask(GL_TRUE);
            }

            /* Bind current draw call texture, activated as GL_TEXTURE0 and Bound to sampler2D texture0 by default */
            glBindTexture(GL_TEXTURE_2D, RGLinfo.batches[i].tex);
            glLineWidth(RGLinfo.batches[i].lineWidth);
            
            #ifdef RGL_EBO
            if (mode != RGL_QUADS && mode != RGL_TRIANGLE_FAN) 
            #endif
                glDrawArrays(mode, vertexOffset, RGLinfo.batches[i].vertexCount);
            #ifdef RGL_EBO
            else {
                glDrawElements(GL_TRIANGLES, (RGLinfo.batches[i].vertexCount) * 2, GL_UNSIGNED_SHORT, (GLvoid *)((vertexOffset) * sizeof(GLushort)));
            }
            #endif

            vertexOffset += (RGLinfo.batches[i].vertexCount + RGLinfo.batches[i].vertexAlignment);

            if (RGLinfo.batches[i].mode > 0x0010) {
                glEnable(GL_DEPTH_TEST);
                glDepthMask(GL_TRUE);
            }

            #ifdef RGL_DEBUG
            rglGetError();
            #endif
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
#endif

    /* Reset vertex counter for next frame */
    RGLinfo.vertexCounter = 0;

    /* Reset RGLinfo.batches array */
    i32 i;
    for (i = 0; i < RGL_MAX_BATCHES; i++) {
        RGLinfo.batches[i].vertexCount = 0;
        RGLinfo.batches[i].tex = RGLinfo.tex;
        RGLinfo.batches[i].lineWidth = RGLinfo.lineWidth;
    }

    /* Reset draws counter to one draw for the batch */
    RGLinfo.drawCounter = 1;

    /* Change to next buffer in the list (in case of multi-buffering) */
    RGLinfo.currentBuffer++;
    if (RGLinfo.currentBuffer >= RGLinfo.bufferCount) 
        RGLinfo.currentBuffer = 0;
#endif
}

void rglPerspective(double fovY, double aspect, double zNear, double zFar) {
    const double f = 1 / tan(fovY / 2.0);
    float projectionMatrix[16] = {0};
    
    projectionMatrix[0] = f / aspect;
    projectionMatrix[5] = f;
    projectionMatrix[10] = (zFar + zNear) / (zNear - zFar);
    projectionMatrix[11] = -1.0;
    projectionMatrix[14] = (2.0 * zFar * zNear) / (zNear - zFar);
    
    rglMultMatrixf(projectionMatrix);
}

RGL_MATRIX rglMatrixScale(float x, float y, float z) {
    RGL_MATRIX result = {{ 
                        x, 0.0f, 0.0f, 0.0f,
                        0.0f, y, 0.0f, 0.0f,
                        0.0f, 0.0f, z, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f 
                      }};

    return result;
}

void rglLegacy(u8 state) {
    #ifndef RGL_OPENGL_LEGACY
    if (state != 2)
        RGLinfo.legacy = state;
    #endif
}

#ifndef RGL_NO_STD

#include <stdio.h>

void rglGetError(void) {
    #if defined(RGL_MODERN_OPENGL) || defined(RGL_OPENGL_LEGACY)
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
            exit(1);
        }
    #endif
}
#endif

#ifndef RGL_OPENGL_LEGACY

int rglCheckRenderBatchLimit(int vCount) {
    if (RGLinfo.legacy || (RGLinfo.vertexCounter + vCount) < (RGLinfo.elementCount * 4))
        return 0;

    /* Store current primitive drawing mode and texture id */
    i32 currentMode = RGLinfo.batches[RGLinfo.drawCounter - 1].mode;
    
    rglRenderBatch();

    /* Restore state of last batch so we can continue adding vertices */
    RGLinfo.batches[RGLinfo.drawCounter - 1].mode = currentMode;
    RGLinfo.batches[RGLinfo.drawCounter - 1].tex = RGLinfo.tex;
    RGLinfo.batches[RGLinfo.drawCounter - 1].lineWidth = RGLinfo.lineWidth;
    return 1; 
}

/* Initialize drawing mode (how to organize vertex) */
void rglBegin(int mode) {
    if (RGLinfo.legacy) {
        if (mode > 0x0010)
            mode -= 0x0010;
        
        return glBegin(mode);
    }

    if (RGLinfo.batches[RGLinfo.drawCounter - 1].mode != mode ||
        RGLinfo.batches[RGLinfo.drawCounter - 1].tex != RGLinfo.tex ||
        RGLinfo.batches[RGLinfo.drawCounter - 1].lineWidth != RGLinfo.lineWidth ||
        RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount > 0) {
            if (RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_LINES || RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_LINES_2D) 
                RGLinfo.batches[RGLinfo.drawCounter - 1].vertexAlignment = ((RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount < 4)? RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount : RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount%4);
            else if (RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_TRIANGLES || RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_TRIANGLES_2D || RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_TRIANGLE_FAN || RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_TRIANGLE_FAN_2D) 
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
            RGLinfo.batches[RGLinfo.drawCounter - 1].lineWidth= RGLinfo.lineWidth;
        }
}

void rglEnd(void) {
    if (RGLinfo.legacy)
        return glEnd();

    if (RGLinfo.drawCounter >= RGL_MAX_BATCHES - 28) {
        rglRenderBatch();
    }
}

/* Define one vertex (texture coordinate) */
void rglTexCoord2f(float x, float y) {
    if (RGLinfo.legacy)
        return glTexCoord2f(x, y);

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
    if (RGLinfo.legacy)
        return glColor4f(r, g, b, a);

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
    if (RGLinfo.legacy)
        return glVertex3f(x, y, z);

    float tx = x;
    float ty = y;
    float tz = z;

    if (RGLinfo.transformRequired){
        tx = RGLinfo.transform.m[0] * x + RGLinfo.transform.m[4] * y + RGLinfo.transform.m[8] * z + RGLinfo.transform.m[12];
        ty = RGLinfo.transform.m[1] * x + RGLinfo.transform.m[5] * y + RGLinfo.transform.m[9] * z + RGLinfo.transform.m[13];
        tz = RGLinfo.transform.m[2] * x + RGLinfo.transform.m[6] * y + RGLinfo.transform.m[10] * z + RGLinfo.transform.m[14];
    }

    if (RGLinfo.vertexCounter > (RGLinfo.elementCount * 4 - 4)) {
        if ((RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_LINES || RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_LINES_2D) &&
            (RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount%2 == 0))
                rglCheckRenderBatchLimit(2 + 1);
        else if ((RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_TRIANGLES || RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_TRIANGLES_2D || RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_TRIANGLE_FAN || RGLinfo.batches[RGLinfo.drawCounter - 1].mode == RGL_TRIANGLE_FAN_2D) &&
            (RGLinfo.batches[RGLinfo.drawCounter - 1].vertexCount%3 == 0))
                rglCheckRenderBatchLimit(3 + 1);
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
    RGLinfo.batches[RGLinfo.drawCounter - 1].lineWidth = RGLinfo.lineWidth;
}


/* Multiply the current matrix by a translation matrix */
void rglTranslatef(float x, float y, float z) {
    if (RGLinfo.legacy)
        return glTranslatef(x, y, z);

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
    if (RGLinfo.legacy) {
        return glRotatef(angle, x, y, z);
    }

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
    if (RGLinfo.legacy) {
        return glOrtho(left, right, bottom, top, znear, zfar);
    }

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
    if (RGLinfo.legacy) {
        return glMatrixMode(mode);
    }

	RGLinfo.matrixMode = mode;

	if (mode == RGL_MODELVIEW)
		RGLinfo.matrix = &RGLinfo.modelview;
	else
		RGLinfo.matrix = &RGLinfo.projection;
}

/* Push the current matrix into RGLinfo.stack */
void rglPushMatrix(void) {
    if (RGLinfo.legacy)
        return glPushMatrix();
    
    RGLinfo.stack[RGLinfo.stackCounter] = *RGLinfo.matrix;

    if (RGLinfo.matrixMode == RGL_MODELVIEW) {
        RGLinfo.transformRequired = 1;
        RGLinfo.matrix = &RGLinfo.transform;
    }

    RGLinfo.stackCounter++;
}

/* Pop lattest inserted matrix from RGLinfo.stack */
void rglPopMatrix(void) {
    if (RGLinfo.legacy)
        return glPopMatrix();

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
    if (RGLinfo.legacy)
        return glLoadIdentity();
    
    *RGLinfo.matrix = rglMatrixIdentity();
}

/* Get identity matrix */
RGL_MATRIX rglMatrixIdentity(void) {
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
    if (RGLinfo.legacy) {
        return glMultMatrixf(m);
    }

	*RGLinfo.matrix = rglMatrixMultiply(RGLinfo.matrix->m, (float*)m);
}

RGL_MATRIX rglMatrixMultiply(float left[16], float right[16]) {
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

#endif /* RGL_OPENGL_LEGACY */

#ifdef RGL_MODERN_OPENGL

#ifndef RGL_NO_GL_LOADER
int RGL_loadGLModern(RGLloadfunc proc) {
    RGL_PROC_DEF(proc, glShaderSource);
    RGL_PROC_DEF(proc, glCreateShader);
    RGL_PROC_DEF(proc, glCompileShader);
    RGL_PROC_DEF(proc, glCreateProgram);
    RGL_PROC_DEF(proc, glAttachShader);
    RGL_PROC_DEF(proc, glBindAttribLocation);
    RGL_PROC_DEF(proc, glLinkProgram);
    RGL_PROC_DEF(proc, glBindBuffer);
    RGL_PROC_DEF(proc, glBufferData);
    RGL_PROC_DEF(proc, glEnableVertexAttribArray);
    RGL_PROC_DEF(proc, glVertexAttribPointer);
    RGL_PROC_DEF(proc, glDisableVertexAttribArray);
    RGL_PROC_DEF(proc, glDeleteBuffers);
    RGL_PROC_DEF(proc, glUseProgram);
    RGL_PROC_DEF(proc, glDetachShader);
    RGL_PROC_DEF(proc, glDeleteShader);
    RGL_PROC_DEF(proc, glDeleteProgram);
    RGL_PROC_DEF(proc, glBufferSubData);
    RGL_PROC_DEF(proc, glGetShaderiv);
    RGL_PROC_DEF(proc, glGetShaderInfoLog);
    RGL_PROC_DEF(proc, glGetProgramiv);
    RGL_PROC_DEF(proc, glGetProgramInfoLog);
    RGL_PROC_DEF(proc, glGenBuffers);
    #if !defined(RGL_OPENGL_21) && !defined(RGL_OPENGL_ES2) && !defined(RGL_OPENGL_ES3)
    RGL_PROC_DEF(proc, glBindVertexArray);
    RGL_PROC_DEF(proc, glGenVertexArrays);
    RGL_PROC_DEF(proc, glDeleteVertexArrays);
    #endif
    RGL_PROC_DEF(proc, glGetUniformLocation);
    RGL_PROC_DEF(proc, glUniformMatrix4fv);
    RGL_PROC_DEF(proc, glActiveTexture);

    #if defined(RGL_OPENGL_ES2) && !defined(RGL_OPENGL_ES3)
        glGenVertexArraysSRC = (PFNGLGENVERTEXARRAYSOESPROC)((RGLloadfunc)loader)("glGenVertexArraysOES");
        glBindVertexArraySRC = (PFNGLBINDVERTEXARRAYOESPROC)((RGLloadfunc)loader)("glBindVertexArrayOES");
        glDeleteVertexArraysSRC = (PFNGLDELETEVERTEXARRAYSOESPROC)((RGLloadfunc)loader)("glDeleteVertexArraysOES");
    #endif

    if (
        glShaderSourceSRC == NULL ||
        glCreateShaderSRC == NULL ||
        glCompileShaderSRC == NULL ||
        glCreateProgramSRC == NULL ||
        glAttachShaderSRC == NULL ||
        glBindAttribLocationSRC == NULL ||
        glLinkProgramSRC == NULL ||
        glBindBufferSRC == NULL ||
        glBufferDataSRC == NULL ||
        glVertexAttribPointerSRC == NULL ||
        glDisableVertexAttribArraySRC == NULL ||
        glDeleteBuffersSRC == NULL ||
        glUseProgramSRC == NULL ||
        glDetachShaderSRC == NULL ||
        glDeleteShaderSRC == NULL ||
        glDeleteProgramSRC == NULL ||
        glBufferSubDataSRC == NULL ||
        glGetShaderivSRC == NULL ||
        glGetShaderInfoLogSRC == NULL ||
        glGetProgramivSRC == NULL ||
        glGetProgramInfoLogSRC == NULL ||
        glGenBuffersSRC == NULL ||
        glGetUniformLocationSRC == NULL ||
        glUniformMatrix4fvSRC == NULL
    )
        return 1;

    #if !defined(RGL_OPENGL_21)
    GLuint vao;
    glGenVertexArraysSRC(1, &vao);
    
    if (vao == 0)
        return 1;
    #endif
    
    #if !defined(RGL_OPENGL_21) && !defined(RGL_OPENGL_ES2)
    glDeleteVertexArraysSRC(1, &vao);
    #endif

    return 0;
}
#endif

#endif /* RGL_MODERN_OPENGL */

#endif /* RGL_IMPLEMENTATION */