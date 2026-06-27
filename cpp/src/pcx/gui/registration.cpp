#include "pcx/gui/gui.hpp"

#include "pcx/file_util.hpp"
#include "pcx/gui/config_system.hpp"
#include "pcx/gui/identity.hpp"
#include "pcx/gui/notifications.hpp"
#include "pcx/gui/theme_system.hpp"
#include "pcx/gui/widgets.hpp"

namespace gui {

static void create_misc(const std::shared_ptr<CTab>& tab) {
    auto settings = tab->create_container("settings", 1, 0.f, "misc.settings");
    config::settings::menu_key = settings->create_keybind("menu toggle", 0x2D, pcx::EKeybindMode::TOGGLE, true);
    config::settings::menu_key->register_hotkey("menu");

    config::settings::streamproof_mode = settings->create_dropdown("force streamproof mode", {"default", "perceptproof"}, 1);

    auto widgets_lbl = settings->create_label("widgets");
    auto widgets_cog = widgets_lbl->create_cogwheel("widgets options");
    config::settings::widgets::watermark = widgets_cog->create_checkbox("watermark", true);
    config::settings::widgets::hotkeys = widgets_cog->create_checkbox("hotkeys", false);

    auto xhair = tab->create_container("crosshair", 2, 0.f, "misc.crosshair");
    config::crosshair::enabled = xhair->create_checkbox("enabled", false);
    config::crosshair::color = config::crosshair::enabled->create_colorpicker("crosshair color", {255, 255, 255, 255});
    config::crosshair::outline = config::crosshair::enabled->create_colorpicker("crosshair outline color", {0, 0, 0, 255});
    config::crosshair::enabled->create_tooltip("renders a crosshair at the center of the target window");

    config::crosshair::style = xhair->create_dropdown("style", {"cross", "circle"}, 0);
    config::crosshair::size = xhair->create_slider_int("size", 1, 20, 4, 1, "px");
    config::crosshair::thickness = xhair->create_slider_int("thickness", 1, 5, 1, 1, "px");

    config::crosshair::gap = xhair->create_slider_int("gap", 0, 10, 2, 1, "px");
    config::crosshair::gap->set_visible([] { return config::crosshair::style && config::crosshair::style->get() == 0; });

    config::crosshair::t_style = xhair->create_checkbox("t-style", false);
    config::crosshair::t_style->set_visible([] { return config::crosshair::style && config::crosshair::style->get() == 0; });

    config::crosshair::center_dot = xhair->create_checkbox("center dot", false);
    config::crosshair::dot_size = xhair->create_slider_int("dot size", 1, 5, 2, 1, "px");
    config::crosshair::dot_size->set_visible([] { return config::crosshair::center_dot && config::crosshair::center_dot->get(); });
}

static void create_configs(const std::shared_ptr<CTab>& tab) {
    auto cfg_container = tab->create_container("configs", 1);
    config_tab::config_list = cfg_container->create_listbox("config list", false, 150.f, true);

    cfg_container->create_button("save", [] {
        auto sel = config_tab::config_list->get_selected_strings();
        if (sel.empty()) { notifications::create_error("no config selected"); return; }
        config_system::save(sel[0]);
        notifications::create_success("config saved: " + sel[0]);
    });

    cfg_container->create_button("export", [] {
        auto sel = config_tab::config_list->get_selected_strings();
        if (sel.empty()) { notifications::create_error("no config selected"); return; }
        std::string json;
        if (!pcx::read_file(config_system::base_path + "\\" + sel[0] + ".cfg", json)) {
            notifications::create_error("failed to read config");
            return;
        }
        pcx::copy_to_clipboard(json);
        notifications::create_success("config exported to clipboard");
    });

    cfg_container->create_button("load", [] {
        auto sel = config_tab::config_list->get_selected_strings();
        if (sel.empty()) { notifications::create_error("no config selected"); return; }
        config_system::load(sel[0]);
        notifications::create_success("config loaded: " + sel[0]);
    });

    cfg_container->create_button("delete", [] {
        auto sel = config_tab::config_list->get_selected_strings();
        if (sel.empty()) { notifications::create_error("no config selected"); return; }
        config_system::remove(sel[0]);
        config_system::refresh_listbox(config_tab::config_list);
        notifications::create_success("config deleted: " + sel[0]);
    });

    config_tab::config_name = cfg_container->create_text_input("config name", "", "enter name...", 32);

    cfg_container->create_button("create", [] {
        const std::string name = config_tab::config_name->get();
        if (name.empty()) { notifications::create_error("enter a config name"); return; }
        config_system::save(name);
        config_system::refresh_listbox(config_tab::config_list);
        config_tab::config_name->value.clear();
        notifications::create_success("config created: " + name);
    });

    cfg_container->create_button("import", [] {
        std::string clip;
        if (!pcx::copy_from_clipboard(clip)) { notifications::create_error("clipboard is empty"); return; }
        const std::string name = config_tab::config_name->get();
        if (name.empty()) { notifications::create_error("enter a config name"); return; }
        pcx::create_file(config_system::base_path + "\\" + name + ".cfg", clip);
        config_system::refresh_listbox(config_tab::config_list);
        config_tab::config_name->value.clear();
        notifications::create_success("config imported: " + name);
        config_system::load(name);
    });

    config_system::refresh_listbox(config_tab::config_list);
}

static void create_themes(const std::shared_ptr<CTab>& tab) {
    auto thm_container = tab->create_container("themes", 1);
    theme_tab::theme_list = thm_container->create_listbox("theme list", false, 150.f, true);

    thm_container->create_button("save", [] {
        auto sel = theme_tab::theme_list->get_selected_strings();
        if (sel.empty()) { notifications::create_error("no theme selected"); return; }
        theme_system::save(sel[0]);
        notifications::create_success("theme saved: " + sel[0]);
    });

    thm_container->create_button("export", [] {
        auto sel = theme_tab::theme_list->get_selected_strings();
        if (sel.empty()) { notifications::create_error("no theme selected"); return; }
        std::string data;
        if (!pcx::read_file(theme_system::base_path + "\\" + sel[0] + ".theme", data)) {
            notifications::create_error("failed to read theme");
            return;
        }
        pcx::copy_to_clipboard(data);
        notifications::create_success("theme exported to clipboard");
    });

    thm_container->create_button("load", [] {
        auto sel = theme_tab::theme_list->get_selected_strings();
        if (sel.empty()) { notifications::create_error("no theme selected"); return; }
        theme_system::load(sel[0]);
        notifications::create_success("theme loaded: " + sel[0]);
    });

    thm_container->create_button("delete", [] {
        auto sel = theme_tab::theme_list->get_selected_strings();
        if (sel.empty()) { notifications::create_error("no theme selected"); return; }
        theme_system::remove(sel[0]);
        theme_system::refresh_listbox(theme_tab::theme_list);
        notifications::create_success("theme deleted: " + sel[0]);
    });

    theme_tab::theme_name = thm_container->create_text_input("theme name", "", "enter name...", 32);

    thm_container->create_button("create", [] {
        const std::string name = theme_tab::theme_name->get();
        if (name.empty()) { notifications::create_error("enter a theme name"); return; }
        theme_system::save(name);
        theme_system::refresh_listbox(theme_tab::theme_list);
        theme_tab::theme_name->value.clear();
        notifications::create_success("theme created: " + name);
    });

    thm_container->create_button("import", [] {
        std::string clip;
        if (!pcx::copy_from_clipboard(clip)) { notifications::create_error("clipboard is empty"); return; }
        const std::string name = theme_tab::theme_name->get();
        if (name.empty()) { notifications::create_error("enter a theme name"); return; }
        pcx::create_file(theme_system::base_path + "\\" + name + ".theme", clip);
        theme_system::refresh_listbox(theme_tab::theme_list);
        theme_tab::theme_name->value.clear();
        notifications::create_success("theme imported: " + name);
        theme_system::load(name);
    });

    theme_system::refresh_listbox(theme_tab::theme_list);

    auto thm_colors = tab->create_container("colors", 2);
    config::theme::accent_primary = thm_colors->create_colorpicker("accent primary", gui::colors::accent_primary);
    config::theme::accent_secondary = thm_colors->create_colorpicker("accent secondary", gui::colors::accent_secondary);
    config::theme::background_primary = thm_colors->create_colorpicker("background primary", gui::colors::background_primary);
    config::theme::background_secondary = thm_colors->create_colorpicker("background secondary", gui::colors::background_secondary);
    config::theme::text_active = thm_colors->create_colorpicker("text active", gui::colors::text_active);
    config::theme::text_inactive = thm_colors->create_colorpicker("text inactive", gui::colors::text_inactive);
    config::theme::border = thm_colors->create_colorpicker("border", gui::colors::border);
    config::theme::glow = thm_colors->create_colorpicker("glow", gui::colors::glow);

    theme_system::register_element("colors.accent_primary", thm_colors->elements.at(0));
    theme_system::register_element("colors.accent_secondary", thm_colors->elements.at(1));
    theme_system::register_element("colors.background_primary", thm_colors->elements.at(2));
    theme_system::register_element("colors.background_secondary", thm_colors->elements.at(3));
    theme_system::register_element("colors.text_active", thm_colors->elements.at(4));
    theme_system::register_element("colors.text_inactive", thm_colors->elements.at(5));
    theme_system::register_element("colors.border", thm_colors->elements.at(6));
    theme_system::register_element("colors.glow", thm_colors->elements.at(7));
}

void initialize(const std::string& title, pcx::Vec2 size) {
    const auto vp = CWindow::get_viewport();
    const pcx::Vec2 pos{(vp.x - size.x) / 2.f, (vp.y - size.y) / 2.f};
    window = CWindow(title, pos, size);

    config_system::initialize();
    theme_system::initialize();
    identity::initialize();

    if (!pcx::does_file_exist(config_system::base_path + "\\.initialized")) {
        notifications::create_persistent("welcome to perception! click x to dismiss");
        notifications::create("press 'insert' to toggle menu state");
        pcx::create_file(config_system::base_path + "\\.initialized", "");
    }

    create_misc(window.create_tab("miscellaneous"));
    create_configs(window.create_tab("configs"));
    create_themes(window.create_tab("themes"));

    if (pcx::does_file_exist(config_system::base_path + "\\default.cfg")) {
        config_system::load("default");
        notifications::create("default config loaded");
    }
    if (pcx::does_file_exist(theme_system::base_path + "\\default.theme")) {
        theme_system::load("default");
        notifications::create("default theme loaded");
    }
}

} // namespace gui
