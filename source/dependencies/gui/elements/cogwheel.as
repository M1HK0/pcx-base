class CCogWheel {
    string name;
    string config_key;
    bool is_open = false;
    bool visible = true;
    float scroll_offset = 0.0f;
    array<CElement@> elements;
    VisibilityFn@ visibility_fn;

    CCogWheel() {}

    CCogWheel(const string &in _name) {
        name = _name;
    }

    void set_visible(VisibilityFn@ fn) {
        @visibility_fn = fn;
    }

    bool is_visible() {
        if (visibility_fn !is null)
            return visibility_fn();
        return visible;
    }

    void register_child(const string &in child_name, CElement@ el) {
        if (config_key.length() > 0)
            gui::config_system::register_element(config_key + "." + child_name, el);
    }

    CCheckbox@ create_checkbox(const string &in checkbox_name, bool default_value = false) {
        CCheckbox@ checkbox = CCheckbox(checkbox_name, default_value);
        CElement@ el = CElement(checkbox);
        elements.insertLast(el);
        if (config_key.length() > 0)
            checkbox.config_key = config_key + "." + checkbox_name;
        register_child(checkbox_name, el);
        return checkbox;
    }

    CSliderInt@ create_slider_int(const string &in slider_name, int min, int max, int default_value, int step = 1, const string &in suffix = "") {
        CSliderInt@ slider = CSliderInt(slider_name, min, max, default_value, step, suffix);
        CElement@ el = CElement(slider);
        elements.insertLast(el);
        if (config_key.length() > 0)
            slider.config_key = config_key + "." + slider_name;
        register_child(slider_name, el);
        return slider;
    }

    CSliderFloat@ create_slider_float(const string &in slider_name, float min, float max, float default_value, float step = 0.1f, int precision = 1, const string &in suffix = "") {
        CSliderFloat@ slider = CSliderFloat(slider_name, min, max, default_value, step, precision, suffix);
        CElement@ el = CElement(slider);
        elements.insertLast(el);
        if (config_key.length() > 0)
            slider.config_key = config_key + "." + slider_name;
        register_child(slider_name, el);
        return slider;
    }

    CKeybind@ create_keybind(const string &in keybind_name, int default_key = 0, EKeybindMode default_mode = EKeybindMode::HOLD) {
        CKeybind@ keybind = CKeybind(keybind_name, default_key, default_mode);
        CElement@ el = CElement(keybind);
        elements.insertLast(el);
        register_child(keybind_name, el);
        return keybind;
    }

    CDropdown@ create_dropdown(const string &in dropdown_name, array<string>@ options, int default_index = 0) {
        CDropdown@ dropdown = CDropdown(dropdown_name, options, default_index);
        CElement@ el = CElement(dropdown);
        elements.insertLast(el);
        register_child(dropdown_name, el);
        return dropdown;
    }

    CMultiSelect@ create_multiselect(const string &in multiselect_name, array<string>@ options) {
        CMultiSelect@ multiselect = CMultiSelect(multiselect_name, options);
        CElement@ el = CElement(multiselect);
        elements.insertLast(el);
        register_child(multiselect_name, el);
        return multiselect;
    }

    CColorPicker@ create_colorpicker(const string &in picker_name, color_t default_color = color_t(255, 255, 255, 255)) {
        CColorPicker@ picker = CColorPicker(picker_name, default_color);
        CElement@ el = CElement(picker);
        elements.insertLast(el);
        register_child(picker_name, el);
        return picker;
    }

    CButton@ create_button(const string &in button_name, ButtonFn@ callback = null) {
        CButton@ button = CButton(button_name, callback);
        elements.insertLast(CElement(button));
        return button;
    }

    CTextInput@ create_text_input(const string &in input_name, const string &in default_value = "", const string &in placeholder = "", int max_length = 128) {
        CTextInput@ input = CTextInput(input_name, default_value, placeholder, max_length);
        CElement@ el = CElement(input);
        elements.insertLast(el);
        register_child(input_name, el);
        return input;
    }

    CLabel@ create_label(const string &in label_name) {
        CLabel@ lbl = CLabel(label_name);
        elements.insertLast(CElement(lbl));
        return lbl;
    }

    float get_content_height() {
        float total = 0;
        bool first = true;
        for (uint i = 0; i < elements.length(); i++) {
            if (!elements[i].is_visible()) continue;
            if (!first) total += 8;
            total += elements[i].get_height();
            first = false;
        }
        return total;
    }

    void tick() {
        for (uint i = 0; i < elements.length(); i++)
            elements[i].tick();
    }

    void handle_input(float x, float y, float w) {
        if (cursor::m1_pressed && cursor::is_hovered(vector2(x, y), vector2(12, 12))) {
            is_open = !is_open;
        }

        if (is_open) {
            float popup_w = 180;
            float content_h = get_content_height();
            float max_content_h = 200;
            float actual_h = content_h < max_content_h ? content_h : max_content_h;
            float popup_h = actual_h + 26 + 6;
            float popup_x = x + 12 + 2;
            float popup_y = y;

            float element_x = popup_x + 6;
            float element_y = popup_y + 20;
            float element_w = popup_w - 12;

            float max_scroll = content_h > actual_h ? content_h - actual_h : 0;
            if (scroll_offset < 0) scroll_offset = 0;
            if (scroll_offset > max_scroll) scroll_offset = max_scroll;

            if (cursor::is_hovered(vector2(popup_x, element_y), vector2(popup_w, actual_h)) && content_h > actual_h) {
                float delta = get_scroll_delta();
                if (delta != 0) {
                    scroll_offset -= delta > 0 ? 20.0f : -20.0f;
                    if (scroll_offset < 0) scroll_offset = 0;
                    if (scroll_offset > max_scroll) scroll_offset = max_scroll;
                }
            }

            int blocking_index = -1;
            float ey = element_y - scroll_offset;
            for (uint i = 0; i < elements.length(); i++) {
                if (!elements[i].is_visible()) continue;
                if (elements[i].is_blocking()) {
                    blocking_index = int(i);
                    break;
                }
            }

            ey = element_y - scroll_offset;
            for (uint i = 0; i < elements.length(); i++) {
                if (!elements[i].is_visible()) continue;
                if (blocking_index == -1 || int(i) <= blocking_index)
                    elements[i].handle_input(element_x, ey, element_w);
                ey += elements[i].get_height() + 8;
            }

            bool hover_popup = cursor::is_hovered(vector2(popup_x, popup_y), vector2(popup_w, popup_h));
            if (cursor::m1_pressed && !cursor::is_hovered(vector2(x, y), vector2(12, 12)) && !hover_popup)
                is_open = false;
        }
    }

    void render(float x, float y, float w) {
        renderer::bitmap::draw(renderer::bitmap::cogwheel_bitmap, vector2(x, y), vector2(12, 12), color_t(180, 180, 180, globals::menu_alpha));
    }

    void render_popup_pass(float x, float y, float w) {
        if (!is_open) return;

        float popup_w = 180;
        float content_h = get_content_height();
        float max_content_h = 200;
        float actual_h = content_h < max_content_h ? content_h : max_content_h;
        float popup_h = actual_h + 26 + 6;
        float popup_x = x + 12 + 2;
        float popup_y = y;

        float element_x = popup_x + 6;
        float element_y = popup_y + 20;
        float element_w = popup_w - 12;

        renderer::rect::draw_filled(vector2(popup_x, popup_y), vector2(popup_w, popup_h), gui::colors::background_secondary.scaled());
        renderer::rect::draw(vector2(popup_x, popup_y), vector2(popup_w, popup_h), gui::colors::border.scaled());

        renderer::text::draw_shadowed(name, vector2(element_x, popup_y + 4), gui::colors::text_active.scaled(), gui::colors::outline.scaled(), EFont::TAHOMABD);

        clip_push(int(popup_x), int(element_y), int(popup_w), int(actual_h));

        float ey = element_y - scroll_offset;
        for (uint i = 0; i < elements.length(); i++) {
            if (!elements[i].is_visible()) continue;
            elements[i].render(element_x, ey, element_w);
            ey += elements[i].get_height() + 8;
        }

        clip_pop();

        if (content_h > actual_h) {
            float bar_h = (actual_h / content_h) * actual_h;
            float bar_y = element_y + (scroll_offset / content_h) * actual_h;
            renderer::rect::draw_filled(vector2(popup_x + popup_w - 4, bar_y), vector2(2, bar_h), gui::colors::border.scaled());
        }

        ey = element_y - scroll_offset;
        for (uint i = 0; i < elements.length(); i++) {
            if (!elements[i].is_visible()) continue;
            elements[i].render_popup(element_x, ey, element_w, 0);
            ey += elements[i].get_height() + 8;
        }
    }

    float get_height() {
        return 12;
    }
}