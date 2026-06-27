#pragma once

#include "pcx/types.hpp"

#include <string>

namespace notifications {

void create(const std::string& message, float lifetime = 5.f);
void create_success(const std::string& message, float lifetime = 5.f);
void create_error(const std::string& message, float lifetime = 5.f);
void create_persistent(const std::string& message, pcx::ENotificationType type = pcx::ENotificationType::NORMAL);
void update();
void render();

} // namespace notifications
