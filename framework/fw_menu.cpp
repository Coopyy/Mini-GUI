#include "fw_menu.h"
#include <string>
// this should be independent of renderer

__forceinline float _animate(float value, float target, float frametime, float rate = 0.065f, float rewind_rate = 0.065f)
{
    const auto interval = (1.0f / (target < value ? rewind_rate : rate)) * frametime;
    const auto delta = target - value;
    return value + delta * interval;
}

__forceinline framework::UIColor _lerp(framework::UIColor a, framework::UIColor b, float t)
{
    return framework::UIColor{
        (unsigned char)(a.r + (b.r - a.r) * t),
            (unsigned char)(a.g + (b.g - a.g) * t),
            (unsigned char)(a.b + (b.b - a.b) * t),
            (unsigned char)(a.a + (b.a - a.a) * t)
    };
}

__forceinline float _clamp(float x, float min = 0.0f, float max = 1.0f)
{
    return x < min ? min : x > max ? max : x;
}

/*

CLASS IMPLEMENTATIONS

*/


framework::Menu::Menu(Backend* backend, UIRect menuRect, Style style)
{
    this->backend = backend;
    this->menuRect = menuRect;
    this->style = style;

    End(); // set defaults
}

framework::Menu::~Menu()
{
}

void framework::Menu::Start(const char* windowName, bool state)
{
    globalAlpha = _animate(globalAlpha, state ? 1.0f : 0.0f, backend->GetFrameTime());

    if (globalAlpha <= 0.08f)
        return;

    backend->DrawRect({ menuRect.x - 1, menuRect.y - 1, menuRect.w + 2, menuRect.h + 2 }, GetColor(style.accentColor), style.rounding);
    backend->DrawFilledRect(menuRect, GetColor(30, 32, 30, 245), style.rounding);
    backend->DrawText({ currentRect.x + currentRect.w / 2, currentRect.y }, GetColor(255, 255, 255, 255), windowName, true);
    cursorPos.y += 10;

    inputPaused = currentDropdown.shouldDraw;
}

void framework::Menu::End()
{
    constexpr int padding = 10;

    DrawDropdown();

    this->currentRect = { menuRect.x + padding, menuRect.y + padding, menuRect.w - padding * 2, menuRect.h - padding * 2 };
    this->cursorPos = { 0, 0 };
}

void framework::Menu::SetCursorPos(UIPos pos)
{
    this->cursorPos = pos;
}

void framework::Menu::AddTabs(std::vector<Tab> tabs, int* activeIndex)
{
    float backupX = cursorPos.x;
    float btnWidth = currentRect.w / tabs.size();
    for (size_t i = 0; i < tabs.size(); i++)
    {
        Tab tab = tabs[i];
        auto btnRect = UIRect{ currentRect.x + cursorPos.x, currentRect.y + cursorPos.y, btnWidth, 20 };
        bool hovered = false;
        if (backend->DoButtonBehavior(btnRect, &hovered) && !inputPaused)
            *activeIndex = i;
        unsigned char alpha = 155 + round(100 * Animate(tab.label, hovered || *activeIndex == i));
        backend->DrawFilledRect(btnRect, GetColor(100, 100, 100, alpha), style.rounding);
        backend->DrawText({ btnRect.x + btnRect.w / 2, btnRect.y + btnRect.h / 2 }, GetColor(255, 255, 255, 255), tab.label, true);
        cursorPos.x += btnWidth;
    }
    cursorPos.x = backupX;
    cursorPos.y += 25;
}

void framework::Menu::AddCheckbox(const char* label, bool* value)
{
    auto checkboxRect = UIRect{ currentRect.x + cursorPos.x, currentRect.y + cursorPos.y, currentRect.w, 15 };
    bool hovered = false;
    if (backend->DoButtonBehavior(checkboxRect, &hovered) && !inputPaused)
        *value = !*value;
    checkboxRect.w = 15;
    UIColor color = _lerp(GetColor(100, 100, 100, 255), GetColor(style.accentColor), Animate(label, *value));
    backend->DrawFilledRect(checkboxRect, color, style.rounding);
    backend->DrawText({ checkboxRect.x + checkboxRect.w + 5, checkboxRect.y }, GetColor(255, 255, 255, 255), label, false);
    cursorPos.y += 20;
}

const char* sliderHeld = nullptr;
void framework::Menu::AddSlider(const char* label, float* value, float min, float max)
{
    auto textPos = UIPos{ currentRect.x + cursorPos.x, currentRect.y + cursorPos.y };
    backend->DrawText(textPos, GetColor(255, 255, 255, 255), label, false);
    auto valTextSize = backend->GetTextSize(std::to_string(*value).c_str());
    backend->DrawText({ textPos.x + currentRect.w - valTextSize.x, textPos.y }, GetColor(255, 255, 255, 255), std::to_string(*value).c_str(), false);
    cursorPos.y += backend->GetTextSize(label).y + 2;

    auto sliderRect = UIRect{ currentRect.x + cursorPos.x, currentRect.y + cursorPos.y, currentRect.w, 10 };
    bool hovered = false;
    if (!sliderHeld && backend->DoButtonBehavior(sliderRect, &hovered, true) && !inputPaused)
        sliderHeld = label;
    if (sliderHeld == label)
    {
        *value = _clamp((backend->GetMousePos().x - currentRect.x) / sliderRect.w * max, min, max);
        if (!backend->IsMouseButtonDown())
            sliderHeld = nullptr;
    }

    backend->DrawFilledRect(sliderRect, GetColor(100, 100, 100, 255), style.rounding);
    backend->DrawFilledRect({ sliderRect.x, sliderRect.y, sliderRect.w * Animate(label, (*value / max)), sliderRect.h }, GetColor(style.accentColor), style.rounding);
    cursorPos.y += 15;
}

void framework::Menu::AddSlider(const char* label, int* value, int min, int max)
{
    float fValue = static_cast<float>(*value);
    AddSlider(label, &fValue, min, max);
    *value = static_cast<int>(fValue);
}

void framework::Menu::AddButton(const char* label, void(*callback)(), float margin, const char* id)
{
    if (AddButton(label, margin, id))
        callback();
}

bool framework::Menu::AddButton(const char* label, float margin, const char* id, unsigned char minAlpha)
{
    constexpr float height = 20;
    bool result = false;
    auto btnRect = UIRect{ currentRect.x + cursorPos.x, currentRect.y + cursorPos.y, currentRect.w, height };
    bool hovered = false;
    if (backend->DoButtonBehavior(btnRect, &hovered) && !inputPaused)
        result = true;
    unsigned char alpha = minAlpha + round((255 - minAlpha) * Animate((id ? id : label), hovered));
    backend->DrawFilledRect(btnRect, GetColor(100, 100, 100, alpha), style.rounding);
    //backend->DrawText({ btnRect.x + btnRect.w / 2, btnRect.y + btnRect.h / 2 }, GetColor(255, 255, 255, 255), label, true);
    backend->DrawText({ btnRect.x + 5, btnRect.y + 3 }, GetColor(255, 255, 255, 255), label);
    cursorPos.y += height + margin;
    return result;
}

void framework::Menu::AddText(const char* label)
{
    backend->DrawText({ currentRect.x + cursorPos.x, currentRect.y + cursorPos.y }, GetColor(255, 255, 255, 255), label, false);
    cursorPos.y += backend->GetTextSize(label).y + 2;
}

void framework::Menu::AddGroup(const char* label)
{

}

void framework::Menu::AddCombo(const char* label, std::vector<const char*> items, int* activeIndex)
{
    if (AddButton((std::string(label) + ": " + items[*activeIndex]).c_str(), 5, label))
    {
        currentDropdown.activeIndex = activeIndex;
        currentDropdown.isMulti = false;

        currentDropdown.items = items;
        currentDropdown.shouldDraw = true;
        currentDropdown.rectUnderButton = UIRect{ currentRect.x + cursorPos.x, currentRect.y + cursorPos.y, currentRect.w, 20.f * items.size() };
    }
}

void framework::Menu::AddMultiCombo(const char* label, std::vector<const char*> items, std::vector<int>* activeItems)
{
    std::string activeItemsStr = "None";

    for (size_t i = 0; i < items.size(); i++)
        if ((*activeItems)[i])
            if (activeItemsStr == "None")
                activeItemsStr = items[i];
            else
                activeItemsStr += std::string(", ") + items[i];

    if (AddButton((std::string(label) + ": " + activeItemsStr).c_str(), 5, label))
    {
        currentDropdown.activeItems = activeItems;
        currentDropdown.isMulti = true;

        currentDropdown.items = items;
        currentDropdown.shouldDraw = true;
        currentDropdown.rectUnderButton = UIRect{ currentRect.x + cursorPos.x, currentRect.y + cursorPos.y, currentRect.w, 20.f * items.size() };
    }
}


void framework::Menu::DrawDropdown()
{
    currentDropdown.alpha = Animate("dropdown__", currentDropdown.shouldDraw);

    if (currentDropdown.alpha <= 0.08f)
        return;

    float backupAlpha = globalAlpha;
    globalAlpha = currentDropdown.alpha;

    if (currentDropdown.shouldDraw && !currentDropdown.shouldCheckInput)
        currentDropdown.shouldCheckInput = true;
    else if (currentDropdown.shouldDraw && backend->IsMouseButtonClicked() &&
        !currentDropdown.rectUnderButton.Contains(backend->GetMousePos()))
        currentDropdown.ResetState();


    currentRect = currentDropdown.rectUnderButton;
    cursorPos = { 0, 0 };

    currentRect.h += currentDropdown.isMulti ? 5 : 10;

    backend->DrawFilledRect(currentRect, GetColor(30, 32, 30, 255));
    backend->DrawRect({ currentRect.x - 1, currentRect.y - 1, currentRect.w + 2, currentRect.h + 2 }, GetColor(style.accentColor), style.rounding);

    currentRect.Pad(5);

    inputPaused = false;
    for (size_t i = 0; i < currentDropdown.items.size(); i++)
    {
        if (currentDropdown.isMulti)
        {
            AddCheckbox(currentDropdown.items[i], (bool*)&(*currentDropdown.activeItems)[i]);
        }
        else if (AddButton(currentDropdown.items[i], 0.0f, nullptr, *currentDropdown.activeIndex == i ? 100 : 0))
        {
            *currentDropdown.activeIndex = static_cast<int>(i);
            currentDropdown.ResetState();
        }
    }
    inputPaused = true;

    globalAlpha = backupAlpha;
}


float framework::Menu::Animate(const char* labelId, float target)
{
    float& value = animMap[labelId];
    value = _animate(value, target, backend->GetFrameTime());
    return value;
}

framework::UIColor framework::Menu::GetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return { r, g, b, (unsigned char)(globalAlpha * a) };
}

framework::UIColor framework::Menu::GetColor(UIColor color)
{
    return GetColor(color.r, color.g, color.b, color.a);
}
