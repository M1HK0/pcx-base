#include "pcx/gui/gui.hpp"

#include "pcx/math.hpp"
#include "pcx/renderer.hpp"

namespace gui {

void sync_colors() {
    if (!config::theme::accent_primary) return;
    gui::colors::accent_primary = config::theme::accent_primary->get();
    gui::colors::accent_secondary = config::theme::accent_secondary->get();
    gui::colors::background_primary = config::theme::background_primary->get();
    gui::colors::background_secondary = config::theme::background_secondary->get();
    gui::colors::text_active = config::theme::text_active->get();
    gui::colors::text_inactive = config::theme::text_inactive->get();
    gui::colors::border = config::theme::border->get();
    gui::colors::glow = config::theme::glow->get();
}

void tick() {
    window.tick();
}

void update() {
    if (!config::settings::menu_key) return;
    const float target = config::settings::menu_key->is_active() ? 1.f : 0.f;
    open_anim = pcx::lerp(open_anim, target, 0.03f);
    if (open_anim < 0.01f) open_anim = 0.f;
    if (open_anim > 0.99f) open_anim = 1.f;
    pcx::globals::menu_alpha = static_cast<uint8_t>(255.f * pcx::smoothstep(0.f, 1.f, open_anim));

    if (config::settings::menu_key->is_active()) {
        sync_colors();
        window.update();
    }
}

void render() {
    if (open_anim <= 0.f) return;
    const float cx = window.position.x + window.size.x / 2.f;
    const float cy = window.position.y + window.size.y / 2.f;
    const float anim_w = window.size.x * open_anim;
    const float anim_h = window.size.y * open_anim;
    const float anim_x = cx - anim_w / 2.f;
    const float anim_y = cy - anim_h / 2.f;

    pcx::renderer::rect::draw_glow({anim_x, anim_y}, {anim_w, anim_h}, gui::colors::scaled(gui::colors::glow), 4.f, 4);
    pcx::renderer::clip_push(static_cast<int>(anim_x), static_cast<int>(anim_y), static_cast<int>(anim_w), static_cast<int>(anim_h));
    window.render();
    pcx::renderer::clip_pop();

    if (open_anim > 0.5f)
        window.render_popups();
}

} // namespace gui
