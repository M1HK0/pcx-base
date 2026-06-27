class CWindow {
    string title;
    vector2 position;
    vector2 size;
    vector2 min_size;
    bool dragging = false;
    bool resizing = false;
    vector2 offset;
    array<CTab@> tabs;
    int active_tab = 0;

    CWindow() {}

    CWindow(const string &in _title, vector2 _position, vector2 _size) {
        title = _title;
        position = _position;
        size = _size;
        min_size = _size;
    }

    CTab@ create_tab(const string &in name) {
        CTab@ tab = CTab(name);
        if (tabs.length() == 0) tab.active = true;
        tabs.insertLast(tab);
        return tab;
    }

    void tick() {
        for (uint i = 0; i < tabs.length(); i++)
            tabs[i].tick();
    }

    void handle_input() {
        if (cursor::m1_pressed && !dragging && !resizing) {
            if (cursor::is_hovered(position + size - vector2(20, 20), vector2(20, 20))) {
                resizing = true;
                offset = cursor::pos() - (position + size);
            } else if (cursor::is_hovered(position, vector2(size.x, 30))) {
                dragging = true;
                offset = cursor::pos() - position;
            }
        }

        if (!cursor::m1_down) {
            dragging = false;
            resizing = false;
            return;
        }

        vector2 vp = window::get_viewport();

        if (dragging) {
            float new_x = cursor::x - offset.x;
            float new_y = cursor::y - offset.y;
            if (new_x < 0) new_x = 0;
            if (new_y < 0) new_y = 0;
            if (new_x + size.x > vp.x) new_x = vp.x - size.x;
            if (new_y + size.y > vp.y) new_y = vp.y - size.y;
            position = vector2(new_x, new_y);
        }

        if (resizing) {
            float new_w = cursor::x - offset.x - position.x;
            float new_h = cursor::y - offset.y - position.y;
            if (new_w < min_size.x) new_w = min_size.x;
            if (new_h < min_size.y) new_h = min_size.y;
            if (position.x + new_w > vp.x) new_w = vp.x - position.x;
            if (position.y + new_h > vp.y) new_h = vp.y - position.y;
            size = vector2(new_w, new_h);
        }
    }

    void handle_tabs() {
        if (tabs.length() == 0) return;

        vector2 title_size = renderer::text::get_size(title, EFont::TAHOMABD);
        float tab_x = position.x + title_size.x + 18;
        float tab_y = position.y + 10;

        for (uint i = 0; i < tabs.length(); i++) {
            if (i > 0) tab_x += 6;
            vector2 text_size = renderer::text::get_size(tabs[i].name, EFont::TAHOMA);
            if (cursor::m1_pressed && cursor::is_hovered(vector2(tab_x, tab_y), text_size)) {
                for (uint j = 0; j < tabs.length(); j++)
                    tabs[j].active = false;
                tabs[i].active = true;
                active_tab = i;
            }
            tab_x += text_size.x;
        }
    }

    void update() {
        handle_input();
        handle_tabs();
        if (tabs.length() > 0)
            tabs[active_tab].update(position, size);
    }

    void render_tabs() {
        if (tabs.length() == 0) return;

        vector2 title_size = renderer::text::get_size(title, EFont::TAHOMABD);
        float tab_x = position.x + title_size.x + 18;
        float tab_y = position.y + 10;

        for (uint i = 0; i < tabs.length(); i++) {
            if (i > 0) tab_x += 6;
            vector2 text_size = renderer::text::get_size(tabs[i].name, EFont::TAHOMA);
            bool hovered = cursor::is_hovered(vector2(tab_x, tab_y), text_size);

            color_t col = gui::colors::text_inactive.scaled();
            if (tabs[i].active)
                col = gui::colors::text_active.scaled();
            else if (hovered)
                col = gui::colors::text_hover.scaled();

            renderer::text::draw_shadowed(tabs[i].name, vector2(tab_x, tab_y), col, gui::colors::outline.scaled(), EFont::TAHOMA);

            if (tabs[i].active) {
                renderer::rect::draw_gradient(
                    vector2(tab_x, tab_y + text_size.y + 2),
                    vector2(text_size.x, 1),
                    gui::colors::accent_primary.scaled(), gui::colors::accent_secondary.scaled(),
                    gui::colors::accent_primary.scaled(), gui::colors::accent_secondary.scaled()
                );
            }

            tab_x += text_size.x;
        }
    }

    void render() {
        float header_h = 30;

        renderer::rect::draw_filled_rounded(position, vector2(size.x, header_h), gui::colors::background_secondary.scaled(), 4.0f, RR_TOP_LEFT | RR_TOP_RIGHT);
        renderer::rect::draw_filled_rounded(vector2(position.x, position.y + header_h), vector2(size.x, size.y - header_h), gui::colors::background_primary.scaled(), 4.0f, RR_BOTTOM_LEFT | RR_BOTTOM_RIGHT);
        renderer::rect::draw(vector2(position.x + 1, position.y + 1), vector2(size.x - 2, size.y - 2), gui::colors::border.scaled(), 1.0f, 4.0f, RR_TOP_LEFT | RR_TOP_RIGHT | RR_BOTTOM_LEFT | RR_BOTTOM_RIGHT);

        renderer::line::draw(vector2(position.x + 4, position.y + 4), vector2(position.x + 4, position.y + header_h - 4), gui::colors::accent_primary.scaled());

        renderer::text::draw_outlined(title, vector2(position.x + 12, position.y + 10), gui::colors::text_active.scaled(), gui::colors::outline.scaled(), EFont::TAHOMABD);

        render_tabs();

        // identity in top-right of header
        string uname = globals::username;
        string role_name = identity::get_role_name();
        string sep = " - ";

        vector2 uname_size = renderer::text::get_size(uname, EFont::TAHOMA);
        vector2 sep_size = vector2(0, 0);
        vector2 role_size = vector2(0, 0);
        float total_w = uname_size.x;

        if (role_name.length() > 0) {
            sep_size = renderer::text::get_size(sep, EFont::TAHOMA);
            role_size = renderer::text::get_size(role_name, EFont::TAHOMA);
            total_w += sep_size.x + role_size.x;
        }

        float id_x = position.x + size.x - total_w - 12;
        float id_y = position.y + 10;

        renderer::text::draw_shadowed(uname, vector2(id_x, id_y), gui::colors::text_active.scaled(), gui::colors::outline.scaled(), EFont::TAHOMA);

        if (role_name.length() > 0) {
            renderer::text::draw_shadowed(sep, vector2(id_x + uname_size.x, id_y), gui::colors::text_inactive.scaled(), gui::colors::outline.scaled(), EFont::TAHOMA);

            color_t role_col = identity::get_role_color();
            role_col = color_t(role_col.r, role_col.g, role_col.b, globals::menu_alpha);
            renderer::text::draw_shadowed(role_name, vector2(id_x + uname_size.x + sep_size.x, id_y), role_col, gui::colors::outline.scaled(), EFont::TAHOMA);
        }

        if (tabs.length() > 0)
            tabs[active_tab].render(position, size);

        // resize indicator - subtle bracket in bottom-right
        float rx = position.x + size.x - 4;
        float ry = position.y + size.y - 4;
        renderer::line::draw(vector2(rx - 6, ry), vector2(rx, ry - 6), gui::colors::border.scaled());
        renderer::line::draw(vector2(rx - 3, ry), vector2(rx, ry - 3), gui::colors::border.scaled());
    }

    void render_popups() {
        if (tabs.length() > 0)
            tabs[active_tab].render_popups(position, size);
    }
}