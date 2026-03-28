# imgui_sample
imguiに関する基本操作の調査

# 基本環境
Windows PleiadesのEclipse IDE上で動作。
Pleiadesではインクルードパスとして、imguiとimgui/backendsが必要。
GCC c++ Compilerのダイアレクトを「C++17」にする。
ライブラリは以下を追加する。
<pre>
d3d11
gdi32
d3dcompiler
dwmapi
dxgi
imm32
user32
</pre>

# フォルダ構成
<pre>
imgui_sample/
  ├ src/
  |    ├main.cpp 
  |    ├platform.cpp 
  |    └ ui.cpp 
  └ imgui/
</pre>

imguiには元のリポジトリと同様のファイルをそのまま置く。imgui/backends/にはimgui_impl_dx11.cpp, imgui_impl_win32.cppが必要になる。それ以外はいらない。

# GUIについて
imguiの使い方について、ウィンドウの表示、テキストの色変更、テキストボックス、スライダー、スクロール、ボタン、メニュー、ガントチャートの書き方についてコメントアウトしながら、それぞれのRenderUI()関数を作成した。どれか一つをコメントアウトを外して確認することができる。

# PDFファイルについて
参考記事として、ChatGPTにPDFファイルを作成してもらった。

# ライセンスについて
This project uses Dear ImGui (MIT License).

Copyright (c) 2014-2026 Omar Cornut

See imgui/LICENSE.txt for details.
