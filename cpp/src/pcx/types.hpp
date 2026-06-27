#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace pcx {

struct Vec2 {
    float x = 0.f;
    float y = 0.f;

    Vec2() = default;
    Vec2(float x_, float y_) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2& o) const { return {x + o.x, y + o.y}; }
    Vec2 operator-(const Vec2& o) const { return {x - o.x, y - o.y}; }
};

struct Color {
    uint8_t r = 255, g = 255, b = 255, a = 255;

    Color() = default;
    Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}

    Color scaled(uint8_t menu_alpha) const {
        return Color(r, g, b, static_cast<uint8_t>((a * menu_alpha) / 255));
    }

    uint32_t to_u32() const {
        return (static_cast<uint32_t>(a) << 24) |
               (static_cast<uint32_t>(b) << 16) |
               (static_cast<uint32_t>(g) << 8) |
               static_cast<uint32_t>(r);
    }

    Color with_alpha(uint8_t alpha) const {
        return Color(r, g, b, alpha);
    }
};

enum class EFont {
    TAHOMA,
    TAHOMABD,
    COUNT
};

enum class EKeybindMode {
    ALWAYS,
    HOLD,
    TOGGLE
};

enum class EElementType {
    CHECKBOX,
    SLIDER_INT,
    SLIDER_FLOAT,
    KEYBIND,
    DROPDOWN,
    MULTISELECT,
    TEXT_INPUT,
    BUTTON,
    COLORPICKER,
    COGWHEEL,
    LISTBOX,
    TOOLTIP,
    LABEL,
    ELEMENT_COUNT
};

enum class ENotificationType {
    NORMAL,
    SUCCESS,
    ERROR
};

enum class EUserRole {
    USER,
    BETA,
    DEVELOPER
};

constexpr uint8_t RR_TOP_LEFT = 1;
constexpr uint8_t RR_TOP_RIGHT = 2;
constexpr uint8_t RR_BOTTOM_LEFT = 4;
constexpr uint8_t RR_BOTTOM_RIGHT = 8;

using VisibilityFn = std::function<bool()>;
using ButtonFn = std::function<void()>;

} // namespace pcx
