#include "platform_dx11.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"

// DirectX11 の各種オブジェクト本体
ID3D11Device*           g_pd3dDevice = nullptr;
ID3D11DeviceContext*    g_pd3dDeviceContext = nullptr;
IDXGISwapChain*         g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// ImGui の Win32 用メッセージハンドラ
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam
);

bool CreateDeviceD3D(HWND hWnd)
{
    // スワップチェーン設定
    // バックバッファの情報や表示方法を決める
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));

    sd.BufferCount = 2; // バッファ数
    sd.BufferDesc.Width = 0; // 0ならウィンドウサイズに合わせる
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 一般的なRGBA形式
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1; // マルチサンプルなし
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE; // ウィンドウモード
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0
    };

    // DirectX11 デバイスとスワップチェーンを作成
    HRESULT res = D3D11CreateDeviceAndSwapChain(
        nullptr,                    // 既定アダプタ
        D3D_DRIVER_TYPE_HARDWARE,   // ハードウェア描画
        nullptr,
        createDeviceFlags,
        featureLevelArray,
        2,
        D3D11_SDK_VERSION,
        &sd,
        &g_pSwapChain,
        &g_pd3dDevice,
        &featureLevel,
        &g_pd3dDeviceContext
    );

    if (res != S_OK)
    {
        return false;
    }

    // 描画先を作成
    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    // RenderTarget を先に解放
    CleanupRenderTarget();

    // 作成した順と逆を意識して解放
    if (g_pSwapChain)
    {
        g_pSwapChain->Release();
        g_pSwapChain = nullptr;
    }

    if (g_pd3dDeviceContext)
    {
        g_pd3dDeviceContext->Release();
        g_pd3dDeviceContext = nullptr;
    }

    if (g_pd3dDevice)
    {
        g_pd3dDevice->Release();
        g_pd3dDevice = nullptr;
    }
}

void CreateRenderTarget()
{
    // スワップチェーンからバックバッファを取得
    ID3D11Texture2D* pBackBuffer = nullptr;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

    // バックバッファを描画先として使えるように RenderTargetView を作成
    g_pd3dDevice->CreateRenderTargetView(
        pBackBuffer,
        nullptr,
        &g_mainRenderTargetView
    );

    // 一時的に取得したバックバッファは解放
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView)
    {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = nullptr;
    }
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // まず ImGui 側にメッセージを渡す
    // ここで ImGui が処理したなら、そのまま return
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;
    }

    switch (msg)
    {
    case WM_SIZE:
        // 最小化中でなければ、ウィンドウサイズ変更時に描画先を作り直す
        if (g_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();

            g_pSwapChain->ResizeBuffers(
                0,
                (UINT)LOWORD(lParam),
                (UINT)HIWORD(lParam),
                DXGI_FORMAT_UNKNOWN,
                0
            );

            CreateRenderTarget();
        }
        return 0;

    case WM_SYSCOMMAND:
        // Alt キーでメニューが出る挙動を抑止
        if ((wParam & 0xfff0) == SC_KEYMENU)
        {
            return 0;
        }
        break;

    case WM_DESTROY:
        // アプリ終了メッセージを投げる
        ::PostQuitMessage(0);
        return 0;
    }

    // それ以外はデフォルト処理
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
