class CContainer {
    string name;
    string config_prefix;
    int column;
    float height_override;
    float scroll_offset = 0.0f;
    array<CElement@> elements;
    VisibilityFn@ visibility_fn;

    CContainer() {}

    CContainer(const string &in _name, int _column, float _height = 0.0f, const string &in _config_prefix = "") {
        name = _name;
        column = _column;
        height_override = _height;
        config_prefix = _config_prefix;
    }

    void set_visible(VisibilityFn@ fn) {
        @visibility_fn = fn;
    }

    bool is_visible() {
        if (visibility_fn !is null)
            return visibility_fn();
        return true;
    }

    void register_element(const string &in element_name, CElement@ el) {
        if (config_prefix.length() > 0)
            gui::config_system::register_element(config_prefix + "." + element_name, el);
    }

    CCheckbox@ create_checkbox(const string &in checkbox_name, bool default_value = false) {
        CCheckbox@ checkbox = CCheckbox(checkbox_name, default_value);
        CElement@ el = CElement(checkbox);
        elements.insertLast(el);
        if (config_prefix.length() > 0)
            checkbox.config_key = config_prefix + "." + checkbox_name;
        register_element(checkbox_name, el);
        return checkbox;
    }

    CSliderInt@ create_slider_int(const string &in slider_name, int min, int max, int default_value, int step = 1, const string &in suffix = "") {
        CSliderInt@ slider = CSliderInt(slider_name, min, max, default_value, step, suffix);
        CElement@ el = CElement(slider);
        elements.insertLast(el);
        if (config_prefix.length() > 0)
            slider.config_key = config_prefix + "." + slider_name;
        register_element(slider_name, el);
        return slider;
    }

    CSliderFloat@ create_slider_float(const string &in slider_name, float min, float max, float default_value, float step = 0.1f, int precision = 1, const string &in suffix = "") {
        CSliderFloat@ slider = CSliderFloat(slider_name, min, max, default_value, step, precision, suffix);
        CElement@ el = CElement(slider);
        elements.insertLast(el);
        if (config_prefix.length() > 0)
            slider.config_key = config_prefix + "." + slider_name;
        register_element(slider_name, el);
        return slider;
    }

    CKeybind@ create_keybind(const string &in keybind_name, int default_key = 0, EKeybindMode default_mode = EKeybindMode::HOLD, bool default_state = false) {
        CKeybind@ keybind = CKeybind(keybind_name, default_key, default_mode, default_state);
        CElement@ el = CElement(keybind);
        elements.insertLast(el);
        register_element(keybind_name, el);
        return keybind;
    }

    CDropdown@ create_dropdown(const string &in dropdown_name, array<string>@ options, int default_index = 0) {
        CDropdown@ dropdown = CDropdown(dropdown_name, options, default_index);
        CElement@ el = CElement(dropdown);
        elements.insertLast(el);
        register_element(dropdown_name, el);
        return dropdown;
    }

    CMultiSelect@ create_multiselect(const string &in multiselect_name, array<string>@ options) {
        CMultiSelect@ multiselect = CMultiSelect(multiselect_name, options);
        CElement@ el = CElement(multiselect);
        elements.insertLast(el);
        register_element(multiselect_name, el);
        return multiselect;
    }

    CTextInput@ create_text_input(const string &in input_name, const string &in default_value = "", const string &in placeholder = "", int max_length = 128) {
        CTextInput@ input = CTextInput(input_name, default_value, placeholder, max_length);
        CElement@ el = CElement(input);
        elements.insertLast(el);
        register_element(input_name, el);
        return input;
    }

    CColorPicker@ create_colorpicker(const string &in picker_name, color_t default_color = color_t(255, 255, 255, 255)) {
        CColorPicker@ picker = CColorPicker(picker_name, default_color);
        CElement@ el = CElement(picker);
        elements.insertLast(el);
        register_element(picker_name, el);
        return picker;
    }

    CButton@ create_button(const string &in button_name, ButtonFn@ callback = null) {
        CButton@ button = CButton(button_name, callback);
        elements.insertLast(CElement(button));
        return button;
    }

    CListBox@ create_listbox(const string &in listbox_name, bool multiselect = true, float max_height = 120, bool searchable = true) {
        CListBox@ listbox = CListBox(listbox_name, multiselect, max_height, searchable);
        elements.insertLast(CElement(listbox));
        return listbox;
    }

    CLabel@ create_label(const string &in label_name) {
        CLabel@ lbl = CLabel(label_name);
        elements.insertLast(CElement(lbl));
        if (config_prefix.length() > 0)
            lbl.config_key = config_prefix + "." + label_name;
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

    void update(float x, float y, float w, float h) {
        float content_y = y + 10;
        float content_h = h - 10;
        float total_h = get_content_height();
        float max_scroll = total_h > content_h ? total_h - content_h : 0;

        if (scroll_offset < 0) scroll_offset = 0;
        if (scroll_offset > max_scroll) scroll_offset = max_scroll;

        if (cursor::is_hovered(vector2(x, content_y), vector2(w, content_h)) && total_h > content_h) {
            float delta = get_scroll_delta();
            if (delta != 0) {
                scroll_offset -= delta > 0 ? 20.0f : -20.0f;
                if (scroll_offset < 0) scroll_offset = 0;
                if (scroll_offset > max_scroll) scroll_offset = max_scroll;
            }
        }

        float element_x = x + 6;
        float element_y = content_y - scroll_offset;
        float element_w = w - 12;

        int blocking_index = -1;
        for (uint i = 0; i < elements.length(); i++) {
            if (!elements[i].is_visible()) continue;
            if (elements[i].is_blocking()) {
                blocking_index = int(i);
                break;
            }
        }

        for (uint i = 0; i < elements.length(); i++) {
            if (!elements[i].is_visible()) continue;
            if (blocking_index == -1 || int(i) <= blocking_index)
                elements[i].handle_input(element_x, element_y, element_w);
            element_y += elements[i].get_height() + 8;
        }
    }

    void render(float x, float y, float w, float h) {
        float rounding = 4.0f;

        renderer::rect::draw_filled_rounded(vector2(x, y), vector2(w, h), gui::colors::background_secondary.scaled(), rounding);
        renderer::rect::draw(vector2(x, y), vector2(w, h), gui::colors::border.scaled(), 1.0f, rounding, RR_TOP_LEFT | RR_TOP_RIGHT | RR_BOTTOM_LEFT | RR_BOTTOM_RIGHT);

        vector2 name_size = renderer::text::get_size(name, EFont::TAHOMABD);
        float name_x = x + 22;
        float name_pad = 4;

        renderer::rect::draw_filled(vector2(name_x - name_pad, y - 1), vector2(name_size.x + name_pad * 2, 3), gui::colors::background_secondary.scaled());

        renderer::text::draw_outlined(name, vector2(name_x, y - name_size.y / 2), gui::colors::text_active.scaled(), gui::colors::outline.scaled(), EFont::TAHOMABD);

        float content_y = y + 10;
        float content_h = h - 10;
        float total_h = get_content_height();

        clip_push(int(x), int(content_y), int(w), int(content_h));

        float element_x = x + 6;
        float element_y = content_y - scroll_offset;
        float element_w = w - 12;

        for (uint i = 0; i < elements.length(); i++) {
            if (!elements[i].is_visible()) continue;
            elements[i].render(element_x, element_y, element_w);
            element_y += elements[i].get_height() + 8;
        }

        clip_pop();

        if (total_h > content_h) {
            float bar_h = (content_h / total_h) * content_h;
            float bar_y = content_y + (scroll_offset / total_h) * content_h;
            renderer::rect::draw_filled(vector2(x + w - 4, bar_y), vector2(2, bar_h), gui::colors::border.scaled());
        }
    }

    void render_popups(float x, float y, float w, float h) {
        float content_y = y + 10;
        float max_y = y + h;
        float element_x = x + 6;
        float element_y = content_y - scroll_offset;
        float element_w = w - 12;

        for (uint i = 0; i < elements.length(); i++) {
            if (!elements[i].is_visible()) continue;
            elements[i].render_popup(element_x, element_y, element_w, max_y);
            element_y += elements[i].get_height() + 8;
        }
    }
}