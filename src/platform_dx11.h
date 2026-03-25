#ifndef PLATFORM_DX11_H
#define PLATFORM_DX11_H

#include <windows.h>
#include <d3d11.h>

// DirectX11 のグローバルリソースを extern 宣言している。
// 実体は platform_dx11.cpp 側で定義する。
extern ID3D11Device*           g_pd3dDevice;
extern ID3D11DeviceContext*    g_pd3dDeviceContext;
extern IDXGISwapChain*         g_pSwapChain;
extern ID3D11RenderTargetView* g_mainRenderTargetView;

// DirectX11 の初期化
// ウィンドウに対して描画できるようにする。
bool CreateDeviceD3D(HWND hWnd);

// DirectX11 の後始末
void CleanupDeviceD3D();

// 描画先(RenderTarget)の作成
void CreateRenderTarget();

// 描画先(RenderTarget)の解放
void CleanupRenderTarget();

// Win32 のメッセージ処理関数
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // PLATFORM_DX11_H
