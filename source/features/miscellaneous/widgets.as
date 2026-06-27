namespace widget_system {
    CWidget@ watermark;
    CWidget@ keybinds;

    void watermark_update(CWidget@ w) {
        w.visible = gui::config::settings::widgets::watermark.get();
    }

    void watermark_title(CWidget@ w, float x, float y) {
        string sep = " | ";
        renderer::text::draw_shadowed("perception", vector2(x, y), gui::colors::text_active, gui::colors::outline, EFont::TAHOMA);
        float tx = x + renderer::text::get_size("perception", EFont::TAHOMA).x;
        renderer::text::draw_shadowed(".cx", vector2(tx, y), gui::colors::text_inactive, gui::colors::outline, EFont::TAHOMA);
        tx += renderer::text::get_size(".cx", EFont::TAHOMA).x;
        renderer::text::draw_shadowed(sep, vector2(tx, y), gui::colors::text_inactive, gui::colors::outline, EFont::TAHOMA);
        tx += renderer::text::get_size(sep, EFont::TAHOMA).x;
        renderer::text::draw_shadowed(globals::username, vector2(tx, y), gui::colors::text_active, gui::colors::outline, EFont::TAHOMA);
    }

    void keybinds_render(CWidget@ w, float x, float y, float cw, float ch) {
        float line_h = renderer::text::get_size("A", EFont::TAHOMA).y;
        float row_h = line_h + 8;
        float current_y = y;

        for (uint i = 0; i < hotkeys::registered.length(); i++) {
            CKeybind@ kb = hotkeys::registered[i];
            if (kb.key == 0 || !kb.is_active()) continue;

            string mode_text = "always";
            if (kb.mode == EKeybindMode::HOLD) mode_text = "hold";
            else if (kb.mode == EKeybindMode::TOGGLE) mode_text = "toggle";

            float text_y = current_y + (row_h - line_h) / 2;
            renderer::text::draw_shadowed(kb.hotkey_name, vector2(x + 2, text_y), gui::colors::text_active, gui::colors::outline, EFont::TAHOMA);

            float mode_w = renderer::text::get_size(mode_text, EFont::TAHOMA).x;
            renderer::text::draw_shadowed(mode_text, vector2(x + cw - mode_w - 2, text_y), gui::colors::accent_primary, gui::colors::outline, EFont::TAHOMA);

            current_y += row_h;
        }
    }

    void keybinds_update(CWidget@ w) {
        if (!gui::config::settings::widgets::hotkeys.get()) {
            w.visible = false;
            return;
        }

        float line_h = renderer::text::get_size("A", EFont::TAHOMA).y;
        float row_h = line_h + 8;
        int count = 0;
        float max_w = 0;

        for (uint i = 0; i < hotkeys::registered.length(); i++) {
            CKeybind@ kb = hotkeys::registered[i];
            if (kb.key == 0 || !kb.is_active()) continue;

            string mode_text = "always";
            if (kb.mode == EKeybindMode::HOLD) mode_text = "hold";
            else if (kb.mode == EKeybindMode::TOGGLE) mode_text = "toggle";

            float row_w = renderer::text::get_size(kb.hotkey_name, EFont::TAHOMA).x + 24 + renderer::text::get_size(mode_text, EFont::TAHOMA).x;
            if (row_w > max_w) max_w = row_w;
            count++;
        }

        w.content_w = max_w > 100 ? max_w : 100;
        w.content_h = count > 0 ? count * row_h + (count - 1) : 0;
        w.visible = count > 0 || gui::config::settings::menu_key.is_active();
    }

    void initialize() {
        @watermark = widgets::create("watermark", "watermark", 10, 10);
        watermark.title_w_override = renderer::text::get_size("perception.cx | " + globals::username, EFont::TAHOMA).x;
        @watermark.title_fn = watermark_title;
        @watermark.update_fn = watermark_update;

        @keybinds = widgets::create("keybinds", "hotkeys", 10, 50);
        keybinds.set_icon(renderer::bitmap::hotkey_bitmap, 18);
        keybinds.content_w = 100;
        @keybinds.render_fn = keybinds_render;
        @keybinds.update_fn = keybinds_update;

        widgets::initialize();
    }
}
