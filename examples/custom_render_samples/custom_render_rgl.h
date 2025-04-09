#define RSGL_CUSTOM_RENDER

#define RSGL_texture u32
#ifdef RSGL_CUSTOM_RENDER
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#endif

#define RGL_IMPLEMENTATION
#include "RGL.h"

void RSGL_renderDeleteTexture(RSGL_texture tex) { glDeleteTextures(1, &tex); }
void RSGL_renderViewport(i32 x, i32 y, i32 w, i32 h) { glViewport(x, y, w ,h); }

void RSGL_renderClear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RSGL_renderInit(void* proc, RSGL_RENDER_INFO* info) { rglInit(proc); }
void RSGL_renderFree(void) { rglClose(); }

void RSGL_renderBatch(RSGL_RENDER_INFO* info) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    rglSetProgram(RSGL_args.program); 
    rglLegacy(RSGL_args.legacy);

    size_t i, j;
    size_t tIndex = 0, cIndex = 0, vIndex = 0;
    for (i = 0; i < info->len; i++) {
        glEnable(GL_TEXTURE_2D);
        rglSetTexture(info->batches[i].tex);
        rglLineWidth(info->batches[i].lineWidth);
        
        u32 mode = info->batches[i].type;
        if (mode > 0x0100) {
            glEnable(GL_BLEND);
            mode -= 0x0100;
        } else {
            glDisable(GL_BLEND);
        }

        if (mode > 0x0010) {
            mode -= 0x0010;
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);
        }
        else {
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
        }

        rglBegin(mode);

        for (j = info->batches[i].start; j < info->batches[i].len; j++) {
            rglTexCoord2f(info->texCoords[tIndex], 
                        info->texCoords[tIndex + 1]
                      );
            
            rglColor4f(info->colors[cIndex], 
                        info->colors[cIndex + 1], 
                        info->colors[cIndex + 2],
                        info->colors[cIndex + 3]
                      );
            
            rglVertex3f(info->verts[vIndex], 
                        info->verts[vIndex + 1], 
                        info->verts[vIndex + 2]
                      );

            tIndex += 2;
            vIndex += 3;
            cIndex += 4;
        }

        rglEnd();
        
        if (info->batches[i].type > 0x0010) {
            glEnable(GL_DEPTH_TEST);
        }
    }

    info->len = 0;
    info->vert_len = 0;
    rglRenderBatch();
}

#ifndef GL_RG
#define GL_RG                             0x8227
#endif

void RSGL_renderScissorStart(RSGL_rectF scissor) {
    RSGL_draw();
    glEnable(GL_SCISSOR_TEST);

    glScissor(scissor.x, RSGL_args.currentRect.h - (scissor.y + scissor.h), scissor.w, scissor.h);
    glScissor(scissor.x, scissor.y, scissor.w, scissor.h);
}

void RSGL_renderScissorEnd(void) {
    RSGL_draw();
    glDisable(GL_SCISSOR_TEST);
}

/* textures / images */
u32 RSGL_renderCreateTexture(u8* bitmap, RSGL_area memsize, u8 channels) {
    unsigned int id = 0;

    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glPixelStorei(GL_UNPACK_ROW_LENGTH, memsize.w);
    
    unsigned int c = 0;

    switch (channels) {
        case 1: c = GL_RED; break;
        case 2: c = GL_RG; break;
        case 3: c = GL_RGB; break;
        case 4: c = GL_RGBA; break;
        default: break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, c, memsize.w, memsize.h, 0, c, GL_UNSIGNED_BYTE, bitmap);

    glBindTexture(GL_TEXTURE_2D, 0);

    return id;
}

void RSGL_renderUpdateTexture(u32 texture, u8* bitmap, RSGL_area memsize, u8 channels) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, memsize.w);

    u16 c = 0;
    switch (channels) {
        case 1: c = GL_RED; break;
        case 2: c = GL_RG; break;
        case 3: c = GL_RGB; break;
        case 4: c = GL_RGBA; break;
        default: break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, c, memsize.w, memsize.h, 0, c, GL_UNSIGNED_BYTE, bitmap);
    glBindTexture(GL_TEXTURE_2D, 0);
}


#ifdef RGL_MODERN_OPENGL
#ifndef GL_DEBUG_TYPE_ERROR
#define GL_DEBUG_TYPE_ERROR               0x824C
#define GL_DEBUG_OUTPUT                   0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS       0x8242
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84 
#endif

void RSGL_opengl_getError(void) {
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

void RSGL_debug_shader(u32 src, const char *shader, const char *action) {
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
				GLchar *infoLog = (GLchar *)RSGL_MALLOC(infoLogLength);
				glGetShaderInfoLog(src, infoLogLength, NULL, infoLog);
				printf("%s Shader info log:\n%s\n", shader, infoLog);
				free(infoLog);
			}
		}

		RSGL_opengl_getError();
	}
}

RSGL_programInfo RSGL_renderCreateProgram(const char* VShaderCode, const char* FShaderCode, char* posName, char* texName, char* colorName) {
    RSGL_programInfo program;

	/* compile vertex shader */
	program.vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(program.vShader, 1, &VShaderCode, NULL);
	glCompileShader(program.vShader);

    RSGL_debug_shader(program.vShader, "Vertex", "compile");

	/* compile fragment shader */
	program.fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(program.fShader, 1, &FShaderCode, NULL);
	glCompileShader(program.fShader);

    RSGL_debug_shader(program.fShader, "Fragment", "compile");

	/* create program and link vertex and fragment shaders */
	program.program = glCreateProgram();

	glAttachShader(program.program, program.vShader);
	glAttachShader(program.program, program.fShader);

    glBindAttribLocation(program.program, 0, posName);
    glBindAttribLocation(program.program, 1, texName);
    glBindAttribLocation(program.program, 2, colorName);

	glLinkProgram(program.program);

    return program;
}

void RSGL_renderDeleteProgram(RSGL_programInfo program) {
    glUseProgram(0);

    glDetachShader(program.program, program.vShader);
    glDetachShader(program.program, program.fShader);
    glDeleteShader(program.vShader);
    glDeleteShader(program.fShader);

    glDeleteProgram(program.program);
}

void RSGL_renderSetShaderValue(u32 program, char* var, float value[], u8 len) {
    glUseProgram(program);
    int loc = glGetUniformLocation(program, var);

    switch (len) {
        case 1: glUniform1f(loc, value[0]); break;
        case 2: glUniform2f(loc, value[0], value[1]); break;
        case 3: glUniform3f(loc, value[0], value[1], value[2]); break;
        case 4: glUniform4f(loc, value[0], value[1], value[2], value[3]); break;
        default: break;
    }

    glUseProgram(0);
}

#ifndef GL_PERSPECTIVE_CORRECTION_HINT
#define GL_PERSPECTIVE_CORRECTION_HINT		0x0C50
#endif

#ifndef GL_TEXTURE_SWIZZLE_RGBA
#define GL_TEXTURE_SWIZZLE_RGBA           0x8E46
#endif

#ifndef GL_TEXTURE0
#define GL_TEXTURE0				0x84C0
#endif

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE			0x812F
#endif

u32 RFont_create_atlas(u32 atlasWidth, u32 atlasHeight) {
 #if defined(RFONT_DEBUG) && !defined(RFONT_RENDER_LEGACY)
   glEnable(GL_DEBUG_OUTPUT);
   #endif
   
   u32 id = 0;
   glEnable(GL_TEXTURE_2D);
   
   glBindTexture(GL_TEXTURE_2D, 0);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glGenTextures(1, &id);

   glBindTexture(GL_TEXTURE_2D, id);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   
   u8* data = (u8*)calloc(atlasWidth * atlasHeight * 4, sizeof(u8));

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

   free(data);

   glBindTexture(GL_TEXTURE_2D, id);
	static GLint swizzleRgbaParams[4] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRgbaParams);

   glBindTexture(GL_TEXTURE_2D, 0);
   return id;
}

b8 RFont_resize_atlas(RFont_texture* atlas, u32 newWidth, u32 newHeight) {
    GLuint newAtlas;
    glGenTextures(1, &newAtlas);
    glBindTexture(GL_TEXTURE_2D, newAtlas);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newWidth, newHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glBindTexture(GL_TEXTURE_2D, *atlas);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, newWidth - RFONT_ATLAS_RESIZE_LEN, newHeight);

    glDeleteTextures(1, (u32*)atlas);

    glBindTexture(GL_TEXTURE_2D, newAtlas);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    /* swizzle new atlas */
    glBindTexture(GL_TEXTURE_2D, newAtlas);
	static GLint swizzleRgbaParams[4] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRgbaParams);

    glBindTexture(GL_TEXTURE_2D, 0);

    *atlas = newAtlas;
    return 1;
}

#ifndef GL_UNPACK_ROW_LENGTH
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#endif


void RFont_push_pixel_values(GLint alignment, GLint rowLength, GLint skipPixels, GLint skipRows);
void RFont_push_pixel_values(GLint alignment, GLint rowLength, GLint skipPixels, GLint skipRows) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, rowLength);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, skipPixels);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, skipRows);
}

void RFont_bitmap_to_atlas(u32 atlas, u8* bitmap, float x, float y, float w, float h) {
   glEnable(GL_TEXTURE_2D);
   
	GLint alignment, rowLength, skipPixels, skipRows;
   glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowLength);
	glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skipPixels);
	glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skipRows);
   
   #if !defined(RFONT_RENDER_LEGACY)
   glActiveTexture(GL_TEXTURE0 + atlas - 1);
   #endif

	glBindTexture(GL_TEXTURE_2D, atlas);

	RFont_push_pixel_values(1, w, 0, 0);

	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RED, GL_UNSIGNED_BYTE, bitmap);

	RFont_push_pixel_values(alignment, rowLength, skipPixels, skipRows);

   glBindTexture(GL_TEXTURE_2D, 0);
}

#endif