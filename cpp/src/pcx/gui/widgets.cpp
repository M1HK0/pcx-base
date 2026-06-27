#include "pcx/gui/widgets.hpp"

#include "pcx/gui/config.hpp"
#include "pcx/gui/config_system.hpp"
#include "pcx/gui/containers.hpp"
#include "pcx/gui/gui.hpp"
#include "pcx/gui/hotkeys.hpp"
#include "pcx/input.hpp"
#include "pcx/renderer.hpp"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <cmath>
#include <vector>

namespace widgets {
namespace {

std::vector<std::shared_ptr<CWidget>> registered;
constexpr float snap_dist = 8.f;

void snap_to_widgets(CWidget& w, float new_x, float new_y, float& out_x, float& out_y) {
    const float box_w = w.get_box_w();
    const float box_h = w.get_box_h();
    float best_dx = snap_dist + 1.f;
    float best_dy = snap_dist + 1.f;
    out_x = new_x;
    out_y = new_y;
    for (const auto& other : registered) {
        if (other.get() == &w || !other->visible) continue;
        const float ox = static_cast<float>(other->pos_x);
        const float oy = static_cast<float>(other->pos_y);
        const float ow = other->get_box_w();
        const float oh = other->get_box_h();
        auto try_snap = [&](float d, float candidate, float& best, float& out, float target) {
            if (d < snap_dist && d < best) { best = d; out = target; }
        };
        try_snap(std::fabs((new_x + box_w) - ox), ox - box_w, best_dx, out_x, ox - box_w);
        try_snap(std::fabs(new_x - (ox + ow)), ox + ow, best_dx, out_x, ox + ow);
        try_snap(std::fabs(new_x - ox), ox, best_dx, out_x, ox);
        try_snap(std::fabs((new_x + box_w) - (ox + ow)), ox + ow - box_w, best_dx, out_x, ox + ow - box_w);
        try_snap(std::fabs((new_y + box_h) - oy), oy - box_h, best_dy, out_y, oy - box_h);
        try_snap(std::fabs(new_y - (oy + oh)), oy + oh, best_dy, out_y, oy + oh);
        try_snap(std::fabs(new_y - oy), oy, best_dy, out_y, oy);
        try_snap(std::fabs((new_y + box_h) - (oy + oh)), oy + oh - box_h, best_dy, out_y, oy + oh - box_h);
    }
    if (best_dx > snap_dist) out_x = new_x;
    if (best_dy > snap_dist) out_y = new_y;
}

void snap_to_viewport(CWidget& w, float new_x, float new_y, float& out_x, float& out_y) {
    const auto vp = CWindow::get_viewport();
    const float box_w = w.get_box_w();
    const float box_h = w.get_box_h();
    out_x = new_x;
    out_y = new_y;
    if (std::fabs(new_x) < snap_dist) out_x = 0.f;
    if (std::fabs(new_y) < snap_dist) out_y = 0.f;
    if (std::fabs((new_x + box_w) - vp.x) < snap_dist) out_x = vp.x - box_w;
    if (std::fabs((new_y + box_h) - vp.y) < snap_dist) out_y = vp.y - box_h;
    if (out_x < 0.f) out_x = 0.f;
    if (out_y < 0.f) out_y = 0.f;
    if (out_x + box_w > vp.x) out_x = vp.x - box_w;
    if (out_y + box_h > vp.y) out_y = vp.y - box_h;
}

} // namespace

float CWidget::get_header_h() const {
    float h = pcx::renderer::text::get_size(title, pcx::EFont::TAHOMA).y + 20.f;
    return h;
}

float CWidget::get_box_w() const {
    const auto ts = pcx::renderer::text::get_size(title, pcx::EFont::TAHOMA);
    const float tw = title_w_override > 0.f ? title_w_override : ts.x;
    return (content_w > tw ? content_w : tw) + 24.f;
}

float CWidget::get_box_h() const {
    float h = get_header_h();
    if (content_h > 0.f) h += content_h + 12.f;
    return h;
}

std::shared_ptr<CWidget> create(const std::string& id, const std::string& title, int x, int y) {
    auto w = std::make_shared<CWidget>(id, title, x, y);
    registered.push_back(w);
    return w;
}

void save() {
    nlohmann::json d;
    for (const auto& w : registered) {
        d[w->id + "_x"] = w->pos_x;
        d[w->id + "_y"] = w->pos_y;
        d[w->id + "_visible"] = w->visible ? 1.0 : 0.0;
    }
    pcx::create_file(gui::config_system::base_path + "\\.widgets", d.dump());
}

void load() {
    std::string data;
    if (!pcx::read_file(gui::config_system::base_path + "\\.widgets", data)) return;
    nlohmann::json d;
    try { d = nlohmann::json::parse(data); }
    catch (...) { return; }
    for (const auto& w : registered) {
        if (d.contains(w->id + "_x")) w->pos_x = d[w->id + "_x"].get<int>();
        if (d.contains(w->id + "_y")) w->pos_y = d[w->id + "_y"].get<int>();
        if (d.contains(w->id + "_visible")) w->visible = d[w->id + "_visible"].get<double>() > 0.5;
    }
}

void initialize() {
    if (pcx::does_file_exist(gui::config_system::base_path + "\\.widgets"))
        load();
}

void update() {
    for (auto& w : registered) {
        if (w->update_fn) w->update_fn(*w);
    }
    if (!gui::config::settings::menu_key || !gui::config::settings::menu_key->is_active()) return;
    for (auto& w : registered) {
        if (!w->visible) continue;
        const float bx = static_cast<float>(w->pos_x);
        const float by = static_cast<float>(w->pos_y);
        const float box_w = w->get_box_w();
        const float box_h = w->get_box_h();
        if (pcx::cursor::m1_pressed && pcx::cursor::is_hovered({bx, by}, {box_w, box_h}) && !w->dragging) {
            w->dragging = true;
            w->drag_ox = pcx::cursor::x - bx;
            w->drag_oy = pcx::cursor::y - by;
        }
        if (w->dragging) {
            if (pcx::cursor::m1_down) {
                float new_x = pcx::cursor::x - w->drag_ox;
                float new_y = pcx::cursor::y - w->drag_oy;
                if (pcx::input::key_down(0x10)) {
                    constexpr float grid = 5.f;
                    new_x = std::round(new_x / grid) * grid;
                    new_y = std::round(new_y / grid) * grid;
                }
                float sx, sy;
                snap_to_widgets(*w, new_x, new_y, sx, sy);
                snap_to_viewport(*w, sx, sy, new_x, new_y);
                w->pos_x = static_cast<int>(new_x);
                w->pos_y = static_cast<int>(new_y);
            } else {
                w->dragging = false;
                save();
            }
        }
    }
}

void render() {
    for (const auto& w : registered) {
        if (!w->visible) continue;
        const float bx = static_cast<float>(w->pos_x);
        const float by = static_cast<float>(w->pos_y);
        const float header_h = w->get_header_h();
        const float box_w = w->get_box_w();
        const float box_h = w->get_box_h();
        const bool has_content = w->content_h > 0.f && w->render_fn;
        pcx::renderer::rect::draw_glow({bx, by}, {box_w, box_h}, gui::colors::glow, 4.f, 4);
        if (has_content) {
            pcx::renderer::rect::draw_filled_rounded({bx, by}, {box_w, header_h}, gui::colors::background_secondary, 4.f, pcx::RR_TOP_LEFT | pcx::RR_TOP_RIGHT);
            pcx::renderer::rect::draw_filled_rounded({bx, by + header_h}, {box_w, box_h - header_h}, gui::colors::background_primary, 4.f, pcx::RR_BOTTOM_LEFT | pcx::RR_BOTTOM_RIGHT);
        } else {
            pcx::renderer::rect::draw_filled_rounded({bx, by}, {box_w, box_h}, gui::colors::background_secondary, 4.f);
        }
        pcx::renderer::rect::draw({bx + 1.f, by + 1.f}, {box_w - 2.f, box_h - 2.f}, gui::colors::border, 1.f, 4.f,
            pcx::RR_TOP_LEFT | pcx::RR_TOP_RIGHT | pcx::RR_BOTTOM_LEFT | pcx::RR_BOTTOM_RIGHT);
        pcx::renderer::line::draw({bx + 4.f, by + 4.f}, {bx + 4.f, by + header_h - 4.f}, gui::colors::accent_primary);
        const auto title_size = pcx::renderer::text::get_size(w->title, pcx::EFont::TAHOMA);
        float title_x = bx + 12.f;
        const float title_y = by + (header_h - title_size.y) / 2.f;
        if (w->title_fn) w->title_fn(*w, title_x, title_y);
        else pcx::renderer::text::draw_shadowed(w->title, {title_x, title_y}, gui::colors::text_active, gui::colors::outline, pcx::EFont::TAHOMA);
        if (has_content)
            w->render_fn(*w, bx + 10.f, by + header_h + 6.f, box_w - 20.f, w->content_h);
    }
}

} // namespace widgets

namespace widget_system {
namespace {

std::shared_ptr<CWidget> watermark;
std::shared_ptr<CWidget> keybinds;

void watermark_update(CWidget& w) {
    if (gui::config::settings::widgets::watermark)
        w.visible = gui::config::settings::widgets::watermark->get();
}

void watermark_title(CWidget&, float x, float y) {
    const std::string sep = " | ";
    pcx::renderer::text::draw_shadowed("perception", {x, y}, gui::colors::text_active, gui::colors::outline, pcx::EFont::TAHOMA);
    float tx = x + pcx::renderer::text::get_size("perception", pcx::EFont::TAHOMA).x;
    pcx::renderer::text::draw_shadowed(".cx", {tx, y}, gui::colors::text_inactive, gui::colors::outline, pcx::EFont::TAHOMA);
    tx += pcx::renderer::text::get_size(".cx", pcx::EFont::TAHOMA).x;
    pcx::renderer::text::draw_shadowed(sep, {tx, y}, gui::colors::text_inactive, gui::colors::outline, pcx::EFont::TAHOMA);
    tx += pcx::renderer::text::get_size(sep, pcx::EFont::TAHOMA).x;
    pcx::renderer::text::draw_shadowed(pcx::globals::username, {tx, y}, gui::colors::text_active, gui::colors::outline, pcx::EFont::TAHOMA);
}

void keybinds_render(CWidget&, float x, float y, float cw, float) {
    const float line_h = pcx::renderer::text::get_size("A", pcx::EFont::TAHOMA).y;
    const float row_h = line_h + 8.f;
    float current_y = y;
    for (const auto& kb : hotkeys::registered()) {
        if (!kb || kb->key == 0 || !kb->is_active()) continue;
        std::string mode_text = "always";
        if (kb->mode == pcx::EKeybindMode::HOLD) mode_text = "hold";
        else if (kb->mode == pcx::EKeybindMode::TOGGLE) mode_text = "toggle";
        const float text_y = current_y + (row_h - line_h) / 2.f;
        pcx::renderer::text::draw_shadowed(kb->hotkey_name, {x + 2.f, text_y}, gui::colors::text_active, gui::colors::outline, pcx::EFont::TAHOMA);
        const float mode_w = pcx::renderer::text::get_size(mode_text, pcx::EFont::TAHOMA).x;
        pcx::renderer::text::draw_shadowed(mode_text, {x + cw - mode_w - 2.f, text_y}, gui::colors::accent_primary, gui::colors::outline, pcx::EFont::TAHOMA);
        current_y += row_h;
    }
}

void keybinds_update(CWidget& w) {
    if (!gui::config::settings::widgets::hotkeys || !gui::config::settings::widgets::hotkeys->get()) {
        w.visible = false;
        return;
    }
    const float line_h = pcx::renderer::text::get_size("A", pcx::EFont::TAHOMA).y;
    const float row_h = line_h + 8.f;
    int count = 0;
    float max_w = 0.f;
    for (const auto& kb : hotkeys::registered()) {
        if (!kb || kb->key == 0 || !kb->is_active()) continue;
        std::string mode_text = "always";
        if (kb->mode == pcx::EKeybindMode::HOLD) mode_text = "hold";
        else if (kb->mode == pcx::EKeybindMode::TOGGLE) mode_text = "toggle";
        const float row_w = pcx::renderer::text::get_size(kb->hotkey_name, pcx::EFont::TAHOMA).x + 24.f +
            pcx::renderer::text::get_size(mode_text, pcx::EFont::TAHOMA).x;
        max_w = std::max(max_w, row_w);
        ++count;
    }
    w.content_w = max_w > 100.f ? max_w : 100.f;
    w.content_h = count > 0 ? count * row_h + (count - 1) : 0.f;
    w.visible = count > 0 || (gui::config::settings::menu_key && gui::config::settings::menu_key->is_active());
}

} // namespace

void initialize() {
    watermark = widgets::create("watermark", "watermark", 10, 10);
    watermark->title_w_override = pcx::renderer::text::get_size("perception.cx | " + pcx::globals::username, pcx::EFont::TAHOMA).x;
    watermark->title_fn = watermark_title;
    watermark->update_fn = watermark_update;

    keybinds = widgets::create("keybinds", "hotkeys", 10, 50);
    keybinds->content_w = 100.f;
    keybinds->render_fn = keybinds_render;
    keybinds->update_fn = keybinds_update;

    widgets::initialize();
}

} // namespace widget_system
