#pragma once

#include <memory>
#include <vector>

class CKeybind;

namespace hotkeys {

void register_keybind(const std::shared_ptr<CKeybind>& kb);
const std::vector<std::shared_ptr<CKeybind>>& registered();

} // namespace hotkeys
