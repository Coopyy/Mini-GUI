#pragma once
#include "fw_backend.h"

namespace framework
{
    class IMGUIBackend : public Backend
    {
    public:
        // Inherited via Backend
        virtual bool Init() override;
        virtual void Shutdown() override;
        virtual void DrawFilledRect(UIRect rect, UIColor color, float radius = 10) override;
        virtual void DrawRect(UIRect rect, UIColor color, float radius = 10) override;
        virtual void DrawText(UIPos pos, UIColor color, const char* text, bool centered = false) override;
        virtual void DrawLine(UIPos pos1, UIPos pos2, UIColor color) override;
        virtual void DrawPixel(UIPos pos, UIColor color) override;
        virtual UIPos GetTextSize(const char* text) override;
        virtual UIPos GetMousePos() override;
        virtual void PushFont(const char* fontName, float fontSize) override;
        virtual void PopFont() override;
        virtual bool DoButtonBehavior(UIRect rect, bool* hovered, bool repeat = false) override;
        virtual float GetFrameTime() override;
        virtual bool IsMouseButtonDown() override;
        virtual bool IsMouseButtonClicked() override;
    };
}

