class CTooltip {
    string name;
    string description;
    bool hovered = false;
    bool visible = true;
    VisibilityFn@ visibility_fn;

    CTooltip() {}

    CTooltip(const string &in _description) {
        description = _description;
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
        hovered = cursor::is_hovered(vector2(x, y), vector2(12, 12));
    }

    void render(float x, float y, float w) {
        vector2 q_size = renderer::text::get_size("?", EFont::TAHOMA);
        color_t icon_col = hovered ? gui::colors::accent_primary.scaled() : gui::colors::text_inactive.scaled();

        renderer::circle::draw(vector2(x + 6, y + 6), 6, icon_col, 1.0f, false);
        renderer::text::draw_shadowed("?",
            vector2(x + (14 - q_size.x) / 2, y + (12 - q_size.y) / 2),
            icon_col, gui::colors::outline.scaled(), EFont::TAHOMA);
    }

    void render_popup_pass(float x, float y, float w) {
        if (!hovered) return;

        vector2 desc_size = renderer::text::get_size(description, EFont::TAHOMA);
        float popup_w = desc_size.x + 12;
        float popup_h = desc_size.y + 8;
        float popup_x = x + 14;
        float popup_y = y;

        renderer::rect::draw_glow(vector2(popup_x, popup_y), vector2(popup_w, popup_h), gui::colors::glow, 4.0f, 3);
        renderer::rect::draw_filled_rounded(vector2(popup_x, popup_y), vector2(popup_w, popup_h), gui::colors::background_secondary.scaled(), 4.0f);
        renderer::text::draw_shadowed(description,
            vector2(popup_x + 6, popup_y + 4),
            gui::colors::text_active.scaled(), gui::colors::outline.scaled(), EFont::TAHOMA);
    }

    float get_height() {
        return 12;
    }
}