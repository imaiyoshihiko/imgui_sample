#include <windows.h>
#include <tchar.h>

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "platform_dx11.h"
#include "ui.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    // ------------------------------------------------------------
    // 1. Win32 のウィンドウクラスを登録
    // ------------------------------------------------------------
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WndProc;              // メッセージ処理関数
    wc.hInstance = hInstance;
    wc.lpszClassName = _T("ImGuiSampleClass");

    ::RegisterClassEx(&wc);

    // ------------------------------------------------------------
    // 2. Win32 ウィンドウを作成
    // ------------------------------------------------------------
    HWND hwnd = ::CreateWindow(
        wc.lpszClassName,
        _T("Dear ImGui + Win32 + DirectX11"),
        WS_OVERLAPPEDWINDOW,
        0, 0,               // 表示位置
        1920, 1080,              // 幅・高さ
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr
    );

    // ------------------------------------------------------------
    // 3. DirectX11 初期化
    // ------------------------------------------------------------
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // ウィンドウ表示
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // ------------------------------------------------------------
    // 4. ImGui 初期化
    // ------------------------------------------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // キーボード操作を有効化
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // スタイル設定
    ImGui::StyleColorsDark();

    // ------------------------------------------------------------
	// 日本語フォントの読み込み
	// ------------------------------------------------------------
	ImFontConfig font_config;
	font_config.MergeMode = false;

	// 日本語に必要な文字範囲を取得
	const ImWchar *ranges = io.Fonts->GetGlyphRangesJapanese();

	// Windows標準フォントのメイリオを読み込む
	io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/meiryo.ttc", 18.0f, &font_config, ranges);

    // プラットフォーム/レンダラ バックエンド初期化
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // ------------------------------------------------------------
    // 5. メインループ
    // ------------------------------------------------------------
    bool done = false;
    while (!done)
    {
        // Win32 メッセージ処理
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                done = true;
            }
        }

        if (done)
        {
            break;
        }

        // ImGui の新しいフレーム開始
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // UI描画 基本ここだけ
        RenderUI();

        // ImGui 描画データ確定
        ImGui::Render();

        // 背景色
        const float clear_color_with_alpha[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 白色

        // 描画先をセット
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);

        // 背景クリア
        g_pd3dDeviceContext->ClearRenderTargetView(
            g_mainRenderTargetView,
            clear_color_with_alpha
        );

        // ImGui の描画実行
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // 画面反映
        g_pSwapChain->Present(1, 0);
    }

    // ------------------------------------------------------------
    // 6. 終了処理
    // ------------------------------------------------------------
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();

    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}
