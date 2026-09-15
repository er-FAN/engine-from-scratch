#pragma once

#include "SimpleSDL.hpp"

namespace simpleimgui {

// Minimal Dear ImGui lifecycle wrapper for a simplesdl::App.
class App {
public:
    App() = default;
    ~App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    // fontPath can be null to keep Dear ImGui's default embedded font.
    bool Init(simplesdl::App& sdlApp, const char* fontPath = nullptr, float fontSize = 18.0f);
    void ProcessEvent(const SDL_Event& event);
    void BeginFrame();
    void Render();
    void Shutdown();

private:
    SDL_Renderer* renderer_ = nullptr;
    bool initialized_ = false;
};

} // namespace simpleimgui
