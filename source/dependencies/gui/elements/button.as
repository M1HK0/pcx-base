funcdef void ButtonFn();

class CButton {
    string name;
    bool visible = true;
    bool hovered = false;
    float click_anim = 0.0f;
    ButtonFn@ callback;
    VisibilityFn@ visibility_fn;

    CButton() {}

    CButton(const string &in _name, ButtonFn@ _callback = null) {
        name = _name;
        @callback = _callback;
    }

    void set_visible(VisibilityFn@ fn) {
        @visibility_fn = fn;
    }

    bool is_visible() {
        if (visibility_fn !is null)
            return visibility_fn();
        return visible;
    }

    void handle_input(float x, float y, float w) {
        float btn_h = 20;
        hovered = cursor::is_hovered(vector2(x, y), vector2(w, btn_h));

        if (cursor::m1_pressed && hovered) {
            click_anim = 1.0f;
            if (callback !is null)
                callback();
        }
    }

    void render(float x, float y, float w) {
        float btn_h = 20;

        renderer::rect::draw_filled(vector2(x, y), vector2(w, btn_h), gui::colors::background_primary.scaled());

        if (click_anim > 0.01f) {
            uint8 a = uint8(gui::colors::accent_primary.a * click_anim * (float(globals::menu_alpha) / 255.0f));
            color_t c1 = color_t(gui::colors::accent_primary.r, gui::colors::accent_primary.g, gui::colors::accent_primary.b, a);
            color_t c2 = color_t(gui::colors::accent_secondary.r, gui::colors::accent_secondary.g, gui::colors::accent_secondary.b, a);
            renderer::rect::draw_gradient(vector2(x + 1, y + 1), vector2(w - 2, btn_h - 2), c1, c2, c1, c2);
            click_anim = lerp(click_anim, 0.0f, 0.04f);
            if (click_anim < 0.01f) click_anim = 0.0f;
        }

        renderer::rect::draw(vector2(x, y), vector2(w, btn_h), hovered ? gui::colors::accent_primary.scaled() : gui::colors::border.scaled());

        vector2 text_size = renderer::text::get_size(name, EFont::TAHOMA);
        float text_x = x + (w - text_size.x) / 2;
        float text_y = y + (btn_h - text_size.y) / 2;

        color_t col = gui::colors::text_inactive.scaled();
        if (click_anim > 0.01f)
            col = gui::colors::text_active.scaled();
        else if (hovered)
            col = gui::colors::text_active.scaled();

        renderer::text::draw_shadowed(name, vector2(text_x, text_y), col, gui::colors::outline.scaled(), EFont::TAHOMA);
    }

    float get_height() {
        return 20;
    }
}