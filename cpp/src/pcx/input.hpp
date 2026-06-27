#pragma once

#include "pcx/types.hpp"

#include <string>
#include <vector>

namespace pcx::input {

void begin_frame();
void update();

bool key_down(int vk);
bool key_singlepress(int vk);
bool key_fired(int vk);
void get_keys_down(std::vector<int>& keys);
std::string get_recent_key_input();
float get_scroll_delta();
std::string get_key_name(int vk);

} // namespace pcx::input

namespace pcx::cursor {

inline float x = 0.f;
inline float y = 0.f;
inline bool m1_down = false;
inline bool m1_pressed = false;
inline bool was_m1_down = false;

void update();

inline Vec2 pos() { return {x, y}; }

inline bool is_hovered(Vec2 position, Vec2 size) {
    return x >= position.x && x <= position.x + size.x &&
           y >= position.y && y <= position.y + size.y;
}

} // namespace pcx::cursor
