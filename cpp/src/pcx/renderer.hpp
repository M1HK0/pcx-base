#pragma once

#include "pcx/types.hpp"

#include <imgui.h>

#include <vector>

namespace pcx::renderer {

void initialize_fonts();
void begin_frame();
void clip_push(int x, int y, int w, int h);
void clip_pop();

ImFont* get_font(EFont font);

namespace rect {
void draw(Vec2 pos, Vec2 size, Color color, float thickness = 1.f, float rounding = 0.f, uint8_t rounding_flags = 0);
void draw_filled(Vec2 pos, Vec2 size, Color color);
void draw_filled_rounded(Vec2 pos, Vec2 size, Color color, float rounding = 4.f, uint8_t flags = RR_TOP_LEFT | RR_TOP_RIGHT | RR_BOTTOM_LEFT | RR_BOTTOM_RIGHT);
void draw_glow(Vec2 pos, Vec2 size, Color color, float rounding = 4.f, int layers = 6);
void draw_gradient(Vec2 pos, Vec2 size, Color tl, Color tr, Color bl, Color br, float rounding = 0.f);
} // namespace rect

namespace text {
void draw(const std::string& text, Vec2 pos, Color color, EFont font = EFont::TAHOMA);
void draw_outlined(const std::string& text, Vec2 pos, Color color, Color outline_color, EFont font = EFont::TAHOMA);
void draw_shadowed(const std::string& text, Vec2 pos, Color color, Color shadow_color, EFont font = EFont::TAHOMA, float shadow_amount = 1.f);
Vec2 get_size(const std::string& text, EFont font = EFont::TAHOMA);
} // namespace text

namespace line {
void draw(Vec2 start, Vec2 end, Color color, float thickness = 1.f);
} // namespace line

namespace circle {
void draw(Vec2 center, float radius, Color color, float thickness = 1.f, bool filled = false);
} // namespace circle

namespace bitmap {
void draw_cogwheel(Vec2 pos, Vec2 size, Color color);
} // namespace bitmap

} // namespace pcx::renderer
