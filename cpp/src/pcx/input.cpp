#include "pcx/input.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <imgui.h>

#include <array>
#include <cstring>
#include <unordered_map>

namespace pcx::input {
namespace {

std::array<bool, 256> prev_down_{};
std::array<bool, 256> fired_{};
std::array<bool, 256> singlepress_{};
std::string recent_input_;
float scroll_delta_ = 0.f;

} // namespace

void begin_frame() {
    fired_.fill(false);
    singlepress_.fill(false);
    recent_input_.clear();
    scroll_delta_ = 0.f;
}

void update() {
    for (int vk = 0; vk < 256; ++vk) {
        const bool down = (GetAsyncKeyState(vk) & 0x8000) != 0;
        if (down && !prev_down_[vk])
            singlepress_[vk] = true;
        prev_down_[vk] = down;
    }

    if (ImGui::GetCurrentContext()) {
        scroll_delta_ = ImGui::GetIO().MouseWheel;
        if (ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard) {
            for (int c = 32; c < 127; ++c) {
                if (singlepress_[c]) {
                    recent_input_.push_back(static_cast<char>(c));
                }
            }
        }
    }

    cursor::update();
}

bool key_down(int vk) {
    if (vk < 0 || vk >= 256) return false;
    return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

bool key_singlepress(int vk) {
    if (vk < 0 || vk >= 256) return false;
    return singlepress_[vk];
}

bool key_fired(int vk) {
    if (vk < 0 || vk >= 256) return false;
    if (key_down(vk)) {
        fired_[vk] = true;
        return true;
    }
    return false;
}

void get_keys_down(std::vector<int>& keys) {
    keys.clear();
    for (int vk = 0; vk < 256; ++vk) {
        if (key_down(vk))
            keys.push_back(vk);
    }
}

std::string get_recent_key_input() {
    return recent_input_;
}

float get_scroll_delta() {
    return scroll_delta_;
}

std::string get_key_name(int vk) {
    switch (vk) {
    case 0x01: return "m1";
    case 0x02: return "m2";
    case 0x04: return "m3";
    case 0x05: return "m4";
    case 0x06: return "m5";
    case 0x08: return "backspace";
    case 0x09: return "tab";
    case 0x0D: return "enter";
    case 0x10: return "shift";
    case 0x11: return "ctrl";
    case 0x12: return "alt";
    case 0x1B: return "escape";
    case 0x20: return "space";
    case 0x21: return "page up";
    case 0x22: return "page down";
    case 0x23: return "end";
    case 0x24: return "home";
    case 0x25: return "left";
    case 0x26: return "up";
    case 0x27: return "right";
    case 0x28: return "down";
    case 0x2D: return "insert";
    case 0x2E: return "delete";
    case 0x70: return "f1";
    case 0x71: return "f2";
    case 0x72: return "f3";
    case 0x73: return "f4";
    case 0x74: return "f5";
    case 0x75: return "f6";
    case 0x76: return "f7";
    case 0x77: return "f8";
    case 0x78: return "f9";
    case 0x79: return "f10";
    case 0x7A: return "f11";
    case 0x7B: return "f12";
    default: break;
    }

    if (vk >= 'A' && vk <= 'Z')
        return std::string(1, static_cast<char>(vk + 32));
    if (vk >= '0' && vk <= '9')
        return std::string(1, static_cast<char>(vk));

    char name[32] = {};
    const UINT scan = MapVirtualKeyA(static_cast<UINT>(vk), MAPVK_VK_TO_VSC);
    LONG lparam = static_cast<LONG>(scan << 16);
    if (GetKeyNameTextA(lparam, name, sizeof(name)) > 0)
        return std::string(name);

    return "0x" + std::to_string(vk);
}

} // namespace pcx::input

namespace pcx::cursor {

void update() {
    if (ImGui::GetCurrentContext()) {
        x = ImGui::GetIO().MousePos.x;
        y = ImGui::GetIO().MousePos.y;
        m1_down = ImGui::GetIO().MouseDown[0];
    } else {
        POINT pt{};
        GetCursorPos(&pt);
        x = static_cast<float>(pt.x);
        y = static_cast<float>(pt.y);
        m1_down = input::key_down(0x01);
    }
    m1_pressed = m1_down && !was_m1_down;
    was_m1_down = m1_down;
}

} // namespace pcx::cursor
