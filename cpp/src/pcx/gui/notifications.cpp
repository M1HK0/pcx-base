#include "pcx/gui/notifications.hpp"

#include "pcx/gui/config.hpp"
#include "pcx/gui/containers.hpp"
#include "pcx/input.hpp"
#include "pcx/math.hpp"
#include "pcx/renderer.hpp"

#include <chrono>
#include <vector>

namespace notifications {
namespace {

struct CNotification {
    std::string message;
    bool dismissable = false;
    float lifetime = 5.f;
    float elapsed = 0.f;
    float slide_anim = 0.f;
    float fade_anim = 0.f;
    bool closing = false;
    std::chrono::steady_clock::time_point start_time;
    pcx::ENotificationType type = pcx::ENotificationType::NORMAL;
};

std::vector<CNotification> active;
constexpr float padding = 10.f;
constexpr float width = 220.f;
constexpr float min_height = 30.f;

pcx::Color get_accent(const CNotification& n, uint8_t a) {
    switch (n.type) {
    case pcx::ENotificationType::SUCCESS: return {50, 200, 80, a};
    case pcx::ENotificationType::ERROR: return {220, 50, 50, a};
    default: return {gui::colors::accent_primary.r, gui::colors::accent_primary.g, gui::colors::accent_primary.b, a};
    }
}

float get_notification_height(const CNotification& n) {
    const auto text_size = pcx::renderer::text::get_size(n.message, pcx::EFont::TAHOMA);
    float content_w = width - 16.f;
    if (n.dismissable) content_w -= 16.f;
    const int lines = static_cast<int>(text_size.x / content_w) + 1;
    const float text_h = text_size.y * static_cast<float>(lines);
    float h = text_h + 12.f;
    if (h < min_height) h = min_height;
    return h;
}

} // namespace

void create(const std::string& message, float lifetime) {
    CNotification n;
    n.message = message;
    n.lifetime = lifetime;
    n.start_time = std::chrono::steady_clock::now();
    active.push_back(n);
}

void create_success(const std::string& message, float lifetime) {
    CNotification n;
    n.message = message;
    n.lifetime = lifetime;
    n.type = pcx::ENotificationType::SUCCESS;
    n.start_time = std::chrono::steady_clock::now();
    active.push_back(n);
}

void create_error(const std::string& message, float lifetime) {
    CNotification n;
    n.message = message;
    n.lifetime = lifetime;
    n.type = pcx::ENotificationType::ERROR;
    n.start_time = std::chrono::steady_clock::now();
    active.push_back(n);
}

void create_persistent(const std::string& message, pcx::ENotificationType type) {
    CNotification n;
    n.message = message;
    n.dismissable = true;
    n.type = type;
    n.start_time = std::chrono::steady_clock::now();
    active.push_back(n);
}

void update() {
    using clock = std::chrono::steady_clock;
    for (int i = static_cast<int>(active.size()) - 1; i >= 0; --i) {
        auto& n = active[static_cast<size_t>(i)];
        if (!n.closing) n.slide_anim = pcx::lerp(n.slide_anim, 1.f, 0.06f);
        else n.slide_anim = pcx::lerp(n.slide_anim, 0.f, 0.06f);
        if (n.slide_anim > 0.99f) n.slide_anim = 1.f;
        if (n.closing && n.slide_anim < 0.01f) n.slide_anim = 0.f;
        if (!n.closing) {
            n.fade_anim = pcx::lerp(n.fade_anim, 1.f, 0.08f);
            if (n.fade_anim > 0.99f) n.fade_anim = 1.f;
        } else {
            n.fade_anim = pcx::lerp(n.fade_anim, 0.f, 0.08f);
            if (n.fade_anim < 0.01f) n.fade_anim = 0.f;
        }
        if (!n.dismissable && n.lifetime > 0.f) {
            const float secs = std::chrono::duration<float>(clock::now() - n.start_time).count();
            n.elapsed = secs;
            if (n.elapsed >= n.lifetime) n.closing = true;
        }
        if (n.dismissable && !n.closing) {
            const auto vp = CWindow::get_viewport();
            const float h = get_notification_height(n);
            const float x = vp.x - (width + padding) * n.slide_anim;
            float current_y = padding;
            for (int j = 0; j < i; ++j) {
                if (active[static_cast<size_t>(j)].slide_anim > 0.01f)
                    current_y += get_notification_height(active[static_cast<size_t>(j)]) * active[static_cast<size_t>(j)].slide_anim + 6.f;
            }
            const float close_x = x + width - 18.f;
            const float close_y = current_y + 4.f;
            if (pcx::cursor::m1_pressed && pcx::cursor::is_hovered({close_x, close_y}, {14.f, 14.f}))
                n.closing = true;
        }
        if (n.closing && n.slide_anim <= 0.f && n.fade_anim <= 0.f)
            active.erase(active.begin() + i);
    }
}

void render() {
    if (active.empty()) return;
    const auto vp = CWindow::get_viewport();
    float current_y = padding;
    for (const auto& n : active) {
        if (n.slide_anim < 0.01f && n.fade_anim < 0.01f) continue;
        const float h = get_notification_height(n);
        const float animated_h = h * n.slide_anim;
        const float x = vp.x - (width + padding) * n.slide_anim;
        const uint8_t a = static_cast<uint8_t>(255.f * n.fade_anim);
        const auto accent = get_accent(n, a);
        const uint8_t glow_a = static_cast<uint8_t>(gui::colors::glow.a * n.fade_anim);
        pcx::renderer::rect::draw_glow({x, current_y}, {width, animated_h}, pcx::Color{gui::colors::glow.r, gui::colors::glow.g, gui::colors::glow.b, glow_a}, 4.f, 3);
        pcx::renderer::rect::draw_filled_rounded({x, current_y}, {width, animated_h},
            pcx::Color{gui::colors::background_secondary.r, gui::colors::background_secondary.g, gui::colors::background_secondary.b, a}, 4.f);
        pcx::renderer::rect::draw({x + 1.f, current_y + 1.f}, {width - 2.f, animated_h - 2.f},
            pcx::Color{gui::colors::border.r, gui::colors::border.g, gui::colors::border.b, a}, 1.f, 4.f,
            pcx::RR_TOP_LEFT | pcx::RR_TOP_RIGHT | pcx::RR_BOTTOM_LEFT | pcx::RR_BOTTOM_RIGHT);
        pcx::renderer::line::draw({x + 4.f, current_y + 4.f}, {x + 4.f, current_y + animated_h - 4.f}, accent);
        if (n.fade_anim > 0.3f) {
            const float text_x = x + 10.f;
            const float text_y = current_y + (animated_h - pcx::renderer::text::get_size(n.message, pcx::EFont::TAHOMA).y) / 2.f;
            float content_w = width - 18.f;
            if (n.dismissable) content_w -= 16.f;
            pcx::renderer::clip_push(static_cast<int>(x + 6), static_cast<int>(current_y + 2), static_cast<int>(content_w), static_cast<int>(animated_h - 4));
            pcx::renderer::text::draw_shadowed(n.message, {text_x, text_y},
                pcx::Color{gui::colors::text_active.r, gui::colors::text_active.g, gui::colors::text_active.b, a},
                pcx::Color{0, 0, 0, a}, pcx::EFont::TAHOMA);
            pcx::renderer::clip_pop();
            if (n.dismissable) {
                const float close_x = x + width - 18.f;
                const float close_y = current_y + (animated_h - 10.f) / 2.f;
                const bool hover_close = pcx::cursor::is_hovered({close_x, close_y - 2.f}, {14.f, 14.f});
                const auto close_col = hover_close ? accent : pcx::Color{gui::colors::text_inactive.r, gui::colors::text_inactive.g, gui::colors::text_inactive.b, a};
                pcx::renderer::line::draw({close_x, close_y}, {close_x + 10.f, close_y + 10.f}, close_col);
                pcx::renderer::line::draw({close_x + 10.f, close_y}, {close_x, close_y + 10.f}, close_col);
            }
        }
        current_y += animated_h + 8.f;
    }
}

} // namespace notifications
