#pragma once
// =====================================================================
// SimpleSDL.hpp
// A small, simple wrapper library around SDL3 for basic tasks:
// initialization, window creation, drawing circles, textures, and
// resource cleanup.
// =====================================================================

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace simplesdl {

// Unique identifier for each loaded texture (0 means invalid)
using TextureId = int;
constexpr TextureId InvalidTexture = 0;

// Simple RGBA color
struct Color {
    Uint8 r = 255, g = 255, b = 255, a = 255;
};

class App {
public:
    App() = default;
    ~App(); // Automatically releases resources (RAII)

    // App is not copyable (since it owns SDL resources)
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    // Initializes SDL and creates the window + renderer
    // Returns false on failure (see the error message via SDL_GetError())
    bool Init(const std::string& title, int width, int height);

    // Checks whether the user requested to close the window (the X button)
    // Also drains other events from the queue so the window doesn't freeze
    bool PollQuit();

    // Clears the screen with a given color
    void Clear(const Color& color = {0, 0, 0, 255});

    // Draws a hollow circle (only the outline)
    void DrawCircle(float centerX, float centerY, float radius,
                     const Color& color = {255, 255, 255, 255});

    // Draws a filled circle
    void FillCircle(float centerX, float centerY, float radius,
                     const Color& color = {255, 255, 255, 255});

    // Presents the final frame to the screen
    void Present();

    // ============= Texture =============

    // Loads an image (PNG, JPG, BMP, ...) from the given path
    // Returns a texture id on success, or InvalidTexture (0) on failure
    TextureId LoadTexture(const std::string& path);

    // Frees a specific texture by its id (optional - Shutdown frees all of them)
    void UnloadTexture(TextureId id);

    // Draws a texture at coordinates x, y
    // If width/height are not given (or negative), the texture's native size is used
    void DrawTexture(TextureId id, float x, float y,
                      float width = -1.0f, float height = -1.0f);

    // Gets the native size of a texture; returns false if the id is invalid
    bool GetTextureSize(TextureId id, float& outWidth, float& outHeight) const;

    // Manually releases resources (optional - also called by the destructor)
    void Shutdown();

    // Direct access in case more advanced SDL features are needed
    SDL_Window* GetWindow() const { return window_; }
    SDL_Renderer* GetRenderer() const { return renderer_; }

private:
    SDL_Window*   window_   = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    bool          initialized_ = false;

    std::unordered_map<TextureId, SDL_Texture*> textures_;
    TextureId nextTextureId_ = 1;
};

} // namespace simplesdl