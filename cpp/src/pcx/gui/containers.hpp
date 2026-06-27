#pragma once

#include "pcx/gui/elements_all.hpp"

#include <memory>
#include <string>
#include <vector>

class CContainer {
public:
    std::string name;
    std::string config_prefix;
    int column = 1;
    float height_override = 0.f;
    float scroll_offset = 0.f;
    std::vector<std::shared_ptr<CElement>> elements;
    pcx::VisibilityFn visibility_fn;

    CContainer() = default;
    CContainer(const std::string& n, int col, float height = 0.f, const std::string& prefix = "")
        : name(n), column(col), height_override(height), config_prefix(prefix) {}

    void set_visible(pcx::VisibilityFn fn) { visibility_fn = std::move(fn); }
    bool is_visible() const { return visibility_fn ? visibility_fn() : true; }

    void register_element(const std::string& element_name, const std::shared_ptr<CElement>& el);

    std::shared_ptr<CCheckbox> create_checkbox(const std::string& n, bool def = false);
    std::shared_ptr<CSliderInt> create_slider_int(const std::string& n, int min, int max, int def, int step = 1, const std::string& suffix = "");
    std::shared_ptr<CSliderFloat> create_slider_float(const std::string& n, float min, float max, float def, float step = 0.1f, int prec = 1, const std::string& suffix = "");
    std::shared_ptr<CKeybind> create_keybind(const std::string& n, int key = 0, pcx::EKeybindMode mode = pcx::EKeybindMode::HOLD, bool def_state = false);
    std::shared_ptr<CDropdown> create_dropdown(const std::string& n, const std::vector<std::string>& opts, int def = 0);
    std::shared_ptr<CMultiSelect> create_multiselect(const std::string& n, const std::vector<std::string>& opts);
    std::shared_ptr<CTextInput> create_text_input(const std::string& n, const std::string& def = "", const std::string& ph = "", int max_len = 128);
    std::shared_ptr<CColorPicker> create_colorpicker(const std::string& n, pcx::Color def = {255, 255, 255, 255});
    std::shared_ptr<CButton> create_button(const std::string& n, pcx::ButtonFn cb = nullptr);
    std::shared_ptr<CListBox> create_listbox(const std::string& n, bool multi = true, float max_h = 120.f, bool search = true);
    std::shared_ptr<CLabel> create_label(const std::string& n);

    float get_content_height() const;
    void tick();
    void update(float x, float y, float w, float h);
    void render(float x, float y, float w, float h);
    void render_popups(float x, float y, float w, float h);
};

class CTab {
public:
    std::string name;
    bool active = false;
    std::vector<std::shared_ptr<CContainer>> containers;

    CTab() = default;
    explicit CTab(const std::string& n) : name(n) {}

    std::shared_ptr<CContainer> create_container(const std::string& n, int col, float height = 0.f, const std::string& prefix = "");
    void tick();
    void update(pcx::Vec2 position, pcx::Vec2 size);
    void render(pcx::Vec2 position, pcx::Vec2 size);
    void render_popups(pcx::Vec2 position, pcx::Vec2 size);

private:
    void update_column(std::vector<std::shared_ptr<CContainer>>& cols, float x, float y, float w, float h);
    void render_column(std::vector<std::shared_ptr<CContainer>>& cols, float x, float y, float w, float h);
    void render_column_popups(std::vector<std::shared_ptr<CContainer>>& cols, float x, float y, float w, float h);
};

class CWindow {
public:
    std::string title;
    pcx::Vec2 position;
    pcx::Vec2 size;
    pcx::Vec2 min_size;
    bool dragging = false;
    bool resizing = false;
    pcx::Vec2 offset;
    std::vector<std::shared_ptr<CTab>> tabs;
    int active_tab = 0;

    CWindow() = default;
    CWindow(const std::string& t, pcx::Vec2 pos, pcx::Vec2 sz) : title(t), position(pos), size(sz), min_size(sz) {}

    std::shared_ptr<CTab> create_tab(const std::string& n);
    void tick();
    void handle_input();
    void handle_tabs();
    void update();
    void render();
    void render_popups();
    void render_tabs();

    static pcx::Vec2 get_viewport();
};
