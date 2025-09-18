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
	u32 vao;
} RSGL_glRenderer;

RSGLDEF RSGL_rendererProc RSGL_GL_rendererProc(void);
RSGLDEF size_t RSGL_GL_size(void);

RSGLDEF RSGL_renderer* RSGL_GL_renderer_init(void* loader);
RSGLDEF void RSGL_GL_renderer_initPtr(void* loader, RSGL_glRenderer* ptr, RSGL_renderer* renderer);
RSGLDEF void RSGL_GL_render(RSGL_glRenderer* ctx, const RSGL_renderPass* pass);
RSGLDEF void RSGL_GL_initPtr(RSGL_glRenderer* ctx, void* proc); /* init render backend */
RSGLDEF void RSGL_GL_freePtr(RSGL_glRenderer* ctx); /* free render backend */
RSGLDEF void RSGL_GL_clear(RSGL_glRenderer* ctx, RSGL_framebuffer framebuffer, float r, float g, float b, float a);
RSGLDEF void RSGL_GL_viewport(RSGL_glRenderer* ctx, i32 x, i32 y, i32 w, i32 h);
RSGLDEF void RSGL_GL_createBuffer(RSGL_glRenderer* ctx, size_t size, const void* data, size_t* buffer);
RSGLDEF void RSGL_GL_updateBuffer(RSGL_glRenderer* ctx, size_t buffer, const void* data, size_t start, size_t end);
RSGLDEF void RSGL_GL_deleteBuffer(RSGL_glRenderer* ctx, size_t buffer);
RSGLDEF RSGL_programBlob RSGL_GL_defaultBlob(RSGL_glRenderer* ctx);
/* create a texture based on a given bitmap, this must be freed later using RSGL_deleteTexture or opengl*/
RSGLDEF RSGL_texture RSGL_GL_createTexture(RSGL_glRenderer* ctx, const RSGL_textureBlob* blob);
/* updates an existing texture wiht a new bitmap */
RSGLDEF void RSGL_GL_copyToTexture(RSGL_glRenderer* ctx, RSGL_texture texture, size_t x, size_t y, const RSGL_textureBlob* blob);
/* delete a texture */
RSGLDEF void RSGL_GL_deleteTexture(RSGL_glRenderer* ctx, RSGL_texture tex);
/* starts scissoring */
RSGLDEF void RSGL_GL_scissorStart(RSGL_glRenderer* ctx, float x, float y, float w, float h, float renderer_height);
/* stops scissoring */
RSGLDEF void RSGL_GL_scissorEnd(RSGL_glRenderer* ctx);
/* program loading */
RSGLDEF RSGL_programInfo RSGL_GL_createProgram(RSGL_glRenderer* ctx, RSGL_programBlob* blob);
RSGLDEF void RSGL_GL_deleteProgram(RSGL_glRenderer* ctx, const RSGL_programInfo* program);
RSGLDEF size_t RSGL_GL_findShaderVariable(RSGL_glRenderer* ctx, const RSGL_programInfo*  program, const char* var, const size_t len);
RSGLDEF void RSGL_GL_updateShaderVariable(RSGL_glRenderer* ctx, const RSGL_programInfo* program, size_t var, const float value[], u8 len);

RSGLDEF RSGL_framebuffer RSGL_GL_createFramebuffer(RSGL_glRenderer* renderer, size_t width, size_t height);
RSGLDEF void RSGL_GL_attachFramebuffer(RSGL_glRenderer* renderer, RSGL_framebuffer fbo, RSGL_texture tex, u8 attachType, u8 mipLevel);
RSGLDEF void RSGL_GL_deleteFramebuffer(RSGL_glRenderer* renderer, RSGL_framebuffer fbo);

#ifdef RSGL_USE_COMPUTE
RSGLDEF RSGL_programInfo RSGL_GL_createComputeProgram(RSGL_glRenderer* ctx, const char* CShaderCode);
RSGLDEF void RSGL_GL_dispatchComputeProgram(RSGL_glRenderer* ctx, RSGL_programInfo program, u32 groups_x, u32 groups_y, u32 groups_z);
RSGLDEF void RSGL_GL_bindComputeTexture(RSGL_glRenderer* ctx, u32 texture, u8 format);
#endif
#endif

#ifdef RSGL_IMPLEMENTATION

RSGL_renderer* RSGL_GL_renderer_init(void* loader) { return RSGL_renderer_init(RSGL_GL_rendererProc(), loader); }
void RSGL_GL_renderer_initPtr(void* loader, RSGL_glRenderer* ptr, RSGL_renderer* renderer) { return RSGL_renderer_initPtr(RSGL_GL_rendererProc(), loader, ptr, renderer); }


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

#define GL_VERTEX_SHADER   0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_ARRAY_BUFFER         0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW  0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_TEXTURE0 0x84C0
#define GL_STATIC_DRAW          0x88E4
#define GL_DYNAMIC_DRAW         0x88E8
#define GL_TEXTURE0             0x84C0
#define GL_FRAMEBUFFER          0x8D40
#define GL_COLOR_ATTACHMENT0    0x8CE0
#define GL_DEPTH_ATTACHMENT     0x8D00
#define GL_STENCIL_ATTACHMENT   0x8D20

#include <GL/glext.h>
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

typedef void (*glBindFramebufferPROC) (GLenum target, GLuint framebuffer);
typedef void (*glGenFramebuffersPROC) (GLsizei n, GLuint *ids);
typedef void (*glDeleteFramebuffersPROC) (GLsizei n, GLuint *framebuffers);
typedef void (*glFramebufferTexture2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

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

glBindFramebufferPROC glBindFramebufferSRC = NULL;
glGenFramebuffersPROC glGenFramebuffersSRC = NULL;
glDeleteFramebuffersPROC glDeleteFramebuffersSRC = NULL;
glFramebufferTexture2DPROC glFramebufferTexture2DSRC = NULL;

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
#define glBindFramebuffer glBindFramebufferSRC
#define glGenFramebuffers glGenFramebuffersSRC
#define glDeleteFramebuffers glDeleteFramebuffersSRC
#define glFramebufferTexture2D glFramebufferTexture2DSRC

#ifdef RSGL_USE_COMPUTE
#define glMemoryBarrier glMemoryBarrierSRC
#define glDispatchCompute glDispatchComputeSRC
#define glBindImageTexture glBindImageTextureSRC
#endif

#ifndef GL_TEXTURE_SWIZZLE_RGBA
	#define GL_TEXTURE_SWIZZLE_RGBA 0x8E46
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

	proc.render = (void (*)(void*, const RSGL_renderPass* pass))RSGL_GL_render;
    proc.size = (size_t (*)(void))RSGL_GL_size;
    proc.initPtr = (void (*)(void*, void*))RSGL_GL_initPtr;
    proc.freePtr = (void (*)(void*))RSGL_GL_freePtr;
    proc.clear = (void (*)(void*, RSGL_framebuffer, float, float, float, float))RSGL_GL_clear;
    proc.viewport = (void (*)(void*, i32, i32, i32, i32))RSGL_GL_viewport;
    proc.createTexture = (RSGL_texture (*)(void*, const RSGL_textureBlob* blob))RSGL_GL_createTexture;
    proc.copyToTexture = (void (*)(void*, RSGL_texture, size_t, size_t, const RSGL_textureBlob* blob))RSGL_GL_copyToTexture;
    proc.deleteTexture = (void (*)(void*, RSGL_texture))RSGL_GL_deleteTexture;
    proc.scissorStart = (void (*)(void*, float, float, float, float, float))RSGL_GL_scissorStart;
    proc.scissorEnd =  (void (*)(void*))RSGL_GL_scissorEnd;
    proc.createProgram = (RSGL_programInfo (*)(void*, RSGL_programBlob* blob))RSGL_GL_createProgram;
    proc.deleteProgram = (void (*)(void*, const RSGL_programInfo*))RSGL_GL_deleteProgram;
	proc.findShaderVariable = (size_t (*)(void*, const RSGL_programInfo*, const char*, size_t))RSGL_GL_findShaderVariable;
	proc.updateShaderVariable = (void (*)(void*, const RSGL_programInfo*, size_t, const float[], u8))RSGL_GL_updateShaderVariable;
	proc.createBuffer = (void (*)(void*, size_t, const void*, size_t*))RSGL_GL_createBuffer;
	proc.updateBuffer = (void (*)(void*, size_t, void*, size_t, size_t))RSGL_GL_updateBuffer;
	proc.deleteBuffer = (void (*)(void*, size_t))RSGL_GL_deleteBuffer;
	proc.defaultBlob = (RSGL_programBlob (*)(void))RSGL_GL_defaultBlob;
	proc.createFramebuffer = (RSGL_framebuffer (*)(void*, size_t, size_t))RSGL_GL_createFramebuffer;
	proc.attachFramebuffer = (void (*)(void*, RSGL_framebuffer, RSGL_texture, u8, u8))RSGL_GL_attachFramebuffer;
	proc.deleteFramebuffer = (void (*)(void*, RSGL_framebuffer))RSGL_GL_deleteFramebuffer;


//	proc.setSurface = (void (*)(void*, void*))RSGL_GL_setSurface;
#ifdef RSGL_USE_COMPUTE
	proc.createComputeProgram = (RSGL_programInfo (*)(void*, const char*))RSGL_GL_createComputeProgram;
	proc.dispatchComputeProgram = (void (*)(void*, const RSGL_programInfo*, u32, u32, u32))RSGL_GL_dispatchComputeProgram;
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

void RSGL_GL_clear(RSGL_glRenderer* ctx, RSGL_framebuffer framebuffer, float r, float g, float b, float a) {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

RSGL_programBlob RSGL_GL_defaultBlob(RSGL_glRenderer* ctx) {
    static const char *defaultVShaderCode = RSGL_MULTILINE_STR(
        \x23version 330                     \n
        in vec3 vertexPosition;            \n
        in vec2 vertexTexCoord;            \n
        in vec4 vertexColor;               \n
        out vec2 fragTexCoord;             \n
        out vec4 fragColor;                \n
        uniform mat4 model; \n
        uniform mat4 pv; \n

        void main() {
            fragTexCoord = vertexTexCoord;
            fragColor = vertexColor;
            gl_Position = pv * model * vec4(vertexPosition, 1.0);
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

	RSGL_programBlob blob;
	blob.vertex = defaultVShaderCode;
	blob.vertexLen = sizeof(defaultVShaderCode);
	blob.fragment = defaultFShaderCode;
	blob.fragmentLen = sizeof(defaultFShaderCode );

	return blob;
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

	glGenVertexArrays(1, &ctx->vao);

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RSGL_GL_freePtr(RSGL_glRenderer* ctx) {
    glDeleteVertexArrays(0, &ctx->vao);
}

void RSGL_GL_render(RSGL_glRenderer* ctx, const RSGL_renderPass* pass) {
	glBindFramebuffer(GL_FRAMEBUFFER, pass->framebuffer);

	glBindVertexArray(ctx->vao);

	glBindBuffer(GL_ARRAY_BUFFER, pass->buffers->vertex);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, pass->buffers->texture);
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, pass->buffers->color);
	glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pass->buffers->elements);

	glUseProgram(pass->program->program);
	glUniformMatrix4fv(pass->program->perspectiveView, 1, GL_FALSE, pass->matrix);

	u32 i;
	for (i = 0; i < pass->buffers->batchCount; i++) {
		GLenum mode = GL_TRIANGLES;
		glBindTexture(GL_TEXTURE_2D, pass->buffers->batches[i].tex);
		glLineWidth(pass->buffers->batches[i].lineWidth);
        glUniformMatrix4fv(pass->program->model, 1, GL_FALSE, pass->buffers->batches[i].matrix.m);

		switch (pass->buffers->batches[i].type) {
			case RSGL_TRIANGLES: mode = GL_TRIANGLES; break;
			case RSGL_POINTS: mode = GL_POINTS; break;
			case RSGL_LINES:  mode = GL_LINES; break;
			default: break;
		}

		glDrawElements(mode, (i32)pass->buffers->batches[i].elmCount, GL_UNSIGNED_SHORT, (void*)(pass->buffers->batches[i].elmStart * sizeof(u16)));
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RSGL_GL_scissorStart(RSGL_glRenderer* ctx, float x, float y, float w, float h, float renderer_height) {
    glEnable(GL_SCISSOR_TEST);

	glScissor(x, renderer_height - (y + h), w, h);
}

void RSGL_GL_scissorEnd(RSGL_glRenderer* ctx) {
    glDisable(GL_SCISSOR_TEST);
}

GLuint RSGL_GL_textureFormatToNative(RSGL_textureFormat format) {
	switch (format) {
		case RSGL_formatRGB: return GL_RGB;
		case RSGL_formatBGR: return GL_BGR;
		case RSGL_formatRGBA: return GL_RGBA;
		case RSGL_formatBGRA: return GL_BGRA;
		case RSGL_formatRed: return GL_RED;
		case RSGL_formatGrayscale: return GL_RED;
		case RSGL_formatGrayscaleAlpha: return GL_RED;
		default: break;
	}

	return GL_RGBA;
}

GLuint RSGL_GL_textureDataTypeToNative(RSGL_textureDataType type) {
    switch (type) {
		case RSGL_textureDataInt: return GL_UNSIGNED_BYTE;
		case RSGL_textureDataFloat: return GL_FLOAT;
        default: break;
    }

	return GL_UNSIGNED_BYTE;
}

GLuint RSGL_GL_textureFilterToNative(RSGL_textureFilter filter) {
    switch (filter) {
		case RSGL_filterNearest:  return GL_NEAREST;
		case RSGL_filterLinear: return GL_LINEAR;
        default: break;
    }

	return GL_LINEAR;
}

/* textures / images */
RSGL_texture RSGL_GL_createTexture(RSGL_glRenderer* ctx, const RSGL_textureBlob* blob) {
    unsigned int id = 0;

    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, RSGL_GL_textureFilterToNative(blob->minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, RSGL_GL_textureFilterToNative(blob->magFilter));

    glPixelStorei(GL_UNPACK_ROW_LENGTH, blob->width);

	u32 dataFormat = RSGL_GL_textureFormatToNative(blob->dataFormat);
	u32 textureFormat = RSGL_GL_textureFormatToNative(blob->textureFormat);
	u32 dataType = RSGL_GL_textureDataTypeToNative(blob->dataType);

	if (blob->dataFormat == RSGL_formatGrayscale) {
		static GLint swizzleRgbaParams[4] = { GL_RED, GL_RED, GL_RED, GL_ONE  };
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRgbaParams);
	} else if (blob->dataFormat == RSGL_formatGrayscaleAlpha) {
		static GLint swizzleRgbaParams[4] = { GL_ONE, GL_ONE, GL_ONE, GL_RED };
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRgbaParams);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, blob->width, blob->height, 0, textureFormat, dataType, blob->data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return id;
}

void RSGL_GL_copyToTexture(RSGL_glRenderer* ctx, RSGL_texture texture, size_t x, size_t y, const RSGL_textureBlob* blob) {
	glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, blob->width);

	u32 dataFormat = RSGL_GL_textureFormatToNative(blob->dataFormat);
	u32 dataType = RSGL_GL_textureDataTypeToNative(blob->dataType);

	if (blob->dataFormat == RSGL_formatGrayscale) {
		static GLint swizzleRgbaParams[4] = { GL_RED, GL_RED, GL_RED, GL_ONE  };
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRgbaParams);
	} else 	if (blob->dataFormat == RSGL_formatGrayscaleAlpha) {
		static GLint swizzleRgbaParams[4] = { GL_ONE, GL_ONE, GL_ONE, GL_RED };
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRgbaParams);
	}

	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, (i32)blob->width, (i32)blob->height, dataFormat, dataType, blob->data);
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

RSGL_programInfo RSGL_GL_createProgram(RSGL_glRenderer* ctx, RSGL_programBlob* blob) {
	RSGL_programInfo program;
	u32 vShader, fShader;

	/* compile vertex shader */
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &blob->vertex, NULL);
	glCompileShader(vShader);

#ifdef RSGL_DEBUG
	RSGL_debug_shader(vShader, "Vertex", "compile");
#endif

	/* compile fragment shader */
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &blob->fragment, NULL);
	glCompileShader(fShader);

#ifdef RSGL_DEBUG
	RSGL_debug_shader(fShader, "Fragment", "compile");
#endif

	/* create program and link vertex and fragment shaders */
	program.program = glCreateProgram();

	glAttachShader(program.program, vShader);
	glAttachShader(program.program, fShader);
	glLinkProgram(program.program);

#ifdef RSGL_DEBUG
	RSGL_debug_shader(program.program, "Program", "link");
#endif

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	glUseProgram(program.program);
	program.perspectiveView = glGetUniformLocation(program.program, "pv");
	program.model = glGetUniformLocation(program.program, "model");
	glUseProgram(0);

	program.type = RSGL_shaderTypeStandard;
	return program;
}

void RSGL_GL_deleteProgram(RSGL_glRenderer* ctx, const RSGL_programInfo* program) {
    glUseProgram(0);
    glDeleteProgram(program->program);
}

size_t RSGL_GL_findShaderVariable(RSGL_glRenderer* ctx, const RSGL_programInfo* program, const char* var, const size_t len) {
	glUseProgram(program->program);
    int loc = glGetUniformLocation(program->program, var);
    glUseProgram(0);
	return loc;
}

void RSGL_GL_updateShaderVariable(RSGL_glRenderer* ctx, const RSGL_programInfo* program, size_t var, const float value[], u8 len) {
	glUseProgram(program->program);
    int loc = (int)var;

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

RSGL_framebuffer RSGL_GL_createFramebuffer(RSGL_glRenderer* ctx, size_t width, size_t height) {
	u32 result = 0;
	glGenFramebuffers(1, &result);
    return (RSGL_framebuffer)result;
}

void RSGL_GL_attachFramebuffer(RSGL_glRenderer* ctx, RSGL_framebuffer fbo, RSGL_texture tex, u8 attachType, u8 mipLevel) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    if (attachType < 8)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachType, GL_TEXTURE_2D, tex, mipLevel);

    if (attachType == 100)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex, mipLevel);

    if (attachType == 200)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex, mipLevel);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RSGL_GL_deleteFramebuffer(RSGL_glRenderer* ctx, RSGL_framebuffer fbo) {
	u32 value = fbo;
    glDeleteFramebuffers(1, &value);
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
    RSGL_PROC_DEF(proc, glBindFramebuffer);
    RSGL_PROC_DEF(proc, glGenFramebuffers);
    RSGL_PROC_DEF(proc, glDeleteFramebuffers);
    RSGL_PROC_DEF(proc, glFramebufferTexture2D);
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
        glUniformMatrix4fvSRC == NULL ||
		glBindFramebufferSRC == NULL ||
		glGenFramebuffersSRC == NULL ||
		glDeleteFramebuffersSRC == NULL ||
		glFramebufferTexture2DSRC == NULL
    )
        return 1;
    return 0;
}
#endif

#endif /* RSGL_IMPLEMENTATION */
