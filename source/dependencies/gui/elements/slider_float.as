class CSliderFloat {
    string name;
    string config_key;
    float value;
    float min_value;
    float max_value;
    float step;
    int precision;
    string suffix;
    float visual_value;
    bool dragging = false;
    bool visible = true;
    VisibilityFn@ visibility_fn;
    array<CElement@> attachments;

    CSliderFloat() {}

    CSliderFloat(const string &in _name, float _min, float _max, float _default, float _step = 0.1f, int _precision = 1, const string &in _suffix = "") {
        name = _name;
        min_value = _min;
        max_value = _max;
        value = _default;
        step = _step;
        precision = _precision;
        suffix = _suffix;
        visual_value = _default;
    }

    void set_visible(VisibilityFn@ fn) {
        @visibility_fn = fn;
    }

    bool is_visible() {
        if (visibility_fn !is null)
            return visibility_fn();
        return visible;
    }

    float get() {
        return snap(value);
    }

    float snap(float v) {
        float mult = pow(10, precision);
        return floor(v * mult + 0.5f) / mult;
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

    float get_attachments_width() {
        return attachments.length() * 16;
    }

    void handle_input(float x, float y, float w) {
        float label_h = renderer::text::get_size(name, EFont::TAHOMA).y + 2;
        float slider_y = y + label_h;
        float slider_w = w - get_attachments_width();

        float right_edge = x + w;
        for (uint i = 0; i < attachments.length(); i++) {
            right_edge -= 12;
            attachments[i].handle_input(right_edge, slider_y, 12);
            right_edge -= 4;
        }

        vector2 minus_size = renderer::text::get_size("-", EFont::TAHOMA);
        vector2 plus_size = renderer::text::get_size("+", EFont::TAHOMA);

        float minus_x = x;
        float track_x = x + minus_size.x + 4;
        float track_w = slider_w - minus_size.x - plus_size.x - 8;
        float plus_x = track_x + track_w + 4;

        if (cursor::m1_pressed) {
            if (cursor::is_hovered(vector2(minus_x, slider_y), vector2(minus_size.x, 12)))
                value = clamp(snap(value - step), min_value, max_value);
            else if (cursor::is_hovered(vector2(plus_x, slider_y), vector2(plus_size.x, 12)))
                value = clamp(snap(value + step), min_value, max_value);
            else if (cursor::is_hovered(vector2(track_x, slider_y), vector2(track_w, 12)))
                dragging = true;
        }

        if (!cursor::m1_down)
            dragging = false;

        if (dragging) {
            float pct = clamp((cursor::x - track_x) / track_w, 0.0f, 1.0f);
            float raw = min_value + pct * (max_value - min_value);
            value = clamp(snap(raw), min_value, max_value);
        }
    }

    void render(float x, float y, float w) {
        float label_h = renderer::text::get_size(name, EFont::TAHOMA).y + 2;
        float slider_y = y + label_h;
        float slider_w = w - get_attachments_width();

        vector2 minus_size = renderer::text::get_size("-", EFont::TAHOMA);
        vector2 plus_size = renderer::text::get_size("+", EFont::TAHOMA);

        float minus_x = x;
        float track_x = x + minus_size.x + 4;
        float track_w = slider_w - minus_size.x - plus_size.x - 8;
        float plus_x = track_x + track_w + 4;

        renderer::text::draw_shadowed(name, vector2(x, y), gui::colors::text_active.scaled(), gui::colors::outline.scaled(), EFont::TAHOMA);

        bool hover_minus = cursor::is_hovered(vector2(minus_x, slider_y), vector2(minus_size.x, 12));
        bool hover_plus = cursor::is_hovered(vector2(plus_x, slider_y), vector2(plus_size.x, 12));

        renderer::text::draw_shadowed("-", vector2(minus_x, slider_y + 1),
            hover_minus ? gui::colors::text_active.scaled() : gui::colors::text_inactive.scaled(),
            gui::colors::outline.scaled(), EFont::TAHOMA);

        renderer::text::draw_shadowed("+", vector2(plus_x, slider_y + 1),
            hover_plus ? gui::colors::text_active.scaled() : gui::colors::text_inactive.scaled(),
            gui::colors::outline.scaled(), EFont::TAHOMA);

        renderer::rect::draw_filled(vector2(track_x, slider_y), vector2(track_w, 12), gui::colors::background_primary.scaled());
        renderer::rect::draw(vector2(track_x, slider_y), vector2(track_w, 12), gui::colors::border.scaled());

        visual_value = lerp(visual_value, value, 0.15f);
        float pct = (visual_value - min_value) / (max_value - min_value);
        float inner_w = track_w - 3;
        float fill_w = inner_w * pct;

        if (fill_w > 0.0f) {
            renderer::rect::draw_gradient(
                vector2(track_x + 2, slider_y + 2), vector2(fill_w, 8),
                gui::colors::accent_primary.scaled(), gui::colors::accent_secondary.scaled(),
                gui::colors::accent_primary.scaled(), gui::colors::accent_secondary.scaled()
            );
        }

        string val_text = formatFloat(value, "", 0, precision) + suffix;
        vector2 val_size = renderer::text::get_size(val_text, EFont::TAHOMABD);
        float val_x = clamp(track_x + fill_w - val_size.x / 2, track_x + 2, track_x + track_w - val_size.x - 2);
        renderer::text::draw_outlined(val_text, vector2(val_x, slider_y + 3), gui::colors::text_active.scaled(), gui::colors::outline.scaled(), EFont::TAHOMABD);

        float right_edge = x + w;
        for (uint i = 0; i < attachments.length(); i++) {
            right_edge -= 12;
            attachments[i].render(right_edge, slider_y, 12);
            right_edge -= 4;
        }
    }

    void render_popup_pass(float x, float y, float w) {
        float label_h = renderer::text::get_size(name, EFont::TAHOMA).y + 2;
        float slider_y = y + label_h;

        float right_edge = x + w;
        for (uint i = 0; i < attachments.length(); i++) {
            right_edge -= 12;
            attachments[i].render_popup(right_edge, slider_y, 12);
            right_edge -= 4;
        }
    }

    float get_height() {
        return renderer::text::get_size(name, EFont::TAHOMA).y + 2 + 12;
    }
}
