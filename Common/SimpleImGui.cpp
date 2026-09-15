#include "SimpleImGui.hpp"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

namespace simpleimgui {

App::~App() {
    Shutdown();
}

bool App::Init(simplesdl::App& sdlApp, const char* fontPath, float fontSize) {
    SDL_Window* window = sdlApp.GetWindow();
    renderer_ = sdlApp.GetRenderer();
    if (!window || !renderer_) {
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    if (fontPath != nullptr && fontPath[0] != '\0') {
        ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath, fontSize);
    }

    if (!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer_)) {
        ImGui::DestroyContext();
        renderer_ = nullptr;
        return false;
    }
    if (!ImGui_ImplSDLRenderer3_Init(renderer_)) {
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        renderer_ = nullptr;
        return false;
    }

    initialized_ = true;
    return true;
}

void App::ProcessEvent(const SDL_Event& event) {
    if (initialized_) {
        ImGui_ImplSDL3_ProcessEvent(&event);
    }
}

void App::BeginFrame() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void App::Render() {
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer_);
}

void App::Shutdown() {
    if (!initialized_) {
        return;
    }

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    renderer_ = nullptr;
    initialized_ = false;
}

} // namespace simpleimgui
