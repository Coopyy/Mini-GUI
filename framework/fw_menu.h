#pragma once
#include "fw_backend.h"
#include <vector>
#include <map>

namespace framework
{
    struct Tab
    {
        const char* label;
        const char* icon;
        Tab(const char* label, const char* icon) : label(label), icon(icon) {}
    };

    struct Style
    {
        float rounding = 0;
        UIColor accentColor = { 30, 175, 255, 255 };
    };

    class Menu
    {
        Backend* backend;
        UIPos cursorPos;
        UIRect menuRect;
        UIRect currentRect;

        Style style;

        std::map<const char*, float> animMap;
        float globalAlpha = 1.0f;
        bool inputPaused;

        struct DropdownData
        {
            bool shouldDraw = false;
            bool shouldCheckInput = false;
            bool isMulti = false;

            float alpha = 0.0f;

            framework::UIRect rectUnderButton;
            const char* label = "";
            std::vector<const char*> items{};
            int* activeIndex = nullptr;
            std::vector<int>* activeItems{};

            inline void ResetState()
            {
                shouldCheckInput = false;
                shouldDraw = false;
            }
        } currentDropdown;

        void DrawDropdown();
    public:
        Menu(Backend* backend, UIRect menuRect, Style style = Style{});
        ~Menu();

        void Start(const char* windowName, bool state);
        void SetCursorPos(UIPos pos);
        void End();

        void AddTabs(std::vector<Tab> tabs, int* activeIndex);
        void AddCheckbox(const char* label, bool* value);
        void AddSlider(const char* label, float* value, float min, float max);
        void AddSlider(const char* label, int* value, int min, int max);
        void AddButton(const char* label, void (*callback)(), float margin = 5, const char* id = nullptr);
        bool AddButton(const char* label, float margin = 5, const char* id = nullptr, unsigned char minFloat = 155);
        void AddText(const char* label);
        void AddGroup(const char* label);
        void AddCombo(const char* label, std::vector<const char*> items, int* activeIndex);
        void AddMultiCombo(const char* label, std::vector<const char*> items, std::vector<int>* activeItems);

        float Animate(const char* labelId, float target);

        UIColor GetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
        UIColor GetColor(UIColor color);

        inline Style& GetStyle() { return style; }
    };
}
