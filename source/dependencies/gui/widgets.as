funcdef void WidgetRenderFn(CWidget@ w, float x, float y, float cw, float ch);
funcdef void WidgetTitleFn(CWidget@ w, float x, float y);
funcdef void WidgetUpdateFn(CWidget@ w);
funcdef color_t WidgetIconColorFn();

class CWidget {
    string id;
    string title;
    int pos_x;
    int pos_y;
    bool visible = true;
    bool dragging = false;
    float drag_ox;
    float drag_oy;
    float content_w;
    float content_h;
    float title_w_override = 0;
    uint64 icon = 0;
    float icon_size = 0;
    WidgetIconColorFn@ icon_color_fn;
    WidgetRenderFn@ render_fn;
    WidgetTitleFn@ title_fn;
    WidgetUpdateFn@ update_fn;

    CWidget() {}

    CWidget(const string &in _id, const string &in _title, int _x, int _y) {
        id = _id;
        title = _title;
        pos_x = _x;
        pos_y = _y;
    }

    void set_icon(uint64 bitmap, float _size = 12) {
        icon = bitmap;
        icon_size = _size;
    }

    float get_header_h() {
        float h = renderer::text::get_size(title, EFont::TAHOMA).y + 20;
        if (icon != 0 && icon_size + 8 > h)
            h = icon_size + 8;
        return h;
    }

    float get_box_w() {
        vector2 ts = renderer::text::get_size(title, EFont::TAHOMA);
        float tw = title_w_override > 0 ? title_w_override : ts.x;
        float iw = icon != 0 ? icon_size + 6 : 0;
        return (content_w > tw ? content_w : tw) + 24 + iw;
    }

    float get_box_h() {
        float h = get_header_h();
        if (content_h > 0) h += content_h + 12;
        return h;
    }
}

namespace widgets {
    array<CWidget@> registered;
    float snap_dist = 8.0f;

    CWidget@ create(const string &in id, const string &in title, int x = 10, int y = 10) {
        CWidget@ w = CWidget(id, title, x, y);
        registered.insertLast(w);
        return w;
    }

    void save() {
        dictionary d;
        for (uint i = 0; i < registered.length(); i++) {
            d.set(registered[i].id + "_x", double(registered[i].pos_x));
            d.set(registered[i].id + "_y", double(registered[i].pos_y));
            d.set(registered[i].id + "_visible", registered[i].visible ? 1.0 : 0.0);
        }
        string json, err;
        if (json_stringify(d, json, err))
            create_file(gui::config_system::base_path + "\\.widgets", json);
    }

    void load() {
        string data;
        if (!read_file(gui::config_system::base_path + "\\.widgets", data)) return;
        dictionary d;
        string err;
        if (!json_parse(data, d, err)) return;
        double v;
        for (uint i = 0; i < registered.length(); i++) {
            if (d.get(registered[i].id + "_x", v)) registered[i].pos_x = int(v);
            if (d.get(registered[i].id + "_y", v)) registered[i].pos_y = int(v);
            if (d.get(registered[i].id + "_visible", v)) registered[i].visible = v > 0.5;
        }
    }

    void initialize() {
        if (does_file_exist(gui::config_system::base_path + "\\.widgets"))
            load();
    }

    void snap_to_widgets(CWidget@ w, float new_x, float new_y, float &out out_x, float &out out_y) {
        float box_w = w.get_box_w();
        float box_h = w.get_box_h();
        float best_dx = snap_dist + 1;
        float best_dy = snap_dist + 1;
        out_x = new_x;
        out_y = new_y;

        for (uint i = 0; i < registered.length(); i++) {
            CWidget@ other = registered[i];
            if (other is w || !other.visible) continue;

            float ox = float(other.pos_x);
            float oy = float(other.pos_y);
            float ow = other.get_box_w();
            float oh = other.get_box_h();
            float d;

            d = abs((new_x + box_w) - ox);
            if (d < snap_dist && d < best_dx) { best_dx = d; out_x = ox - box_w; }
            d = abs(new_x - (ox + ow));
            if (d < snap_dist && d < best_dx) { best_dx = d; out_x = ox + ow; }
            d = abs(new_x - ox);
            if (d < snap_dist && d < best_dx) { best_dx = d; out_x = ox; }
            d = abs((new_x + box_w) - (ox + ow));
            if (d < snap_dist && d < best_dx) { best_dx = d; out_x = ox + ow - box_w; }

            d = abs((new_y + box_h) - oy);
            if (d < snap_dist && d < best_dy) { best_dy = d; out_y = oy - box_h; }
            d = abs(new_y - (oy + oh));
            if (d < snap_dist && d < best_dy) { best_dy = d; out_y = oy + oh; }
            d = abs(new_y - oy);
            if (d < snap_dist && d < best_dy) { best_dy = d; out_y = oy; }
            d = abs((new_y + box_h) - (oy + oh));
            if (d < snap_dist && d < best_dy) { best_dy = d; out_y = oy + oh - box_h; }
        }

        if (best_dx > snap_dist) out_x = new_x;
        if (best_dy > snap_dist) out_y = new_y;
    }

    void snap_to_viewport(CWidget@ w, float new_x, float new_y, float &out out_x, float &out out_y) {
        vector2 vp = window::get_viewport();
        float box_w = w.get_box_w();
        float box_h = w.get_box_h();
        out_x = new_x;
        out_y = new_y;

        if (abs(new_x) < snap_dist) out_x = 0;
        if (abs(new_y) < snap_dist) out_y = 0;
        if (abs((new_x + box_w) - vp.x) < snap_dist) out_x = vp.x - box_w;
        if (abs((new_y + box_h) - vp.y) < snap_dist) out_y = vp.y - box_h;

        if (out_x < 0) out_x = 0;
        if (out_y < 0) out_y = 0;
        if (out_x + box_w > vp.x) out_x = vp.x - box_w;
        if (out_y + box_h > vp.y) out_y = vp.y - box_h;
    }

    void update() {
        for (uint i = 0; i < registered.length(); i++) {
            CWidget@ w = registered[i];
            if (w.update_fn !is null)
                w.update_fn(w);
        }

        if (!gui::config::settings::menu_key.is_active()) return;

        for (uint i = 0; i < registered.length(); i++) {
            CWidget@ w = registered[i];
            if (!w.visible) continue;

            float bx = float(w.pos_x);
            float by = float(w.pos_y);
            float box_w = w.get_box_w();
            float box_h = w.get_box_h();

            if (cursor::m1_pressed && cursor::is_hovered(vector2(bx, by), vector2(box_w, box_h)) && !w.dragging) {
                w.dragging = true;
                w.drag_ox = cursor::x - bx;
                w.drag_oy = cursor::y - by;
            }

            if (w.dragging) {
                if (cursor::m1_down) {
                    float new_x = cursor::x - w.drag_ox;
                    float new_y = cursor::y - w.drag_oy;

                    if (key_down(0x10)) {
                        float grid = 5.0f;
                        new_x = float(int(new_x / grid + 0.5f)) * grid;
                        new_y = float(int(new_y / grid + 0.5f)) * grid;
                    }

                    float sx, sy;
                    snap_to_widgets(w, new_x, new_y, sx, sy);
                    snap_to_viewport(w, sx, sy, new_x, new_y);

                    w.pos_x = int(new_x);
                    w.pos_y = int(new_y);
                } else {
                    w.dragging = false;
                    save();
                }
            }
        }
    }

    void render() {
        for (uint i = 0; i < registered.length(); i++) {
            CWidget@ w = registered[i];
            if (!w.visible) continue;

            float bx = float(w.pos_x);
            float by = float(w.pos_y);
            float header_h = w.get_header_h();
            float box_w = w.get_box_w();
            float box_h = w.get_box_h();
            bool has_content = w.content_h > 0 && w.render_fn !is null;

            renderer::rect::draw_glow(vector2(bx, by), vector2(box_w, box_h), gui::colors::glow, 4.0f, 4);

            if (has_content) {
                renderer::rect::draw_filled_rounded(vector2(bx, by), vector2(box_w, header_h), gui::colors::background_secondary, 4.0f, RR_TOP_LEFT | RR_TOP_RIGHT);
                renderer::rect::draw_filled_rounded(vector2(bx, by + header_h), vector2(box_w, box_h - header_h), gui::colors::background_primary, 4.0f, RR_BOTTOM_LEFT | RR_BOTTOM_RIGHT);
            } else {
                renderer::rect::draw_filled_rounded(vector2(bx, by), vector2(box_w, box_h), gui::colors::background_secondary, 4.0f);
            }

            renderer::rect::draw(vector2(bx + 1, by + 1), vector2(box_w - 2, box_h - 2), gui::colors::border, 1.0f, 4.0f, RR_TOP_LEFT | RR_TOP_RIGHT | RR_BOTTOM_LEFT | RR_BOTTOM_RIGHT);

            renderer::line::draw(vector2(bx + 4, by + 4), vector2(bx + 4, by + header_h - 4), gui::colors::accent_primary);

            vector2 title_size = renderer::text::get_size(w.title, EFont::TAHOMA);
            float title_x = bx + 12;
            float title_y = by + (header_h - title_size.y) / 2;

            if (w.icon != 0) {
                float icon_y = by + (header_h - w.icon_size) / 2;
                color_t ic = w.icon_color_fn !is null ? w.icon_color_fn() : color_t(255, 255, 255, 255);
                renderer::bitmap::draw(w.icon, vector2(title_x, icon_y), vector2(w.icon_size, w.icon_size), ic);
                title_x += w.icon_size + 6;
            }

            if (w.title_fn !is null)
                w.title_fn(w, title_x, title_y);
            else
                renderer::text::draw_shadowed(w.title, vector2(title_x, title_y), gui::colors::text_active, gui::colors::outline, EFont::TAHOMA);

            if (has_content) {
                float cx = bx + 10;
                float cy = by + header_h + 6;
                float cw = box_w - 20;
                float ch = w.content_h;
                w.render_fn(w, cx, cy, cw, ch);
            }
        }
    }
}
