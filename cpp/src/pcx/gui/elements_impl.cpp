#include "pcx/gui/elements_all.hpp"

#include "pcx/gui/config_system.hpp"

#include <cstdio>

// ===========================================================================
// CElement
// ===========================================================================
bool CElement::is_visible() const {
    switch (type) {
    case pcx::EElementType::CHECKBOX: return checkbox->is_visible();
    case pcx::EElementType::SLIDER_INT: return slider_int->is_visible();
    case pcx::EElementType::SLIDER_FLOAT: return slider_float->is_visible();
    case pcx::EElementType::KEYBIND: return keybind->is_visible();
    case pcx::EElementType::DROPDOWN: return dropdown->is_visible();
    case pcx::EElementType::MULTISELECT: return multiselect->is_visible();
    case pcx::EElementType::TEXT_INPUT: return text_input->is_visible();
    case pcx::EElementType::BUTTON: return button->is_visible();
    case pcx::EElementType::COLORPICKER: return colorpicker->is_visible();
    case pcx::EElementType::COGWHEEL: return cogwheel->is_visible();
    case pcx::EElementType::LISTBOX: return listbox->is_visible();
    case pcx::EElementType::TOOLTIP: return tooltip->is_visible();
    case pcx::EElementType::LABEL: return label->is_visible();
    default: return true;
    }
}

bool CElement::is_blocking() const {
    switch (type) {
    case pcx::EElementType::DROPDOWN: return dropdown->is_open;
    case pcx::EElementType::KEYBIND: return keybind->is_open;
    case pcx::EElementType::MULTISELECT: return multiselect->is_open;
    case pcx::EElementType::TEXT_INPUT: return text_input->focused;
    case pcx::EElementType::COLORPICKER: return colorpicker->is_open;
    case pcx::EElementType::COGWHEEL: return cogwheel->is_open;
    default: return false;
    }
}

void CElement::tick() {
    switch (type) {
    case pcx::EElementType::CHECKBOX: checkbox->tick(); break;
    case pcx::EElementType::SLIDER_INT: slider_int->tick(); break;
    case pcx::EElementType::SLIDER_FLOAT: slider_float->tick(); break;
    case pcx::EElementType::KEYBIND: keybind->tick(); break;
    case pcx::EElementType::COGWHEEL: cogwheel->tick(); break;
    case pcx::EElementType::LABEL: label->tick(); break;
    default: break;
    }
}

void CElement::handle_input(float x, float y, float w) {
    switch (type) {
    case pcx::EElementType::CHECKBOX: checkbox->handle_input(x, y, w); break;
    case pcx::EElementType::SLIDER_INT: slider_int->handle_input(x, y, w); break;
    case pcx::EElementType::SLIDER_FLOAT: slider_float->handle_input(x, y, w); break;
    case pcx::EElementType::KEYBIND: keybind->handle_input(x, y, w); break;
    case pcx::EElementType::DROPDOWN: dropdown->handle_input(x, y, w); break;
    case pcx::EElementType::MULTISELECT: multiselect->handle_input(x, y, w); break;
    case pcx::EElementType::TEXT_INPUT: text_input->handle_input(x, y, w); break;
    case pcx::EElementType::BUTTON: button->handle_input(x, y, w); break;
    case pcx::EElementType::COLORPICKER: colorpicker->handle_input(x, y, w); break;
    case pcx::EElementType::COGWHEEL: cogwheel->handle_input(x, y, w); break;
    case pcx::EElementType::LISTBOX: listbox->handle_input(x, y, w); break;
    case pcx::EElementType::TOOLTIP: tooltip->handle_input(x, y, w); break;
    case pcx::EElementType::LABEL: label->handle_input(x, y, w); break;
    default: break;
    }
}

void CElement::render(float x, float y, float w) {
    switch (type) {
    case pcx::EElementType::CHECKBOX: checkbox->render(x, y, w); break;
    case pcx::EElementType::SLIDER_INT: slider_int->render(x, y, w); break;
    case pcx::EElementType::SLIDER_FLOAT: slider_float->render(x, y, w); break;
    case pcx::EElementType::KEYBIND: keybind->render(x, y, w); break;
    case pcx::EElementType::DROPDOWN: dropdown->render(x, y, w); break;
    case pcx::EElementType::MULTISELECT: multiselect->render(x, y, w); break;
    case pcx::EElementType::TEXT_INPUT: text_input->render(x, y, w); break;
    case pcx::EElementType::BUTTON: button->render(x, y, w); break;
    case pcx::EElementType::COLORPICKER: colorpicker->render(x, y, w); break;
    case pcx::EElementType::COGWHEEL: cogwheel->render(x, y, w); break;
    case pcx::EElementType::LISTBOX: listbox->render(x, y, w); break;
    case pcx::EElementType::TOOLTIP: tooltip->render(x, y, w); break;
    case pcx::EElementType::LABEL: label->render(x, y, w); break;
    default: break;
    }
}

void CElement::render_popup(float x, float y, float w, float max_y) {
    switch (type) {
    case pcx::EElementType::CHECKBOX: checkbox->render_popup(x, y, w); break;
    case pcx::EElementType::SLIDER_INT: slider_int->render_popup(x, y, w); break;
    case pcx::EElementType::SLIDER_FLOAT: slider_float->render_popup(x, y, w); break;
    case pcx::EElementType::KEYBIND: keybind->render_popup(x, y, w); break;
    case pcx::EElementType::DROPDOWN: dropdown->render_popup(x, y, w, max_y); break;
    case pcx::EElementType::MULTISELECT: multiselect->render_popup(x, y, w, max_y); break;
    case pcx::EElementType::COLORPICKER: colorpicker->render_popup(x, y, w); break;
    case pcx::EElementType::COGWHEEL: cogwheel->render_popup(x, y, w); break;
    case pcx::EElementType::TOOLTIP: tooltip->render_popup(x, y, w); break;
    case pcx::EElementType::LABEL: label->render_popup(x, y, w); break;
    default: break;
    }
}

float CElement::get_height() const {
    switch (type) {
    case pcx::EElementType::CHECKBOX: return checkbox->get_height();
    case pcx::EElementType::SLIDER_INT: return slider_int->get_height();
    case pcx::EElementType::SLIDER_FLOAT: return slider_float->get_height();
    case pcx::EElementType::KEYBIND: return keybind->get_height();
    case pcx::EElementType::DROPDOWN: return dropdown->get_height();
    case pcx::EElementType::MULTISELECT: return multiselect->get_height();
    case pcx::EElementType::TEXT_INPUT: return text_input->get_height();
    case pcx::EElementType::BUTTON: return button->get_height();
    case pcx::EElementType::COLORPICKER: return colorpicker->get_height();
    case pcx::EElementType::COGWHEEL: return cogwheel->get_height();
    case pcx::EElementType::LISTBOX: return listbox->get_height();
    case pcx::EElementType::TOOLTIP: return tooltip->get_height();
    case pcx::EElementType::LABEL: return label->get_height();
    default: return 0.f;
    }
}

// ===========================================================================
// CKeybind
// ===========================================================================
void CKeybind::handle_input(float x, float y, float w) {
    const float box_w = 32.f, box_h = 12.f;
    const float box_x = attached ? x : x + w - box_w;

    if (pcx::cursor::m1_pressed && pcx::cursor::is_hovered({box_x, y}, {box_w, box_h}))
        is_open = !is_open;

    if (!is_open) return;

    const float popup_w = 120.f;
    const float popup_h = 20.f + 24.f + 6.f + (3 * 18.f) + 6.f;
    const float popup_x = box_x + box_w + 2.f;
    const float popup_y = y;
    const bool hover_popup = pcx::cursor::is_hovered({popup_x, popup_y}, {popup_w, popup_h});

    const float key_btn_x = popup_x + 6.f;
    const float key_btn_y = popup_y + 20.f;
    const float key_btn_w = popup_w - 12.f;
    const float key_btn_h = 20.f;

    if (pcx::cursor::m1_pressed && pcx::cursor::is_hovered({key_btn_x, key_btn_y}, {key_btn_w, key_btn_h})) {
        waiting_for_key = true;
        waiting_for_release = true;
    }

    if (waiting_for_key) {
        if (waiting_for_release) {
            if (!pcx::cursor::m1_down) waiting_for_release = false;
        } else {
            if (pcx::input::key_singlepress(0x1B)) {
                key = 0;
                waiting_for_key = false;
                toggle_state = false;
            } else {
                std::vector<int> keys;
                pcx::input::get_keys_down(keys);
                for (int k : keys) {
                    if (k != 0x1B && pcx::input::key_singlepress(k)) {
                        key = k;
                        waiting_for_key = false;
                        toggle_state = false;
                        break;
                    }
                }
            }
        }
    }

    if (!waiting_for_key) {
        const float mode_y = key_btn_y + key_btn_h + 6.f;
        const float mode_h = 16.f;
        const float mode_w = popup_w - 12.f;
        for (int i = 0; i < 3; ++i) {
            const float option_y = mode_y + (i * (mode_h + 2.f));
            if (pcx::cursor::m1_pressed && pcx::cursor::is_hovered({popup_x + 6.f, option_y}, {mode_w, mode_h})) {
                mode = static_cast<pcx::EKeybindMode>(i);
                toggle_state = false;
            }
        }
        if (pcx::cursor::m1_pressed && !pcx::cursor::is_hovered({box_x, y}, {box_w, box_h}) && !hover_popup) {
            is_open = false;
            waiting_for_key = false;
        }
    }
}

void CKeybind::render(float x, float y, float w) {
    const float box_w = 32.f, box_h = 12.f;
    const float box_x = attached ? x : x + w - box_w;
    if (!attached)
        pcx::renderer::text::draw_shadowed(name, {x, y}, gui::colors::scaled(gui::colors::text_active),
            gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    pcx::renderer::rect::draw_filled({box_x, y}, {box_w, box_h}, gui::colors::scaled(gui::colors::background_primary));
    pcx::renderer::rect::draw({box_x, y}, {box_w, box_h}, gui::colors::scaled(gui::colors::border));
    const std::string key_text = get_key_display();
    const auto key_size = pcx::renderer::text::get_size(key_text, pcx::EFont::TAHOMA);
    const auto key_col = is_open ? gui::colors::scaled(gui::colors::accent_primary) : gui::colors::scaled(gui::colors::text_inactive);
    pcx::renderer::text::draw_shadowed(key_text, {box_x + (box_w - key_size.x) / 2.f, y + (box_h - key_size.y) / 2.f},
        key_col, gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
}

void CKeybind::render_popup(float x, float y, float w) {
    if (!is_open) return;
    const float box_w = 32.f;
    const float box_x = attached ? x : x + w - box_w;
    const float popup_w = 120.f;
    const float popup_h = 20.f + 24.f + 6.f + (3 * 18.f) + 6.f;
    const float popup_x = box_x + box_w + 2.f;
    const float popup_y = y;

    pcx::renderer::rect::draw_filled({popup_x, popup_y}, {popup_w, popup_h}, gui::colors::scaled(gui::colors::background_secondary));
    pcx::renderer::rect::draw({popup_x, popup_y}, {popup_w, popup_h}, gui::colors::scaled(gui::colors::border));
    pcx::renderer::text::draw_shadowed(name, {popup_x + 6.f, popup_y + 4.f}, gui::colors::scaled(gui::colors::text_active),
        gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMABD);

    const float key_btn_x = popup_x + 6.f;
    const float key_btn_y = popup_y + 20.f;
    const float key_btn_w = popup_w - 12.f;
    const float key_btn_h = 20.f;
    const bool hover_key = pcx::cursor::is_hovered({key_btn_x, key_btn_y}, {key_btn_w, key_btn_h});

    pcx::renderer::rect::draw_filled({key_btn_x, key_btn_y}, {key_btn_w, key_btn_h}, gui::colors::scaled(gui::colors::background_primary));
    pcx::renderer::rect::draw({key_btn_x, key_btn_y}, {key_btn_w, key_btn_h},
        hover_key ? gui::colors::scaled(gui::colors::accent_primary) : gui::colors::scaled(gui::colors::border));

    const std::string key_display = waiting_for_key ? "..." : get_key_display();
    const auto key_col = waiting_for_key ? gui::colors::scaled(gui::colors::accent_primary) : gui::colors::scaled(gui::colors::text_active);
    const auto key_size = pcx::renderer::text::get_size(key_display, pcx::EFont::TAHOMA);
    pcx::renderer::text::draw_shadowed(key_display,
        {key_btn_x + (key_btn_w - key_size.x) / 2.f, key_btn_y + (key_btn_h - key_size.y) / 2.f},
        key_col, gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);

    const float mode_y = key_btn_y + key_btn_h + 6.f;
    const float mode_h = 16.f;
    const float mode_w = popup_w - 12.f;
    const char* modes[] = {"always on", "on hold", "on toggle"};
    for (int i = 0; i < 3; ++i) {
        const float option_y = mode_y + (i * (mode_h + 2.f));
        const bool hovering = pcx::cursor::is_hovered({popup_x + 6.f, option_y}, {mode_w, mode_h});
        const bool selected = i == static_cast<int>(mode);
        if (hovering)
            pcx::renderer::rect::draw_filled({popup_x + 6.f, option_y}, {mode_w, mode_h}, gui::colors::scaled(gui::colors::background_primary));
        if (selected)
            pcx::renderer::line::draw({popup_x + 6.f, option_y + 2.f}, {popup_x + 6.f, option_y + mode_h - 2.f}, gui::colors::scaled(gui::colors::accent_primary));
        pcx::Color col = gui::colors::scaled(gui::colors::text_inactive);
        if (selected) col = gui::colors::scaled(gui::colors::accent_primary);
        else if (hovering) col = gui::colors::scaled(gui::colors::text_active);
        pcx::renderer::text::draw_shadowed(modes[i], {popup_x + 12.f, option_y + 2.f}, col, gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    }
}

// ===========================================================================
// CColorPicker
// ===========================================================================
void CColorPicker::handle_input(float x, float y, float w) {
    const float box_w = 12.f, box_h = 12.f;
    const float box_x = attached ? x : x + w - box_w;
    if (pcx::cursor::m1_pressed && pcx::cursor::is_hovered({box_x, y}, {box_w, box_h}))
        is_open = !is_open;
    if (!is_open) return;

    const float popup_w = 160.f, popup_h = 200.f;
    const float popup_x = box_x + box_w + 2.f;
    const float popup_y = y;
    const float picker_x = popup_x + 6.f;
    const float picker_y = popup_y + 20.f;
    const float picker_w = popup_w - 12.f;
    const float picker_h = 100.f;
    const float hue_y = picker_y + picker_h + 6.f;
    const float hue_h = 12.f;
    const float alpha_y = hue_y + hue_h + 6.f;
    const float alpha_h = 12.f;
    const float btn_y = alpha_y + alpha_h + 6.f;
    const float btn_w = (picker_w - 4.f) / 2.f;
    const float btn_h = 16.f;

    if (pcx::cursor::m1_pressed) {
        if (pcx::cursor::is_hovered({picker_x, picker_y}, {picker_w, picker_h})) dragging_picker = true;
        else if (pcx::cursor::is_hovered({picker_x, hue_y}, {picker_w, hue_h})) dragging_hue = true;
        else if (pcx::cursor::is_hovered({picker_x, alpha_y}, {picker_w, alpha_h})) dragging_alpha = true;
        else if (pcx::cursor::is_hovered({picker_x, btn_y}, {btn_w, btn_h})) gui::colors::copied_color = value;
        else if (pcx::cursor::is_hovered({picker_x + btn_w + 4.f, btn_y}, {btn_w, btn_h})) {
            value = gui::colors::copied_color;
            rgb_to_hsv(value.r, value.g, value.b, hue, sat, val);
        }
    }
    if (!pcx::cursor::m1_down) {
        dragging_picker = dragging_hue = dragging_alpha = false;
    }
    if (dragging_picker) {
        sat = pcx::clamp((pcx::cursor::x - picker_x) / picker_w, 0.f, 1.f);
        val = 1.f - pcx::clamp((pcx::cursor::y - picker_y) / picker_h, 0.f, 1.f);
        update_from_hsv();
    }
    if (dragging_hue) {
        hue = pcx::clamp((pcx::cursor::x - picker_x) / picker_w, 0.f, 1.f) * 360.f;
        update_from_hsv();
    }
    if (dragging_alpha) {
        value.a = static_cast<uint8_t>(pcx::clamp((pcx::cursor::x - picker_x) / picker_w, 0.f, 1.f) * 255.f);
    }
    const bool hover_popup = pcx::cursor::is_hovered({popup_x, popup_y}, {popup_w, popup_h});
    if (pcx::cursor::m1_pressed && !pcx::cursor::is_hovered({box_x, y}, {box_w, box_h}) && !hover_popup)
        is_open = false;
}

void CColorPicker::render(float x, float y, float w) {
    const float box_w = 12.f, box_h = 12.f;
    const float box_x = attached ? x : x + w - box_w;
    if (!attached)
        pcx::renderer::text::draw_shadowed(name, {x, y}, gui::colors::scaled(gui::colors::text_active),
            gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    const uint8_t a = static_cast<uint8_t>((value.a * pcx::globals::menu_alpha) / 255);
    const uint8_t lr = static_cast<uint8_t>(pcx::clamp(static_cast<int>(value.r) + 30, 0, 255));
    const uint8_t lg = static_cast<uint8_t>(pcx::clamp(static_cast<int>(value.g) + 30, 0, 255));
    const uint8_t lb = static_cast<uint8_t>(pcx::clamp(static_cast<int>(value.b) + 30, 0, 255));
    const uint8_t dr = static_cast<uint8_t>(pcx::clamp(static_cast<int>(value.r) - 30, 0, 255));
    const uint8_t dg = static_cast<uint8_t>(pcx::clamp(static_cast<int>(value.g) - 30, 0, 255));
    const uint8_t db = static_cast<uint8_t>(pcx::clamp(static_cast<int>(value.b) - 30, 0, 255));
    pcx::renderer::rect::draw_gradient({box_x + 2.f, y + 2.f}, {box_w - 4.f, box_h - 4.f},
        pcx::Color{lr, lg, lb, a}, pcx::Color{value.r, value.g, value.b, a},
        pcx::Color{value.r, value.g, value.b, a}, pcx::Color{dr, dg, db, a});
    pcx::renderer::rect::draw({box_x, y}, {box_w, box_h}, gui::colors::scaled(gui::colors::border));
}

void CColorPicker::render_popup(float x, float y, float w) {
    if (!is_open) return;
    const float box_w = 12.f;
    const float box_x = attached ? x : x + w - box_w;
    const float popup_w = 160.f, popup_h = 200.f;
    const float popup_x = box_x + box_w + 2.f;
    const float popup_y = y;
    const float picker_x = popup_x + 6.f;
    const float picker_y = popup_y + 20.f;
    const float picker_w = popup_w - 12.f;
    const float picker_h = 100.f;
    const float hue_y = picker_y + picker_h + 6.f;
    const float hue_h = 12.f;
    const float alpha_y = hue_y + hue_h + 6.f;
    const float alpha_h = 12.f;
    const float btn_y = alpha_y + alpha_h + 6.f;
    const float btn_w = (picker_w - 4.f) / 2.f;
    const float btn_h = 16.f;

    pcx::renderer::rect::draw_filled({popup_x, popup_y}, {popup_w, popup_h}, gui::colors::scaled(gui::colors::background_secondary));
    pcx::renderer::rect::draw({popup_x, popup_y}, {popup_w, popup_h}, gui::colors::scaled(gui::colors::border));
    pcx::renderer::text::draw_shadowed(name, {picker_x, popup_y + 4.f}, gui::colors::scaled(gui::colors::text_active),
        gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMABD);

    constexpr int num_cols = 60, num_rows = 60;
    const float cell_w = picker_w / static_cast<float>(num_cols);
    const float cell_h = picker_h / static_cast<float>(num_rows);
    for (int row = 0; row < num_rows; ++row) {
        const float v = 1.f - (static_cast<float>(row) + 0.5f) / static_cast<float>(num_rows);
        for (int col = 0; col < num_cols; ++col) {
            const float s = (static_cast<float>(col) + 0.5f) / static_cast<float>(num_cols);
            uint8_t cr, cg, cb;
            hsv_to_rgb(hue, s, v, cr, cg, cb);
            pcx::renderer::rect::draw_filled({picker_x + col * cell_w, picker_y + row * cell_h},
                {cell_w + 1.f, cell_h + 1.f}, pcx::Color{cr, cg, cb, pcx::globals::menu_alpha});
        }
    }
    pcx::renderer::rect::draw({picker_x, picker_y}, {picker_w, picker_h}, gui::colors::scaled(gui::colors::border));

    const float cx = picker_x + sat * picker_w;
    const float cy = picker_y + (1.f - val) * picker_h;
    pcx::renderer::rect::draw({cx - 3.f, cy - 3.f}, {6.f, 6.f}, pcx::Color{255, 255, 255, pcx::globals::menu_alpha});

    const float hue_step = picker_w / 6.f;
    for (int i = 0; i < 6; ++i) {
        auto left = hue_to_rgb(i * 60.f);
        auto right = hue_to_rgb((i + 1) * 60.f);
        left.a = pcx::globals::menu_alpha;
        right.a = pcx::globals::menu_alpha;
        pcx::renderer::rect::draw_gradient({picker_x + i * hue_step, hue_y}, {hue_step + 1.f, hue_h}, left, right, left, right);
    }
    pcx::renderer::rect::draw({picker_x, hue_y}, {picker_w, hue_h}, gui::colors::scaled(gui::colors::border));
    const float hue_cx = picker_x + (hue / 360.f) * picker_w;
    pcx::renderer::rect::draw_filled({hue_cx - 1.f, hue_y - 1.f}, {3.f, hue_h + 2.f}, pcx::Color{255, 255, 255, pcx::globals::menu_alpha});

    pcx::renderer::rect::draw_gradient({picker_x, alpha_y}, {picker_w, alpha_h},
        pcx::Color{value.r, value.g, value.b, 0}, pcx::Color{value.r, value.g, value.b, pcx::globals::menu_alpha},
        pcx::Color{value.r, value.g, value.b, 0}, pcx::Color{value.r, value.g, value.b, pcx::globals::menu_alpha});
    pcx::renderer::rect::draw({picker_x, alpha_y}, {picker_w, alpha_h}, gui::colors::scaled(gui::colors::border));
    const float alpha_cx = picker_x + (static_cast<float>(value.a) / 255.f) * picker_w;
    pcx::renderer::rect::draw_filled({alpha_cx - 1.f, alpha_y - 1.f}, {3.f, alpha_h + 2.f}, pcx::Color{255, 255, 255, pcx::globals::menu_alpha});

    const bool hover_copy = pcx::cursor::is_hovered({picker_x, btn_y}, {btn_w, btn_h});
    pcx::renderer::rect::draw_filled({picker_x, btn_y}, {btn_w, btn_h}, gui::colors::scaled(gui::colors::background_primary));
    pcx::renderer::rect::draw({picker_x, btn_y}, {btn_w, btn_h}, hover_copy ? gui::colors::scaled(gui::colors::accent_primary) : gui::colors::scaled(gui::colors::border));
    const auto copy_size = pcx::renderer::text::get_size("copy", pcx::EFont::TAHOMA);
    pcx::renderer::text::draw_shadowed("copy", {picker_x + (btn_w - copy_size.x) / 2.f, btn_y + (btn_h - copy_size.y) / 2.f},
        hover_copy ? gui::colors::scaled(gui::colors::text_active) : gui::colors::scaled(gui::colors::text_inactive),
        gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);

    const float paste_x = picker_x + btn_w + 4.f;
    const bool hover_paste = pcx::cursor::is_hovered({paste_x, btn_y}, {btn_w, btn_h});
    pcx::renderer::rect::draw_filled({paste_x, btn_y}, {btn_w, btn_h}, gui::colors::scaled(gui::colors::background_primary));
    pcx::renderer::rect::draw({paste_x, btn_y}, {btn_w, btn_h}, hover_paste ? gui::colors::scaled(gui::colors::accent_primary) : gui::colors::scaled(gui::colors::border));
    const auto paste_size = pcx::renderer::text::get_size("paste", pcx::EFont::TAHOMA);
    pcx::renderer::text::draw_shadowed("paste", {paste_x + (btn_w - paste_size.x) / 2.f, btn_y + (btn_h - paste_size.y) / 2.f},
        hover_paste ? gui::colors::scaled(gui::colors::text_active) : gui::colors::scaled(gui::colors::text_inactive),
        gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
}

// ===========================================================================
// CCheckbox
// ===========================================================================
std::shared_ptr<CKeybind> CCheckbox::create_keybind(const std::string& n, int key, pcx::EKeybindMode mode) {
    auto kb = std::make_shared<CKeybind>(n, key, mode);
    kb->attached = true;
    kb->parent_checkbox = shared_from_this();
    attachments.push_back(std::make_shared<CElement>(kb));
    if (!config_key.empty())
        gui::config_system::register_element(config_key + "." + n, attachments.back());
    return kb;
}

std::shared_ptr<CColorPicker> CCheckbox::create_colorpicker(const std::string& n, pcx::Color def) {
    auto cp = std::make_shared<CColorPicker>(n, def);
    cp->attached = true;
    attachments.push_back(std::make_shared<CElement>(cp));
    if (!config_key.empty())
        gui::config_system::register_element(config_key + "." + n, attachments.back());
    return cp;
}

std::shared_ptr<CCogWheel> CCheckbox::create_cogwheel(const std::string& n) {
    auto cog = std::make_shared<CCogWheel>(n);
    if (!config_key.empty()) cog->config_key = config_key + "." + n;
    attachments.push_back(std::make_shared<CElement>(cog));
    return cog;
}

std::shared_ptr<CTooltip> CCheckbox::create_tooltip(const std::string& desc) {
    auto tip = std::make_shared<CTooltip>(desc);
    attachments.push_back(std::make_shared<CElement>(tip));
    return tip;
}

void CCheckbox::tick() {
    for (auto& a : attachments) a->tick();
}

float CCheckbox::get_attachment_width(const std::shared_ptr<CElement>& el) const {
    switch (el->type) {
    case pcx::EElementType::KEYBIND:
    case pcx::EElementType::COLORPICKER:
    case pcx::EElementType::COGWHEEL:
    case pcx::EElementType::TOOLTIP:
        return 12.f;
    default: return 12.f;
    }
}

void CCheckbox::handle_input(float x, float y, float w) {
    float right_edge = x + w;
    for (auto& att : attachments) {
        const float att_w = get_attachment_width(att);
        right_edge -= att_w;
        att->handle_input(right_edge, y, att_w);
        right_edge -= 4.f;
    }
    const auto text_size = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA);
    const float hitbox_w = 12.f + 4.f + text_size.x;
    if (pcx::cursor::m1_pressed && pcx::cursor::is_hovered({x, y}, {hitbox_w, 12.f}))
        value = !value;
}

void CCheckbox::render(float x, float y, float w) {
    pcx::renderer::rect::draw_filled({x, y}, {12.f, 12.f}, gui::colors::scaled(gui::colors::background_primary));
    pcx::renderer::rect::draw({x, y}, {12.f, 12.f}, gui::colors::scaled(gui::colors::border));
    const float target = value ? 1.f : 0.f;
    alpha = pcx::lerp(alpha, target, 0.05f);
    if (alpha > 0.01f) {
        const uint8_t a1 = static_cast<uint8_t>((gui::colors::accent_primary.a * alpha * pcx::globals::menu_alpha) / 255);
        const uint8_t a2 = static_cast<uint8_t>((gui::colors::accent_secondary.a * alpha * pcx::globals::menu_alpha) / 255);
        pcx::Color c1{gui::colors::accent_primary.r, gui::colors::accent_primary.g, gui::colors::accent_primary.b, a1};
        pcx::Color c2{gui::colors::accent_secondary.r, gui::colors::accent_secondary.g, gui::colors::accent_secondary.b, a2};
        pcx::renderer::rect::draw_gradient({x + 2.f, y + 2.f}, {8.f, 8.f}, c1, c2, c1, c2);
    }
    const auto text_size = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA);
    const bool hovered = pcx::cursor::is_hovered({x, y}, {12.f + 4.f + text_size.x, 12.f});
    pcx::Color col = gui::colors::scaled(gui::colors::text_inactive);
    if (value) col = gui::colors::scaled(gui::colors::text_active);
    else if (hovered) col = gui::colors::scaled(gui::colors::text_hover);
    pcx::renderer::text::draw_shadowed(name, {x + 16.f, y + (12.f - text_size.y) / 2.f}, col, gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    float right_edge = x + w;
    for (auto& att : attachments) {
        const float att_w = get_attachment_width(att);
        right_edge -= att_w;
        att->render(right_edge, y, att_w);
        right_edge -= 4.f;
    }
}

void CCheckbox::render_popup(float x, float y, float w) {
    float right_edge = x + w;
    for (auto& att : attachments) {
        const float att_w = get_attachment_width(att);
        right_edge -= att_w;
        att->render_popup(right_edge, y, att_w);
        right_edge -= 4.f;
    }
}

// ===========================================================================
// CLabel
// ===========================================================================
std::shared_ptr<CCogWheel> CLabel::create_cogwheel(const std::string& n) {
    auto cog = std::make_shared<CCogWheel>(n);
    if (!config_key.empty()) cog->config_key = config_key + "." + n;
    attachments.push_back(std::make_shared<CElement>(cog));
    return cog;
}

std::shared_ptr<CTooltip> CLabel::create_tooltip(const std::string& desc) {
    auto tip = std::make_shared<CTooltip>(desc);
    attachments.push_back(std::make_shared<CElement>(tip));
    return tip;
}

void CLabel::tick() { for (auto& a : attachments) a->tick(); }

float CLabel::get_attachment_width(const std::shared_ptr<CElement>&) const { return 12.f; }

void CLabel::handle_input(float x, float y, float w) {
    float right_edge = x + w;
    for (auto& att : attachments) {
        const float att_w = get_attachment_width(att);
        right_edge -= att_w;
        att->handle_input(right_edge, y, att_w);
        right_edge -= 4.f;
    }
}

void CLabel::render(float x, float y, float w) {
    const auto text_size = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA);
    pcx::renderer::text::draw_shadowed(name, {x, y + (12.f - text_size.y) / 2.f},
        gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    float right_edge = x + w;
    for (auto& att : attachments) {
        const float att_w = get_attachment_width(att);
        right_edge -= att_w;
        att->render(right_edge, y, att_w);
        right_edge -= 4.f;
    }
}

void CLabel::render_popup(float x, float y, float w) {
    float right_edge = x + w;
    for (auto& att : attachments) {
        const float att_w = get_attachment_width(att);
        right_edge -= att_w;
        att->render_popup(right_edge, y, att_w);
        right_edge -= 4.f;
    }
}

// ===========================================================================
// Sliders
// ===========================================================================
static void slider_handle_common(float x, float y, float w, float attachments_w,
    float& minus_x, float& track_x, float& track_w, float& plus_x, float slider_y) {
    const auto minus_size = pcx::renderer::text::get_size("-", pcx::EFont::TAHOMA);
    const auto plus_size = pcx::renderer::text::get_size("+", pcx::EFont::TAHOMA);
    const float slider_w = w - attachments_w;
    minus_x = x;
    track_x = x + minus_size.x + 4.f;
    plus_x = x + slider_w - plus_size.x - 2.f;
    track_w = (std::max)(0.f, plus_x - track_x - 4.f);
}

static float slider_value_text_x(float track_x, float track_w, float pct, float val_text_w) {
    const float inner_w = track_w - 3.f;
    const float track_min = track_x + 2.f;
    const float track_max = track_x + track_w - val_text_w - 2.f;
    const float clamped_pct = pcx::clamp(pct, 0.f, 1.f);
    const float anchor_x = track_min + inner_w * clamped_pct;
    return pcx::clamp(anchor_x - val_text_w * 0.5f, track_min, track_max);
}

std::shared_ptr<CCogWheel> CSliderInt::create_cogwheel(const std::string& n) {
    auto cog = std::make_shared<CCogWheel>(n);
    if (!config_key.empty()) cog->config_key = config_key + "." + n;
    attachments.push_back(std::make_shared<CElement>(cog));
    return cog;
}
std::shared_ptr<CTooltip> CSliderInt::create_tooltip(const std::string& desc) {
    auto tip = std::make_shared<CTooltip>(desc);
    attachments.push_back(std::make_shared<CElement>(tip));
    return tip;
}
void CSliderInt::tick() { for (auto& a : attachments) a->tick(); }

void CSliderInt::handle_input(float x, float y, float w) {
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    const float slider_y = y + label_h;
    float minus_x, track_x, track_w, plus_x;
    slider_handle_common(x, y, w, get_attachments_width(), minus_x, track_x, track_w, plus_x, slider_y);
    float right_edge = x + w;
    for (auto& att : attachments) {
        right_edge -= 12.f;
        att->handle_input(right_edge, slider_y, 12.f);
        right_edge -= 4.f;
    }
    const auto minus_size = pcx::renderer::text::get_size("-", pcx::EFont::TAHOMA);
    const auto plus_size = pcx::renderer::text::get_size("+", pcx::EFont::TAHOMA);
    if (pcx::cursor::m1_pressed) {
        if (pcx::cursor::is_hovered({minus_x, slider_y}, {minus_size.x, 12.f}))
            value = static_cast<int>(pcx::clamp(static_cast<float>(value - step), static_cast<float>(min_value), static_cast<float>(max_value)));
        else if (pcx::cursor::is_hovered({plus_x, slider_y}, {plus_size.x, 12.f}))
            value = static_cast<int>(pcx::clamp(static_cast<float>(value + step), static_cast<float>(min_value), static_cast<float>(max_value)));
        else if (pcx::cursor::is_hovered({track_x, slider_y}, {track_w, 12.f}))
            dragging = true;
    }
    if (!pcx::cursor::m1_down) dragging = false;
    if (dragging) {
        const float pct = pcx::clamp((pcx::cursor::x - track_x) / track_w, 0.f, 1.f);
        const float raw = static_cast<float>(min_value) + pct * static_cast<float>(max_value - min_value);
        const int steps = static_cast<int>((raw - static_cast<float>(min_value)) / static_cast<float>(step) + 0.5f);
        value = static_cast<int>(pcx::clamp(static_cast<float>(min_value + steps * step), static_cast<float>(min_value), static_cast<float>(max_value)));
    }
}

void CSliderInt::render(float x, float y, float w) {
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    const float slider_y = y + label_h;
    float minus_x, track_x, track_w, plus_x;
    slider_handle_common(x, y, w, get_attachments_width(), minus_x, track_x, track_w, plus_x, slider_y);
    const auto minus_size = pcx::renderer::text::get_size("-", pcx::EFont::TAHOMA);
    const auto plus_size = pcx::renderer::text::get_size("+", pcx::EFont::TAHOMA);
    pcx::renderer::text::draw_shadowed(name, {x, y}, gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    const bool hover_minus = pcx::cursor::is_hovered({minus_x, slider_y}, {minus_size.x, 12.f});
    const bool hover_plus = pcx::cursor::is_hovered({plus_x, slider_y}, {plus_size.x, 12.f});
    pcx::renderer::text::draw_shadowed("-", {minus_x, slider_y + 1.f}, hover_minus ? gui::colors::scaled(gui::colors::text_active) : gui::colors::scaled(gui::colors::text_inactive), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    pcx::renderer::text::draw_shadowed("+", {plus_x, slider_y + 1.f}, hover_plus ? gui::colors::scaled(gui::colors::text_active) : gui::colors::scaled(gui::colors::text_inactive), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    pcx::renderer::rect::draw_filled({track_x, slider_y}, {track_w, 12.f}, gui::colors::scaled(gui::colors::background_primary));
    pcx::renderer::rect::draw({track_x, slider_y}, {track_w, 12.f}, gui::colors::scaled(gui::colors::border));
    visual_value = pcx::lerp(visual_value, static_cast<float>(value), 0.15f);
    const float range = static_cast<float>(max_value - min_value);
    const float pct = range > 0.f ? pcx::clamp((visual_value - static_cast<float>(min_value)) / range, 0.f, 1.f) : 0.f;
    const float inner_w = track_w - 3.f;
    const float fill_w = inner_w * pct;
    if (fill_w > 0.f)
        pcx::renderer::rect::draw_gradient({track_x + 2.f, slider_y + 2.f}, {fill_w, 8.f},
            gui::colors::scaled(gui::colors::accent_primary), gui::colors::scaled(gui::colors::accent_secondary),
            gui::colors::scaled(gui::colors::accent_primary), gui::colors::scaled(gui::colors::accent_secondary));
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%d%s", value, suffix.c_str());
    const auto val_size = pcx::renderer::text::get_size(buf, pcx::EFont::TAHOMABD);
    const float val_x = slider_value_text_x(track_x, track_w, pct, val_size.x);
    pcx::renderer::text::draw_outlined(buf, {val_x, slider_y + 3.f}, gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMABD);
    float right_edge = x + w;
    for (auto& att : attachments) {
        right_edge -= 12.f;
        att->render(right_edge, slider_y, 12.f);
        right_edge -= 4.f;
    }
}

void CSliderInt::render_popup(float x, float y, float w) {
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    const float slider_y = y + label_h;
    float right_edge = x + w;
    for (auto& att : attachments) {
        right_edge -= 12.f;
        att->render_popup(right_edge, slider_y, 12.f);
        right_edge -= 4.f;
    }
}

float CSliderInt::get_height() const {
    return pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f + 12.f;
}

std::shared_ptr<CCogWheel> CSliderFloat::create_cogwheel(const std::string& n) {
    auto cog = std::make_shared<CCogWheel>(n);
    if (!config_key.empty()) cog->config_key = config_key + "." + n;
    attachments.push_back(std::make_shared<CElement>(cog));
    return cog;
}
std::shared_ptr<CTooltip> CSliderFloat::create_tooltip(const std::string& desc) {
    auto tip = std::make_shared<CTooltip>(desc);
    attachments.push_back(std::make_shared<CElement>(tip));
    return tip;
}
void CSliderFloat::tick() { for (auto& a : attachments) a->tick(); }

void CSliderFloat::handle_input(float x, float y, float w) {
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    const float slider_y = y + label_h;
    float minus_x, track_x, track_w, plus_x;
    slider_handle_common(x, y, w, get_attachments_width(), minus_x, track_x, track_w, plus_x, slider_y);
    float right_edge = x + w;
    for (auto& att : attachments) {
        right_edge -= 12.f;
        att->handle_input(right_edge, slider_y, 12.f);
        right_edge -= 4.f;
    }
    const auto minus_size = pcx::renderer::text::get_size("-", pcx::EFont::TAHOMA);
    const auto plus_size = pcx::renderer::text::get_size("+", pcx::EFont::TAHOMA);
    if (pcx::cursor::m1_pressed) {
        if (pcx::cursor::is_hovered({minus_x, slider_y}, {minus_size.x, 12.f}))
            value = pcx::clamp(snap(value - step), min_value, max_value);
        else if (pcx::cursor::is_hovered({plus_x, slider_y}, {plus_size.x, 12.f}))
            value = pcx::clamp(snap(value + step), min_value, max_value);
        else if (pcx::cursor::is_hovered({track_x, slider_y}, {track_w, 12.f}))
            dragging = true;
    }
    if (!pcx::cursor::m1_down) dragging = false;
    if (dragging) {
        const float pct = pcx::clamp((pcx::cursor::x - track_x) / track_w, 0.f, 1.f);
        value = pcx::clamp(snap(min_value + pct * (max_value - min_value)), min_value, max_value);
    }
}

void CSliderFloat::render(float x, float y, float w) {
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    const float slider_y = y + label_h;
    float minus_x, track_x, track_w, plus_x;
    slider_handle_common(x, y, w, get_attachments_width(), minus_x, track_x, track_w, plus_x, slider_y);
    const auto minus_size = pcx::renderer::text::get_size("-", pcx::EFont::TAHOMA);
    const auto plus_size = pcx::renderer::text::get_size("+", pcx::EFont::TAHOMA);
    pcx::renderer::text::draw_shadowed(name, {x, y}, gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    const bool hover_minus = pcx::cursor::is_hovered({minus_x, slider_y}, {minus_size.x, 12.f});
    const bool hover_plus = pcx::cursor::is_hovered({plus_x, slider_y}, {plus_size.x, 12.f});
    pcx::renderer::text::draw_shadowed("-", {minus_x, slider_y + 1.f}, hover_minus ? gui::colors::scaled(gui::colors::text_active) : gui::colors::scaled(gui::colors::text_inactive), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    pcx::renderer::text::draw_shadowed("+", {plus_x, slider_y + 1.f}, hover_plus ? gui::colors::scaled(gui::colors::text_active) : gui::colors::scaled(gui::colors::text_inactive), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    pcx::renderer::rect::draw_filled({track_x, slider_y}, {track_w, 12.f}, gui::colors::scaled(gui::colors::background_primary));
    pcx::renderer::rect::draw({track_x, slider_y}, {track_w, 12.f}, gui::colors::scaled(gui::colors::border));
    visual_value = pcx::lerp(visual_value, value, 0.15f);
    const float range = max_value - min_value;
    const float pct = range > 0.f ? pcx::clamp((visual_value - min_value) / range, 0.f, 1.f) : 0.f;
    const float inner_w = track_w - 3.f;
    const float fill_w = inner_w * pct;
    if (fill_w > 0.f)
        pcx::renderer::rect::draw_gradient({track_x + 2.f, slider_y + 2.f}, {fill_w, 8.f},
            gui::colors::scaled(gui::colors::accent_primary), gui::colors::scaled(gui::colors::accent_secondary),
            gui::colors::scaled(gui::colors::accent_primary), gui::colors::scaled(gui::colors::accent_secondary));
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.*f%s", precision, static_cast<double>(value), suffix.c_str());
    const auto val_size = pcx::renderer::text::get_size(buf, pcx::EFont::TAHOMABD);
    const float val_x = slider_value_text_x(track_x, track_w, pct, val_size.x);
    pcx::renderer::text::draw_outlined(buf, {val_x, slider_y + 3.f}, gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMABD);
    float right_edge = x + w;
    for (auto& att : attachments) {
        right_edge -= 12.f;
        att->render(right_edge, slider_y, 12.f);
        right_edge -= 4.f;
    }
}

void CSliderFloat::render_popup(float x, float y, float w) {
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    float right_edge = x + w;
    for (auto& att : attachments) {
        right_edge -= 12.f;
        att->render_popup(right_edge, y + label_h, 12.f);
        right_edge -= 4.f;
    }
}

float CSliderFloat::get_height() const {
    return pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f + 12.f;
}

// ===========================================================================
// CDropdown / CMultiSelect / CTextInput / CListBox / CCogWheel
// ===========================================================================
void CDropdown::handle_input(float x, float y, float w) {
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    const float dropdown_y = y + label_h;
    const float dropdown_h = 20.f;
    if (pcx::cursor::m1_pressed && pcx::cursor::is_hovered({x, dropdown_y}, {w, dropdown_h}))
        is_open = !is_open;
    if (!is_open) return;
    const float list_y = dropdown_y + dropdown_h + 2.f;
    const float option_h = 20.f;
    const float list_h = static_cast<float>(options.size()) * option_h;
    for (size_t i = 0; i < options.size(); ++i) {
        const float option_y = list_y + (i * option_h);
        if (pcx::cursor::m1_pressed && pcx::cursor::is_hovered({x, option_y}, {w, option_h})) {
            selected = static_cast<int>(i);
            is_open = false;
            return;
        }
    }
    const bool hover_box = pcx::cursor::is_hovered({x, dropdown_y}, {w, dropdown_h});
    const bool hover_list = pcx::cursor::is_hovered({x, list_y}, {w, list_h});
    if (pcx::cursor::m1_pressed && !hover_box && !hover_list) is_open = false;
}

void CDropdown::render(float x, float y, float w) {
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    const float dropdown_y = y + label_h;
    const float dropdown_h = 20.f;
    pcx::renderer::text::draw_shadowed(name, {x, y}, gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    pcx::renderer::rect::draw_filled({x, dropdown_y}, {w, dropdown_h}, gui::colors::scaled(gui::colors::background_primary));
    pcx::renderer::rect::draw({x, dropdown_y}, {w, dropdown_h}, gui::colors::scaled(gui::colors::border));
    if (selected >= 0 && selected < static_cast<int>(options.size())) {
        const auto ts = pcx::renderer::text::get_size(options[selected], pcx::EFont::TAHOMA);
        pcx::renderer::text::draw_shadowed(options[selected], {x + 6.f, dropdown_y + (dropdown_h - ts.y) / 2.f},
            gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    }
    const float bx = x + w - 16.f, by = dropdown_y + 6.f;
    pcx::renderer::line::draw({bx, by}, {bx + 8.f, by}, gui::colors::scaled(gui::colors::text_inactive));
    pcx::renderer::line::draw({bx, by + 4.f}, {bx + 8.f, by + 4.f}, gui::colors::scaled(gui::colors::text_inactive));
    pcx::renderer::line::draw({bx, by + 8.f}, {bx + 8.f, by + 8.f}, gui::colors::scaled(gui::colors::text_inactive));
}

void CDropdown::render_popup(float x, float y, float w, float max_y) {
    if (!is_open) return;
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    const float dropdown_y = y + label_h;
    const float dropdown_h = 20.f;
    const float list_y = dropdown_y + dropdown_h + 2.f;
    const float option_h = 20.f;
    float list_h = static_cast<float>(options.size()) * option_h;
    if (max_y > 0.f && list_y + list_h > max_y) list_h = max_y - list_y;
    if (list_h <= 0.f) return;
    pcx::renderer::rect::draw_filled({x, list_y}, {w, list_h}, gui::colors::scaled(gui::colors::background_secondary));
    pcx::renderer::rect::draw({x, list_y}, {w, list_h}, gui::colors::scaled(gui::colors::border));
    pcx::renderer::clip_push(static_cast<int>(x), static_cast<int>(list_y), static_cast<int>(w), static_cast<int>(list_h));
    for (size_t i = 0; i < options.size(); ++i) {
        const float option_y = list_y + (i * option_h);
        const bool hovering = pcx::cursor::is_hovered({x, option_y}, {w, option_h});
        const bool is_sel = static_cast<int>(i) == selected;
        if (hovering) pcx::renderer::rect::draw_filled({x + 2.f, option_y + 1.f}, {w - 4.f, option_h - 2.f}, gui::colors::scaled(gui::colors::background_primary));
        if (is_sel) pcx::renderer::line::draw({x + 2.f, option_y + 2.f}, {x + 2.f, option_y + option_h - 2.f}, gui::colors::scaled(gui::colors::accent_primary));
        pcx::Color col = gui::colors::scaled(gui::colors::text_inactive);
        if (is_sel) col = gui::colors::scaled(gui::colors::accent_primary);
        else if (hovering) col = gui::colors::scaled(gui::colors::text_active);
        pcx::renderer::text::draw_shadowed(options[i], {x + 8.f, option_y + 4.f}, col, gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    }
    pcx::renderer::clip_pop();
}

float CDropdown::get_height() const {
    return pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f + 20.f;
}

std::vector<int> CMultiSelect::get_selected() const {
    std::vector<int> r;
    for (size_t i = 0; i < selected.size(); ++i)
        if (selected[i]) r.push_back(static_cast<int>(i));
    return r;
}

std::vector<std::string> CMultiSelect::get_selected_strings() const {
    std::vector<std::string> r;
    for (size_t i = 0; i < selected.size(); ++i)
        if (selected[i]) r.push_back(options[i]);
    return r;
}

std::string CMultiSelect::get_display_text() const {
    auto active = get_selected_strings();
    if (active.empty()) return "none";
    std::string result;
    for (size_t i = 0; i < active.size(); ++i) {
        if (i > 0) result += ", ";
        result += active[i];
    }
    return result;
}

void CMultiSelect::handle_input(float x, float y, float w) {
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    const float dropdown_y = y + label_h;
    const float dropdown_h = 20.f;
    if (pcx::cursor::m1_pressed && pcx::cursor::is_hovered({x, dropdown_y}, {w, dropdown_h}))
        is_open = !is_open;
    if (!is_open) return;
    const float list_y = dropdown_y + dropdown_h + 2.f;
    const float option_h = 20.f;
    const float list_h = static_cast<float>(options.size()) * option_h;
    for (size_t i = 0; i < options.size(); ++i) {
        const float option_y = list_y + (i * option_h);
        if (pcx::cursor::m1_pressed && pcx::cursor::is_hovered({x, option_y}, {w, option_h})) {
            selected[i] = !selected[i];
            return;
        }
    }
    const bool hover_box = pcx::cursor::is_hovered({x, dropdown_y}, {w, dropdown_h});
    const bool hover_list = pcx::cursor::is_hovered({x, list_y}, {w, list_h});
    if (pcx::cursor::m1_pressed && !hover_box && !hover_list) is_open = false;
}

void CMultiSelect::render(float x, float y, float w) {
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    const float dropdown_y = y + label_h;
    const float dropdown_h = 20.f;
    pcx::renderer::text::draw_shadowed(name, {x, y}, gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    pcx::renderer::rect::draw_filled({x, dropdown_y}, {w, dropdown_h}, gui::colors::scaled(gui::colors::background_primary));
    pcx::renderer::rect::draw({x, dropdown_y}, {w, dropdown_h}, gui::colors::scaled(gui::colors::border));
    const std::string display = get_display_text();
    const auto display_size = pcx::renderer::text::get_size(display, pcx::EFont::TAHOMA);
    pcx::renderer::clip_push(static_cast<int>(x + 6), static_cast<int>(dropdown_y), static_cast<int>(w - 28), static_cast<int>(dropdown_h));
    pcx::renderer::text::draw_shadowed(display, {x + 6.f, dropdown_y + (dropdown_h - display_size.y) / 2.f},
        gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    pcx::renderer::clip_pop();
    const float bx = x + w - 16.f, by = dropdown_y + 6.f;
    pcx::renderer::line::draw({bx, by}, {bx + 8.f, by}, gui::colors::scaled(gui::colors::text_inactive));
    pcx::renderer::line::draw({bx, by + 4.f}, {bx + 8.f, by + 4.f}, gui::colors::scaled(gui::colors::text_inactive));
    pcx::renderer::line::draw({bx, by + 8.f}, {bx + 8.f, by + 8.f}, gui::colors::scaled(gui::colors::text_inactive));
}

void CMultiSelect::render_popup(float x, float y, float w, float max_y) {
    if (!is_open) return;
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    const float dropdown_y = y + label_h;
    const float dropdown_h = 20.f;
    const float list_y = dropdown_y + dropdown_h + 2.f;
    const float option_h = 20.f;
    float list_h = static_cast<float>(options.size()) * option_h;
    if (max_y > 0.f && list_y + list_h > max_y) list_h = max_y - list_y;
    if (list_h <= 0.f) return;
    pcx::renderer::rect::draw_filled({x, list_y}, {w, list_h}, gui::colors::scaled(gui::colors::background_secondary));
    pcx::renderer::rect::draw({x, list_y}, {w, list_h}, gui::colors::scaled(gui::colors::border));
    pcx::renderer::clip_push(static_cast<int>(x), static_cast<int>(list_y), static_cast<int>(w), static_cast<int>(list_h));
    for (size_t i = 0; i < options.size(); ++i) {
        const float option_y = list_y + (i * option_h);
        const bool hovering = pcx::cursor::is_hovered({x, option_y}, {w, option_h});
        const bool is_sel = selected[i];
        if (hovering) pcx::renderer::rect::draw_filled({x + 2.f, option_y + 1.f}, {w - 4.f, option_h - 2.f}, gui::colors::scaled(gui::colors::background_primary));
        if (is_sel) pcx::renderer::line::draw({x + 2.f, option_y + 2.f}, {x + 2.f, option_y + option_h - 2.f}, gui::colors::scaled(gui::colors::accent_primary));
        pcx::Color col = gui::colors::scaled(gui::colors::text_inactive);
        if (is_sel) col = gui::colors::scaled(gui::colors::accent_primary);
        else if (hovering) col = gui::colors::scaled(gui::colors::text_active);
        pcx::renderer::text::draw_shadowed(options[i], {x + 8.f, option_y + 4.f}, col, gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    }
    pcx::renderer::clip_pop();
}

float CMultiSelect::get_height() const {
    return pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f + 20.f;
}

void CTextInput::handle_input(float x, float y, float w) {
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    const float box_y = y + label_h;
    const float box_h = 20.f;
    if (pcx::cursor::m1_pressed) {
        if (pcx::cursor::is_hovered({x, box_y}, {w, box_h})) focused = true;
        else focused = false;
    }
    if (!focused) return;
    if (pcx::input::key_singlepress(0x1B) || pcx::input::key_singlepress(0x0D)) { focused = false; return; }
    if (pcx::input::key_fired(0x08)) {
        if (!value.empty()) value.pop_back();
        return;
    }
    if (ImGui::GetCurrentContext()) {
        ImGuiIO& io = ImGui::GetIO();
        for (ImWchar c : io.InputQueueCharacters) {
            if (c >= 32 && c < 127 && static_cast<int>(value.size()) < max_length)
                value.push_back(static_cast<char>(c));
        }
        io.InputQueueCharacters.resize(0);
    }
}

void CTextInput::render(float x, float y, float w) {
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    const float box_y = y + label_h;
    const float box_h = 20.f;
    pcx::renderer::text::draw_shadowed(name, {x, y}, gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    pcx::renderer::rect::draw_filled({x, box_y}, {w, box_h}, gui::colors::scaled(gui::colors::background_primary));
    pcx::renderer::rect::draw({x, box_y}, {w, box_h}, focused ? gui::colors::scaled(gui::colors::accent_primary) : gui::colors::scaled(gui::colors::border));
    pcx::renderer::clip_push(static_cast<int>(x + 4), static_cast<int>(box_y), static_cast<int>(w - 8), static_cast<int>(box_h));
    std::string display = value;
    pcx::Color text_col = gui::colors::scaled(gui::colors::text_active);
    if (value.empty() && !focused) { display = placeholder; text_col = gui::colors::scaled(gui::colors::text_inactive); }
    const auto text_size = pcx::renderer::text::get_size(display, pcx::EFont::TAHOMA);
    pcx::renderer::text::draw_shadowed(display, {x + 6.f, box_y + (box_h - text_size.y) / 2.f}, text_col, gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    if (focused) {
        const auto val_size = pcx::renderer::text::get_size(value, pcx::EFont::TAHOMA);
        pcx::renderer::line::draw({x + 6.f + val_size.x + 1.f, box_y + 4.f}, {x + 6.f + val_size.x + 1.f, box_y + box_h - 4.f}, gui::colors::scaled(gui::colors::text_active));
    }
    pcx::renderer::clip_pop();
}

float CTextInput::get_height() const {
    return pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f + 20.f;
}

void CListBox::add_item(const std::string& item, const std::string& desc) {
    items.push_back({item, desc});
    selected.push_back(false);
}

void CListBox::clear() {
    items.clear();
    selected.clear();
    scroll_offset = 0.f;
}

bool CListBox::get(int i) const {
    return (i >= 0 && i < static_cast<int>(selected.size())) ? selected[i] : false;
}

std::vector<int> CListBox::get_selected() const {
    std::vector<int> r;
    for (size_t i = 0; i < selected.size(); ++i)
        if (selected[i]) r.push_back(static_cast<int>(i));
    return r;
}

std::vector<std::string> CListBox::get_selected_strings() const {
    std::vector<std::string> r;
    for (size_t i = 0; i < selected.size(); ++i)
        if (selected[i]) r.push_back(items[i].name);
    return r;
}

bool CListBox::matches_search(size_t index) const {
    if (search_text.empty()) return true;
    if (search_text.size() > items[index].name.size()) return false;
    const std::string& lower_name = items[index].name;
    const std::string& lower_search = search_text;
    for (size_t i = 0; i + lower_search.size() <= lower_name.size(); ++i) {
        bool match = true;
        for (size_t j = 0; j < lower_search.size(); ++j) {
            char a = lower_name[i + j], b = lower_search[j];
            if (a >= 'A' && a <= 'Z') a += 32;
            if (b >= 'A' && b <= 'Z') b += 32;
            if (a != b) { match = false; break; }
        }
        if (match) return true;
    }
    return false;
}

float CListBox::get_item_height(size_t index) const {
    if (!items[index].description.empty())
        return pcx::renderer::text::get_size(items[index].name, pcx::EFont::TAHOMA).y +
               pcx::renderer::text::get_size(items[index].description, pcx::EFont::TAHOMA).y + 6.f;
    return pcx::renderer::text::get_size(items[index].name, pcx::EFont::TAHOMA).y + 6.f;
}

float CListBox::get_filtered_list_height() const {
    float total = 0.f;
    for (size_t i = 0; i < items.size(); ++i)
        if (matches_search(i)) total += get_item_height(i);
    return total;
}

void CListBox::handle_input(float x, float y, float w) {
    float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    float current_y = y + label_h;
    if (searchable) {
        const float search_h = 20.f;
        if (pcx::cursor::m1_pressed) {
            if (pcx::cursor::is_hovered({x, current_y}, {w, search_h})) search_focused = true;
            else search_focused = false;
        }
        if (search_focused) {
            if (pcx::input::key_singlepress(0x1B)) search_focused = false;
            else if (pcx::input::key_fired(0x08)) {
                if (!search_text.empty()) search_text.pop_back();
                scroll_offset = 0.f;
            } else if (ImGui::GetCurrentContext()) {
                ImGuiIO& io = ImGui::GetIO();
                for (ImWchar c : io.InputQueueCharacters) {
                    if (c >= 32 && c < 127) { search_text.push_back(static_cast<char>(c)); scroll_offset = 0.f; }
                }
                io.InputQueueCharacters.resize(0);
            }
        }
        current_y += search_h + 4.f;
    }
    const float list_y = current_y;
    const float list_h = get_filtered_list_height();
    const float actual_h = list_h < max_height ? list_h : max_height;
    const float max_scroll = list_h > actual_h ? list_h - actual_h : 0.f;
    scroll_offset = pcx::clamp(scroll_offset, 0.f, max_scroll);
    if (pcx::cursor::is_hovered({x, list_y}, {w, actual_h}) && list_h > actual_h) {
        const float delta = pcx::input::get_scroll_delta();
        if (delta != 0.f) scroll_offset = pcx::clamp(scroll_offset - (delta > 0.f ? 20.f : -20.f), 0.f, max_scroll);
    }
    if (pcx::cursor::m1_pressed) {
        float item_y = list_y - scroll_offset;
        for (size_t i = 0; i < items.size(); ++i) {
            if (!matches_search(i)) continue;
            const float ih = get_item_height(i);
            if (pcx::cursor::is_hovered({x, item_y}, {w, ih}) && pcx::cursor::y >= list_y && pcx::cursor::y <= list_y + actual_h) {
                if (multiselect) selected[i] = !selected[i];
                else {
                    std::fill(selected.begin(), selected.end(), false);
                    selected[i] = true;
                }
                break;
            }
            item_y += ih;
        }
    }
}

void CListBox::render(float x, float y, float w) {
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    float current_y = y + label_h;
    pcx::renderer::text::draw_shadowed(name, {x, y}, gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    if (searchable) {
        const float search_h = 20.f;
        pcx::renderer::rect::draw_filled({x, current_y}, {w, search_h}, gui::colors::scaled(gui::colors::background_primary));
        pcx::renderer::rect::draw({x, current_y}, {w, search_h}, search_focused ? gui::colors::scaled(gui::colors::accent_primary) : gui::colors::scaled(gui::colors::border));
        std::string display = search_text;
        pcx::Color text_col = gui::colors::scaled(gui::colors::text_active);
        if (search_text.empty() && !search_focused) { display = "search..."; text_col = gui::colors::scaled(gui::colors::text_inactive); }
        const auto display_size = pcx::renderer::text::get_size(display, pcx::EFont::TAHOMA);
        pcx::renderer::clip_push(static_cast<int>(x + 4), static_cast<int>(current_y), static_cast<int>(w - 8), static_cast<int>(search_h));
        pcx::renderer::text::draw_shadowed(display, {x + 6.f, current_y + (search_h - display_size.y) / 2.f}, text_col, gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
        if (search_focused) {
            const auto val_size = pcx::renderer::text::get_size(search_text, pcx::EFont::TAHOMA);
            pcx::renderer::line::draw({x + 6.f + val_size.x + 1.f, current_y + 4.f}, {x + 6.f + val_size.x + 1.f, current_y + search_h - 4.f}, gui::colors::scaled(gui::colors::text_active));
        }
        pcx::renderer::clip_pop();
        current_y += search_h + 4.f;
    }
    const float list_y = current_y;
    const float list_h = get_filtered_list_height();
    const float actual_h = list_h > 0.f ? (list_h < max_height ? list_h : max_height) : 20.f;
    pcx::renderer::rect::draw_filled({x, list_y}, {w, actual_h}, gui::colors::scaled(gui::colors::background_primary));
    pcx::renderer::rect::draw({x, list_y}, {w, actual_h}, gui::colors::scaled(gui::colors::border));
    if (list_h <= 0.f) return;
    pcx::renderer::clip_push(static_cast<int>(x), static_cast<int>(list_y), static_cast<int>(w), static_cast<int>(actual_h));
    float item_y = list_y - scroll_offset;
    for (size_t i = 0; i < items.size(); ++i) {
        if (!matches_search(i)) continue;
        const float ih = get_item_height(i);
        const bool hovering = pcx::cursor::is_hovered({x, item_y}, {w, ih}) && pcx::cursor::y >= list_y && pcx::cursor::y <= list_y + actual_h;
        const bool is_sel = selected[i];
        if (hovering) pcx::renderer::rect::draw_filled({x + 2.f, item_y + 1.f}, {w - 4.f, ih - 2.f}, gui::colors::scaled(gui::colors::background_secondary));
        if (is_sel) pcx::renderer::line::draw({x + 2.f, item_y + 2.f}, {x + 2.f, item_y + ih - 2.f}, gui::colors::scaled(gui::colors::accent_primary));
        pcx::Color name_col = gui::colors::scaled(gui::colors::text_inactive);
        if (is_sel) name_col = gui::colors::scaled(gui::colors::accent_primary);
        else if (hovering) name_col = gui::colors::scaled(gui::colors::text_active);
        pcx::renderer::text::draw_shadowed(items[i].name, {x + 8.f, item_y + 3.f}, name_col, gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
        if (!items[i].description.empty()) {
            const float name_h = pcx::renderer::text::get_size(items[i].name, pcx::EFont::TAHOMA).y;
            pcx::Color desc_col = is_sel ? gui::colors::scaled(gui::colors::text_active) : gui::colors::scaled(gui::colors::text_inactive);
            pcx::renderer::text::draw_shadowed(items[i].description, {x + 8.f, item_y + 3.f + name_h}, desc_col, gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
        }
        item_y += ih;
    }
    pcx::renderer::clip_pop();
    if (list_h > actual_h) {
        const float bar_h = (actual_h / list_h) * actual_h;
        const float bar_y = list_y + (scroll_offset / list_h) * actual_h;
        pcx::renderer::rect::draw_filled({x + w - 4.f, bar_y}, {2.f, bar_h}, gui::colors::scaled(gui::colors::border));
    }
}

float CListBox::get_height() const {
    const float label_h = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA).y + 2.f;
    const float list_h = get_filtered_list_height();
    const float actual_h = list_h > 0.f ? (list_h < max_height ? list_h : max_height) : 20.f;
    float total = label_h + actual_h;
    if (searchable) total += 24.f;
    return total;
}

// CCogWheel factory methods and logic

std::shared_ptr<CCheckbox> CCogWheel::create_checkbox(const std::string& n, bool def) {
    auto cb = std::make_shared<CCheckbox>(n, def);
    if (!config_key.empty()) cb->config_key = config_key + "." + n;
    elements.push_back(std::make_shared<CElement>(cb));
    if (!config_key.empty()) gui::config_system::register_element(config_key + "." + n, elements.back());
    return cb;
}

std::shared_ptr<CSliderInt> CCogWheel::create_slider_int(const std::string& n, int min, int max, int def, int step, const std::string& suffix) {
    auto s = std::make_shared<CSliderInt>(n, min, max, def, step, suffix);
    if (!config_key.empty()) s->config_key = config_key + "." + n;
    elements.push_back(std::make_shared<CElement>(s));
    if (!config_key.empty()) gui::config_system::register_element(config_key + "." + n, elements.back());
    return s;
}

std::shared_ptr<CSliderFloat> CCogWheel::create_slider_float(const std::string& n, float min, float max, float def, float step, int prec, const std::string& suffix) {
    auto s = std::make_shared<CSliderFloat>(n, min, max, def, step, prec, suffix);
    if (!config_key.empty()) s->config_key = config_key + "." + n;
    elements.push_back(std::make_shared<CElement>(s));
    if (!config_key.empty()) gui::config_system::register_element(config_key + "." + n, elements.back());
    return s;
}

std::shared_ptr<CKeybind> CCogWheel::create_keybind(const std::string& n, int key, pcx::EKeybindMode mode) {
    auto kb = std::make_shared<CKeybind>(n, key, mode);
    elements.push_back(std::make_shared<CElement>(kb));
    if (!config_key.empty()) gui::config_system::register_element(config_key + "." + n, elements.back());
    return kb;
}

std::shared_ptr<CDropdown> CCogWheel::create_dropdown(const std::string& n, const std::vector<std::string>& opts, int def) {
    auto d = std::make_shared<CDropdown>(n, opts, def);
    elements.push_back(std::make_shared<CElement>(d));
    if (!config_key.empty()) gui::config_system::register_element(config_key + "." + n, elements.back());
    return d;
}

std::shared_ptr<CMultiSelect> CCogWheel::create_multiselect(const std::string& n, const std::vector<std::string>& opts) {
    auto m = std::make_shared<CMultiSelect>(n, opts);
    elements.push_back(std::make_shared<CElement>(m));
    if (!config_key.empty()) gui::config_system::register_element(config_key + "." + n, elements.back());
    return m;
}

std::shared_ptr<CColorPicker> CCogWheel::create_colorpicker(const std::string& n, pcx::Color def) {
    auto cp = std::make_shared<CColorPicker>(n, def);
    elements.push_back(std::make_shared<CElement>(cp));
    if (!config_key.empty()) gui::config_system::register_element(config_key + "." + n, elements.back());
    return cp;
}

std::shared_ptr<CButton> CCogWheel::create_button(const std::string& n, pcx::ButtonFn cb) {
    auto b = std::make_shared<CButton>(n, std::move(cb));
    elements.push_back(std::make_shared<CElement>(b));
    return b;
}

std::shared_ptr<CTextInput> CCogWheel::create_text_input(const std::string& n, const std::string& def, const std::string& ph, int max_len) {
    auto t = std::make_shared<CTextInput>(n, def, ph, max_len);
    elements.push_back(std::make_shared<CElement>(t));
    if (!config_key.empty()) gui::config_system::register_element(config_key + "." + n, elements.back());
    return t;
}

std::shared_ptr<CLabel> CCogWheel::create_label(const std::string& n) {
    auto l = std::make_shared<CLabel>(n);
    elements.push_back(std::make_shared<CElement>(l));
    return l;
}

float CCogWheel::get_content_height() const {
    float total = 0.f;
    bool first = true;
    for (const auto& el : elements) {
        if (!el->is_visible()) continue;
        if (!first) total += 8.f;
        total += el->get_height();
        first = false;
    }
    return total;
}

void CCogWheel::tick() { for (auto& e : elements) e->tick(); }

void CCogWheel::handle_input(float x, float y, float) {
    if (pcx::cursor::m1_pressed && pcx::cursor::is_hovered({x, y}, {12.f, 12.f}))
        is_open = !is_open;
    if (!is_open) return;
    const float popup_w = 180.f;
    const float content_h = get_content_height();
    const float max_content_h = 200.f;
    const float actual_h = content_h < max_content_h ? content_h : max_content_h;
    const float popup_h = actual_h + 26.f + 6.f;
    const float popup_x = x + 12.f + 2.f;
    const float popup_y = y;
    const float element_x = popup_x + 6.f;
    const float element_y = popup_y + 20.f;
    const float element_w = popup_w - 12.f;
    const float max_scroll = content_h > actual_h ? content_h - actual_h : 0.f;
    scroll_offset = pcx::clamp(scroll_offset, 0.f, max_scroll);
    if (pcx::cursor::is_hovered({popup_x, element_y}, {popup_w, actual_h}) && content_h > actual_h) {
        const float delta = pcx::input::get_scroll_delta();
        if (delta != 0.f) scroll_offset = pcx::clamp(scroll_offset - (delta > 0.f ? 20.f : -20.f), 0.f, max_scroll);
    }
    int blocking_index = -1;
    for (size_t i = 0; i < elements.size(); ++i) {
        if (!elements[i]->is_visible()) continue;
        if (elements[i]->is_blocking()) { blocking_index = static_cast<int>(i); break; }
    }
    float ey = element_y - scroll_offset;
    for (size_t i = 0; i < elements.size(); ++i) {
        if (!elements[i]->is_visible()) continue;
        if (blocking_index == -1 || static_cast<int>(i) <= blocking_index)
            elements[i]->handle_input(element_x, ey, element_w);
        ey += elements[i]->get_height() + 8.f;
    }
    const bool hover_popup = pcx::cursor::is_hovered({popup_x, popup_y}, {popup_w, popup_h});
    if (pcx::cursor::m1_pressed && !pcx::cursor::is_hovered({x, y}, {12.f, 12.f}) && !hover_popup)
        is_open = false;
}

void CCogWheel::render(float x, float y, float) {
    pcx::renderer::bitmap::draw_cogwheel({x, y}, {12.f, 12.f}, pcx::Color{180, 180, 180, pcx::globals::menu_alpha});
}

void CCogWheel::render_popup(float x, float y, float) {
    if (!is_open) return;
    const float popup_w = 180.f;
    const float content_h = get_content_height();
    const float max_content_h = 200.f;
    const float actual_h = content_h < max_content_h ? content_h : max_content_h;
    const float popup_h = actual_h + 26.f + 6.f;
    const float popup_x = x + 12.f + 2.f;
    const float popup_y = y;
    const float element_x = popup_x + 6.f;
    const float element_y = popup_y + 20.f;
    const float element_w = popup_w - 12.f;
    pcx::renderer::rect::draw_filled({popup_x, popup_y}, {popup_w, popup_h}, gui::colors::scaled(gui::colors::background_secondary));
    pcx::renderer::rect::draw({popup_x, popup_y}, {popup_w, popup_h}, gui::colors::scaled(gui::colors::border));
    pcx::renderer::text::draw_shadowed(name, {element_x, popup_y + 4.f}, gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMABD);
    pcx::renderer::clip_push(static_cast<int>(popup_x), static_cast<int>(element_y), static_cast<int>(popup_w), static_cast<int>(actual_h));
    float ey = element_y - scroll_offset;
    for (const auto& el : elements) {
        if (!el->is_visible()) continue;
        el->render(element_x, ey, element_w);
        ey += el->get_height() + 8.f;
    }
    pcx::renderer::clip_pop();
    if (content_h > actual_h) {
        const float bar_h = (actual_h / content_h) * actual_h;
        const float bar_y = element_y + (scroll_offset / content_h) * actual_h;
        pcx::renderer::rect::draw_filled({popup_x + popup_w - 4.f, bar_y}, {2.f, bar_h}, gui::colors::scaled(gui::colors::border));
    }
    ey = element_y - scroll_offset;
    for (const auto& el : elements) {
        if (!el->is_visible()) continue;
        el->render_popup(element_x, ey, element_w);
        ey += el->get_height() + 8.f;
    }
}
