class CListBoxItem {
    string name;
    string description;

    CListBoxItem() {}

    CListBoxItem(const string &in _name, const string &in _description = "") {
        name = _name;
        description = _description;
    }
}

class CListBox {
    string name;
    array<CListBoxItem> items;
    array<bool> selected;
    bool visible = true;
    bool multiselect = true;
    bool searchable = true;
    bool search_focused = false;
    string search_text;
    float scroll_offset = 0.0f;
    float max_height = 120;
    VisibilityFn@ visibility_fn;

    CListBox() {}

    CListBox(const string &in _name, bool _multiselect = true, float _max_height = 120, bool _searchable = true) {
        name = _name;
        multiselect = _multiselect;
        max_height = _max_height;
        searchable = _searchable;
    }

    void add_item(const string &in item_name, const string &in description = "") {
        items.insertLast(CListBoxItem(item_name, description));
        selected.insertLast(false);
    }

    void remove_item(int index) {
        if (index >= 0 && index < int(items.length())) {
            items.removeAt(index);
            selected.removeAt(index);
        }
    }

    void clear() {
        items.resize(0);
        selected.resize(0);
        scroll_offset = 0;
    }

    void set_items(array<string>@ names) {
        clear();
        for (uint i = 0; i < names.length(); i++)
            add_item(names[i]);
    }

    void update_item(int index, const string &in item_name, const string &in description = "") {
        if (index >= 0 && index < int(items.length())) {
            items[index].name = item_name;
            items[index].description = description;
        }
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
                result.insertLast(items[i].name);
        }
        return result;
    }

    string get_selected_string() {
        for (uint i = 0; i < selected.length(); i++) {
            if (selected[i])
                return items[i].name;
        }
        return "";
    }

    bool matches_search(uint index) {
        if (search_text.length() == 0) return true;
        if (search_text.length() > items[index].name.length()) return false;

        string lower_name = items[index].name;
        string lower_search = search_text;

        for (uint i = 0; i + lower_search.length() <= lower_name.length(); i++) {
            bool match = true;
            for (uint j = 0; j < lower_search.length(); j++) {
                uint8 a = lower_name[i + j];
                uint8 b = lower_search[j];
                if (a >= 65 && a <= 90) a += 32;
                if (b >= 65 && b <= 90) b += 32;
                if (a != b) { match = false; break; }
            }
            if (match) return true;
        }
        return false;
    }

    float get_item_height(uint index) {
        if (items[index].description.length() > 0)
            return renderer::text::get_size(items[index].name, EFont::TAHOMA).y + renderer::text::get_size(items[index].description, EFont::TAHOMA).y + 6;
        return renderer::text::get_size(items[index].name, EFont::TAHOMA).y + 6;
    }

    float get_filtered_list_height() {
        float total = 0;
        for (uint i = 0; i < items.length(); i++) {
            if (matches_search(i))
                total += get_item_height(i);
        }
        return total;
    }

    void handle_input(float x, float y, float w) {
        float label_h = renderer::text::get_size(name, EFont::TAHOMA).y + 2;
        float current_y = y + label_h;

        if (searchable) {
            float search_h = 20;

            if (cursor::m1_pressed) {
                if (cursor::is_hovered(vector2(x, current_y), vector2(w, search_h)))
                    search_focused = true;
                else
                    search_focused = false;
            }

            if (search_focused) {
                if (key_singlepress(0x1B)) {
                    search_focused = false;
                } else if (key_fired(0x08)) {
                    if (search_text.length() > 0)
                        search_text = search_text.substr(0, search_text.length() - 1);
                    scroll_offset = 0;
                } else {
                    string input = get_recent_key_input();
                    if (input.length() > 0) {
                        search_text += input;
                        scroll_offset = 0;
                    }
                }
            }

            current_y += search_h + 4;
        }

        float list_y = current_y;
        float list_h = get_filtered_list_height();
        float actual_h = list_h < max_height ? list_h : max_height;

        float max_scroll = list_h > actual_h ? list_h - actual_h : 0;
        if (scroll_offset < 0) scroll_offset = 0;
        if (scroll_offset > max_scroll) scroll_offset = max_scroll;

        if (cursor::is_hovered(vector2(x, list_y), vector2(w, actual_h)) && list_h > actual_h) {
            float delta = get_scroll_delta();
            if (delta != 0) {
                scroll_offset -= delta > 0 ? 20.0f : -20.0f;
                if (scroll_offset < 0) scroll_offset = 0;
                if (scroll_offset > max_scroll) scroll_offset = max_scroll;
            }
        }

        if (cursor::m1_pressed) {
            float item_y = list_y - scroll_offset;
            for (uint i = 0; i < items.length(); i++) {
                if (!matches_search(i)) continue;
                float ih = get_item_height(i);
                if (cursor::is_hovered(vector2(x, item_y), vector2(w, ih)) &&
                    cursor::y >= list_y && cursor::y <= list_y + actual_h) {
                    if (multiselect) {
                        selected[i] = !selected[i];
                    } else {
                        for (uint j = 0; j < selected.length(); j++)
                            selected[j] = false;
                        selected[i] = true;
                    }
                    break;
                }
                item_y += ih;
            }
        }
    }

    void render(float x, float y, float w) {
        float label_h = renderer::text::get_size(name, EFont::TAHOMA).y + 2;
        float current_y = y + label_h;

        renderer::text::draw_shadowed(name, vector2(x, y), gui::colors::text_active.scaled(), gui::colors::outline.scaled(), EFont::TAHOMA);

        if (searchable) {
            float search_h = 20;

            renderer::rect::draw_filled(vector2(x, current_y), vector2(w, search_h), gui::colors::background_primary.scaled());
            renderer::rect::draw(vector2(x, current_y), vector2(w, search_h), search_focused ? gui::colors::accent_primary.scaled() : gui::colors::border.scaled());

            string display = search_text;
            color_t text_col = gui::colors::text_active.scaled();

            if (search_text.length() == 0 && !search_focused) {
                display = "search...";
                text_col = gui::colors::text_inactive.scaled();
            }

            vector2 display_size = renderer::text::get_size(display, EFont::TAHOMA);
            clip_push(int(x + 4), int(current_y), int(w - 8), int(search_h));
            renderer::text::draw_shadowed(display, vector2(x + 6, current_y + (search_h - display_size.y) / 2), text_col, gui::colors::outline.scaled(), EFont::TAHOMA);

            if (search_focused) {
                vector2 val_size = renderer::text::get_size(search_text, EFont::TAHOMA);
                float cursor_x = x + 6 + val_size.x + 1;
                renderer::line::draw(vector2(cursor_x, current_y + 4), vector2(cursor_x, current_y + search_h - 4), gui::colors::text_active.scaled());
            }

            clip_pop();
            current_y += search_h + 4;
        }

        float list_y = current_y;
        float list_h = get_filtered_list_height();
        float actual_h = list_h > 0 ? (list_h < max_height ? list_h : max_height) : 20;

        renderer::rect::draw_filled(vector2(x, list_y), vector2(w, actual_h), gui::colors::background_primary.scaled());
        renderer::rect::draw(vector2(x, list_y), vector2(w, actual_h), gui::colors::border.scaled());

        if (list_h <= 0) return;

        clip_push(int(x), int(list_y), int(w), int(actual_h));

        float item_y = list_y - scroll_offset;
        for (uint i = 0; i < items.length(); i++) {
            if (!matches_search(i)) continue;

            float ih = get_item_height(i);
            bool hovering = cursor::is_hovered(vector2(x, item_y), vector2(w, ih)) &&
                            cursor::y >= list_y && cursor::y <= list_y + actual_h;
            bool is_selected = selected[i];

            if (hovering)
                renderer::rect::draw_filled(vector2(x + 2, item_y + 1), vector2(w - 4, ih - 2), gui::colors::background_secondary.scaled());

            if (is_selected)
                renderer::line::draw(vector2(x + 2, item_y + 2), vector2(x + 2, item_y + ih - 2), gui::colors::accent_primary.scaled());

            color_t name_col = gui::colors::text_inactive.scaled();
            if (is_selected)
                name_col = gui::colors::accent_primary.scaled();
            else if (hovering)
                name_col = gui::colors::text_active.scaled();

            renderer::text::draw_shadowed(items[i].name, vector2(x + 8, item_y + 3), name_col, gui::colors::outline.scaled(), EFont::TAHOMA);

            if (items[i].description.length() > 0) {
                float name_h = renderer::text::get_size(items[i].name, EFont::TAHOMA).y;
                color_t desc_col = is_selected ? gui::colors::text_active.scaled() : gui::colors::text_inactive.scaled();
                renderer::text::draw_shadowed(items[i].description, vector2(x + 8, item_y + 3 + name_h), desc_col, gui::colors::outline.scaled(), EFont::TAHOMA);
            }

            item_y += ih;
        }

        clip_pop();

        if (list_h > actual_h) {
            float bar_h = (actual_h / list_h) * actual_h;
            float bar_y = list_y + (scroll_offset / list_h) * actual_h;
            renderer::rect::draw_filled(vector2(x + w - 4, bar_y), vector2(2, bar_h), gui::colors::border.scaled());
        }
    }

    float get_height() {
        float label_h = renderer::text::get_size(name, EFont::TAHOMA).y + 2;
        float list_h = get_filtered_list_height();
        float actual_h = list_h > 0 ? (list_h < max_height ? list_h : max_height) : 20;
        float total = label_h + actual_h;
        if (searchable) total += 24;
        return total;
    }
}