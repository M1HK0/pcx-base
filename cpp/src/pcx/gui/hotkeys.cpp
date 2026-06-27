#include "pcx/gui/hotkeys.hpp"

#include "pcx/gui/elements_all.hpp"

#include <vector>

namespace hotkeys {
namespace {

std::vector<std::shared_ptr<CKeybind>> registered_;

} // namespace

void register_keybind(const std::shared_ptr<CKeybind>& kb) {
    registered_.push_back(kb);
}

const std::vector<std::shared_ptr<CKeybind>>& registered() {
    return registered_;
}

} // namespace hotkeys
