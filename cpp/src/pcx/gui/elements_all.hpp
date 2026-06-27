#pragma once

#include "pcx/gui/config.hpp"
#include "pcx/gui/hotkeys.hpp"
#include "pcx/globals.hpp"
#include "pcx/input.hpp"
#include "pcx/math.hpp"
#include "pcx/renderer.hpp"

#include <imgui.h>

#include <algorithm>
#include <cmath>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class CElement;
class CCheckbox;
class CSliderInt;
class CSliderFloat;
class CKeybind;
class CDropdown;
class CMultiSelect;
class CTextInput;
class CButton;
class CColorPicker;
class CCogWheel;
class CListBox;
class CTooltip;
class CLabel;

// ---------------------------------------------------------------------------
// CTooltip
// ---------------------------------------------------------------------------
class CTooltip {
public:
    std::string description;
    bool hovered = false;
    bool visible = true;
    pcx::VisibilityFn visibility_fn;

    explicit CTooltip(const std::string& desc) : description(desc) {}

    void set_visible(pcx::VisibilityFn fn) { visibility_fn = std::move(fn); }

    bool is_visible() const {
        if (visibility_fn) return visibility_fn();
        return visible;
    }

    void handle_input(float x, float y, float) {
        hovered = pcx::cursor::is_hovered({x, y}, {12.f, 12.f});
    }

    void render(float x, float y, float) {
        const auto q_size = pcx::renderer::text::get_size("?", pcx::EFont::TAHOMA);
        const auto icon_col = hovered ? gui::colors::scaled(gui::colors::accent_primary)
                                      : gui::colors::scaled(gui::colors::text_inactive);
        pcx::renderer::circle::draw({x + 6.f, y + 6.f}, 6.f, icon_col, 1.f, false);
        pcx::renderer::text::draw_shadowed("?",
            {(x + (14.f - q_size.x) / 2.f), (y + (12.f - q_size.y) / 2.f)},
            icon_col, gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    }

    void render_popup(float x, float y, float) {
        if (!hovered) return;
        const auto desc_size = pcx::renderer::text::get_size(description, pcx::EFont::TAHOMA);
        const float popup_w = desc_size.x + 12.f;
        const float popup_h = desc_size.y + 8.f;
        const float popup_x = x + 14.f;
        const float popup_y = y;
        pcx::renderer::rect::draw_glow({popup_x, popup_y}, {popup_w, popup_h}, gui::colors::glow, 4.f, 3);
        pcx::renderer::rect::draw_filled_rounded({popup_x, popup_y}, {popup_w, popup_h},
            gui::colors::scaled(gui::colors::background_secondary), 4.f);
        pcx::renderer::text::draw_shadowed(description, {popup_x + 6.f, popup_y + 4.f},
            gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    }

    float get_height() const { return 12.f; }
};

// ---------------------------------------------------------------------------
// CButton
// ---------------------------------------------------------------------------
class CButton {
public:
    std::string name;
    bool visible = true;
    bool hovered = false;
    float click_anim = 0.f;
    pcx::ButtonFn callback;
    pcx::VisibilityFn visibility_fn;

    CButton(const std::string& n, pcx::ButtonFn cb = nullptr) : name(n), callback(std::move(cb)) {}

    void set_visible(pcx::VisibilityFn fn) { visibility_fn = std::move(fn); }

    bool is_visible() const {
        if (visibility_fn) return visibility_fn();
        return visible;
    }

    void handle_input(float x, float y, float w) {
        const float btn_h = 20.f;
        hovered = pcx::cursor::is_hovered({x, y}, {w, btn_h});
        if (pcx::cursor::m1_pressed && hovered) {
            click_anim = 1.f;
            if (callback) callback();
        }
    }

    void render(float x, float y, float w) {
        const float btn_h = 20.f;
        pcx::renderer::rect::draw_filled({x, y}, {w, btn_h}, gui::colors::scaled(gui::colors::background_primary));
        if (click_anim > 0.01f) {
            const uint8_t a = static_cast<uint8_t>(gui::colors::accent_primary.a * click_anim *
                (static_cast<float>(pcx::globals::menu_alpha) / 255.f));
            pcx::Color c1{gui::colors::accent_primary.r, gui::colors::accent_primary.g, gui::colors::accent_primary.b, a};
            pcx::Color c2{gui::colors::accent_secondary.r, gui::colors::accent_secondary.g, gui::colors::accent_secondary.b, a};
            pcx::renderer::rect::draw_gradient({x + 1.f, y + 1.f}, {w - 2.f, btn_h - 2.f}, c1, c2, c1, c2);
            click_anim = pcx::lerp(click_anim, 0.f, 0.04f);
            if (click_anim < 0.01f) click_anim = 0.f;
        }
        pcx::renderer::rect::draw({x, y}, {w, btn_h},
            hovered ? gui::colors::scaled(gui::colors::accent_primary) : gui::colors::scaled(gui::colors::border));
        const auto text_size = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMA);
        pcx::Color col = gui::colors::scaled(gui::colors::text_inactive);
        if (click_anim > 0.01f || hovered) col = gui::colors::scaled(gui::colors::text_active);
        pcx::renderer::text::draw_shadowed(name, {x + (w - text_size.x) / 2.f, y + (btn_h - text_size.y) / 2.f},
            col, gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    }

    float get_height() const { return 20.f; }
};

// ---------------------------------------------------------------------------
// CKeybind
// ---------------------------------------------------------------------------
class CKeybind : public std::enable_shared_from_this<CKeybind> {
public:
    std::string name;
    int key = 0;
    pcx::EKeybindMode mode = pcx::EKeybindMode::HOLD;
    bool is_open = false;
    bool waiting_for_key = false;
    bool waiting_for_release = false;
    bool toggle_state = false;
    bool visible = true;
    bool attached = false;
    std::string hotkey_name;
    bool show_in_hotkeys = false;
    pcx::VisibilityFn visibility_fn;
    pcx::VisibilityFn hotkey_visibility_fn;
    std::weak_ptr<CCheckbox> parent_checkbox;

    CKeybind() = default;

    CKeybind(const std::string& n, int k = 0, pcx::EKeybindMode m = pcx::EKeybindMode::HOLD, bool def = false)
        : name(n), key(k), mode(m), toggle_state(def) {}

    void register_hotkey(const std::string& display) {
        hotkey_name = display;
        show_in_hotkeys = true;
        hotkeys::register_keybind(shared_from_this());
    }

    void register_hotkey(const std::string& display, pcx::VisibilityFn fn) {
        hotkey_name = display;
        show_in_hotkeys = true;
        hotkey_visibility_fn = std::move(fn);
        hotkeys::register_keybind(shared_from_this());
    }

    void set_visible(pcx::VisibilityFn fn) { visibility_fn = std::move(fn); }

    bool is_visible() const {
        if (visibility_fn) return visibility_fn();
        return visible;
    }

    bool hotkey_visible() const {
        if (!show_in_hotkeys) return false;
        if (auto p = parent_checkbox.lock(); p && !p->get()) return false;
        if (hotkey_visibility_fn && !hotkey_visibility_fn()) return false;
        return true;
    }

    bool is_active() const {
        if (mode == pcx::EKeybindMode::ALWAYS) return true;
        if (mode == pcx::EKeybindMode::HOLD) return key != 0 && pcx::input::key_down(key);
        if (mode == pcx::EKeybindMode::TOGGLE) return toggle_state;
        return false;
    }

    int get_key() const { return key; }
    pcx::EKeybindMode get_mode() const { return mode; }

    std::string get_key_display() const {
        return key == 0 ? "-" : pcx::input::get_key_name(key);
    }

    void tick() {
        if (mode == pcx::EKeybindMode::TOGGLE && key != 0) {
            if (pcx::input::key_singlepress(key))
                toggle_state = !toggle_state;
        }
    }

    void handle_input(float x, float y, float w);
    void render(float x, float y, float w);
    void render_popup(float x, float y, float w);
    float get_height() const { return 12.f; }
};

// ---------------------------------------------------------------------------
// CColorPicker
// ---------------------------------------------------------------------------
class CColorPicker {
public:
    std::string name;
    pcx::Color value{255, 255, 255, 255};
    bool is_open = false;
    bool visible = true;
    bool attached = false;
    bool dragging_picker = false;
    bool dragging_hue = false;
    bool dragging_alpha = false;
    float hue = 0.f, sat = 1.f, val = 1.f;
    pcx::VisibilityFn visibility_fn;

    CColorPicker() = default;

    explicit CColorPicker(const std::string& n, pcx::Color def = {255, 255, 255, 255}) : name(n), value(def) {
        rgb_to_hsv(value.r, value.g, value.b, hue, sat, val);
    }

    void set_visible(pcx::VisibilityFn fn) { visibility_fn = std::move(fn); }

    bool is_visible() const {
        if (visibility_fn) return visibility_fn();
        return visible;
    }

    pcx::Color get() const { return value; }

    static void rgb_to_hsv(uint8_t r, uint8_t g, uint8_t b, float& h, float& s, float& v) {
        const float rf = r / 255.f, gf = g / 255.f, bf = b / 255.f;
        const float mx = std::max({rf, gf, bf});
        const float mn = std::min({rf, gf, bf});
        const float delta = mx - mn;
        v = mx;
        s = mx > 0.f ? delta / mx : 0.f;
        if (delta < 0.001f) h = 0.f;
        else if (mx == rf) { h = 60.f * ((gf - bf) / delta); if (h < 0.f) h += 360.f; }
        else if (mx == gf) h = 60.f * (2.f + (bf - rf) / delta);
        else h = 60.f * (4.f + (rf - gf) / delta);
    }

    static void hsv_to_rgb(float h, float s, float v, uint8_t& r, uint8_t& g, uint8_t& b) {
        const float c = v * s;
        const float x = c * (1.f - std::fabs(std::fmod(h / 60.f, 2.f) - 1.f));
        const float m = v - c;
        float rf = 0, gf = 0, bf = 0;
        if (h < 60) { rf = c; gf = x; }
        else if (h < 120) { rf = x; gf = c; }
        else if (h < 180) { gf = c; bf = x; }
        else if (h < 240) { gf = x; bf = c; }
        else if (h < 300) { rf = x; bf = c; }
        else { rf = c; bf = x; }
        r = static_cast<uint8_t>((rf + m) * 255.f);
        g = static_cast<uint8_t>((gf + m) * 255.f);
        b = static_cast<uint8_t>((bf + m) * 255.f);
    }

    pcx::Color hue_to_rgb(float h) const {
        uint8_t r, g, b;
        hsv_to_rgb(h, 1.f, 1.f, r, g, b);
        return {r, g, b, 255};
    }

    void update_from_hsv() { hsv_to_rgb(hue, sat, val, value.r, value.g, value.b); }

    void handle_input(float x, float y, float w);
    void render(float x, float y, float w);
    void render_popup(float x, float y, float w);
    float get_height() const { return 12.f; }
};

// ---------------------------------------------------------------------------
// CCogWheel
// ---------------------------------------------------------------------------
class CCogWheel {
public:
    std::string name;
    std::string config_key;
    bool is_open = false;
    bool visible = true;
    float scroll_offset = 0.f;
    std::vector<std::shared_ptr<CElement>> elements;
    pcx::VisibilityFn visibility_fn;

    explicit CCogWheel(const std::string& n) : name(n) {}

    void set_visible(pcx::VisibilityFn fn) { visibility_fn = std::move(fn); }

    bool is_visible() const {
        if (visibility_fn) return visibility_fn();
        return visible;
    }

    std::shared_ptr<CCheckbox> create_checkbox(const std::string& n, bool def = false);
    std::shared_ptr<CSliderInt> create_slider_int(const std::string& n, int min, int max, int def, int step = 1, const std::string& suffix = "");
    std::shared_ptr<CSliderFloat> create_slider_float(const std::string& n, float min, float max, float def, float step = 0.1f, int precision = 1, const std::string& suffix = "");
    std::shared_ptr<CKeybind> create_keybind(const std::string& n, int key = 0, pcx::EKeybindMode mode = pcx::EKeybindMode::HOLD);
    std::shared_ptr<CDropdown> create_dropdown(const std::string& n, const std::vector<std::string>& opts, int def = 0);
    std::shared_ptr<CMultiSelect> create_multiselect(const std::string& n, const std::vector<std::string>& opts);
    std::shared_ptr<CColorPicker> create_colorpicker(const std::string& n, pcx::Color def = {255, 255, 255, 255});
    std::shared_ptr<CButton> create_button(const std::string& n, pcx::ButtonFn cb = nullptr);
    std::shared_ptr<CTextInput> create_text_input(const std::string& n, const std::string& def = "", const std::string& placeholder = "", int max_len = 128);
    std::shared_ptr<CLabel> create_label(const std::string& n);

    float get_content_height() const;
    void tick();
    void handle_input(float x, float y, float w);
    void render(float x, float y, float w);
    void render_popup(float x, float y, float w);
    float get_height() const { return 12.f; }
};

// ---------------------------------------------------------------------------
// CCheckbox
// ---------------------------------------------------------------------------
class CCheckbox : public std::enable_shared_from_this<CCheckbox> {
public:
    std::string name;
    std::string config_key;
    bool value = false;
    bool visible = true;
    float alpha = 0.f;
    pcx::VisibilityFn visibility_fn;
    std::vector<std::shared_ptr<CElement>> attachments;

    CCheckbox(const std::string& n, bool def = false) : name(n), value(def), alpha(def ? 1.f : 0.f) {}

    void set_visible(pcx::VisibilityFn fn) { visibility_fn = std::move(fn); }

    bool is_visible() const {
        if (visibility_fn) return visibility_fn();
        return visible;
    }

    bool get() const { return value; }

    std::shared_ptr<CKeybind> create_keybind(const std::string& n, int key = 0, pcx::EKeybindMode mode = pcx::EKeybindMode::HOLD);
    std::shared_ptr<CColorPicker> create_colorpicker(const std::string& n, pcx::Color def = {255, 255, 255, 255});
    std::shared_ptr<CCogWheel> create_cogwheel(const std::string& n);
    std::shared_ptr<CTooltip> create_tooltip(const std::string& desc);

    void tick();
    void handle_input(float x, float y, float w);
    void render(float x, float y, float w);
    void render_popup(float x, float y, float w);
    float get_attachment_width(const std::shared_ptr<CElement>& el) const;
    float get_height() const { return 12.f; }
};

// ---------------------------------------------------------------------------
// CLabel
// ---------------------------------------------------------------------------
class CLabel {
public:
    std::string name;
    std::string config_key;
    bool visible = true;
    pcx::VisibilityFn visibility_fn;
    std::vector<std::shared_ptr<CElement>> attachments;

    explicit CLabel(const std::string& n) : name(n) {}

    void set_visible(pcx::VisibilityFn fn) { visibility_fn = std::move(fn); }

    bool is_visible() const {
        if (visibility_fn) return visibility_fn();
        return visible;
    }

    std::shared_ptr<CCogWheel> create_cogwheel(const std::string& n);
    std::shared_ptr<CTooltip> create_tooltip(const std::string& desc);

    void tick();
    void handle_input(float x, float y, float w);
    void render(float x, float y, float w);
    void render_popup(float x, float y, float w);
    float get_attachment_width(const std::shared_ptr<CElement>& el) const;
    float get_height() const { return 12.f; }
};

// ---------------------------------------------------------------------------
// CSliderInt / CSliderFloat
// ---------------------------------------------------------------------------
class CSliderInt {
public:
    std::string name, config_key, suffix;
    int value = 0, min_value = 0, max_value = 100, step = 1;
    float visual_value = 0.f;
    bool dragging = false, visible = true;
    pcx::VisibilityFn visibility_fn;
    std::vector<std::shared_ptr<CElement>> attachments;

    CSliderInt(const std::string& n, int min, int max, int def, int st = 1, const std::string& suf = "")
        : name(n), min_value(min), max_value(max), value(def), step(st), suffix(suf), visual_value(static_cast<float>(def)) {}

    void set_visible(pcx::VisibilityFn fn) { visibility_fn = std::move(fn); }
    bool is_visible() const { return visibility_fn ? visibility_fn() : visible; }
    int get() const { return value; }

    std::shared_ptr<CCogWheel> create_cogwheel(const std::string& n);
    std::shared_ptr<CTooltip> create_tooltip(const std::string& desc);

    void tick();
    float get_attachments_width() const { return static_cast<float>(attachments.size()) * 16.f; }
    void handle_input(float x, float y, float w);
    void render(float x, float y, float w);
    void render_popup(float x, float y, float w);
    float get_height() const;
};

class CSliderFloat {
public:
    std::string name, config_key, suffix;
    float value = 0.f, min_value = 0.f, max_value = 1.f, step = 0.1f;
    int precision = 1;
    float visual_value = 0.f;
    bool dragging = false, visible = true;
    pcx::VisibilityFn visibility_fn;
    std::vector<std::shared_ptr<CElement>> attachments;

    CSliderFloat(const std::string& n, float min, float max, float def, float st = 0.1f, int prec = 1, const std::string& suf = "")
        : name(n), min_value(min), max_value(max), value(def), step(st), precision(prec), suffix(suf), visual_value(def) {}

    void set_visible(pcx::VisibilityFn fn) { visibility_fn = std::move(fn); }
    bool is_visible() const { return visibility_fn ? visibility_fn() : visible; }

    float snap(float v) const {
        const float mult = std::pow(10.f, static_cast<float>(precision));
        return std::floor(v * mult + 0.5f) / mult;
    }

    float get() const { return snap(value); }

    std::shared_ptr<CCogWheel> create_cogwheel(const std::string& n);
    std::shared_ptr<CTooltip> create_tooltip(const std::string& desc);

    void tick();
    float get_attachments_width() const { return static_cast<float>(attachments.size()) * 16.f; }
    void handle_input(float x, float y, float w);
    void render(float x, float y, float w);
    void render_popup(float x, float y, float w);
    float get_height() const;
};

// ---------------------------------------------------------------------------
// CDropdown / CMultiSelect / CTextInput / CListBox
// ---------------------------------------------------------------------------
class CDropdown {
public:
    std::string name;
    std::vector<std::string> options;
    int selected = 0;
    bool is_open = false, visible = true;
    pcx::VisibilityFn visibility_fn;

    CDropdown(const std::string& n, const std::vector<std::string>& opts, int def = 0)
        : name(n), options(opts), selected(def) {}

    void set_visible(pcx::VisibilityFn fn) { visibility_fn = std::move(fn); }
    bool is_visible() const { return visibility_fn ? visibility_fn() : visible; }
    int get() const { return selected; }
    std::string get_string() const {
        return (selected >= 0 && selected < static_cast<int>(options.size())) ? options[selected] : "";
    }

    void handle_input(float x, float y, float w);
    void render(float x, float y, float w);
    void render_popup(float x, float y, float w, float max_y = 0.f);
    float get_height() const;
};

class CMultiSelect {
public:
    std::string name;
    std::vector<std::string> options;
    std::vector<bool> selected;
    bool is_open = false, visible = true;
    pcx::VisibilityFn visibility_fn;

    CMultiSelect(const std::string& n, const std::vector<std::string>& opts) : name(n), options(opts), selected(opts.size(), false) {}

    void set_visible(pcx::VisibilityFn fn) { visibility_fn = std::move(fn); }
    bool is_visible() const { return visibility_fn ? visibility_fn() : visible; }
    bool get(int i) const { return (i >= 0 && i < static_cast<int>(selected.size())) ? selected[i] : false; }

    std::vector<int> get_selected() const;
    std::vector<std::string> get_selected_strings() const;
    std::string get_display_text() const;

    void handle_input(float x, float y, float w);
    void render(float x, float y, float w);
    void render_popup(float x, float y, float w, float max_y = 0.f);
    float get_height() const;
};

class CTextInput {
public:
    std::string name, value, placeholder;
    bool focused = false, visible = true;
    int max_length = 128;
    pcx::VisibilityFn visibility_fn;

    CTextInput(const std::string& n, const std::string& def = "", const std::string& ph = "", int max_len = 128)
        : name(n), value(def), placeholder(ph), max_length(max_len) {}

    void set_visible(pcx::VisibilityFn fn) { visibility_fn = std::move(fn); }
    bool is_visible() const { return visibility_fn ? visibility_fn() : visible; }
    std::string get() const { return value; }

    void handle_input(float x, float y, float w);
    void render(float x, float y, float w);
    float get_height() const;
};

struct CListBoxItem {
    std::string name;
    std::string description;
};

class CListBox {
public:
    std::string name;
    std::vector<CListBoxItem> items;
    std::vector<bool> selected;
    bool visible = true, multiselect = true, searchable = true;
    bool search_focused = false;
    std::string search_text;
    float scroll_offset = 0.f, max_height = 120.f;
    pcx::VisibilityFn visibility_fn;

    CListBox(const std::string& n, bool multi = true, float max_h = 120.f, bool search = true)
        : name(n), multiselect(multi), max_height(max_h), searchable(search) {}

    void add_item(const std::string& item, const std::string& desc = "");
    void clear();
    void set_visible(pcx::VisibilityFn fn) { visibility_fn = std::move(fn); }
    bool is_visible() const { return visibility_fn ? visibility_fn() : visible; }
    bool get(int i) const;
    std::vector<int> get_selected() const;
    std::vector<std::string> get_selected_strings() const;

    bool matches_search(size_t index) const;
    float get_item_height(size_t index) const;
    float get_filtered_list_height() const;

    void handle_input(float x, float y, float w);
    void render(float x, float y, float w);
    float get_height() const;
};

// ---------------------------------------------------------------------------
// CElement
// ---------------------------------------------------------------------------
class CElement {
public:
    pcx::EElementType type{};
    std::shared_ptr<CCheckbox> checkbox;
    std::shared_ptr<CSliderInt> slider_int;
    std::shared_ptr<CSliderFloat> slider_float;
    std::shared_ptr<CKeybind> keybind;
    std::shared_ptr<CDropdown> dropdown;
    std::shared_ptr<CMultiSelect> multiselect;
    std::shared_ptr<CTextInput> text_input;
    std::shared_ptr<CButton> button;
    std::shared_ptr<CColorPicker> colorpicker;
    std::shared_ptr<CCogWheel> cogwheel;
    std::shared_ptr<CListBox> listbox;
    std::shared_ptr<CTooltip> tooltip;
    std::shared_ptr<CLabel> label;

    explicit CElement(const std::shared_ptr<CCheckbox>& e) : type(pcx::EElementType::CHECKBOX), checkbox(e) {}
    explicit CElement(const std::shared_ptr<CSliderInt>& e) : type(pcx::EElementType::SLIDER_INT), slider_int(e) {}
    explicit CElement(const std::shared_ptr<CSliderFloat>& e) : type(pcx::EElementType::SLIDER_FLOAT), slider_float(e) {}
    explicit CElement(const std::shared_ptr<CKeybind>& e) : type(pcx::EElementType::KEYBIND), keybind(e) {}
    explicit CElement(const std::shared_ptr<CDropdown>& e) : type(pcx::EElementType::DROPDOWN), dropdown(e) {}
    explicit CElement(const std::shared_ptr<CMultiSelect>& e) : type(pcx::EElementType::MULTISELECT), multiselect(e) {}
    explicit CElement(const std::shared_ptr<CTextInput>& e) : type(pcx::EElementType::TEXT_INPUT), text_input(e) {}
    explicit CElement(const std::shared_ptr<CButton>& e) : type(pcx::EElementType::BUTTON), button(e) {}
    explicit CElement(const std::shared_ptr<CColorPicker>& e) : type(pcx::EElementType::COLORPICKER), colorpicker(e) {}
    explicit CElement(const std::shared_ptr<CCogWheel>& e) : type(pcx::EElementType::COGWHEEL), cogwheel(e) {}
    explicit CElement(const std::shared_ptr<CListBox>& e) : type(pcx::EElementType::LISTBOX), listbox(e) {}
    explicit CElement(const std::shared_ptr<CTooltip>& e) : type(pcx::EElementType::TOOLTIP), tooltip(e) {}
    explicit CElement(const std::shared_ptr<CLabel>& e) : type(pcx::EElementType::LABEL), label(e) {}

    bool is_visible() const;
    bool is_blocking() const;
    void tick();
    void handle_input(float x, float y, float w);
    void render(float x, float y, float w);
    void render_popup(float x, float y, float w, float max_y = 0.f);
    float get_height() const;
};
