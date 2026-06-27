class CColorPicker {
    string name;
    color_t value;
    bool is_open = false;
    bool visible = true;
    bool attached = false;
    bool dragging_picker = false;
    bool dragging_hue = false;
    bool dragging_alpha = false;
    float hue = 0.0f;
    float sat = 1.0f;
    float val = 1.0f;
    VisibilityFn@ visibility_fn;

    CColorPicker() {}

    CColorPicker(const string &in _name, color_t _default = color_t(255, 255, 255, 255)) {
        name = _name;
        value = _default;
        rgb_to_hsv(value.r, value.g, value.b, hue, sat, val);
    }

    void set_visible(VisibilityFn@ fn) {
        @visibility_fn = fn;
    }

    bool is_visible() {
        if (visibility_fn !is null)
            return visibility_fn();
        return visible;
    }

    color_t get() {
        return value;
    }

    void rgb_to_hsv(uint8 r, uint8 g, uint8 b, float &out h, float &out s, float &out v) {
        float rf = r / 255.0f;
        float gf = g / 255.0f;
        float bf = b / 255.0f;
        float mx = rf > gf ? (rf > bf ? rf : bf) : (gf > bf ? gf : bf);
        float mn = rf < gf ? (rf < bf ? rf : bf) : (gf < bf ? gf : bf);
        float delta = mx - mn;

        v = mx;
        s = mx > 0.0f ? delta / mx : 0.0f;

        if (delta < 0.001f) {
            h = 0.0f;
        } else if (mx == rf) {
            h = 60.0f * ((gf - bf) / delta);
            if (h < 0.0f) h += 360.0f;
        } else if (mx == gf) {
            h = 60.0f * (2.0f + (bf - rf) / delta);
        } else {
            h = 60.0f * (4.0f + (rf - gf) / delta);
        }
    }

    void hsv_to_rgb(float h, float s, float v, uint8 &out r, uint8 &out g, uint8 &out b) {
        float c = v * s;
        float x = c * (1.0f - abs(((h / 60.0f) % 2.0f) - 1.0f));
        float m = v - c;
        float rf = 0, gf = 0, bf = 0;

        if (h < 60)       { rf = c; gf = x; bf = 0; }
        else if (h < 120) { rf = x; gf = c; bf = 0; }
        else if (h < 180) { rf = 0; gf = c; bf = x; }
        else if (h < 240) { rf = 0; gf = x; bf = c; }
        else if (h < 300) { rf = x; gf = 0; bf = c; }
        else              { rf = c; gf = 0; bf = x; }

        r = uint8((rf + m) * 255.0f);
        g = uint8((gf + m) * 255.0f);
        b = uint8((bf + m) * 255.0f);
    }

    color_t hue_to_rgb(float h) {
        uint8 r, g, b;
        hsv_to_rgb(h, 1.0f, 1.0f, r, g, b);
        return color_t(r, g, b, 255);
    }

    void update_from_hsv() {
        hsv_to_rgb(hue, sat, val, value.r, value.g, value.b);
    }

    void handle_input(float x, float y, float w) {
        float box_w = 12;
        float box_h = 12;
        float box_x = attached ? x : x + w - box_w;

        if (cursor::m1_pressed && cursor::is_hovered(vector2(box_x, y), vector2(box_w, box_h)))
            is_open = !is_open;

        if (is_open) {
            float popup_w = 160;
            float popup_h = 200;
            float popup_x = box_x + box_w + 2;
            float popup_y = y;

            float picker_x = popup_x + 6;
            float picker_y = popup_y + 20;
            float picker_w = popup_w - 12;
            float picker_h = 100;

            float hue_y = picker_y + picker_h + 6;
            float hue_h = 12;

            float alpha_y = hue_y + hue_h + 6;
            float alpha_h = 12;

            float btn_y = alpha_y + alpha_h + 6;
            float btn_w = (picker_w - 4) / 2;
            float btn_h = 16;

            if (cursor::m1_pressed) {
                if (cursor::is_hovered(vector2(picker_x, picker_y), vector2(picker_w, picker_h)))
                    dragging_picker = true;
                else if (cursor::is_hovered(vector2(picker_x, hue_y), vector2(picker_w, hue_h)))
                    dragging_hue = true;
                else if (cursor::is_hovered(vector2(picker_x, alpha_y), vector2(picker_w, alpha_h)))
                    dragging_alpha = true;
                else if (cursor::is_hovered(vector2(picker_x, btn_y), vector2(btn_w, btn_h)))
                    gui::colors::copied_color = value;
                else if (cursor::is_hovered(vector2(picker_x + btn_w + 4, btn_y), vector2(btn_w, btn_h))) {
                    value = gui::colors::copied_color;
                    rgb_to_hsv(value.r, value.g, value.b, hue, sat, val);
                }
            }

            if (!cursor::m1_down) {
                dragging_picker = false;
                dragging_hue = false;
                dragging_alpha = false;
            }

            if (dragging_picker) {
                sat = clamp((cursor::x - picker_x) / picker_w, 0.0f, 1.0f);
                val = 1.0f - clamp((cursor::y - picker_y) / picker_h, 0.0f, 1.0f);
                update_from_hsv();
            }

            if (dragging_hue) {
                hue = clamp((cursor::x - picker_x) / picker_w, 0.0f, 1.0f) * 360.0f;
                update_from_hsv();
            }

            if (dragging_alpha) {
                value.a = uint8(clamp((cursor::x - picker_x) / picker_w, 0.0f, 1.0f) * 255.0f);
            }

            bool hover_popup = cursor::is_hovered(vector2(popup_x, popup_y), vector2(popup_w, popup_h));
            if (cursor::m1_pressed && !cursor::is_hovered(vector2(box_x, y), vector2(box_w, box_h)) && !hover_popup)
                is_open = false;
        }
    }

    void render(float x, float y, float w) {
        float box_w = 12;
        float box_h = 12;
        float box_x = attached ? x : x + w - box_w;

        if (!attached)
            renderer::text::draw_shadowed(name, vector2(x, y), gui::colors::text_active.scaled(), gui::colors::outline.scaled(), EFont::TAHOMA);

        uint8 a = uint8((value.a * globals::menu_alpha) / 255);
        uint8 lr = uint8(clamp(int(value.r) + 30, 0, 255));
        uint8 lg = uint8(clamp(int(value.g) + 30, 0, 255));
        uint8 lb = uint8(clamp(int(value.b) + 30, 0, 255));
        uint8 dr = uint8(clamp(int(value.r) - 30, 0, 255));
        uint8 dg = uint8(clamp(int(value.g) - 30, 0, 255));
        uint8 db = uint8(clamp(int(value.b) - 30, 0, 255));

        renderer::rect::draw_gradient(
            vector2(box_x + 2, y + 2), vector2(box_w - 4, box_h - 4),
            color_t(lr, lg, lb, a), color_t(value.r, value.g, value.b, a),
            color_t(value.r, value.g, value.b, a), color_t(dr, dg, db, a)
        );
        renderer::rect::draw(vector2(box_x, y), vector2(box_w, box_h), gui::colors::border.scaled());
    }

    void render_popup_pass(float x, float y, float w) {
        if (!is_open) return;

        float box_w = 12;
        float box_x = attached ? x : x + w - box_w;

        float popup_w = 160;
        float popup_h = 200;
        float popup_x = box_x + box_w + 2;
        float popup_y = y;

        float picker_x = popup_x + 6;
        float picker_y = popup_y + 20;
        float picker_w = popup_w - 12;
        float picker_h = 100;

        float hue_y = picker_y + picker_h + 6;
        float hue_h = 12;

        float alpha_y = hue_y + hue_h + 6;
        float alpha_h = 12;

        float btn_y = alpha_y + alpha_h + 6;
        float btn_w = (picker_w - 4) / 2;
        float btn_h = 16;

        renderer::rect::draw_filled(vector2(popup_x, popup_y), vector2(popup_w, popup_h), gui::colors::background_secondary.scaled());
        renderer::rect::draw(vector2(popup_x, popup_y), vector2(popup_w, popup_h), gui::colors::border.scaled());

        renderer::text::draw_shadowed(name, vector2(picker_x, popup_y + 4), gui::colors::text_active.scaled(), gui::colors::outline.scaled(), EFont::TAHOMABD);

        // saturation/value picker
        int num_cols = 60;
        int num_rows = 60;
        float cell_w = picker_w / float(num_cols);
        float cell_h = picker_h / float(num_rows);
        for (int row = 0; row < num_rows; row++) {
            float v = 1.0f - (float(row) + 0.5f) / float(num_rows);
            for (int col = 0; col < num_cols; col++) {
                float s = (float(col) + 0.5f) / float(num_cols);
                uint8 cr, cg, cb;
                hsv_to_rgb(hue, s, v, cr, cg, cb);
                renderer::rect::draw_filled(vector2(picker_x + col * cell_w, picker_y + row * cell_h), vector2(cell_w + 1, cell_h + 1), color_t(cr, cg, cb, globals::menu_alpha));
            }
        }
        renderer::rect::draw(vector2(picker_x, picker_y), vector2(picker_w, picker_h), gui::colors::border.scaled());

        // picker cursor
        float cx = picker_x + sat * picker_w;
        float cy = picker_y + (1.0f - val) * picker_h;
        renderer::rect::draw(vector2(cx - 3, cy - 3), vector2(6, 6), color_t(255, 255, 255, globals::menu_alpha));

        // hue bar
        float hue_step = picker_w / 6.0f;
        for (int i = 0; i < 6; i++) {
            color_t left = hue_to_rgb(i * 60.0f);
            color_t right = hue_to_rgb((i + 1) * 60.0f);
            left.a = globals::menu_alpha;
            right.a = globals::menu_alpha;
            float seg_x = picker_x + i * hue_step;
            float seg_w = hue_step + 1;
            renderer::rect::draw_gradient(
                vector2(seg_x, hue_y), vector2(seg_w, hue_h),
                left, right, left, right
            );
        }
        renderer::rect::draw(vector2(picker_x, hue_y), vector2(picker_w, hue_h), gui::colors::border.scaled());

        // hue cursor
        float hue_cx = picker_x + (hue / 360.0f) * picker_w;
        renderer::rect::draw_filled(vector2(hue_cx - 1, hue_y - 1), vector2(3, hue_h + 2), color_t(255, 255, 255, globals::menu_alpha));

        // alpha bar
        renderer::rect::draw_gradient(
            vector2(picker_x, alpha_y), vector2(picker_w, alpha_h),
            color_t(value.r, value.g, value.b, 0), color_t(value.r, value.g, value.b, globals::menu_alpha),
            color_t(value.r, value.g, value.b, 0), color_t(value.r, value.g, value.b, globals::menu_alpha)
        );
        renderer::rect::draw(vector2(picker_x, alpha_y), vector2(picker_w, alpha_h), gui::colors::border.scaled());

        // alpha cursor
        float alpha_cx = picker_x + (float(value.a) / 255.0f) * picker_w;
        renderer::rect::draw_filled(vector2(alpha_cx - 1, alpha_y - 1), vector2(3, alpha_h + 2), color_t(255, 255, 255, globals::menu_alpha));

        // copy button
        bool hover_copy = cursor::is_hovered(vector2(picker_x, btn_y), vector2(btn_w, btn_h));
        renderer::rect::draw_filled(vector2(picker_x, btn_y), vector2(btn_w, btn_h), gui::colors::background_primary.scaled());
        renderer::rect::draw(vector2(picker_x, btn_y), vector2(btn_w, btn_h), hover_copy ? gui::colors::accent_primary.scaled() : gui::colors::border.scaled());
        vector2 copy_size = renderer::text::get_size("copy", EFont::TAHOMA);
        renderer::text::draw_shadowed("copy",
            vector2(picker_x + (btn_w - copy_size.x) / 2, btn_y + (btn_h - copy_size.y) / 2),
            hover_copy ? gui::colors::text_active.scaled() : gui::colors::text_inactive.scaled(),
            gui::colors::outline.scaled(), EFont::TAHOMA);

        // paste button
        float paste_x = picker_x + btn_w + 4;
        bool hover_paste = cursor::is_hovered(vector2(paste_x, btn_y), vector2(btn_w, btn_h));
        renderer::rect::draw_filled(vector2(paste_x, btn_y), vector2(btn_w, btn_h), gui::colors::background_primary.scaled());
        renderer::rect::draw(vector2(paste_x, btn_y), vector2(btn_w, btn_h), hover_paste ? gui::colors::accent_primary.scaled() : gui::colors::border.scaled());
        vector2 paste_size = renderer::text::get_size("paste", EFont::TAHOMA);
        renderer::text::draw_shadowed("paste",
            vector2(paste_x + (btn_w - paste_size.x) / 2, btn_y + (btn_h - paste_size.y) / 2),
            hover_paste ? gui::colors::text_active.scaled() : gui::colors::text_inactive.scaled(),
            gui::colors::outline.scaled(), EFont::TAHOMA);
    }

    float get_height() {
        return 12;
    }
}