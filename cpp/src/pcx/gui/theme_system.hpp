#pragma once

#include "pcx/gui/elements_all.hpp"

#include <memory>
#include <string>

namespace gui::theme_system {

void register_element(const std::string& key, const std::shared_ptr<CElement>& el);
void initialize();
void refresh_listbox(const std::shared_ptr<CListBox>& list);
bool save(const std::string& name);
bool load(const std::string& name);
bool remove(const std::string& name);

inline std::string base_path = pcx::globals::themes_path;

} // namespace gui::theme_system
