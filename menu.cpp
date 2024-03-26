#include "fw_menu.h"   
#include "fw_backend_imgui.h"

enum Tab : int {
    TAB_1,
    TAB_2,
    TAB_3,
    TAB_4
};

struct Options {
    bool menuOpen = true;
    bool checkboxTest = true;
    float sliderTest = 5.5f;
    int sliderIntTest = 5;

    std::vector<int> multiPoint = { 0, 0, 0 };
    int aimPoint = 0;
} options;

void Render() {
    static framework::Backend* backend = new framework::IMGUIBackend(); // use whatever framework you need here
    static framework::Menu menu(backend, { 10, 10, 500, 400 });

    static std::vector<framework::Tab> tabs{ framework::Tab("Tab 1", ""), framework::Tab("Tab 2", ""), framework::Tab("Tab 3", ""), framework::Tab("Tab 4", "") };
    static Tab tabIndex = TAB_1;

    menu.Start("Cool Menu", options.menuOpen);
    menu.AddTabs(tabs, (int*)&tabIndex);

    int iter = 0;
    switch (tabIndex) {
    case TAB_1:

        menu.AddCheckbox(("Checkbox " + std::to_string(iter++)).c_str(), &options.checkboxTest);
        menu.AddCheckbox(("Checkbox " + std::to_string(iter++)).c_str(), &options.checkboxTest);

        menu.AddButton(("Button " + std::to_string(iter++)).c_str(), []() { printf("hi"); });

        menu.AddSlider(("Slider " + std::to_string(iter++)).c_str(), &options.sliderTest, 0, 10);
        menu.AddCombo("Aim Point", { "Head", "Chest", "Toes" }, &options.aimPoint);
        menu.AddCheckbox(("Checkbox " + std::to_string(iter++)).c_str(), &options.checkboxTest);
        menu.AddCheckbox(("Checkbox " + std::to_string(iter++)).c_str(), &options.checkboxTest);
        menu.AddCheckbox(("Checkbox " + std::to_string(iter++)).c_str(), &options.checkboxTest);
        menu.AddCheckbox(("Checkbox " + std::to_string(iter++)).c_str(), &options.checkboxTest);
        menu.AddCheckbox(("Checkbox " + std::to_string(iter++)).c_str(), &options.checkboxTest);
        menu.AddCheckbox(("Checkbox " + std::to_string(iter++)).c_str(), &options.checkboxTest);

        menu.AddMultiCombo("Multi Point", { "Head", "Chest", "Toes" }, &options.multiPoint);
        menu.AddSlider("Int Slider", &options.sliderIntTest, 0, 10);

        break;
    case TAB_2:
        menu.AddText("emptiness 2");
        break;
    case TAB_3:
        menu.AddText("emptiness 3");
        break;
    case TAB_4:
        menu.AddText("emptiness 4");
        break;
    }

    menu.End();
}