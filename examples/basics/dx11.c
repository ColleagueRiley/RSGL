#if _MSC_VER
#pragma comment(lib, "uuid")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "windowscodecs.lib")
#endif

#define RSGL_IMPLEMENTATION
#include "RSGL.h"

#ifndef _WIN32
	#include "RSGL_gl.h"
	#define RGFW_OPENGL
#else
	#include "RSGL_dx11.h"
	#define RGFW_DIRECTX
#endif

#define RGFW_INT_DEFINED
#define RGFW_IMPLEMENTATION
#include "examples/deps/RGFW.h"

#ifdef _WIN32
	RSGLDEF int dx11_context_init(RSGL_dx11Context* ctx);
	RSGLDEF int dx11_surface_init(const RSGL_dx11Context* ctx, RGFW_window* win, RSGL_dx11Surface* surf);

	RSGLDEF void dx11_surface_free(RSGL_dx11Surface* surf);
	RSGLDEF void dx11_context_free(RSGL_dx11Context* ctx);
#endif

int main() {
	RGFW_windowFlags flags = RGFW_windowCenter;
#ifndef _WIN32
	flags |= RGFW_windowOpenGL;
	RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
	hints->major = 3;
	hints->minor = 3;
	RGFW_setGlobalHints_OpenGL(hints);
#endif

	RGFW_window* window = RGFW_createWindow("window", 0, 0, 500, 500, flags);

#ifndef _WIN32
	RSGL_renderer* renderer= RSGL_renderer_init(RSGL_GL_rendererProc(), RSGL_AREA(500, 500), (void*)RGFW_getProcAddress_OpenGL);
#else
	RSGL_dx11Context ctx;
	RSGL_dx11Surface surf;

	if (dx11_context_init(&ctx) == 0 || dx11_surface_init(&ctx, window, &surf) == 0) {
        printf("failed to create a directX context\n");
        return 1;
    }

	RSGL_renderer* renderer = RSGL_renderer_init(RSGL_DX11_rendererProc(), RSGL_AREA(500, 500), (void*)&ctx);
	RSGL_renderer_setSurface(renderer, &surf);
#endif

	while (RGFW_window_shouldClose(window) == RGFW_FALSE) {
		RGFW_pollEvents();

		RSGL_renderer_clear(renderer, RSGL_RGB(10, 50, 100));

		RSGL_renderer_setColor(renderer, RSGL_RGB(255, 0, 0));
//		RSGL_drawRect(renderer, RSGL_RECT(200, 200, 200, 200));



		RSGL_renderer_render(renderer);

		#ifndef _WIN32
			RGFW_window_swapBuffers_OpenGL(window);
		#else
			surf.swapchain->lpVtbl->Present(surf.swapchain, 0, 0);
		#endif
	}

	#ifdef _WIN32
		dx11_surface_free(&surf);
		dx11_context_free(&ctx);
	#endif

	RSGL_renderer_free(renderer);
	RGFW_window_close(window);
}

#ifdef _WIN32
int dx11_context_init(RSGL_dx11Context* ctx) {
	RGFW_ASSERT(FAILED(CreateDXGIFactory(&__uuidof(IDXGIFactory), (void**) &ctx->pFactory)) == 0);

	if (FAILED(ctx->pFactory->lpVtbl->EnumAdapters(ctx->pFactory, 0, &ctx->pAdapter))) {
		#ifdef RGFW_DEBUG
			fprintf(stderr, "Failed to enumerate DXGI adapters\n");
		#endif
		ctx->pFactory->lpVtbl->Release(ctx->pFactory);
		return 0;
	}

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	if (FAILED(D3D11CreateDevice(ctx->pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, 0, featureLevels, 1, D3D11_SDK_VERSION, &ctx->pDevice, NULL, &ctx->pDeviceContext))) {
		#ifdef RGFW_DEBUG
			fprintf(stderr, "Failed to create Direct3D device\n");
		#endif
		ctx->pAdapter->lpVtbl->Release(ctx->pAdapter);
		ctx->pFactory->lpVtbl->Release(ctx->pFactory);
		return 0;
	}
	return 1;
}

int dx11_surface_init(const RSGL_dx11Context* ctx, RGFW_window* win, RSGL_dx11Surface* surf) {
	RGFW_window_createSwapChain_DirectX(win, ctx->pFactory, (IUnknown*)ctx->pDevice, &surf->swapchain);

	ID3D11Texture2D* pBackBuffer;
	surf->swapchain->lpVtbl->GetBuffer(surf->swapchain, 0, &__uuidof(ID3D11Texture2D), (LPVOID*) &pBackBuffer);
	ctx->pDevice->lpVtbl->CreateRenderTargetView(ctx->pDevice, (ID3D11Resource*) pBackBuffer, NULL, &surf->renderTargetView);
	pBackBuffer->lpVtbl->Release(pBackBuffer);

	D3D11_TEXTURE2D_DESC depthStencilDesc = { 0 };
	depthStencilDesc.Width = win->w;
	depthStencilDesc.Height = win->h;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D* pDepthStencilTexture = NULL;
	ctx->pDevice->lpVtbl->CreateTexture2D(ctx->pDevice, &depthStencilDesc, NULL, &pDepthStencilTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = { 0 };
	depthStencilViewDesc.Format = depthStencilDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	ctx->pDevice->lpVtbl->CreateDepthStencilView(ctx->pDevice, (ID3D11Resource*) pDepthStencilTexture, &depthStencilViewDesc, &surf->pDepthStencilView);

	pDepthStencilTexture->lpVtbl->Release(pDepthStencilTexture);

	ctx->pDeviceContext->lpVtbl->OMSetRenderTargets(ctx->pDeviceContext, 1, &surf->renderTargetView, surf->pDepthStencilView);

	return 1;
}

void dx11_surface_free(RSGL_dx11Surface* surf) {
    surf->swapchain->lpVtbl->Release(surf->swapchain);
    surf->renderTargetView->lpVtbl->Release(surf->renderTargetView);
    surf->pDepthStencilView->lpVtbl->Release(surf->pDepthStencilView);
}

void dx11_context_free(RSGL_dx11Context* ctx) {
    ctx->pDeviceContext->lpVtbl->Release(ctx->pDeviceContext);
    ctx->pDevice->lpVtbl->Release(ctx->pDevice);
    ctx->pAdapter->lpVtbl->Release(ctx->pAdapter);
    ctx->pFactory->lpVtbl->Release(ctx->pFactory);
}
#endif
