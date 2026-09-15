#include "ComponentInspector.hpp"
#include "ComponentManager.hpp"
#include "SimpleImGui.hpp"
#include "SimpleSDL.hpp"

#include <array>

// CMake supplies the Inter path for the executable. The fallback keeps this
// source analyzable when it is opened outside a configured CMake build.
#ifndef COMPONENT_INSPECTOR_INTER_FONT_PATH
#define COMPONENT_INSPECTOR_INTER_FONT_PATH nullptr
#endif

namespace
{
    void PopulateDemo(step_0002::ComponentManager &manager)
    {
        manager.RegisterComponent(1, sizeof(float) * 3, "Position (float x, y, z)");
        manager.RegisterComponent(2, sizeof(std::int32_t), "Health (int32)");
        manager.RegisterComponent(3, 16, "Tag (16-byte ASCII)");

        for (int entity = 0; entity < 4; ++entity)
            manager.EntityCreated();

        const std::array<float, 3> position{ 12.5f, -3.0f, 42.0f };
        const std::int32_t health = 100;
        const std::array<char, 16> tag{"player"};
        manager.AddComponent(0, 1, position.data());
        manager.AddComponent(0, 2, &health);
        manager.AddComponent(0, 3, tag.data());
    }
}

int main()
{
    simplesdl::App window;
    if (!window.Init("Component Inspector", 1440, 900))
    {
        return 1;
    }

    simpleimgui::App gui;
    if (!gui.Init(window, COMPONENT_INSPECTOR_INTER_FONT_PATH, 18.0f))
    {
        return 1;
    }

    step_0002::ComponentManager manager;
    PopulateDemo(manager);
    step_0002::ComponentInspector inspector;
    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (window.PollEvent(event))
        {
            gui.ProcessEvent(event);
            if (event.type == SDL_EVENT_QUIT ||
                (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window.GetWindow())))
                running = false;
        }

        gui.BeginFrame();
        inspector.Draw(manager);
        window.Clear({24, 24, 28, 255});
        gui.Render();
        window.Present();
    }

    return 0;
}
