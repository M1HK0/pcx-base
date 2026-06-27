enum EKeybindMode {
    ALWAYS,
    HOLD,
    TOGGLE
}

namespace hotkeys {
    array<CKeybind@> registered;

    void register(CKeybind@ kb) {
        registered.insertLast(kb);
    }
}

class CKeybind {
    string name;
    int key;
    EKeybindMode mode;
    bool is_open = false;
    bool waiting_for_key = false;
    bool waiting_for_release = false;
    bool toggle_state = false;
    bool visible = true;
    bool attached = false;
    string hotkey_name;
    bool show_in_hotkeys = false;
    VisibilityFn@ visibility_fn;
    VisibilityFn@ hotkey_visibility_fn;
    CCheckbox@ parent_checkbox;

    bool hotkey_visible() {
        if (!show_in_hotkeys) return false;
        if (parent_checkbox !is null && !parent_checkbox.get()) return false;
        if (hotkey_visibility_fn !is null && !hotkey_visibility_fn()) return false;
        return true;
    }

    CKeybind() {}

    CKeybind(const string &in _name, int _key = 0, EKeybindMode _mode = EKeybindMode::HOLD, bool _default_state = false) {
        name = _name;
        key = _key;
        mode = _mode;
        toggle_state = _default_state;
    }

    void register_hotkey(const string &in display_name) {
        hotkey_name = display_name;
        show_in_hotkeys = true;
        hotkeys::register(this);
    }

    void register_hotkey(const string &in display_name, VisibilityFn@ hotkey_vis_fn) {
        hotkey_name = display_name;
        show_in_hotkeys = true;
        @hotkey_visibility_fn = hotkey_vis_fn;
        hotkeys::register(this);
    }

    void set_visible(VisibilityFn@ fn) {
        @visibility_fn = fn;
    }

    bool is_visible() {
        if (visibility_fn !is null)
            return visibility_fn();
        return visible;
    }

    bool is_active() {
        if (mode == EKeybindMode::ALWAYS) return true;
        if (mode == EKeybindMode::HOLD) return key != 0 && key_down(key);
        if (mode == EKeybindMode::TOGGLE) return toggle_state;
        return false;
    }

    int get_key() {
        return key;
    }

    EKeybindMode get_mode() {
        return mode;
    }

    string get_key_display() {
        if (key == 0) return "-";
        return get_key_name(key);
    }

    void tick() {
        if (mode == EKeybindMode::TOGGLE && key != 0) {
            if (key_singlepress(key))
                toggle_state = !toggle_state;
        }
    }

    void handle_input(float x, float y, float w) {
        float box_w = 32;
        float box_h = 12;
        float box_x = attached ? x : x + w - box_w;

        if (cursor::m1_pressed && cursor::is_hovered(vector2(box_x, y), vector2(box_w, box_h)))
            is_open = !is_open;

        if (is_open) {
            float popup_w = 120;
            float popup_h = 20 + 24 + 6 + (3 * 18) + 6;
            float popup_x = box_x + box_w + 2;
            float popup_y = y;

            bool hover_popup = cursor::is_hovered(vector2(popup_x, popup_y), vector2(popup_w, popup_h));

            float key_btn_x = popup_x + 6;
            float key_btn_y = popup_y + 20;
            float key_btn_w = popup_w - 12;
            float key_btn_h = 20;

            if (cursor::m1_pressed && cursor::is_hovered(vector2(key_btn_x, key_btn_y), vector2(key_btn_w, key_btn_h))) {
                waiting_for_key = true;
                waiting_for_release = true;
            }

            if (waiting_for_key) {
                if (waiting_for_release) {
                    if (!cursor::m1_down)
                        waiting_for_release = false;
                } else {
                    if (key_singlepress(0x1B)) {
                        key = 0;
                        waiting_for_key = false;
                        toggle_state = false;
                    } else {
                        array<int> keys_down;
                        get_keys_down(keys_down);
                        for (uint i = 0; i < keys_down.length(); i++) {
                            if (keys_down[i] != 0x1B && key_singlepress(keys_down[i])) {
                                key = keys_down[i];
                                waiting_for_key = false;
                                toggle_state = false;
                                break;
                            }
                        }
                    }
                }
            }

            if (!waiting_for_key) {
                float mode_y = key_btn_y + key_btn_h + 6;
                float mode_h = 16;
                float mode_w = popup_w - 12;

                for (int i = 0; i < 3; i++) {
                    float option_y = mode_y + (i * (mode_h + 2));
                    if (cursor::m1_pressed && cursor::is_hovered(vector2(popup_x + 6, option_y), vector2(mode_w, mode_h))) {
                        mode = EKeybindMode(i);
                        toggle_state = false;
                    }
                }

                if (cursor::m1_pressed && !cursor::is_hovered(vector2(box_x, y), vector2(box_w, box_h)) && !hover_popup) {
                    is_open = false;
                    waiting_for_key = false;
                }
            }
        }
    }

    void render(float x, float y, float w) {
        float box_w = 32;
        float box_h = 12;
        float box_x = attached ? x : x + w - box_w;

        if (!attached)
            renderer::text::draw_shadowed(name, vector2(x, y), gui::colors::text_active.scaled(), gui::colors::outline.scaled(), EFont::TAHOMA);

        renderer::rect::draw_filled(vector2(box_x, y), vector2(box_w, box_h), gui::colors::background_primary.scaled());
        renderer::rect::draw(vector2(box_x, y), vector2(box_w, box_h), gui::colors::border.scaled());

        string key_text = get_key_display();
        vector2 key_size = renderer::text::get_size(key_text, EFont::TAHOMA);
        float key_x = box_x + (box_w - key_size.x) / 2;
        float key_y = y + (box_h - key_size.y) / 2;

        color_t key_col = is_open ? gui::colors::accent_primary.scaled() : gui::colors::text_inactive.scaled();
        renderer::text::draw_shadowed(key_text, vector2(key_x, key_y), key_col, gui::colors::outline.scaled(), EFont::TAHOMA);
    }

    void render_popup_pass(float x, float y, float w) {
        if (!is_open) return;
        float box_w = 32;
        float box_x = attached ? x : x + w - box_w;

        float popup_w = 120;
        float popup_h = 20 + 24 + 6 + (3 * 18) + 6;
        float popup_x = box_x + box_w + 2;
        float popup_y = y;

        renderer::rect::draw_filled(vector2(popup_x, popup_y), vector2(popup_w, popup_h), gui::colors::background_secondary.scaled());
        renderer::rect::draw(vector2(popup_x, popup_y), vector2(popup_w, popup_h), gui::colors::border.scaled());

        renderer::text::draw_shadowed(name, vector2(popup_x + 6, popup_y + 4), gui::colors::text_active.scaled(), gui::colors::outline.scaled(), EFont::TAHOMABD);

        float key_btn_x = popup_x + 6;
        float key_btn_y = popup_y + 20;
        float key_btn_w = popup_w - 12;
        float key_btn_h = 20;

        bool hover_key = cursor::is_hovered(vector2(key_btn_x, key_btn_y), vector2(key_btn_w, key_btn_h));

        renderer::rect::draw_filled(vector2(key_btn_x, key_btn_y), vector2(key_btn_w, key_btn_h), gui::colors::background_primary.scaled());
        renderer::rect::draw(vector2(key_btn_x, key_btn_y), vector2(key_btn_w, key_btn_h),
            hover_key ? gui::colors::accent_primary.scaled() : gui::colors::border.scaled());

        string key_display = waiting_for_key ? "..." : get_key_display();
        color_t key_col = waiting_for_key ? gui::colors::accent_primary.scaled() : gui::colors::text_active.scaled();
        vector2 key_size = renderer::text::get_size(key_display, EFont::TAHOMA);
        renderer::text::draw_shadowed(key_display,
            vector2(key_btn_x + (key_btn_w - key_size.x) / 2, key_btn_y + (key_btn_h - key_size.y) / 2),
            key_col, gui::colors::outline.scaled(), EFont::TAHOMA);

        float mode_y = key_btn_y + key_btn_h + 6;
        float mode_h = 16;
        float mode_w = popup_w - 12;
        array<string> modes = {"always on", "on hold", "on toggle"};

        for (uint i = 0; i < modes.length(); i++) {
            float option_y = mode_y + (i * (mode_h + 2));
            bool hovering = cursor::is_hovered(vector2(popup_x + 6, option_y), vector2(mode_w, mode_h));
            bool selected = int(i) == int(mode);

            if (hovering)
                renderer::rect::draw_filled(vector2(popup_x + 6, option_y), vector2(mode_w, mode_h), gui::colors::background_primary.scaled());

            if (selected)
                renderer::line::draw(vector2(popup_x + 6, option_y + 2), vector2(popup_x + 6, option_y + mode_h - 2), gui::colors::accent_primary.scaled());

            color_t col = gui::colors::text_inactive.scaled();
            if (selected)
                col = gui::colors::accent_primary.scaled();
            else if (hovering)
                col = gui::colors::text_active.scaled();

            renderer::text::draw_shadowed(modes[i], vector2(popup_x + 12, option_y + 2), col, gui::colors::outline.scaled(), EFont::TAHOMA);
        }
    }

    float get_height() {
        return 12;
    }
}