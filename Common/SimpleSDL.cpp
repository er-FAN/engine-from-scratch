#include "SimpleSDL.hpp"
#include <cstdio>

namespace simplesdl {

App::~App() {
    Shutdown();
}

bool App::Init(const std::string& title, int width, int height) {
    // In SDL3, SDL_Init returns true on success
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return false;
    }

    // SDL_WINDOW_RESIZABLE lets the user resize the window
    window_ = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);
    if (!window_) {
        std::fprintf(stderr, "Window creation error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // In SDL3, the second parameter of SDL_CreateRenderer is the driver name;
    // nullptr means "pick automatically"
    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (!renderer_) {
        std::fprintf(stderr, "Renderer creation error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        SDL_Quit();
        return false;
    }

    // Enable VSync so rendering is synced to the monitor's refresh rate
    // (prevents screen tearing and uneven/jittery motion)
    SDL_SetRenderVSync(renderer_, 1);

    initialized_ = true;
    return true;
}

bool App::PollQuit() {
    SDL_Event event;
    bool quitRequested = false;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            quitRequested = true;
        }
    }
    return quitRequested;
}

void App::Clear(const Color& color) {
    if (!renderer_) return;
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer_);
}

void App::DrawCircle(float centerX, float centerY, float radius, const Color& color) {
    if (!renderer_ || radius <= 0.0f) return;

    // Blending is needed for soft, semi-transparent edges;
    // save the previous blend mode so we can restore it afterward
    SDL_BlendMode previousBlendMode;
    SDL_GetRenderDrawBlendMode(renderer_, &previousBlendMode);
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

    const float thickness = 1.0f; // outline thickness in pixels - smaller means a thinner, more saturated edge
    const float margin    = 1.5f; // extra margin so the full fade of the edge is visible

    // The range of screen pixels we need to check (integer coords since the final output is pixels)
    int minX = static_cast<int>(SDL_floorf(centerX - radius - margin));
    int maxX = static_cast<int>(SDL_ceilf (centerX + radius + margin));
    int minY = static_cast<int>(SDL_floorf(centerY - radius - margin));
    int maxY = static_cast<int>(SDL_ceilf (centerY + radius + margin));

    // Walk over the circle's bounding box and, for each pixel, compute its
    // "coverage" based on its distance from the true radius (using float precision)
    for (int py = minY; py <= maxY; ++py) {
        for (int px = minX; px <= maxX; ++px) {
            float dx = (static_cast<float>(px) + 0.5f) - centerX;
            float dy = (static_cast<float>(py) + 0.5f) - centerY;
            float dist = SDL_sqrtf(dx * dx + dy * dy);
            float diff = SDL_fabsf(dist - radius);

            float coverage = 1.0f - SDL_clamp(diff - thickness * 0.5f, 0.0f, 1.0f);
            if (coverage <= 0.0f) continue;

            Uint8 alpha = static_cast<Uint8>(coverage * static_cast<float>(color.a));
            SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, alpha);
            SDL_RenderPoint(renderer_, static_cast<float>(px), static_cast<float>(py));
        }
    }

    SDL_SetRenderDrawBlendMode(renderer_, previousBlendMode);
}

void App::FillCircle(float centerX, float centerY, float radius, const Color& color) {
    if (!renderer_ || radius <= 0.0f) return;

    SDL_BlendMode previousBlendMode;
    SDL_GetRenderDrawBlendMode(renderer_, &previousBlendMode);

    // Step 1: draw the circle's main body fully opaque, with no blending
    // (exactly like the original solid version), so the interior color is unaffected
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);

    const float edgeBand = 1.0f; // width of the band reserved for anti-aliasing
    int solidMinY = static_cast<int>(SDL_ceilf(centerY - radius));
    int solidMaxY = static_cast<int>(SDL_floorf(centerY + radius));

    for (int py = solidMinY; py <= solidMaxY; ++py) {
        float dy = (static_cast<float>(py) + 0.5f) - centerY;
        float underSqrt = radius * radius - dy * dy;
        if (underSqrt <= 0.0f) continue;

        // Draw slightly narrower than the true radius, leaving room for the AA band in step 2
        float halfWidth = SDL_sqrtf(underSqrt) - edgeBand;
        if (halfWidth <= 0.0f) continue;

        SDL_RenderLine(renderer_,
                        centerX - halfWidth, static_cast<float>(py) + 0.5f,
                        centerX + halfWidth, static_cast<float>(py) + 0.5f);
    }

    // Step 2: only soften the thin edge band with blending (the main body stays untouched)
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

    int minX = static_cast<int>(SDL_floorf(centerX - radius - 1.0f));
    int maxX = static_cast<int>(SDL_ceilf (centerX + radius + 1.0f));
    int minY = static_cast<int>(SDL_floorf(centerY - radius - 1.0f));
    int maxY = static_cast<int>(SDL_ceilf (centerY + radius + 1.0f));

    for (int py = minY; py <= maxY; ++py) {
        for (int px = minX; px <= maxX; ++px) {
            float dx = (static_cast<float>(px) + 0.5f) - centerX;
            float dy = (static_cast<float>(py) + 0.5f) - centerY;
            float dist = SDL_sqrtf(dx * dx + dy * dy);

            // Only check pixels within the edge band
            if (dist < radius - edgeBand) continue; // already drawn fully opaque in step 1
            float coverage = SDL_clamp(radius + 0.5f - dist, 0.0f, 1.0f);
            if (coverage <= 0.0f) continue;

            Uint8 alpha = static_cast<Uint8>(coverage * static_cast<float>(color.a));
            SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, alpha);
            SDL_RenderPoint(renderer_, static_cast<float>(px), static_cast<float>(py));
        }
    }

    SDL_SetRenderDrawBlendMode(renderer_, previousBlendMode);
}

void App::Present() {
    if (!renderer_) return;
    SDL_RenderPresent(renderer_);
}

// ============= Texture =============

TextureId App::LoadTexture(const std::string& path) {
    if (!renderer_) return InvalidTexture;

    SDL_Texture* texture = IMG_LoadTexture(renderer_, path.c_str());
    if (!texture) {
        std::fprintf(stderr, "Failed to load texture (%s): %s\n", path.c_str(), SDL_GetError());
        return InvalidTexture;
    }

    // Needed for transparency support (e.g. the alpha channel in PNGs)
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    TextureId id = nextTextureId_++;
    textures_[id] = texture;
    return id;
}

void App::UnloadTexture(TextureId id) {
    auto it = textures_.find(id);
    if (it == textures_.end()) return;

    SDL_DestroyTexture(it->second);
    textures_.erase(it);
}

void App::DrawTexture(TextureId id, float x, float y, float width, float height) {
    if (!renderer_) return;

    auto it = textures_.find(id);
    if (it == textures_.end()) return;

    SDL_Texture* texture = it->second;

    float texW = 0.0f, texH = 0.0f;
    SDL_GetTextureSize(texture, &texW, &texH);

    SDL_FRect dest;
    dest.x = x;
    dest.y = y;
    dest.w = (width  > 0.0f) ? width  : texW;
    dest.h = (height > 0.0f) ? height : texH;

    SDL_RenderTexture(renderer_, texture, nullptr, &dest);
}

bool App::GetTextureSize(TextureId id, float& outWidth, float& outHeight) const {
    auto it = textures_.find(id);
    if (it == textures_.end()) return false;

    return SDL_GetTextureSize(it->second, &outWidth, &outHeight);
}

void App::Shutdown() {
    // First free all loaded textures (before destroying the renderer)
    for (auto& pair : textures_) {
        SDL_DestroyTexture(pair.second);
    }
    textures_.clear();

    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    if (initialized_) {
        SDL_Quit();
        initialized_ = false;
    }
}

} // namespace simplesdl