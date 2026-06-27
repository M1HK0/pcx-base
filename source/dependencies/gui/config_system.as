namespace gui {
    namespace config_system {
        string base_path = globals::config_path;
        array<string> registry_keys;
        array<CElement@> registry;

        void register_element(const string &in key, CElement@ el) {
            registry_keys.insertLast(key);
            registry.insertLast(el);
        }

        void initialize() {
            create_directory("configs");
            create_directory("configs/base");
        }

        void refresh_listbox(CListBox@ list) {
            list.clear();
            array<string> exts = {".cfg"};
            array<string> files;
            if (query_directory(base_path, false, true, exts, files)) {
                for (uint i = 0; i < files.length(); i++) {
                    string name = files[i].substr(0, files[i].length() - 3);
                    list.add_item(name);
                }
            }
        }

        bool save(const string &in name) {
            dictionary root;

            for (uint i = 0; i < registry.length(); i++) {
                CElement@ el = registry[i];
                string key = registry_keys[i];
                dictionary@ entry = dictionary();

                switch (el.type) {
                    case EElementType::CHECKBOX: {
                        entry.set("type", "checkbox");
                        entry.set("value", el.checkbox.value ? 1.0 : 0.0);
                        break;
                    }
                    case EElementType::SLIDER_INT: {
                        entry.set("type", "slider_int");
                        entry.set("value", double(el.slider_int.value));
                        break;
                    }
                    case EElementType::SLIDER_FLOAT: {
                        entry.set("type", "slider_float");
                        entry.set("value", double(el.slider_float.value));
                        break;
                    }
                    case EElementType::KEYBIND: {
                        entry.set("type", "keybind");
                        entry.set("key", double(el.keybind.key));
                        entry.set("mode", double(int(el.keybind.mode)));
                        entry.set("toggle", el.keybind.toggle_state ? 1.0 : 0.0);
                        break;
                    }
                    case EElementType::DROPDOWN: {
                        entry.set("type", "dropdown");
                        entry.set("value", double(el.dropdown.selected));
                        break;
                    }
                    case EElementType::MULTISELECT: {
                        entry.set("type", "multiselect");
                        dictionary@ sel = dictionary();
                        for (uint j = 0; j < el.multiselect.selected.length(); j++)
                            sel.set("" + j, el.multiselect.selected[j] ? 1.0 : 0.0);
                        entry.set("selected", @sel);
                        break;
                    }
                    case EElementType::COLORPICKER: {
                        entry.set("type", "colorpicker");
                        entry.set("r", double(el.colorpicker.value.r));
                        entry.set("g", double(el.colorpicker.value.g));
                        entry.set("b", double(el.colorpicker.value.b));
                        entry.set("a", double(el.colorpicker.value.a));
                        break;
                    }
                    case EElementType::TEXT_INPUT: {
                        entry.set("type", "text_input");
                        entry.set("value", el.text_input.value);
                        break;
                    }
                }

                root.set(key, @entry);
            }

            string json, err;
            if (!json_stringify(root, json, err))
                return false;

            string encoded = util_base64_encode(json);
            return create_file(base_path + "\\" + name + ".cfg", encoded);
        }

        bool load(const string &in name) {
            string raw;
            if (!read_file(base_path + "\\" + name + ".cfg", raw))
                return false;

            string json, decode_err;
            if (!util_base64_decode(raw, json, decode_err))
                return false;

            dictionary root;
            string err;
            if (!json_parse(json, root, err))
                return false;

            for (uint i = 0; i < registry.length(); i++) {
                CElement@ el = registry[i];
                string key = registry_keys[i];

                if (!root.exists(key)) continue;

                dictionary@ entry;
                root.get(key, @entry);
                if (entry is null) continue;

                switch (el.type) {
                    case EElementType::CHECKBOX: {
                        double v = 0;
                        entry.get("value", v);
                        el.checkbox.value = v > 0.5;
                        break;
                    }
                    case EElementType::SLIDER_INT: {
                        double v = 0;
                        entry.get("value", v);
                        el.slider_int.value = int(v);
                        el.slider_int.visual_value = float(v);
                        break;
                    }
                    case EElementType::SLIDER_FLOAT: {
                        double v = 0;
                        entry.get("value", v);
                        el.slider_float.value = float(v);
                        el.slider_float.visual_value = float(v);
                        break;
                    }
                    case EElementType::KEYBIND: {
                        double k = 0, m = 0, t = 0;
                        entry.get("key", k);
                        entry.get("mode", m);
                        entry.get("toggle", t);
                        el.keybind.key = int(k);
                        el.keybind.mode = EKeybindMode(int(m));
                        el.keybind.toggle_state = t > 0.5;
                        break;
                    }
                    case EElementType::DROPDOWN: {
                        double v = 0;
                        entry.get("value", v);
                        el.dropdown.selected = int(v);
                        break;
                    }
                    case EElementType::MULTISELECT: {
                        dictionary@ sel;
                        entry.get("selected", @sel);
                        if (sel !is null) {
                            for (uint j = 0; j < el.multiselect.selected.length(); j++) {
                                double v = 0;
                                if (sel.exists("" + j)) {
                                    sel.get("" + j, v);
                                    el.multiselect.selected[j] = v > 0.5;
                                }
                            }
                        }
                        break;
                    }
                    case EElementType::COLORPICKER: {
                        double r = 255, g = 255, b = 255, a = 255;
                        entry.get("r", r);
                        entry.get("g", g);
                        entry.get("b", b);
                        entry.get("a", a);
                        el.colorpicker.value = color_t(uint8(r), uint8(g), uint8(b), uint8(a));
                        el.colorpicker.rgb_to_hsv(el.colorpicker.value.r, el.colorpicker.value.g, el.colorpicker.value.b, el.colorpicker.hue, el.colorpicker.sat, el.colorpicker.val);
                        break;
                    }
                    case EElementType::TEXT_INPUT: {
                        string v;
                        entry.get("value", v);
                        el.text_input.value = v;
                        break;
                    }
                }
            }

            return true;
        }

        bool remove(const string &in name) {
            return delete_file(base_path + "\\" + name + ".cfg");
        }
    }
}