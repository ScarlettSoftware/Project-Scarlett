using namespace Gui;

void GetWindowPosition(HWND hwnd, int& x, int& y)
{
    RECT rect;
    if (GetWindowRect(hwnd, &rect))
    {
        x = rect.left;
        y = rect.top;
       // width = rect.right - rect.left;
       // height = rect.bottom - rect.top;
    }
    else
    {
        x = y = 0; // width = height = 0;
    }
}

void ResizeWindow(int newX, int newY, int newWidth, int newHeight, bool ChangeMove = false , bool ChangeSize = false)
{
    // Adjust the window size
    int swp = 0;
    
    if (!ChangeMove)
        swp |= SWP_NOMOVE;

    if (!ChangeSize)
        swp |= SWP_NOSIZE;

    SetWindowPos(window, nullptr, newX, newY, newWidth, newHeight, swp);
    WIDTH = newWidth; HEIGHT = newHeight;

    // Update DirectX resources
    if (g_pSwapChain)
    {
        // Release the old render target view
        if (g_pRenderTargetView)
        {
            g_pRenderTargetView->Release();
            g_pRenderTargetView = nullptr;
        }

        // Resize the swap chain
        g_pSwapChain->ResizeBuffers(0, newWidth, newHeight, DXGI_FORMAT_UNKNOWN, 0);

        // Get the new back buffer and create a render target view
        ID3D11Texture2D* pBackBuffer = nullptr;
        HRESULT hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
        if (FAILED(hr))
        {
            // Handle error (optional)
            return;
        }

        hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
        pBackBuffer->Release();
        if (FAILED(hr))
        {
            // Handle error (optional)
            return;
        }

        // Set the new render target view
        g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);

        // Set up the viewport
        D3D11_VIEWPORT vp;
        vp.Width = (FLOAT)newWidth;
        vp.Height = (FLOAT)newHeight;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        g_pImmediateContext->RSSetViewports(1, &vp);
    }
}