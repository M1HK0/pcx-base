#pragma once

#include "pcx/gui/containers.hpp"

#include <memory>
#include <string>

namespace gui {

inline CWindow window;
inline float open_anim = 0.f;

namespace config_tab {
inline std::shared_ptr<CListBox> config_list;
inline std::shared_ptr<CTextInput> config_name;
} // namespace config_tab

namespace theme_tab {
inline std::shared_ptr<CListBox> theme_list;
inline std::shared_ptr<CTextInput> theme_name;
} // namespace theme_tab

namespace config {
namespace settings {
inline std::shared_ptr<CKeybind> menu_key;
inline std::shared_ptr<CDropdown> streamproof_mode;
namespace widgets {
inline std::shared_ptr<CCheckbox> watermark;
inline std::shared_ptr<CCheckbox> hotkeys;
} // namespace widgets
} // namespace settings

namespace crosshair {
inline std::shared_ptr<CCheckbox> enabled;
inline std::shared_ptr<CColorPicker> color;
inline std::shared_ptr<CColorPicker> outline;
inline std::shared_ptr<CDropdown> style;
inline std::shared_ptr<CSliderInt> size;
inline std::shared_ptr<CSliderInt> gap;
inline std::shared_ptr<CSliderInt> thickness;
inline std::shared_ptr<CCheckbox> t_style;
inline std::shared_ptr<CCheckbox> center_dot;
inline std::shared_ptr<CSliderInt> dot_size;
} // namespace crosshair

namespace theme {
inline std::shared_ptr<CColorPicker> accent_primary;
inline std::shared_ptr<CColorPicker> accent_secondary;
inline std::shared_ptr<CColorPicker> background_primary;
inline std::shared_ptr<CColorPicker> background_secondary;
inline std::shared_ptr<CColorPicker> text_active;
inline std::shared_ptr<CColorPicker> text_inactive;
inline std::shared_ptr<CColorPicker> border;
inline std::shared_ptr<CColorPicker> glow;
} // namespace theme
} // namespace config

void initialize(const std::string& title, pcx::Vec2 size);
void sync_colors();
void tick();
void update();
void render();

} // namespace gui
