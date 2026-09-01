#include "SimpleSDL.hpp"
#include <cstdio>

namespace simplesdl {

App::~App() {
    Shutdown();
}

bool App::Init(const std::string& title, int width, int height) {
    // در SDL3، تابع SDL_Init در صورت موفقیت true برمی‌گرداند
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::fprintf(stderr, "خطا در SDL_Init: %s\n", SDL_GetError());
        return false;
    }

    window_ = SDL_CreateWindow(title.c_str(), width, height, 0);
    if (!window_) {
        std::fprintf(stderr, "خطا در ساخت پنجره: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // در SDL3 پارامتر دوم SDL_CreateRenderer نام درایور است، nullptr یعنی انتخاب خودکار
    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (!renderer_) {
        std::fprintf(stderr, "خطا در ساخت رندرر: %s\n", SDL_GetError());
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        SDL_Quit();
        return false;
    }

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

    // برای شفافیت لبه‌ها نیاز به حالت ترکیب رنگ (Blend) داریم؛
    // حالت قبلی رندرر را ذخیره می‌کنیم تا در پایان برگردانیم
    SDL_BlendMode previousBlendMode;
    SDL_GetRenderDrawBlendMode(renderer_, &previousBlendMode);
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

    const float thickness = 1.0f; // ضخامت خط دور دایره (پیکسل) - کمتر یعنی لبه باریک‌تر و رنگ پررنگ‌تر
    const float margin    = 1.5f; // حاشیه اضافه تا فید شدن لبه کامل دیده شود

    // محدوده‌ی پیکسل‌های صفحه که باید بررسی شوند (مختصات صحیح چون خروجی نهایی پیکسل است)
    int minX = static_cast<int>(SDL_floorf(centerX - radius - margin));
    int maxX = static_cast<int>(SDL_ceilf (centerX + radius + margin));
    int minY = static_cast<int>(SDL_floorf(centerY - radius - margin));
    int maxY = static_cast<int>(SDL_ceilf (centerY + radius + margin));

    // روی مربع محاطی دایره پیمایش می‌کنیم و برای هر پیکسل بر اساس
    // فاصله‌اش تا شعاع واقعی (با دقت float)، میزان "پوشش" (coverage) را حساب می‌کنیم
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

    // مرحله ۱: بدنه‌ی اصلی دایره را کاملاً بدون blend و با رنگ خام رسم می‌کنیم
    // (دقیقاً مثل نسخه‌ی قبلی) تا رنگ داخلی هیچ تغییری نکند
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);

    const float edgeBand = 1.0f; // عرض نواری که برای AA کنار گذاشته می‌شود
    int solidMinY = static_cast<int>(SDL_ceilf(centerY - radius));
    int solidMaxY = static_cast<int>(SDL_floorf(centerY + radius));

    for (int py = solidMinY; py <= solidMaxY; ++py) {
        float dy = (static_cast<float>(py) + 0.5f) - centerY;
        float underSqrt = radius * radius - dy * dy;
        if (underSqrt <= 0.0f) continue;

        // کمی کوچک‌تر از شعاع واقعی رسم می‌کنیم تا جا برای نوار AA در مرحله ۲ بماند
        float halfWidth = SDL_sqrtf(underSqrt) - edgeBand;
        if (halfWidth <= 0.0f) continue;

        SDL_RenderLine(renderer_,
                        centerX - halfWidth, static_cast<float>(py) + 0.5f,
                        centerX + halfWidth, static_cast<float>(py) + 0.5f);
    }

    // مرحله ۲: فقط نوار نازک لبه را با blend نرم می‌کنیم (بدنه اصلی دست‌نخورده می‌ماند)
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

            // فقط پیکسل‌هایی که در محدوده نوار لبه هستند را بررسی کن
            if (dist < radius - edgeBand) continue; // قبلا در مرحله ۱ کامل رسم شده
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

// ============= تکسچر (Texture) =============

TextureId App::LoadTexture(const std::string& path) {
    if (!renderer_) return InvalidTexture;

    SDL_Texture* texture = IMG_LoadTexture(renderer_, path.c_str());
    if (!texture) {
        std::fprintf(stderr, "خطا در بارگذاری تکسچر (%s): %s\n", path.c_str(), SDL_GetError());
        return InvalidTexture;
    }

    // برای پشتیبانی از شفافیت (مثلاً کانال آلفا در PNG)
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
    // اول تمام تکسچرهای بارگذاری‌شده را آزاد کن (قبل از نابودی رندرر)
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