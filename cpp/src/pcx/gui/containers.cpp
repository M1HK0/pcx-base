#include "pcx/gui/containers.hpp"
#include "pcx/gui/config_system.hpp"
#include "pcx/gui/identity.hpp"
#include "pcx/input.hpp"
#include "pcx/renderer.hpp"

#include <imgui.h>

// ===========================================================================
// CContainer
// ===========================================================================
void CContainer::register_element(const std::string& element_name, const std::shared_ptr<CElement>& el) {
    if (!config_prefix.empty())
        gui::config_system::register_element(config_prefix + "." + element_name, el);
}

std::shared_ptr<CCheckbox> CContainer::create_checkbox(const std::string& n, bool def) {
    auto cb = std::make_shared<CCheckbox>(n, def);
    auto el = std::make_shared<CElement>(cb);
    elements.push_back(el);
    if (!config_prefix.empty()) cb->config_key = config_prefix + "." + n;
    register_element(n, el);
    return cb;
}

std::shared_ptr<CSliderInt> CContainer::create_slider_int(const std::string& n, int min, int max, int def, int step, const std::string& suffix) {
    auto s = std::make_shared<CSliderInt>(n, min, max, def, step, suffix);
    auto el = std::make_shared<CElement>(s);
    elements.push_back(el);
    if (!config_prefix.empty()) s->config_key = config_prefix + "." + n;
    register_element(n, el);
    return s;
}

std::shared_ptr<CSliderFloat> CContainer::create_slider_float(const std::string& n, float min, float max, float def, float step, int prec, const std::string& suffix) {
    auto s = std::make_shared<CSliderFloat>(n, min, max, def, step, prec, suffix);
    auto el = std::make_shared<CElement>(s);
    elements.push_back(el);
    if (!config_prefix.empty()) s->config_key = config_prefix + "." + n;
    register_element(n, el);
    return s;
}

std::shared_ptr<CKeybind> CContainer::create_keybind(const std::string& n, int key, pcx::EKeybindMode mode, bool def_state) {
    auto kb = std::make_shared<CKeybind>(n, key, mode, def_state);
    auto el = std::make_shared<CElement>(kb);
    elements.push_back(el);
    register_element(n, el);
    return kb;
}

std::shared_ptr<CDropdown> CContainer::create_dropdown(const std::string& n, const std::vector<std::string>& opts, int def) {
    auto d = std::make_shared<CDropdown>(n, opts, def);
    auto el = std::make_shared<CElement>(d);
    elements.push_back(el);
    register_element(n, el);
    return d;
}

std::shared_ptr<CMultiSelect> CContainer::create_multiselect(const std::string& n, const std::vector<std::string>& opts) {
    auto m = std::make_shared<CMultiSelect>(n, opts);
    auto el = std::make_shared<CElement>(m);
    elements.push_back(el);
    register_element(n, el);
    return m;
}

std::shared_ptr<CTextInput> CContainer::create_text_input(const std::string& n, const std::string& def, const std::string& ph, int max_len) {
    auto t = std::make_shared<CTextInput>(n, def, ph, max_len);
    auto el = std::make_shared<CElement>(t);
    elements.push_back(el);
    register_element(n, el);
    return t;
}

std::shared_ptr<CColorPicker> CContainer::create_colorpicker(const std::string& n, pcx::Color def) {
    auto cp = std::make_shared<CColorPicker>(n, def);
    auto el = std::make_shared<CElement>(cp);
    elements.push_back(el);
    register_element(n, el);
    return cp;
}

std::shared_ptr<CButton> CContainer::create_button(const std::string& n, pcx::ButtonFn cb) {
    auto b = std::make_shared<CButton>(n, std::move(cb));
    elements.push_back(std::make_shared<CElement>(b));
    return b;
}

std::shared_ptr<CListBox> CContainer::create_listbox(const std::string& n, bool multi, float max_h, bool search) {
    auto lb = std::make_shared<CListBox>(n, multi, max_h, search);
    elements.push_back(std::make_shared<CElement>(lb));
    return lb;
}

std::shared_ptr<CLabel> CContainer::create_label(const std::string& n) {
    auto l = std::make_shared<CLabel>(n);
    elements.push_back(std::make_shared<CElement>(l));
    if (!config_prefix.empty()) l->config_key = config_prefix + "." + n;
    return l;
}

float CContainer::get_content_height() const {
    float total = 0.f;
    bool first = true;
    for (const auto& el : elements) {
        if (!el->is_visible()) continue;
        if (!first) total += 8.f;
        total += el->get_height();
        first = false;
    }
    return total;
}

void CContainer::tick() {
    for (auto& el : elements) el->tick();
}

void CContainer::update(float x, float y, float w, float h) {
    const float content_y = y + 10.f;
    const float content_h = h - 10.f;
    const float total_h = get_content_height();
    const float max_scroll = total_h > content_h ? total_h - content_h : 0.f;
    scroll_offset = pcx::clamp(scroll_offset, 0.f, max_scroll);
    if (pcx::cursor::is_hovered({x, content_y}, {w, content_h}) && total_h > content_h) {
        const float delta = pcx::input::get_scroll_delta();
        if (delta != 0.f) scroll_offset = pcx::clamp(scroll_offset - (delta > 0.f ? 20.f : -20.f), 0.f, max_scroll);
    }
    float element_x = x + 6.f;
    float element_y = content_y - scroll_offset;
    const float element_w = w - 12.f;
    int blocking_index = -1;
    for (size_t i = 0; i < elements.size(); ++i) {
        if (!elements[i]->is_visible()) continue;
        if (elements[i]->is_blocking()) { blocking_index = static_cast<int>(i); break; }
    }
    for (size_t i = 0; i < elements.size(); ++i) {
        if (!elements[i]->is_visible()) continue;
        if (blocking_index == -1 || static_cast<int>(i) <= blocking_index)
            elements[i]->handle_input(element_x, element_y, element_w);
        element_y += elements[i]->get_height() + 8.f;
    }
}

void CContainer::render(float x, float y, float w, float h) {
    const float rounding = 4.f;
    pcx::renderer::rect::draw_filled_rounded({x, y}, {w, h}, gui::colors::scaled(gui::colors::background_secondary), rounding);
    pcx::renderer::rect::draw({x, y}, {w, h}, gui::colors::scaled(gui::colors::border), 1.f, rounding,
        pcx::RR_TOP_LEFT | pcx::RR_TOP_RIGHT | pcx::RR_BOTTOM_LEFT | pcx::RR_BOTTOM_RIGHT);
    const auto name_size = pcx::renderer::text::get_size(name, pcx::EFont::TAHOMABD);
    const float name_x = x + 22.f;
    const float name_pad = 4.f;
    pcx::renderer::rect::draw_filled({name_x - name_pad, y - 1.f}, {name_size.x + name_pad * 2.f, 3.f}, gui::colors::scaled(gui::colors::background_secondary));
    pcx::renderer::text::draw_outlined(name, {name_x, y - name_size.y / 2.f}, gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMABD);
    const float content_y = y + 10.f;
    const float content_h = h - 10.f;
    const float total_h = get_content_height();
    pcx::renderer::clip_push(static_cast<int>(x), static_cast<int>(content_y), static_cast<int>(w), static_cast<int>(content_h));
    float element_x = x + 6.f;
    float element_y = content_y - scroll_offset;
    const float element_w = w - 12.f;
    for (const auto& el : elements) {
        if (!el->is_visible()) continue;
        el->render(element_x, element_y, element_w);
        element_y += el->get_height() + 8.f;
    }
    pcx::renderer::clip_pop();
    if (total_h > content_h) {
        const float bar_h = (content_h / total_h) * content_h;
        const float bar_y = content_y + (scroll_offset / total_h) * content_h;
        pcx::renderer::rect::draw_filled({x + w - 4.f, bar_y}, {2.f, bar_h}, gui::colors::scaled(gui::colors::border));
    }
}

void CContainer::render_popups(float x, float y, float w, float h) {
    const float content_y = y + 10.f;
    const float max_y = y + h;
    float element_x = x + 6.f;
    float element_y = content_y - scroll_offset;
    const float element_w = w - 12.f;
    for (const auto& el : elements) {
        if (!el->is_visible()) continue;
        el->render_popup(element_x, element_y, element_w, max_y);
        element_y += el->get_height() + 8.f;
    }
}

// ===========================================================================
// CTab
// ===========================================================================
std::shared_ptr<CContainer> CTab::create_container(const std::string& n, int col, float height, const std::string& prefix) {
    auto c = std::make_shared<CContainer>(n, col, height, prefix);
    containers.push_back(c);
    return c;
}

void CTab::tick() {
    for (auto& c : containers) c->tick();
}

void CTab::update_column(std::vector<std::shared_ptr<CContainer>>& cols, float x, float y, float w, float h) {
    if (cols.empty()) return;
    int auto_count = 0;
    float fixed_total = 0.f;
    for (const auto& c : cols) {
        if (c->height_override > 0.f) fixed_total += c->height_override;
        else ++auto_count;
    }
    const float gap = 8.f;
    const float total_gaps = (static_cast<float>(cols.size()) - 1.f) * gap;
    const float auto_h = auto_count > 0 ? (h - fixed_total - total_gaps) / static_cast<float>(auto_count) : 0.f;
    float current_y = y;
    for (const auto& c : cols) {
        const float ch = c->height_override > 0.f ? c->height_override : auto_h;
        c->update(x, current_y, w, ch);
        current_y += ch + gap;
    }
}

void CTab::update(pcx::Vec2 position, pcx::Vec2 size) {
    if (containers.empty()) return;
    const float pad = 8.f;
    const float header_h = 30.f;
    const float content_x = position.x + pad;
    const float content_y = position.y + header_h + pad;
    const float content_w = size.x - pad * 2.f;
    const float content_h = size.y - header_h - pad * 2.f;
    const float gap = pad;
    const float col_w = (content_w - gap) / 2.f;
    std::vector<std::shared_ptr<CContainer>> col1, col2;
    for (const auto& c : containers) {
        if (!c->is_visible()) continue;
        if (c->column == 1) col1.push_back(c);
        else col2.push_back(c);
    }
    update_column(col1, content_x, content_y, col_w, content_h);
    update_column(col2, content_x + col_w + gap, content_y, col_w, content_h);
}

void CTab::render_column(std::vector<std::shared_ptr<CContainer>>& cols, float x, float y, float w, float h) {
    if (cols.empty()) return;
    int auto_count = 0;
    float fixed_total = 0.f;
    for (const auto& c : cols) {
        if (c->height_override > 0.f) fixed_total += c->height_override;
        else ++auto_count;
    }
    const float gap = 8.f;
    const float total_gaps = (static_cast<float>(cols.size()) - 1.f) * gap;
    const float auto_h = auto_count > 0 ? (h - fixed_total - total_gaps) / static_cast<float>(auto_count) : 0.f;
    float current_y = y;
    for (const auto& c : cols) {
        const float ch = c->height_override > 0.f ? c->height_override : auto_h;
        c->render(x, current_y, w, ch);
        current_y += ch + gap;
    }
}

void CTab::render(pcx::Vec2 position, pcx::Vec2 size) {
    if (containers.empty()) return;
    const float pad = 8.f;
    const float header_h = 30.f;
    const float content_x = position.x + pad;
    const float content_y = position.y + header_h + pad;
    const float content_w = size.x - pad * 2.f;
    const float content_h = size.y - header_h - pad * 2.f;
    const float gap = pad;
    const float col_w = (content_w - gap) / 2.f;
    std::vector<std::shared_ptr<CContainer>> col1, col2;
    for (const auto& c : containers) {
        if (!c->is_visible()) continue;
        if (c->column == 1) col1.push_back(c);
        else col2.push_back(c);
    }
    render_column(col1, content_x, content_y, col_w, content_h);
    render_column(col2, content_x + col_w + gap, content_y, col_w, content_h);
}

void CTab::render_column_popups(std::vector<std::shared_ptr<CContainer>>& cols, float x, float y, float w, float h) {
    if (cols.empty()) return;
    int auto_count = 0;
    float fixed_total = 0.f;
    for (const auto& c : cols) {
        if (c->height_override > 0.f) fixed_total += c->height_override;
        else ++auto_count;
    }
    const float gap = 8.f;
    const float total_gaps = (static_cast<float>(cols.size()) - 1.f) * gap;
    const float auto_h = auto_count > 0 ? (h - fixed_total - total_gaps) / static_cast<float>(auto_count) : 0.f;
    float current_y = y;
    for (const auto& c : cols) {
        const float ch = c->height_override > 0.f ? c->height_override : auto_h;
        c->render_popups(x, current_y, w, ch);
        current_y += ch + gap;
    }
}

void CTab::render_popups(pcx::Vec2 position, pcx::Vec2 size) {
    if (containers.empty()) return;
    const float pad = 8.f;
    const float header_h = 30.f;
    const float content_x = position.x + pad;
    const float content_y = position.y + header_h + pad;
    const float content_w = size.x - pad * 2.f;
    const float content_h = size.y - header_h - pad * 2.f;
    const float gap = pad;
    const float col_w = (content_w - gap) / 2.f;
    std::vector<std::shared_ptr<CContainer>> col1, col2;
    for (const auto& c : containers) {
        if (!c->is_visible()) continue;
        if (c->column == 1) col1.push_back(c);
        else col2.push_back(c);
    }
    render_column_popups(col1, content_x, content_y, col_w, content_h);
    render_column_popups(col2, content_x + col_w + gap, content_y, col_w, content_h);
}

// ===========================================================================
// CWindow
// ===========================================================================
pcx::Vec2 CWindow::get_viewport() {
    if (ImGui::GetCurrentContext())
        return {ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y};
    return {1280.f, 720.f};
}

std::shared_ptr<CTab> CWindow::create_tab(const std::string& n) {
    auto tab = std::make_shared<CTab>(n);
    if (tabs.empty()) tab->active = true;
    tabs.push_back(tab);
    return tab;
}

void CWindow::tick() {
    for (auto& t : tabs) t->tick();
}

void CWindow::handle_input() {
    if (pcx::cursor::m1_pressed && !dragging && !resizing) {
        if (pcx::cursor::is_hovered(position + size - pcx::Vec2{20.f, 20.f}, {20.f, 20.f})) {
            resizing = true;
            offset = pcx::cursor::pos() - (position + size);
        } else if (pcx::cursor::is_hovered(position, {size.x, 30.f})) {
            dragging = true;
            offset = pcx::cursor::pos() - position;
        }
    }
    if (!pcx::cursor::m1_down) {
        dragging = resizing = false;
        return;
    }
    const auto vp = get_viewport();
    if (dragging) {
        float new_x = pcx::cursor::x - offset.x;
        float new_y = pcx::cursor::y - offset.y;
        new_x = pcx::clamp(new_x, 0.f, vp.x - size.x);
        new_y = pcx::clamp(new_y, 0.f, vp.y - size.y);
        position = {new_x, new_y};
    }
    if (resizing) {
        float new_w = pcx::cursor::x - offset.x - position.x;
        float new_h = pcx::cursor::y - offset.y - position.y;
        new_w = pcx::clamp(new_w, min_size.x, vp.x - position.x);
        new_h = pcx::clamp(new_h, min_size.y, vp.y - position.y);
        size = {new_w, new_h};
    }
}

void CWindow::handle_tabs() {
    if (tabs.empty()) return;
    const auto title_size = pcx::renderer::text::get_size(title, pcx::EFont::TAHOMABD);
    float tab_x = position.x + title_size.x + 18.f;
    const float tab_y = position.y + 10.f;
    for (size_t i = 0; i < tabs.size(); ++i) {
        if (i > 0) tab_x += 6.f;
        const auto text_size = pcx::renderer::text::get_size(tabs[i]->name, pcx::EFont::TAHOMA);
        if (pcx::cursor::m1_pressed && pcx::cursor::is_hovered({tab_x, tab_y}, text_size)) {
            for (auto& t : tabs) t->active = false;
            tabs[i]->active = true;
            active_tab = static_cast<int>(i);
        }
        tab_x += text_size.x;
    }
}

void CWindow::update() {
    handle_input();
    handle_tabs();
    if (!tabs.empty())
        tabs[active_tab]->update(position, size);
}

void CWindow::render_tabs() {
    if (tabs.empty()) return;
    const auto title_size = pcx::renderer::text::get_size(title, pcx::EFont::TAHOMABD);
    float tab_x = position.x + title_size.x + 18.f;
    const float tab_y = position.y + 10.f;
    for (size_t i = 0; i < tabs.size(); ++i) {
        if (i > 0) tab_x += 6.f;
        const auto text_size = pcx::renderer::text::get_size(tabs[i]->name, pcx::EFont::TAHOMA);
        const bool hovered = pcx::cursor::is_hovered({tab_x, tab_y}, text_size);
        pcx::Color col = gui::colors::scaled(gui::colors::text_inactive);
        if (tabs[i]->active) col = gui::colors::scaled(gui::colors::text_active);
        else if (hovered) col = gui::colors::scaled(gui::colors::text_hover);
        pcx::renderer::text::draw_shadowed(tabs[i]->name, {tab_x, tab_y}, col, gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
        if (tabs[i]->active) {
            pcx::renderer::rect::draw_gradient({tab_x, tab_y + text_size.y + 2.f}, {text_size.x, 1.f},
                gui::colors::scaled(gui::colors::accent_primary), gui::colors::scaled(gui::colors::accent_secondary),
                gui::colors::scaled(gui::colors::accent_primary), gui::colors::scaled(gui::colors::accent_secondary));
        }
        tab_x += text_size.x;
    }
}

void CWindow::render() {
    const float header_h = 30.f;
    pcx::renderer::rect::draw_filled_rounded(position, {size.x, header_h}, gui::colors::scaled(gui::colors::background_secondary), 4.f, pcx::RR_TOP_LEFT | pcx::RR_TOP_RIGHT);
    pcx::renderer::rect::draw_filled_rounded({position.x, position.y + header_h}, {size.x, size.y - header_h}, gui::colors::scaled(gui::colors::background_primary), 4.f, pcx::RR_BOTTOM_LEFT | pcx::RR_BOTTOM_RIGHT);
    pcx::renderer::rect::draw({position.x + 1.f, position.y + 1.f}, {size.x - 2.f, size.y - 2.f}, gui::colors::scaled(gui::colors::border), 1.f, 4.f,
        pcx::RR_TOP_LEFT | pcx::RR_TOP_RIGHT | pcx::RR_BOTTOM_LEFT | pcx::RR_BOTTOM_RIGHT);
    pcx::renderer::line::draw({position.x + 4.f, position.y + 4.f}, {position.x + 4.f, position.y + header_h - 4.f}, gui::colors::scaled(gui::colors::accent_primary));
    pcx::renderer::text::draw_outlined(title, {position.x + 12.f, position.y + 10.f}, gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMABD);
    render_tabs();

    const std::string& uname = pcx::globals::username;
    const std::string role_name = gui::identity::get_role_name();
    const std::string sep = " - ";
    const auto uname_size = pcx::renderer::text::get_size(uname, pcx::EFont::TAHOMA);
    float total_w = uname_size.x;
    pcx::Vec2 sep_size{}, role_size{};
    if (!role_name.empty()) {
        sep_size = pcx::renderer::text::get_size(sep, pcx::EFont::TAHOMA);
        role_size = pcx::renderer::text::get_size(role_name, pcx::EFont::TAHOMA);
        total_w += sep_size.x + role_size.x;
    }
    const float id_x = position.x + size.x - total_w - 12.f;
    const float id_y = position.y + 10.f;
    pcx::renderer::text::draw_shadowed(uname, {id_x, id_y}, gui::colors::scaled(gui::colors::text_active), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    if (!role_name.empty()) {
        pcx::renderer::text::draw_shadowed(sep, {id_x + uname_size.x, id_y}, gui::colors::scaled(gui::colors::text_inactive), gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
        auto role_col = gui::identity::get_role_color();
        role_col.a = pcx::globals::menu_alpha;
        pcx::renderer::text::draw_shadowed(role_name, {id_x + uname_size.x + sep_size.x, id_y}, role_col, gui::colors::scaled(gui::colors::outline), pcx::EFont::TAHOMA);
    }

    if (!tabs.empty())
        tabs[active_tab]->render(position, size);

    const float rx = position.x + size.x - 4.f;
    const float ry = position.y + size.y - 4.f;
    pcx::renderer::line::draw({rx - 6.f, ry}, {rx, ry - 6.f}, gui::colors::scaled(gui::colors::border));
    pcx::renderer::line::draw({rx - 3.f, ry}, {rx, ry - 3.f}, gui::colors::scaled(gui::colors::border));
}

void CWindow::render_popups() {
    if (!tabs.empty())
        tabs[active_tab]->render_popups(position, size);
}
