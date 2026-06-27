class CCheckbox {
    string name;
    string config_key;
    bool value;
    bool visible = true;
    float alpha = 0.0f;
    VisibilityFn@ visibility_fn;
    array<CElement@> attachments;

    CCheckbox() {}

    CCheckbox(const string &in _name, bool _default = false) {
        name = _name;
        value = _default;
        alpha = _default ? 1.0f : 0.0f;
    }

    void set_visible(VisibilityFn@ fn) {
        @visibility_fn = fn;
    }

    bool is_visible() {
        if (visibility_fn !is null)
            return visibility_fn();
        return visible;
    }

    bool get() {
        return value;
    }

    CKeybind@ create_keybind(const string &in keybind_name, int default_key = 0, EKeybindMode default_mode = EKeybindMode::HOLD) {
        CKeybind@ keybind = CKeybind(keybind_name, default_key, default_mode);
        keybind.attached = true;
        @keybind.parent_checkbox = this;
        CElement@ el = CElement(keybind);
        attachments.insertLast(el);
        if (config_key.length() > 0)
            gui::config_system::register_element(config_key + "." + keybind_name, el);
        return keybind;
    }

    CColorPicker@ create_colorpicker(const string &in picker_name, color_t default_color = color_t(255, 255, 255, 255)) {
        CColorPicker@ picker = CColorPicker(picker_name, default_color);
        picker.attached = true;
        CElement@ el = CElement(picker);
        attachments.insertLast(el);
        if (config_key.length() > 0)
            gui::config_system::register_element(config_key + "." + picker_name, el);
        return picker;
    }

    CCogWheel@ create_cogwheel(const string &in cog_name) {
        CCogWheel@ cog = CCogWheel(cog_name);
        if (config_key.length() > 0)
            cog.config_key = config_key + "." + cog_name;
        attachments.insertLast(CElement(cog));
        return cog;
    }

    CTooltip@ create_tooltip(const string &in description) {
        CTooltip@ tip = CTooltip(description);
        attachments.insertLast(CElement(tip));
        return tip;
    }

    void tick() {
        for (uint i = 0; i < attachments.length(); i++)
            attachments[i].tick();
    }

    void handle_input(float x, float y, float w) {
        float right_edge = x + w;

        for (uint i = 0; i < attachments.length(); i++) {
            float att_w = get_attachment_width(attachments[i]);
            right_edge -= att_w;
            attachments[i].handle_input(right_edge, y, att_w);
            right_edge -= 4;
        }

        vector2 text_size = renderer::text::get_size(name, EFont::TAHOMA);
        float hitbox_w = 12 + 4 + text_size.x;

        if (cursor::m1_pressed && cursor::is_hovered(vector2(x, y), vector2(hitbox_w, 12)))
            value = !value;
    }

    void render(float x, float y, float w) {
        renderer::rect::draw_filled(vector2(x, y), vector2(12, 12), gui::colors::background_primary.scaled());
        renderer::rect::draw(vector2(x, y), vector2(12, 12), gui::colors::border.scaled());

        float target = value ? 1.0f : 0.0f;
        alpha = lerp(alpha, target, 0.05f);

        if (alpha > 0.01f) {
            uint8 a1 = uint8((gui::colors::accent_primary.a * alpha * globals::menu_alpha) / 255);
            uint8 a2 = uint8((gui::colors::accent_secondary.a * alpha * globals::menu_alpha) / 255);
            color_t c1 = color_t(gui::colors::accent_primary.r, gui::colors::accent_primary.g, gui::colors::accent_primary.b, a1);
            color_t c2 = color_t(gui::colors::accent_secondary.r, gui::colors::accent_secondary.g, gui::colors::accent_secondary.b, a2);
            renderer::rect::draw_gradient(vector2(x + 2, y + 2), vector2(8, 8), c1, c2, c1, c2);
        }

        vector2 text_size = renderer::text::get_size(name, EFont::TAHOMA);
        float text_x = x + 16;
        float text_y = y + (12 - text_size.y) / 2;

        bool hovered = cursor::is_hovered(vector2(x, y), vector2(12 + 4 + text_size.x, 12));

        color_t col = gui::colors::text_inactive.scaled();
        if (value)
            col = gui::colors::text_active.scaled();
        else if (hovered)
            col = gui::colors::text_hover.scaled();

        renderer::text::draw_shadowed(name, vector2(text_x, text_y), col, gui::colors::outline.scaled(), EFont::TAHOMA);

        float right_edge = x + w;

        for (uint i = 0; i < attachments.length(); i++) {
            float att_w = get_attachment_width(attachments[i]);
            right_edge -= att_w;
            attachments[i].render(right_edge, y, att_w);
            right_edge -= 4;
        }
    }

    void render_popup_pass(float x, float y, float w) {
        float right_edge = x + w;

        for (uint i = 0; i < attachments.length(); i++) {
            float att_w = get_attachment_width(attachments[i]);
            right_edge -= att_w;
            attachments[i].render_popup(right_edge, y, att_w);
            right_edge -= 4;
        }
    }

    float get_attachment_width(CElement@ el) {
        if (el.type == EElementType::KEYBIND) return 32;
        if (el.type == EElementType::COLORPICKER) return 12;
        if (el.type == EElementType::COGWHEEL) return 12;
        if (el.type == EElementType::TOOLTIP) return 12;
        return 12;
    }

    float get_height() {
        return 12;
    }
}