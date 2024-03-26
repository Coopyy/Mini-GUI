#pragma once
#undef DrawText

namespace framework
{
    struct UIPos
    {
        float x, y;

        // add and subtract operators
        inline UIPos operator+(UIPos pos)
        {
            return { x + pos.x, y + pos.y };
        }

        inline UIPos operator-(UIPos pos)
        {
            return { x - pos.x, y - pos.y };
        }
    };

    struct UIRect
    {
        float x, y, w, h;

        inline bool Contains(UIPos pos)
        {
            return pos.x >= x && pos.x <= x + w && pos.y >= y && pos.y <= y + h;
        }

        // add and subtract operators
        inline UIRect operator+(UIPos pos)
        {
            return { x + pos.x, y + pos.y, w, h };
        }

        inline UIRect operator-(UIPos pos)
        {
            return { x - pos.x, y - pos.y, w, h };
        }

        inline UIPos Pos()
        {
            return { x, y };
        }

        inline void Pad(float padding)
        {
            x += padding;
            y += padding;
            w -= padding * 2;
            h -= padding * 2;
        }

        inline UIRect Padded(float padding)
        {
            UIRect rect = *this;
            rect.Pad(padding);
            return rect;
        }
    };

    struct UIColor
    { // 0-255
        unsigned char r, g, b, a;
    };

    class Backend
    {
    public:
        virtual bool Init() = 0;
        virtual void Shutdown() = 0;

        virtual void DrawRect(UIRect rect, UIColor color, float radius = 0) = 0;
        virtual void DrawFilledRect(UIRect rect, UIColor color, float radius = 0) = 0;
        virtual void DrawText(UIPos pos, UIColor color, const char* text, bool centered = false) = 0;
        virtual void DrawLine(UIPos pos1, UIPos pos2, UIColor color) = 0;
        virtual void DrawPixel(UIPos pos, UIColor color) = 0;

        virtual UIPos GetTextSize(const char* text) = 0;

        virtual UIPos GetMousePos() = 0;

        virtual bool DoButtonBehavior(UIRect rect, bool* hovered, bool repeat = false) = 0;
        virtual bool IsMouseButtonDown() = 0;
        virtual bool IsMouseButtonClicked() = 0;

        virtual void PushFont(const char* fontName, float fontSize) = 0;
        virtual void PopFont() = 0;

        virtual float GetFrameTime() = 0;
    };
}
