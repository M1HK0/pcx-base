#pragma once

#include "pcx/types.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

class CWidget;

using WidgetRenderFn = std::function<void(CWidget&, float, float, float, float)>;
using WidgetTitleFn = std::function<void(CWidget&, float, float)>;
using WidgetUpdateFn = std::function<void(CWidget&)>;
using WidgetIconColorFn = std::function<pcx::Color()>;

class CWidget {
public:
    std::string id;
    std::string title;
    int pos_x = 10;
    int pos_y = 10;
    bool visible = true;
    bool dragging = false;
    float drag_ox = 0.f;
    float drag_oy = 0.f;
    float content_w = 0.f;
    float content_h = 0.f;
    float title_w_override = 0.f;
    WidgetIconColorFn icon_color_fn;
    WidgetRenderFn render_fn;
    WidgetTitleFn title_fn;
    WidgetUpdateFn update_fn;

    CWidget() = default;
    CWidget(const std::string& i, const std::string& t, int x, int y) : id(i), title(t), pos_x(x), pos_y(y) {}

    float get_header_h() const;
    float get_box_w() const;
    float get_box_h() const;
};

namespace widgets {

std::shared_ptr<CWidget> create(const std::string& id, const std::string& title, int x = 10, int y = 10);
void save();
void load();
void initialize();
void update();
void render();

} // namespace widgets

namespace widget_system {
void initialize();
} // namespace widget_system
