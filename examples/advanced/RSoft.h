/* TODO
- textures for poylgons
- textures for triangles
*/



/*
* Copyright (C) 2024 ColleagueRiley
*
* libpng license
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
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*
*
*/


#ifndef RSOFT_MALLOC
	#include <stdlib.h>

	#ifndef __USE_POSIX199309
	#define __USE_POSIX199309
	#endif

	#include <time.h>
	#define RSOFT_MALLOC malloc
	#define RSOFT_CALLOC calloc
	#define RSOFT_FREE free
#endif


#if !_MSC_VER
	#ifndef inline
		#ifndef __APPLE__
			#define inline __inline
		#endif
	#endif
#endif

#if defined(RSOFT_EXPORT) ||  defined(RSOFT_IMPORT)
	#if defined(_WIN32)
		#if defined(__TINYC__) && (defined(RSOFT_EXPORT) ||  defined(RSOFT_IMPORT))
			#define __declspec(x) __attribute__((x))
		#endif

		#if defined(RSOFT_EXPORT)
			#define RSOFTDEF __declspec(dllexport)
		#else 
			#define RSOFTDEF __declspec(dllimport)
		#endif
	#else
		#if defined(RSOFT_EXPORT)
			#define RSOFTDEF __attribute__((visibility("default")))
		#endif
	#endif
#endif 

#ifndef RSOFTDEF
	#ifdef __clang__
		#define RSOFTDEF static inline
	#else
		#define RSOFTDEF inline
	#endif
#endif

#ifndef RSOFT_ENUM
	#define RSOFT_ENUM(type, name) type name; enum
#endif

#ifndef RSOFT_UNUSED
	#define RSOFT_UNUSED(x) (void)(x);
#endif

#if defined(__cplusplus) && !defined(__EMSCRIPTEN__)
	extern "C" {
#endif

	/* makes sure the header file part is only defined once by default */
#ifndef RSOFT_HEADER

#define RSOFT_HEADER

#if !defined(u8)
	#if ((defined(_MSC_VER) || defined(__SYMBIAN32__)) && !defined(RSOFT_STD_INT)) /* MSVC might not have stdint.h */
		typedef unsigned char 	u8;
		typedef signed char		i8;
		typedef unsigned short  u16;
		typedef signed short 	i16;
		typedef unsigned int 	u32;
		typedef signed int		i32;
		typedef unsigned long	u64;
		typedef signed long		i64;
	#else /* use stdint standard types instead of c ""standard"" types */
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

#if !defined(b8) /* RSOFT bool type */
	typedef u8 b8;
	typedef u32 b32;
#endif

#define RSOFT_TRUE 1
#define RSOFT_FALSE 0

#ifndef RSoft_point
typedef struct RSoft_point { i32 x, y, z; } RSoft_point;
#endif

#define RSOFT_POINT(x, y) (RSoft_point){x, y}

#ifndef RSoft_area
typedef struct RSoft_area { u32 w, h; } RSoft_area;
#endif

#define RSOFT_AREA(w, h) (RSoft_area){w, h}

#ifndef RSoft_rect
typedef struct RSoft_rect { i32 x, y;  u32 w, h; } RSoft_rect;
#endif

#define RSOFT_RECT(x, y, w, h) (RSoft_rect){x, y, w, h}

#ifndef RSoft_vector
typedef struct RSoft_vector { float x, y, z; } RSoft_vector;
#endif

#define RSOFT_VECTOR2D(x, y) (RSoft_vector){x, y, 1}
#define RSOFT_VECTOR3D(x, y, z) (RSoft_vector){x, y, z}

#ifndef RSoft_rectF
typedef struct RSoft_rectF { float x, y, w, h; } RSoft_rectF;
#endif

#define RSOFT_RECTF(x, y, w, h) (RSoft_rectF){x, y, w, h}

#include <math.h>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif


#ifndef DEG2RAD
#define DEG2RAD M_PI/180
#endif

/* LUT based trig functions (uses degrees) */
RSOFTDEF float RSoft_sin(i32 angle);
RSOFTDEF float RSoft_cos(i32 angle);

 // 4 * 4 matrix
typedef struct RSoft_matrix { float m[16];} RSoft_matrix;

RSOFTDEF void RSoft_setCanvasSize(RSoft_area area);
RSOFTDEF void RSoft_setBufferSize(RSoft_area area);

RSOFTDEF void RSoft_copyBuffer(u8* dest, size_t width, u8* src, RSoft_rect rect);

RSOFTDEF void RSoft_setMatrix(RSoft_matrix matrix);
RSOFTDEF RSoft_matrix RSoft_initMatrix(void);
RSOFTDEF RSoft_matrix RSoft_multiplyMatrix(float left[16], float right[16]);
RSOFTDEF RSoft_matrix RSoft_simpleRotateMatrix(RSoft_matrix matrix, float angle);
RSOFTDEF RSoft_matrix RSoft_rotateMatrix(RSoft_matrix matrix, float angle, float x, float y, float z);
RSOFTDEF RSoft_matrix RSoft_translateMatrix(RSoft_matrix matrix, RSoft_vector v);
RSOFTDEF RSoft_vector RSoft_applyMatrix(RSoft_matrix matrix, RSoft_vector v);

RSOFTDEF void RSoft_setTexture(u8* texture, RSoft_rect texRect, RSoft_area textureArea);
RSOFTDEF u32 RSoft_textureGetColor(RSoft_point texPoint, u8 color[4]);

RSOFTDEF void RSoft_clear(u8* buffer, u8 color[4]);

RSOFTDEF void RSoft_drawRect(u8* buffer, RSoft_rect r, u8 color[4]);
RSOFTDEF void RSoft_drawRectF(u8* buffer, RSoft_rectF r, u8 color[4]);

RSOFTDEF void RSoft_drawRectOutline(u8* buffer, RSoft_rect r, u8 color[4]);
RSOFTDEF void RSoft_drawRectFOutline(u8* buffer, RSoft_rectF r, u8 color[4]);

RSOFTDEF void RSoft_drawTriangleF(u8* buffer, const RSoft_vector points[3], u8 color[4]);
RSOFTDEF void RSoft_drawTriangle(u8* buffer, const RSoft_point points[3], u8 color[4]);

RSOFTDEF void RSoft_drawTriangleOutline(u8* buffer, const RSoft_vector points[3], u8 color[4]);
RSOFTDEF void RSoft_drawTriangleOutlineF(u8* buffer, const RSoft_vector points[3], u8 color[4]);

RSOFTDEF void RSoft_drawPolygon(u8* buffer, RSoft_rect r, size_t angles, u8 color[4]);
RSOFTDEF void RSoft_drawPolygonF(u8* buffer, RSoft_rectF r, size_t angles, u8 color[4]);

RSOFTDEF void RSoft_drawPolygonFOutline(u8* buffer, RSoft_rectF r, size_t angles, u8 color[4]);
RSOFTDEF void RSoft_drawPolygonOutline(u8* buffer, RSoft_rect r, size_t angles, u8 color[4]);

RSOFTDEF void RSoft_drawVector(u8* buffer, RSoft_vector v, u8 color[4]);
RSOFTDEF void RSoft_drawPoint(u8* buffer, RSoft_point p, u8 color[4]);
RSOFTDEF void RSoft_drawLine(u8* buffer, RSoft_point start, RSoft_point end, u8 color[4]);
RSOFTDEF void RSoft_drawLineF(u8* buffer, RSoft_vector start, RSoft_vector end, u8 color[4]); 

#endif /* RSOFT_HEADER */

#ifdef RSOFT_IMPLEMENTATION

#ifdef __x86_64__
#define RSOFT_X86
#include <xmmintrin.h> 
#include <x86intrin.h>
#endif

typedef struct RSoft_renderInfoStruct {
	RSoft_area bufferSize;
	RSoft_area canvasSize;
	RSoft_matrix matrix;

	RSoft_rect texRect; 
	u8* texture; 
	RSoft_area textureArea;
} RSoft_renderInfoStruct;

RSoft_renderInfoStruct RSoft_renderInfo = {.matrix = (RSoft_matrix) {{
	/* x */  1.0f, 0.0f, 0.0f, 0.0f,
	/* y */	 0.0f, 1.0f, 0.0f, 0.0f,
	/* z */	 0.0f, 0.0f, 1.0f, 0.0f,
    /* w */  0.0f, 0.0f, 0.0f, 1.0f
    }},
	.texture = NULL
};

float RSoft_sin(i32 angle) {
	static float sinLUT[360] = {1};
	if (sinLUT[0]) {
		for (size_t i = 0; i < 360; i++)
			sinLUT[i] = sin(i * DEG2RAD);
	}

	while (angle < 0) angle += 360;	

	if (angle >= 360)
		angle -= 360 * (u32)(angle / 360);
	
	return sinLUT[angle];
}

float RSoft_cos(i32 angle) {
	static float cosLUT[360] = {0};
	if (cosLUT[0] == 0) {
		for (size_t i = 0; i < 360; i++)
			cosLUT[i] = cos(i * DEG2RAD);
	}
	
	while (angle < 0) angle += 360;
	
	if (angle >= 360)
		angle -= 360 * (u32)(angle / 360);

	return cosLUT[angle];
}

void RSoft_setCanvasSize(RSoft_area area) {
	RSoft_renderInfo.canvasSize = area;
}

void RSoft_setBufferSize(RSoft_area area) {
	RSoft_renderInfo.bufferSize = area;	
}

void RSoft_copyBuffer(u8* dest, size_t width, u8* src, RSoft_rect rect) {
	u32 y;
    for (y = 0; y < (u32)rect.h; y++) {
        u32 index = ((rect.y + y) * (4 * width)) + rect.x * 4;
        memcpy(dest + index, src + (4 * rect.w * y), rect.w * 4 * sizeof(u8));
    }
}

void RSoft_setMatrix(RSoft_matrix matrix) {
	RSoft_renderInfo.matrix = matrix;
}

	
RSoft_matrix RSoft_initMatrix(void) {
	/* sets the x, y, z, w base values to 1.0 (so we aren't multiplying by zero) */
	return (RSoft_matrix) {{
    //		 x	   y	 z	   w	
	/* x */  1.0f, 0.0f, 0.0f, 0.0f,
	/* y */	 0.0f, 1.0f, 0.0f, 0.0f,
	/* z */	 0.0f, 0.0f, 1.0f, 0.0f,
    /* w */  0.0f, 0.0f, 0.0f, 1.0f
    }};
}

RSoft_matrix RSoft_multiplyMatrix(float left[16], float right[16]) {
	return (RSoft_matrix) {{
		(left[0] * right[0]) + (left[1] * right[4]) + (left[2] * right[8]) + (left[3] * right[12]),
		(left[0] * right[1]) + (left[1] * right[5]) + (left[2] * right[9]) + (left[3] * right[13]),
		(left[0] * right[2]) + (left[1] * right[6]) + (left[2] * right[10]) + (left[3] * right[14]),
		(left[0] * right[3]) + (left[1] * right[7]) + (left[2] * right[11]) + (left[3] * right[15]),
		(left[4] * right[0]) + (left[5] * right[4]) + (left[6] * right[8]) + (left[7] * right[12]),
		(left[4] * right[1]) + (left[5] * right[5]) + (left[6] * right[9]) + (left[7] * right[13]),
		(left[4] * right[2]) + (left[5] * right[6]) + (left[6] * right[10]) + (left[7] * right[14]),
		(left[4] * right[3]) + (left[5] * right[7]) + (left[6] * right[11]) + (left[7] * right[15]),
		(left[8] * right[0]) + (left[9] * right[4]) + (left[10] * right[8]) + (left[11] * right[12]),
		(left[8] * right[1]) + (left[9] * right[5]) + (left[10] * right[9]) + (left[11] * right[13]),
		(left[8] * right[2]) + (left[9] * right[6]) + (left[10] * right[10]) + (left[11] * right[14]),
		(left[8] * right[3]) + (left[9] * right[7]) + (left[10] * right[11]) + (left[11] * right[15]),
		(left[12] * right[0]) + (left[13] * right[4]) + (left[14] * right[8]) + (left[15] * right[12]),
		(left[12] * right[1]) + (left[13] * right[5]) + (left[14] * right[9]) + (left[15] * right[13]),
		(left[12] * right[2]) + (left[13] * right[6]) + (left[14] * right[10]) + (left[15] * right[14]),
		(left[12] * right[3]) + (left[13] * right[7]) + (left[14] * right[11]) + (left[15] * right[15])
	}};
}

RSoft_matrix RSoft_rotateMatrix(RSoft_matrix matrix, float angle, float x, float y, float z) {
	/* Axis vector (x, y, z) normalization */
	float lengthSquared = x * x + y * y + z * z;
	if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f)) {
		float inverseLength = 1.0f / sqrtf(lengthSquared);
		x *= inverseLength;
		y *= inverseLength;
		z *= inverseLength;
	}

	/* Rotation matrix generation */
	float sinres = RSoft_sin(angle);
	float cosres = RSoft_cos(angle);
	float t = 1.0f - cosres;

	float rotateMatrix[16] = {
					x * x * t + cosres,   	  	y * x * t + z * sinres,   	z * x * t - y * sinres,   	0.0f,
					x * y * t - z * sinres,   	y * y * t + cosres,   		z * y * t + x * sinres,   	0.0f,
					x * z * t + y * sinres,   	y * z * t - x * sinres,  	z * z * t + cosres,   		0.0f,
					0.0f,   					0.0f,   					0.0f,   					1.0f
	};

	return RSoft_multiplyMatrix(matrix.m, rotateMatrix);
}

RSoft_matrix RSoft_simpleRotateMatrix(RSoft_matrix matrix, float angle) {
	float sinres = RSoft_sin(angle);
	float cosres = RSoft_cos(angle);
	
	/* this acts a 4 x 4 2D matrix */
	float rotateMatrix[16] = {
					cosres,						-sinres,					0.0f,   	0.0f,
					sinres,						cosres,						0.0f,		0.0f,
					0.0f,						0.0f,						1.0f,		0.0f,
					0.0f,   					0.0f,   					0.0f,   	1.0f
	};

	return RSoft_multiplyMatrix(matrix.m, rotateMatrix);
}

RSoft_matrix RSoft_translateMatrix(RSoft_matrix matrix, RSoft_vector v) {
	/* this acts a 4 x 4 2D matrix */
	float rotateMatrix[16] = {
					1.0,						0.0,						0.0f,   	0.0f,
					0.0,						1.0,						0.0f,		0.0f,
					0.0,						0.0f,						1.0f,		0.0f,
					v.x,						v.y,						v.z,		1.0f
	};

	return RSoft_multiplyMatrix(matrix.m, rotateMatrix);
}

RSoft_vector RSoft_applyMatrix(RSoft_matrix matrix, RSoft_vector v) {
	return (RSoft_vector) {
		//	x * Mx[x]	    +    y * My[x]		  +    z * Mz[x]		+ 1 * Mw[x]			 			 
		(v.x * matrix.m[0]) + (v.y * matrix.m[4]) + (v.z * matrix.m[8]) + matrix.m[12],
		(v.x * matrix.m[1]) + (v.y * matrix.m[5]) + (v.z * matrix.m[9]) + matrix.m[13],
		(v.x * matrix.m[2]) + (v.y * matrix.m[6]) + (v.z * matrix.m[10]) + matrix.m[14],
	}; 
}

void RSoft_setTexture(u8* texture, RSoft_rect texRect, RSoft_area textureArea) {
	RSoft_renderInfo.texture = texture;
	RSoft_renderInfo.texRect = texRect;
	RSoft_renderInfo.textureArea = textureArea;
}

u32 RSoft_textureGetColor(RSoft_point texPoint, u8 color[4]) {
	RSoft_renderInfoStruct info = RSoft_renderInfo;
	
	u8 output[4] = {color[0], color[1], color[2], color[3]};
	
	if (info.texture == NULL) 
		return *((u32*)output);

	info.texRect.x += texPoint.x / ((float)info.texRect.w / (float)info.textureArea.w); 
	info.texRect.y += texPoint.y / ((float)info.texRect.h / (float)info.textureArea.h);

	size_t index = (info.texRect.y * info.textureArea.w * 4) + info.texRect.x * 4; 
	if (index > info.textureArea.w * info.textureArea.h * 4)
		return *((u32*)output);
	
	output[0] = info.texture[index];
	output[1] = info.texture[index + 1];
	output[2] = info.texture[index + 2];
	output[3] = info.texture[index + 3];

	output[0] *= (float)(color[0] / 255.0f);
	output[1] *= (float)(color[1] / 255.0f);
	output[2] *= (float)(color[2] / 255.0f);
	output[3] *= (float)(color[3] / 255.0f);

	return *((u32*)output);
}

/* fill buffer with a color, clearing anything that was on it */
void RSoft_clear(u8* buffer, u8 color[4]) {
	RSoft_renderInfoStruct info = RSoft_renderInfo;

	/* if all the values are the same */
    if (color[0] == color[1] && color[0] == color[2] && color[0] == color[3]) {
        /* set it all in one function */
        memset(buffer, 0, info.bufferSize.w * info.canvasSize.h * 4 * sizeof(u8));  
        return;
    }

    /* else we'll have to something more complex... */

    /* loop through each *pixel* (not channel) of the buffer */
    u32 x, y;
    for (y = 0; y < (u32)info.canvasSize.h; y++) {
        for (x = 0; x < info.bufferSize.w; x++) {
            u32 index = (y * 4 * info.bufferSize.w) + x * 4;
            
            /* copy the color to that pixel */
            memcpy(buffer + index, color, 4 * sizeof(u8));
        }
    }    
}

void RSoft_drawRectOutline(u8* buffer, RSoft_rect r, u8 color[4]) {
	RSoft_drawRectFOutline(buffer, RSOFT_RECTF(r.x, r.y, r.w, r.h), color);
}

void RSoft_drawRectFOutline(u8* buffer, RSoft_rectF r, u8 color[4]) {
	RSoft_drawLineF(buffer, RSOFT_VECTOR2D(r.x, r.y), RSOFT_VECTOR2D(r.x + r.w, r.y), color);	
	RSoft_drawLineF(buffer, RSOFT_VECTOR2D(r.x, r.y), RSOFT_VECTOR2D(r.x, r.y + r.h), color);	
	RSoft_drawLineF(buffer, RSOFT_VECTOR2D(r.x + r.w, r.y), RSOFT_VECTOR2D(r.x + r.w, r.y + r.h), color);	
	RSoft_drawLineF(buffer, RSOFT_VECTOR2D(r.x, r.y + r.h), RSOFT_VECTOR2D(r.x + r.w, r.y + r.h), color);	
}

void RSoft_drawRect(u8* buffer, RSoft_rect r, u8 color[4]) {
	RSoft_drawRectF(buffer, RSOFT_RECTF(r.x, r.y, r.w, r.h), color);	
}


	
void RSoft_drawRectF(u8* buffer, RSoft_rectF r, u8 color[4]) {
	RSoft_renderInfoStruct info = RSoft_renderInfo;
	
	for(float x = r.x; x < (r.x + r.w); x++) {
		for(float y = r.y; y < (r.y + r.h); y++) {
			u32 texColor = RSoft_textureGetColor(RSOFT_POINT(x - r.x, y - r.y), color);
			RSoft_drawVector(buffer, RSOFT_VECTOR2D(x, y), (u8*)(&texColor));
		}
    }
}

void RSoft_drawPolygon(u8* buffer, RSoft_rect r, size_t angles, u8 color[4]) {
	RSoft_drawPolygonF(buffer, RSOFT_RECTF(r.x, r.y, r.w, r.h), angles, color);
}

void RSoft_drawPolygonF(u8* buffer, RSoft_rectF r, size_t angles, u8 color[4]) {
	RSoft_renderInfoStruct info = RSoft_renderInfo;

	float slopeX = r.w;  
	float slopeY = r.h;
	
	float steps = slopeX;
	if (r.h >= r.w)
		steps = slopeY;
	
	slopeX /= steps;
	slopeY /= steps;

	RSoft_rectF rect = r;

	for (float i = 0; i < steps; i++) {
		//RSoft_drawPolygonFOutline(buffer, RSOFT_RECTF(r.x, r.y + slopeY, r.w, r.h), angles, color);
		for (size_t j = 0; j < angles; j++) {
			float delta = (j * (360 / angles));
			float delta2 = ((j + 1) * (360 / angles));
		
			RSoft_vector p1 = RSOFT_VECTOR2D(r.x - (RSoft_cos(delta) * r.w), r.y + (RSoft_sin(delta) * r.h));
			RSoft_vector p2 = RSOFT_VECTOR2D(r.x - (RSoft_cos(delta2) * r.w), r.y + (RSoft_sin(delta2) * r.h));
			RSoft_point texPoint = RSOFT_POINT(abs((p1.x - (r.x - rect.w))), abs((p2.y - (r.y - rect.h))));


			u32 texColor = RSoft_textureGetColor(RSOFT_POINT(abs((p1.x - (r.x - (rect.w / 2)))), 
															 abs((p1.y - (r.y - (rect.h / 4))))), color);

			//u32 texColor = RSoft_textureGetColor(texPoint, info.texRect, info.texture, info.textureArea, color);
			RSoft_drawLineF(buffer, p1, p2, (u8*)&texColor);
		}
		
		r.w -= slopeX;
		r.h -= slopeY;
	}
}

void RSoft_drawPolygonOutline(u8* buffer, RSoft_rect r, size_t angles, u8 color[4]) {
	RSoft_drawPolygonFOutline(buffer, RSOFT_RECTF(r.x, r.y, r.w, r.h), angles, color);
}

void RSoft_drawPolygonFOutline(u8* buffer, RSoft_rectF r, size_t angles, u8 color[4]) {
	RSoft_renderInfoStruct info = RSoft_renderInfo;
	
	for (size_t i = 0; i < angles; i ++) {
		float delta = (i * (360 / angles));
		float delta2 = ((i + 1) * (360 / angles)) ;
		
		RSoft_vector p1 = RSOFT_VECTOR2D(r.x - (RSoft_cos(delta) * r.w), r.y + (RSoft_sin(delta) * r.h));
		RSoft_vector p2 = RSOFT_VECTOR2D(r.x - (RSoft_cos(delta2) * r.w), r.y + (RSoft_sin(delta2) * r.h));

		//printf("%i\n", r.x - p1.x);
		//u32 texColor = RSoft_textureGetColor(RSOFT_POINT(sqrt(pow(r.x - p1.x, 2)), sqrt(pow(r.y - p1.y, 2))), color);
			
		RSoft_drawLineF(buffer, p1, p2, (u8*)&color);
	}
}


void RSoft_drawPoint(u8* buffer, RSoft_point p, u8 color[4]) {
	RSoft_drawVector(buffer, RSOFT_VECTOR2D(p.x, p.y), color);
}

void RSoft_drawVector(u8* buffer, RSoft_vector v, u8 color[4]) {
	RSoft_renderInfoStruct info = RSoft_renderInfo;
	
	v = RSoft_applyMatrix(info.matrix, v);	
	if (v.x < 0 || v.y < 0 || v.x > info.bufferSize.w || v.y > info.bufferSize.h)
		return;
	
	int index = ((u32)v.y) * (4 * info.bufferSize.w) + (u32)v.x * 4;
	if (color[3] != 255) {
		float alpha = color[3] / 255;

		color[0] = (color[0] * alpha) + ((1 - alpha) * buffer[index]);
		color[1] =  (color[1] * alpha) + ((1 - alpha) * buffer[index + 1]);
		color[2] =  (color[2] * alpha) + ((1 - alpha) * buffer[index + 2]);
		color[3] =  (color[3] * alpha) + ((1 - alpha) * buffer[index + 3]);
	}

    memcpy(buffer + index, color, 4 * sizeof(u8));
}

void RSoft_drawLine(u8* buffer, RSoft_point start, RSoft_point end, u8 color[4]) {
	RSoft_drawLineF(buffer, RSOFT_VECTOR2D(start.x, start.y), RSOFT_VECTOR2D(end.x, end.y), color);
}

void RSoft_drawLineF(u8* buffer, RSoft_vector start, RSoft_vector end, u8 color[4]) {
	if (end.x < start.x) {
		RSoft_vector oldStart = start;
		start = end;
		end = oldStart;
	}

	float slopeX = (end.x - start.x); 
	float slopeY = (end.y - start.y);
	
	float steps = fabs(slopeY);
	if (fabs(slopeX) >= fabs(slopeY))
		steps = fabs(slopeX);
	
	slopeX /= steps;
	slopeY /= steps;

	for (float i = 0; i < steps; i++) {
		u32 texColor = RSoft_textureGetColor(RSOFT_POINT(i, 0), color); 
		RSoft_drawVector(buffer, RSOFT_VECTOR2D(start.x, start.y), (u8*)&texColor);
		start.x += slopeX;
		start.y += slopeY;
	}
}


void RSoft_drawTriangleOutline(u8* buffer, const RSoft_vector points[3], u8 color[4]) {
	RSoft_vector npoints[3] = {RSOFT_VECTOR2D(points[0].x, points[0].y), 
							   RSOFT_VECTOR2D(points[1].x, points[1].y), 
							   RSOFT_VECTOR2D(points[2].x, points[2].y)};

	RSoft_drawTriangleOutlineF(buffer, npoints, color);
}

void RSoft_drawTriangleOutlineF(u8* buffer, const RSoft_vector points[3], u8 color[4]) {
	RSoft_drawLineF(buffer, points[0], points[1], color);	
	RSoft_drawLineF(buffer, points[1], points[2], color);	
	RSoft_drawLineF(buffer, points[2], points[0], color);		
}

void RSoft_drawTriangle(u8* buffer, const RSoft_point points[3], u8 color[4]) {
	RSoft_vector npoints[3] = {RSOFT_VECTOR2D(points[0].x, points[0].y), 
							   RSOFT_VECTOR2D(points[1].x, points[1].y), 
							   RSOFT_VECTOR2D(points[2].x, points[2].y)};

	RSoft_drawTriangleF(buffer, npoints, color);
}


void RSoft_vectorSwap(RSoft_vector* v1, RSoft_vector* v2) {
	RSoft_vector save = *v1;
	*v1 = *v2;
	*v2 = *(&save);
}

void RSoft_drawTriangleF(u8* buffer, const RSoft_vector points[3], u8 color[4]) {	
	RSoft_renderInfoStruct info = RSoft_renderInfo;
	
	/* source from 
		https://github.com/itsYakub/Silk/blob/c996bdb9f42407dcf9cfcc6b6b9b0046261f9f02/silk.h#L1619
	*/

    if(points[0].y > points[1].y) RSoft_vectorSwap((RSoft_vector*)points + 0, (RSoft_vector*)points + 1);
    if(points[0].y > points[2].y) RSoft_vectorSwap((RSoft_vector*)points + 0, (RSoft_vector*)points + 2);
    if(points[1].y > points[2].y) RSoft_vectorSwap((RSoft_vector*)points + 1, (RSoft_vector*)points + 2);

    RSoft_vector delta_vector_ab = {
        points[1].x - points[0].x,
        points[1].y - points[0].y
    };

    RSoft_vector delta_vector_ac = {
        points[2].x - points[0].x,
        points[2].y - points[0].y
    };

    RSoft_vector delta_vector_cb = {
        points[1].x - points[2].x,
        points[1].y - points[2].y
    };

    RSoft_vector delta_vector_ca = {
        points[0].x - points[2].x,
        points[0].y - points[2].y
    };

    for(float y = points[0].y; y < points[1].y; y++) {
		if(y < 0 || y > info.bufferSize.h)
			continue;
		
		float s1 = delta_vector_ab.y != 0 ?
			(y - points[0].y) * delta_vector_ab.x / delta_vector_ab.y + points[0].x :
			points[0].x;

		float s2 = delta_vector_ac.y != 0 ?
			(y - points[0].y) * delta_vector_ac.x / delta_vector_ac.y + points[0].x :
			points[0].x;

		if(s1 > s2) {
			float b = s1;
			s1 = s2;
			s2 = b;
		}

		for(float x = s1; x <= s2; x++) {
			u32 texColor = RSoft_textureGetColor(RSOFT_POINT(x - s1, y - points[1].y), color);
			RSoft_drawVector(buffer, RSOFT_VECTOR2D(x, y), (u8*)(&texColor));
		}
    }

    for(i32 y = points[1].y; y < points[2].y; y++) {
        if(y < 0 || y > info.bufferSize.h)
			continue;
		
		i32 s1 = delta_vector_cb.y != 0 ?
			(y - points[2].y) * delta_vector_cb.x / delta_vector_cb.y + points[2].x :
			points[2].x;

		i32 s2 = delta_vector_ca.y != 0 ?
			(y - points[2].y) * delta_vector_ca.x / delta_vector_ca.y + points[2].x :
			points[2].x;

		if(s1 > s2) {
			float b = s1;
			s1 = s2;
			s2 = b;
		}

		for(i32 x = s1; x <= s2; x++) {
			u32 texColor = RSoft_textureGetColor(RSOFT_POINT(x - s1, y - points[1].y), color);
			RSoft_drawVector(buffer, RSOFT_VECTOR2D(x, y), (u8*)(&texColor));
		}
	}
}

#endif
