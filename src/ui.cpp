#include "ui.h"
#include "imgui.h"

//ガントチャート用
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdio>

// 1つのRenderUI()を実行する


// ウィンドウの表示
void RenderUI()
{
 ImGui::SetNextWindowPos(ImVec2(40, 40), ImGuiCond_Once);
 ImGui::SetNextWindowSize(ImVec2(420, 180), ImGuiCond_Once);
 ImGui::Begin("Window のサンプル");
 ImGui::Text("This is a basic Dear ImGui window.");
 ImGui::Separator();
 ImGui::Text("The window can be moved and resized.");
 ImGui::End();
}


/*
// テキストの色
void RenderUI()
{
 ImGui::Begin("Text Example");
 ImGui::Text("Plain text");
 ImGui::Text("Formatted value: %d", 42);
 ImGui::TextColored(ImVec4(0.4f, 0.9f, 0.4f, 1.0f), "Status: OK");
 ImGui::End();
}
*/

/*
// テキストボックス
void RenderUI()
{
 static char input_name[128] = "Yoshihiko";
 static char memo[256] =
 "This is a multi-line text box.\n"
 "You can paste a longer message here.";
 ImGui::Begin("Text Box Example");
 ImGui::InputText("Name", input_name, IM_ARRAYSIZE(input_name));
 ImGui::Separator();
 ImGui::InputTextMultiline(
 "Memo",
 memo,
 IM_ARRAYSIZE(memo),
 ImVec2(440, 150)
 );
 ImGui::Text("Current name: %s", input_name);
 ImGui::End();
}
*/

/*
// スライダー
void RenderUI()
{
 static int value = 100;
 ImGui::Begin("Slider Example");
 ImGui::SliderInt("Value", &value, 0, 100);
 ImGui::Text("Current value = %d", value);
 ImGui::End();
}
*/

/*
 // ボタン
void RenderUI()
{
 static int count = 0;
 ImGui::Begin("Button Example");
 if (ImGui::Button("Count Up"))
 {
 count++;
 }
 ImGui::SameLine();
 ImGui::Text("Button Count = %d", count);
 ImGui::End();
}
*/

/*
// スクロール
void RenderUI()
{
 ImGui::Begin("Scroll Example");
 ImGui::Text("Scroll Area");
 ImGui::BeginChild("ScrollRegion", ImVec2(0, 180), true);
 for (int i = 0; i < 30; i++)
 {
 ImGui::Text("Scrollable Item %d", i + 1);
 }
 ImGui::EndChild();
 ImGui::End();
}
*/

/*
// メニュー
void RenderUI() {
    // ----------------------------
    // メニュー描画
    // ----------------------------
    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Open");
            ImGui::MenuItem("Save");
            ImGui::MenuItem("Exit");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            ImGui::MenuItem("About");
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
*/


/*
// 色の変更
void RenderUI()
{
 ImGui::Begin("Color Example");
 ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.23f, 0.51f, 0.96f, 1.0f));
 ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.33f, 0.61f, 1.00f, 1.0f));
 ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.16f, 0.42f, 0.86f, 1.0f));
 ImGui::Button("Blue Button", ImVec2(180, 0));
 ImGui::PopStyleColor(3);
 ImGui::End();
}
*/

/*
// ガントチャート
struct SimpleDate
{
    int year;
    int month;
    int day;
};

struct Task
{
    int id;
    std::string name;
    SimpleDate start;
    SimpleDate end;
    float progress;
};

time_t ToTimeT(const SimpleDate& d)
{
    std::tm tm_date = {};
    tm_date.tm_year = d.year - 1900;
    tm_date.tm_mon  = d.month - 1;
    tm_date.tm_mday = d.day;
    tm_date.tm_hour = 0;
    tm_date.tm_min  = 0;
    tm_date.tm_sec  = 0;
    tm_date.tm_isdst = -1;
    return std::mktime(&tm_date);
}

int DaysBetween(const SimpleDate& a, const SimpleDate& b)
{
    time_t ta = ToTimeT(a);
    time_t tb = ToTimeT(b);
    double diff = std::difftime(tb, ta);
    return static_cast<int>(diff / (60 * 60 * 24));
}

SimpleDate AddDays(const SimpleDate& d, int days)
{
    time_t t = ToTimeT(d);
    t += static_cast<time_t>(days) * 24 * 60 * 60;

    std::tm* tm_date = std::localtime(&t);

    SimpleDate result;
    result.year  = tm_date->tm_year + 1900;
    result.month = tm_date->tm_mon + 1;
    result.day   = tm_date->tm_mday;
    return result;
}

void RenderUI()
{
    static std::vector<Task> tasks =
    {
        {1, "Requirements",   {2026, 3, 1},  {2026, 3, 10}, 1.0f},
        {2, "Design",         {2026, 3, 8},  {2026, 3, 25}, 0.7f},
        {3, "Implementation", {2026, 3, 20}, {2026, 4, 20}, 0.4f},
        {4, "Test",           {2026, 4, 15}, {2026, 5, 10}, 0.1f},
        {5, "Release",        {2026, 5, 12}, {2026, 5, 20}, 0.0f}
    };

    // 表示範囲（数か月）
    SimpleDate viewStart = {2026, 3, 1};
    SimpleDate viewEnd   = {2026, 5, 31};

    const float dayWidth = 22.0f;
    const float rowHeight = 32.0f;
    const float nameColumnWidth = 180.0f;
    const float monthHeaderHeight = 28.0f;
    const float dayHeaderHeight = 28.0f;
    const float headerHeight = monthHeaderHeight + dayHeaderHeight;

    int totalDays = DaysBetween(viewStart, viewEnd) + 1;

    ImGui::SetNextWindowPos(ImVec2(30, 30), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(1100, 500), ImGuiCond_Once);

    ImGui::Begin("Gantt Chart (Multi-Month)");

    ImGui::BeginChild("GanttRegion", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 origin = ImGui::GetCursorScreenPos();

    float chartWidth = nameColumnWidth + totalDays * dayWidth;
    float chartHeight = headerHeight + tasks.size() * rowHeight;

    // 背景
    draw_list->AddRectFilled(
        origin,
        ImVec2(origin.x + chartWidth, origin.y + chartHeight),
        IM_COL32(255, 255, 255, 255)
    );

    // 左列背景
    draw_list->AddRectFilled(
        ImVec2(origin.x, origin.y),
        ImVec2(origin.x + nameColumnWidth, origin.y + chartHeight),
        IM_COL32(245, 245, 245, 255)
    );

    // 月ヘッダ背景
    draw_list->AddRectFilled(
        ImVec2(origin.x, origin.y),
        ImVec2(origin.x + chartWidth, origin.y + monthHeaderHeight),
        IM_COL32(225, 230, 240, 255)
    );

    // 日ヘッダ背景
    draw_list->AddRectFilled(
        ImVec2(origin.x, origin.y + monthHeaderHeight),
        ImVec2(origin.x + chartWidth, origin.y + headerHeight),
        IM_COL32(235, 235, 235, 255)
    );

    // 月と日付の描画
    int monthStartDayIndex = 0;
    SimpleDate current = viewStart;

    for (int i = 0; i < totalDays; ++i)
    {
        SimpleDate d = AddDays(viewStart, i);
        float x = origin.x + nameColumnWidth + i * dayWidth;

        // 日ごとの縦線
        draw_list->AddLine(
            ImVec2(x, origin.y + monthHeaderHeight),
            ImVec2(x, origin.y + chartHeight),
            IM_COL32(210, 210, 210, 255)
        );

        // 日付数字
        char dayBuf[8];
        std::snprintf(dayBuf, sizeof(dayBuf), "%d", d.day);
        draw_list->AddText(
            ImVec2(x + 4, origin.y + monthHeaderHeight + 6),
            IM_COL32(0, 0, 0, 255),
            dayBuf
        );

        // 月切り替わり判定
        bool isMonthEnd = false;
        if (i == totalDays - 1)
        {
            isMonthEnd = true;
        }
        else
        {
            SimpleDate next = AddDays(viewStart, i + 1);
            if (next.month != d.month || next.year != d.year)
            {
                isMonthEnd = true;
            }
        }

        if (isMonthEnd)
        {
            float monthX1 = origin.x + nameColumnWidth + monthStartDayIndex * dayWidth;
            float monthX2 = origin.x + nameColumnWidth + (i + 1) * dayWidth;

            char monthBuf[32];
            std::snprintf(monthBuf, sizeof(monthBuf), "%04d/%02d", d.year, d.month);

            draw_list->AddRect(
                ImVec2(monthX1, origin.y),
                ImVec2(monthX2, origin.y + monthHeaderHeight),
                IM_COL32(180, 180, 180, 255)
            );

            draw_list->AddText(
                ImVec2(monthX1 + 6, origin.y + 6),
                IM_COL32(0, 0, 0, 255),
                monthBuf
            );

            monthStartDayIndex = i + 1;
        }
    }

    // 最後の縦線
    draw_list->AddLine(
        ImVec2(origin.x + chartWidth, origin.y + monthHeaderHeight),
        ImVec2(origin.x + chartWidth, origin.y + chartHeight),
        IM_COL32(210, 210, 210, 255)
    );

    // 横線
    for (size_t i = 0; i <= tasks.size(); ++i)
    {
        float y = origin.y + headerHeight + i * rowHeight;
        draw_list->AddLine(
            ImVec2(origin.x, y),
            ImVec2(origin.x + chartWidth, y),
            IM_COL32(210, 210, 210, 255)
        );
    }

    // タスク描画
    for (size_t i = 0; i < tasks.size(); ++i)
    {
        const Task& task = tasks[i];

        int startOffset = DaysBetween(viewStart, task.start);
        int endOffset   = DaysBetween(viewStart, task.end);

        float y = origin.y + headerHeight + i * rowHeight;
        float barX1 = origin.x + nameColumnWidth + startOffset * dayWidth;
        float barX2 = origin.x + nameColumnWidth + (endOffset + 1) * dayWidth;
        float barY1 = y + 6;
        float barY2 = y + rowHeight - 6;

        draw_list->AddText(
            ImVec2(origin.x + 8, y + 8),
            IM_COL32(0, 0, 0, 255),
            task.name.c_str()
        );

        // 期間バー
        draw_list->AddRectFilled(
            ImVec2(barX1, barY1),
            ImVec2(barX2, barY2),
            IM_COL32(120, 170, 255, 255),
            4.0f
        );

        // 進捗
        float progressWidth = (barX2 - barX1) * std::clamp(task.progress, 0.0f, 1.0f);
        draw_list->AddRectFilled(
            ImVec2(barX1, barY1),
            ImVec2(barX1 + progressWidth, barY2),
            IM_COL32(80, 190, 110, 255),
            4.0f
        );

        draw_list->AddRect(
            ImVec2(barX1, barY1),
            ImVec2(barX2, barY2),
            IM_COL32(60, 60, 60, 255),
            4.0f
        );
    }

    // 例として今日線
    SimpleDate today = {2026, 4, 10};
    int todayOffset = DaysBetween(viewStart, today);
    if (todayOffset >= 0 && todayOffset < totalDays)
    {
        float todayX = origin.x + nameColumnWidth + todayOffset * dayWidth;
        draw_list->AddLine(
            ImVec2(todayX, origin.y),
            ImVec2(todayX, origin.y + chartHeight),
            IM_COL32(255, 80, 80, 255),
            2.0f
        );
    }

    ImGui::Dummy(ImVec2(chartWidth, chartHeight));

    ImGui::EndChild();
    ImGui::End();
}
*/
