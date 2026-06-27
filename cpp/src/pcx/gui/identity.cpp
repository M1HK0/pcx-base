#include "pcx/gui/identity.hpp"

#include "pcx/globals.hpp"
#include "pcx/gui/config.hpp"

#include <unordered_map>

namespace gui::identity {
namespace {

pcx::EUserRole role = pcx::EUserRole::USER;

} // namespace

void initialize() {
    static const std::unordered_map<std::string, pcx::EUserRole> roles = {
        {"underscore", pcx::EUserRole::DEVELOPER},
        {"draxle", pcx::EUserRole::BETA},
        {"touka123", pcx::EUserRole::BETA},
        {"Josh", pcx::EUserRole::BETA},
        {"promethyzx", pcx::EUserRole::BETA},
        {"aat", pcx::EUserRole::BETA},
        {"bay", pcx::EUserRole::BETA},
        {"GSP", pcx::EUserRole::BETA},
        {"Wtf", pcx::EUserRole::BETA},
    };
    const auto it = roles.find(pcx::globals::username);
    if (it != roles.end()) role = it->second;
}

std::string get_role_name() {
    switch (role) {
    case pcx::EUserRole::DEVELOPER: return "developer";
    case pcx::EUserRole::BETA: return "beta";
    default: return "";
    }
}

pcx::Color get_role_color() {
    switch (role) {
    case pcx::EUserRole::DEVELOPER: return {255, 80, 80, 255};
    case pcx::EUserRole::BETA: return {80, 180, 255, 255};
    default: return gui::colors::text_active;
    }
}

} // namespace gui::identity
