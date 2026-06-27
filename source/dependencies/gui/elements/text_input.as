class CTextInput {
    string name;
    string value;
    string placeholder;
    bool focused = false;
    bool visible = true;
    int max_length;
    VisibilityFn@ visibility_fn;

    CTextInput() {}

    CTextInput(const string &in _name, const string &in _default = "", const string &in _placeholder = "", int _max_length = 128) {
        name = _name;
        value = _default;
        placeholder = _placeholder;
        max_length = _max_length;
    }

    void set_visible(VisibilityFn@ fn) {
        @visibility_fn = fn;
    }

    bool is_visible() {
        if (visibility_fn !is null)
            return visibility_fn();
        return visible;
    }

    string get() {
        return value;
    }

    void handle_input(float x, float y, float w) {
        float label_h = renderer::text::get_size(name, EFont::TAHOMA).y + 2;
        float box_y = y + label_h;
        float box_h = 20;

        if (cursor::m1_pressed) {
            if (cursor::is_hovered(vector2(x, box_y), vector2(w, box_h)))
                focused = true;
            else
                focused = false;
        }

        if (!focused) return;

        if (key_singlepress(0x1B)) {
            focused = false;
            return;
        }

        if (key_singlepress(0x0D)) {
            focused = false;
            return;
        }

        if (key_fired(0x08)) {
            if (value.length() > 0)
                value = value.substr(0, value.length() - 1);
            return;
        }

        string input = get_recent_key_input();
        if (input.length() > 0 && int(value.length() + input.length()) <= max_length)
            value += input;
    }

    void render(float x, float y, float w) {
        float label_h = renderer::text::get_size(name, EFont::TAHOMA).y + 2;
        float box_y = y + label_h;
        float box_h = 20;

        renderer::text::draw_shadowed(name, vector2(x, y), gui::colors::text_active.scaled(), gui::colors::outline.scaled(), EFont::TAHOMA);

        renderer::rect::draw_filled(vector2(x, box_y), vector2(w, box_h), gui::colors::background_primary.scaled());
        renderer::rect::draw(vector2(x, box_y), vector2(w, box_h), focused ? gui::colors::accent_primary.scaled() : gui::colors::border.scaled());

        clip_push(int(x + 4), int(box_y), int(w - 8), int(box_h));

        string display = value;
        color_t text_col = gui::colors::text_active.scaled();

        if (value.length() == 0 && !focused) {
            display = placeholder;
            text_col = gui::colors::text_inactive.scaled();
        }

        vector2 text_size = renderer::text::get_size(display, EFont::TAHOMA);
        float text_y = box_y + (box_h - text_size.y) / 2;

        renderer::text::draw_shadowed(display, vector2(x + 6, text_y), text_col, gui::colors::outline.scaled(), EFont::TAHOMA);

        if (focused) {
            vector2 val_size = renderer::text::get_size(value, EFont::TAHOMA);
            float cursor_x = x + 6 + val_size.x + 1;
            float cursor_y1 = box_y + 4;
            float cursor_y2 = box_y + box_h - 4;
            renderer::line::draw(vector2(cursor_x, cursor_y1), vector2(cursor_x, cursor_y2), gui::colors::text_active.scaled());
        }

        clip_pop();
    }

    float get_height() {
        return renderer::text::get_size(name, EFont::TAHOMA).y + 2 + 20;
    }
}