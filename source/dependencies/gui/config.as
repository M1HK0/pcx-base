namespace gui {
    namespace colors {
        color_t background_primary = color_t(18, 18, 18, 255);
        color_t background_secondary = color_t(24, 24, 24, 255);
        color_t accent_primary = color_t(130, 100, 255, 255);
        color_t accent_secondary = color_t(60, 40, 200, 255);
        color_t text_active = color_t(255, 255, 255, 255);
        color_t text_inactive = color_t(140, 140, 140, 255);
        color_t text_hover = color_t(200, 200, 200, 255);
        color_t outline = color_t(0, 0, 0, 255);
        color_t border = color_t(60, 60, 60, 255);
        color_t glow = color_t(0, 0, 0, 80);
        color_t copied_color = color_t(255, 255, 255, 255);
    }

    namespace config {
        namespace misc {
            CSliderInt@ some_value;
            CCheckbox@ some_bool;
        }

        namespace settings {
            CKeybind@ menu_key;
            CDropdown@ streamproof_mode;

            namespace widgets {
                CCheckbox@ watermark;
                CCheckbox@ hotkeys;
                CCheckbox@ ultimates;
            }
        }

        namespace theme {
            CColorPicker@ accent_primary;
            CColorPicker@ accent_secondary;
            CColorPicker@ background_primary;
            CColorPicker@ background_secondary;
            CColorPicker@ text_active;
            CColorPicker@ text_inactive;
            CColorPicker@ border;
            CColorPicker@ glow;
        }

        namespace crosshair {
            CCheckbox@ enabled;
            CColorPicker@ color;
            CColorPicker@ outline;
            CDropdown@ style;
            CSliderInt@ size;
            CSliderInt@ gap;
            CSliderInt@ thickness;
            CCheckbox@ t_style;
            CCheckbox@ center_dot;
            CSliderInt@ dot_size;
        }

        namespace legit {
            CListBox@ selection;
            CListBox@ hero_selection;

            namespace aimbot {
                array<CCheckbox@> enabled;
                array<CKeybind@> keybind;
                array<CKeybind@> secondary_keybind;
                array<CCheckbox@> visible_only;
                array<CCheckbox@> team_check;
                array<CSliderInt@> max_distance;
                array<CMultiSelect@> hitboxes;
                array<CSliderInt@> fov;
                array<CCheckbox@> dynamic_fov;
                array<CSliderInt@> dynamic_min_fov;
                array<CSliderInt@> dynamic_min_distance;
                array<CSliderInt@> dynamic_max_distance;
                array<CCheckbox@> sticky_target;
                array<CSliderInt@> target_switch_delay;
                array<CSliderFloat@> speed;
                array<CSliderFloat@> inertia;
                array<CSliderFloat@> curve_intensity;
                array<CCheckbox@> visualize_fov;
                array<CColorPicker@> fov_color;
                array<CColorPicker@> fov_outline_color;
                array<CCheckbox@> draw_target;
                array<CColorPicker@> target_color;

                CCheckbox@ invert_controls;
            }

            namespace triggerbot {
                array<CCheckbox@> enabled;
                array<CKeybind@> keybind;
                array<CCheckbox@> visible_only;
                array<CCheckbox@> team_check;
                array<CSliderInt@> max_distance;
                array<CMultiSelect@> hitboxes;
                array<CDropdown@> triggerbot_type;
                array<CSliderInt@> radius_scale;
                array<CSliderInt@> delay;
                array<CSliderInt@> delay_variance;
                array<CSliderInt@> between_shot_delay;
                array<CSliderInt@> between_shot_variance;
            }
        }

        namespace visuals {
            CListBox@ selection;

            namespace players {
                CCheckbox@ enabled;
                CCheckbox@ visible_only;
                CSliderInt@ render_distance;

                CCheckbox@ box;
                CColorPicker@ box_visible;
                CColorPicker@ box_occluded;
                CDropdown@ box_style;

                CCheckbox@ name;
                CColorPicker@ name_visible;
                CColorPicker@ name_occluded;

                CCheckbox@ hero_icon;

                CCheckbox@ skeleton;
                CColorPicker@ skeleton_visible;
                CColorPicker@ skeleton_occluded;

                CCheckbox@ headcross;
                CColorPicker@ headcross_visible;
                CColorPicker@ headcross_occluded;

                CCheckbox@ healthbar;
                CDropdown@ healthbar_style;
                CColorPicker@ healthbar_visible;
                CColorPicker@ healthbar_occluded;
                CCheckbox@ healthbar_text;
                CColorPicker@ healthbar_text_visible;
                CColorPicker@ healthbar_text_occluded;

                CCheckbox@ ultimate_charge;
                CColorPicker@ ultimate_charge_visible;
                CColorPicker@ ultimate_charge_occluded;

                CCheckbox@ distance;
                CColorPicker@ distance_visible;
                CColorPicker@ distance_occluded;
            }
        }
    }
}