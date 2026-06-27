#include "pcx/gui/theme_system.hpp"

#include "pcx/base64.hpp"
#include "pcx/file_util.hpp"

#include <nlohmann/json.hpp>

#include <vector>

namespace gui::theme_system {
namespace {

std::vector<std::string> registry_keys;
std::vector<std::shared_ptr<CElement>> registry;

nlohmann::json element_to_json(const std::shared_ptr<CElement>& el) {
    nlohmann::json entry;
    switch (el->type) {
    case pcx::EElementType::COLORPICKER:
        entry["type"] = "colorpicker";
        entry["r"] = el->colorpicker->value.r;
        entry["g"] = el->colorpicker->value.g;
        entry["b"] = el->colorpicker->value.b;
        entry["a"] = el->colorpicker->value.a;
        break;
    case pcx::EElementType::CHECKBOX:
        entry["type"] = "checkbox";
        entry["value"] = el->checkbox->value ? 1.0 : 0.0;
        break;
    case pcx::EElementType::SLIDER_INT:
        entry["type"] = "slider_int";
        entry["value"] = el->slider_int->value;
        break;
    case pcx::EElementType::SLIDER_FLOAT:
        entry["type"] = "slider_float";
        entry["value"] = el->slider_float->value;
        break;
    case pcx::EElementType::DROPDOWN:
        entry["type"] = "dropdown";
        entry["value"] = el->dropdown->selected;
        break;
    case pcx::EElementType::TEXT_INPUT:
        entry["type"] = "text_input";
        entry["value"] = el->text_input->value;
        break;
    default: break;
    }
    return entry;
}

void apply_json(const std::shared_ptr<CElement>& el, const nlohmann::json& entry) {
    switch (el->type) {
    case pcx::EElementType::COLORPICKER:
        el->colorpicker->value = pcx::Color{
            static_cast<uint8_t>(entry.value("r", 255)),
            static_cast<uint8_t>(entry.value("g", 255)),
            static_cast<uint8_t>(entry.value("b", 255)),
            static_cast<uint8_t>(entry.value("a", 255))};
        CColorPicker::rgb_to_hsv(el->colorpicker->value.r, el->colorpicker->value.g, el->colorpicker->value.b,
            el->colorpicker->hue, el->colorpicker->sat, el->colorpicker->val);
        break;
    case pcx::EElementType::CHECKBOX:
        el->checkbox->value = entry.value("value", 0.0) > 0.5;
        break;
    case pcx::EElementType::SLIDER_INT:
        el->slider_int->value = entry.value("value", 0);
        el->slider_int->visual_value = static_cast<float>(el->slider_int->value);
        break;
    case pcx::EElementType::SLIDER_FLOAT:
        el->slider_float->value = entry.value("value", 0.f);
        el->slider_float->visual_value = el->slider_float->value;
        break;
    case pcx::EElementType::DROPDOWN:
        el->dropdown->selected = entry.value("value", 0);
        break;
    case pcx::EElementType::TEXT_INPUT:
        el->text_input->value = entry.value("value", "");
        break;
    default: break;
    }
}

} // namespace

void register_element(const std::string& key, const std::shared_ptr<CElement>& el) {
    registry_keys.push_back(key);
    registry.push_back(el);
}

void initialize() {
    pcx::create_directory("themes");
    pcx::create_directory(base_path);
}

void refresh_listbox(const std::shared_ptr<CListBox>& list) {
    list->clear();
    std::vector<std::string> files;
    if (pcx::query_directory(base_path, {".theme"}, files)) {
        for (const auto& f : files) {
            if (f.size() > 6)
                list->add_item(f.substr(0, f.size() - 6));
        }
    }
}

bool save(const std::string& name) {
    nlohmann::json root;
    for (size_t i = 0; i < registry.size(); ++i)
        root[registry_keys[i]] = element_to_json(registry[i]);
    return pcx::create_file(base_path + "\\" + name + ".theme", pcx::base64_encode(root.dump()));
}

bool load(const std::string& name) {
    std::string raw;
    if (!pcx::read_file(base_path + "\\" + name + ".theme", raw)) return false;
    std::string json;
    if (!pcx::base64_decode(raw, json)) return false;
    nlohmann::json root;
    try { root = nlohmann::json::parse(json); }
    catch (...) { return false; }
    for (size_t i = 0; i < registry.size(); ++i) {
        if (!root.contains(registry_keys[i])) continue;
        apply_json(registry[i], root[registry_keys[i]]);
    }
    return true;
}

bool remove(const std::string& name) {
    return pcx::delete_file(base_path + "\\" + name + ".theme");
}

} // namespace gui::theme_system
