#ifndef RSGL_H
#include "RSGL.h"
#endif

#ifndef RSGL_GET_WORLD_X
#define RSGL_GET_WORLD_X(x) (float)(2.0f * (x) / RSGL_args.currentArea.w - 1.0f)
#define RSGL_GET_WORLD_Y(y) (float)(1.0f + -2.0f * (y) / RSGL_args.currentArea.h)
#define RSGL_GET_WORLD_Z(z) (float)(z)
#endif

#define RSGL_GET_WORLD_POINT(x, y, z) RSGL_GET_WORLD_X(x), RSGL_GET_WORLD_Y(y), RSGL_GET_WORLD_Z(z)


#ifndef RSGL_GL_H
#define RSGL_GL_H

/* toggle the use of legacy OpenGL, on by default unless it fails to load */
RSGLDEF void RSGL_GL_setLegacy(i32 legacy);
RSGLDEF RSGL_renderer RSGL_GL_renderer(void);

RSGLDEF void RSGL_GL_batch(RSGL_RENDER_INFO* info);
RSGLDEF void RSGL_GL_init(void* proc, RSGL_RENDER_INFO* info); /* init render backend */
RSGLDEF void RSGL_GL_free(void); /* free render backend */
RSGLDEF void RSGL_GL_clear(float r, float g, float b, float a);
RSGLDEF void RSGL_GL_viewport(i32 x, i32 y, i32 w, i32 h);
/* create a texture based on a given bitmap, this must be freed later using RSGL_deleteTexture or opengl*/
RSGLDEF RSGL_texture RSGL_GL_createTexture(u8* bitmap, RSGL_area memsize,  u8 channels);
/* updates an existing texture wiht a new bitmap */
RSGLDEF void RSGL_GL_updateTexture(RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels);
/* delete a texture */
RSGLDEF void RSGL_GL_deleteTexture(RSGL_texture tex);
/* starts scissoring */
RSGLDEF void RSGL_GL_scissorStart(RSGL_rectF scissor);
/* stops scissoring */
RSGLDEF void RSGL_GL_scissorEnd(void);
/* program loading */
RSGLDEF RSGL_programInfo RSGL_GL_createProgram(const char* VShaderCode, const char* FShaderCode, const char* posName, const char* texName, const char* colorName);
RSGLDEF void RSGL_GL_deleteProgram(RSGL_programInfo program);
RSGLDEF void RSGL_GL_setShaderValue(u32 program, char* var, float value[], u8 len);
#ifndef RSGL_NO_TEXT
/* RFont */
RFont_texture RFont_GL_create_atlas(u32 atlasWidth, u32 atlasHeight);
b8 RFont_GL_resize_atlas(RFont_texture* atlas, u32 newWidth, u32 newHeight);
void RFont_GL_bitmap_to_atlas(RFont_texture atlas, u8* bitmap, float x, float y, float w, float h);
#endif /* RSGL_NO_TEXT */

#ifdef RSGL_USE_COMPUTE
RSGLDEF RSGL_programInfo RSGL_GL_createComputeProgram(const char* CShaderCode);
RSGLDEF void RSGL_GL_dispatchComputeProgram(RSGL_programInfo program, u32 groups_x, u32 groups_y, u32 groups_z);
RSGLDEF void RSGL_GL_bindComputeTexture(u32 texture, u8 format);
#endif

#endif

#ifdef RSGL_IMPLEMENTATION

b8 RSGL_GL_legacy = 0;
void RSGL_GL_setLegacy(i32 legacy) {
    RSGL_GL_legacy = legacy;
}

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#ifdef __EMSCRIPTEN__
#define RSGL_OPENGL_ES2
#define RSGL_NO_GL_LOADER
#include <GLES3/gl3.h>
#endif

#if !defined(RSGL_RENDER_LEGACY)
#define RSGL_MODERN_OPENGL
#if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_33) && !defined(RSGL_OPENGL_43) && !defined(RSGL_OPENGL_ES2)
#define RSGL_OPENGL_33
#endif
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

#if !defined(RSGL_NO_GL_LOADER) && defined(RSGL_MODERN_OPENGL)

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

#if defined(RSGL_OPENGL_ES2) && !defined(RSGL_OPENGL_ES3)
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

typedef struct RSGL_INFO {
    RSGL_programInfo program;       /* Default shader program id, supports vertex color and diffuse texture*/
    u32 defaultTex;

    u32 vao, vbo, tbo, cbo; /* array object and array buffers */
} RSGL_INFO;

RSGL_INFO RSGL_gl;

RSGL_renderer RSGL_GL_renderer() {
    RSGL_renderer renderer;
    renderer.batch = RSGL_GL_batch;
    renderer.init = RSGL_GL_init;
    renderer.free = RSGL_GL_free;
    renderer.clear = RSGL_GL_clear;
    renderer.viewport = RSGL_GL_viewport;
    renderer.createTexture = RSGL_GL_createTexture;
    renderer.updateTexture = RSGL_GL_updateTexture;
    renderer.deleteTexture = RSGL_GL_deleteTexture;
    renderer.scissorStart = RSGL_GL_scissorStart;
    renderer.scissorEnd =  RSGL_GL_scissorEnd;
    renderer.createProgram = RSGL_GL_createProgram;
    renderer.deleteProgram = RSGL_GL_deleteProgram;
    renderer.setShaderValue = RSGL_GL_setShaderValue;
    #ifndef RSGL_NO_TEXT
    renderer.createAtlas = RFont_GL_create_atlas;
    renderer.resizeAtlas = RFont_GL_resize_atlas;
    renderer.bitmapToAtlas = RFont_GL_bitmap_to_atlas;
    #endif /* RSGL_NO_TEXT */

#ifdef RSGL_USE_COMPUTE
	renderer.createComputeProgram = RSGL_GL_createComputeProgram;
	renderer.dispatchComputeProgram = RSGL_GL_dispatchComputeProgram;
	renderer.bindComputeTexture = RSGL_GL_bindComputeTexture;
#endif
    return renderer;
}

void RSGL_GL_deleteTexture(RSGL_texture tex) { glDeleteTextures(1, (u32*)&tex); }
void RSGL_GL_viewport(i32 x, i32 y, i32 w, i32 h) { glViewport(x, y, w ,h); }

void RSGL_GL_clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RSGL_GL_init(void* proc, RSGL_RENDER_INFO* info) {
    RSGL_UNUSED(info);

    #ifdef RSGL_MODERN_OPENGL
    #if !defined(__EMSCRIPTEN__) && !defined(RSGL_NO_GL_LOADER)
    if (RSGL_loadGLModern((RSGLloadfunc)proc)) {
        RSGL_GL_legacy = 2;
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

    static const char *defaultVShaderCode = 
#ifdef RSGL_OPENGL_21
RSGL_MULTILINE_STR(
        \x23version 120                       \n
        attribute vec3 vertexPosition;     \n
        attribute vec2 vertexTexCoord;     \n
        attribute vec4 vertexColor;        \n
        varying vec2 fragTexCoord;         \n
        varying vec4 fragColor;            \n
     
        uniform mat4 mat; \n
        void main() {
            fragTexCoord = vertexTexCoord;
            fragColor = vertexColor;
            gl_Position = mat * vec4(vertexPosition, 1.0);
        }
    );
#elif defined(RSGL_OPENGL_33)
RSGL_MULTILINE_STR(
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
#elif defined(RSGL_OPENGL_ES2)
    RSGL_MULTILINE_STR(
        \x23version 100                     \n
        precision mediump float;           \n
        attribute vec3 vertexPosition;     \n
        attribute vec2 vertexTexCoord;     \n
        attribute vec4 vertexColor;        \n
        varying vec2 fragTexCoord;         \n
        varying vec4 fragColor;            \n
        uniform mat4 mat; \n

        void main() {
            fragTexCoord = vertexTexCoord;
            fragColor = vertexColor;
            gl_Position = mat * vec4(vertexPosition, 1.0);
        }
    );
#endif

        static const char* defaultFShaderCode =
#if defined(RSGL_OPENGL_21)
RSGL_MULTILINE_STR(
    \x23version 120 \n
    varying vec2 fragTexCoord;
    varying vec4 fragColor;
    void main() { 
            gl_FragColor = texture2D(texture0, fragTexCoord) * fragColor;
        }                               
    );
#elif defined(RSGL_OPENGL_33)
RSGL_MULTILINE_STR(
    \x23version 330       \n
    in vec2 fragTexCoord;
    in vec4 fragColor;
    out vec4 finalColor;
    uniform sampler2D texture0;        
    void main() { 
            finalColor = texture(texture0, fragTexCoord) * fragColor;
        }                               
    );
#elif defined(RSGL_OPENGL_ES2)
RSGL_MULTILINE_STR(
        \x23version 100                    \n
        precision mediump float;           \n
        varying vec2 fragTexCoord;         \n
        varying vec4 fragColor;            \n
        uniform sampler2D texture0;        
        void main() { 
            gl_FragColor = texture2D(texture0, fragTexCoord) * fragColor;
        }                               
    );
#else
RSGL_MULTILINE_STR(
        uniform sampler2D texture0;        
        void main() { 
            gl_FragColor = texture2D(texture0, fragTexCoord) * fragColor;
        }                               
    );
#endif

    #if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_ES2)
	glGenVertexArrays(1, &RSGL_gl.vao);
	glBindVertexArray(RSGL_gl.vao);
    #endif

	glGenBuffers(1, &RSGL_gl.vbo);
	glGenBuffers(1, &RSGL_gl.tbo);
	glGenBuffers(1, &RSGL_gl.cbo);

    RSGL_gl.program = RSGL_renderCreateProgram(defaultVShaderCode, defaultFShaderCode, "vertexPosition", "vertexTexCoord", "vertexColor");
    
    /* Init default vertex arrays buffers */
    /* Initialize CPU (RAM) vertex buffers (position, texcoord, color data and indexes) */

    #if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_ES2)
    glBindVertexArray(RSGL_gl.vao);
    #endif

    /* Quads - Vertex buffers binding and attributes enable */
    /* Vertex position buffer (shader-location = 0) */
    glBindBuffer(GL_ARRAY_BUFFER, RSGL_gl.vbo);
    glBufferData(GL_ARRAY_BUFFER, RSGL_MAX_VERTS * 3 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);

    /* Vertex texcoord buffer (shader-location = 1) */
    glBindBuffer(GL_ARRAY_BUFFER, RSGL_gl.tbo);
    glBufferData(GL_ARRAY_BUFFER, RSGL_MAX_VERTS * 2 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);
    
    /* Vertex color buffer (shader-location = 3) */
    glBindBuffer(GL_ARRAY_BUFFER, RSGL_gl.cbo);
    glBufferData(GL_ARRAY_BUFFER, RSGL_MAX_VERTS * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
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

    #else
    RSGL_UNUSED(proc);
    #endif
}

void RSGL_GL_free(void) {   
    #ifdef RSGL_MODERN_OPENGL 
    if (RSGL_GL_legacy == 2)
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

    #if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_ES2)
    glDeleteVertexArrays(1, &RSGL_gl.vao);
    #endif

    RSGL_renderDeleteProgram(RSGL_gl.program);

    glDeleteTextures(1, (u32*)&RSGL_gl.defaultTex); /* Unload default texture */
    #endif
}

void RSGL_GL_batch(RSGL_RENDER_INFO* info) { 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    #ifdef RSGL_MODERN_OPENGL
    if (info->vert_len > 0 && RSGL_GL_legacy == 0) {
        #if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_ES2)
        glBindVertexArray(RSGL_gl.vao);
        #endif

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

        u32 i;
        for (i = 0; i < info->len; i++) {
            GLenum mode = info->batches[i].type;

            if (mode > 0x0100) {
                mode -= 0x0100;
            }

            if (mode > 0x0010) {
                mode -= 0x0010;
            }

            /* Bind current draw call texture, activated as GL_TEXTURE0 and Bound to sampler2D texture0 by default */
            if (info->batches[i].tex == 0)
                info->batches[i].tex = RSGL_gl.defaultTex;
            
            glBindTexture(GL_TEXTURE_2D, info->batches[i].tex);
            glLineWidth(info->batches[i].lineWidth > 0 ? info->batches[i].lineWidth : 0.1f);
            
            if (RSGL_args.program)
                glUseProgram(RSGL_args.program);
            else {
                glUseProgram(RSGL_gl.program.program);

                int loc = glGetUniformLocation(RSGL_gl.program.program, "mat");
                glUniformMatrix4fv(loc, 1, GL_FALSE, info->batches[i].matrix.m);
           }

            glDrawArrays(mode, info->batches[i].start, info->batches[i].len);
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

    else if (RSGL_GL_legacy)
    #endif
    #ifndef RSGL_GL_NO_LEGACY
    {
        size_t i, j;
        size_t tIndex = 0, cIndex = 0, vIndex = 0;
        for (i = 0; i < info->len; i++) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, info->batches[i].tex);
            glLineWidth(info->batches[i].lineWidth > 0 ? info->batches[i].lineWidth : 0.1f);

            u32 mode = info->batches[i].type;
            glEnable(GL_BLEND);
            glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            //glDisable(GL_BLEND);

            //glDisable(GL_DEPTH_TEST);
             //   glDepthMask(GL_FALSE);
        
            //glEnable(GL_DEPTH_TEST);
            //glDepthMask(GL_TRUE);

            glBegin(mode);

            for (j = info->batches[i].start; j < info->batches[i].len; j++) {
                glTexCoord2f(info->texCoords[tIndex], info->texCoords[tIndex + 1]);
                glColor4f(info->colors[cIndex], info->colors[cIndex + 1], info->colors[cIndex + 2], info->colors[cIndex + 3]);
                glVertex3f(info->verts[vIndex], info->verts[vIndex + 1],  info->verts[vIndex + 2]);

                tIndex += 2;
                vIndex += 3;
                cIndex += 4;
            }

            glEnd();
            // glEnable(GL_DEPTH_TEST);
        }
    }
    #endif

    info->len = 0;
    info->vert_len = 0;
}

void RSGL_GL_scissorStart(RSGL_rectF scissor) {
    RSGL_draw();
    glEnable(GL_SCISSOR_TEST);

    glScissor(scissor.x, RSGL_args.currentArea.h - (scissor.y + scissor.h), scissor.w, scissor.h);
    glScissor(scissor.x, scissor.y, scissor.w, scissor.h);
}

void RSGL_GL_scissorEnd(void) {
    RSGL_draw();
    glDisable(GL_SCISSOR_TEST);
}

#ifndef GL_RG
#define GL_RG                             0x8227
#endif

/* textures / images */
RSGL_texture RSGL_GL_createTexture(u8* bitmap, RSGL_area memsize, u8 channels) {
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

void RSGL_GL_updateTexture(RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels) {
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

#ifdef RSGL_MODERN_OPENGL

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

RSGL_programInfo RSGL_GL_createProgram(const char* VShaderCode, const char* FShaderCode, const char* posName, const char* texName, const char* colorName) {
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

void RSGL_GL_deleteProgram(RSGL_programInfo program) {
    glUseProgram(0);
    glDeleteProgram(program.program);
}

void RSGL_GL_setShaderValue(u32 program, char* var, float value[], u8 len) {
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
#else
RSGL_programInfo RSGL_GL_createProgram(const char* VShaderCode, const char* FShaderCode, const char* posName, const char* texName, const char* colorName) {
    RSGL_UNUSED(VShaderCode); RSGL_UNUSED(FShaderCode); RSGL_UNUSED(posName); RSGL_UNUSED(texName); RSGL_UNUSED(colorName);
    RSGL_programInfo program = {0};
    return program;
}

void RSGL_GL_deleteProgram(RSGL_programInfo program) {
    RSGL_UNUSED(program);
}

void RSGL_GL_setShaderValue(u32 program, char* var, float value[], u8 len) {
    RSGL_UNUSED(program); RSGL_UNUSED(var); RSGL_UNUSED(value); RSGL_UNUSED(len);
}
#endif

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

#ifndef RSGL_NO_TEXT
RFont_texture RFont_GL_create_atlas(u32 atlasWidth, u32 atlasHeight) {
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

b8 RFont_GL_resize_atlas(RFont_texture* atlas, u32 newWidth, u32 newHeight) {
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

void RFont_push_pixel_values(GLint alignment, GLint rowLength, GLint skipPixels, GLint skipRows);
void RFont_push_pixel_values(GLint alignment, GLint rowLength, GLint skipPixels, GLint skipRows) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, rowLength);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, skipPixels);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, skipRows);
}

void RFont_GL_bitmap_to_atlas(RFont_texture atlas, u8* bitmap, float x, float y, float w, float h) {
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
#endif /* RSGL_NO_TEXT */

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

RSGL_programInfo RSGL_GL_createComputeProgram(const char* CShaderCode) {
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

void RSGL_GL_dispatchComputeProgram(RSGL_programInfo program, u32 groups_x, u32 groups_y, u32 groups_z) {
	glUseProgram(program.program);
	glDispatchCompute(groups_x, groups_y, groups_z);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}


void RSGL_GL_bindComputeTexture(u32 texture, u8 format) {
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

#ifdef RSGL_MODERN_OPENGL

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
    #if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_ES2) && !defined(RSGL_OPENGL_ES3)
    RSGL_PROC_DEF(proc, glBindVertexArray);
    RSGL_PROC_DEF(proc, glGenVertexArrays);
    RSGL_PROC_DEF(proc, glDeleteVertexArrays);
    #endif
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
    #if defined(RSGL_OPENGL_ES2) && !defined(RSGL_OPENGL_ES3)
        glGenVertexArraysSRC = (PFNGLGENVERTEXARRAYSOESPROC)((RSGLloadfunc)loader)("glGenVertexArraysOES");
        glBindVertexArraySRC = (PFNGLBINDVERTEXARRAYOESPROC)((RSGLloadfunc)loader)("glBindVertexArrayOES");
        glDeleteVertexArraysSRC = (PFNGLDELETEVERTEXARRAYSOESPROC)((RSGLloadfunc)loader)("glDeleteVertexArraysOES");
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
    
    #if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_ES2)
    glDeleteVertexArraysSRC(1, &vao);
    #endif

    return 0;
}
#endif

#endif /* RSGL_MODERN_OPENGL */
#endif /* RSGL_IMPLEMENTATION */
