namespace gui {
    CWindow window;
    float open_anim = 0.0f;

    namespace config_tab {
        CListBox@ config_list;
        CTextInput@ config_name;
    }

    namespace theme_tab {
        CListBox@ theme_list;
        CTextInput@ theme_name;
    }

    void create_misc(CTab@ tab) {

        auto settings = tab.create_container("settings", 1, 0, "misc.settings");
        @gui::config::settings::menu_key = settings.create_keybind("menu toggle", 0x2D, EKeybindMode::TOGGLE, true);
        gui::config::settings::menu_key.register_hotkey("menu");

        @gui::config::settings::streamproof_mode = settings.create_dropdown("force streamproof mode", {"default", "perceptproof"}, 1);

        auto widgets_lbl = settings.create_label("widgets");
        auto widgets_cog = widgets_lbl.create_cogwheel("widgets options");
        @gui::config::settings::widgets::watermark = widgets_cog.create_checkbox("watermark", true);
        @gui::config::settings::widgets::hotkeys = widgets_cog.create_checkbox("hotkeys", false);
        

        auto xhair = tab.create_container("crosshair", 2, 0, "misc.crosshair");
        @gui::config::crosshair::enabled = xhair.create_checkbox("enabled", false);
        @gui::config::crosshair::color = gui::config::crosshair::enabled.create_colorpicker("crosshair color", color_t(255, 255, 255, 255));
        @gui::config::crosshair::outline = gui::config::crosshair::enabled.create_colorpicker("crosshair outline color", color_t(0, 0, 0, 255));
        gui::config::crosshair::enabled.create_tooltip("renders a crosshair at the center of the target window");

        @gui::config::crosshair::style = xhair.create_dropdown("style", {"cross", "circle"}, 0);
        @gui::config::crosshair::size = xhair.create_slider_int("size", 1, 20, 4, 1, "px");
        @gui::config::crosshair::thickness = xhair.create_slider_int("thickness", 1, 5, 1, 1, "px");

        @gui::config::crosshair::gap = xhair.create_slider_int("gap", 0, 10, 2, 1, "px");
        gui::config::crosshair::gap.set_visible(function() { return gui::config::crosshair::style.get() == 0; });

        @gui::config::crosshair::t_style = xhair.create_checkbox("t-style", false);
        gui::config::crosshair::t_style.set_visible(function() { return gui::config::crosshair::style.get() == 0; });

        @gui::config::crosshair::center_dot = xhair.create_checkbox("center dot", false);
        @gui::config::crosshair::dot_size = xhair.create_slider_int("dot size", 1, 5, 2, 1, "px");
        gui::config::crosshair::dot_size.set_visible(function() { return gui::config::crosshair::center_dot.get(); });
    }

    void create_configs(CTab@ tab) {
        auto cfg_container = tab.create_container("configs", 1);

        @gui::config_tab::config_list = cfg_container.create_listbox("config list", false, 150, true);

        cfg_container.create_button("save", function() {
            array<string> sel = gui::config_tab::config_list.get_selected_strings();
            if (sel.length() == 0) { notifications::create_error("no config selected"); return; }
            gui::config_system::save(sel[0]);
            notifications::create_success("config saved: " + sel[0]);
        });

        cfg_container.create_button("export", function() {
            array<string> sel = gui::config_tab::config_list.get_selected_strings();
            if (sel.length() == 0) { notifications::create_error("no config selected"); return; }
            string json;
            if (!read_file(gui::config_system::base_path + "\\" + sel[0] + ".cfg", json)) { notifications::create_error("failed to read config"); return; }
            copy_to_clipboard(json);
            notifications::create_success("config exported to clipboard");
        });

        cfg_container.create_button("load", function() {
            array<string> sel = gui::config_tab::config_list.get_selected_strings();
            if (sel.length() == 0) { notifications::create_error("no config selected"); return; }
            gui::config_system::load(sel[0]);
            notifications::create_success("config loaded: " + sel[0]);
        });

        cfg_container.create_button("delete", function() {
            array<string> sel = gui::config_tab::config_list.get_selected_strings();
            if (sel.length() == 0) { notifications::create_error("no config selected"); return; }
            gui::config_system::remove(sel[0]);
            gui::config_system::refresh_listbox(gui::config_tab::config_list);
            notifications::create_success("config deleted: " + sel[0]);
        });

        @gui::config_tab::config_name = cfg_container.create_text_input("config name", "", "enter name...", 32);

        cfg_container.create_button("create", function() {
            string name = gui::config_tab::config_name.get();
            if (name.length() == 0) { notifications::create_error("enter a config name"); return; }
            gui::config_system::save(name);
            gui::config_system::refresh_listbox(gui::config_tab::config_list);
            gui::config_tab::config_name.value = "";
            notifications::create_success("config created: " + name);
        });

        cfg_container.create_button("import", function() {
            string clip;
            copy_from_clipboard(clip);
            if (clip.length() == 0) { notifications::create_error("clipboard is empty"); return; }
            string name = gui::config_tab::config_name.get();
            if (name.length() == 0) { notifications::create_error("enter a config name"); return; }
            create_file(gui::config_system::base_path + "\\" + name + ".cfg", clip);
            gui::config_system::refresh_listbox(gui::config_tab::config_list);
            gui::config_tab::config_name.value = "";
            notifications::create_success("config imported: " + name);
            gui::config_system::load(name);
        });

        gui::config_system::refresh_listbox(gui::config_tab::config_list);
    }

    void create_themes(CTab@ tab) {
        auto thm_container = tab.create_container("themes", 1);

        @gui::theme_tab::theme_list = thm_container.create_listbox("theme list", false, 150, true);

        thm_container.create_button("save", function() {
            array<string> sel = gui::theme_tab::theme_list.get_selected_strings();
            if (sel.length() == 0) { notifications::create_error("no theme selected"); return; }
            gui::theme_system::save(sel[0]);
            notifications::create_success("theme saved: " + sel[0]);
        });

        thm_container.create_button("export", function() {
            array<string> sel = gui::theme_tab::theme_list.get_selected_strings();
            if (sel.length() == 0) { notifications::create_error("no theme selected"); return; }
            string data;
            if (!read_file(gui::theme_system::base_path + "\\" + sel[0] + ".theme", data)) { notifications::create_error("failed to read theme"); return; }
            copy_to_clipboard(data);
            notifications::create_success("theme exported to clipboard");
        });

        thm_container.create_button("load", function() {
            array<string> sel = gui::theme_tab::theme_list.get_selected_strings();
            if (sel.length() == 0) { notifications::create_error("no theme selected"); return; }
            gui::theme_system::load(sel[0]);
            notifications::create_success("theme loaded: " + sel[0]);
        });

        thm_container.create_button("delete", function() {
            array<string> sel = gui::theme_tab::theme_list.get_selected_strings();
            if (sel.length() == 0) { notifications::create_error("no theme selected"); return; }
            gui::theme_system::remove(sel[0]);
            gui::theme_system::refresh_listbox(gui::theme_tab::theme_list);
            notifications::create_success("theme deleted: " + sel[0]);
        });

        @gui::theme_tab::theme_name = thm_container.create_text_input("theme name", "", "enter name...", 32);

        thm_container.create_button("create", function() {
            string name = gui::theme_tab::theme_name.get();
            if (name.length() == 0) { notifications::create_error("enter a theme name"); return; }
            gui::theme_system::save(name);
            gui::theme_system::refresh_listbox(gui::theme_tab::theme_list);
            gui::theme_tab::theme_name.value = "";
            notifications::create_success("theme created: " + name);
        });

        thm_container.create_button("import", function() {
            string clip;
            copy_from_clipboard(clip);
            if (clip.length() == 0) { notifications::create_error("clipboard is empty"); return; }
            string name = gui::theme_tab::theme_name.get();
            if (name.length() == 0) { notifications::create_error("enter a theme name"); return; }
            create_file(gui::theme_system::base_path + "\\" + name + ".theme", clip);
            gui::theme_system::refresh_listbox(gui::theme_tab::theme_list);
            gui::theme_tab::theme_name.value = "";
            notifications::create_success("theme imported: " + name);
            gui::theme_system::load(name);
        });

        gui::theme_system::refresh_listbox(gui::theme_tab::theme_list);

        auto thm_colors = tab.create_container("colors", 2);
        @gui::config::theme::accent_primary = thm_colors.create_colorpicker("accent primary", gui::colors::accent_primary);
        @gui::config::theme::accent_secondary = thm_colors.create_colorpicker("accent secondary", gui::colors::accent_secondary);
        @gui::config::theme::background_primary = thm_colors.create_colorpicker("background primary", gui::colors::background_primary);
        @gui::config::theme::background_secondary = thm_colors.create_colorpicker("background secondary", gui::colors::background_secondary);
        @gui::config::theme::text_active = thm_colors.create_colorpicker("text active", gui::colors::text_active);
        @gui::config::theme::text_inactive = thm_colors.create_colorpicker("text inactive", gui::colors::text_inactive);
        @gui::config::theme::border = thm_colors.create_colorpicker("border", gui::colors::border);
        @gui::config::theme::glow = thm_colors.create_colorpicker("glow", gui::colors::glow);

        gui::theme_system::register_element("colors.accent_primary", thm_colors.elements[0]);
        gui::theme_system::register_element("colors.accent_secondary", thm_colors.elements[1]);
        gui::theme_system::register_element("colors.background_primary", thm_colors.elements[2]);
        gui::theme_system::register_element("colors.background_secondary", thm_colors.elements[3]);
        gui::theme_system::register_element("colors.text_active", thm_colors.elements[4]);
        gui::theme_system::register_element("colors.text_inactive", thm_colors.elements[5]);
        gui::theme_system::register_element("colors.border", thm_colors.elements[6]);
        gui::theme_system::register_element("colors.glow", thm_colors.elements[7]);
    }

    void initialize(const string &in title, vector2 size) {
        vector2 vp = window::get_viewport();
        vector2 pos = vector2((vp.x - size.x) / 2, (vp.y - size.y) / 2);
        window = CWindow(title, pos, size);

        gui::config_system::initialize();
        gui::theme_system::initialize();
        identity::initialize();

        if (!does_file_exist(gui::config_system::base_path + "\\.initialized")) {
            notifications::create_persistent("welcome to perception! click x to dismiss");
            notifications::create("press 'insert' to toggle menu state");
            create_file(gui::config_system::base_path + "\\.initialized", "");
        }

        create_misc(window.create_tab("miscellaneous"));
        create_configs(window.create_tab("configs"));
        create_themes(window.create_tab("themes"));

        if (does_file_exist(gui::config_system::base_path + "\\default.cfg")) {
            gui::config_system::load("default");
            notifications::create("default config loaded");
        }

        if (does_file_exist(gui::theme_system::base_path + "\\default.theme")) {
            gui::theme_system::load("default");
            notifications::create("default theme loaded");
        }
    }

    void sync_colors() {
        gui::colors::accent_primary = gui::config::theme::accent_primary.get();
        gui::colors::accent_secondary = gui::config::theme::accent_secondary.get();
        gui::colors::background_primary = gui::config::theme::background_primary.get();
        gui::colors::background_secondary = gui::config::theme::background_secondary.get();
        gui::colors::text_active = gui::config::theme::text_active.get();
        gui::colors::text_inactive = gui::config::theme::text_inactive.get();
        gui::colors::border = gui::config::theme::border.get();
        gui::colors::glow = gui::config::theme::glow.get();
    }

    void tick() {
        window.tick();
    }

    void update() {
        float target = gui::config::settings::menu_key.is_active() ? 1.0f : 0.0f;
        open_anim = lerp(open_anim, target, 0.03f);
        if (open_anim < 0.01f) open_anim = 0.0f;
        if (open_anim > 0.99f) open_anim = 1.0f;
        globals::menu_alpha = uint8(255 * smoothstep(0.0f, 1.0f, open_anim));

        if (gui::config::settings::menu_key.is_active()) {
            sync_colors();
            window.update();
        }

    }

    void render() {
        if (open_anim <= 0.0f) return;

        float cx = window.position.x + window.size.x / 2;
        float cy = window.position.y + window.size.y / 2;
        float anim_w = window.size.x * open_anim;
        float anim_h = window.size.y * open_anim;
        float anim_x = cx - anim_w / 2;
        float anim_y = cy - anim_h / 2;

        renderer::rect::draw_glow(vector2(anim_x, anim_y), vector2(anim_w, anim_h), gui::colors::glow.scaled(), 4.0f, 4);

        clip_push(int(anim_x), int(anim_y), int(anim_w), int(anim_h));
        window.render();
        clip_pop();

        if (open_anim > 0.5f)
            window.render_popups();
    }
}