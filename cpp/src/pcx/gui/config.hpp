#pragma once

#include "pcx/types.hpp"

namespace gui::colors {

inline pcx::Color background_primary{18, 18, 18, 255};
inline pcx::Color background_secondary{24, 24, 24, 255};
inline pcx::Color accent_primary{130, 100, 255, 255};
inline pcx::Color accent_secondary{60, 40, 200, 255};
inline pcx::Color text_active{255, 255, 255, 255};
inline pcx::Color text_inactive{140, 140, 140, 255};
inline pcx::Color text_hover{200, 200, 200, 255};
inline pcx::Color outline{0, 0, 0, 255};
inline pcx::Color border{60, 60, 60, 255};
inline pcx::Color glow{0, 0, 0, 80};
inline pcx::Color copied_color{255, 255, 255, 255};

inline pcx::Color scaled(const pcx::Color& c) {
    return c.scaled(pcx::globals::menu_alpha);
}

} // namespace gui::colors
