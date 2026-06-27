namespace crosshair {
    void render() {
        if (!gui::config::crosshair::enabled.get()) return;

        int wx, wy, ww, wh;
        window::get_rect(globals::hwnd, wx, wy, ww, wh);
        float cx = float(wx + ww / 2);
        float cy = float(wy + wh / 2);

        color_t col = gui::config::crosshair::color.get();
        color_t out_col = gui::config::crosshair::outline.get();
        int style = gui::config::crosshair::style.get();
        int sz = gui::config::crosshair::size.value;
        int gap = gui::config::crosshair::gap.value;
        int thick = gui::config::crosshair::thickness.value;
        bool t = gui::config::crosshair::t_style.get();
        bool dot = gui::config::crosshair::center_dot.get();
        int dot_sz = gui::config::crosshair::dot_size.value;
        bool has_outline = out_col.a > 0;

        int half = thick / 2;
        float fx = cx - float(half);
        float fy = cy - float(half);

        if (style == 0) {
            if (has_outline) {
                renderer::rect::draw_filled(vector2(fx - gap - sz - 1, fy - 1), vector2(float(sz) + 2, float(thick) + 2), out_col);
                renderer::rect::draw_filled(vector2(fx + thick + gap - 1, fy - 1), vector2(float(sz) + 2, float(thick) + 2), out_col);
                if (!t)
                    renderer::rect::draw_filled(vector2(fx - 1, fy - gap - sz - 1), vector2(float(thick) + 2, float(sz) + 2), out_col);
                renderer::rect::draw_filled(vector2(fx - 1, fy + thick + gap - 1), vector2(float(thick) + 2, float(sz) + 2), out_col);
            }

            renderer::rect::draw_filled(vector2(fx - gap - sz, fy), vector2(float(sz), float(thick)), col);
            renderer::rect::draw_filled(vector2(fx + thick + gap, fy), vector2(float(sz), float(thick)), col);
            if (!t)
                renderer::rect::draw_filled(vector2(fx, fy - gap - sz), vector2(float(thick), float(sz)), col);
            renderer::rect::draw_filled(vector2(fx, fy + thick + gap), vector2(float(thick), float(sz)), col);
        }

        if (style == 1) {
            if (has_outline)
                renderer::circle::draw(vector2(cx, cy), float(sz), out_col, float(thick) + 2);
            renderer::circle::draw(vector2(cx, cy), float(sz), col, float(thick));
        }

        if (dot) {
            float dr = float(dot_sz);
            if (has_outline)
                renderer::circle::draw(vector2(cx, cy), dr + 1, out_col, 1.0f, true);
            renderer::circle::draw(vector2(cx, cy), dr, col, 1.0f, true);
        }
    }
}