namespace gui {
    namespace theme_system {
        string base_path = globals::themes_path;
        array<string> registry_keys;
        array<CElement@> registry;

        void register_element(const string &in key, CElement@ el) {
            registry_keys.insertLast(key);
            registry.insertLast(el);
        }

        void initialize() {
            create_directory("themes");
            create_directory("themes/base");
        }

        void refresh_listbox(CListBox@ list) {
            list.clear();
            array<string> exts = {".theme"};
            array<string> files;
            if (query_directory(base_path, false, true, exts, files)) {
                for (uint i = 0; i < files.length(); i++) {
                    string name = files[i].substr(0, files[i].length() - 6);
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
                    case EElementType::COLORPICKER: {
                        entry.set("type", "colorpicker");
                        entry.set("r", double(el.colorpicker.value.r));
                        entry.set("g", double(el.colorpicker.value.g));
                        entry.set("b", double(el.colorpicker.value.b));
                        entry.set("a", double(el.colorpicker.value.a));
                        break;
                    }
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
                    case EElementType::DROPDOWN: {
                        entry.set("type", "dropdown");
                        entry.set("value", double(el.dropdown.selected));
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
            return create_file(base_path + "\\" + name + ".theme", encoded);
        }

        bool load(const string &in name) {
            string raw;
            if (!read_file(base_path + "\\" + name + ".theme", raw))
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
                    case EElementType::DROPDOWN: {
                        double v = 0;
                        entry.get("value", v);
                        el.dropdown.selected = int(v);
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
            return delete_file(base_path + "\\" + name + ".theme");
        }
    }
}