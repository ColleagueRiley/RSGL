#define RSGL_CUSTOM_RENDER
#define RSGL_IMPLEMENTATION

#define RSGL_GET_WORLD_X(x) (float)(x)
#define RSGL_GET_WORLD_Y(y) (float)(y)
#define RSGL_GET_WORLD_Z(z) (float)(z)


#define RGFW_BUFFER

#include "RSGL.h"

#define RSOFT_IMPLEMENTATION
#include "RSoft.h"


typedef struct RSGL_INFO {
	u8* buffer;
} RSGL_INFO;

RSGL_INFO RSGL_rsoft;

void RSGL_renderDeleteTexture(u32 tex) { 

}
void RSGL_renderViewport(i32 x, i32 y, i32 w, i32 h) { 
	RSoft_setCanvasSize(RSOFT_AREA(w, h));
}

void RSGL_renderClear(float r, float g, float b, float a) {
	u8 color[4] = {r * 255, g * 255, b * 255, a * 255};

	RSoft_clear(RSGL_rsoft.buffer, color);
}

void RSGL_renderInit(void* buffer, RSGL_RENDER_INFO* info) {
    RSGL_UNUSED(info);
	RSGL_rsoft.buffer = buffer;
}

void RSGL_renderFree(void) {   

}

void RSGL_renderBatch(RSGL_RENDER_INFO* info) { 
        size_t i, j;
        size_t tIndex = 0, cIndex = 0, vIndex = 0;
       
		RSGL_UNUSED(tIndex);
		for (i = 0; i < info->len; i++) {
            u32 mode = info->batches[i].type;
            if (mode > 0x0100)
                mode -= 0x0100;
            if (mode > 0x0010)
                mode -= 0x0010;
           
            // glDrawArrays(mode, info->batches[i].start, info->batches[i].len);

			switch (mode) {
				case RSGL_TRIANGLES:
					for (j = info->batches[i].start; j < info->batches[i].len ; j += 3) {
						// info->texCoords[tIndex], info->texCoords[tIndex + 1]
						RSoft_vector npoints[3] = {
													RSOFT_VECTOR3D(info->verts[vIndex], info->verts[vIndex + 1],  info->verts[vIndex + 2]), 
													RSOFT_VECTOR3D(info->verts[vIndex + 3], info->verts[vIndex + 4],  info->verts[vIndex + 5]), 
													RSOFT_VECTOR3D(info->verts[vIndex + 6], info->verts[vIndex + 7],  info->verts[vIndex + 8])
												 };
						u8 color[4] = {info->colors[cIndex] * 255, info->colors[cIndex + 1] * 255, 
										info->colors[cIndex + 2] * 255, info->colors[cIndex + 3] * 255};
						
						RSoft_drawTriangleF(RSGL_rsoft.buffer, npoints, color);
						
						tIndex += 6;
						vIndex += 9;
						cIndex += 12;
					}
					break;
				case RSGL_LINES:
					for (j = info->batches[i].start; j < info->batches[i].len; j += 2) {
						// info->texCoords[tIndex], info->texCoords[tIndex + 1]
						u8 color[4] = {info->colors[cIndex] * 255, info->colors[cIndex + 1] * 255, 
										info->colors[cIndex + 2] * 255, info->colors[cIndex + 3] * 255};
							
						RSoft_drawLineF(RSGL_rsoft.buffer, 
										RSOFT_VECTOR3D(info->verts[vIndex], info->verts[vIndex + 1],  info->verts[vIndex + 2]), 
										RSOFT_VECTOR3D(info->verts[vIndex + 3], info->verts[vIndex + 4],  info->verts[vIndex + 5]), 
										color);

						tIndex += 4;
						vIndex += 6;
						cIndex += 8;
					}
					break;
				case RSGL_TRIANGLE_FAN:
					size_t start = vIndex + 3;
					for (j = info->batches[i].start; j < info->batches[i].len; j += 3) {
						// info->texCoords[tIndex], info->texCoords[tIndex + 1]
						RSoft_vector npoints[3] = {
							RSOFT_VECTOR3D(300, 300, info->verts[start + 2]),
							RSOFT_VECTOR3D(info->verts[vIndex], info->verts[vIndex + 1], info->verts[vIndex + 2]),
							RSOFT_VECTOR3D(info->verts[vIndex + 3], info->verts[vIndex + 4], info->verts[vIndex + 5])
						};

						u8 color[4] = {info->colors[cIndex] * 255, info->colors[cIndex + 1] * 255, 
									info->colors[cIndex + 2] * 255, info->colors[cIndex + 3] * 255};

						/*printf("{%f %f %f}, {%f %f %f}, {%f %f %f}\n", 
							info->verts[vIndex], info->verts[vIndex + 1], info->verts[vIndex + 2], 
							info->verts[vIndex + 3], info->verts[vIndex + 4], info->verts[vIndex + 5], 
							info->verts[vIndex + 6], info->verts[vIndex + 7], info->verts[vIndex + 8]);*/

						RSoft_drawTriangleF(RSGL_rsoft.buffer, npoints, color);

						tIndex += 6;
						vIndex += 9;
						cIndex += 12;
					}
					break;
				default: break;
			}
		}

    info->len = 0;
    info->vert_len = 0;
}

/* textures / images */
u32 RSGL_renderCreateTexture(u8* bitmap, RSGL_area memsize, u8 channels) {
    unsigned int id = 0;

    return id;
}

void RSGL_renderUpdateTexture(u32 texture, u8* bitmap, RSGL_area memsize, u8 channels) {

}

u32 RFont_create_atlas(u32 atlasWidth, u32 atlasHeight) {
	u32 id = 0;
	return id;
}

void RFont_bitmap_to_atlas(u32 atlas, u8* bitmap, float x, float y, float w, float h) {

}

void RSGL_renderSetShaderValue(u32 program, char* var, float value[], u8 len) { RSGL_UNUSED(program); RSGL_UNUSED(value); RSGL_UNUSED(len);}
void RSGL_renderDeleteProgram(RSGL_programInfo program) { }
RSGL_programInfo RSGL_renderCreateProgram(const char* VShaderCode, const char* FShaderCode, char* posName, char* texName, char* colorName) { 
	RSGL_UNUSED(VShaderCode); RSGL_UNUSED(FShaderCode); RSGL_UNUSED(posName); RSGL_UNUSED(texName); RSGL_UNUSED(colorName);
	return (RSGL_programInfo) {};
}
