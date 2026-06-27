class CMultiSelect {
    string name;
    array<string> options;
    array<bool> selected;
    bool is_open = false;
    bool visible = true;
    VisibilityFn@ visibility_fn;

    CMultiSelect() {}

    CMultiSelect(const string &in _name, array<string>@ _options) {
        name = _name;
        options = _options;
        selected.resize(_options.length());
        for (uint i = 0; i < selected.length(); i++)
            selected[i] = false;
    }

    void set_visible(VisibilityFn@ fn) {
        @visibility_fn = fn;
    }

    bool is_visible() {
        if (visibility_fn !is null)
            return visibility_fn();
        return visible;
    }

    bool get(int index) {
        if (index >= 0 && index < int(selected.length()))
            return selected[index];
        return false;
    }

    array<int> get_selected() {
        array<int> result;
        for (uint i = 0; i < selected.length(); i++) {
            if (selected[i])
                result.insertLast(int(i));
        }
        return result;
    }

    array<string> get_selected_strings() {
        array<string> result;
        for (uint i = 0; i < selected.length(); i++) {
            if (selected[i])
                result.insertLast(options[i]);
        }
        return result;
    }

    string get_display_text() {
        array<string> active = get_selected_strings();
        if (active.length() == 0) return "none";
        string result = "";
        for (uint i = 0; i < active.length(); i++) {
            if (i > 0) result += ", ";
            result += active[i];
        }
        return result;
    }

    void handle_input(float x, float y, float w) {
        float label_h = renderer::text::get_size(name, EFont::TAHOMA).y + 2;
        float dropdown_y = y + label_h;
        float dropdown_h = 20;

        if (cursor::m1_pressed && cursor::is_hovered(vector2(x, dropdown_y), vector2(w, dropdown_h)))
            is_open = !is_open;

        if (is_open) {
            float list_y = dropdown_y + dropdown_h + 2;
            float option_h = 20;
            float list_h = options.length() * option_h;

            for (uint i = 0; i < options.length(); i++) {
                float option_y = list_y + (i * option_h);
                if (cursor::m1_pressed && cursor::is_hovered(vector2(x, option_y), vector2(w, option_h))) {
                    selected[i] = !selected[i];
                    return;
                }
            }

            bool hover_box = cursor::is_hovered(vector2(x, dropdown_y), vector2(w, dropdown_h));
            bool hover_list = cursor::is_hovered(vector2(x, list_y), vector2(w, list_h));

            if (cursor::m1_pressed && !hover_box && !hover_list)
                is_open = false;
        }
    }

    void render(float x, float y, float w) {
        float label_h = renderer::text::get_size(name, EFont::TAHOMA).y + 2;
        float dropdown_y = y + label_h;
        float dropdown_h = 20;

        renderer::text::draw_shadowed(name, vector2(x, y), gui::colors::text_active.scaled(), gui::colors::outline.scaled(), EFont::TAHOMA);

        renderer::rect::draw_filled(vector2(x, dropdown_y), vector2(w, dropdown_h), gui::colors::background_primary.scaled());
        renderer::rect::draw(vector2(x, dropdown_y), vector2(w, dropdown_h), gui::colors::border.scaled());

        string display = get_display_text();
        vector2 display_size = renderer::text::get_size(display, EFont::TAHOMA);

        clip_push(int(x + 6), int(dropdown_y), int(w - 28), int(dropdown_h));
        renderer::text::draw_shadowed(
            display,
            vector2(x + 6, dropdown_y + (dropdown_h - display_size.y) / 2),
            gui::colors::text_active.scaled(),
            gui::colors::outline.scaled(),
            EFont::TAHOMA
        );
        clip_pop();

        float bx = x + w - 16;
        float by = dropdown_y + 6;
        renderer::line::draw(vector2(bx, by), vector2(bx + 8, by), gui::colors::text_inactive.scaled());
        renderer::line::draw(vector2(bx, by + 4), vector2(bx + 8, by + 4), gui::colors::text_inactive.scaled());
        renderer::line::draw(vector2(bx, by + 8), vector2(bx + 8, by + 8), gui::colors::text_inactive.scaled());
    }

    void render_popup_pass(float x, float y, float w, float max_y = 0) {
        if (!is_open) return;

        float label_h = renderer::text::get_size(name, EFont::TAHOMA).y + 2;
        float dropdown_y = y + label_h;
        float dropdown_h = 20;
        float list_y = dropdown_y + dropdown_h + 2;
        float option_h = 20;
        float list_h = options.length() * option_h;

        if (max_y > 0 && list_y + list_h > max_y)
            list_h = max_y - list_y;

        if (list_h <= 0) return;

        renderer::rect::draw_filled(vector2(x, list_y), vector2(w, list_h), gui::colors::background_secondary.scaled());
        renderer::rect::draw(vector2(x, list_y), vector2(w, list_h), gui::colors::border.scaled());

        clip_push(int(x), int(list_y), int(w), int(list_h));

        for (uint i = 0; i < options.length(); i++) {
            float option_y = list_y + (i * option_h);
            if (option_y + option_h < list_y || option_y > list_y + list_h) continue;

            bool hovering = cursor::is_hovered(vector2(x, option_y), vector2(w, option_h));
            bool is_selected = selected[i];

            if (hovering)
                renderer::rect::draw_filled(vector2(x + 2, option_y + 1), vector2(w - 4, option_h - 2), gui::colors::background_primary.scaled());

            if (is_selected)
                renderer::line::draw(vector2(x + 2, option_y + 2), vector2(x + 2, option_y + option_h - 2), gui::colors::accent_primary.scaled());

            color_t col = gui::colors::text_inactive.scaled();
            if (is_selected)
                col = gui::colors::accent_primary.scaled();
            else if (hovering)
                col = gui::colors::text_active.scaled();

            renderer::text::draw_shadowed(options[i], vector2(x + 8, option_y + 4), col, gui::colors::outline.scaled(), EFont::TAHOMA);
        }

        clip_pop();
    }

    float get_height() {
        return renderer::text::get_size(name, EFont::TAHOMA).y + 2 + 20;
    }
}