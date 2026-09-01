#pragma once
// =====================================================================
// SimpleSDL.hpp
// یک لایبرری کوچک و ساده روی SDL3 برای کارهای ابتدایی:
// راه‌اندازی، ساخت پنجره، کشیدن دایره، تکسچر، و آزادسازی منابع.
// =====================================================================

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace simplesdl {

// شناسه‌ی یکتا برای هر تکسچر بارگذاری‌شده (۰ یعنی نامعتبر)
using TextureId = int;
constexpr TextureId InvalidTexture = 0;

// رنگ ساده RGBA
struct Color {
    Uint8 r = 255, g = 255, b = 255, a = 255;
};

class App {
public:
    App() = default;
    ~App(); // خودکار منابع را آزاد می‌کند (RAII)

    // App قابل کپی نیست (چون مالک منابع SDL است)
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    // راه‌اندازی SDL و ساخت پنجره + رندرر
    // در صورت خطا false برمی‌گرداند (پیام خطا را با SDL_GetError() ببینید)
    bool Init(const std::string& title, int width, int height);

    // بررسی می‌کند که آیا کاربر درخواست بستن پنجره (X) را داده یا نه
    // همچنین رویدادهای دیگر را از صف خارج می‌کند تا پنجره فریز نشود
    bool PollQuit();

    // پاک کردن صفحه با یک رنگ مشخص
    void Clear(const Color& color = {0, 0, 0, 255});

    // کشیدن یک دایره توخالی (فقط خطوط دور دایره)
    void DrawCircle(float centerX, float centerY, float radius,
                     const Color& color = {255, 255, 255, 255});

    // کشیدن یک دایره پر (تو پر)
    void FillCircle(float centerX, float centerY, float radius,
                     const Color& color = {255, 255, 255, 255});

    // نمایش نهایی فریم روی صفحه
    void Present();

    // ============= تکسچر (Texture) =============

    // بارگذاری یک عکس (PNG, JPG, BMP, ...) از مسیر داده‌شده
    // در صورت موفقیت یک شناسه (id) برمی‌گرداند، در صورت خطا InvalidTexture (0)
    TextureId LoadTexture(const std::string& path);

    // آزادسازی یک تکسچر خاص با شناسه‌اش (اختیاری - Shutdown همه را آزاد می‌کند)
    void UnloadTexture(TextureId id);

    // رسم یک تکسچر در مختصات x,y
    // اگر width/height داده نشود (یا منفی باشد)، اندازه‌ی اصلی خود تکسچر استفاده می‌شود
    void DrawTexture(TextureId id, float x, float y,
                      float width = -1.0f, float height = -1.0f);

    // گرفتن اندازه‌ی اصلی یک تکسچر؛ در صورت نامعتبر بودن id مقدار false برمی‌گرداند
    bool GetTextureSize(TextureId id, float& outWidth, float& outHeight) const;

    // آزادسازی دستی منابع (اختیاری - در مخرب هم صدا زده می‌شود)
    void Shutdown();

    // دسترسی مستقیم در صورت نیاز به قابلیت‌های پیشرفته‌تر SDL
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