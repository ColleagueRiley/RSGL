#define RSGL_CUSTOM_RENDER

#define RSGL_CUSTOM_DRAW
#define __gl_h_
#define RGFW_BUFFER

#include "portableGL.h"

#define RSGL_texture u32

#ifdef RSGL_CUSTOM_RENDER
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#endif

#define PORTABLEGL_IMPLEMENTATION
#include "portableGL.h"

typedef struct Uniforms {
	vec4 v_color;
    vec2 vertexTexCoord;
    u32 tex;
} Uniforms;

Uniforms the_uniforms;

void defaultVShaderCode(float* vs_output, vec4* vertex_attribs, Shader_Builtins* builtins, void* uniforms)
{
	builtins->gl_Position = vertex_attribs[0];
    ((Uniforms*)uniforms)->vertexTexCoord = (vec2){vertex_attribs[1].x, vertex_attribs[1].y};
}

void defaultFShaderCode(float* fs_input, Shader_Builtins* builtins, void* uniforms)
{
    Uniforms uniform = *((Uniforms*)uniforms);
    vec4 texVec = texture2D(uniform.tex, uniform.vertexTexCoord.x, uniform.vertexTexCoord.y);

    if (texVec.x || texVec.y || texVec.z || texVec.w) {
        uniform.v_color.x *= texVec.x;
        uniform.v_color.y *= texVec.y;
        uniform.v_color.z *= texVec.z;
        uniform.v_color.w *= texVec.w;
    }   

	builtins->gl_FragColor = uniform.v_color;
} 

typedef struct RSGL_INFO {
	glContext context;

    RSGL_programInfo program;       /* Default shader program id, supports vertex color and diffuse texture*/
    u32 defaultTex;

    u32 vao, vbo, tbo, cbo; /* array object and array buffers */
} RSGL_INFO;

RSGL_INFO RSGL_gl;

void RSGL_renderDeleteTexture(u32 tex) { glDeleteTextures(1, &tex); }
void RSGL_renderViewport(i32 x, i32 y, i32 w, i32 h) { glViewport(x, y, w ,h); }

void RSGL_renderClear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RSGL_renderInit(void* proc, RSGL_RENDER_INFO* info) {
    RGFW_area area = RGFW_getScreenSize();
    
	init_glContext(&RSGL_gl.context, (u32**)proc, area.w, 500, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    pglSetUniform(&the_uniforms);

    #if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_ES2)
	glGenVertexArrays(1, &RSGL_gl.vao);
	glBindVertexArray(RSGL_gl.vao);
    #endif

	glGenBuffers(1, &RSGL_gl.vbo);
	glGenBuffers(1, &RSGL_gl.tbo);
	glGenBuffers(1, &RSGL_gl.cbo);

    RSGL_gl.program = RSGL_renderCreateProgram((const char*)defaultVShaderCode, (const char*)defaultFShaderCode, "vertexPosition", "vertexTexCoord", "vertexColor");
    
    /* Init default vertex arrays buffers */
    /* Initialize CPU (RAM) vertex buffers (position, texcoord, color data and indexes) */

    #if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_ES2)
    glBindVertexArray(RSGL_gl.vao);
    #endif

    /* Quads - Vertex buffers binding and attributes enable */
    /* Vertex position buffer (shader-location = 0) */
    glBindBuffer(GL_ARRAY_BUFFER, RSGL_gl.vbo);
    glBufferData(GL_ARRAY_BUFFER, RSGL_MAX_VERTS * 3 * 4 * sizeof(float), info->verts, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);

    /* Vertex texcoord buffer (shader-location = 1) */
    glBindBuffer(GL_ARRAY_BUFFER, RSGL_gl.tbo);
    glBufferData(GL_ARRAY_BUFFER, RSGL_MAX_VERTS * 2 * 4 * sizeof(float), info->texCoords, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);

    /* Vertex color buffer (shader-location = 3) */
    glBindBuffer(GL_ARRAY_BUFFER, RSGL_gl.cbo);
    glBufferData(GL_ARRAY_BUFFER, RSGL_MAX_VERTS * 4 * 4 * sizeof(float), info->colors, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, 0, 0);

    #if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_ES2)
    /* Unbind the current VAO */
    if (RSGL_gl.vao) 
        glBindVertexArray(0);
    #endif

    /* load default texture */
    u8 white[4] = {255, 255, 255, 255};
    RSGL_gl.defaultTex = RSGL_renderCreateTexture(white, RSGL_AREA(1, 1), 4);
}

void RSGL_renderFree(void) {  
    if (RSGL_args.legacy == 2)
        return;
    
    /* Unbind everything */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Unload all vertex buffers data */
    #if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_ES2)
    glBindVertexArray(RSGL_gl.vao);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glBindVertexArray(0);
    #endif

    /* Delete VBOs from GPU (VRAM) */
    glDeleteBuffers(1, &RSGL_gl.vbo);
    glDeleteBuffers(1, &RSGL_gl.tbo);
    glDeleteBuffers(1, &RSGL_gl.cbo);

    RSGL_renderDeleteProgram(RSGL_gl.program);

    glDeleteTextures(1, &RSGL_gl.defaultTex); /* Unload default texture */

	free_glContext(&RSGL_gl.context);
}

void RSGL_renderBatch(RSGL_RENDER_INFO* info) { 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (info->vert_len > 0) {
        /* Vertex positions buffer */
        glBindBuffer(GL_ARRAY_BUFFER, RSGL_gl.vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, info->vert_len * 3 * sizeof(float), info->verts);

        /* Texture coordinates buffer */
        glBindBuffer(GL_ARRAY_BUFFER, RSGL_gl.tbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, info->vert_len * 2 * sizeof(float), info->texCoords);
        
        /* Colors buffer */
        glBindBuffer(GL_ARRAY_BUFFER, RSGL_gl.cbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, info->vert_len * 4 * sizeof(float), info->colors); 
        
        #if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_ES2)
        glBindVertexArray(0);
        #endif

        /* Set current shader */
        if (RSGL_args.program)
            glUseProgram(RSGL_args.program);
        else
            glUseProgram(RSGL_gl.program.program);
        
        #if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_ES2)
        glBindVertexArray(RSGL_gl.vao);
        #endif

        /* Bind vertex attrib: position (shader-location = 0) */
        glBindBuffer(GL_ARRAY_BUFFER, RSGL_gl.vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
        glEnableVertexAttribArray(0);

        /* Bind vertex attrib: texcoord (shader-location = 1) */
        glBindBuffer(GL_ARRAY_BUFFER, RSGL_gl.tbo);
        glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);
        glEnableVertexAttribArray(1);

        /* Bind vertex attrib: color (shader-location = 3) */
        glBindBuffer(GL_ARRAY_BUFFER, RSGL_gl.cbo);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, 0, 0);
        glEnableVertexAttribArray(2);

        glActiveTexture(GL_TEXTURE0);

        u32 vertexOffset = 0;
        u32 i;
        for (i = 0; i < info->len; i++) {
            GLenum mode = info->batches[i].type;

            if (mode > 0x0100) {
                mode -= 0x0100;
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

            /* Bind current draw call texture, activated as GL_TEXTURE0 and Bound to sampler2D texture0 by default */
            if (info->batches[i].tex == 0)
                info->batches[i].tex = RSGL_gl.defaultTex;
            
            glBindTexture(GL_TEXTURE_2D, info->batches[i].tex);
            glLineWidth(info->batches[i].lineWidth);
            
            if (RSGL_args.program)
                glUseProgram(RSGL_args.program);
            else
                glUseProgram(RSGL_gl.program.program);
            
            glDrawArrays(mode, vertexOffset, info->batches[i].len);

            vertexOffset += info->batches[i].len;

            if (info->batches[i].type > 0x0010) {
                glEnable(GL_DEPTH_TEST);
                glDepthMask(GL_TRUE);
            }
        }

        if (!RSGL_gl.vao) {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        glBindTexture(GL_TEXTURE_2D, 0);    /* Unbind textures */
    
        if (RSGL_gl.vao) 
            glBindVertexArray(0); /* Unbind VAO */

        glUseProgram(0);    /* Unbind shader program */
    }
    
    info->len = 0;
    info->vert_len = 0;
}

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


#ifndef GL_RG
#define GL_RG                             0x8227
#endif

#ifndef GL_UNPACK_ROW_LENGTH
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#endif

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

	/* create program and link vertex and fragment shaders */
	program.program = pglCreateProgram((vert_func)VShaderCode, (frag_func)FShaderCode, 0, NULL, GL_FALSE);

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
    //glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, newWidth - RFONT_ATLAS_RESIZE_LEN, newHeight);

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