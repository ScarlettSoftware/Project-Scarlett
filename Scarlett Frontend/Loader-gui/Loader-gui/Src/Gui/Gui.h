#pragma once
#include <d3d11.h>

namespace wnd_prop {

	inline int screenWidth;
	inline int screenHeight;

}

namespace Gui {

	inline ID3D11Device* g_pd3dDevice = nullptr;
	inline ID3D11DeviceContext* g_pImmediateContext = nullptr;
	inline IDXGISwapChain* g_pSwapChain = nullptr;
	inline ID3D11RenderTargetView* g_pRenderTargetView = nullptr;

	inline int X = 0;
	inline int Y = 0;

	// Window Size
	inline int WIDTH = 450;
	inline int HEIGHT = 350;

	inline bool isRunning = true;

	// Winapi window vars
	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = { };

	// Points for window movement
	inline POINTS position = { };

	// Handle window creation & destruction
	void CreateHWindow(const char* windowName) noexcept;
	void DestroyHWindow() noexcept;

	// Handle device creation & destruction
	bool CreateDevice() noexcept;
	void ResetDevice() noexcept;
	void DestroyDevice() noexcept;

	// Handle ImGui creation & destruction
	void CreateImGui() noexcept;
	void DestroyImGui() noexcept;

	void BeginRender() noexcept;
	void EndRender() noexcept;
	void Render() noexcept;
}