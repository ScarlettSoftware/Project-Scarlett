#include "Gui/Gui.h"

#include <thread>

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nCmdShow
) 
{
    Gui::CreateHWindow("Loader_gui");
    Gui::CreateDevice();
    Gui::CreateImGui();

    while (Gui::isRunning) {
        Gui::BeginRender();
        Gui::Render();
        Gui::EndRender();

        //std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    Gui::DestroyImGui();
    Gui::DestroyDevice();
    Gui::DestroyHWindow();

    return EXIT_SUCCESS;
}