#include "fw_backend_imgui.h"
#include "imgui.h"
#include <vector>
#include <map>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

std::map<std::string, ImFont*> fonts;

void framework::IMGUIBackend::DrawPixel(UIPos pos, UIColor color)
{
    // retarded, todo: add it to draw buffer instead
    ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + 1, pos.y + 1), ImColor(color.r, color.g, color.b, color.a));
}

bool framework::IMGUIBackend::Init()
{
    return false;
}

void framework::IMGUIBackend::Shutdown()
{
}

void framework::IMGUIBackend::DrawFilledRect(UIRect rect, UIColor color, float radius)
{
    ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(rect.x, rect.y), ImVec2(rect.x + rect.w, rect.y + rect.h), ImColor(color.r, color.g, color.b, color.a), radius);
}

void framework::IMGUIBackend::DrawRect(UIRect rect, UIColor color, float radius)
{
    ImGui::GetForegroundDrawList()->AddRect(ImVec2(rect.x, rect.y), ImVec2(rect.x + rect.w, rect.y + rect.h), ImColor(color.r, color.g, color.b, color.a), radius);
}

void framework::IMGUIBackend::DrawText(UIPos pos, UIColor color, const char* text, bool centered)
{
    if (centered)
    {
        auto textSize = GetTextSize(text);
        pos.x -= textSize.x / 2;
        pos.y -= textSize.y / 2;
    }
    ImGui::GetForegroundDrawList()->AddText(ImVec2(pos.x, pos.y), ImColor(color.r, color.g, color.b, color.a), text);
}

void framework::IMGUIBackend::DrawLine(UIPos pos1, UIPos pos2, UIColor color)
{
    ImGui::GetForegroundDrawList()->AddLine(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), ImColor(color.r, color.g, color.b, color.a));
}


framework::UIPos framework::IMGUIBackend::GetTextSize(const char* text)
{
    ImVec2 size = ImGui::CalcTextSize(text);
    return UIPos{ size.x, size.y };
}

framework::UIPos framework::IMGUIBackend::GetMousePos()
{
    auto pos = ImGui::GetIO().MousePos;
    return UIPos{ pos.x, pos.y };
}

void framework::IMGUIBackend::PushFont(const char* fontName, float fontSize)
{
    
}

void framework::IMGUIBackend::PopFont()
{

}

bool framework::IMGUIBackend::DoButtonBehavior(UIRect rect, bool* hovered, bool repeat)
{
    *hovered = rect.Contains(GetMousePos());
    if (*hovered)
        return ImGui::IsMouseClicked(0, repeat);
    return false;
}

float framework::IMGUIBackend::GetFrameTime()
{
    return ImGui::GetIO().DeltaTime;
}

bool framework::IMGUIBackend::IsMouseButtonDown()
{
    return ImGui::IsMouseDown(0);
}

bool framework::IMGUIBackend::IsMouseButtonClicked()
{
    return ImGui::IsMouseClicked(0);
}
