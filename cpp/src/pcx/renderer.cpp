#include "pcx/renderer.hpp"

#include "pcx/math.hpp"

#include <imgui_internal.h>

#include <array>
#include <cmath>
#include <vector>

namespace pcx::renderer {
namespace {

ImDrawList* draw_list() {
    return ImGui::GetBackgroundDrawList();
}

ImU32 to_col(Color c) {
    return IM_COL32(c.r, c.g, c.b, c.a);
}

ImDrawFlags round_flags(uint8_t flags) {
    ImDrawFlags result = ImDrawFlags_None;
    if (flags & RR_TOP_LEFT) result |= ImDrawFlags_RoundCornersTopLeft;
    if (flags & RR_TOP_RIGHT) result |= ImDrawFlags_RoundCornersTopRight;
    if (flags & RR_BOTTOM_LEFT) result |= ImDrawFlags_RoundCornersBottomLeft;
    if (flags & RR_BOTTOM_RIGHT) result |= ImDrawFlags_RoundCornersBottomRight;
    if (result == ImDrawFlags_None)
        result = ImDrawFlags_RoundCornersAll;
    return result;
}

std::array<ImFont*, static_cast<size_t>(EFont::COUNT)> fonts_{};

struct ClipEntry {
    ImVec2 min;
    ImVec2 max;
    bool intersect;
};

std::vector<ClipEntry> clip_stack_;

} // namespace

void initialize_fonts() {
    ImGuiIO& io = ImGui::GetIO();
    const char* tahoma = "C:\\Windows\\Fonts\\tahoma.ttf";
    const char* tahoma_bd = "C:\\Windows\\Fonts\\tahomabd.ttf";

    fonts_[static_cast<size_t>(EFont::TAHOMA)] =
        io.Fonts->AddFontFromFileTTF(tahoma, 13.f);
    fonts_[static_cast<size_t>(EFont::TAHOMABD)] =
        io.Fonts->AddFontFromFileTTF(tahoma_bd, 13.f);

    if (!fonts_[static_cast<size_t>(EFont::TAHOMA)])
        fonts_[static_cast<size_t>(EFont::TAHOMA)] = io.Fonts->AddFontDefault();
    if (!fonts_[static_cast<size_t>(EFont::TAHOMABD)])
        fonts_[static_cast<size_t>(EFont::TAHOMABD)] = fonts_[static_cast<size_t>(EFont::TAHOMA)];
}

void begin_frame() {
    clip_stack_.clear();
}

void clip_push(int x, int y, int w, int h) {
    ImDrawList* dl = draw_list();
    const ImVec2 min(static_cast<float>(x), static_cast<float>(y));
    const ImVec2 max(static_cast<float>(x + w), static_cast<float>(y + h));
    dl->PushClipRect(min, max, true);
    clip_stack_.push_back({min, max, true});
}

void clip_pop() {
    if (clip_stack_.empty()) return;
    draw_list()->PopClipRect();
    clip_stack_.pop_back();
}

ImFont* get_font(EFont font) {
    return fonts_[static_cast<size_t>(font)];
}

namespace rect {

void draw(Vec2 pos, Vec2 size, Color color, float thickness, float rounding, uint8_t rounding_flags) {
    draw_list()->AddRect(
        ImVec2(pos.x, pos.y),
        ImVec2(pos.x + size.x, pos.y + size.y),
        to_col(color), rounding, round_flags(rounding_flags), thickness);
}

void draw_filled(Vec2 pos, Vec2 size, Color color) {
    draw_list()->AddRectFilled(
        ImVec2(pos.x, pos.y),
        ImVec2(pos.x + size.x, pos.y + size.y),
        to_col(color));
}

void draw_filled_rounded(Vec2 pos, Vec2 size, Color color, float rounding, uint8_t flags) {
    draw_list()->AddRectFilled(
        ImVec2(pos.x, pos.y),
        ImVec2(pos.x + size.x, pos.y + size.y),
        to_col(color), rounding, round_flags(flags));
}

void draw_glow(Vec2 pos, Vec2 size, Color color, float rounding, int layers) {
    for (int i = layers; i >= 1; --i) {
        const float expand = static_cast<float>(i) * 2.f;
        const uint8_t a = static_cast<uint8_t>(static_cast<float>(color.a) / static_cast<float>(i * 2));
        draw_filled_rounded(
            {pos.x - expand, pos.y - expand},
            {size.x + expand * 2.f, size.y + expand * 2.f},
            color.with_alpha(a),
            rounding + expand);
    }
}

void draw_gradient(Vec2 pos, Vec2 size, Color tl, Color tr, Color bl, Color br, float rounding) {
    if (rounding <= 0.f) {
        draw_list()->AddRectFilledMultiColor(
            ImVec2(pos.x, pos.y),
            ImVec2(pos.x + size.x, pos.y + size.y),
            to_col(tl), to_col(tr), to_col(br), to_col(bl));
        return;
    }

    draw_filled_rounded(pos, size, tl, rounding);
    draw_list()->AddRectFilledMultiColor(
        ImVec2(pos.x, pos.y),
        ImVec2(pos.x + size.x, pos.y + size.y),
        to_col(tl), to_col(tr), to_col(br), to_col(bl));
}

} // namespace rect

namespace text {

void draw(const std::string& text, Vec2 pos, Color color, EFont font) {
    ImFont* f = get_font(font);
    if (!f) return;
    draw_list()->AddText(f, f->FontSize, ImVec2(pos.x, pos.y), to_col(color), text.c_str());
}

void draw_outlined(const std::string& text, Vec2 pos, Color color, Color outline_color, EFont font) {
    outline_color.a = static_cast<uint8_t>(color.a / 2);
    const ImVec2 p(pos.x, pos.y);
    ImFont* f = get_font(font);
    if (!f) return;
    const ImU32 oc = to_col(outline_color);
    const ImU32 fc = to_col(color);
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            draw_list()->AddText(f, f->FontSize, ImVec2(p.x + dx, p.y + dy), oc, text.c_str());
        }
    }
    draw_list()->AddText(f, f->FontSize, p, fc, text.c_str());
}

void draw_shadowed(const std::string& text, Vec2 pos, Color color, Color shadow_color, EFont font, float shadow_amount) {
    shadow_color.a = static_cast<uint8_t>(color.a / 2);
    ImFont* f = get_font(font);
    if (!f) return;
    const float offset = shadow_amount;
    draw_list()->AddText(f, f->FontSize, ImVec2(pos.x + offset, pos.y + offset), to_col(shadow_color), text.c_str());
    draw_list()->AddText(f, f->FontSize, ImVec2(pos.x, pos.y), to_col(color), text.c_str());
}

Vec2 get_size(const std::string& text, EFont font) {
    ImFont* f = get_font(font);
    if (!f) return {};
    const ImVec2 sz = f->CalcTextSizeA(f->FontSize, FLT_MAX, 0.f, text.c_str());
    return {sz.x, sz.y};
}

} // namespace text

namespace line {

void draw(Vec2 start, Vec2 end, Color color, float thickness) {
    draw_list()->AddLine(
        ImVec2(start.x, start.y),
        ImVec2(end.x, end.y),
        to_col(color), thickness);
}

} // namespace line

namespace circle {

void draw(Vec2 center, float radius, Color color, float thickness, bool filled) {
    if (filled)
        draw_list()->AddCircleFilled(ImVec2(center.x, center.y), radius, to_col(color));
    else
        draw_list()->AddCircle(ImVec2(center.x, center.y), radius, to_col(color), 0, thickness);
}

} // namespace circle

namespace bitmap {

void draw_cogwheel(Vec2 pos, Vec2 size, Color color) {
    const float cx = pos.x + size.x * 0.5f;
    const float cy = pos.y + size.y * 0.5f;
    const float outer_r = size.x * 0.5f - 0.5f;
    const float inner_r = size.x * 0.22f;
    const int teeth = 8;
    constexpr float pi = 3.14159265f;

    for (int i = 0; i < teeth; ++i) {
        const float a0 = (static_cast<float>(i) / teeth) * 2.f * pi;
        const float a1 = (static_cast<float>(i) + 0.35f) / teeth * 2.f * pi;
        const float a2 = (static_cast<float>(i) + 0.65f) / teeth * 2.f * pi;
        const float a3 = (static_cast<float>(i) + 1.f) / teeth * 2.f * pi;

        const Vec2 p0{cx + std::cos(a0) * inner_r, cy + std::sin(a0) * inner_r};
        const Vec2 p1{cx + std::cos(a1) * outer_r, cy + std::sin(a1) * outer_r};
        const Vec2 p2{cx + std::cos(a2) * outer_r, cy + std::sin(a2) * outer_r};
        const Vec2 p3{cx + std::cos(a3) * inner_r, cy + std::sin(a3) * inner_r};

        line::draw(p0, p1, color, 1.f);
        line::draw(p1, p2, color, 1.f);
        line::draw(p2, p3, color, 1.f);
    }

    circle::draw({cx, cy}, inner_r * 0.55f, color, 1.f, false);
}

} // namespace bitmap

} // namespace pcx::renderer
