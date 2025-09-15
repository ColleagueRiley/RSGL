/*
*
* Copyright (c) 2021-24 ColleagueRiley ColleagueRiley@gmail.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
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
    define args
    (MAKE SURE RSGL_IMPLEMENTATION is in exactly one header or you use -DRSGL_IMPLEMENTATION)
	#define RSGL_IMPLEMENTATION - makes it so source code is included with header

    #define RSGL_RFONT - do include functions to help with integrating RFont and RSGL
	#define RSGL_MAX_BATCHES [number of batches] - set max number of batches to be allocated
    #define RSGL_MAX_VERTS [number of verts] - set max number of verts to be allocated (global, not per batch)
*/
#include <stdint.h>
#ifndef RSGL_MAX_BATCHES
#define RSGL_MAX_BATCHES 2028
#endif
#ifndef RSGL_MAX_VERTS
#define RSGL_MAX_VERTS 8192
#endif

#ifndef RSGL_MALLOC
#include <stdlib.h>
#define RSGL_MALLOC malloc
#define RSGL_REALLOC realloc
#define RSGL_FREE free
#endif

#ifndef RSGL_SIN
#include <math.h>
#define RSGL_SIN sinf
#define RSGL_COS cosf
#endif

#ifndef RSGL_UNUSED
#define RSGL_UNUSED(x) (void) (x);
#endif

#ifndef RSGL_MEMCPY
	#include <string.h>
	#define RSGL_MEMCPY(dest, src, count) memcpy(dest, src, count)
	#define RSGL_MEMSET(ptr, value, num) memset(ptr, value, num)
#endif

#ifndef RSGL_H
#define RSGL_H
#ifndef RSGLDEF
#ifdef __APPLE__
#define RSGLDEF extern inline
#else
#define RSGLDEF inline
#endif
#endif

#ifndef RSGL_INT_DEFINED
    #define RSGL_INT_DEFINED
	#if defined(_MSC_VER) || defined(__SYMBIAN32__)
		typedef unsigned char 	u8;
		typedef signed char		i8;
		typedef unsigned short  u16;
		typedef signed short 	i16;
		typedef unsigned int 	u32;
		typedef signed int		i32;
		typedef unsigned long long	u64;
		typedef signed   long long	i64;
	#else
		#include <stdint.h>

		typedef uint8_t     u8;
		typedef int8_t      i8;
		typedef uint16_t   u16;
		typedef int16_t    i16;
		typedef uint32_t   u32;
		typedef int32_t    i32;
		typedef uint64_t   u64;
		typedef int64_t    i64;
	#endif
#endif

#ifndef RSGL_BOOL_DEFINED
#define RSGL_BOOL_DEFINED

#include <stdbool.h>
typedef bool RSGL_bool;
#endif

#define RSGL_TRUE (RSGL_bool)1
#define RSGL_FALSE (RSGL_bool)0

typedef enum RSGL_textureFormat {
	RSGL_formatNone = 0,
	RSGL_formatRGB,    /*!< 8-bit RGB (3 channels) */
    RSGL_formatBGR,    /*!< 8-bit BGR (3 channels) */
	RSGL_formatRGBA,   /*!< 8-bit RGBA (4 channels) */
    RSGL_formatBGRA,   /*!< 8-bit BGRA (4 channels) */
    RSGL_formatRed,   /*!< 8-bit RED (1 channel) */
    RSGL_formatGrayscale,   /*!< 8-bit grayscale (1 channel) */
    RSGL_formatGrayscaleAlpha,   /*!< 8-bit grayscale alpha (1 channel) */
	RSGL_formatCount
} RSGL_textureFormat;

typedef enum RSGL_textureDataType {
	RSGL_textureDataInt = 0,
	RSGL_textureDataFloat
} RSGL_textureDataType;

typedef enum RSGL_textureFilter {
	RSGL_filterNearest = 0,
	RSGL_filterLinear
} RSGL_textureFilter;

typedef struct RSGL_textureBlob {
	void* data; /* input data */
	size_t width; /* width of the texture */
	size_t height; /* height of the texture */
	RSGL_textureDataType dataType;
	RSGL_textureFormat dataFormat; /* format of the input data */
	RSGL_textureFormat textureFormat; /* final format for the texture */
	RSGL_textureFilter minFilter; /* filter used when rendering a surface smaller than the base texture */
	RSGL_textureFilter magFilter;  /* filter used when rendering a surface bigger than the base texture */
} RSGL_textureBlob;

#ifndef RSGL_texture
#define RSGL_texture size_t
#endif

/*
*******
RSGL shapes
*******
*/

#ifndef RSGL_rect
typedef struct RSGL_rect {
    i32 x, y, w, h;
} RSGL_rect;
#endif
#define RSGL_RECT(x, y, w, h) (RSGL_rect){(i32)(x), (i32)(y), (i32)(w), (i32)(h)}

#ifndef RSGL_rectF
typedef struct RSGL_rectF { float x, y, w, h; } RSGL_rectF;
#endif

#define RSGL_RECTF(x, y, w, h) (RSGL_rectF){(float)x, (float)y, (float)w, (float)h}

#ifndef RSGL_cube
typedef struct RSGL_cube { float x, y, z, w, h, l; } RSGL_cube;
#endif

#define RSGL_CUBE(x, y, z, w, h, l) (RSGL_cube){(float)x, (float)y, (float)z, (float)w, (float)h, (float)l}

#ifndef RSGL_point
typedef struct RSGL_point {
    i32 x, y;
} RSGL_point;
#endif
#define RSGL_POINT(x, y) (RSGL_point){(i32)x, (i32)y}

#ifndef RSGL_area
typedef struct RSGL_area {
    i32 w, h;
} RSGL_area;
#endif
#define RSGL_AREA(w, h) (RSGL_area){(i32)w, (i32)h}

#ifndef RSGL_pointF
typedef struct RSGL_pointF { float x, y; } RSGL_pointF;
#endif
#define RSGL_POINTF(x, y) (RSGL_pointF){(float)x, (float)y}

#ifndef RSGL_point3D
typedef struct RSGL_point3D { float x, y, z; } RSGL_point3D;
#endif

#define RSGL_POINT3D(x, y, z) (RSGL_point3D){(float)x, (float)y, (float)z}

#ifndef RSGL_areaF
typedef struct RSGL_areaF { float w, h;} RSGL_areaF;
#define RSGL_AREAF(w, h) (RSGL_areaF){(float)w, (float)h}
#endif

#ifndef RSGL_circle
typedef struct RSGL_circle {
    i32 x, y, d;
} RSGL_circle;
#endif

#define RSGL_CIRCLE(x, y, d) (RSGL_circle){(i32)x, (i32)y, (i32)d}

#ifndef RSGL_circleF
typedef struct RSGL_circleF { float x, y, d; } RSGL_circleF;
#endif
#define RSGL_CIRCLEF(x, y, d) (RSGL_circleF){(float)x, (float)y, (float)d}

#ifndef RSGL_triangle
typedef struct RSGL_triangle {
    RSGL_point p1, p2, p3;
} RSGL_triangle;
#endif

#define RSGL_TRIANGLE(p1, p2, p3) (RSGL_triangle){p1, p2, p3}

#ifndef RSGL_triangleF
typedef struct RSGL_triangleF { RSGL_pointF p1, p2, p3; } RSGL_triangleF;
#endif

#define RSGL_TRIANGLEF(p1, p2, p3) (RSGL_triangleF){p1, p2, p3}

#define RSGL_createTriangle(x1, y1, x2, y2, x3, y3) (RSGL_triangle){{x1, y1}, {x2, y2}, {x3, y3}}
#define RSGL_createTriangleF(x1, y1, x2, y2, x3, y3) (RSGL_triangleF){{x1, y1}, {x2, y2}, {x3, y3}}


#ifndef RSGL_triangle
typedef struct RSGL_triangle3D {
    RSGL_point3D p1, p2, p3;
} RSGL_triangle3D;
#endif

#define RSGL_TRIANGLE3D(p1, p2, p3) (RSGL_triangle3D){p1, p2, p3}

#define RSGL_createTriangle3D(x1, y1, z1, x2, y2, z2, x3, y3, z3) (RSGL_triangle3D){(RSGL_point3D){x1, y1, z1}, (RSGL_point3D){x2, y2, z2}, (RSGL_point3D){x3, y3, z3}}

/*
the color stucture is in
ABGR by default for performance reasons
(converting color to hex for example)
*/
#ifndef RSGL_color
typedef struct RSGL_color {
    u8 a, b, g, r;
} RSGL_color;
#endif

#define RSGL_RGBA(r, g, b, a) ((RSGL_color){(u8)(a), (u8)(b), (u8)(g), (u8)(r)})
#define RSGL_RGB(r, g, b) ((RSGL_color){255, (u8)(b), (u8)(g), (u8)(r)})

#define RSGL_COLOR_TO_HEX(color) ((u32)(color) & 0xFFFFFF00)
#define RSGL_RGB_TO_HEX(r, g, b, a) (RSGL_COLOR_TO_HEX(RSGL_RGBA(r, g, b, a)))
#define RSGL_RGBA_TO_HEX(r, g, b) (RSGL_COLOR_TO_HEX(RSGL_RGB(r, g, b, a)))

/*
*********************
RSGL matrix math
*********************
*/

#ifndef RSGL_mat4
typedef struct RSGL_mat4 {
    float m[16];
} RSGL_mat4;
#endif

RSGLDEF RSGL_mat4 RSGL_mat4_loadIdentity(void);
RSGLDEF RSGL_mat4 RSGL_mat4_scale(float matrix[16], float x, float y, float z);
RSGLDEF RSGL_mat4 RSGL_mat4_rotate(float matrix[16], float angle, float x, float y, float z);
RSGLDEF RSGL_mat4 RSGL_mat4_translate(float matrix[16], float x, float y, float z);
RSGLDEF RSGL_mat4 RSGL_mat4_perspective(float matrix[16], float fovY, float aspect, float zNear, float zFar);
RSGLDEF RSGL_mat4 RSGL_mat4_ortho(float matrix[16], float left, float right, float bottom, float top, float znear, float zfar);
RSGLDEF RSGL_mat4 RSGL_mat4_lookAt(float matrix[16], float eyeX, float eyeY, float eyeZ, float targetX, float targetY, float targetZ, float upX, float upY, float upZ);

RSGLDEF RSGL_mat4 RSGL_mat4_multiply(float left[16], float right[16]);

/*
*******
RSGL_perspective
*******
*/

typedef enum RSGL_projectionType {
	RSGL_projectionOrtho2D = 0,
	RSGL_projectionOrtho3D,
	RSGL_projectionPerspective3D,
} RSGL_projectionType;

typedef struct RSGL_projection2D {
	RSGL_projectionType type;
	u32 width;
	u32 height;
} RSGL_projection2D;

typedef struct RSGL_projection3D {
	RSGL_projectionType type;
	float fov;
	float ratio;
	float pNear;
	float pFar;
} RSGL_projection3D;

typedef union RSGL_projection {
	RSGL_projectionType type;
	RSGL_projection2D p2D;
	RSGL_projection3D p3D;
} RSGL_projection;

RSGLDEF RSGL_mat4 RSGL_projection_getMatrix(const RSGL_projection* projection);



/*
*******
RSGL_view
*******
*/

typedef enum RSGL_viewType {
	RSGL_viewTypeNone = 0,
	RSGL_viewType2D,
	RSGL_viewType3D,
} RSGL_viewType;

typedef struct RSGL_view2D {
	RSGL_viewType type;
	RSGL_point3D offset;
	RSGL_point3D target;
    float rotation;
    float zoom;
} RSGL_view2D;

/* RSGL translation */
typedef struct RSGL_view3D {
	RSGL_viewType type;
	RSGL_point3D pos;
	RSGL_point3D target;
    RSGL_point3D up;
} RSGL_view3D;

typedef union RSGL_view {
	RSGL_viewType type;
	RSGL_view2D view2D;
	RSGL_view3D view3D;
} RSGL_view;

RSGLDEF RSGL_mat4 RSGL_view_getMatrix(const RSGL_view* view);

/*
*********************
RSGL renderer
*********************
*/

/* used internally for RSGL_deleteProgram */
typedef enum RSGL_shaderType {
	RSGL_shaderTypeNone = 0,
	RSGL_shaderTypeStandard = 1, /* standard vertex+fragment shader */
	RSGL_shaderTypeCompute = 2,
	RSGL_shaderTypeGeometry = 4, /* unimplemented as of now */
} RSGL_shaderType;

/* shader program and blob */
typedef struct RSGL_programBlob {
	const char* vertex;
	size_t vertexLen;
	const char* fragment;
	size_t fragmentLen;
} RSGL_programBlob;

typedef struct RSGL_programInfo {
    size_t program;
	size_t perspectiveView;
	size_t model;
	RSGL_shaderType type;
} RSGL_programInfo;

typedef struct RSGL_BATCH {
    size_t start, len; /* when batch starts and it's length */
    size_t elmStart, elmCount; /* when element batch starts and it's length */
    u32 type;
    RSGL_texture tex;
    float lineWidth;
    RSGL_mat4 matrix;
} RSGL_BATCH; /* batch data type for rendering */

typedef struct RSGL_renderData {
	float* verts;
	float* texCoords;
	float* colors;
	u16* elements;
	size_t elements_count;
    size_t len; /* number of verts */

	RSGL_mat4 perspective; /* perspective matrix */
} RSGL_renderData;

typedef struct RSGL_renderBuffers {
	size_t vertex, color, texture, elements;
	size_t maxVerts;

	RSGL_BATCH batches[RSGL_MAX_BATCHES];
    size_t batchCount;
} RSGL_renderBuffers;

typedef struct RSGL_renderState {
    float* gradient; /* does not allocate any memory */

    RSGL_texture texture;
    u32 gradient_len;

	RSGL_color color;

    RSGL_point3D rotate;
	RSGL_programInfo* program;
	RSGL_renderBuffers* buffers;

    RSGL_point3D center;
    float lineWidth;
	RSGL_mat4 modelMatrix;
	RSGL_mat4 viewMatrix;
	RSGL_mat4 perspectiveMatrix;
	RSGL_bool forceBatch;
	RSGL_bool overflow;
} RSGL_renderState;

typedef struct RSGL_rendererProc {
	size_t (*size)(void); /* get the size of the renderer's internal struct */
	RSGL_programBlob (*defaultBlob)(void);
	void (*initPtr)(void* ctx, void* proc); /* init render backend */
	void (*freePtr)(void* ctx); /* free render backend */
	void (*render)(void* ctx, const RSGL_programInfo* program, const float* matrix, const RSGL_renderBuffers* info);
	void (*clear)(void* ctx, float r, float g, float b, float a);
	void (*viewport)(void* ctx, i32 x, i32 y, i32 w, i32 h);
	void (*setSurface)(void* ctx, void* surface);
	RSGL_texture (*createTexture)(void* ctx, const RSGL_textureBlob* blob);
	void (*copyToTexture)(void* ctx, RSGL_texture texture, size_t x, size_t y, const RSGL_textureBlob* blob);
	void (*deleteTexture)(void* ctx, RSGL_texture tex);
	void (*scissorStart)(void* ctx, RSGL_rectF scissor, i32 renderer_height);
	void (*scissorEnd)(void* ctx);
	RSGL_programInfo (*createProgram)(void* ctx, RSGL_programBlob* blob);
	void (*deleteProgram)(void* ctx, const RSGL_programInfo* program);
	size_t (*findShaderVariable)(void*, const RSGL_programInfo*, const char*, size_t);
	void (*updateShaderVariable)(void*, const RSGL_programInfo*, size_t, const float[], u8);
	RSGL_programInfo (*createComputeProgram)(void* ctx, const char* CShaderCode);
	void (*dispatchComputeProgram)(void* ctx, const RSGL_programInfo* program, u32 groups_x, u32 groups_y, u32 groups_z);
	void (*bindComputeTexture)(void* ctx, u32 texture, u8 format);
	void (*createBuffer)(void* ctx, size_t size, const void* data, size_t* buffer);
	void (*updateBuffer)(void* ctx, size_t buffer, void* data, size_t start, size_t len);
	void (*deleteBuffer)(void* ctx, size_t buffer);
} RSGL_rendererProc;

typedef struct RSGL_renderer {
	RSGL_renderData data;
	RSGL_renderState state;
	RSGL_rendererProc proc;
	void* userPtr;
	void* ctx; /* pointer for the renderer backend to store any internal data it wants/needs  */

	RSGL_texture defaultTexture;
	RSGL_programInfo defaultProgram;
	RSGL_mat4 defaultPerspectiveMatrix;

    float verts[RSGL_MAX_VERTS * 3];
    float texCoords[RSGL_MAX_VERTS * 2];
    float colors[RSGL_MAX_VERTS * 4];
    u16 elements[RSGL_MAX_VERTS * 6];
	RSGL_renderBuffers buffers;
} RSGL_renderer;

RSGLDEF void RSGL_renderer_getRenderState(RSGL_renderer* renderer, RSGL_renderState* state);

RSGLDEF size_t RSGL_renderer_size(RSGL_renderer* renderer);

RSGLDEF void RSGL_renderer_initPtr(RSGL_rendererProc proc,
                        RSGL_area area, /* graphics context size */
                        void* loader, /* opengl prozc address ex. wglProcAddress */
						void* ptr, /* pointer to allocate backend data */
					    RSGL_renderer* renderer
					);

RSGLDEF RSGL_renderer* RSGL_renderer_init(RSGL_rendererProc proc, RSGL_area area, void* loader);
RSGLDEF void RSGL_renderer_updateSize(RSGL_renderer* renderer, RSGL_area area);
RSGLDEF void RSGL_renderer_freePtr(RSGL_renderer* renderer);

RSGLDEF void RSGL_renderer_setSurface(RSGL_renderer* renderer, void* surface);

RSGLDEF void RSGL_renderer_createBuffer(RSGL_renderer* renderer, size_t size, const void* data, size_t* buffer);
RSGLDEF void RSGL_renderer_updateBuffer(RSGL_renderer* renderer, size_t buffer, void* data, size_t start, size_t len);
RSGLDEF void RSGL_renderer_deleteBuffer(RSGL_renderer* renderer, size_t buffer);

RSGLDEF void RSGL_renderer_createRenderBuffers(RSGL_renderer* renderer, size_t size, RSGL_renderBuffers* buffers);
RSGLDEF void RSGL_renderer_deleteRenderBuffers(RSGL_renderer* renderer, RSGL_renderBuffers* buffers);

RSGLDEF void RSGL_renderer_render(RSGL_renderer* renderer); /* draw current batch */
RSGLDEF void RSGL_renderer_updateRenderBuffers(RSGL_renderer* renderer);
RSGLDEF void RSGL_renderer_renderBuffers(RSGL_renderer* renderer);

RSGLDEF void RSGL_renderer_free(RSGL_renderer* renderer);

RSGLDEF void RSGL_renderer_setRotate(RSGL_renderer* renderer, RSGL_point3D rotate); /* apply rotation to drawing */
RSGLDEF void RSGL_renderer_setTexture(RSGL_renderer* renderer, RSGL_texture texture); /* apply texture to drawing */
RSGLDEF void RSGL_renderer_setColor(RSGL_renderer* renderer, RSGL_color color); /* apply color to drawing */
RSGLDEF void RSGL_renderer_setProgram(RSGL_renderer* renderer, RSGL_programInfo* program); /* use shader program for drawing */
RSGLDEF void RSGL_renderer_setRenderBuffers(RSGL_renderer* renderer, RSGL_renderBuffers* buffers);
RSGLDEF void RSGL_renderer_setGradient(RSGL_renderer* renderer,
                                float* gradient, /* array of gradients */
                                size_t len /* length of array */
                            ); /* apply gradient to drawing, based on color list*/
RSGLDEF void RSGL_renderer_setCenter(RSGL_renderer* renderer, RSGL_point3D center); /* the center of the drawing (or shape), this is used for rotation */
RSGLDEF void RSGL_renderer_setOverflow(RSGL_renderer* renderer, RSGL_bool overflow);
/* args clear after a draw function by default, this toggles that */
RSGLDEF void RSGL_renderer_clearArgs(RSGL_renderer* renderer); /* clears the args */

RSGLDEF RSGL_mat4 RSGL_renderer_initDrawMatrix(RSGL_renderer* renderer, RSGL_point3D center);

/* renders the current batches */
RSGLDEF void RSGL_renderer_clear(RSGL_renderer* renderer, RSGL_color color);
RSGLDEF void RSGL_renderer_viewport(RSGL_renderer* renderer, RSGL_rect rect);
/* create a texture based on a given bitmap, this must be freed later using RSGL_deleteTexture or opengl*/
RSGLDEF RSGL_texture RSGL_renderer_createTexture(RSGL_renderer* renderer, const RSGL_textureBlob* blob);
/* updates an existing texture wiht a new bitmap */
RSGLDEF void RSGL_renderer_copyToTexture(RSGL_renderer* renderer, RSGL_texture texture, size_t x, size_t y, const RSGL_textureBlob* blob);
/* delete a texture */
RSGLDEF void RSGL_renderer_deleteTexture(RSGL_renderer* renderer, RSGL_texture tex);
/* starts scissoring */
RSGLDEF void RSGL_renderer_scissorStart(RSGL_renderer* renderer, RSGL_rectF scissor, i32 height);
/* stops scissoring */
RSGLDEF void RSGL_renderer_scissorEnd(RSGL_renderer* renderer);
/* custom shader program */
RSGLDEF RSGL_programBlob RSGL_renderer_defaultBlob(RSGL_renderer* ctx);
RSGLDEF RSGL_programInfo RSGL_renderer_createProgram(RSGL_renderer* renderer, RSGL_programBlob* blob);
RSGLDEF void RSGL_renderer_deleteProgram(RSGL_renderer* renderer, const RSGL_programInfo* program);
RSGLDEF size_t RSGL_renderer_findShaderVariable(RSGL_renderer* renderer, const RSGL_programInfo* program, const char* var, size_t len);
RSGLDEF void RSGL_renderer_updateShaderVariable(RSGL_renderer* renderer, const RSGL_programInfo* program, size_t var, const float value[], u8 len);
RSGLDEF void RSGL_renderer_forceBatch(RSGL_renderer* renderer);

RSGLDEF void RSGL_renderer_setPerspectiveMatrix(RSGL_renderer* renderer, RSGL_mat4 matrix);
RSGLDEF void RSGL_renderer_setDefaultPerspectiveMatrix(RSGL_renderer* renderer, RSGL_mat4 matrix);

RSGLDEF void RSGL_renderer_setModelMatrix(RSGL_renderer* renderer, RSGL_mat4 matrix);
RSGLDEF void RSGL_renderer_resetModelMatrix(RSGL_renderer* renderer);

RSGLDEF RSGL_programInfo RSGL_renderer_createComputeProgram(RSGL_renderer* renderer, const char *CShaderCode);
RSGLDEF void RSGL_renderer_dispatchComputeProgram(RSGL_renderer* renderer, const RSGL_programInfo* program, u32 groups_x, u32 groups_y, u32 groups_z);
RSGLDEF void RSGL_renderer_bindComputeTexture(RSGL_renderer* renderer, u32 texture, u8 format);


/*
 ******
 * Rfont_RSGL RFont integration *
 ******
 * */

#ifdef RSGL_RFONT
RSGLDEF struct RFont_renderer_proc RFont_RSGL_renderer_proc(void);

RSGLDEF struct RFont_renderer* RFont_RSGL_renderer_init(struct RSGL_renderer* ptr);
RSGLDEF void RFont_RSGL_renderer_initPtr(struct RSGL_renderer* ptr, struct RFont_renderer* renderer);
RSGLDEF void RFont_RSGL_renderer_free(struct RFont_renderer* renderer);
#endif

/*
*******
RSGL_draw
*******
*/

/*
RSGL_drawRawVerts is a function used internally by RSGL, but you can use it yourself
RSGL_drawRawVerts batches a given set of points based on th data to be rendered
*/

typedef enum RSGL_drawType {
	RSGL_TRIANGLES = 0,
	RSGL_POINTS = 1,
	RSGL_LINES = 2
} RSGL_drawType;

typedef struct RSGL_rawVerts {
	RSGL_drawType type;
	float* verts;
	float* texCoords;
	u16* elements;
	size_t elmCount;
	size_t vert_count;
} RSGL_rawVerts;

RSGLDEF i32 RSGL_drawRawVerts(RSGL_renderer* renderer, const RSGL_rawVerts* data);

/* 2D shape drawing */
/* in the function names, F means float */

RSGLDEF i32 RSGL_drawPoint(RSGL_renderer* renderer, RSGL_point p);
RSGLDEF i32 RSGL_drawPointF(RSGL_renderer* renderer, RSGL_pointF p);

RSGLDEF i32 RSGL_drawTriangle(RSGL_renderer* renderer, RSGL_triangle t);
RSGLDEF i32 RSGL_drawTriangleF(RSGL_renderer* renderer, RSGL_triangleF t);

RSGLDEF i32 RSGL_drawTriangleHyp(RSGL_renderer* renderer, RSGL_pointF p, size_t angle, float hypotenuse);

RSGLDEF i32 RSGL_drawRect(RSGL_renderer* renderer, RSGL_rect r);
RSGLDEF i32 RSGL_drawRectF(RSGL_renderer* renderer, RSGL_rectF r);

RSGLDEF i32 RSGL_drawRoundRect(RSGL_renderer* renderer, RSGL_rect r, RSGL_point rounding);
RSGLDEF i32 RSGL_drawRoundRectF(RSGL_renderer* renderer, RSGL_rectF r, RSGL_point rounding);

RSGLDEF i32 RSGL_drawPolygon(RSGL_renderer* renderer, RSGL_rect r, u32 sides);
RSGLDEF i32 RSGL_drawPolygonF(RSGL_renderer* renderer, RSGL_rectF r, u32 sides);

RSGLDEF i32 RSGL_drawArc(RSGL_renderer* renderer, RSGL_rect o, RSGL_point arc);
RSGLDEF i32 RSGL_drawArcF(RSGL_renderer* renderer, RSGL_rectF o, RSGL_pointF arc);

RSGLDEF i32 RSGL_drawCircle(RSGL_renderer* renderer, RSGL_circle c);
RSGLDEF i32 RSGL_drawCircleF(RSGL_renderer* renderer, RSGL_circleF c);

RSGLDEF i32 RSGL_drawOval(RSGL_renderer* renderer, RSGL_rect o);
RSGLDEF i32 RSGL_drawOvalF(RSGL_renderer* renderer, RSGL_rectF o);

RSGLDEF i32 RSGL_drawLine(RSGL_renderer* renderer, RSGL_point p1, RSGL_point p2, u32 thickness);
RSGLDEF i32 RSGL_drawLineF(RSGL_renderer* renderer, RSGL_pointF p1, RSGL_pointF p2, u32 thickness);

/* 3D objects */
RSGLDEF i32 RSGL_drawTriangle3D(RSGL_renderer* renderer, RSGL_triangle3D t);
RSGLDEF i32 RSGL_drawPoint3D(RSGL_renderer* renderer, RSGL_point3D p);
RSGLDEF i32 RSGL_drawLine3D(RSGL_renderer* renderer, RSGL_point3D p1, RSGL_point3D p2, u32 thickness);
RSGLDEF i32 RSGL_drawCube(RSGL_renderer* renderer, RSGL_cube cube);

/* 2D outlines */

/* thickness means the thickness of the line */

RSGLDEF i32 RSGL_drawTriangleOutline(RSGL_renderer* renderer, RSGL_triangle t, u32 thickness);
RSGLDEF i32 RSGL_drawTriangleFOutline(RSGL_renderer* renderer, RSGL_triangleF t, u32 thickness);

RSGLDEF i32 RSGL_drawRectOutline(RSGL_renderer* renderer, RSGL_rect r, u32 thickness);
RSGLDEF i32 RSGL_drawRectFOutline(RSGL_renderer* renderer, RSGL_rectF r, u32 thickness);

RSGLDEF i32 RSGL_drawRoundRectOutline(RSGL_renderer* renderer, RSGL_rect r, RSGL_point rounding, u32 thickness);
RSGLDEF i32 RSGL_drawRoundRectFOutline(RSGL_renderer* renderer, RSGL_rectF r, RSGL_point rounding, u32 thickness);

RSGLDEF i32 RSGL_drawPolygonOutline(RSGL_renderer* renderer, RSGL_rect r, u32 sides, u32 thickness);
RSGLDEF i32 RSGL_drawPolygonFOutline(RSGL_renderer* renderer, RSGL_rectF r, u32 sides, u32 thickness);

RSGLDEF i32 RSGL_drawArcOutline(RSGL_renderer* renderer, RSGL_rect o, RSGL_point arc, u32 thickness);
RSGLDEF i32 RSGL_drawArcFOutline(RSGL_renderer* renderer, RSGL_rectF o, RSGL_pointF arc, u32 thickness);

RSGLDEF i32 RSGL_drawCircleOutline(RSGL_renderer* renderer, RSGL_circle c, u32 thickness);
RSGLDEF i32 RSGL_drawCircleFOutline(RSGL_renderer* renderer, RSGL_circleF c, u32 thickness);

RSGLDEF i32 RSGL_drawOvalFOutline(RSGL_renderer* renderer, RSGL_rectF o, u32 thickness);
RSGLDEF i32 RSGL_drawOvalOutline(RSGL_renderer* renderer, RSGL_rect o, u32 thickness);
#endif /* ndef RSGL_H */

#ifdef RSGL_IMPLEMENTATION

#ifndef M_PI
    #define M_PI 3.14159265358979323846f
#endif
#ifndef DEG2RAD
    #define DEG2RAD (float)(M_PI / 180.0f)
#endif
#ifndef RAD2DEG
    #define RAD2DEG (float)(180.0f / M_PI)
#endif

#define RSGL_GET_MATRIX_X(x, y, z) (float)(matrix.m[0] * x + matrix.m[4] * y + matrix.m[8] * z + matrix.m[12])
#define RSGL_GET_MATRIX_Y(x, y, z) (float)(matrix.m[1] * x + matrix.m[5] * y + matrix.m[9] * z + matrix.m[13])
#define RSGL_GET_MATRIX_Z(x, y, z) (float)(matrix.m[2] * x + matrix.m[6] * y + matrix.m[10] * z + matrix.m[14])
#define RSGL_GET_MATRIX_W(x, y, z) (float)(matrix.m[2] * x + matrix.m[7] * y + matrix.m[11] * z + matrix.m[15])

#define RSGL_GET_MATRIX_POINT(x, y, z) \
            RSGL_GET_MATRIX_X((x), (y), (z)) / RSGL_GET_MATRIX_W((x), (y), (z)), \
            RSGL_GET_MATRIX_Y((x), (y), (z)) / RSGL_GET_MATRIX_W((x), (y), (z)), \
            RSGL_GET_MATRIX_Z((x), (y), (z)) / RSGL_GET_MATRIX_W((x), (y), (z))

#define RSGL_GET_MATRIX_POINTW(x, y, z) \
            RSGL_GET_MATRIX_X(x, y, z), \
            RSGL_GET_MATRIX_Y(x, y, z), \
            RSGL_GET_MATRIX_Z(x, y, z), \
            RSGL_GET_MATRIX_W(x, y, z) \

void RSGL_renderer_forceBatch(RSGL_renderer* renderer) {
    renderer->state.forceBatch = RSGL_TRUE;
}

void RSGL_renderer_setPerspectiveMatrix(RSGL_renderer* renderer, RSGL_mat4 matrix) {
	renderer->state.perspectiveMatrix = matrix;
}

void RSGL_renderer_setDefaultPerspectiveMatrix(RSGL_renderer* renderer, RSGL_mat4 matrix) {
	renderer->defaultPerspectiveMatrix = matrix;
}

void RSGL_renderer_setViewMatrix(RSGL_renderer* renderer, RSGL_mat4 matrix) {
    renderer->state.viewMatrix = matrix;
}
void RSGL_renderer_resetViewMatrix(RSGL_renderer* renderer) {
	RSGL_renderer_setViewMatrix(renderer, RSGL_mat4_loadIdentity());
}

void RSGL_renderer_setModelMatrix(RSGL_renderer* renderer, RSGL_mat4 matrix) {
    renderer->state.modelMatrix = matrix;
    renderer->state.forceBatch = RSGL_TRUE;
}


void RSGL_renderer_resetModelMatrix(RSGL_renderer* renderer) {
	RSGL_renderer_setModelMatrix(renderer, RSGL_mat4_loadIdentity());
}

void RSGL_renderer_getRenderState(RSGL_renderer* renderer, RSGL_renderState* state) {
	if (state) *state = renderer->state;
}

void RSGL_renderer_setOverflow(RSGL_renderer* renderer, RSGL_bool overflow) {
	renderer->state.overflow = overflow;
}
#include <stdio.h>
i32 RSGL_drawRawVerts(RSGL_renderer* renderer, const RSGL_rawVerts* data) {
	if ((renderer->state.buffers->batchCount + 1 >= RSGL_MAX_BATCHES || renderer->data.len + data->vert_count >= renderer->state.buffers->maxVerts) && renderer->state.overflow) {
        RSGL_renderer_render(renderer);
    }

	RSGL_BATCH* batch = NULL;
	RSGL_color c = renderer->state.color;

    if (
        renderer->state.buffers->batchCount == 0 ||
        renderer->state.buffers->batches[renderer->state.buffers->batchCount - 1].tex != renderer->state.texture  ||
        renderer->state.buffers->batches[renderer->state.buffers->batchCount - 1].lineWidth != renderer->state.lineWidth ||
        renderer->state.buffers->batches[renderer->state.buffers->batchCount - 1].type != data->type ||
        renderer->state.forceBatch
    ) {
        renderer->state.forceBatch = RSGL_FALSE;
        renderer->state.buffers->batchCount += 1;

        batch = &renderer->state.buffers->batches[renderer->state.buffers->batchCount - 1];
		batch->start = renderer->data.len;
        batch->len = 0;
   		batch->elmStart = renderer->data.elements_count;
        batch->elmCount = 0;
		batch->type = data->type;
        batch->tex = renderer->state.texture;
        batch->lineWidth = renderer->state.lineWidth;
		batch->matrix = renderer->state.modelMatrix;
    } else {
        batch = &renderer->state.buffers->batches[renderer->state.buffers->batchCount - 1];
    }

    if (batch == NULL) {
        return -1;
	}

	batch->elmCount += data->elmCount;
    batch->len += data->vert_count;

    RSGL_MEMCPY(&renderer->data.verts[renderer->data.len * 3], data->verts, data->vert_count * sizeof(float) * 3);
    RSGL_MEMCPY(&renderer->data.texCoords[renderer->data.len * 2], data->texCoords, data->vert_count * sizeof(float) * 2);

	size_t i;
	for (i = 0; i < data->elmCount; i++) {
		size_t index = renderer->data.elements_count + i;
		u16 elm = data->elements[i] + (u16)renderer->data.len;
		renderer->data.elements[index] = elm;
	}

	renderer->data.elements_count += data->elmCount;

    float color[4] = {c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f};

    if (renderer->state.gradient_len && renderer->state.gradient && (i64)(data->vert_count - 1) > 0) {
        RSGL_MEMCPY(&renderer->data.colors[renderer->data.len * 4], color, sizeof(float) * 4);
        RSGL_MEMCPY(&renderer->data.colors[renderer->data.len * 4 + 4], renderer->state.gradient, (data->vert_count - 1) * sizeof(float) * 4);
    }
    else {
        size_t i;
        for (i = 0; i < data->vert_count * 4; i += 4)
            RSGL_MEMCPY(&renderer->data.colors[(renderer->data.len * 4) + i], color, sizeof(float) * 4);
    }

    renderer->data.len += data->vert_count;
	return renderer->state.buffers->batchCount - 1;
}

/*
*********************
RSGL_GRAPHICS_CONTEXT
*********************
*/

void RSGL_renderer_setSurface(RSGL_renderer* renderer, void* surface) {
	renderer->proc.setSurface(renderer->ctx, surface);
}

void RSGL_renderer_createBuffer(RSGL_renderer* renderer, size_t size, const void* data, size_t* buffer) {
	if (renderer->proc.createBuffer) {
		renderer->proc.createBuffer(renderer->ctx, size, data, buffer);
	}
}

void RSGL_renderer_deleteRenderBuffers(RSGL_renderer* renderer, RSGL_renderBuffers* buffers) {
	RSGL_renderer_deleteBuffer(renderer, buffers->elements);
	RSGL_renderer_deleteBuffer(renderer, buffers->vertex);
	RSGL_renderer_deleteBuffer(renderer, buffers->color);
	RSGL_renderer_deleteBuffer(renderer, buffers->texture);
}

void RSGL_renderer_createRenderBuffers(RSGL_renderer* renderer, size_t size, RSGL_renderBuffers* buffers) {
	buffers->maxVerts = size;
	renderer->proc.createBuffer(renderer->ctx, size * 3 * sizeof(float), NULL, &buffers->vertex);
	renderer->proc.createBuffer(renderer->ctx, size * 4 * sizeof(float), NULL, &buffers->color);
	renderer->proc.createBuffer(renderer->ctx, size * 2 * sizeof(float), NULL, &buffers->texture);
	renderer->proc.createBuffer(renderer->ctx, size * 6 * sizeof(u16), NULL, &buffers->elements);
}

void RSGL_renderer_updateBuffer(RSGL_renderer* renderer, size_t buffer, void* data, size_t start, size_t len) {
	if (renderer->proc.updateBuffer)
		renderer->proc.updateBuffer(renderer->ctx, buffer, data, start, len);
}

void RSGL_renderer_deleteBuffer(RSGL_renderer* renderer, size_t buffer) {
	if (renderer->proc.deleteBuffer)
		renderer->proc.deleteBuffer(renderer->ctx, buffer);
}

void RSGL_renderer_updateRenderBuffers(RSGL_renderer* renderer) {
	RSGL_renderer_updateBuffer(renderer, renderer->state.buffers->vertex, renderer->data.verts, 0, renderer->data.len * 3 * sizeof(float));
	RSGL_renderer_updateBuffer(renderer, renderer->state.buffers->color, renderer->data.colors, 0, renderer->data.len * 4 * sizeof(float));
	RSGL_renderer_updateBuffer(renderer, renderer->state.buffers->texture, renderer->data.texCoords, 0, renderer->data.len * 2 * sizeof(float));
	RSGL_renderer_updateBuffer(renderer, renderer->state.buffers->elements, renderer->data.elements, 0, renderer->data.elements_count * sizeof(u16));
}

void RSGL_renderer_render(RSGL_renderer* renderer) {
	if (renderer->data.len && renderer->state.buffers->batchCount) {
		RSGL_renderer_updateRenderBuffers(renderer);

		RSGL_mat4 matrix = RSGL_mat4_multiply(renderer->defaultPerspectiveMatrix.m, renderer->state.perspectiveMatrix.m);
		matrix = RSGL_mat4_multiply(matrix.m, renderer->state.viewMatrix.m);

		if (renderer->proc.render)
			renderer->proc.render(renderer->ctx, renderer->state.program, matrix.m, renderer->state.buffers);
	}

	renderer->data.len = 0;
	renderer->data.elements_count = 0;
	renderer->state.buffers->batchCount = 0;
}

void RSGL_renderer_renderBuffers(RSGL_renderer* renderer) {
	if (renderer->proc.render && renderer->state.buffers->batchCount) {
		RSGL_mat4 matrix = RSGL_mat4_multiply(renderer->defaultPerspectiveMatrix.m, renderer->state.perspectiveMatrix.m);
		matrix = RSGL_mat4_multiply(matrix.m, renderer->state.viewMatrix.m);
		renderer->proc.render(renderer->ctx, renderer->state.program, matrix.m, renderer->state.buffers);
	}

	renderer->data.elements_count = 0;
	renderer->data.len = 0;
}

size_t RSGL_renderer_size(RSGL_renderer* renderer) {
	if (renderer->proc.size) return renderer->proc.size();
	return 0;
}

void RSGL_renderer_initPtr(RSGL_rendererProc proc, RSGL_area area, void* loader, void* data, RSGL_renderer* renderer) {
	renderer->ctx = data;
	renderer->proc = proc;
    RSGL_renderer_clearArgs(renderer);
    renderer->state.color = RSGL_RGBA(0, 0, 0, 255);
    RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, area.w, area.h));

    renderer->state.modelMatrix = RSGL_mat4_loadIdentity();
	renderer->data.verts = renderer->verts;
	renderer->data.texCoords = renderer->texCoords;
	renderer->data.elements = renderer->elements;
	renderer->data.colors = renderer->colors;
	renderer->data.len = 0;

	if (renderer->proc.initPtr)
		renderer->proc.initPtr(renderer->ctx, loader);

	RSGL_programBlob pBlob = RSGL_renderer_defaultBlob(renderer);
	renderer->defaultProgram = RSGL_renderer_createProgram(renderer, &pBlob);
	RSGL_renderer_setProgram(renderer, &renderer->defaultProgram);

    u8 white[4] = {255, 255, 255, 255};
	RSGL_textureBlob blob;
	blob.data = white;
	blob.width = 1;
	blob.height = 1;
	blob.dataType = RSGL_textureDataInt;
	blob.dataFormat = RSGL_formatRGBA;
	blob.textureFormat = RSGL_formatRGBA;
    renderer->defaultTexture = RSGL_renderer_createTexture(renderer, &blob);

	RSGL_renderer_setTexture(renderer, renderer->defaultTexture);

	RSGL_renderer_createRenderBuffers(renderer, RSGL_MAX_VERTS, &renderer->buffers);
    renderer->buffers.batchCount = 0;
	renderer->state.buffers = &renderer->buffers;

	RSGL_renderer_setModelMatrix(renderer, RSGL_mat4_loadIdentity());
	RSGL_renderer_resetViewMatrix(renderer);
	RSGL_renderer_updateSize(renderer, area);
	RSGL_renderer_setPerspectiveMatrix(renderer, RSGL_mat4_loadIdentity());
}


RSGL_renderer* RSGL_renderer_init(RSGL_rendererProc proc, RSGL_area area, void* loader) {
	RSGL_renderer* renderer = (RSGL_renderer*)RSGL_MALLOC(sizeof(RSGL_renderer));
	void* data = RSGL_MALLOC(proc.size());
	RSGL_renderer_initPtr(proc, area, loader, data, renderer);
	return renderer;
}

void RSGL_renderer_freePtr(RSGL_renderer* renderer) {
	RSGL_renderer_deleteRenderBuffers(renderer, &renderer->buffers);

	if (renderer->proc.freePtr)
		renderer->proc.freePtr(renderer->ctx);

	renderer->state.buffers->batchCount = 0;
	renderer->data.len = 0;
}

void RSGL_renderer_free(RSGL_renderer* renderer) {
	RSGL_renderer_freePtr(renderer);
	if (renderer->ctx)
		RSGL_FREE(renderer->ctx);
	RSGL_FREE(renderer);
}

void RSGL_renderer_clear(RSGL_renderer* renderer, RSGL_color color) {
	if (renderer->proc.clear)
		renderer->proc.clear(renderer->ctx, ((float)color.r) / 255.0f, ((float)color.g) / 255.0f, ((float)color.b) / 255.0f, ((float)color.a) / 255.0f);
}
void RSGL_renderer_viewport(RSGL_renderer* renderer, RSGL_rect rect) { renderer->proc.viewport(renderer->ctx, rect.x, rect.y, rect.w, rect.h); }
RSGL_texture RSGL_renderer_createTexture(RSGL_renderer* renderer, const RSGL_textureBlob* blob) {
    RSGL_texture tex = 0;
	if (renderer->proc.createTexture) tex = renderer->proc.createTexture(renderer->ctx, blob);
	return tex;
}
void RSGL_renderer_copyToTexture(RSGL_renderer* renderer, RSGL_texture texture, size_t x, size_t y, const RSGL_textureBlob* blob) {
    return renderer->proc.copyToTexture(renderer->ctx, texture, x, y, blob);
}
void RSGL_renderer_deleteTexture(RSGL_renderer* renderer, RSGL_texture tex) { renderer->proc.deleteTexture(renderer->ctx, tex); }
void RSGL_renderer_scissorStart(RSGL_renderer* renderer, RSGL_rectF scissor, i32 height) {
    RSGL_renderer_render(renderer);
	renderer->proc.scissorStart(renderer->ctx, scissor, height);
}
void RSGL_renderer_scissorEnd(RSGL_renderer* renderer) {
    RSGL_renderer_render(renderer);
	renderer->proc.scissorEnd(renderer->ctx);
}
RSGL_programBlob RSGL_renderer_defaultBlob(RSGL_renderer* renderer) {
	RSGL_programBlob blob;
	RSGL_MEMSET(&blob, 0, sizeof(blob));

	if (renderer->proc.defaultBlob) {
		blob = renderer->proc.defaultBlob();
	}

	return blob;
}
RSGL_programInfo RSGL_renderer_createProgram(RSGL_renderer* renderer, RSGL_programBlob* blob) {
	RSGL_programInfo info;
	RSGL_MEMSET(&info, 0, sizeof(info));

	if (renderer->proc.createProgram) {
		info = renderer->proc.createProgram(renderer->ctx, blob);
	}

	return info;
}
void RSGL_renderer_deleteProgram(RSGL_renderer* renderer, const RSGL_programInfo* program) { return renderer->proc.deleteProgram(renderer->ctx, program); }

size_t RSGL_renderer_findShaderVariable(RSGL_renderer* renderer, const RSGL_programInfo* program, const char* var, size_t len) {
    return renderer->proc.findShaderVariable(renderer->ctx, program, var, len);
}

void RSGL_renderer_updateShaderVariable(RSGL_renderer* renderer, const RSGL_programInfo* program, size_t var, const float value[], u8 len) {
	if (renderer->proc.updateShaderVariable) {
		renderer->proc.updateShaderVariable(renderer->ctx, program, var, value, len);
	}
}

RSGL_programInfo RSGL_renderer_createComputeProgram(RSGL_renderer* renderer, const char* CShaderCode) {
	return renderer->proc.createComputeProgram(renderer->ctx, CShaderCode);
}

void RSGL_renderer_dispatchComputeProgram(RSGL_renderer* renderer, const RSGL_programInfo* program, u32 groups_x, u32 groups_y, u32 groups_z) {
	renderer->proc.dispatchComputeProgram(renderer->ctx, program, groups_x, groups_y, groups_z);
}

void RSGL_renderer_bindComputeTexture(RSGL_renderer* renderer, u32 texture, u8 format) {
	renderer->proc.bindComputeTexture(renderer->ctx, texture, format);
}

void RSGL_renderer_updateSize(RSGL_renderer* renderer, RSGL_area area) {
	RSGL_projection projection;
	projection.p2D.type = RSGL_projectionOrtho2D;
	projection.p2D.width = area.w;
	projection.p2D.height = area.h;

	RSGL_mat4 matrix = RSGL_projection_getMatrix(&projection);
	RSGL_renderer_setDefaultPerspectiveMatrix(renderer, matrix);
}

void RSGL_renderer_setRotate(RSGL_renderer* renderer, RSGL_point3D rotate){
    renderer->state.rotate = RSGL_POINT3D(rotate.x * DEG2RAD, rotate.y * DEG2RAD, rotate.z * DEG2RAD);
}
void RSGL_renderer_setTexture(RSGL_renderer* renderer, RSGL_texture texture) {
	if (texture == 0)
		renderer->state.texture = renderer->defaultTexture;
	else
		renderer->state.texture = texture;
}
void RSGL_renderer_setColor(RSGL_renderer* renderer, RSGL_color color) {
	renderer->state.color = color;
}

void RSGL_renderer_setRenderBuffers(RSGL_renderer* renderer, RSGL_renderBuffers* buffers) {
	if (buffers == NULL)
		renderer->state.buffers = &renderer->buffers;
	else
		renderer->state.buffers = buffers;
}

void RSGL_renderer_setProgram(RSGL_renderer* renderer, RSGL_programInfo* program) {
	if (program == NULL)
		renderer->state.program = &renderer->defaultProgram;
	else
		renderer->state.program = program;
}

void RSGL_renderer_setGradient(RSGL_renderer* renderer, float gradient[], size_t len) {
    renderer->state.gradient_len = len;
    renderer->state.gradient = gradient;
}
void RSGL_renderer_setCenter(RSGL_renderer* renderer, RSGL_point3D center) {
    renderer->state.center = center;
}

RSGL_mat4 RSGL_renderer_initDrawMatrix(RSGL_renderer* renderer, RSGL_point3D center) {
    RSGL_mat4 matrix = RSGL_mat4_loadIdentity();

    if (renderer->state.rotate.x || renderer->state.rotate.y || renderer->state.rotate.z) {
        if (renderer->state.center.x != -1 && renderer->state.center.y != -1 &&  renderer->state.center.z != -1)
            center = renderer->state.center;

        matrix = RSGL_mat4_translate(matrix.m, center.x, center.y, center.z);
        matrix = RSGL_mat4_rotate(matrix.m, renderer->state.rotate.z,  0, 0, 1);
        matrix = RSGL_mat4_rotate(matrix.m, renderer->state.rotate.y, 0, 1, 0);
        matrix = RSGL_mat4_rotate(matrix.m, renderer->state.rotate.x, 1, 0, 0);
        matrix = RSGL_mat4_translate(matrix.m, -center.x, -center.y, -center.z);
    }

    return matrix;
}

void RSGL_renderer_clearArgs(RSGL_renderer* renderer) {
	RSGL_MEMSET(&renderer->state, 0, sizeof(renderer->state));
	renderer->state.center =  RSGL_POINT3D(-1, -1, -1);
	renderer->state.overflow = RSGL_TRUE;
}

/*
****
RFont_RSGL integration
****
*/

#ifdef RSGL_RFONT

#ifndef RFONT_H

#ifdef RSGL_INT_DEFINED
	#ifndef RFONT_INT_DEFINED
		#define RFONT_INT_DEFINED
	#endif
#endif

#include "RFont.h"
#endif

struct RFont_renderer* RFont_RSGL_renderer_init(struct RSGL_renderer* ptr) {
	RFont_renderer* renderer = (RFont_renderer*)RFONT_MALLOC(sizeof(RFont_renderer));
	RFont_renderer_initPtr(RFont_RSGL_renderer_proc(), ptr, renderer);
	return renderer;
}
void RFont_RSGL_renderer_initPtr(struct RSGL_renderer* ptr, struct RFont_renderer* renderer) { RFont_renderer_initPtr(RFont_RSGL_renderer_proc(), ptr, renderer); }

void RFont_RSGL_renderer_free(struct RFont_renderer* renderer) {
	RFONT_FREE(renderer);
}

i32 RFont_RSGL_render_text(RSGL_renderer* renderer, const RFont_render_data* src) {
    RSGL_texture save = renderer->state.texture;
	RSGL_renderer_setTexture(renderer, src->atlas);

	RSGL_rawVerts data;
	data.type = RSGL_TRIANGLES;
	data.verts = src->verts;
	data.texCoords = src->tcoords;
	data.elements = src->elements;
	data.elmCount = src->nelements;
	data.vert_count = src->nverts;
	i32 batch = RSGL_drawRawVerts(renderer, &data);
    RSGL_renderer_setTexture(renderer, save);

	return batch;
}

RFont_texture RFont_RSGL_createAtlas(RSGL_renderer* renderer, u32 atlasWidth, u32 atlasHeight) {
	RSGL_textureBlob blob;
	blob.data = NULL;
	blob.width = atlasWidth;
	blob.height = atlasWidth;
	blob.dataType = RSGL_textureDataInt;
	blob.dataFormat = RSGL_formatRGBA;
	blob.textureFormat = RSGL_formatRGBA;
	RFont_texture id = RSGL_renderer_createTexture(renderer, &blob);
	return id;
}

void RFont_RSGL_deleteAtlas(RSGL_renderer* renderer, RFont_texture atlas) {
	RSGL_renderer_deleteTexture(renderer, atlas);
}

void RFont_RSGL_bitmapToAtlas(RSGL_renderer* renderer, RFont_texture atlas, u32 atlasWidth, u32 atlasHeight, u32 maxHeight, u8* bitmap, float w, float h, float* x, float* y) {
	RSGL_UNUSED(atlasHeight);
	if (((*x) + w) >= atlasWidth) {
		*x = 0;
		*y += (float)maxHeight;
	}

	RSGL_textureBlob blob;
	blob.data = bitmap;
	blob.width = w;
	blob.height = h;
	blob.dataType = RSGL_textureDataInt;
	blob.dataFormat = RSGL_formatGrayscaleAlpha;
	blob.textureFormat = blob.dataFormat;

	RSGL_renderer_copyToTexture(renderer, atlas, (size_t)(*x), (size_t)(*y), &blob);

	*x += w;
}

void RFont_RSGL_setFrameBuffer(RSGL_renderer* renderer, u32 width, u32 height) {
	RSGL_renderer_updateSize(renderer, RSGL_AREA(width, height));
}

void RFont_RSGL_setColor(RSGL_renderer* renderer, float r, float g, float b, float a) {
	RSGL_renderer_setColor(renderer, RSGL_RGBA(r * 255.0f, g * 255.0f, b * 255.0f, a * 255.0f));
}

RFont_renderer_proc RFont_RSGL_renderer_proc(void) {
	RFont_renderer_proc proc;
	RSGL_MEMSET(&proc, 0, sizeof(proc));
	proc.create_atlas = (RFont_texture (*)(void* ctx, u32 atlasWidth, u32 atlasHeight))RFont_RSGL_createAtlas;
	proc.free_atlas = (void (*)(void*, RSGL_texture))RFont_RSGL_deleteAtlas;
	proc.bitmap_to_atlas = (void(*)(void*, RFont_texture, u32, u32, u32, u8*, float, float, float*, float*))RFont_RSGL_bitmapToAtlas;
	proc.render = (void (*)(void*, const RFont_render_data* data))RFont_RSGL_render_text;
	proc.set_framebuffer = (void (*)(void*, u32, u32))RFont_RSGL_setFrameBuffer;
	proc.set_color = (void (*)(void*, float, float, float, float))RFont_RSGL_setColor;
	return proc;
}

#endif

/*
****
RSGL_draw
****
*/

i32 RSGL_drawPoint(RSGL_renderer* renderer, RSGL_point p) {
    return RSGL_drawPointF(renderer, (RSGL_pointF){(float)p.x, (float)p.y});
}

i32 RSGL_drawTriangle(RSGL_renderer* renderer, RSGL_triangle t) {
    return RSGL_drawTriangleF(renderer, RSGL_createTriangleF((float)t.p1.x, (float)t.p1.y, (float)t.p2.x, (float)t.p2.y, (float)t.p3.x, (float)t.p3.y));
}

i32 RSGL_drawRect(RSGL_renderer* renderer, RSGL_rect r) {
    return RSGL_drawRectF(renderer, (RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h});
}

i32 RSGL_drawRoundRect(RSGL_renderer* renderer, RSGL_rect r, RSGL_point rounding) {
    return RSGL_drawRoundRectF(renderer, (RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, rounding);
}

i32 RSGL_drawPolygon(RSGL_renderer* renderer, RSGL_rect r, u32 sides) {
    return RSGL_drawPolygonF(renderer, (RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, sides);
}

i32 RSGL_drawArc(RSGL_renderer* renderer, RSGL_rect o, RSGL_point arc) {
    return RSGL_drawArcF(renderer, (RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, (RSGL_pointF){(float)arc.x, (float)arc.y});
}

i32 RSGL_drawCircle(RSGL_renderer* renderer, RSGL_circle c) {
    return RSGL_drawCircleF(renderer, (RSGL_circleF){(float)c.x, (float)c.y, (float)c.d});
}

i32 RSGL_drawOval(RSGL_renderer* renderer, RSGL_rect o) {
    return RSGL_drawOvalF(renderer, (RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h});
}

i32 RSGL_drawLine(RSGL_renderer* renderer, RSGL_point p1, RSGL_point p2, u32 thickness) {
    return RSGL_drawLineF(renderer, (RSGL_pointF){(float)p1.x, (float)p1.y}, (RSGL_pointF){(float)p2.x, (float)p2.y}, thickness);
}

i32 RSGL_drawTriangleOutline(RSGL_renderer* renderer, RSGL_triangle t, u32 thickness) {
    return RSGL_drawTriangleFOutline(renderer, RSGL_createTriangleF((float)t.p1.x, (float)t.p1.y, (float)t.p2.x, (float)t.p2.y, (float)t.p3.x, (float)t.p3.y), thickness);
}

i32 RSGL_drawRectOutline(RSGL_renderer* renderer, RSGL_rect r, u32 thickness) {
    return RSGL_drawRectFOutline(renderer, (RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, thickness);
}

i32 RSGL_drawRoundRectOutline(RSGL_renderer* renderer, RSGL_rect r, RSGL_point rounding, u32 thickness) {
    return RSGL_drawRoundRectFOutline(renderer, (RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, rounding, thickness);
}

i32 RSGL_drawPolygonOutline(RSGL_renderer* renderer, RSGL_rect r, u32 sides, u32 thickness) {
    return RSGL_drawPolygonFOutline(renderer, (RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, sides, thickness);
}

i32 RSGL_drawArcOutline(RSGL_renderer* renderer, RSGL_rect o, RSGL_point arc, u32 thickness) {
    return RSGL_drawArcFOutline(renderer, (RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, (RSGL_pointF){(float)arc.x, (float)arc.y}, thickness);
}

i32 RSGL_drawCircleOutline(RSGL_renderer* renderer, RSGL_circle c, u32 thickness) {
    return RSGL_drawCircleFOutline(renderer, (RSGL_circleF){(float)c.x, (float)c.y, (float)c.d}, thickness);
}

i32 RSGL_drawOvalOutline(RSGL_renderer* renderer, RSGL_rect o, u32 thickness) {
    return RSGL_drawOvalFOutline(renderer, (RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, thickness);
}

i32 RSGL_drawPointF(RSGL_renderer* renderer, RSGL_pointF p) {
    RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, RSGL_POINT3D(p.x, p.y, 0.0f));

    float points[] = {RSGL_GET_MATRIX_POINT((float)p.x, (float)p.y, 0.0f)};
    float texPoints[] = { 0.0f, 0.0f };
	u16 elements[] = { 0 };

	RSGL_rawVerts data;
	data.type = RSGL_POINTS;
	data.verts = points;
	data.texCoords = texPoints;
	data.elements = elements;
	data.elmCount = 1;
	data.vert_count = 1;

    return RSGL_drawRawVerts(renderer, &data);
}

i32 RSGL_drawTriangleF(RSGL_renderer* renderer, RSGL_triangleF t) {
    RSGL_point3D center = {t.p3.x, (t.p3.y + t.p1.y) / 2.0f, 0};
    RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, center);

    float points[] = {RSGL_GET_MATRIX_POINT((float)t.p1.x, (float)t.p1.y, 0.0f),
                      RSGL_GET_MATRIX_POINT((float)t.p2.x, (float)t.p2.y, 0.0f),
                      RSGL_GET_MATRIX_POINT((float)t.p3.x, (float)t.p3.y, 0.0f)};

    float texPoints[] = {
                0.0f, 1.0f,
                1.0f, 1.0f,
                ((float)(t.p3.x - t.p1.x)/t.p2.x < 1) ? (float)(t.p3.x - t.p1.x) / t.p2.x : 0, 0.0f,
    };

	u16 elements[] = {
		0, 1, 2,
	};

	RSGL_rawVerts data;
	data.type = RSGL_TRIANGLES;
	data.verts = points;
	data.texCoords = texPoints;
	data.elements = elements;
	data.elmCount = 3;
	data.vert_count = 3;

    return RSGL_drawRawVerts(renderer, &data);
}

i32 RSGL_drawTriangleHyp(RSGL_renderer* renderer, RSGL_pointF p, size_t angle, float hypotenuse) {
    float dir = (hypotenuse > 0);
    hypotenuse = fabsf(hypotenuse);

    float base = hypotenuse * (RSGL_COS(angle) * DEG2RAD);
    float opp = hypotenuse * (RSGL_SIN(angle) * DEG2RAD);

    RSGL_triangleF t = RSGL_TRIANGLEF(
        p,
        RSGL_POINTF(p.x + base, p.y),
        RSGL_POINTF(p.x + (base * dir), p.y - opp)
    );

    return RSGL_drawTriangleF(renderer, t);
}

i32 RSGL_drawRectF(RSGL_renderer* renderer, RSGL_rectF r) {
    float texPoints[] = {
                                0.0f, 0.0f,
                                0.0f, 1.0f,
                                1.0f, 0.0f,
                                1.0f, 1.0f,
                            };

    RSGL_point3D center = (RSGL_point3D){r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f};
    RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, center);

    float points[] = {
                                RSGL_GET_MATRIX_POINT(r.x, r.y, 0.0f),
                                RSGL_GET_MATRIX_POINT(r.x, r.y + r.h, 0.0f),
                                RSGL_GET_MATRIX_POINT(r.x + r.w, r.y, 0.0f),
                                RSGL_GET_MATRIX_POINT(r.x + r.w, r.y + r.h, 0.0f),
                            };

	u16 elements[] = {
		0, 1, 2,
		3, 2, 1
	};

	RSGL_rawVerts data;
	data.type = RSGL_TRIANGLES;
	data.verts = points;
	data.texCoords = texPoints;
	data.elements = elements;
	data.elmCount = 6;
	data.vert_count = 4;

	return RSGL_drawRawVerts(renderer, &data);
}

i32 RSGL_drawRoundRectF(RSGL_renderer* renderer, RSGL_rectF r, RSGL_point rounding) {
    RSGL_drawRect(renderer, RSGL_RECT(r.x + (rounding.x / 2), r.y, r.w - rounding.x, r.h));
    RSGL_drawRect(renderer, RSGL_RECT(r.x, r.y + (rounding.y / 2), r.w,  r.h - rounding.y));

    RSGL_drawArc(renderer, RSGL_RECT(r.x, r.y, rounding.x, rounding.y), (RSGL_point){180, 270});
    RSGL_drawArc(renderer, RSGL_RECT(r.x + (r.w - rounding.x), r.y, rounding.x, rounding.y), (RSGL_point){90, 180});
    RSGL_drawArc(renderer, RSGL_RECT(r.x + (r.w - rounding.x), r.y  + (r.h - rounding.y), rounding.x, rounding.y), (RSGL_point){0, 90});
	return RSGL_drawArc(renderer, RSGL_RECT(r.x, r.y  + (r.h - rounding.y),  rounding.x, rounding.y), (RSGL_point){270, 360});
}

i32 RSGL_drawPolygonFOutlineEx(RSGL_renderer* renderer, RSGL_rectF o, u32 sides, RSGL_pointF arc);

i32 RSGL_drawPolygonFEx(RSGL_renderer* renderer, RSGL_rectF o, u32 sides, RSGL_pointF arc) {
	static float verts[360 * 3];
    static float texcoords[360 * 2];
	static u16 elements[360 * 6];

    RSGL_point3D center =  (RSGL_point3D){o.x + (o.w / 2.0f), o.y + (o.h / 2.0f), 0};

	o = (RSGL_rectF){o.x, o.y, o.w / 2, o.h / 2};
    RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, center);

    float displacement = 360.0f / (float)sides;
    float angle = displacement * arc.x;

    size_t vIndex = 0;
    size_t tIndex = 0;
	size_t iIndex = 0;

	{
        RSGL_pointF p = {center.x, center.y};

        texcoords[tIndex] = 0.5;
        texcoords[tIndex + 1] = 0.5;
        float temp[3] = { RSGL_GET_MATRIX_POINT(p.x, p.y, 0.0) };
        memcpy(verts + vIndex, temp, 3 * sizeof(float));

		angle += displacement;
        tIndex += 2;
        vIndex += 3;
	}

    u32 i;
    for (i = 0; i < sides + 1; i++) {
		RSGL_pointF p = {RSGL_SIN(angle * DEG2RAD), RSGL_COS(angle * DEG2RAD)};

        texcoords[tIndex] = (p.x + 1.0f) * 0.5;
        texcoords[tIndex + 1] = (p.y + 1.0f) * 0.5;

        float temp[3] = { RSGL_GET_MATRIX_POINT(o.x + o.w + (p.x * o.w), o.y + o.h + (p.y * o.h), 0.0) };
        memcpy(&verts[vIndex], temp, 3 * sizeof(float));

		elements[iIndex + 0] = i;

		if (i < sides)
			elements[iIndex + 1] = i + 1;
		else
			elements[iIndex + 1] = 1;

		elements[iIndex + 2] = 0;

		angle += displacement;
        tIndex += 2;
        vIndex += 3;
		iIndex += 3;
    }

	RSGL_rawVerts data;
	data.type = RSGL_TRIANGLES;
	data.verts = verts;
	data.texCoords = texcoords;
	data.elements = elements;
	data.elmCount = iIndex;
	data.vert_count = (vIndex / 3);

    i32 out = RSGL_drawRawVerts(renderer, &data);
	return out;
}

i32 RSGL_drawPolygonF(RSGL_renderer* renderer, RSGL_rectF o, u32 sides) { return RSGL_drawPolygonFEx(renderer, o, sides, (RSGL_pointF){0, (float)sides}); }


i32 RSGL_drawArcF(RSGL_renderer* renderer, RSGL_rectF o, RSGL_pointF arc) {
    u32 verts = (u32)((float)((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10) + 0.5);
    verts %= 360;

    return RSGL_drawPolygonFEx(renderer, o, verts, arc);
}

i32 RSGL_drawCircleF(RSGL_renderer* renderer, RSGL_circleF c) {
    u32 verts = (u32)((2.0 * M_PI * c.d) / 10) % 360;

    return RSGL_drawPolygonFEx(renderer, (RSGL_rectF){c.x, c.y, c.d, c.d}, verts, (RSGL_pointF){0, (float)verts});
}

i32 RSGL_drawOvalF(RSGL_renderer* renderer, RSGL_rectF o) {
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    return RSGL_drawPolygonFEx(renderer, o, verts, (RSGL_pointF){0, verts});
}

/*
    3D
*/

i32 RSGL_drawPoint3D(RSGL_renderer* renderer, RSGL_point3D p) {
    RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, p);

    float points[] = {RSGL_GET_MATRIX_POINT((float)p.x, (float)p.y, (float)p.z)};
    float texPoints[] = { 0.0f, 0.0f };
	u16 elements[] = { 0, };

	RSGL_rawVerts data;
	data.type = RSGL_POINTS;
	data.verts = points;
	data.texCoords = texPoints;
	data.elements = elements;
	data.elmCount = 1;
	data.vert_count = 1;

    return RSGL_drawRawVerts(renderer, &data);
}

i32 RSGL_drawLine3D(RSGL_renderer* renderer, RSGL_point3D p1, RSGL_point3D p2, u32 thickness) {
    renderer->state.lineWidth = thickness;

    RSGL_point3D center = {(p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f, (p1.z + p2.z) / 2.0f};
    RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, center);

    float points[] = {RSGL_GET_MATRIX_POINT(p1.x, p1.y, p1.z), RSGL_GET_MATRIX_POINT(p2.x, p2.y, p2.z)};
    float texPoints[] = {0, 0.0f,          0, 0.0f};
	u16 elements[] = { 0, 1,    2, 3 };

	RSGL_rawVerts data;
	data.type = RSGL_LINES;
	data.verts = points;
	data.texCoords = texPoints;
	data.elements = elements;
	data.elmCount = 4;
	data.vert_count = 2;

	return RSGL_drawRawVerts(renderer, &data);
}

i32 RSGL_drawTriangle3D(RSGL_renderer* renderer, RSGL_triangle3D t) {
    RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, RSGL_POINT3D(200, 400, 1));

    float points[] = {
            RSGL_GET_MATRIX_POINT(t.p1.x, t.p1.y, t.p1.z),
            RSGL_GET_MATRIX_POINT(t.p2.x, t.p2.y, t.p2.z),
            RSGL_GET_MATRIX_POINT(t.p3.x, t.p3.y, t.p3.z)
    };

    float texPoints[] = {
                0.0f, 1.0f,
                1.0f, 1.0f,
                ((float)(points[6] - points[0])/points[3]< 1) ? (float)(points[6] - points[0]) / points[3] : 0, 0.0f,
    };

	u16 elements[] = {
		0, 1, 2,
	};

	RSGL_rawVerts data;
	data.type = RSGL_TRIANGLES;
	data.verts = points;
	data.texCoords = texPoints;
	data.elements = elements;
	data.elmCount = 3;
	data.vert_count = 2;

    return RSGL_drawRawVerts(renderer, &data);
}

i32 RSGL_drawCube(RSGL_renderer* renderer, RSGL_cube cube) {
    float texPoints[] = {
        0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
        1.0f, 1.0f,  1.0f, 0.0f,  0.0f, 1.0f,

        0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
        1.0f, 1.0f,  1.0f, 0.0f,  0.0f, 1.0f,

        0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
        1.0f, 1.0f,  1.0f, 0.0f,  0.0f, 1.0f,

        0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
        1.0f, 1.0f,  1.0f, 0.0f,  0.0f, 1.0f,

        0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
        1.0f, 1.0f,  1.0f, 0.0f,  0.0f, 1.0f,

        0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
        1.0f, 1.0f,  1.0f, 0.0f,  0.0f, 1.0f
    };

    RSGL_point3D center = {
        cube.x + (cube.w / 2.0f),
        cube.y + (cube.h / 2.0f),
        cube.z + (cube.l / 2.0f)
    };

    RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, center);

    float points[] = {
        // Front face
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y,         cube.z),
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y + cube.h, cube.z),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y,         cube.z),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y + cube.h, cube.z),
        // Back face
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y,         cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y + cube.h, cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y,         cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y + cube.h, cube.z + cube.l),
        // Left face
        RSGL_GET_MATRIX_POINT(cube.x, cube.y,         cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x, cube.y + cube.h, cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x, cube.y,         cube.z),
        RSGL_GET_MATRIX_POINT(cube.x, cube.y + cube.h, cube.z),
        // Right face
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y,         cube.z),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y + cube.h, cube.z),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y,         cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y + cube.h, cube.z + cube.l),
        // Top face
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y + cube.h, cube.z),
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y + cube.h, cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y + cube.h, cube.z),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y + cube.h, cube.z + cube.l),
        // Bottom face
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y, cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y, cube.z),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y, cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y, cube.z),
    };

	u16 elements[] = {
		0, 1, 2,
		3, 2, 1,

		4, 5, 6,
		7, 6, 5,

		8, 9, 10,
		11, 10, 9,

		12, 13, 14,
		15, 14, 13,

		16, 17, 18,
		19, 18, 17,

		20, 21, 22,
		23, 21, 22,
	};

	RSGL_rawVerts data;
	data.type = RSGL_TRIANGLES;
	data.verts = points;
	data.texCoords = texPoints;
	data.elements = elements;
	data.elmCount = sizeof(elements) / sizeof(u16);
	data.vert_count = sizeof(points) / sizeof(float) / 3;

	return RSGL_drawRawVerts(renderer, &data);
}

/*
outlines
*/

i32 RSGL_drawLineF(RSGL_renderer* renderer, RSGL_pointF p1, RSGL_pointF p2, u32 thickness) {
    renderer->state.lineWidth = thickness;

    RSGL_point3D center = {(p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f, 0.0f};
    RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, center);

    float points[] = {RSGL_GET_MATRIX_POINT(p1.x, p1.y, 0.0f), RSGL_GET_MATRIX_POINT(p2.x, p2.y, 0.0f)};
    float texPoints[] = {0, 0.0f,          0, 0.0f};


	u16 elements[] = {
		0, 1,
		2, 3,
	};

	RSGL_rawVerts data;
	data.type = RSGL_LINES;
	data.verts = points;
	data.texCoords = texPoints;
	data.elements = elements;
	data.elmCount = 4;
	data.vert_count = 2;

	return RSGL_drawRawVerts(renderer, &data);
}

i32 RSGL_drawTriangleFOutline(RSGL_renderer* renderer, RSGL_triangleF t, u32 thickness) {
    renderer->state.lineWidth = thickness;
    RSGL_point3D center = {t.p3.x, (t.p3.y + t.p1.y) / 2.0f, 0};
    RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, center);

    float points[] = {RSGL_GET_MATRIX_POINT(t.p3.x, t.p3.y, 0.0f),
                        RSGL_GET_MATRIX_POINT(t.p1.x, t.p1.y, 0.0f),
                        RSGL_GET_MATRIX_POINT(t.p1.x, t.p1.y, 0.0f),
                    RSGL_GET_MATRIX_POINT(t.p2.x, t.p2.y, 0.0f),
                    RSGL_GET_MATRIX_POINT(t.p2.x, t.p2.y, 0.0f),
                    RSGL_GET_MATRIX_POINT(t.p3.x, t.p3.y, 0.0f)};

    float texCoords[18];

	u16 elements[] = {
		0, 1, 2,
		3, 2, 1
	};

	RSGL_rawVerts data;
	data.type = RSGL_LINES;
	data.verts = points;
	data.texCoords = texCoords;
	data.elements = elements;
	data.elmCount = 6;
	data.vert_count = 6;

    return RSGL_drawRawVerts(renderer, &data);
}
i32 RSGL_drawRectFOutline(RSGL_renderer* renderer, RSGL_rectF r, u32 thickness) {
    RSGL_renderer_setCenter(renderer, (RSGL_point3D){r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f});
    RSGL_drawLineF(renderer, (RSGL_pointF){r.x, r.y}, (RSGL_pointF){r.x + r.w, r.y}, thickness);

    RSGL_renderer_setCenter(renderer, (RSGL_point3D){r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f});
    RSGL_drawLineF(renderer, (RSGL_pointF){r.x, r.y}, (RSGL_pointF){r.x, r.y + r.h}, thickness);

    RSGL_renderer_setCenter(renderer, (RSGL_point3D){r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f});
    RSGL_drawLineF(renderer, (RSGL_pointF){r.x, r.y + r.h}, (RSGL_pointF){r.x + r.w, r.y + r.h}, thickness);

    RSGL_renderer_setCenter(renderer, (RSGL_point3D){r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f});
    return RSGL_drawLineF(renderer, (RSGL_pointF){r.x + r.w, r.y}, (RSGL_pointF){r.x + r.w, r.y + r.h}, thickness);
}
i32 RSGL_drawRoundRectFOutline(RSGL_renderer* renderer, RSGL_rectF r, RSGL_point rounding, u32 thickness) {
    RSGL_drawRectF(renderer, RSGL_RECTF(r.x + (rounding.x/2), r.y, r.w - rounding.x, (int)(thickness + !thickness)));
    RSGL_drawRectF(renderer, RSGL_RECTF(r.x + (rounding.x/2), r.y + r.h, r.w - rounding.x, (int)(thickness + !thickness)));
    RSGL_drawRectF(renderer, RSGL_RECTF(r.x, r.y + (rounding.y/2), (int)(thickness + !thickness),  r.h - rounding.y));
    RSGL_drawRectF(renderer, RSGL_RECTF(r.x + r.w, r.y + (rounding.y/2), (int)(thickness + !thickness),  r.h - rounding.y));

    RSGL_drawArcFOutline(renderer, RSGL_RECTF(r.x, r.y, rounding.x, rounding.y), (RSGL_pointF){180, 270}, thickness);
    RSGL_drawArcFOutline(renderer, RSGL_RECTF(r.x + (r.w - rounding.x), r.y, rounding.x, rounding.y), (RSGL_pointF){90, 180}, thickness);
    RSGL_drawArcFOutline(renderer, RSGL_RECTF(r.x + (r.w - rounding.x), r.y  + (r.h - rounding.y) - 1, rounding.x, rounding.y + 2), (RSGL_pointF){0, 90}, thickness);
    return RSGL_drawArcFOutline(renderer, RSGL_RECTF(r.x + 1, r.y  + (r.h - rounding.y) - 1,  rounding.x, rounding.y + 2), (RSGL_pointF){270, 360}, thickness);
}

i32 RSGL_drawPolygonFOutlineEx(RSGL_renderer* renderer, RSGL_rectF o, u32 sides, RSGL_pointF arc) {
    static float verts[360 * 2 * 3];
    static float texCoords[360 * 2 * 2];

    RSGL_point3D center = (RSGL_point3D) {o.x + (o.w / 2.0f), o.y + (o.h / 2.0f), 0.0f};
        RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, center);

    o = (RSGL_rectF){o.x + (o.w / 2), o.y + (o.h / 2), o.w / 2, o.h / 2};

    float displacement = 360.0f / (float)sides;
    float centralAngle = displacement * arc.x;

    i32 i;
    u32 j;
    size_t index = 0;

    for (i = arc.x; i < arc.y; i++) {
        for (j = 0; j < 2; j++) {
            float temp[3] = {
                RSGL_GET_MATRIX_POINT(
                    o.x + (RSGL_SIN(DEG2RAD * centralAngle) * o.w),
                    o.y + (RSGL_COS(DEG2RAD * centralAngle) * o.h),
                    (0.0)
                )
            };
            memcpy(verts + index, temp, sizeof(float) * 3);

            if (!j) centralAngle += displacement;
            index += 3;
        }
    }

	u16 elements[] = {
		0, 1, 2,
		3, 2, 1
	};

	RSGL_rawVerts data;
	data.type = RSGL_LINES;
	data.verts = verts;
	data.texCoords = texCoords;
	data.elements = elements;
	data.elmCount = 6;
	data.vert_count = 6;

    return RSGL_drawRawVerts(renderer, &data);
}

i32 RSGL_drawPolygonFOutline(RSGL_renderer* renderer, RSGL_rectF o, u32 sides, u32 thickness) {
    renderer->state.lineWidth = thickness;
    return RSGL_drawPolygonFOutlineEx(renderer, o, sides, (RSGL_pointF){0, (float)sides});
}
i32 RSGL_drawArcFOutline(RSGL_renderer* renderer, RSGL_rectF o, RSGL_pointF arc, u32 thickness) {
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    renderer->state.lineWidth = thickness;
    return RSGL_drawPolygonFOutlineEx(renderer, o, verts, arc);
}
i32 RSGL_drawCircleFOutline(RSGL_renderer* renderer, RSGL_circleF c, u32 thickness) {
    float verts = ((2 * M_PI * c.d) / 10);
    verts = (verts > 360 ? 360 : verts);

    renderer->state.lineWidth = thickness;
    return RSGL_drawPolygonFOutlineEx(renderer, (RSGL_rectF){c.x, c.y, c.d, c.d}, verts, (RSGL_pointF){0, verts});
}
i32 RSGL_drawOvalFOutline(RSGL_renderer* renderer, RSGL_rectF o, u32 thickness) {
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    renderer->state.lineWidth = thickness;
    return RSGL_drawPolygonFOutlineEx(renderer, o, verts, (RSGL_pointF){0, verts});
}

/*
******
RSGL_view
******
*/

/* Multiply the current matrix by a rotation matrix */
RSGL_mat4 RSGL_projection_getMatrix(const RSGL_projection* projection) {
    RSGL_mat4 matrix = RSGL_mat4_loadIdentity();
	switch (projection->type) {
		case RSGL_projectionPerspective3D:
			matrix = RSGL_mat4_perspective(matrix.m, projection->p3D.fov, projection->p3D.ratio, projection->p3D.pNear, projection->p3D.pFar);
			break;
		case RSGL_projectionOrtho3D: {
			double top = projection->p3D.fov / 2.0;
			double right = top * projection->p3D.ratio;

			matrix = RSGL_mat4_ortho(matrix.m, -right, right, -top, top, projection->p3D.pNear, projection->p3D.pFar);
			break;
		}
		case RSGL_projectionOrtho2D:
			matrix = RSGL_mat4_ortho(matrix.m, 0, projection->p2D.width, projection->p2D.height, 0, 0, 1.0);
			break;
		default: break;
	}

    return matrix;
}


RSGL_mat4 RSGL_view_getMatrix(const RSGL_view* view) {
    RSGL_mat4 matrix = RSGL_mat4_loadIdentity();
	switch (view->type) {
		case RSGL_viewType2D:
			matrix = RSGL_mat4_translate(matrix.m, -view->view2D.target.x, -view->view2D.target.y, -view->view2D.target.z);
			matrix = RSGL_mat4_rotate(matrix.m, view->view2D.rotation, 0, 0, 1);
			matrix = RSGL_mat4_scale(matrix.m, view->view2D.zoom, view->view2D.zoom, 1.0f);
			matrix = RSGL_mat4_translate(matrix.m, view->view2D.offset.x, view->view2D.offset.y, view->view2D.offset.z);
			break;
		case RSGL_viewType3D:
			matrix = RSGL_mat4_lookAt(matrix.m, view->view3D.pos.x, view->view3D.pos.y, view->view3D.pos.z, view->view3D.target.x, view->view3D.target.y, view->view3D.target.z,
										view->view3D.up.x, view->view3D.up.y, view->view3D.up.z);
			break;
		default: break;
	}

	return matrix;
}

/*
******
RSGL_Matrix
******
*/


RSGL_mat4 RSGL_mat4_lookAt(float matrix[16], float eyeX, float eyeY, float eyeZ, float targetX, float targetY, float targetZ, float upX, float upY, float upZ)  {
	float matLookAt[16];

    float length = 0.0f;
    float ilength = 0.0f;

    RSGL_point3D vz = { eyeX - targetX, eyeY - targetY, eyeZ - targetZ };

    RSGL_point3D v = vz;
    length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f/length;
    vz.x *= ilength;
    vz.y *= ilength;
    vz.z *= ilength;

    RSGL_point3D vx = { upY*vz.z - upZ*vz.y, upZ*vz.x - upX*vz.z, upX*vz.y - upY*vz.x };

    v = vx;
    length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f/length;
    vx.x *= ilength;
    vx.y *= ilength;
    vx.z *= ilength;

    RSGL_point3D vy = { vz.y*vx.z - vz.z*vx.y, vz.z*vx.x - vz.x*vx.z, vz.x*vx.y - vz.y*vx.x };

    matLookAt[0] = vx.x;
    matLookAt[1] = vy.x;
    matLookAt[2] = vz.x;
    matLookAt[3] = 0.0f;
    matLookAt[4] = vx.y;
    matLookAt[5] = vy.y;
    matLookAt[6] = vz.y;
    matLookAt[7] = 0.0f;
    matLookAt[8] = vx.z;
    matLookAt[9] = vy.z;
    matLookAt[10] = vz.z;
    matLookAt[11] = 0.0f;
    matLookAt[12] = -(vx.x*eyeX + vx.y*eyeY + vx.z*eyeZ);
    matLookAt[13] = -(vy.x*eyeX + vy.y*eyeY + vy.z*eyeZ);
    matLookAt[14] = -(vz.x*eyeX + vz.y*eyeY + vz.z*eyeZ);
	matLookAt[15] = 1.0f;

    return RSGL_mat4_multiply(matrix, matLookAt);
}

RSGL_mat4 RSGL_mat4_ortho(float matrix[16], float left, float right, float bottom, float top, float znear, float zfar) {
    float rl = (float)(right - left);
    float tb = (float)(top - bottom);
    float fn = (float)(zfar - znear);

    float matOrtho[16] = {
        (2.0f / rl),                          0.0f,                             0.0f,                                 0.0f,
        0.0f,                                (2.0f / tb),                       0.0f,                                0.00,
        0.0f,                                 0.0f,                             (-2.0f / fn),                         0.0f,
        (-((float)left + (float)right) / rl), -((float)top + (float)bottom)/tb, (-((float)zfar + (float)znear) / fn), 1.0f
    };

    return RSGL_mat4_multiply(matrix, matOrtho);
}

RSGL_mat4 RSGL_mat4_scale(float matrix[16], float x, float y, float z) {
	RSGL_mat4 result;

	for (int i = 0; i < 16; ++i) {
		result.m[i] = matrix[i];
	}

	result.m[0] += matrix[0]*x + matrix[4]*y + matrix[8]*z;
	result.m[13] += matrix[1]*x + matrix[5]*y + matrix[9]*z;
	result.m[10] += matrix[2]*x + matrix[6]*y + matrix[10]*z;
	result.m[11] += matrix[2] + matrix[6] + matrix[10];

	return result;
}



/* Multiply the current matrix by a translation matrix */
RSGL_mat4 RSGL_mat4_translate(float matrix[16], float x, float y, float z) {
	RSGL_mat4 result;

	for (int i = 0; i < 16; ++i) {
		result.m[i] = matrix[i];
	}

	result.m[12] += matrix[0]*x + matrix[4]*y + matrix[8]*z;
	result.m[13] += matrix[1]*x + matrix[5]*y + matrix[9]*z;
	result.m[14] += matrix[2]*x + matrix[6]*y + matrix[10]*z;

	return result;
}

RSGL_mat4 RSGL_mat4_loadIdentity(void) {
    RSGL_mat4 matrix = (RSGL_mat4) {
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    };

    return matrix;
}

RSGL_mat4 RSGL_mat4_rotate(float matrix[16], float angle, float x, float y, float z) {
	/* Axis vector (x, y, z) normalization */
	float lengthSquared = x * x + y * y + z * z;
	if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f)) {
		float inverseLength = 1.0f / sqrtf(lengthSquared);
		x *= inverseLength;
		y *= inverseLength;
		z *= inverseLength;
	}

	/* Rotation matrix generation */
	float sinres = RSGL_SIN(angle);
	float cosres = RSGL_COS(angle);
	float t = 1.0f - cosres;

	float matRotation[16] =  {
            x * x * t + cosres,   	  	y * x * t + z * sinres,   	z * x * t - y * sinres,   	0.0f,
            x * y * t - z * sinres,   	y * y * t + cosres,   		z * y * t + x * sinres,   	0.0f,
            x * z * t + y * sinres,   	y * z * t - x * sinres,  	z * z * t + cosres,   		0.0f,
            0.0f,   					0.0f,   					0.0f,   					1.0f
	};

	return RSGL_mat4_multiply(matRotation, matrix);
}

RSGL_mat4 RSGL_mat4_perspective(float matrix[16], float fovY, float aspect, float zNear, float zFar) {
    fovY =  (fovY * DEG2RAD) / 2.0f;
    const float f = (RSGL_COS(fovY) / RSGL_SIN(fovY));

    float perspective[16] = {
            (f / aspect), 0.0f,  0.0f,                                   0.0f,
            0,            f,     0.0f,                                   0.0f,
            0.0f,         0.0f,  (zFar + zNear) / (zNear - zFar),       -1.0f,
            0.0f,         0.0f,  (2.0f * zFar * zNear) / (zNear - zFar),  0.0f
    };

	return RSGL_mat4_multiply(matrix, perspective);
}

RSGL_mat4 RSGL_mat4_multiply(float left[16], float right[16]) {
    return (RSGL_mat4) {
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
#endif /* RSGL_IMPLEMENTATION */
