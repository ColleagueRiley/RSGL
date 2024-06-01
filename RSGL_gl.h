#ifdef RSGL_CUSTOM_RENDER
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#endif

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#if !defined(RSGL_RENDER_LEGACY)
#define RSGL_MODERN_OPENGL
#if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_33) && !defined(RSGL_OPENGL_43)
#define RSGL_OPENGL_33
#endif
#endif

#if defined(__WIN32) && !defined(__linux__) && !defined(GL_VERTEX_SHADER)
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
#define RSGL_PROC_DEF(proc, name) name##SRC = (name##PROC)proc(#name)

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

extern int RSGL_loadGLModern(RSGLloadfunc proc);

#define RSGL_MULTILINE_STR(...) #__VA_ARGS__

typedef struct RSGL_INFO {
    u32 vShader;      /* Default vertex shader id (used by default shader program)*/
    u32 fShader;      /* Default fragment shader id (used by default shader program)*/

    u32 program;       /* Default shader program id, supports vertex color and diffuse texture*/
    u32 defaultTex;

    u32 vao, vbo, tbo, cbo; /* array object and array buffers */
} RSGL_INFO;

RSGL_INFO RSGL_gl;
#endif

void RSGL_renderDeleteTexture(u32 tex) { glDeleteTextures(1, &tex); }
void RSGL_renderViewport(i32 x, i32 y, i32 w, i32 h) { glViewport(x, y, w ,h); }

void RSGL_renderClear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RSGL_renderInit(void* proc, RSGL_RENDER_INFO* info) {
    #ifdef RSGL_MODERN_OPENGL
    if (RSGL_loadGLModern((RSGLloadfunc)proc)) {
        RSGL_args.legacy = 2;
        #ifdef RSGL_DEBUG
        printf("Failed to load an OpenGL 3.3 Context, reverting to OpenGL Legacy\n");
        #endif
        return;
    }
    
    static const char *defaultVShaderCode = RSGL_MULTILINE_STR(
    #if defined(RSGL_OPENGL_21)
        \x23version 120                       \n
        attribute vec3 vertexPosition;     \n
        attribute vec2 vertexTexCoord;     \n
        attribute vec4 vertexColor;        \n
        varying vec2 fragTexCoord;         \n
        varying vec4 fragColor;            \n
    #elif defined(RSGL_OPENGL_33)
        \x23version 330                     \n
        in vec3 vertexPosition;            \n
        in vec2 vertexTexCoord;            \n
        in vec4 vertexColor;               \n
        out vec2 fragTexCoord;             \n
        out vec4 fragColor;                \n
    #endif
    #if defined(RSGL_OPENGL_ES2)
        \x23version 100                     \n
        precision mediump float;           \n
        attribute vec3 vertexPosition;     \n
        attribute vec2 vertexTexCoord;     \n
        attribute vec4 vertexColor;        \n
        varying vec2 fragTexCoord;         \n
        varying vec4 fragColor;            \n
    #endif
        void main() {
            fragTexCoord = vertexTexCoord;
            fragColor = vertexColor;
            gl_Position = vec4(vertexPosition, 1.0);
        }
    );

    static const char* defaultFShaderCode = RSGL_MULTILINE_STR(
#if defined(RSGL_OPENGL_21)
    \x23version 120 \n
    varying vec2 fragTexCoord;
    varying vec4 fragColor;
#elif defined(RSGL_OPENGL_33)
    \x23version 330       \n
    in vec2 fragTexCoord;
    in vec4 fragColor;
    out vec4 finalColor;
#endif
#if defined(RSGL_OPENGL_ES2)
        \x23version 100                    \n
        precision mediump float;           \n
        varying vec2 fragTexCoord;         \n
        varying vec4 fragColor;            \n
#endif
        uniform sampler2D texture0;        
        void main() { 
            #ifdef RSGL_OPENGL_33
            finalColor = texture(texture0, fragTexCoord) * fragColor;
            #else
            gl_FragColor = texture2D(texture0, fragTexCoord) * fragColor;
            #endif
        }                               
    );

    #if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_ES2)
	glGenVertexArrays(1, &RSGL_gl.vao);
	glBindVertexArray(RSGL_gl.vao);
    #endif

	glGenBuffers(1, &RSGL_gl.vbo);
	glGenBuffers(1, &RSGL_gl.tbo);
	glGenBuffers(1, &RSGL_gl.cbo);

	/* compile vertex shader */
	RSGL_gl.vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(RSGL_gl.vShader, 1, &defaultVShaderCode, NULL);
	glCompileShader(RSGL_gl.vShader);

	/* compile fragment shader */
	RSGL_gl.fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(RSGL_gl.fShader, 1, &defaultFShaderCode, NULL);
	glCompileShader(RSGL_gl.fShader);

	/* create program and link vertex and fragment shaders */
	RSGL_gl.program = glCreateProgram();

	glAttachShader(RSGL_gl.program, RSGL_gl.vShader);
	glAttachShader(RSGL_gl.program, RSGL_gl.fShader);

    glBindAttribLocation(RSGL_gl.program, 0, "vertexPosition");
    glBindAttribLocation(RSGL_gl.program, 1, "vertexTexCoord");
    glBindAttribLocation(RSGL_gl.program, 2, "vertexColor");

	glLinkProgram(RSGL_gl.program);

    RSGL_gl.program = RSGL_gl.program;

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

    #else
    RSGL_UNUSED(proc); RSGL_UNUSED(info);
    #endif
}

void RSGL_renderFree(void) {   
    #ifdef RSGL_MODERN_OPENGL 
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

    #if !defined(RSGL_OPENGL_21) && !defined(RSGL_OPENGL_ES2)
    glDeleteVertexArrays(1, &RSGL_gl.vao);
    #endif

    glUseProgram(0);

    glDetachShader(RSGL_gl.program, RSGL_gl.vShader);
    glDetachShader(RSGL_gl.program, RSGL_gl.fShader);
    glDeleteShader(RSGL_gl.vShader);
    glDeleteShader(RSGL_gl.fShader);

    glDeleteProgram(RSGL_gl.program);

    glDeleteTextures(1, &RSGL_gl.defaultTex); /* Unload default texture */
    #endif
}

void RSGL_renderBatch(RSGL_RENDER_INFO* info) { 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    #ifdef RSGL_MODERN_OPENGL
    if (info->vert_len > 0 && RSGL_args.legacy == 0) {
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
        glUseProgram(RSGL_gl.program);
        
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
            
            glUseProgram(RSGL_gl.program);
            
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

    else if (RSGL_args.legacy)
    #endif
    {
        size_t i, j;
        size_t tIndex = 0, cIndex = 0, vIndex = 0;
        for (i = 0; i < info->len; i++) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, info->batches[i].tex);
            glLineWidth(info->batches[i].lineWidth);

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

            if (info->batches[i].type > 0x0010)
                glEnable(GL_DEPTH_TEST);
        }
    }

    info->len = 0;
    info->vert_len = 0;
}

#ifndef GL_RG
#define GL_RG                             0x8227
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