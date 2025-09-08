#ifndef RSGL_H
#include "RSGL.h"
#endif

// WebGL doesn't support compute shaders iirc so yeah
#if defined(__EMSCRIPTEN__) && defined(RSGL_USE_COMPUTE)
#undef RSGL_USE_COMPUTE
#endif

#ifndef RSGL_DX11_H
#define RSGL_DX11_H

/* prevent winapi conflicts (drect includes windows.h ) */
#define OEMRESOURCE

#include <d3d11.h>
#include <d3dcompiler.h>

typedef struct RSGL_dx11Context {
	IDXGIFactory* pFactory;
	IDXGIAdapter* pAdapter;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
} RSGL_dx11Context;

typedef struct RSGL_dx11Surface {
	IDXGISwapChain* swapchain;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* pDepthStencilView;
} RSGL_dx11Surface;

typedef struct RSGL_dx11Renderer {
    RSGL_programInfo program;       /* Default shader program id, supports vertex color and diffuse texture*/
	size_t defaultTex;
	RSGL_dx11Context* ctx;
	RSGL_dx11Surface* surf;
} RSGL_dx11Renderer;

typedef struct RSGL_dx11Program {
    ID3D11VertexShader* pVertexShader;
    ID3D11PixelShader* pPixelShader;
    ID3D11InputLayout* pInputLayout;
} RSGL_dx11Program;

RSGLDEF RSGL_rendererProc RSGL_DX11_rendererProc(void);
RSGLDEF size_t RSGL_DX11_size(void);

RSGLDEF RSGL_renderer* RSGL_DX11_renderer_init(RSGL_area r, RSGL_dx11Context*  loader);
RSGLDEF void RSGL_DX11_renderer_initPtr(RSGL_area r, RSGL_dx11Context* loader, RSGL_dx11Renderer* ptr, RSGL_renderer* renderer);
RSGLDEF void RSGL_DX11_render(RSGL_dx11Renderer* ctx, RSGL_programInfo program, const RSGL_renderBuffers* info);
RSGLDEF void RSGL_DX11_initPtr(RSGL_dx11Renderer* ctx, RSGL_dx11Context* proc); /* init render backend */
RSGLDEF void RSGL_DX11_freePtr(RSGL_dx11Renderer* ctx); /* free render backend */
RSGLDEF void RSGL_DX11_clear(RSGL_dx11Renderer* ctx, float r, float g, float b, float a);
RSGLDEF void RSGL_DX11_viewport(RSGL_dx11Renderer* ctx, i32 x, i32 y, i32 w, i32 h);
RSGLDEF void RSGL_DX11_setSurface(RSGL_dx11Renderer* ctx, RSGL_dx11Surface* surface);
RSGLDEF void RSGL_DX11_createBuffer(RSGL_dx11Renderer* ctx, size_t size, const void* data, size_t* buffer);
RSGLDEF void RSGL_DX11_updateBuffer(RSGL_dx11Renderer* ctx, size_t buffer, const void* data, size_t start, size_t end);
RSGLDEF void RSGL_DX11_deleteBuffer(RSGL_dx11Renderer* ctx, size_t buffer);
/* create a texture based on a given bitmap, this must be freed later using RSGL_deleteTexture or opengl*/
RSGLDEF RSGL_texture RSGL_DX11_createTexture(RSGL_dx11Renderer* ctx, u8* bitmap, RSGL_area memsize,  u8 channels);
/* updates an existing texture wiht a new bitmap */
RSGLDEF void RSGL_DX11_updateTexture(RSGL_dx11Renderer* ctx, RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels);
/* delete a texture */
RSGLDEF void RSGL_DX11_deleteTexture(RSGL_dx11Renderer* ctx, RSGL_texture tex);
/* starts scissoring */
RSGLDEF void RSGL_DX11_scissorStart(RSGL_dx11Renderer* ctx, RSGL_rectF scissor, i32 renderer_height);
/* stops scissoring */
RSGLDEF void RSGL_DX11_scissorEnd(RSGL_dx11Renderer* ctx);
/* program loading */
RSGLDEF RSGL_programInfo RSGL_DX11_createProgram(RSGL_dx11Renderer* ctx, const char* VShaderCode, const char* FShaderCode, const char* posName, const char* texName, const char* colorName);
RSGLDEF void RSGL_DX11_deleteProgram(RSGL_dx11Renderer* ctx, RSGL_programInfo program);
RSGLDEF void RSGL_DX11_setShaderValue(RSGL_dx11Renderer* ctx, u32 program, const char* var, const float value[], u8 len);
RSGLDEF RSGL_texture RSGL_DX11_create_atlas(RSGL_dx11Renderer* ctx, u32 atlasWidth, u32 atlasHeight);
RSGLDEF void RSGL_DX11_bitmap_to_atlas(RSGL_dx11Renderer* ctx, RFont_texture atlas, u32 atlasWidth, u32 atlasHeight, u32 maxHeight, u8* bitmap, float w, float h, float* x, float* y);
#ifdef RSGL_USE_COMPUTE
RSGLDEF RSGL_programInfo RSGL_DX11_createComputeProgram(RSGL_dx11Renderer* ctx, const char* CShaderCode);
RSGLDEF void RSGL_DX11_dispatchComputeProgram(RSGL_dx11Renderer* ctx, RSGL_programInfo program, u32 groups_x, u32 groups_y, u32 groups_z);
RSGLDEF void RSGL_DX11_bindComputeTexture(RSGL_dx11Renderer* ctx, u32 texture, u8 format);
#endif
#endif

#ifdef RSGL_IMPLEMENTATION
RSGL_renderer* RSGL_DX11_renderer_init(RSGL_area r, RSGL_dx11Context* loader) { return RSGL_renderer_init(RSGL_DX11_rendererProc(), r, loader); }
void RSGL_DX11_renderer_initPtr(RSGL_area r, RSGL_dx11Context* loader, RSGL_dx11Renderer* ptr, RSGL_renderer* renderer) { return RSGL_renderer_initPtr(RSGL_DX11_rendererProc(), r, loader, ptr, renderer); }

#define RSGL_MULTILINE_STR(...) #__VA_ARGS__
size_t RSGL_DX11_size(void) {
	return sizeof(RSGL_dx11Renderer);
}

RSGL_rendererProc RSGL_DX11_rendererProc() {
	RSGL_rendererProc proc;
	RSGL_MEMSET(&proc, 0, sizeof(proc));

	proc.render = (void (*)(void*, RSGL_programInfo, const RSGL_renderBuffers*))RSGL_DX11_render;
    proc.size = (size_t (*)(void))RSGL_DX11_size;
    proc.initPtr = (void (*)(void*, void*))RSGL_DX11_initPtr;
    proc.freePtr = (void (*)(void*))RSGL_DX11_freePtr;
    proc.clear = (void (*)(void*, float, float, float, float))RSGL_DX11_clear;
    proc.viewport = (void (*)(void*, i32, i32, i32, i32))RSGL_DX11_viewport;
    proc.createTexture = (RSGL_texture (*)(void*, u8*, RSGL_area,  u8))RSGL_DX11_createTexture;
    proc.updateTexture = (void (*)(void*, RSGL_texture, u8*, RSGL_area, u8))RSGL_DX11_updateTexture;
    proc.deleteTexture = (void (*)(void*, RSGL_texture))RSGL_DX11_deleteTexture;
	proc.setSurface = (void (*)(void*, void*))RSGL_DX11_setSurface;
    proc.scissorStart = (void (*)(void*, RSGL_rectF, i32))RSGL_DX11_scissorStart;
    proc.scissorEnd =  (void (*)(void*))RSGL_DX11_scissorEnd;
    proc.createProgram = (RSGL_programInfo (*)(void*, const char*, const char*, const char*, const char*, const char*))RSGL_DX11_createProgram;
    proc.deleteProgram = (void (*)(void*, RSGL_programInfo))RSGL_DX11_deleteProgram;
    proc.setShaderValue = (void (*)(void*, u32, const char*, const float[], u8))RSGL_DX11_setShaderValue;
    proc.createAtlas = (RSGL_texture (*)(void*, u32, u32))RSGL_DX11_create_atlas;
    proc.bitmapToAtlas = (void(*)(void*, RSGL_texture, u32, u32, u32, u8*, float, float, float*, float*))RSGL_DX11_bitmap_to_atlas;
	proc.createBuffer = (void (*)(void*, size_t, const void*, size_t*))RSGL_DX11_createBuffer;
	proc.updateBuffer = (void (*)(void*, size_t, void*, size_t, size_t))RSGL_DX11_updateBuffer;
	proc.deleteBuffer = (void (*)(void*, size_t))RSGL_DX11_deleteBuffer;

#ifdef RSGL_USE_COMPUTE
	proc.createComputeProgram = (RSGL_programInfo (*)(void*, const char*))RSGL_DX11_createComputeProgram;
	proc.dispatchComputeProgram = (void (*)(void*, RSGL_programInfo, u32, u32, u32))RSGL_DX11_dispatchComputeProgram;
	proc.bindComputeTexture = (void (*)(void*, u32, u8))RSGL_DX11_bindComputeTexture;
#else
	proc.createComputeProgram = NULL;
	proc.dispatchComputeProgram = NULL;
	proc.bindComputeTexture = NULL;
#endif
	return proc;
}

void RSGL_DX11_deleteTexture(RSGL_dx11Renderer* ctx, RSGL_texture tex) {
	/* TODO */
}
void RSGL_DX11_viewport(RSGL_dx11Renderer* ctx, i32 x, i32 y, i32 w, i32 h) {
	/* TODO */
}

void RSGL_DX11_setSurface(RSGL_dx11Renderer* ctx, RSGL_dx11Surface* surface) {
	ctx->surf = surface;
}

void RSGL_DX11_clear(RSGL_dx11Renderer* ctx, float r, float g, float b, float a) {
	float clearColor[4] = { r, g, b, a };
	ctx->ctx->pDeviceContext->lpVtbl->ClearRenderTargetView(ctx->ctx->pDeviceContext, ctx->surf->renderTargetView, clearColor);
}

void RSGL_DX11_createBuffer(RSGL_dx11Renderer* ctx, size_t size, const void* data, size_t* buffer) {
    ID3D11Buffer* pVertexBuffer;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = size;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    ctx->ctx->pDevice->lpVtbl->CreateBuffer(ctx->ctx->pDevice, &bd, NULL, &pVertexBuffer);

	if (data) {
		RSGL_DX11_updateBuffer(ctx, (size_t)pVertexBuffer, data, 0, size);
	}

	if (buffer) *buffer = (size_t)pVertexBuffer;
}

void RSGL_DX11_updateBuffer(RSGL_dx11Renderer* ctx, size_t buffer, const void* data, size_t start, size_t end) {
    D3D11_MAPPED_SUBRESOURCE ms;
    ctx->ctx->pDeviceContext->lpVtbl->Map(ctx->ctx->pDeviceContext, (ID3D11Resource*)buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
    memcpy(ms.pData, &((u8*)data)[start], end - start);
    ctx->ctx->pDeviceContext->lpVtbl->Unmap(ctx->ctx->pDeviceContext, (ID3D11Resource*)buffer, 0);
}

void RSGL_DX11_deleteBuffer(RSGL_dx11Renderer* ctx, size_t buffer) {
	/* TODO */
}

void RSGL_DX11_initPtr(RSGL_dx11Renderer* ctx, RSGL_dx11Context* loader) {
	ctx->ctx = loader;
	const char* shaderString = RSGL_MULTILINE_STR(
		struct VOut
			{
				float4 position : SV_POSITION;
			};

		VOut VS(float3 position : POSITION)
		{
			VOut output;
			output.position = float4(position, 1.0);

			return output;
		}


		float4 PS(VOut input) : SV_TARGET
		{
			return float4(1.0, 1.0, 1.0, 1.0);
		}
	);

	ctx->program = RSGL_DX11_createProgram(ctx, shaderString, shaderString, "vertexPosition", "vertexTexCoord", "vertexColor");
    /* load default texture */
    u8 white[4] = {255, 255, 255, 255};
    ctx->defaultTex = RSGL_DX11_createTexture(ctx, white, RSGL_AREA(1, 1), 4);
}

void RSGL_DX11_freePtr(RSGL_dx11Renderer* ctx) {
	/* TODO */
}

void RSGL_DX11_render(RSGL_dx11Renderer* ctx, RSGL_programInfo program, const RSGL_renderBuffers* buffers) {
	if (program.program == 0) program = ctx->program;

	RSGL_dx11Program* prog = (RSGL_dx11Program*)program.program;

    ctx->ctx->pDeviceContext->lpVtbl->VSSetShader(ctx->ctx->pDeviceContext, prog->pVertexShader, 0, 0);
    ctx->ctx->pDeviceContext->lpVtbl->PSSetShader(ctx->ctx->pDeviceContext, prog->pPixelShader, 0, 0);
    ctx->ctx->pDeviceContext->lpVtbl->IASetInputLayout(ctx->ctx->pDeviceContext, prog->pInputLayout);

	UINT stride = sizeof(float) * 3;
	UINT offset = 0;
	ID3D11Buffer* vertex_buffer = (ID3D11Buffer*)buffers->vertex;
	ctx->ctx->pDeviceContext->lpVtbl->IASetVertexBuffers(ctx->ctx->pDeviceContext, 0, 1, &vertex_buffer, &stride, &offset);

	D3D_PRIMITIVE_TOPOLOGY type = 0;

	u32 i;
	for (i = 0; i < buffers->batchCount; i++) {
		switch (buffers->batches[i].type) {
			case RSGL_LINES: type = D3D_PRIMITIVE_TOPOLOGY_LINELIST; break;
			/*case RSGL_TRIANGLE_FAN: type = D3D_PRIMITIVE_TOPOLOGY_TRIANGLEFAN; break;*/
			case RSGL_TRIANGLES:  type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
			default: type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
		}


		ctx->ctx->pDeviceContext->lpVtbl->IASetPrimitiveTopology(ctx->ctx->pDeviceContext, type);
		ctx->ctx->pDeviceContext->lpVtbl->Draw(ctx->ctx->pDeviceContext, buffers->batches[i].len, buffers->batches[i].start);
	}

}

void RSGL_DX11_scissorStart(RSGL_dx11Renderer* ctx, RSGL_rectF scissor, i32 renderer_height) {
	/* TODO */
}

void RSGL_DX11_scissorEnd(RSGL_dx11Renderer* ctx) {
	/* TODO */
}

/* textures / images */
RSGL_texture RSGL_DX11_createTexture(RSGL_dx11Renderer* ctx, u8* bitmap, RSGL_area memsize, u8 channels) {
    unsigned int id = 0;

	/* TODO */
    return id;
}

void RSGL_DX11_updateTexture(RSGL_dx11Renderer* ctx, RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels) {
	/* TODO */
}


RSGL_programInfo RSGL_DX11_createProgram(RSGL_dx11Renderer* ctx, const char* VShaderCode, const char* FShaderCode, const char* posName, const char* texName, const char* colorName) {
	RSGL_programInfo program;

	/* TODO */
	program.type = RSGL_shaderTypeStandard;

	ID3D10Blob* pVertexShaderBlob = NULL;
    ID3D10Blob* pPixelShaderBlob = NULL;
    ID3D10Blob* pErrorBlob = NULL;

	D3DCompile(VShaderCode, strlen(VShaderCode), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &pVertexShaderBlob, &pErrorBlob);
    D3DCompile(FShaderCode, strlen(FShaderCode), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &pPixelShaderBlob, &pErrorBlob);

	RSGL_dx11Program* prog = RSGL_MALLOC(sizeof(RSGL_dx11Program));
	program.program = (size_t)prog;

    ctx->ctx->pDevice->lpVtbl->CreateVertexShader(ctx->ctx->pDevice, pVertexShaderBlob->lpVtbl->GetBufferPointer(pVertexShaderBlob), pVertexShaderBlob->lpVtbl->GetBufferSize(pVertexShaderBlob), NULL, &prog->pVertexShader);
    ctx->ctx->pDevice->lpVtbl->CreatePixelShader(ctx->ctx->pDevice, pPixelShaderBlob->lpVtbl->GetBufferPointer(pPixelShaderBlob), pPixelShaderBlob->lpVtbl->GetBufferSize(pPixelShaderBlob), NULL, &prog->pPixelShader);

	D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    ctx->ctx->pDevice->lpVtbl->CreateInputLayout(ctx->ctx->pDevice, layout, 1, pVertexShaderBlob->lpVtbl->GetBufferPointer(pVertexShaderBlob), pVertexShaderBlob->lpVtbl->GetBufferSize(pVertexShaderBlob), &prog->pInputLayout);

	return program;
}

void RSGL_DX11_deleteProgram(RSGL_dx11Renderer* ctx, RSGL_programInfo program) {
	/* TODO */
	RSGL_FREE((RSGL_dx11Program*)program.program);
}

void RSGL_DX11_setShaderValue(RSGL_dx11Renderer* ctx, u32 program, const char* var, const float value[], u8 len) {
    switch (len) {
        case 1:  break;
        case 2:  break;
        case 3:  break;
        case 4:  break;
        case 16:  break;
        default: break;
    }

	/* TODO */
}

RSGL_texture RSGL_DX11_create_atlas(RSGL_dx11Renderer* ctx, u32 atlasWidth, u32 atlasHeight) {
	u32 id = 0;
	/* TODO */
	return id;
}

void RSGL_DX11_bitmap_to_atlas(RSGL_dx11Renderer* ctx, RFont_texture atlas, u32 atlasWidth, u32 atlasHeight, u32 maxHeight, u8* bitmap, float w, float h, float* x, float* y) {
	RSGL_UNUSED(ctx); RSGL_UNUSED(atlasHeight);
	if (((*x) + w) >= atlasWidth) {
		*x = 0;
		*y += (float)maxHeight;
	}

	/* TODO */
	*x += w;
}

#ifdef RSGL_USE_COMPUTE

RSGL_programInfo RSGL_DX11_createComputeProgram(RSGL_dx11Renderer* ctx, const char* CShaderCode) {
	RSGL_programInfo program;
	program.type = RSGL_shaderTypeCompute;
	/* TODO */
	return program;
}

void RSGL_DX11_dispatchComputeProgram(RSGL_dx11Renderer* ctx, RSGL_programInfo program, u32 groups_x, u32 groups_y, u32 groups_z) {
	/* TODO */
}


void RSGL_DX11_bindComputeTexture(RSGL_dx11Renderer* ctx, u32 texture, u8 format) {
	u16 c = 0;
   switch (format) {
       case 2: c = GL_RG8; break;
       case 3: c = GL_RGB8; break;
       case 4: c = GL_RGBA8; break;
       default: break;
   }
	/* TODO */
}

#endif

#endif /* RSGL_IMPLEMENTATION */
