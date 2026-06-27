class CTab {
    string name;
    bool active = false;
    array<CContainer@> containers;

    CTab() {}

    CTab(const string &in _name) {
        name = _name;
    }

    CContainer@ create_container(const string &in container_name, int column, float height = 0.0f, const string &in config_prefix = "") {
        CContainer@ container = CContainer(container_name, column, height, config_prefix);
        containers.insertLast(container);
        return container;
    }

    void tick() {
        for (uint i = 0; i < containers.length(); i++)
            containers[i].tick();
    }

    void update(vector2 position, vector2 size) {
        if (containers.length() == 0) return;

        float pad = 8;
        float header_h = 30;
        float content_x = position.x + pad;
        float content_y = position.y + header_h + pad;
        float content_w = size.x - pad * 2;
        float content_h = size.y - header_h - pad * 2;
        float gap = pad;
        float col_w = (content_w - gap) / 2.0f;

        array<CContainer@> col1, col2;
        for (uint i = 0; i < containers.length(); i++) {
            if (!containers[i].is_visible()) continue;
            if (containers[i].column == 1)
                col1.insertLast(containers[i]);
            else
                col2.insertLast(containers[i]);
        }

        update_column(col1, content_x, content_y, col_w, content_h);
        update_column(col2, content_x + col_w + gap, content_y, col_w, content_h);
    }

    void update_column(array<CContainer@>@ cols, float x, float y, float w, float h) {
        if (cols.length() == 0) return;

        int auto_count = 0;
        float fixed_total = 0;
        for (uint i = 0; i < cols.length(); i++) {
            if (cols[i].height_override > 0)
                fixed_total += cols[i].height_override;
            else
                auto_count++;
        }

        float gap = 8;
        float total_gaps = (cols.length() - 1) * gap;
        float auto_h = auto_count > 0 ? (h - fixed_total - total_gaps) / auto_count : 0;
        float current_y = y;

        for (uint i = 0; i < cols.length(); i++) {
            float ch = cols[i].height_override > 0 ? cols[i].height_override : auto_h;
            cols[i].update(x, current_y, w, ch);
            current_y += ch + gap;
        }
    }

    void render(vector2 position, vector2 size) {
        if (containers.length() == 0) return;

        float pad = 8;
        float header_h = 30;
        float content_x = position.x + pad;
        float content_y = position.y + header_h + pad;
        float content_w = size.x - pad * 2;
        float content_h = size.y - header_h - pad * 2;
        float gap = pad;
        float col_w = (content_w - gap) / 2.0f;

        array<CContainer@> col1, col2;
        for (uint i = 0; i < containers.length(); i++) {
            if (!containers[i].is_visible()) continue;
            if (containers[i].column == 1)
                col1.insertLast(containers[i]);
            else
                col2.insertLast(containers[i]);
        }

        render_column(col1, content_x, content_y, col_w, content_h);
        render_column(col2, content_x + col_w + gap, content_y, col_w, content_h);
    }

    void render_column(array<CContainer@>@ cols, float x, float y, float w, float h) {
        if (cols.length() == 0) return;

        int auto_count = 0;
        float fixed_total = 0;
        for (uint i = 0; i < cols.length(); i++) {
            if (cols[i].height_override > 0)
                fixed_total += cols[i].height_override;
            else
                auto_count++;
        }

        float gap = 8;
        float total_gaps = (cols.length() - 1) * gap;
        float auto_h = auto_count > 0 ? (h - fixed_total - total_gaps) / auto_count : 0;
        float current_y = y;

        for (uint i = 0; i < cols.length(); i++) {
            float ch = cols[i].height_override > 0 ? cols[i].height_override : auto_h;
            cols[i].render(x, current_y, w, ch);
            current_y += ch + gap;
        }
    }

    void render_popups(vector2 position, vector2 size) {
        if (containers.length() == 0) return;

        float pad = 8;
        float header_h = 30;
        float content_x = position.x + pad;
        float content_y = position.y + header_h + pad;
        float content_w = size.x - pad * 2;
        float content_h = size.y - header_h - pad * 2;
        float gap = pad;
        float col_w = (content_w - gap) / 2.0f;

        array<CContainer@> col1, col2;
        for (uint i = 0; i < containers.length(); i++) {
            if (!containers[i].is_visible()) continue;
            if (containers[i].column == 1)
                col1.insertLast(containers[i]);
            else
                col2.insertLast(containers[i]);
        }

        render_column_popups(col1, content_x, content_y, col_w, content_h);
        render_column_popups(col2, content_x + col_w + gap, content_y, col_w, content_h);
    }

    void render_column_popups(array<CContainer@>@ cols, float x, float y, float w, float h) {
        if (cols.length() == 0) return;

        int auto_count = 0;
        float fixed_total = 0;
        for (uint i = 0; i < cols.length(); i++) {
            if (cols[i].height_override > 0)
                fixed_total += cols[i].height_override;
            else
                auto_count++;
        }

        float gap = 8;
        float total_gaps = (cols.length() - 1) * gap;
        float auto_h = auto_count > 0 ? (h - fixed_total - total_gaps) / auto_count : 0;
        float current_y = y;

        for (uint i = 0; i < cols.length(); i++) {
            float ch = cols[i].height_override > 0 ? cols[i].height_override : auto_h;
            cols[i].render_popups(x, current_y, w, ch);
            current_y += ch + gap;
        }
    }
}