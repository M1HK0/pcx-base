#pragma once

#include "pcx/gui/elements_all.hpp"

#include <memory>
#include <string>
#include <vector>

namespace gui::config_system {

void register_element(const std::string& key, const std::shared_ptr<CElement>& el);
void initialize();
void refresh_listbox(const std::shared_ptr<CListBox>& list);
bool save(const std::string& name);
bool load(const std::string& name);
bool remove(const std::string& name);

inline std::string base_path = pcx::globals::config_path;

} // namespace gui::config_system
