#include "Gui.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <string>

#include <wrl.h>
#include <d3d11.h>

#include "functions.hpp"

#include "animation.hpp"

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

static const float clear_color[4] = { float(125 / 255), float(125 / 255), float(125 / 255), 0.f};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter);

long __stdcall WindowProcess(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter) {
    if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
        return true;

    switch (message) {
    case WM_SIZE: {
        if (Gui::g_pd3dDevice != nullptr && wideParameter != SIZE_MINIMIZED) {
            if (Gui::g_pRenderTargetView) { Gui::g_pRenderTargetView->Release(); Gui::g_pRenderTargetView = nullptr; }
            Gui::g_pSwapChain->ResizeBuffers(0, LOWORD(longParameter), HIWORD(longParameter), DXGI_FORMAT_UNKNOWN, 0);
            ID3D11Texture2D* pBackBuffer;
            Gui::g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            Gui::g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &Gui::g_pRenderTargetView);
            pBackBuffer->Release();
            Gui::g_pImmediateContext->OMSetRenderTargets(1, &Gui::g_pRenderTargetView, nullptr);

            D3D11_VIEWPORT vp;
            vp.Width = (FLOAT)LOWORD(longParameter);
            vp.Height = (FLOAT)HIWORD(longParameter);
            vp.MinDepth = 0.0f;
            vp.MaxDepth = 1.0f;
            vp.TopLeftX = 0;
            vp.TopLeftY = 0;
            Gui::g_pImmediateContext->RSSetViewports(1, &vp);
        }
    } return 0;

    case WM_SYSCOMMAND: {
        if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
    } break;

    case WM_DESTROY: {
        PostQuitMessage(0);
    } return 0;

    case WM_LBUTTONDOWN: {
        Gui::position = MAKEPOINTS(longParameter); // set click points
    } return 0;

    case WM_MOUSEMOVE: {
        if (wideParameter == MK_LBUTTON) {
            const auto points = MAKEPOINTS(longParameter);
            auto rect = ::RECT{ };

            GetWindowRect(Gui::window, &rect);

            rect.left += points.x - Gui::position.x;
            rect.top += points.y - Gui::position.y;

            if (Gui::position.x >= 0 && Gui::position.x <= Gui::WIDTH && Gui::position.y >= 0 && Gui::position.y <= 19)
                SetWindowPos(Gui::window, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
        }
    } return 0;
    }

    return DefWindowProc(window, message, wideParameter, longParameter);
}

std::wstring ConvertToWideString(const char* narrowString) {
    int length = MultiByteToWideChar(CP_ACP, 0, narrowString, -1, nullptr, 0);
    std::wstring wideString(length, 0);
    MultiByteToWideChar(CP_ACP, 0, narrowString, -1, &wideString[0], length);
    return wideString;
}

void Gui::CreateHWindow(const char* windowName) noexcept {
    WNDCLASSEXW windowClass;
    windowClass.cbSize = sizeof(WNDCLASSEXW);
    windowClass.style = CS_CLASSDC;
    windowClass.lpfnWndProc = reinterpret_cast<WNDPROC>(WindowProcess);
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandle(nullptr);
    windowClass.hIcon = nullptr;
    windowClass.hCursor = nullptr;
    windowClass.hbrBackground = nullptr;
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = L"class001"; // Use wide-character class name
    windowClass.hIconSm = nullptr;

    if (!RegisterClassExW(&windowClass)) {
        // Handle registration failure
        return;
    }

    std::wstring wideWindowName = ConvertToWideString(windowName);

    window = CreateWindowExW(
        0,
        windowClass.lpszClassName,
        wideWindowName.c_str(), 
        WS_POPUP,
        100,
        100,
        WIDTH,
        HEIGHT,
        nullptr,
        nullptr,
        windowClass.hInstance,
        nullptr
    );

    // Set the transparency color key to black
    SetLayeredWindowAttributes(window, RGB(0, 0, 0), 0, LWA_COLORKEY);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(window, &margins);

    ShowWindow(window, SW_SHOWDEFAULT);
    UpdateWindow(window);

    wnd_prop::screenWidth = GetSystemMetrics(SM_CXSCREEN);
    wnd_prop::screenHeight = GetSystemMetrics(SM_CYSCREEN);
}

void Gui::DestroyHWindow() noexcept {
    DestroyWindow(window);
    UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool Gui::CreateDevice() noexcept {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = WIDTH;
    sd.BufferDesc.Height = HEIGHT;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = window;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };

    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pImmediateContext);
    if (FAILED(hr))
        return false;

    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
    pBackBuffer->Release();
    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);

    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)WIDTH;
    vp.Height = (FLOAT)HEIGHT;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports(1, &vp);

    return true;
}

void Gui::DestroyDevice() noexcept {
    if (g_pImmediateContext) { g_pImmediateContext->ClearState(); }
    if (g_pRenderTargetView) { g_pRenderTargetView->Release(); g_pRenderTargetView = nullptr; }
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pImmediateContext) { g_pImmediateContext->Release(); g_pImmediateContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void Gui::ResetDevice() noexcept {
    if (g_pRenderTargetView) { g_pRenderTargetView->Release(); g_pRenderTargetView = nullptr; }
    g_pSwapChain->ResizeBuffers(0, WIDTH, HEIGHT, DXGI_FORMAT_UNKNOWN, 0);
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
    pBackBuffer->Release();
    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);

    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)WIDTH;
    vp.Height = (FLOAT)HEIGHT;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports(1, &vp);
}

void Gui::CreateImGui() noexcept {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;

    ImGui::StyleColorsDark();

    ImGui::GetStyle().Alpha = 0;

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pImmediateContext);
}

void Gui::DestroyImGui() noexcept {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void Gui::BeginRender() noexcept {
    MSG message;
    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);

        if (message.message == WM_QUIT) {
            isRunning = !isRunning;
            return;
        }
    }

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void Gui::EndRender() noexcept {
    ImGui::Render();
    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, (float*)&clear_color);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    g_pSwapChain->Present(1, 0); // Present with vsync
}



void Gui::Render() noexcept {
    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ (float)WIDTH, (float)HEIGHT });
    ImGui::Begin("Scarlett", &isRunning, ImGuiWindowFlags_NoDecoration);

    static int x = wnd_prop::screenWidth / 2 - WIDTH / 2 - 100;
    static int y = wnd_prop::screenHeight / 2 - HEIGHT / 2 - 100;

    static bool IsMoveDone = false;

    if (!IsMoveDone)
    {
        Animate(x, int(wnd_prop::screenWidth / 2 - WIDTH / 2), &x, 0.1f);

        //IsMoveDone = true;
    }

    Animate(0.f, 1.f, &ImGui::GetStyle().Alpha, 0.2f);

    if(!IsMoveDone)
        ResizeWindow(x, y, WIDTH, HEIGHT, true, false);

    ImGui::End();
}
