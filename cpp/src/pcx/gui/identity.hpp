#pragma once

#include "pcx/types.hpp"

#include <string>

namespace gui::identity {

void initialize();
std::string get_role_name();
pcx::Color get_role_color();

} // namespace gui::identity
