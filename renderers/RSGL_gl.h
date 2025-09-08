#ifndef RSGL_H
#include "RSGL.h"
#endif

// WebGL doesn't support compute shaders iirc so yeah
#if defined(__EMSCRIPTEN__) && defined(RSGL_USE_COMPUTE)
#undef RSGL_USE_COMPUTE
#endif

#ifndef RSGL_GL_H
#define RSGL_GL_H

typedef struct RSGL_glRenderer {
    RSGL_programInfo program;       /* Default shader program id, supports vertex color and diffuse texture*/
    u32 defaultTex;
	u32 vao;
	size_t vbo, tbo, cbo; /* array object and array buffers */
} RSGL_glRenderer;

RSGLDEF RSGL_rendererProc RSGL_GL_rendererProc(void);
RSGLDEF size_t RSGL_GL_size(void);

RSGLDEF RSGL_renderer* RSGL_GL_renderer_init(RSGL_area r, void* loader);
RSGLDEF void RSGL_GL_renderer_initPtr(RSGL_area r, void* loader, RSGL_glRenderer* ptr, RSGL_renderer* renderer);
RSGLDEF void RSGL_GL_render(RSGL_glRenderer* ctx, RSGL_programInfo program, const RSGL_renderBuffers* info);
RSGLDEF void RSGL_GL_initPtr(RSGL_glRenderer* ctx, void* proc); /* init render backend */
RSGLDEF void RSGL_GL_freePtr(RSGL_glRenderer* ctx); /* free render backend */
RSGLDEF void RSGL_GL_clear(RSGL_glRenderer* ctx, float r, float g, float b, float a);
RSGLDEF void RSGL_GL_viewport(RSGL_glRenderer* ctx, i32 x, i32 y, i32 w, i32 h);
RSGLDEF void RSGL_GL_createBuffer(RSGL_glRenderer* ctx, size_t size, const void* data, size_t* buffer);
RSGLDEF void RSGL_GL_updateBuffer(RSGL_glRenderer* ctx, size_t buffer, const void* data, size_t start, size_t end);
RSGLDEF void RSGL_GL_deleteBuffer(RSGL_glRenderer* ctx, size_t buffer);
/* create a texture based on a given bitmap, this must be freed later using RSGL_deleteTexture or opengl*/
RSGLDEF RSGL_texture RSGL_GL_createTexture(RSGL_glRenderer* ctx, u8* bitmap, RSGL_area memsize,  u8 channels);
/* updates an existing texture wiht a new bitmap */
RSGLDEF void RSGL_GL_updateTexture(RSGL_glRenderer* ctx, RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels);
/* delete a texture */
RSGLDEF void RSGL_GL_deleteTexture(RSGL_glRenderer* ctx, RSGL_texture tex);
/* starts scissoring */
RSGLDEF void RSGL_GL_scissorStart(RSGL_glRenderer* ctx, RSGL_rectF scissor, i32 renderer_height);
/* stops scissoring */
RSGLDEF void RSGL_GL_scissorEnd(RSGL_glRenderer* ctx);
/* program loading */
RSGLDEF RSGL_programInfo RSGL_GL_createProgram(RSGL_glRenderer* ctx, const char* VShaderCode, const char* FShaderCode, const char* posName, const char* texName, const char* colorName);
RSGLDEF void RSGL_GL_deleteProgram(RSGL_glRenderer* ctx, RSGL_programInfo program);
RSGLDEF void RSGL_GL_setShaderValue(RSGL_glRenderer* ctx, u32 program, const char* var, const float value[], u8 len);
RSGLDEF RSGL_texture RSGL_GL_create_atlas(RSGL_glRenderer* ctx, u32 atlasWidth, u32 atlasHeight);
RSGLDEF void RSGL_GL_bitmap_to_atlas(RSGL_glRenderer* ctx, RFont_texture atlas, u32 atlasWidth, u32 atlasHeight, u32 maxHeight, u8* bitmap, float w, float h, float* x, float* y);
#ifdef RSGL_USE_COMPUTE
RSGLDEF RSGL_programInfo RSGL_GL_createComputeProgram(RSGL_glRenderer* ctx, const char* CShaderCode);
RSGLDEF void RSGL_GL_dispatchComputeProgram(RSGL_glRenderer* ctx, RSGL_programInfo program, u32 groups_x, u32 groups_y, u32 groups_z);
RSGLDEF void RSGL_GL_bindComputeTexture(RSGL_glRenderer* ctx, u32 texture, u8 format);
#endif
#endif

#ifdef RSGL_IMPLEMENTATION

RSGL_renderer* RSGL_GL_renderer_init(RSGL_area r, void* loader) { return RSGL_renderer_init(RSGL_GL_rendererProc(), r, loader); }
void RSGL_GL_renderer_initPtr(RSGL_area r, void* loader, RSGL_glRenderer* ptr, RSGL_renderer* renderer) { return RSGL_renderer_initPtr(RSGL_GL_rendererProc(), r, loader, ptr, renderer); }


/* prevent winapi conflicts (opengl includes windows.h for some reason) */
#define OEMRESOURCE

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#if defined(_WIN32)
typedef char GLchar;
typedef int	 GLsizei;
typedef ptrdiff_t GLintptr;
typedef uintptr_t GLsizeiptr;

#define GL_VERTEX_SHADER   0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_ARRAY_BUFFER         0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW  0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_TEXTURE0 0x84C0
#endif

#ifndef RSGL_NO_GL_LOADER

typedef void (*RSGL_gl_proc)(void); // function pointer equivalent of void*
#define RSGL_PROC_DEF(proc, name) name##SRC = (name##PROC)(RSGL_gl_proc)proc(#name)

typedef void (*RSGLapiproc)(void);
typedef RSGLapiproc (*RSGLloadfunc)(const char *name);

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
typedef void (*glUniform1fPROC) (GLint location, GLfloat v0);
typedef void (*glUniform2fPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (*glUniform3fPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (*glUniform4fPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

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
glUniform1fPROC glUniform1fSRC = NULL;
glUniform2fPROC glUniform2fSRC = NULL;
glUniform3fPROC glUniform3fSRC = NULL;
glUniform4fPROC glUniform4fSRC = NULL;

#ifdef RSGL_USE_COMPUTE
typedef void (*glDispatchComputePROC)(GLuint x, GLuint y, GLuint z);
glDispatchComputePROC glDispatchComputeSRC = NULL;

typedef void (*glMemoryBarrierPROC)(GLenum e);
glMemoryBarrierPROC glMemoryBarrierSRC = NULL;

typedef void (*glBindImageTexturePROC)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
glBindImageTexturePROC glBindImageTextureSRC = NULL;

#endif

glGenVertexArraysPROC glGenVertexArraysSRC = NULL;
glBindVertexArrayPROC glBindVertexArraySRC = NULL;
glDeleteVertexArraysPROC glDeleteVertexArraysSRC = NULL;

#define glUniform1f glUniform1fSRC
#define glUniform2f glUniform2fSRC
#define glUniform3f glUniform3fSRC
#define glUniform4f glUniform4fSRC
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

#ifdef RSGL_USE_COMPUTE
#define glMemoryBarrier glMemoryBarrierSRC
#define glDispatchCompute glDispatchComputeSRC
#define glBindImageTexture glBindImageTextureSRC
#endif

extern int RSGL_loadGLModern(RSGLloadfunc proc);
#endif

#define RSGL_MULTILINE_STR(...) #__VA_ARGS__
size_t RSGL_GL_size(void) {
	return sizeof(RSGL_glRenderer);
}

RSGL_rendererProc RSGL_GL_rendererProc() {
	RSGL_rendererProc proc;
	RSGL_MEMSET(&proc, 0, sizeof(proc));

	proc.render = (void (*)(void*, RSGL_programInfo, const RSGL_renderBuffers*))RSGL_GL_render;
    proc.size = (size_t (*)(void))RSGL_GL_size;
    proc.initPtr = (void (*)(void*, void*))RSGL_GL_initPtr;
    proc.freePtr = (void (*)(void*))RSGL_GL_freePtr;
    proc.clear = (void (*)(void*, float, float, float, float))RSGL_GL_clear;
    proc.viewport = (void (*)(void*, i32, i32, i32, i32))RSGL_GL_viewport;
    proc.createTexture = (RSGL_texture (*)(void*, u8*, RSGL_area,  u8))RSGL_GL_createTexture;
    proc.updateTexture = (void (*)(void*, RSGL_texture, u8*, RSGL_area, u8))RSGL_GL_updateTexture;
    proc.deleteTexture = (void (*)(void*, RSGL_texture))RSGL_GL_deleteTexture;
    proc.scissorStart = (void (*)(void*, RSGL_rectF, i32))RSGL_GL_scissorStart;
    proc.scissorEnd =  (void (*)(void*))RSGL_GL_scissorEnd;
    proc.createProgram = (RSGL_programInfo (*)(void*, const char*, const char*, const char*, const char*, const char*))RSGL_GL_createProgram;
    proc.deleteProgram = (void (*)(void*, RSGL_programInfo))RSGL_GL_deleteProgram;
    proc.setShaderValue = (void (*)(void*, u32, const char*, const float[], u8))RSGL_GL_setShaderValue;
    proc.createAtlas = (RSGL_texture (*)(void*, u32, u32))RSGL_GL_create_atlas;
    proc.bitmapToAtlas = (void(*)(void*, RSGL_texture, u32, u32, u32, u8*, float, float, float*, float*))RSGL_GL_bitmap_to_atlas;
	proc.createBuffer = (void (*)(void*, size_t, const void*, size_t*))RSGL_GL_createBuffer;
	proc.updateBuffer = (void (*)(void*, size_t, void*, size_t, size_t))RSGL_GL_updateBuffer;
	proc.deleteBuffer = (void (*)(void*, size_t))RSGL_GL_deleteBuffer;
//	proc.setSurface = (void (*)(void*, void*))RSGL_GL_setSurface;
#ifdef RSGL_USE_COMPUTE
	proc.createComputeProgram = (RSGL_programInfo (*)(void*, const char*))RSGL_GL_createComputeProgram;
	proc.dispatchComputeProgram = (void (*)(void*, RSGL_programInfo, u32, u32, u32))RSGL_GL_dispatchComputeProgram;
	proc.bindComputeTexture = (void (*)(void*, u32, u8))RSGL_GL_bindComputeTexture;
#else
	proc.createComputeProgram = NULL;
	proc.dispatchComputeProgram = NULL;
	proc.bindComputeTexture = NULL;
#endif
	return proc;
}

void RSGL_GL_deleteTexture(RSGL_glRenderer* ctx, RSGL_texture tex) { glDeleteTextures(1, (u32*)&tex); }
void RSGL_GL_viewport(RSGL_glRenderer* ctx, i32 x, i32 y, i32 w, i32 h) { glViewport(x, y, w ,h); }

void RSGL_GL_clear(RSGL_glRenderer* ctx, float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RSGL_GL_createBuffer(RSGL_glRenderer* ctx, size_t size, const void* data, size_t* buffer) {
	glGenBuffers(1, (u32*)buffer);

	GLenum usage = GL_STATIC_DRAW;
	if (data == NULL)
		usage = GL_DYNAMIC_DRAW;

	glBindBuffer(GL_ARRAY_BUFFER, *(u32*)buffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void RSGL_GL_updateBuffer(RSGL_glRenderer* ctx, size_t buffer, const void* data, size_t start, size_t end) {
	glBindBuffer(GL_ARRAY_BUFFER, *(u32*)&buffer);
	glBufferSubData(GL_ARRAY_BUFFER, start, end, data);
}

void RSGL_GL_deleteBuffer(RSGL_glRenderer* ctx, size_t buffer) {
	glDeleteBuffers(1, (u32*)&buffer);
}

void RSGL_GL_initPtr(RSGL_glRenderer* ctx, void* proc) {
    #if !defined(__EMSCRIPTEN__) && !defined(RSGL_NO_GL_LOADER)
    if (RSGL_loadGLModern((RSGLloadfunc)proc)) {
        #ifdef RSGL_DEBUG
        printf("Failed to load an OpenGL 3.3 Context, reverting to OpenGL Legacy\n");
        #endif
        return;
    }
    #else
    RSGL_UNUSED(proc);
    #endif

#ifdef RSGL_DEBUG
    #ifndef GL_SHADING_LANGUAGE_VERSION
        #define GL_SHADING_LANGUAGE_VERSION 0x8B8C
    #endif


    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
#endif

    static const char *defaultVShaderCode = RSGL_MULTILINE_STR(
        \x23version 330                     \n
        in vec3 vertexPosition;            \n
        in vec2 vertexTexCoord;            \n
        in vec4 vertexColor;               \n
        out vec2 fragTexCoord;             \n
        out vec4 fragColor;                \n
        uniform mat4 mat; \n

        void main() {
            fragTexCoord = vertexTexCoord;
            fragColor = vertexColor;
            gl_Position = mat * vec4(vertexPosition, 1.0);
        }
    );

    static const char* defaultFShaderCode = RSGL_MULTILINE_STR(
		\x23version 330       \n
		in vec2 fragTexCoord;
		in vec4 fragColor;
		out vec4 finalColor;
		uniform sampler2D texture0;
		void main() {
				finalColor = texture(texture0, fragTexCoord) * fragColor;
			}
		);

	glGenVertexArrays(1, &ctx->vao);
	glBindVertexArray(ctx->vao);

    ctx->program = RSGL_GL_createProgram(ctx, defaultVShaderCode, defaultFShaderCode, "vertexPosition", "vertexTexCoord", "vertexColor");
    /* Init default vertex arrays buffers */
    /* Initialize CPU (RAM) vertex buffers (position, texcoord, color data and indexes) */

    glBindVertexArray(ctx->vao);

	/* Unbind the current VAO */
    glBindVertexArray(0);

    /* load default texture */
    u8 white[4] = {255, 255, 255, 255};
    ctx->defaultTex = RSGL_GL_createTexture(ctx, white, RSGL_AREA(1, 1), 4);

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RSGL_GL_freePtr(RSGL_glRenderer* ctx) {
    /* Unbind everything */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Unload all vertex buffers data */
    glBindVertexArray(ctx->vao);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glBindVertexArray(0);

    /* Delete VBOs from GPU (VRAM) */
    glDeleteVertexArrays(0, &ctx->vao);

    RSGL_GL_deleteProgram(ctx, ctx->program);

    glDeleteTextures(1, (u32*)&ctx->defaultTex); /* Unload default texture */
}

void RSGL_GL_render(RSGL_glRenderer* ctx, RSGL_programInfo program, const RSGL_renderBuffers* buffers) {
	if (program.program == 0) program = ctx->program;

    glBindVertexArray(ctx->vao);

	/* Vertex positions buffer */
	glBindBuffer(GL_ARRAY_BUFFER, buffers->vertex);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);

	/* Texture coordinates buffer */
	glBindBuffer(GL_ARRAY_BUFFER, buffers->texture);
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);

	/* Colors buffer */
	glBindBuffer(GL_ARRAY_BUFFER, buffers->color);
	glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->elements);

	/* Set current shader */
	glUseProgram(program.program);

	int loc = glGetUniformLocation(program.program, "mat");
	u32 i;

	for (i = 0; i < buffers->batchCount; i++) {
		GLenum mode = GL_TRIANGLES;
		glBindTexture(GL_TEXTURE_2D, (buffers->batches[i].tex == 0) ? (ctx->defaultTex) : (buffers->batches[i].tex));
		glLineWidth(buffers->batches[i].lineWidth > 0 ? buffers->batches[i].lineWidth : 0.1f);
		if (loc >= 0) {
			glUniformMatrix4fv(loc, 1, GL_FALSE, buffers->batches[i].matrix.m);
		}

		switch (buffers->batches[i].type) {
			case RSGL_TRIANGLES: mode = GL_TRIANGLES; break;
			case RSGL_POINTS: mode = GL_POINTS; break;
			case RSGL_LINES:  mode = GL_LINES; break;
			default: break;
		}

		glDrawElements(mode, (i32)buffers->batches[i].elmCount, GL_UNSIGNED_SHORT, (void*)(buffers->batches[i].elmStart * sizeof(u16)));
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);    /* Unbind textures */

	glBindVertexArray(0); /* Unbind VAO */
}

void RSGL_GL_scissorStart(RSGL_glRenderer* ctx, RSGL_rectF scissor, i32 renderer_height) {
    glEnable(GL_SCISSOR_TEST);
    glScissor(scissor.x, renderer_height - (scissor.y + scissor.h), scissor.w, scissor.h);
}

void RSGL_GL_scissorEnd(RSGL_glRenderer* ctx) {
    glDisable(GL_SCISSOR_TEST);
}

#ifndef GL_RG
#define GL_RG                             0x8227
#endif

/* textures / images */
RSGL_texture RSGL_GL_createTexture(RSGL_glRenderer* ctx, u8* bitmap, RSGL_area memsize, u8 channels) {
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

void RSGL_GL_updateTexture(RSGL_glRenderer* ctx, RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels) {
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

#ifdef RSGL_DEBUG
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

		GLchar infoLog[512];
		if (action[0] == 'c') {
			glGetShaderInfoLog(src, 512, NULL, infoLog);
			printf("%s Shader info log:\n%s\n", shader, infoLog);
		} else {
			glGetProgramInfoLog(src, 512, NULL, infoLog);
			printf("%s info log:\n%s\n", shader, infoLog);
		}

		RSGL_opengl_getError();
	}
}
#endif

RSGL_programInfo RSGL_GL_createProgram(RSGL_glRenderer* ctx, const char* VShaderCode, const char* FShaderCode, const char* posName, const char* texName, const char* colorName) {
	RSGL_programInfo program;
	u32 vShader, fShader;

	/* compile vertex shader */
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &VShaderCode, NULL);
	glCompileShader(vShader);

#ifdef RSGL_DEBUG
	RSGL_debug_shader(vShader, "Vertex", "compile");
#endif

	/* compile fragment shader */
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &FShaderCode, NULL);
	glCompileShader(fShader);

#ifdef RSGL_DEBUG
	RSGL_debug_shader(fShader, "Fragment", "compile");
#endif

	/* create program and link vertex and fragment shaders */
	program.program = glCreateProgram();

	glAttachShader(program.program, vShader);
	glAttachShader(program.program, fShader);

	glBindAttribLocation(program.program, 0, posName);
	glBindAttribLocation(program.program, 1, texName);
	glBindAttribLocation(program.program, 2, colorName);

	glLinkProgram(program.program);

#ifdef RSGL_DEBUG
	RSGL_debug_shader(program.program, "Program", "link");
#endif

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	program.type = RSGL_shaderTypeStandard;
	return program;
}

void RSGL_GL_deleteProgram(RSGL_glRenderer* ctx, RSGL_programInfo program) {
    glUseProgram(0);
    glDeleteProgram(program.program);
}

void RSGL_GL_setShaderValue(RSGL_glRenderer* ctx, u32 program, const char* var, const float value[], u8 len) {
    glUseProgram(program);
    int loc = glGetUniformLocation(program, var);

    switch (len) {
        case 1: glUniform1f(loc, value[0]); break;
        case 2: glUniform2f(loc, value[0], value[1]); break;
        case 3: glUniform3f(loc, value[0], value[1], value[2]); break;
        case 4: glUniform4f(loc, value[0], value[1], value[2], value[3]); break;
        case 16: glUniformMatrix4fv(loc, 1, GL_FALSE, value); break;
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

#ifndef GL_UNPACK_ROW_LENGTH
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#endif

RSGL_texture RSGL_GL_create_atlas(RSGL_glRenderer* ctx, u32 atlasWidth, u32 atlasHeight) {
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

	u8* data = (u8*)RSGL_MALLOC(atlasWidth * atlasHeight * 4);
	RSGL_MEMSET(data, 0, atlasWidth * atlasHeight * 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	RSGL_FREE(data);

	static GLint swizzleRgbaParams[4] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRgbaParams);

	glBindTexture(GL_TEXTURE_2D, 0);
	return id;
}

void RSGL_GL_push_pixel_values(GLint alignment, GLint rowLength, GLint skipPixels, GLint skipRows);
void RSGL_GL_push_pixel_values(GLint alignment, GLint rowLength, GLint skipPixels, GLint skipRows) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, rowLength);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, skipPixels);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, skipRows);
}

void RSGL_GL_bitmap_to_atlas(RSGL_glRenderer* ctx, RFont_texture atlas, u32 atlasWidth, u32 atlasHeight, u32 maxHeight, u8* bitmap, float w, float h, float* x, float* y) {
	glBindTexture(GL_TEXTURE_2D, 0);
	GLint alignment, rowLength, skipPixels, skipRows;
	RSGL_UNUSED(ctx); RSGL_UNUSED(atlasHeight);
	if (((*x) + w) >= atlasWidth) {
		*x = 0;
		*y += (float)maxHeight;
	}

	glEnable(GL_TEXTURE_2D);

	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowLength);
	glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skipPixels);
	glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skipRows);

	glBindTexture(GL_TEXTURE_2D, (u32)atlas);

	RSGL_GL_push_pixel_values(1, (i32)w, 0, 0);

	glTexSubImage2D(GL_TEXTURE_2D, 0, (i32)(*x), (i32)*y, (i32)w, (i32)h, GL_RED, GL_UNSIGNED_BYTE, bitmap);

	RSGL_GL_push_pixel_values(alignment, rowLength, skipPixels, skipRows);

	glBindTexture(GL_TEXTURE_2D, 0);
	*x += w;
}

#ifdef RSGL_USE_COMPUTE

#ifndef GL_RG8
#define GL_RG8 0x822B
#endif

#ifndef GL_READ_WRITE
#define GL_READ_WRITE 0x88BA
#endif

#ifndef GL_COMPUTE_SHADER
#define GL_COMPUTE_SHADER 0x91B9
#endif

#ifndef GL_SHADER_IMAGE_ACCESS_BARRIER_BIT
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT 0x00000020
#endif

RSGL_programInfo RSGL_GL_createComputeProgram(RSGL_glRenderer* ctx, const char* CShaderCode) {
	RSGL_programInfo program;
	program.type = RSGL_shaderTypeCompute;

	u32 compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &CShaderCode, NULL);
	glCompileShader(compute);

#ifdef RSGL_DEBUG
	RSGL_debug_shader(compute, "Compute", "compile");
#endif

	program.program = glCreateProgram();
	glAttachShader(program.program, compute);
	glLinkProgram(program.program);
#ifdef RSGL_DEBUG
	RSGL_debug_shader(program.program, "Program", "link");
#endif

	glDeleteShader(compute);

	return program;
}

void RSGL_GL_dispatchComputeProgram(RSGL_glRenderer* ctx, RSGL_programInfo program, u32 groups_x, u32 groups_y, u32 groups_z) {
	glUseProgram(program.program);
	glDispatchCompute(groups_x, groups_y, groups_z);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}


void RSGL_GL_bindComputeTexture(RSGL_glRenderer* ctx, u32 texture, u8 format) {
	u16 c = 0;
   switch (format) {
       case 2: c = GL_RG8; break;
       case 3: c = GL_RGB8; break;
       case 4: c = GL_RGBA8; break;
       default: break;
   }
	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, c);
}

#endif

#ifndef RSGL_NO_GL_LOADER
int RSGL_loadGLModern(RSGLloadfunc proc) {
    RSGL_PROC_DEF(proc, glShaderSource);
    RSGL_PROC_DEF(proc, glCreateShader);
    RSGL_PROC_DEF(proc, glCompileShader);
    RSGL_PROC_DEF(proc, glCreateProgram);
    RSGL_PROC_DEF(proc, glAttachShader);
    RSGL_PROC_DEF(proc, glBindAttribLocation);
    RSGL_PROC_DEF(proc, glLinkProgram);
    RSGL_PROC_DEF(proc, glBindBuffer);
    RSGL_PROC_DEF(proc, glBufferData);
    RSGL_PROC_DEF(proc, glEnableVertexAttribArray);
    RSGL_PROC_DEF(proc, glVertexAttribPointer);
    RSGL_PROC_DEF(proc, glDisableVertexAttribArray);
    RSGL_PROC_DEF(proc, glDeleteBuffers);
    RSGL_PROC_DEF(proc, glUseProgram);
    RSGL_PROC_DEF(proc, glDetachShader);
    RSGL_PROC_DEF(proc, glDeleteShader);
    RSGL_PROC_DEF(proc, glDeleteProgram);
    RSGL_PROC_DEF(proc, glBufferSubData);
    RSGL_PROC_DEF(proc, glGetShaderiv);
    RSGL_PROC_DEF(proc, glGetShaderInfoLog);
    RSGL_PROC_DEF(proc, glGetProgramiv);
    RSGL_PROC_DEF(proc, glGetProgramInfoLog);
    RSGL_PROC_DEF(proc, glGenBuffers);
    RSGL_PROC_DEF(proc, glBindVertexArray);
    RSGL_PROC_DEF(proc, glGenVertexArrays);
    RSGL_PROC_DEF(proc, glDeleteVertexArrays);
    RSGL_PROC_DEF(proc, glGetUniformLocation);
    RSGL_PROC_DEF(proc, glUniformMatrix4fv);
    RSGL_PROC_DEF(proc, glActiveTexture);
    RSGL_PROC_DEF(proc, glUniform1f);
    RSGL_PROC_DEF(proc, glUniform2f);
    RSGL_PROC_DEF(proc, glUniform3f);
    RSGL_PROC_DEF(proc, glUniform4f);
    #ifdef RSGL_USE_COMPUTE
    RSGL_PROC_DEF(proc, glDispatchCompute);
	 RSGL_PROC_DEF(proc, glMemoryBarrier);
	 RSGL_PROC_DEF(proc, glBindImageTexture);
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

    #if !defined(RSGL_OPENGL_21)
    GLuint vao;
    glGenVertexArraysSRC(1, &vao);

    if (vao == 0)
        return 1;
    #endif

    glDeleteVertexArraysSRC(1, &vao);

    return 0;
}
#endif

#endif /* RSGL_IMPLEMENTATION */
