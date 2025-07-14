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
    #define RSGL_INIT_FONTS [number of fonts] - set how much room should be pre-allocated for fonts by fontstash
                                                this avoids performance issues related to RSGL_REALLOC
                                                RSGL_INIT_FONTS = 4 by default
    #define RSGL_INIT_IMAGES [number of fonts] - set how much room should be pre-allocated for images by RSGL
                                                this avoids performance issues related to RSGL_REALLOC
                                                RSGL_INIT_IMAGES = 20 by default
    #define RSGL_NEW_IMAGES [number of fonts] - set how much room should be RSGL_REALLOCated at a time for images by RSGL
                                                this avoids performance issues related to RSGL_REALLOC
                                                RSGL_NEW_IMAGES 10 by default

    #define RSGL_MAX_BATCHES [number of batches] - set max number of batches to be allocated
    #define RSGL_MAX_VERTS [number of verts] - set max number of verts to be allocated (global, not per batch)

    #define RSGL_RENDER_LEGACY - use legacy rendering (ex. opengl) functions
    
    #define RSGL_NO_DEPS_FOLDER - Do not use '/deps' for the deps includes, use "./"
*/ 
#include <stdint.h>
#ifndef RSGL_INIT_FONTS
#define RSGL_INIT_FONTS 4
#endif
#ifndef RSGL_NEW_FONTS
#define RSGL_NEW_FONTS 2
#endif
#ifndef RSGL_INIT_IMAGES
#define RSGL_INIT_IMAGES 20
#endif
#ifndef RSGL_NEW_IMAGES
#define RSGL_NEW_IMAGES 10
#endif
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

#ifndef RSGL_framebuffer
typedef struct RSGL_framebuffer { u32 id, texture; } RSGL_framebuffer;
#endif

#ifndef RSGL_texture
#define RSGL_texture size_t
#endif

// WebGL doesn't support compute shaders iirc so yeah
#if defined(__EMSCRIPTEN__) && defined(RSGL_USE_COMPUTE)
#undef RSGL_USE_COMPUTE
#endif

/* 
*******
RSGL_[shape]
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

#define RSGL_createTriangle3D(x1, y1, z1, x2, y2, z2, x3, y3, z3) (RSGL_triangle3D){{x1, y1, z1}, {x2, y2, z2}, {x3, y3, z3}}

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
RSGL_GRAPHICS_CONTEXT
*********************
*/

#ifndef RSGL_mat4
typedef struct RSGL_mat4 {
    float m[16];
} RSGL_mat4;
#endif

typedef struct RSGL_BATCH {
    size_t start, len; /* when batch starts and it's length */
    u32 type;
    RSGL_texture tex;
    float lineWidth;
    RSGL_mat4 matrix;
} RSGL_BATCH; /* batch data type for rendering */

typedef struct RSGL_RENDER_INFO {
    RSGL_BATCH* batches;

    float* verts;
    float* texCoords;
    float* colors;

    size_t len; /* number of batches*/
    size_t vert_len; /* number of verts */
    RSGL_mat4 matrix;
} RSGL_RENDER_INFO; /* render data */

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
    u32 program;
	 RSGL_shaderType type;
} RSGL_programInfo;
#endif

typedef struct RSGL_renderer {
    void (* batch)(RSGL_RENDER_INFO* info);
    void (* init)(void* proc, RSGL_RENDER_INFO* info); /* init render backend */
    void (* free)(void); /* free render backend */
    void (* clear)(float r, float g, float b, float a);
    void (* viewport)(i32 x, i32 y, i32 w, i32 h);
    void (* setFramebuffer)(u32 id);
    RSGL_framebuffer (* createFramebuffer)(RSGL_area memsize);
    RSGL_texture (* createTexture)(u8* bitmap, RSGL_area memsize,  u8 channels);
    void (* updateTexture)(RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels);
    void (* deleteTexture)(RSGL_texture tex);
    void (* deleteFramebuffer)(RSGL_framebuffer fbo);
    void (* scissorStart)(RSGL_rectF scissor);
    void (* scissorEnd)(void);
    RSGL_programInfo (*createProgram)(const char* VShaderCode, const char* FShaderCode, const char* posName, const char* texName, const char* colorName);
    void (* deleteProgram)(RSGL_programInfo program);
    void (* setShaderValue)(u32 program, char* var, float value[], u8 len);
    RSGL_texture (* createAtlas)(u32 atlasWidth, u32 atlasHeight);
    u8 (* resizeAtlas)(RSGL_texture* atlas, u32 newWidth, u32 newHeight);
    void (* bitmapToAtlas)(RSGL_texture atlas, u8* bitmap, float x, float y, float w, float h);

#ifdef RSGL_USE_COMPUTE
	 RSGL_programInfo (*createComputeProgram)(const char* CShaderCode);
	 void (*dispatchComputeProgram)(RSGL_programInfo program, u32 groups_x, u32 groups_y, u32 groups_z);
	 void (*bindComputeTexture)(u32 texture, u8 format);
#endif
} RSGL_renderer;

RSGLDEF void RSGL_setRenderer(RSGL_renderer renderer);

RSGLDEF void RSGL_init(
                            RSGL_area r, /* graphics context size */
                            void* loader, /* opengl prozc address ex. wglProcAddress */
                            RSGL_renderer renderer     
                       ); 
RSGLDEF void RSGL_updateSize(RSGL_area r);
RSGLDEF void RSGL_draw(void); /* draw current batch */
RSGLDEF void RSGL_clear(RSGL_color c);
RSGLDEF void RSGL_free(void);

/* 
*******
RSGL_draw
*******
*/


/* 
    RSGL_draw args 

    RSGL has internal args which control how RSGL draws certain things
    by default these args clear after each RSGL_draw<whatever> call 

    but you can run RSGL_setClearArgs to enable or disable this behavior
    you can also run RSGL_clearArgs to clear the args by hand
*/
/* RSGL_args */
typedef struct RSGL_drawArgs {
    float* gradient; /* does not allocate any memory */
    
    RSGL_texture texture;
    u32 gradient_len;

    RSGL_area currentArea; /* size of current surface */
    RSGL_point3D rotate; 

    RSGL_point3D center;
    float lineWidth;
    u32 program;
    RSGL_mat4 matrix;
} RSGL_drawArgs;

RSGLDEF void RSGL_setRotate(RSGL_point3D rotate); /* apply rotation to drawing */
RSGLDEF void RSGL_setTexture(RSGL_texture texture); /* apply texture to drawing */
RSGLDEF void RSGL_setProgram(u32 program); /* use shader program for drawing */
RSGLDEF void RSGL_setGradient(
                                float* gradient, /* array of gradients */
                                size_t len /* length of array */
                            ); /* apply gradient to drawing, based on color list*/
RSGLDEF void RSGL_setCenter(RSGL_point3D center); /* the center of the drawing (or shape), this is used for rotation */
/* args clear after a draw function by default, this toggles that */
RSGLDEF void RSGL_setClearArgs(RSGL_bool clearArgs); /* toggles if args are cleared by default or not */
RSGLDEF void RSGL_clearArgs(void); /* clears the args */

RSGLDEF RSGL_mat4 RSGL_initDrawMatrix(RSGL_point3D center);

/* 
RSGL_basicDraw is a function used internally by RSGL, but you can use it yourself
RSGL_basicDraw batches a given set of points based on the data to be rendered
*/
RSGLDEF void RSGL_basicDraw(
                u32 TYPE, /* type of shape  RSGL_TRIANGLES, RSGL_LINES */
                float* points, /* array of 3D points */
                float* texPoints, /* array of 2D texture points (must be same length as points)*/
                RSGL_color c, /* the color to draw the shape */
                size_t len /* the length of the points array */
            );
/* 
    All of these functions are to be defined by the external render backend
*/

/* renders the current batches */
RSGLDEF void RSGL_renderBatch(RSGL_RENDER_INFO* info);
RSGLDEF void RSGL_renderInit(void* proc, RSGL_RENDER_INFO* info); /* init render backend */
RSGLDEF void RSGL_renderFree(void); /* free render backend */
RSGLDEF void RSGL_renderClear(float r, float g, float b, float a);
RSGLDEF void RSGL_renderViewport(i32 x, i32 y, i32 w, i32 h);
/* create a framebuffer, this must be freed later using RSGL_deleteFramebuffer or opengl*/
RSGLDEF RSGL_framebuffer RSGL_renderCreateFramebuffer(RSGL_area memsize);
/* create a texture based on a given bitmap, this must be freed later using RSGL_deleteTexture or opengl*/
RSGLDEF RSGL_texture RSGL_renderCreateTexture(u8* bitmap, RSGL_area memsize,  u8 channels);
/* updates an existing texture wiht a new bitmap */
RSGLDEF void RSGL_renderUpdateTexture(RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels);
/* delete a texture */
RSGLDEF void RSGL_renderDeleteTexture(RSGL_texture tex);
/* delete a framebuffer */
RSGLDEF void RSGL_renderDeleteFramebuffer(RSGL_framebuffer fbo);
/* starts scissoring */
RSGLDEF void RSGL_renderScissorStart(RSGL_rectF scissor);
/* stops scissoring */
RSGLDEF void RSGL_renderScissorEnd(void);
/* custom shader program */
RSGLDEF RSGL_programInfo RSGL_renderCreateProgram(const char* VShaderCode, const char* FShaderCode, const char* posName, const char* texName, const char* colorName);
RSGLDEF void RSGL_renderDeleteProgram(RSGL_programInfo program);
RSGLDEF void RSGL_renderSetShaderValue(u32 program, char* var, float value[], u8 len);

#ifdef RSGL_USE_COMPUTE
RSGLDEF RSGL_programInfo RSGL_renderCreateComputeProgram(const char *CShaderCode);
RSGLDEF void RSGL_renderDispatchComputeProgram(RSGL_programInfo program, u32 groups_x, u32 groups_y, u32 groups_z);
RSGLDEF void RSGL_renderBindComputeTexture(u32 texture, u8 format);
#endif

/* these are RFont functions that also must be defined by the renderer

32 RFont_create_atlas(u32 atlasWidth, u32 atlasHeight);
void RFont_bitmap_to_atlas(RSGL_rsoft_texture atlas, u8* bitmap, float x, float y, float w, float h);

*/

/* RSGL translation */
#ifndef RSGL_camera
typedef struct RSGL_camera {
    RSGL_point3D pos;
    float fov; 
    float pitch, yaw; 
} RSGL_camera;
#endif

RSGLDEF RSGL_mat4 RSGL_getCameraMatrix(RSGL_camera camera);
RSGLDEF RSGL_mat4 RSGL_getCameraMatrixEx(RSGL_camera camera, float ratio, float maxPitch, float min, float max);

RSGLDEF void RSGL_setGlobalMatrix(RSGL_mat4 matrix);
RSGLDEF void RSGL_resetGlobalMatrix();

RSGLDEF RSGL_mat4 RSGL_loadIdentity(void);
RSGLDEF RSGL_mat4 RSGL_rotate(float matrix[16], float angle, float x, float y, float z); 
RSGLDEF RSGL_mat4 RSGL_translate(float matrix[16], float x, float y, float z);
RSGLDEF RSGL_mat4 RSGL_perspective(float matrix[16], float fovY, float aspect, float zNear, float zFar);
RSGLDEF RSGL_mat4 RSGL_ortho(float matrix[16], float left, float right, float bottom, float top, float znear, float zfar);

RSGLDEF RSGL_mat4 RSGL_mat4Multiply(float left[16], float right[16]);
/* 2D shape drawing */
/* in the function names, F means float */

RSGLDEF void RSGL_drawPoint(RSGL_point p, RSGL_color c);
RSGLDEF void RSGL_drawPointF(RSGL_pointF p, RSGL_color c);

RSGLDEF void RSGL_drawTriangle(RSGL_triangle t, RSGL_color c);
RSGLDEF void RSGL_drawTriangleF(RSGL_triangleF t, RSGL_color c);

RSGLDEF void RSGL_drawTriangleHyp(RSGL_pointF p, size_t angle, float hypotenuse, RSGL_color color);

RSGLDEF void RSGL_drawRect(RSGL_rect r, RSGL_color c);
RSGLDEF void RSGL_drawRectF(RSGL_rectF r, RSGL_color c);

RSGLDEF void RSGL_drawRoundRect(RSGL_rect r, RSGL_point rounding, RSGL_color c);
RSGLDEF void RSGL_drawRoundRectF(RSGL_rectF r, RSGL_point rounding, RSGL_color c);

RSGLDEF void RSGL_drawPolygon(RSGL_rect r, u32 sides, RSGL_color c);
RSGLDEF void RSGL_drawPolygonF(RSGL_rectF r, u32 sides, RSGL_color c);

RSGLDEF void RSGL_drawArc(RSGL_rect o, RSGL_point arc, RSGL_color color);
RSGLDEF void RSGL_drawArcF(RSGL_rectF o, RSGL_pointF arc, RSGL_color color);

RSGLDEF void RSGL_drawCircle(RSGL_circle c, RSGL_color color);
RSGLDEF void RSGL_drawCircleF(RSGL_circleF c, RSGL_color color);

RSGLDEF void RSGL_drawOval(RSGL_rect o, RSGL_color c);
RSGLDEF void RSGL_drawOvalF(RSGL_rectF o, RSGL_color c);

RSGLDEF void RSGL_drawLine(RSGL_point p1, RSGL_point p2, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawLineF(RSGL_pointF p1, RSGL_pointF p2, u32 thickness, RSGL_color c);

/* 3D objects */
RSGLDEF void RSGL_drawTriangle3D(RSGL_triangle3D t, RSGL_color c);
RSGLDEF void RSGL_drawPoint3D(RSGL_point3D p, RSGL_color c);
RSGLDEF void RSGL_drawLine3D(RSGL_point3D p1, RSGL_point3D p2, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawCube(RSGL_cube cube, RSGL_color c);

/* 2D outlines */

/* thickness means the thickness of the line */

RSGLDEF void RSGL_drawTriangleOutline(RSGL_triangle t, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawTriangleFOutline(RSGL_triangleF t, u32 thickness, RSGL_color c);

RSGLDEF void RSGL_drawRectOutline(RSGL_rect r, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawRectFOutline(RSGL_rectF r, u32 thickness, RSGL_color c);

RSGLDEF void RSGL_drawRoundRectOutline(RSGL_rect r, RSGL_point rounding, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawRoundRectFOutline(RSGL_rectF r, RSGL_point rounding, u32 thickness, RSGL_color c);

RSGLDEF void RSGL_drawPolygonOutline(RSGL_rect r, u32 sides, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawPolygonFOutline(RSGL_rectF r, u32 sides, u32 thickness, RSGL_color c);

RSGLDEF void RSGL_drawArcOutline(RSGL_rect o, RSGL_point arc, u32 thickness, RSGL_color color);
RSGLDEF void RSGL_drawArcFOutline(RSGL_rectF o, RSGL_pointF arc, u32 thickness, RSGL_color color);

RSGLDEF void RSGL_drawCircleOutline(RSGL_circle c, u32 thickness, RSGL_color color);
RSGLDEF void RSGL_drawCircleFOutline(RSGL_circleF c, u32 thickness, RSGL_color color);

RSGLDEF void RSGL_drawOvalFOutline(RSGL_rectF o, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawOvalOutline(RSGL_rect o, u32 thickness, RSGL_color c);

/* format a string */
#ifndef RSGL_NO_TEXT
struct RFont_font;
typedef struct RFont_font RSGL_font;

/* loads a font into the RSGL_font struct */
RSGLDEF RSGL_font* RSGL_loadFont(const char* font);
/*     using a given atlasWidth + atlasHeight */
RSGLDEF RSGL_font* RSGL_loadFontEx(const char* font, size_t atlasWidth, size_t atlasHeight);
RSGLDEF void RSGL_freeFont(RSGL_font* font);

RSGLDEF void RSGL_setFont(RSGL_font* font);

RSGLDEF void RSGL_drawText_len(const char* text, size_t len, RSGL_circle c, RSGL_color color);
RSGLDEF void RSGL_drawText_pro(const char* text, size_t len, float spacing, RSGL_circle c, RSGL_color color);
RSGLDEF void RSGL_drawText(const char* text, RSGL_circle c, RSGL_color color);

/* 
    returns the width of a text when rendered with the set font with the size of `fontSize
    stops at `textEnd` or when it reaches '\0'
*/
RSGLDEF RSGL_area RSGL_textArea(const char* text, u32 fontSize, size_t textEnd);
RSGLDEF RSGL_area RSGL_textLineArea(const char* text, u32 fontSize, size_t textEnd, size_t line);
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

#ifndef RSGL_NO_TEXT
#define RFONT_IMPLEMENTATION

#define RFONT_RENDER_LEGACY
#define RFONT_NO_OPENGL

#define RFont_area RSGL_area

#define RFont_texture RSGL_texture
#define RFONT_MALLOC RSGL_MALLOC
#define RFONT_FREE RSGL_FREE

#define RFONT_GET_WORLD_X(x, w) (float)x
#define RFONT_GET_WORLD_Y(y, h) (float)y 
#include "RFont.h"
#endif /* RSGL_NO_TEXT */

RSGL_drawArgs RSGL_args;
RSGL_bool RSGL_argsClear = RSGL_FALSE;
RSGL_color RFontcolor; 

RSGL_mat4 RSGL_customMatrix;
RSGL_bool RSGL_forceBatch = RSGL_FALSE;

void RSGL_setGlobalMatrix(RSGL_mat4 matrix) {
    RSGL_customMatrix = matrix;
    RSGL_forceBatch = RSGL_TRUE;
}

void RSGL_resetGlobalMatrix() {
    RSGL_customMatrix = RSGL_loadIdentity();
    RSGL_forceBatch = RSGL_TRUE;
}

RSGL_mat4 RSGL_initDrawMatrix(RSGL_point3D center) {    
    RSGL_mat4 matrix = RSGL_loadIdentity(); 

    if (RSGL_args.rotate.x || RSGL_args.rotate.y || RSGL_args.rotate.z) {
        if (RSGL_args.center.x != -1 && RSGL_args.center.y != -1 &&  RSGL_args.center.z != -1)
            center = RSGL_args.center;
        
        matrix = RSGL_translate(matrix.m, center.x, center.y, center.z);
        matrix = RSGL_rotate(matrix.m, RSGL_args.rotate.z,  0, 0, 1);
        matrix = RSGL_rotate(matrix.m, RSGL_args.rotate.y, 0, 1, 0);
        matrix = RSGL_rotate(matrix.m, RSGL_args.rotate.x, 1, 0, 0);
        matrix = RSGL_translate(matrix.m, -center.x, -center.y, -center.z);
    }
    
    return matrix;
}

RSGL_RENDER_INFO RSGL_renderInfo = {NULL, NULL, NULL, NULL, 0, 0};

void RSGL_basicDraw(u32 type, float* points, float* texPoints, RSGL_color c, size_t len) {
    if (RSGL_renderInfo.len + 1 >= RSGL_MAX_BATCHES || RSGL_renderInfo.vert_len + len >= RSGL_MAX_VERTS) {
        RSGL_renderBatch(&RSGL_renderInfo);
    }

    RSGL_BATCH* batch = NULL;

    if (
        RSGL_renderInfo.len == 0 || 
        RSGL_renderInfo.batches[RSGL_renderInfo.len - 1].tex != RSGL_args.texture  ||
        RSGL_renderInfo.batches[RSGL_renderInfo.len - 1].lineWidth != RSGL_args.lineWidth ||
        RSGL_renderInfo.batches[RSGL_renderInfo.len - 1].type != type ||
        RSGL_renderInfo.batches[RSGL_renderInfo.len - 1].type == RSGL_TRIANGLE_FAN ||
        RSGL_forceBatch
    ) {
        RSGL_forceBatch = RSGL_FALSE;
        RSGL_renderInfo.len += 1;
    
        batch = &RSGL_renderInfo.batches[RSGL_renderInfo.len - 1];
        batch->start = RSGL_renderInfo.vert_len;
        batch->len = 0;
        batch->type = type;
        batch->tex = RSGL_args.texture;
        batch->lineWidth = RSGL_args.lineWidth;
        
        batch->matrix = RSGL_renderInfo.matrix;
        batch->matrix = RSGL_mat4Multiply(batch->matrix.m, RSGL_customMatrix.m);
    } else {
        batch = &RSGL_renderInfo.batches[RSGL_renderInfo.len - 1];
    }

    if (batch == NULL)
        return;

    batch->len += len;

    memcpy(&RSGL_renderInfo.verts[RSGL_renderInfo.vert_len * 3], points, len * sizeof(float) * 3);
    memcpy(&RSGL_renderInfo.texCoords[RSGL_renderInfo.vert_len * 2], texPoints, len * sizeof(float) * 2);

    float color[4] = {c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f};

    if (RSGL_args.gradient_len && RSGL_args.gradient && (i64)(len - 1) > 0) {
        memcpy(&RSGL_renderInfo.colors[RSGL_renderInfo.vert_len * 4], color, sizeof(float) * 4);
        memcpy(&RSGL_renderInfo.colors[RSGL_renderInfo.vert_len * 4 + 4], RSGL_args.gradient, (len - 1) * sizeof(float) * 4);
    }
    else {
        size_t i;
        for (i = 0; i < len * 4; i += 4)
            memcpy(&RSGL_renderInfo.colors[(RSGL_renderInfo.vert_len * 4) + i], color, sizeof(float) * 4);
    }

    RSGL_renderInfo.vert_len += len;

    if (RSGL_argsClear) {
        RSGL_setTexture(0);
        RSGL_clearArgs();
    }
}

/*
*********************
RSGL_GRAPHICS_CONTEXT
*********************
*/

RSGL_renderer RSGL_currentRenderer;
void RSGL_setRenderer(RSGL_renderer renderer) { RSGL_currentRenderer = renderer; }
void RSGL_setFramebuffer(u32 id) { RSGL_currentRenderer.setFramebuffer(id); }

void RSGL_renderBatch(RSGL_RENDER_INFO* info) {  RSGL_currentRenderer.batch(info); }
void RSGL_renderInit(void* proc, RSGL_RENDER_INFO* info) { RSGL_currentRenderer.init(proc, info); }
void RSGL_renderFree(void) { RSGL_currentRenderer.free(); }
void RSGL_renderClear(float r, float g, float b, float a) { RSGL_currentRenderer.clear(r, g, b, a); }
void RSGL_renderViewport(i32 x, i32 y, i32 w, i32 h) { RSGL_currentRenderer.viewport(x, y, w, h); }
RSGL_framebuffer RSGL_renderCreateFramebuffer(RSGL_area memsize) { 
    return RSGL_currentRenderer.createFramebuffer(memsize); 
}
RSGL_texture RSGL_renderCreateTexture(u8* bitmap, RSGL_area memsize,  u8 channels) { 
    return RSGL_currentRenderer.createTexture(bitmap,  memsize, channels); 
}
void RSGL_renderUpdateTexture(RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels) { 
    return RSGL_currentRenderer.updateTexture(texture, bitmap, memsize, channels);
}
void RSGL_renderDeleteTexture(RSGL_texture tex) { RSGL_currentRenderer.deleteTexture(tex); }
void RSGL_renderDeleteFramebuffer(RSGL_framebuffer fbo) { RSGL_currentRenderer.deleteFramebuffer(fbo); }
void RSGL_renderScissorStart(RSGL_rectF scissor) { RSGL_currentRenderer.scissorStart(scissor); }
void RSGL_renderScissorEnd(void) { RSGL_currentRenderer.scissorEnd(); }
RSGL_programInfo RSGL_renderCreateProgram(const char* VShaderCode, const char* FShaderCode, const char* posName, const char* texName, const char* colorName) {
    return RSGL_currentRenderer.createProgram(VShaderCode, FShaderCode, posName, texName, colorName);
}
void RSGL_renderDeleteProgram(RSGL_programInfo program) { return RSGL_currentRenderer.deleteProgram(program); }
void RSGL_renderSetShaderValue(u32 program, char* var, float value[], u8 len) {
    return RSGL_currentRenderer.setShaderValue(program, var, value, len);
}

#ifdef RSGL_USE_COMPUTE
RSGL_programInfo RSGL_renderCreateComputeProgram(const char* CShaderCode) {
	return RSGL_currentRenderer.createComputeProgram(CShaderCode);
}

void RSGL_renderDispatchComputeProgram(RSGL_programInfo program, u32 groups_x, u32 groups_y, u32 groups_z) {
	RSGL_currentRenderer.dispatchComputeProgram(program, groups_x, groups_y, groups_z);
}

void RSGL_renderBindComputeTexture(u32 texture, u8 format) {
	RSGL_currentRenderer.bindComputeTexture(texture, format);
}
#endif

void RSGL_init(RSGL_area r, void* loader, RSGL_renderer renderer) {
    RSGL_setRenderer(renderer);

    RSGL_clearArgs();
    RFontcolor = RSGL_RGBA(0, 0, 0, 0); 
    RSGL_renderViewport(0, 0, r.w, r.h);

    RSGL_customMatrix = RSGL_loadIdentity();
    RSGL_args.currentArea = r;

    #ifndef RSGL_NO_TEXT
    RFont_init(r.w, r.h);
    #endif

    RSGL_renderInfo.len = 0;
    RSGL_renderInfo.vert_len = 0;
    RSGL_renderInfo.batches = (RSGL_BATCH*)RSGL_MALLOC(sizeof(RSGL_BATCH) * RSGL_MAX_BATCHES);
    RSGL_renderInfo.verts = (float*)RSGL_MALLOC(sizeof(float) * RSGL_MAX_VERTS * 2);
    RSGL_renderInfo.colors = (float*)RSGL_MALLOC(sizeof(float) * RSGL_MAX_VERTS * 4);
    RSGL_renderInfo.texCoords = (float*)RSGL_MALLOC(sizeof(float) * RSGL_MAX_VERTS * 2);
    RSGL_renderInfo.matrix = RSGL_ortho(RSGL_loadIdentity().m, 0, r.w, r.h, 0, 0, 1.0);

    RSGL_renderInit(loader, &RSGL_renderInfo);
}

void RSGL_draw(void) {
    RSGL_renderBatch(&RSGL_renderInfo);
}

void RSGL_clear(RSGL_color color) {
    RSGL_renderClear(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
}

void RSGL_updateSize(RSGL_area r) {
    RSGL_args.currentArea = r;
    RFont_update_framebuffer(r.w, r.h);
    RSGL_renderViewport(0, 0, r.w, r.h);

    RSGL_renderInfo.matrix = RSGL_ortho(RSGL_loadIdentity().m, 0, r.w, r.h, 0, 0, 1.0);
}

void RSGL_free() {
    if (RSGL_renderInfo.batches != NULL) {
        RSGL_renderFree();
        RSGL_FREE(RSGL_renderInfo.batches);
        RSGL_FREE(RSGL_renderInfo.verts);
        RSGL_FREE(RSGL_renderInfo.colors);
        RSGL_FREE(RSGL_renderInfo.texCoords);
        RSGL_renderInfo.batches = NULL;
        RSGL_renderInfo.len = 0;
        RSGL_renderInfo.vert_len = 0;
    }
}

/* 
****
RSGL_draw
****
*/

/* RSGL_args */
void RSGL_setRotate(RSGL_point3D rotate){
    RSGL_args.rotate = RSGL_POINT3D(rotate.x * DEG2RAD, rotate.y * DEG2RAD, rotate.z * DEG2RAD);
}
void RSGL_setTexture(RSGL_texture texture) { 
    RSGL_args.texture = texture;
}
void RSGL_setProgram(u32 program) { 
    if (RSGL_args.program != program) {
        /* render using the current program */
        RSGL_renderBatch(&RSGL_renderInfo);
        RSGL_args.program = program;
    }
}

void RSGL_setGradient(float gradient[], size_t len) {
    RSGL_args.gradient_len = len;
    RSGL_args.gradient = gradient;
}
void RSGL_setCenter(RSGL_point3D center) {
    RSGL_args.center = center;
}
void RSGL_setClearArgs(RSGL_bool clearArgs) {
    RSGL_argsClear = clearArgs;
}

#ifndef my_RSGL_clearArgs 
void RSGL_clearArgs(void) {
    RSGL_args = (RSGL_drawArgs){NULL, 0, 0, { }, {0, 0, 0}, RSGL_POINT3D(-1, -1, -1), 0, 0};
}
#endif


void RSGL_drawPoint(RSGL_point p, RSGL_color c) {
    RSGL_drawPointF((RSGL_pointF){(float)p.x, (float)p.y}, c);
}

void RSGL_drawTriangle(RSGL_triangle t, RSGL_color c) {
    RSGL_drawTriangleF(RSGL_createTriangleF((float)t.p1.x, (float)t.p1.y, (float)t.p2.x, (float)t.p2.y, (float)t.p3.x, (float)t.p3.y), c);
}

void RSGL_drawRect(RSGL_rect r, RSGL_color c) {
    RSGL_drawRectF((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, c);
}

void RSGL_drawRoundRect(RSGL_rect r, RSGL_point rounding, RSGL_color c) {
    RSGL_drawRoundRectF((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, rounding, c);
}

void RSGL_drawPolygon(RSGL_rect r, u32 sides, RSGL_color c) {
    RSGL_drawPolygonF((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, sides, c);
}

void RSGL_drawArc(RSGL_rect o, RSGL_point arc, RSGL_color color) {
    RSGL_drawArcF((RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, (RSGL_pointF){(float)arc.x, (float)arc.y}, color);
}

void RSGL_drawCircle(RSGL_circle c, RSGL_color color) {
    RSGL_drawCircleF((RSGL_circleF){(float)c.x, (float)c.y, (float)c.d}, color);
}

void RSGL_drawOval(RSGL_rect o, RSGL_color c) {
    RSGL_drawOvalF((RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, c);
}

void RSGL_drawLine(RSGL_point p1, RSGL_point p2, u32 thickness, RSGL_color c) {
    RSGL_drawLineF((RSGL_pointF){(float)p1.x, (float)p1.y}, (RSGL_pointF){(float)p2.x, (float)p2.y}, thickness, c);
}

void RSGL_drawTriangleOutline(RSGL_triangle t, u32 thickness, RSGL_color c) {
    RSGL_drawTriangleFOutline(RSGL_createTriangleF((float)t.p1.x, (float)t.p1.y, (float)t.p2.x, (float)t.p2.y, (float)t.p3.x, (float)t.p3.y), thickness, c);
}

void RSGL_drawRectOutline(RSGL_rect r, u32 thickness, RSGL_color c) {
    RSGL_drawRectFOutline((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, thickness, c);
}

void RSGL_drawRoundRectOutline(RSGL_rect r, RSGL_point rounding, u32 thickness, RSGL_color c) {
    RSGL_drawRoundRectFOutline((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, rounding, thickness, c);
}

void RSGL_drawPolygonOutline(RSGL_rect r, u32 sides, u32 thickness, RSGL_color c) {
    RSGL_drawPolygonFOutline((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, sides, thickness, c);
}

void RSGL_drawArcOutline(RSGL_rect o, RSGL_point arc, u32 thickness, RSGL_color color) {
    RSGL_drawArcFOutline((RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, (RSGL_pointF){(float)arc.x, (float)arc.y}, thickness, color);
}

void RSGL_drawCircleOutline(RSGL_circle c, u32 thickness, RSGL_color color) {
    RSGL_drawCircleFOutline((RSGL_circleF){(float)c.x, (float)c.y, (float)c.d}, thickness, color);
}

void RSGL_drawOvalOutline(RSGL_rect o, u32 thickness, RSGL_color c) {
    RSGL_drawOvalFOutline((RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, thickness, c);
}

void RSGL_drawPointF(RSGL_pointF p, RSGL_color c) {
    RSGL_drawRectF((RSGL_rectF){p.x, p.y, 1.0f, 1.0f}, c);
}

void RSGL_drawTriangleF(RSGL_triangleF t, RSGL_color c) {
    RSGL_point3D center = {t.p3.x, (t.p3.y + t.p1.y) / 2.0f, 0};
    RSGL_mat4 matrix = RSGL_initDrawMatrix(center);
    
    float points[] = {RSGL_GET_MATRIX_POINT((float)t.p1.x, (float)t.p1.y, 0.0f), 
                      RSGL_GET_MATRIX_POINT((float)t.p2.x, (float)t.p2.y, 0.0f), 
                      RSGL_GET_MATRIX_POINT((float)t.p3.x, (float)t.p3.y, 0.0f)};
    
    float texPoints[] = {   
                0.0f, 1.0f, 
                1.0f, 1.0f,
                ((float)(t.p3.x - t.p1.x)/t.p2.x < 1) ? (float)(t.p3.x - t.p1.x) / t.p2.x : 0, 0.0f,
    };
    
    RSGL_basicDraw(RSGL_TRIANGLES, (float*)points, (float*)texPoints, c, 3);
}

void RSGL_drawTriangleHyp(RSGL_pointF p, size_t angle, float hypotenuse, RSGL_color color) {
    float dir = (hypotenuse > 0);
    hypotenuse = fabsf(hypotenuse);

    float base = hypotenuse * (RSGL_COS(angle) * DEG2RAD);
    float opp = hypotenuse * (RSGL_SIN(angle) * DEG2RAD); 
    
    RSGL_triangleF t = RSGL_TRIANGLEF(
        p,
        RSGL_POINTF(p.x + base, p.y),
        RSGL_POINTF(p.x + (base * dir), p.y - opp)
    );
    
    RSGL_drawTriangleF(t, color);   
}

void RSGL_drawRectF(RSGL_rectF r, RSGL_color c) {
    float texPoints[] = {
                                0.0f, 0.0f,
                                0.0f, 1.0f,
                                1.0f, 0.0f,
                                1.0f, 1.0f,
                                1.0f, 0.0f, 
                                0.0f, 1.0f
                            };

    RSGL_point3D center = (RSGL_point3D){r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f};
    RSGL_mat4 matrix = RSGL_initDrawMatrix(center);

    float points[] = {
                                RSGL_GET_MATRIX_POINT(r.x, r.y, 0.0f), 
                                RSGL_GET_MATRIX_POINT(r.x, r.y + r.h, 0.0f),       
                                RSGL_GET_MATRIX_POINT(r.x + r.w, r.y, 0.0f), 

                                RSGL_GET_MATRIX_POINT(r.x + r.w, r.y + r.h, 0.0f),        
                                RSGL_GET_MATRIX_POINT(r.x + r.w, r.y, 0.0f),     
                                RSGL_GET_MATRIX_POINT(r.x, r.y + r.h, 0.0f),  
                            };

    RSGL_basicDraw(RSGL_TRIANGLES, (float*)points, (float*)texPoints, c, 6);
}

void RSGL_drawRoundRectF(RSGL_rectF r, RSGL_point rounding, RSGL_color c) {
    RSGL_drawRect(RSGL_RECT(r.x + (rounding.x / 2), r.y, r.w - rounding.x, r.h), c);
    RSGL_drawRect(RSGL_RECT(r.x, r.y + (rounding.y / 2), r.w,  r.h - rounding.y), c);

    RSGL_drawArc(RSGL_RECT(r.x, r.y, rounding.x, rounding.y), (RSGL_point){180, 270}, c);
    RSGL_drawArc(RSGL_RECT(r.x + (r.w - rounding.x), r.y, rounding.x, rounding.y), (RSGL_point){90, 180}, c);
    RSGL_drawArc(RSGL_RECT(r.x + (r.w - rounding.x), r.y  + (r.h - rounding.y), rounding.x, rounding.y), (RSGL_point){0, 90}, c);
    RSGL_drawArc(RSGL_RECT(r.x, r.y  + (r.h - rounding.y),  rounding.x, rounding.y), (RSGL_point){270, 360}, c);
}

void RSGL_drawPolygonFOutlineEx(RSGL_rectF o, u32 sides, RSGL_pointF arc, RSGL_color c);

void RSGL_drawPolygonFEx(RSGL_rectF o, u32 sides, RSGL_pointF arc, RSGL_color c) {
    static float verts[360 * 3];
    static float texcoords[360 * 2];

    RSGL_point3D center =  (RSGL_point3D){o.x + (o.w / 2.0f), o.y + (o.h / 2.0f), 0};
    
o = (RSGL_rectF){o.x, o.y, o.w / 2, o.h / 2};    
    RSGL_mat4 matrix = RSGL_initDrawMatrix(center);

    float displacement = 360.0f / (float)sides;
    float angle = displacement * arc.x;
    
    size_t vIndex = 0;
    size_t tIndex = 0; 

    u32 i;
    for (i = 0; i < sides; i++) {
        RSGL_pointF p = {RSGL_SIN(angle * DEG2RAD), RSGL_COS(angle * DEG2RAD)};

        texcoords[tIndex] = (p.x + 1.0f) * 0.5;
        texcoords[tIndex + 1] = (p.y + 1.0f) * 0.5;

        memcpy(verts + vIndex, (float[3]){RSGL_GET_MATRIX_POINT(o.x + o.w + (p.x * o.w), o.y + o.h + (p.y * o.h), 0.0)}, 3 * sizeof(float));

        angle += displacement;
        tIndex += 2;
        vIndex += 3;
    }

    texcoords[tIndex + 1] = 0;
    texcoords[tIndex + 2] = 0;

    RSGL_basicDraw(RSGL_TRIANGLE_FAN, verts, texcoords, c, vIndex / 3);
}

void RSGL_drawPolygonF(RSGL_rectF o, u32 sides, RSGL_color c) { RSGL_drawPolygonFEx(o, sides, (RSGL_pointF){0, (float)sides}, c); }


void RSGL_drawArcF(RSGL_rectF o, RSGL_pointF arc, RSGL_color color) {  
    u32 verts = (u32)((float)((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10) + 0.5);
    verts %= 360;
    
    RSGL_drawPolygonFEx(o, verts, arc, color); 
}

void RSGL_drawCircleF(RSGL_circleF c, RSGL_color color) {  
    u32 verts = (u32)((2.0 * M_PI * c.d) / 10) % 360;

    RSGL_drawPolygonFEx((RSGL_rectF){c.x, c.y, c.d, c.d}, verts, (RSGL_pointF){0, (float)verts}, color); 
}

void RSGL_drawOvalF(RSGL_rectF o, RSGL_color c) { 
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_drawPolygonFEx(o, verts, (RSGL_pointF){0, verts}, c); 
}

/*
    3D
*/

void RSGL_drawPoint3D(RSGL_point3D p, RSGL_color c) {
    RSGL_drawCube((RSGL_cube){p.x, p.y, p.z, 1.0f, 1.0f, 1.0f}, c);
}

void RSGL_drawLine3D(RSGL_point3D p1, RSGL_point3D p2, u32 thickness, RSGL_color c) {
    RSGL_args.lineWidth = thickness;
    
    RSGL_point3D center = {(p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f, (p1.z + p2.z) / 2.0f};
    RSGL_mat4 matrix = RSGL_initDrawMatrix(center);

    float points[] = {RSGL_GET_MATRIX_POINT(p1.x, p1.y, p1.z), RSGL_GET_MATRIX_POINT(p2.x, p2.y, p2.z)};
    float texPoints[] = {0, 0.0f,          0, 0.0f};

    RSGL_basicDraw(RSGL_LINES, (float*)points, (float*)texPoints, c, 2);
}

void RSGL_drawTriangle3D(RSGL_triangle3D t, RSGL_color c) {
    RSGL_mat4 matrix = RSGL_initDrawMatrix(RSGL_POINT3D(200, 400, 1));
     
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

    RSGL_basicDraw(RSGL_TRIANGLES, (float*)points, (float*)texPoints, c, 3);
}

void RSGL_drawCube(RSGL_cube cube, RSGL_color c) {
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

    RSGL_mat4 matrix = RSGL_initDrawMatrix(center);

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

    RSGL_basicDraw(RSGL_TRIANGLES, points, texPoints, c, 36);
}

/* 
outlines
*/

void RSGL_drawLineF(RSGL_pointF p1, RSGL_pointF p2, u32 thickness, RSGL_color c) {
    RSGL_args.lineWidth = thickness;
    
    RSGL_point3D center = {(p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f, 0.0f};
    RSGL_mat4 matrix = RSGL_initDrawMatrix(center);

    float points[] = {RSGL_GET_MATRIX_POINT(p1.x, p1.y, 0.0f), RSGL_GET_MATRIX_POINT(p2.x, p2.y, 0.0f)};
    float texPoints[] = {0, 0.0f,          0, 0.0f};

    RSGL_basicDraw(RSGL_LINES, (float*)points, (float*)texPoints, c, 2);
}

void RSGL_drawTriangleFOutline(RSGL_triangleF t, u32 thickness, RSGL_color c) {
    RSGL_args.lineWidth = thickness;
    RSGL_point3D center = {t.p3.x, (t.p3.y + t.p1.y) / 2.0f, 0};
    RSGL_mat4 matrix = RSGL_initDrawMatrix(center);

    float points[] = {RSGL_GET_MATRIX_POINT(t.p3.x, t.p3.y, 0.0f), 
                        RSGL_GET_MATRIX_POINT(t.p1.x, t.p1.y, 0.0f),     
                        RSGL_GET_MATRIX_POINT(t.p1.x, t.p1.y, 0.0f), 
                    RSGL_GET_MATRIX_POINT(t.p2.x, t.p2.y, 0.0f),     
                    RSGL_GET_MATRIX_POINT(t.p2.x, t.p2.y, 0.0f),     
                    RSGL_GET_MATRIX_POINT(t.p3.x, t.p3.y, 0.0f)};
    
    float texCoords[18];

    RSGL_basicDraw(RSGL_LINES, (float*)points, texCoords, c, 6);
}
void RSGL_drawRectFOutline(RSGL_rectF r, u32 thickness, RSGL_color c) {
    RSGL_point3D oCenter = RSGL_args.center;

    RSGL_setCenter((RSGL_point3D){r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f});
    RSGL_drawLineF((RSGL_pointF){r.x, r.y}, (RSGL_pointF){r.x + r.w, r.y}, thickness, c);

    RSGL_setCenter((RSGL_point3D){r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f});
    RSGL_drawLineF((RSGL_pointF){r.x, r.y}, (RSGL_pointF){r.x, r.y + r.h}, thickness, c);

    RSGL_setCenter((RSGL_point3D){r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f});
    RSGL_drawLineF((RSGL_pointF){r.x, r.y + r.h}, (RSGL_pointF){r.x + r.w, r.y + r.h}, thickness, c);

    RSGL_setCenter((RSGL_point3D){r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f});
    RSGL_drawLineF((RSGL_pointF){r.x + r.w, r.y}, (RSGL_pointF){r.x + r.w, r.y + r.h}, thickness, c);

    RSGL_setCenter(oCenter);
}
void RSGL_drawRoundRectFOutline(RSGL_rectF r, RSGL_point rounding, u32 thickness, RSGL_color c) {
    RSGL_drawRectF(RSGL_RECTF(r.x + (rounding.x/2), r.y, r.w - rounding.x, (int)(thickness + !thickness)), c);
    RSGL_drawRectF(RSGL_RECTF(r.x + (rounding.x/2), r.y + r.h, r.w - rounding.x, (int)(thickness + !thickness)), c);    
    RSGL_drawRectF(RSGL_RECTF(r.x, r.y + (rounding.y/2), (int)(thickness + !thickness),  r.h - rounding.y), c);
    RSGL_drawRectF(RSGL_RECTF(r.x + r.w, r.y + (rounding.y/2), (int)(thickness + !thickness),  r.h - rounding.y), c);

    RSGL_drawArcFOutline(RSGL_RECTF(r.x, r.y, rounding.x, rounding.y), (RSGL_pointF){180, 270}, thickness, c);
    RSGL_drawArcFOutline(RSGL_RECTF(r.x + (r.w - rounding.x), r.y, rounding.x, rounding.y), (RSGL_pointF){90, 180}, thickness, c);
    RSGL_drawArcFOutline(RSGL_RECTF(r.x + (r.w - rounding.x), r.y  + (r.h - rounding.y) - 1, rounding.x, rounding.y + 2), (RSGL_pointF){0, 90}, thickness, c);
    RSGL_drawArcFOutline(RSGL_RECTF(r.x + 1, r.y  + (r.h - rounding.y) - 1,  rounding.x, rounding.y + 2), (RSGL_pointF){270, 360}, thickness, c);
}

void RSGL_drawPolygonFOutlineEx(RSGL_rectF o, u32 sides, RSGL_pointF arc, RSGL_color c) {
    static float verts[360 * 2 * 3];
    static float texCoords[360 * 2 * 2];

    RSGL_point3D center = (RSGL_point3D) {o.x + (o.w / 2.0f), o.y + (o.h / 2.0f), 0.0f};
        RSGL_mat4 matrix = RSGL_initDrawMatrix(center);

    o = (RSGL_rectF){o.x + (o.w / 2), o.y + (o.h / 2), o.w / 2, o.h / 2};
    
    float displacement = 360.0f / (float)sides;
    float centralAngle = displacement * arc.x;

    i32 i;
    u32 j;
    size_t index = 0;

    for (i = arc.x; i < arc.y; i++) {
        for (j = 0; j < 2; j++) {
            memcpy(verts + index, (float[3]) {
                        RSGL_GET_MATRIX_POINT(
                            o.x + (RSGL_SIN(DEG2RAD * centralAngle) * o.w),
                            o.y + (RSGL_COS(DEG2RAD * centralAngle) * o.h),
                            (0.0))
                        }, sizeof(float) * 3);
            
            if (!j) centralAngle += displacement;
            index += 3;
        }
    }

    RSGL_basicDraw(RSGL_LINES, verts, texCoords, c, index / 3);
}

void RSGL_drawPolygonFOutline(RSGL_rectF o, u32 sides, u32 thickness, RSGL_color c) {
    RSGL_args.lineWidth = thickness;
    RSGL_drawPolygonFOutlineEx(o, sides, (RSGL_pointF){0, (float)sides}, c);
}
void RSGL_drawArcFOutline(RSGL_rectF o, RSGL_pointF arc, u32 thickness, RSGL_color color) {
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_args.lineWidth = thickness;
    RSGL_drawPolygonFOutlineEx(o, verts, arc, color);
}
void RSGL_drawCircleFOutline(RSGL_circleF c, u32 thickness, RSGL_color color) {
    float verts = ((2 * M_PI * c.d) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_args.lineWidth = thickness;
    RSGL_drawPolygonFOutlineEx((RSGL_rectF){c.x, c.y, c.d, c.d}, verts, (RSGL_pointF){0, verts}, color);
}
void RSGL_drawOvalFOutline(RSGL_rectF o, u32 thickness, RSGL_color c) {
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_args.lineWidth = thickness;
    RSGL_drawPolygonFOutlineEx(o, verts, (RSGL_pointF){0, verts}, c);
}

#ifndef RSGL_NO_TEXT
RSGL_font* RSGL_internalFont = NULL;

RSGL_font* RSGL_loadFont(const char* font) {
    return RSGL_loadFontEx(font, RFONT_ATLAS_WIDTH_DEFAULT, RFONT_ATLAS_HEIGHT_DEFAULT);
}

RFont_font* RSGL_loadFontEx(const char* font, size_t atlasWidth, size_t atlasHeight) {
    return RFont_font_init_ex(font, atlasWidth, atlasHeight);
}

void RSGL_freeFont(RSGL_font* font) { RFont_font_free(font); }

void RSGL_setFont(RFont_font* font) {
    if (font == NULL) {

#ifdef RSGL_DEBUG
        printf("RSGL_setFont : invalid font\n");
#endif

        return;
    }
    
    RSGL_internalFont = font;
}

void RSGL_drawText(const char* text, RSGL_circle c, RSGL_color color) {
    RSGL_drawText_pro(text, strlen(text), 0.0f, c, color);
}

void RSGL_drawText_len(const char* text, size_t len, RSGL_circle c, RSGL_color color) {
    RSGL_drawText_pro(text, len, 0.0f, c, color);
}

void RSGL_drawText_pro(const char* text, size_t len, float spacing, RSGL_circle c, RSGL_color color) {
    if (text == NULL || RSGL_internalFont == NULL)
        return;
    
    RFont_set_color(color.r / 255.0f, color.b / 255.0f, color.g / 255.0f, color.a / 255.0f);
    RFont_draw_text_len(RSGL_internalFont, text, len, c.x, c.y, c.d, spacing);
}

RSGL_area RSGL_textArea(const char* text, u32 fontSize, size_t textEnd) {
    if (RSGL_internalFont == NULL)
        return RSGL_AREA(0, 0);
    
    return RFont_text_area_len(RSGL_internalFont, text, textEnd, fontSize, 0, 0.0);
}

RSGL_area RSGL_textLineArea(const char* text, u32 fontSize, size_t textEnd, size_t line) {
    return RFont_text_area_len(RSGL_internalFont, text, textEnd, fontSize, line, 0.0);
}

RFont_texture RFont_create_atlas(u32 atlasWidth, u32 atlasHeight) {
    return RSGL_currentRenderer.createAtlas(atlasWidth, atlasHeight);
}

b8 RFont_resize_atlas(RFont_texture* atlas, u32 newWidth, u32 newHeight) {
    return RSGL_currentRenderer.resizeAtlas(atlas, newWidth, newHeight); 
}

void RFont_bitmap_to_atlas(RFont_texture atlas, u8* bitmap, float x, float y, float w, float h) {
    return RSGL_currentRenderer.bitmapToAtlas(atlas, bitmap, x, y, w, h);
}


void RFont_render_set_color(float r, float g, float b, float a) {
    RFontcolor = RSGL_RGBA(r * 255, g * 255, b * 255, a * 255);
}

void RFont_render_text(RFont_texture atlas, float* verts, float* tcoords, size_t nverts) {
    RSGL_drawArgs save = RSGL_args;
    RSGL_setRotate(RSGL_POINT3D(0, 0, 0));
    RSGL_setTexture(atlas);
    RSGL_basicDraw(RSGL_TRIANGLES, verts, tcoords, RFontcolor, nverts);
    RSGL_args = save;
}

void RFont_render_init(void) { }

void RFont_render_free(RFont_texture atlas) {
   RSGL_renderDeleteTexture(atlas);
}

void RFont_render_legacy(u8 legacy) { RSGL_UNUSED(legacy); }
#endif /* RSGL_NO_TEXT */

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

/* Multiply the current matrix by a rotation matrix */
RSGL_mat4 RSGL_getCameraMatrix(RSGL_camera camera) {
    return RSGL_getCameraMatrixEx(camera, (16.0 / 9.0), 90, 0.001, 1000.0);
}

RSGL_mat4 RSGL_getCameraMatrixEx(RSGL_camera camera, float ratio, float maxPitch, float min, float max) {
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
