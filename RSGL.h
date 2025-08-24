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

    #define RSGL_NO_TEXT - do not include text rendering functions
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

/*
RSGL basicDraw types
*/

#define RSGL_LINES                                0x0001
#define RSGL_TRIANGLES                            0x0004
#define RSGL_TRIANGLE_FAN                         0x0006

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
#define RSGL_POINT(x, y) (RSGL_point){x, y}

#ifndef RSGL_area
typedef struct RSGL_area {
    u32 w, h;
} RSGL_area;
#endif
#define RSGL_AREA(w, h) (RSGL_area){w, h}

#ifndef RSGL_pointF
typedef struct RSGL_pointF { float x, y; } RSGL_pointF;
#endif
#define RSGL_POINTF(x, y) (RSGL_pointF){x, y}

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


#define RFONT_RENDERER_H
#define RFont_texture RSGL_texture

typedef struct RFont_renderer_proc {
	size_t (*size)(void); /*!< get the size of the renderer context */
	void (*initPtr)(void* ctx); /* any initalizations the renderer needs to do */
	RFont_texture (*create_atlas)(void* ctx, u32 atlasWidth, u32 atlasHeight); /* create a bitmap texture based on the given size */
	void (*free_atlas)(void* ctx, RFont_texture atlas);
	void (*bitmap_to_atlas)(void* ctx, RFont_texture atlas, u32 atlasWidth, u32 atlasHeight, u32 maxHeight, u8* bitmap, float w, float h, float* x, float* y); /* add the given bitmap to the texture based on the given coords and size data */
	void (*render)(void* ctx, RFont_texture atlas, float* verts, float* tcoords, size_t nverts); /* render the text, using the vertices, atlas texture, and texture coords given. */
	void (*set_framebuffer)(void* ctx, u32 weight, u32 height); /*!< set the frame buffer size (for ortho, for example) */
	void (*set_color)(void* ctx, float r, float g, float b, float a); /*!< set the current rendering color */
	void (*freePtr)(void* ctx); /* free any memory the renderer might need to free */
} RFont_renderer_proc;

typedef struct RFont_renderer {
	void* ctx; /*!< source renderer data */
	RFont_renderer_proc proc;
} RFont_renderer;

typedef struct RFont_font RFont_font;
typedef RFont_font RSGL_font;

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

RSGLDEF RSGL_mat4 RSGL_loadIdentity(void);
RSGLDEF RSGL_mat4 RSGL_rotate(float matrix[16], float angle, float x, float y, float z);
RSGLDEF RSGL_mat4 RSGL_translate(float matrix[16], float x, float y, float z);
RSGLDEF RSGL_mat4 RSGL_perspective(float matrix[16], float fovY, float aspect, float zNear, float zFar);
RSGLDEF RSGL_mat4 RSGL_ortho(float matrix[16], float left, float right, float bottom, float top, float znear, float zfar);

RSGLDEF RSGL_mat4 RSGL_mat4Multiply(float left[16], float right[16]);

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

/* custom shader program */
#ifndef RSGL_programInfo
typedef struct RSGL_programInfo {
    size_t program;
	RSGL_shaderType type;
} RSGL_programInfo;
#endif

typedef struct RSGL_BATCH {
    size_t start, len; /* when batch starts and it's length */
    u32 type;
    RSGL_texture tex;
    float lineWidth;
    RSGL_mat4 matrix;
} RSGL_BATCH; /* batch data type for rendering */

typedef struct RSGL_RENDER_INFO {
    RSGL_BATCH batches[RSGL_MAX_BATCHES];
    float verts[RSGL_MAX_VERTS * 2];
    float texCoords[RSGL_MAX_VERTS * 4];
    float colors[RSGL_MAX_VERTS * 2];

    size_t len; /* number of batches*/
    size_t vert_len; /* number of verts */
    RSGL_mat4 matrix;
} RSGL_RENDER_INFO; /* render data */

typedef struct RSGL_renderState {
    float* gradient; /* does not allocate any memory */

    RSGL_texture texture;
    u32 gradient_len;

	RSGL_color color;

    RSGL_area currentArea; /* size of current surface */
    RSGL_point3D rotate;
	RSGL_programInfo program;

    RSGL_point3D center;
    float lineWidth;
    RSGL_mat4 matrix;
	RSGL_mat4 customMatrix;
	RSGL_bool forceBatch;
	RSGL_font* font;
} RSGL_renderState;

typedef struct RSGL_rendererProc {
	size_t (*size)(void); /* get the size of the renderer's internal struct */
	void (*initPtr)(void* ctx, void* proc); /* init render backend */
	void (*freePtr)(void* ctx); /* free render backend */
	void (*render)(void* ctx, RSGL_programInfo program, RSGL_RENDER_INFO* info);
	void (*clear)(void* ctx, float r, float g, float b, float a);
	void (*viewport)(void* ctx, i32 x, i32 y, i32 w, i32 h);
	RSGL_texture (*createTexture)(void* ctx, u8* bitmap, RSGL_area memsize,  u8 channels);
	void (*updateTexture)(void* ctx, RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels);
	void (*deleteTexture)(void* ctx, RSGL_texture tex);
	void (*scissorStart)(void* ctx, RSGL_rectF scissor, i32 renderer_height);
	void (*scissorEnd)(void* ctx);
	RSGL_programInfo (*createProgram)(void* ctx, const char* VShaderCode, const char* FShaderCode, const char* posName, const char* texName, const char* colorName);
	void (*deleteProgram)(void* ctx, RSGL_programInfo program);
	void (*setShaderValue)(void* ctx, u32 program, char* var, float value[], u8 len);
	RSGL_texture (*createAtlas)(void* ctx,u32 atlasWidth, u32 atlasHeight);
	void (*bitmapToAtlas)(void* ctx, RSGL_texture atlas, u32 atlasWidth, u32 atlasHeight, u32 maxHeight, u8* bitmap, float w, float h, float* x, float* y);
	RSGL_programInfo (*createComputeProgram)(void* ctx, const char* CShaderCode);
	void (*dispatchComputeProgram)(void* ctx, RSGL_programInfo program, u32 groups_x, u32 groups_y, u32 groups_z);
	void (*bindComputeTexture)(void* ctx, u32 texture, u8 format);
} RSGL_rendererProc;

typedef struct RSGL_renderer {
	RSGL_RENDER_INFO info;
	RSGL_renderState state;
	RSGL_rendererProc proc;
	RFont_renderer rfont;
	void* userPtr;
	void* ctx; /* pointer for the renderer backend to store any internal data it wants/needs  */
} RSGL_renderer;

RSGLDEF void RSGL_renderer_getRenderState(RSGL_renderer* renderer, RSGL_renderState* state);

RSGLDEF size_t RSGL_renderer_size(RSGL_renderer* renderer);

RSGLDEF void RSGL_renderer_initPtr(RSGL_rendererProc proc,
                        RSGL_area r, /* graphics context size */
                        void* loader, /* opengl prozc address ex. wglProcAddress */
						void* ptr, /* pointer to allocate backend data */
					    RSGL_renderer* renderer
					);

RSGLDEF RSGL_renderer* RSGL_renderer_init(RSGL_rendererProc proc, RSGL_area r, void* loader);
RSGLDEF void RSGL_renderer_updateSize(RSGL_renderer* renderer, RSGL_area r);
RSGLDEF void RSGL_renderer_render(RSGL_renderer* renderer); /* draw current batch */
RSGLDEF void RSGL_renderer_freePtr(RSGL_renderer* renderer);

RSGLDEF void RSGL_renderer_free(RSGL_renderer* renderer);

RSGLDEF void RSGL_renderer_setRotate(RSGL_renderer* renderer, RSGL_point3D rotate); /* apply rotation to drawing */
RSGLDEF void RSGL_renderer_setTexture(RSGL_renderer* renderer, RSGL_texture texture); /* apply texture to drawing */
RSGLDEF void RSGL_renderer_setColor(RSGL_renderer* renderer, RSGL_color color); /* apply color to drawing */
RSGLDEF void RSGL_renderer_setProgram(RSGL_renderer* renderer, const RSGL_programInfo* program); /* use shader program for drawing */
RSGLDEF void RSGL_renderer_setGradient(RSGL_renderer* renderer,
                                float* gradient, /* array of gradients */
                                size_t len /* length of array */
                            ); /* apply gradient to drawing, based on color list*/
RSGLDEF void RSGL_renderer_setCenter(RSGL_renderer* renderer, RSGL_point3D center); /* the center of the drawing (or shape), this is used for rotation */
/* args clear after a draw function by default, this toggles that */
RSGLDEF void RSGL_renderer_clearArgs(RSGL_renderer* renderer); /* clears the args */

RSGLDEF RSGL_mat4 RSGL_renderer_initDrawMatrix(RSGL_renderer* renderer, RSGL_point3D center);

/* renders the current batches */
RSGLDEF void RSGL_renderer_clear(RSGL_renderer* renderer, RSGL_color color);
RSGLDEF void RSGL_renderer_viewport(RSGL_renderer* renderer, RSGL_rect rect);
/* create a texture based on a given bitmap, this must be freed later using RSGL_deleteTexture or opengl*/
RSGLDEF RSGL_texture RSGL_renderer_createTexture(RSGL_renderer* renderer, u8* bitmap, RSGL_area memsize,  u8 channels);
/* updates an existing texture wiht a new bitmap */
RSGLDEF void RSGL_renderer_updateTexture(RSGL_renderer* renderer, RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels);
/* delete a texture */
RSGLDEF void RSGL_renderer_deleteTexture(RSGL_renderer* renderer, RSGL_texture tex);
/* starts scissoring */
RSGLDEF void RSGL_renderer_scissorStart(RSGL_renderer* renderer, RSGL_rectF scissor);
/* stops scissoring */
RSGLDEF void RSGL_renderer_scissorEnd(RSGL_renderer* renderer);
/* custom shader program */
RSGLDEF RSGL_programInfo RSGL_renderer_createProgram(RSGL_renderer* renderer, const char* VShaderCode, const char* FShaderCode, const char* posName, const char* texName, const char* colorName);
RSGLDEF void RSGL_renderer_deleteProgram(RSGL_renderer* renderer, RSGL_programInfo program);
RSGLDEF void RSGL_renderer_setShaderValue(RSGL_renderer* renderer, u32 program, char* var, float value[], u8 len);

RSGLDEF void RSGL_renderer_setMatrix(RSGL_renderer* renderer, RSGL_mat4 matrix);
RSGLDEF void RSGL_renderer_resetMatrix(RSGL_renderer* renderer);

RSGLDEF RSGL_programInfo RSGL_renderer_createComputeProgram(RSGL_renderer* renderer, const char *CShaderCode);
RSGLDEF void RSGL_renderer_dispatchComputeProgram(RSGL_renderer* renderer, RSGL_programInfo program, u32 groups_x, u32 groups_y, u32 groups_z);
RSGLDEF void RSGL_renderer_bindComputeTexture(RSGL_renderer* renderer, u32 texture, u8 format);

/*
*******
RSGL_font
*******
*/

#ifndef RSGL_NO_TEXT
RSGLDEF RSGL_font* RSGL_loadFont(RSGL_renderer* renderer, const char* font, size_t maxHeight, size_t atlasWidth, size_t atlasHeight);
RSGLDEF void RSGL_loadFontPtr(RSGL_renderer* renderer, const char* font, size_t maxHeight, size_t atlasWidth, size_t atlasHeight, RFont_font* ptr);

RSGLDEF void RSGL_renderer_setFont(RSGL_renderer* renderer, RSGL_font* font);

RSGLDEF void RSGL_font_freePtr(RSGL_renderer* renderer, RSGL_font* font);
RSGLDEF void RSGL_font_free(RSGL_renderer* renderer, RSGL_font* font);
#endif

/*
*******
RSGL_camera
*******
*/

/* RSGL translation */
#ifndef RSGL_camera
typedef struct RSGL_camera {
    RSGL_point3D pos;
    float fov;
    float pitch, yaw;
} RSGL_camera;
#endif

RSGLDEF RSGL_mat4 RSGL_renderer_getCameraMatrix(RSGL_renderer* renderer, RSGL_camera camera);
RSGLDEF RSGL_mat4 RSGL_renderer_getCameraMatrixEx(RSGL_renderer* renderer, RSGL_camera camera, float ratio, float maxPitch, float min, float max);

/*
*******
RSGL_draw
*******
*/

/*
RSGL_basicDraw is a function used internally by RSGL, but you can use it yourself
RSGL_basicDraw batches a given set of points based on the data to be rendered
*/
RSGLDEF void RSGL_basicDraw(RSGL_renderer* renderer,
                u32 TYPE, /* type of shape  RSGL_TRIANGLES, RSGL_LINES */
                float* points, /* array of 3D points */
                float* texPoints, /* array of 2D texture points (must be same length as points)*/
                size_t len /* the length of the points array */
            );

/* 2D shape drawing */
/* in the function names, F means float */

RSGLDEF void RSGL_drawPoint(RSGL_renderer* renderer, RSGL_point p);
RSGLDEF void RSGL_drawPointF(RSGL_renderer* renderer, RSGL_pointF p);

RSGLDEF void RSGL_drawTriangle(RSGL_renderer* renderer, RSGL_triangle t);
RSGLDEF void RSGL_drawTriangleF(RSGL_renderer* renderer, RSGL_triangleF t);

RSGLDEF void RSGL_drawTriangleHyp(RSGL_renderer* renderer, RSGL_pointF p, size_t angle, float hypotenuse);

RSGLDEF void RSGL_drawRect(RSGL_renderer* renderer, RSGL_rect r);
RSGLDEF void RSGL_drawRectF(RSGL_renderer* renderer, RSGL_rectF r);

RSGLDEF void RSGL_drawRoundRect(RSGL_renderer* renderer, RSGL_rect r, RSGL_point rounding);
RSGLDEF void RSGL_drawRoundRectF(RSGL_renderer* renderer, RSGL_rectF r, RSGL_point rounding);

RSGLDEF void RSGL_drawPolygon(RSGL_renderer* renderer, RSGL_rect r, u32 sides);
RSGLDEF void RSGL_drawPolygonF(RSGL_renderer* renderer, RSGL_rectF r, u32 sides);

RSGLDEF void RSGL_drawArc(RSGL_renderer* renderer, RSGL_rect o, RSGL_point arc);
RSGLDEF void RSGL_drawArcF(RSGL_renderer* renderer, RSGL_rectF o, RSGL_pointF arc);

RSGLDEF void RSGL_drawCircle(RSGL_renderer* renderer, RSGL_circle c);
RSGLDEF void RSGL_drawCircleF(RSGL_renderer* renderer, RSGL_circleF c);

RSGLDEF void RSGL_drawOval(RSGL_renderer* renderer, RSGL_rect o);
RSGLDEF void RSGL_drawOvalF(RSGL_renderer* renderer, RSGL_rectF o);

RSGLDEF void RSGL_drawLine(RSGL_renderer* renderer, RSGL_point p1, RSGL_point p2, u32 thickness);
RSGLDEF void RSGL_drawLineF(RSGL_renderer* renderer, RSGL_pointF p1, RSGL_pointF p2, u32 thickness);

/* 3D objects */
RSGLDEF void RSGL_drawTriangle3D(RSGL_renderer* renderer, RSGL_triangle3D t);
RSGLDEF void RSGL_drawPoint3D(RSGL_renderer* renderer, RSGL_point3D p);
RSGLDEF void RSGL_drawLine3D(RSGL_renderer* renderer, RSGL_point3D p1, RSGL_point3D p2, u32 thickness);
RSGLDEF void RSGL_drawCube(RSGL_renderer* renderer, RSGL_cube cube);

/* 2D outlines */

/* thickness means the thickness of the line */

RSGLDEF void RSGL_drawTriangleOutline(RSGL_renderer* renderer, RSGL_triangle t, u32 thickness);
RSGLDEF void RSGL_drawTriangleFOutline(RSGL_renderer* renderer, RSGL_triangleF t, u32 thickness);

RSGLDEF void RSGL_drawRectOutline(RSGL_renderer* renderer, RSGL_rect r, u32 thickness);
RSGLDEF void RSGL_drawRectFOutline(RSGL_renderer* renderer, RSGL_rectF r, u32 thickness);

RSGLDEF void RSGL_drawRoundRectOutline(RSGL_renderer* renderer, RSGL_rect r, RSGL_point rounding, u32 thickness);
RSGLDEF void RSGL_drawRoundRectFOutline(RSGL_renderer* renderer, RSGL_rectF r, RSGL_point rounding, u32 thickness);

RSGLDEF void RSGL_drawPolygonOutline(RSGL_renderer* renderer, RSGL_rect r, u32 sides, u32 thickness);
RSGLDEF void RSGL_drawPolygonFOutline(RSGL_renderer* renderer, RSGL_rectF r, u32 sides, u32 thickness);

RSGLDEF void RSGL_drawArcOutline(RSGL_renderer* renderer, RSGL_rect o, RSGL_point arc, u32 thickness);
RSGLDEF void RSGL_drawArcFOutline(RSGL_renderer* renderer, RSGL_rectF o, RSGL_pointF arc, u32 thickness);

RSGLDEF void RSGL_drawCircleOutline(RSGL_renderer* renderer, RSGL_circle c, u32 thickness);
RSGLDEF void RSGL_drawCircleFOutline(RSGL_renderer* renderer, RSGL_circleF c, u32 thickness);

RSGLDEF void RSGL_drawOvalFOutline(RSGL_renderer* renderer, RSGL_rectF o, u32 thickness);
RSGLDEF void RSGL_drawOvalOutline(RSGL_renderer* renderer, RSGL_rect o, u32 thickness);

#ifndef RSGL_NO_TEXT
RSGLDEF void RSGL_drawText_len(RSGL_renderer* renderer, const char* text, size_t len, RSGL_circle c);
RSGLDEF void RSGL_drawText_pro(RSGL_renderer* renderer, const char* text, size_t len, float spacing, RSGL_circle c);
RSGLDEF void RSGL_drawText(RSGL_renderer* renderer, const char* text, RSGL_circle c);

/*
    returns the width of a text when rendered with the set font with the size of `fontSize
    stops at `textEnd` or when it reaches '\0'
*/
RSGLDEF RSGL_area RSGL_renderer_textArea(RSGL_renderer* renderer, const char* text, u32 fontSize, size_t textEnd);
RSGLDEF RSGL_area RSGL_renderer_textLineArea(RSGL_renderer* renderer, const char* text, u32 fontSize, size_t textEnd, size_t line);
#endif /* RSGL_NO_TEXT */
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

#ifdef RSGL_RENDER_LEGACY
#define RFONT_RENDER_LEGACY
#endif

void RSGL_renderer_setMatrix(RSGL_renderer* renderer, RSGL_mat4 matrix) {
    renderer->state.customMatrix = matrix;
    renderer->state.forceBatch = RSGL_TRUE;
}

void RSGL_renderer_resetMatrix(RSGL_renderer* renderer) {
    renderer->state.customMatrix = RSGL_loadIdentity();
    renderer->state.forceBatch = RSGL_TRUE;
}

void RSGL_renderer_getRenderState(RSGL_renderer* renderer, RSGL_renderState* state) {
	if (state) *state = renderer->state;
}

RSGL_mat4 RSGL_renderer_initDrawMatrix(RSGL_renderer* renderer, RSGL_point3D center) {
    RSGL_mat4 matrix = RSGL_loadIdentity();

    if (renderer->state.rotate.x || renderer->state.rotate.y || renderer->state.rotate.z) {
        if (renderer->state.center.x != -1 && renderer->state.center.y != -1 &&  renderer->state.center.z != -1)
            center = renderer->state.center;

        matrix = RSGL_translate(matrix.m, center.x, center.y, center.z);
        matrix = RSGL_rotate(matrix.m, renderer->state.rotate.z,  0, 0, 1);
        matrix = RSGL_rotate(matrix.m, renderer->state.rotate.y, 0, 1, 0);
        matrix = RSGL_rotate(matrix.m, renderer->state.rotate.x, 1, 0, 0);
        matrix = RSGL_translate(matrix.m, -center.x, -center.y, -center.z);
    }

    return matrix;
}

void RSGL_basicDraw(RSGL_renderer* renderer, u32 type, float* points, float* texPoints, size_t len) {
    if (renderer->info.len + 1 >= RSGL_MAX_BATCHES || renderer->info.vert_len + len >= RSGL_MAX_VERTS) {
        RSGL_renderer_render(renderer);
    }

    RSGL_BATCH* batch = NULL;
	RSGL_color c = renderer->state.color;

    if (
        renderer->info.len == 0 ||
        renderer->info.batches[renderer->info.len - 1].tex != renderer->state.texture  ||
        renderer->info.batches[renderer->info.len - 1].lineWidth != renderer->state.lineWidth ||
        renderer->info.batches[renderer->info.len - 1].type != type ||
        renderer->info.batches[renderer->info.len - 1].type == RSGL_TRIANGLE_FAN ||
        renderer->state.forceBatch
    ) {
        renderer->state.forceBatch = RSGL_FALSE;
        renderer->info.len += 1;

        batch = &renderer->info.batches[renderer->info.len - 1];
        batch->start = renderer->info.vert_len;
        batch->len = 0;
        batch->type = type;
        batch->tex = renderer->state.texture;
        batch->lineWidth = renderer->state.lineWidth;

        batch->matrix = renderer->info.matrix;
        batch->matrix = RSGL_mat4Multiply(batch->matrix.m, renderer->state.customMatrix.m);
    } else {
        batch = &renderer->info.batches[renderer->info.len - 1];
    }

    if (batch == NULL)
        return;

    batch->len += len;

    RSGL_MEMCPY(&renderer->info.verts[renderer->info.vert_len * 3], points, len * sizeof(float) * 3);
    RSGL_MEMCPY(&renderer->info.texCoords[renderer->info.vert_len * 2], texPoints, len * sizeof(float) * 2);

    float color[4] = {c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f};

    if (renderer->state.gradient_len && renderer->state.gradient && (i64)(len - 1) > 0) {
        RSGL_MEMCPY(&renderer->info.colors[renderer->info.vert_len * 4], color, sizeof(float) * 4);
        RSGL_MEMCPY(&renderer->info.colors[renderer->info.vert_len * 4 + 4], renderer->state.gradient, (len - 1) * sizeof(float) * 4);
    }
    else {
        size_t i;
        for (i = 0; i < len * 4; i += 4)
            RSGL_MEMCPY(&renderer->info.colors[(renderer->info.vert_len * 4) + i], color, sizeof(float) * 4);
    }

    renderer->info.vert_len += len;
}

/*
*********************
RSGL_GRAPHICS_CONTEXT
*********************
*/

#ifndef RSGL_NO_TEXT
#define RFONT_IMPLEMENTATION
#include "RFont.h"

void RSGL_RFont_render_text(RSGL_renderer* renderer, RFont_texture atlas, float* verts, float* tcoords, size_t nverts) {
    RSGL_renderState save = renderer->state;
    RSGL_renderer_setRotate(renderer, RSGL_POINT3D(0, 0, 0));
    RSGL_renderer_setTexture(renderer, atlas);
    RSGL_basicDraw(renderer, RSGL_TRIANGLES, verts, tcoords, nverts);
    renderer->state = save;
}
#endif

void RSGL_renderer_render(RSGL_renderer* renderer) {
	if (renderer->proc.render)
		renderer->proc.render(renderer->ctx, renderer->state.program, &renderer->info);
}

size_t RSGL_renderer_size(RSGL_renderer* renderer) {
	if (renderer->proc.size) return renderer->proc.size();
	return 0;
}

void RSGL_renderer_initPtr(RSGL_rendererProc proc, RSGL_area r, void* loader, void* data, RSGL_renderer* renderer) {
	renderer->ctx = data;
	renderer->proc = proc;
    RSGL_renderer_clearArgs(renderer);
    renderer->state.color = RSGL_RGBA(0, 0, 0, 255);
    RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, r.w, r.h));

    renderer->state.customMatrix = RSGL_loadIdentity();
    renderer->state.currentArea = r;

#ifndef RSGL_NO_TEXT
	renderer->rfont.proc.create_atlas = (RFont_texture (*)(void* ctx, u32 atlasWidth, u32 atlasHeight))renderer->proc.createAtlas;
	renderer->rfont.proc.free_atlas = (void (*)(void*, RSGL_texture))renderer->proc.deleteTexture;
	renderer->rfont.proc.bitmap_to_atlas = (void(*)(void*, RFont_texture, u32, u32, u32, u8*, float, float, float*, float*))renderer->proc.bitmapToAtlas;
	renderer->rfont.proc.render = (void (*)(void*, RFont_texture, float*, float*, size_t))RSGL_RFont_render_text;
	renderer->rfont.ctx = renderer;
#endif

    renderer->info.len = 0;
    renderer->info.vert_len = 0;
	renderer->info.matrix = RSGL_ortho(RSGL_loadIdentity().m, 0, r.w, r.h, 0, 0, 1.0);

	if (renderer->proc.initPtr)
		renderer->proc.initPtr(renderer->ctx, loader);
}


RSGL_renderer* RSGL_renderer_init(RSGL_rendererProc proc, RSGL_area r, void* loader) {
	RSGL_renderer* renderer = (RSGL_renderer*)RSGL_MALLOC(sizeof(RSGL_renderer));
	void* data = RSGL_MALLOC(proc.size());
	RSGL_renderer_initPtr(proc, r, loader, data, renderer);
	return renderer;
}

void RSGL_renderer_freePtr(RSGL_renderer* renderer) {
	if (renderer->proc.freePtr)
		renderer->proc.freePtr(renderer->ctx);

	renderer->info.len = 0;
	renderer->info.vert_len = 0;
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
RSGL_texture RSGL_renderer_createTexture(RSGL_renderer* renderer, u8* bitmap, RSGL_area memsize,  u8 channels) {
    RSGL_texture tex = 0;
	if (renderer->proc.createTexture) tex = renderer->proc.createTexture(renderer->ctx, bitmap,  memsize, channels);
	return tex;
}
void RSGL_renderer_updateTexture(RSGL_renderer* renderer, RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels) {
    return renderer->proc.updateTexture(renderer->ctx, texture, bitmap, memsize, channels);
}
void RSGL_renderer_deleteTexture(RSGL_renderer* renderer, RSGL_texture tex) { renderer->proc.deleteTexture(renderer->ctx, tex); }
void RSGL_renderer_scissorStart(RSGL_renderer* renderer, RSGL_rectF scissor) {
    RSGL_renderer_render(renderer);
	renderer->proc.scissorStart(renderer->ctx, scissor, renderer->state.currentArea.h);
}
void RSGL_renderer_scissorEnd(RSGL_renderer* renderer) {
    RSGL_renderer_render(renderer);
	renderer->proc.scissorEnd(renderer->ctx);
}
RSGL_programInfo RSGL_renderer_createProgram(RSGL_renderer* renderer, const char* VShaderCode, const char* FShaderCode, const char* posName, const char* texName, const char* colorName) {
    return renderer->proc.createProgram(renderer->ctx, VShaderCode, FShaderCode, posName, texName, colorName);
}
void RSGL_renderer_deleteProgram(RSGL_renderer* renderer, RSGL_programInfo program) { return renderer->proc.deleteProgram(renderer->ctx, program); }
void RSGL_renderer_setShaderValue(RSGL_renderer* renderer, u32 program, char* var, float value[], u8 len) {
    return renderer->proc.setShaderValue(renderer->ctx, program, var, value, len);
}

RSGL_programInfo RSGL_renderer_createComputeProgram(RSGL_renderer* renderer, const char* CShaderCode) {
	return renderer->proc.createComputeProgram(renderer->ctx, CShaderCode);
}

void RSGL_renderer_dispatchComputeProgram(RSGL_renderer* renderer, RSGL_programInfo program, u32 groups_x, u32 groups_y, u32 groups_z) {
	renderer->proc.dispatchComputeProgram(renderer->ctx, program, groups_x, groups_y, groups_z);
}

void RSGL_renderer_bindComputeTexture(RSGL_renderer* renderer, u32 texture, u8 format) {
	renderer->proc.bindComputeTexture(renderer->ctx, texture, format);
}

void RSGL_renderer_updateSize(RSGL_renderer* renderer, RSGL_area r) {
    renderer->state.currentArea = r;
    RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, r.w, r.h));
    renderer->info.matrix = RSGL_ortho(RSGL_loadIdentity().m, 0, r.w, r.h, 0, 0, 1.0);
}

void RSGL_renderer_setRotate(RSGL_renderer* renderer, RSGL_point3D rotate){
    renderer->state.rotate = RSGL_POINT3D(rotate.x * DEG2RAD, rotate.y * DEG2RAD, rotate.z * DEG2RAD);
}
void RSGL_renderer_setTexture(RSGL_renderer* renderer, RSGL_texture texture) {
    renderer->state.texture = texture;
}
void RSGL_renderer_setColor(RSGL_renderer* renderer, RSGL_color color) {
	renderer->state.color = color;
}
void RSGL_renderer_setProgram(RSGL_renderer* renderer, const RSGL_programInfo* program) {
	RSGL_renderer_render(renderer);
	if (program == NULL)
		renderer->state.program.program = 0;
	else
		renderer->state.program = *program;
}

void RSGL_renderer_setGradient(RSGL_renderer* renderer, float gradient[], size_t len) {
    renderer->state.gradient_len = len;
    renderer->state.gradient = gradient;
}
void RSGL_renderer_setCenter(RSGL_renderer* renderer, RSGL_point3D center) {
    renderer->state.center = center;
}

void RSGL_renderer_clearArgs(RSGL_renderer* renderer) {
	RSGL_MEMSET(&renderer->state, 0, sizeof(renderer->state));
	renderer->state.center =  RSGL_POINT3D(-1, -1, -1);
}

/*
****
RSGL_draw
****
*/

void RSGL_drawPoint(RSGL_renderer* renderer, RSGL_point p) {
    RSGL_drawPointF(renderer, (RSGL_pointF){(float)p.x, (float)p.y});
}

void RSGL_drawTriangle(RSGL_renderer* renderer, RSGL_triangle t) {
    RSGL_drawTriangleF(renderer, RSGL_createTriangleF((float)t.p1.x, (float)t.p1.y, (float)t.p2.x, (float)t.p2.y, (float)t.p3.x, (float)t.p3.y));
}

void RSGL_drawRect(RSGL_renderer* renderer, RSGL_rect r) {
    RSGL_drawRectF(renderer, (RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h});
}

void RSGL_drawRoundRect(RSGL_renderer* renderer, RSGL_rect r, RSGL_point rounding) {
    RSGL_drawRoundRectF(renderer, (RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, rounding);
}

void RSGL_drawPolygon(RSGL_renderer* renderer, RSGL_rect r, u32 sides) {
    RSGL_drawPolygonF(renderer, (RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, sides);
}

void RSGL_drawArc(RSGL_renderer* renderer, RSGL_rect o, RSGL_point arc) {
    RSGL_drawArcF(renderer, (RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, (RSGL_pointF){(float)arc.x, (float)arc.y});
}

void RSGL_drawCircle(RSGL_renderer* renderer, RSGL_circle c) {
    RSGL_drawCircleF(renderer, (RSGL_circleF){(float)c.x, (float)c.y, (float)c.d});
}

void RSGL_drawOval(RSGL_renderer* renderer, RSGL_rect o) {
    RSGL_drawOvalF(renderer, (RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h});
}

void RSGL_drawLine(RSGL_renderer* renderer, RSGL_point p1, RSGL_point p2, u32 thickness) {
    RSGL_drawLineF(renderer, (RSGL_pointF){(float)p1.x, (float)p1.y}, (RSGL_pointF){(float)p2.x, (float)p2.y}, thickness);
}

void RSGL_drawTriangleOutline(RSGL_renderer* renderer, RSGL_triangle t, u32 thickness) {
    RSGL_drawTriangleFOutline(renderer, RSGL_createTriangleF((float)t.p1.x, (float)t.p1.y, (float)t.p2.x, (float)t.p2.y, (float)t.p3.x, (float)t.p3.y), thickness);
}

void RSGL_drawRectOutline(RSGL_renderer* renderer, RSGL_rect r, u32 thickness) {
    RSGL_drawRectFOutline(renderer, (RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, thickness);
}

void RSGL_drawRoundRectOutline(RSGL_renderer* renderer, RSGL_rect r, RSGL_point rounding, u32 thickness) {
    RSGL_drawRoundRectFOutline(renderer, (RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, rounding, thickness);
}

void RSGL_drawPolygonOutline(RSGL_renderer* renderer, RSGL_rect r, u32 sides, u32 thickness) {
    RSGL_drawPolygonFOutline(renderer, (RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, sides, thickness);
}

void RSGL_drawArcOutline(RSGL_renderer* renderer, RSGL_rect o, RSGL_point arc, u32 thickness) {
    RSGL_drawArcFOutline(renderer, (RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, (RSGL_pointF){(float)arc.x, (float)arc.y}, thickness);
}

void RSGL_drawCircleOutline(RSGL_renderer* renderer, RSGL_circle c, u32 thickness) {
    RSGL_drawCircleFOutline(renderer, (RSGL_circleF){(float)c.x, (float)c.y, (float)c.d}, thickness);
}

void RSGL_drawOvalOutline(RSGL_renderer* renderer, RSGL_rect o, u32 thickness) {
    RSGL_drawOvalFOutline(renderer, (RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, thickness);
}

void RSGL_drawPointF(RSGL_renderer* renderer, RSGL_pointF p) {
    RSGL_drawRectF(renderer, (RSGL_rectF){p.x, p.y, 1.0f, 1.0f});
}

void RSGL_drawTriangleF(RSGL_renderer* renderer, RSGL_triangleF t) {
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

    RSGL_basicDraw(renderer, RSGL_TRIANGLES, (float*)points, (float*)texPoints, 3);
}

void RSGL_drawTriangleHyp(RSGL_renderer* renderer, RSGL_pointF p, size_t angle, float hypotenuse) {
    float dir = (hypotenuse > 0);
    hypotenuse = fabsf(hypotenuse);

    float base = hypotenuse * (RSGL_COS(angle) * DEG2RAD);
    float opp = hypotenuse * (RSGL_SIN(angle) * DEG2RAD);

    RSGL_triangleF t = RSGL_TRIANGLEF(
        p,
        RSGL_POINTF(p.x + base, p.y),
        RSGL_POINTF(p.x + (base * dir), p.y - opp)
    );

    RSGL_drawTriangleF(renderer, t);
}

void RSGL_drawRectF(RSGL_renderer* renderer, RSGL_rectF r) {
    float texPoints[] = {
                                0.0f, 0.0f,
                                0.0f, 1.0f,
                                1.0f, 0.0f,
                                1.0f, 1.0f,
                                1.0f, 0.0f,
                                0.0f, 1.0f
                            };

    RSGL_point3D center = (RSGL_point3D){r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f};
    RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, center);

    float points[] = {
                                RSGL_GET_MATRIX_POINT(r.x, r.y, 0.0f),
                                RSGL_GET_MATRIX_POINT(r.x, r.y + r.h, 0.0f),
                                RSGL_GET_MATRIX_POINT(r.x + r.w, r.y, 0.0f),

                                RSGL_GET_MATRIX_POINT(r.x + r.w, r.y + r.h, 0.0f),
                                RSGL_GET_MATRIX_POINT(r.x + r.w, r.y, 0.0f),
                                RSGL_GET_MATRIX_POINT(r.x, r.y + r.h, 0.0f),
                            };

    RSGL_basicDraw(renderer, RSGL_TRIANGLES, (float*)points, (float*)texPoints, 6);
}

void RSGL_drawRoundRectF(RSGL_renderer* renderer, RSGL_rectF r, RSGL_point rounding) {
    RSGL_drawRect(renderer, RSGL_RECT(r.x + (rounding.x / 2), r.y, r.w - rounding.x, r.h));
    RSGL_drawRect(renderer, RSGL_RECT(r.x, r.y + (rounding.y / 2), r.w,  r.h - rounding.y));

    RSGL_drawArc(renderer, RSGL_RECT(r.x, r.y, rounding.x, rounding.y), (RSGL_point){180, 270});
    RSGL_drawArc(renderer, RSGL_RECT(r.x + (r.w - rounding.x), r.y, rounding.x, rounding.y), (RSGL_point){90, 180});
    RSGL_drawArc(renderer, RSGL_RECT(r.x + (r.w - rounding.x), r.y  + (r.h - rounding.y), rounding.x, rounding.y), (RSGL_point){0, 90});
    RSGL_drawArc(renderer, RSGL_RECT(r.x, r.y  + (r.h - rounding.y),  rounding.x, rounding.y), (RSGL_point){270, 360});
}

void RSGL_drawPolygonFOutlineEx(RSGL_renderer* renderer, RSGL_rectF o, u32 sides, RSGL_pointF arc);

void RSGL_drawPolygonFEx(RSGL_renderer* renderer, RSGL_rectF o, u32 sides, RSGL_pointF arc) {
    static float verts[360 * 3];
    static float texcoords[360 * 2];

    RSGL_point3D center =  (RSGL_point3D){o.x + (o.w / 2.0f), o.y + (o.h / 2.0f), 0};

	o = (RSGL_rectF){o.x, o.y, o.w / 2, o.h / 2};
    RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, center);

    float displacement = 360.0f / (float)sides;
    float angle = displacement * arc.x;

    size_t vIndex = 0;
    size_t tIndex = 0;

    u32 i;
    for (i = 0; i < sides; i++) {
        RSGL_pointF p = {RSGL_SIN(angle * DEG2RAD), RSGL_COS(angle * DEG2RAD)};

        texcoords[tIndex] = (p.x + 1.0f) * 0.5;
        texcoords[tIndex + 1] = (p.y + 1.0f) * 0.5;

        float temp[3] = { RSGL_GET_MATRIX_POINT(o.x + o.w + (p.x * o.w), o.y + o.h + (p.y * o.h), 0.0) };
        memcpy(verts + vIndex, temp, 3 * sizeof(float));

        angle += displacement;
        tIndex += 2;
        vIndex += 3;
    }

    texcoords[tIndex + 1] = 0;
    texcoords[tIndex + 2] = 0;

    RSGL_basicDraw(renderer, RSGL_TRIANGLE_FAN, verts, texcoords, vIndex / 3);
}

void RSGL_drawPolygonF(RSGL_renderer* renderer, RSGL_rectF o, u32 sides) { RSGL_drawPolygonFEx(renderer, o, sides, (RSGL_pointF){0, (float)sides}); }


void RSGL_drawArcF(RSGL_renderer* renderer, RSGL_rectF o, RSGL_pointF arc) {
    u32 verts = (u32)((float)((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10) + 0.5);
    verts %= 360;

    RSGL_drawPolygonFEx(renderer, o, verts, arc);
}

void RSGL_drawCircleF(RSGL_renderer* renderer, RSGL_circleF c) {
    u32 verts = (u32)((2.0 * M_PI * c.d) / 10) % 360;

    RSGL_drawPolygonFEx(renderer, (RSGL_rectF){c.x, c.y, c.d, c.d}, verts, (RSGL_pointF){0, (float)verts});
}

void RSGL_drawOvalF(RSGL_renderer* renderer, RSGL_rectF o) {
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_drawPolygonFEx(renderer, o, verts, (RSGL_pointF){0, verts});
}

/*
    3D
*/

void RSGL_drawPoint3D(RSGL_renderer* renderer, RSGL_point3D p) {
    RSGL_drawCube(renderer, (RSGL_cube){p.x, p.y, p.z, 1.0f, 1.0f, 1.0f});
}

void RSGL_drawLine3D(RSGL_renderer* renderer, RSGL_point3D p1, RSGL_point3D p2, u32 thickness) {
    renderer->state.lineWidth = thickness;

    RSGL_point3D center = {(p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f, (p1.z + p2.z) / 2.0f};
    RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, center);

    float points[] = {RSGL_GET_MATRIX_POINT(p1.x, p1.y, p1.z), RSGL_GET_MATRIX_POINT(p2.x, p2.y, p2.z)};
    float texPoints[] = {0, 0.0f,          0, 0.0f};

    RSGL_basicDraw(renderer, RSGL_LINES, (float*)points, (float*)texPoints, 2);
}

void RSGL_drawTriangle3D(RSGL_renderer* renderer, RSGL_triangle3D t) {
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

    RSGL_basicDraw(renderer, RSGL_TRIANGLES, (float*)points, (float*)texPoints, 3);
}

void RSGL_drawCube(RSGL_renderer* renderer, RSGL_cube cube) {
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
        1.0f, 1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
    };

    RSGL_point3D center = {
        cube.x + cube.w / 2.0f,
        cube.y + cube.h / 2.0f,
        cube.z + cube.l / 2.0f
    };

    RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, center);

    float points[] = {
        // Front face
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y,         cube.z),
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y + cube.h, cube.z),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y,         cube.z),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y + cube.h, cube.z),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y,         cube.z),
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y + cube.h, cube.z),
        // Back face
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y,         cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y + cube.h, cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y,         cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y + cube.h, cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y,         cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y + cube.h, cube.z + cube.l),
        // Left face
        RSGL_GET_MATRIX_POINT(cube.x, cube.y,         cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x, cube.y + cube.h, cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x, cube.y,         cube.z),
        RSGL_GET_MATRIX_POINT(cube.x, cube.y + cube.h, cube.z),
        RSGL_GET_MATRIX_POINT(cube.x, cube.y,         cube.z),
        RSGL_GET_MATRIX_POINT(cube.x, cube.y + cube.h, cube.z + cube.l),
        // Right face
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y,         cube.z),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y + cube.h, cube.z),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y,         cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y + cube.h, cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y,         cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y + cube.h, cube.z),
        // Top face
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y + cube.h, cube.z),
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y + cube.h, cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y + cube.h, cube.z),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y + cube.h, cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y + cube.h, cube.z),
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y + cube.h, cube.z + cube.l),
        // Bottom face
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y, cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y, cube.z),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y, cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y, cube.z),
        RSGL_GET_MATRIX_POINT(cube.x + cube.w, cube.y, cube.z + cube.l),
        RSGL_GET_MATRIX_POINT(cube.x,         cube.y, cube.z),
    };

    RSGL_basicDraw(renderer, RSGL_TRIANGLES, points, texPoints, 36);
}

/*
outlines
*/

void RSGL_drawLineF(RSGL_renderer* renderer, RSGL_pointF p1, RSGL_pointF p2, u32 thickness) {
    renderer->state.lineWidth = thickness;

    RSGL_point3D center = {(p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f, 0.0f};
    RSGL_mat4 matrix = RSGL_renderer_initDrawMatrix(renderer, center);

    float points[] = {RSGL_GET_MATRIX_POINT(p1.x, p1.y, 0.0f), RSGL_GET_MATRIX_POINT(p2.x, p2.y, 0.0f)};
    float texPoints[] = {0, 0.0f,          0, 0.0f};

    RSGL_basicDraw(renderer, RSGL_LINES, (float*)points, (float*)texPoints, 2);
}

void RSGL_drawTriangleFOutline(RSGL_renderer* renderer, RSGL_triangleF t, u32 thickness) {
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

    RSGL_basicDraw(renderer, RSGL_LINES, (float*)points, texCoords, 6);
}
void RSGL_drawRectFOutline(RSGL_renderer* renderer, RSGL_rectF r, u32 thickness) {
    RSGL_point3D oCenter = renderer->state.center;

    RSGL_renderer_setCenter(renderer, (RSGL_point3D){r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f});
    RSGL_drawLineF(renderer, (RSGL_pointF){r.x, r.y}, (RSGL_pointF){r.x + r.w, r.y}, thickness);

    RSGL_renderer_setCenter(renderer, (RSGL_point3D){r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f});
    RSGL_drawLineF(renderer, (RSGL_pointF){r.x, r.y}, (RSGL_pointF){r.x, r.y + r.h}, thickness);

    RSGL_renderer_setCenter(renderer, (RSGL_point3D){r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f});
    RSGL_drawLineF(renderer, (RSGL_pointF){r.x, r.y + r.h}, (RSGL_pointF){r.x + r.w, r.y + r.h}, thickness);

    RSGL_renderer_setCenter(renderer, (RSGL_point3D){r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f});
    RSGL_drawLineF(renderer, (RSGL_pointF){r.x + r.w, r.y}, (RSGL_pointF){r.x + r.w, r.y + r.h}, thickness);

    RSGL_renderer_setCenter(renderer, oCenter);
}
void RSGL_drawRoundRectFOutline(RSGL_renderer* renderer, RSGL_rectF r, RSGL_point rounding, u32 thickness) {
    RSGL_drawRectF(renderer, RSGL_RECTF(r.x + (rounding.x/2), r.y, r.w - rounding.x, (int)(thickness + !thickness)));
    RSGL_drawRectF(renderer, RSGL_RECTF(r.x + (rounding.x/2), r.y + r.h, r.w - rounding.x, (int)(thickness + !thickness)));
    RSGL_drawRectF(renderer, RSGL_RECTF(r.x, r.y + (rounding.y/2), (int)(thickness + !thickness),  r.h - rounding.y));
    RSGL_drawRectF(renderer, RSGL_RECTF(r.x + r.w, r.y + (rounding.y/2), (int)(thickness + !thickness),  r.h - rounding.y));

    RSGL_drawArcFOutline(renderer, RSGL_RECTF(r.x, r.y, rounding.x, rounding.y), (RSGL_pointF){180, 270}, thickness);
    RSGL_drawArcFOutline(renderer, RSGL_RECTF(r.x + (r.w - rounding.x), r.y, rounding.x, rounding.y), (RSGL_pointF){90, 180}, thickness);
    RSGL_drawArcFOutline(renderer, RSGL_RECTF(r.x + (r.w - rounding.x), r.y  + (r.h - rounding.y) - 1, rounding.x, rounding.y + 2), (RSGL_pointF){0, 90}, thickness);
    RSGL_drawArcFOutline(renderer, RSGL_RECTF(r.x + 1, r.y  + (r.h - rounding.y) - 1,  rounding.x, rounding.y + 2), (RSGL_pointF){270, 360}, thickness);
}

void RSGL_drawPolygonFOutlineEx(RSGL_renderer* renderer, RSGL_rectF o, u32 sides, RSGL_pointF arc) {
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

    RSGL_basicDraw(renderer, RSGL_LINES, verts, texCoords, index / 3);
}

void RSGL_drawPolygonFOutline(RSGL_renderer* renderer, RSGL_rectF o, u32 sides, u32 thickness) {
    renderer->state.lineWidth = thickness;
    RSGL_drawPolygonFOutlineEx(renderer, o, sides, (RSGL_pointF){0, (float)sides});
}
void RSGL_drawArcFOutline(RSGL_renderer* renderer, RSGL_rectF o, RSGL_pointF arc, u32 thickness) {
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    renderer->state.lineWidth = thickness;
    RSGL_drawPolygonFOutlineEx(renderer, o, verts, arc);
}
void RSGL_drawCircleFOutline(RSGL_renderer* renderer, RSGL_circleF c, u32 thickness) {
    float verts = ((2 * M_PI * c.d) / 10);
    verts = (verts > 360 ? 360 : verts);

    renderer->state.lineWidth = thickness;
    RSGL_drawPolygonFOutlineEx(renderer, (RSGL_rectF){c.x, c.y, c.d, c.d}, verts, (RSGL_pointF){0, verts});
}
void RSGL_drawOvalFOutline(RSGL_renderer* renderer, RSGL_rectF o, u32 thickness) {
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    renderer->state.lineWidth = thickness;
    RSGL_drawPolygonFOutlineEx(renderer, o, verts, (RSGL_pointF){0, verts});
}

#ifndef RSGL_NO_TEXT
RSGL_font* RSGL_loadFont(RSGL_renderer* renderer, const char* font, size_t maxHeight, size_t atlasWidth, size_t atlasHeight) {
	RSGL_font* ptr = (RSGL_font*)RSGL_MALLOC(sizeof(RSGL_font));
	RSGL_loadFontPtr(renderer, font, maxHeight, atlasWidth, atlasHeight, ptr);
	return ptr;
}

void RSGL_loadFontPtr(RSGL_renderer* renderer, const char* font, size_t maxHeight, size_t atlasWidth, size_t atlasHeight, RFont_font* ptr) {
	RFont_font_init_ptr(&renderer->rfont, font, maxHeight, atlasWidth, atlasHeight, ptr);
}

void RSGL_font_freePtr(RSGL_renderer* renderer, RSGL_font* font) { RFont_font_free_ptr(&renderer->rfont, font); }
void RSGL_font_free(RSGL_renderer* renderer, RSGL_font* font) { RSGL_font_freePtr(renderer, font); RSGL_FREE(font); }

void RSGL_renderer_setFont(RSGL_renderer* renderer, RSGL_font* font) {
    if (font == NULL) {
        return;
    }

    renderer->state.font = font;
}

void RSGL_drawText(RSGL_renderer* renderer, const char* text, RSGL_circle c) {
    RSGL_drawText_pro(renderer, text, strlen(text), 0.0f, c);
}

void RSGL_drawText_len(RSGL_renderer* renderer, const char* text, size_t len, RSGL_circle c) {
    RSGL_drawText_pro(renderer, text, len, 0.0f, c);
}

void RSGL_drawText_pro(RSGL_renderer* renderer, const char* text, size_t len, float spacing, RSGL_circle c) {
    if (text == NULL || renderer->state.font == NULL) {
        return;
	}

    RFont_draw_text_len(&renderer->rfont, renderer->state.font, text, len, c.x, c.y, c.d, spacing);
}

RSGL_area RSGL_renderer_textArea(RSGL_renderer* renderer, const char* text, u32 fontSize, size_t textEnd) {
	RSGL_area area = RSGL_AREA(0, 0);

	if (renderer->state.font == NULL)
		RFont_text_area_len(&renderer->rfont, renderer->state.font, text, textEnd, fontSize, 0, 0.0, &area.w, &area.h);

	return area;
}

RSGL_area RSGL_renderer_textLineArea(RSGL_renderer* renderer, const char* text, u32 fontSize, size_t textEnd, size_t line) {
	RSGL_area area = RSGL_AREA(0, 0);
	RFont_text_area_len(&renderer->rfont, renderer->state.font, text, textEnd, fontSize, line, 0.0, &area.w, &area.h);
	return area;
}

#endif /* RSGL_NO_TEXT */

/*
******
RSGL_camera
******
*/

/* Multiply the current matrix by a rotation matrix */
RSGL_mat4 RSGL_renderer_getCameraMatrix(RSGL_renderer* renderer, RSGL_camera camera) {
    return RSGL_renderer_getCameraMatrixEx(renderer, camera, (16.0 / 9.0), 90, 0.001, 1000.0);
}

RSGL_mat4 RSGL_renderer_getCameraMatrixEx(RSGL_renderer* renderer, RSGL_camera camera, float ratio, float maxPitch, float min, float max) {
    RSGL_mat4 matrix = RSGL_loadIdentity();
    matrix = RSGL_perspective(matrix.m, camera.fov, ratio, min, max);

    if (camera.pitch >= maxPitch)
        camera.pitch = maxPitch;
    else if (camera.pitch <= -maxPitch)
        camera.pitch = -maxPitch;

    matrix = RSGL_rotate(matrix.m, camera.pitch * DEG2RAD, 1.0, 0.0, 0.0);
    matrix = RSGL_rotate(matrix.m, camera.yaw * DEG2RAD, 0.0, 1.0, 0.0);
    matrix = RSGL_translate(matrix.m, camera.pos.x, camera.pos.y, -camera.pos.z);

    return matrix;
}

/*
******
RSGL_Matrix
******
*/

RSGL_mat4 RSGL_ortho(float matrix[16], float left, float right, float bottom, float top, float znear, float zfar) {
    float rl = (float)(right - left);
    float tb = (float)(top - bottom);
    float fn = (float)(zfar - znear);

    float matOrtho[16] = {
        (2.0f / rl),                          0.0f,                             0.0f,                                 0.0f,
        0.0f,                                (2.0f / tb),                       0.0f,                                0.00,
        0.0f,                                 0.0f,                             (-2.0f / fn),                         0.0f,
        (-((float)left + (float)right) / rl), -((float)top + (float)bottom)/tb, (-((float)zfar + (float)znear) / fn), 1.0f
    };

    return RSGL_mat4Multiply(matrix, matOrtho);
}

/* Multiply the current matrix by a translation matrix */
RSGL_mat4 RSGL_translate(float matrix[16], float x, float y, float z) {
	RSGL_mat4 result;

	for (int i = 0; i < 16; ++i) {
		result.m[i] = matrix[i];
	}

	result.m[12] += matrix[0]*x + matrix[4]*y + matrix[8]*z;
	result.m[13] += matrix[1]*x + matrix[5]*y + matrix[9]*z;
	result.m[14] += matrix[2]*x + matrix[6]*y + matrix[10]*z;

	return result;
}

RSGL_mat4 RSGL_loadIdentity(void) {
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

RSGL_mat4 RSGL_rotate(float matrix[16], float angle, float x, float y, float z) {
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

	return RSGL_mat4Multiply(matRotation, matrix);
}

RSGL_mat4 RSGL_perspective(float matrix[16], float fovY, float aspect, float zNear, float zFar) {
    fovY =  (fovY * DEG2RAD) / 2.0f;
    const float f = (RSGL_COS(fovY) / RSGL_SIN(fovY));

    float perspective[16] = {
            (f / aspect), 0.0f,  0.0f,                                   0.0f,
            0,            f,     0.0f,                                   0.0f,
            0.0f,         0.0f,  (zFar + zNear) / (zNear - zFar),       -1.0f,
            0.0f,         0.0f,  (2.0f * zFar * zNear) / (zNear - zFar),  0.0f
    };

	return RSGL_mat4Multiply(matrix, perspective);
}

RSGL_mat4 RSGL_mat4Multiply(float left[16], float right[16]) {
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
