funcdef bool VisibilityFn();

enum EElementType {
    CHECKBOX,
    SLIDER_INT,
    SLIDER_FLOAT,
    KEYBIND,
    DROPDOWN,
    MULTISELECT,
    TEXT_INPUT,
    BUTTON,
    COLORPICKER,
    COGWHEEL,
    LISTBOX,
    TOOLTIP,
    LABEL,
    ELEMENT_COUNT
}

class CElement {
    EElementType type;
    CCheckbox@ checkbox;
    CSliderInt@ slider_int;
    CSliderFloat@ slider_float;
    CKeybind@ keybind;
    CDropdown@ dropdown;
    CMultiSelect@ multiselect;
    CTextInput@ text_input;
    CButton@ button;
    CColorPicker@ colorpicker;
    CCogWheel@ cogwheel;
    CListBox@ listbox;
    CTooltip@ tooltip;
    CLabel@ label;

    CElement(CCheckbox@ _checkbox) { type = EElementType::CHECKBOX; @checkbox = _checkbox; }
    CElement(CSliderInt@ _slider_int) { type = EElementType::SLIDER_INT; @slider_int = _slider_int; }
    CElement(CSliderFloat@ _slider_float) { type = EElementType::SLIDER_FLOAT; @slider_float = _slider_float; }
    CElement(CKeybind@ _keybind) { type = EElementType::KEYBIND; @keybind = _keybind; }
    CElement(CDropdown@ _dropdown) { type = EElementType::DROPDOWN; @dropdown = _dropdown; }
    CElement(CMultiSelect@ _multiselect) { type = EElementType::MULTISELECT; @multiselect = _multiselect; }
    CElement(CTextInput@ _text_input) { type = EElementType::TEXT_INPUT; @text_input = _text_input; }
    CElement(CButton@ _button) { type = EElementType::BUTTON; @button = _button; }
    CElement(CColorPicker@ _colorpicker) { type = EElementType::COLORPICKER; @colorpicker = _colorpicker; }
    CElement(CCogWheel@ _cogwheel) { type = EElementType::COGWHEEL; @cogwheel = _cogwheel; }
    CElement(CListBox@ _listbox) { type = EElementType::LISTBOX; @listbox = _listbox; }
    CElement(CTooltip@ _tooltip) { type = EElementType::TOOLTIP; @tooltip = _tooltip; }
    CElement(CLabel@ _label) { type = EElementType::LABEL; @label = _label; }

    bool is_visible() {
        switch (type) {
            case EElementType::CHECKBOX: return checkbox.is_visible();
            case EElementType::SLIDER_INT: return slider_int.is_visible();
            case EElementType::SLIDER_FLOAT: return slider_float.is_visible();
            case EElementType::KEYBIND: return keybind.is_visible();
            case EElementType::DROPDOWN: return dropdown.is_visible();
            case EElementType::MULTISELECT: return multiselect.is_visible();
            case EElementType::TEXT_INPUT: return text_input.is_visible();
            case EElementType::BUTTON: return button.is_visible();
            case EElementType::COLORPICKER: return colorpicker.is_visible();
            case EElementType::COGWHEEL: return cogwheel.is_visible();
            case EElementType::LISTBOX: return listbox.is_visible();
            case EElementType::TOOLTIP: return tooltip.is_visible();
            case EElementType::LABEL: return label.is_visible();
        }
        return true;
    }

    bool is_blocking() {
        switch (type) {
            case EElementType::DROPDOWN: return dropdown.is_open;
            case EElementType::KEYBIND: return keybind.is_open;
            case EElementType::MULTISELECT: return multiselect.is_open;
            case EElementType::TEXT_INPUT: return text_input.focused;
            case EElementType::COLORPICKER: return colorpicker.is_open;
            case EElementType::COGWHEEL: return cogwheel.is_open;
        }
        return false;
    }

    void tick() {
        switch (type) {
            case EElementType::CHECKBOX: checkbox.tick(); break;
            case EElementType::SLIDER_INT: slider_int.tick(); break;
            case EElementType::SLIDER_FLOAT: slider_float.tick(); break;
            case EElementType::KEYBIND: keybind.tick(); break;
            case EElementType::COGWHEEL: cogwheel.tick(); break;
            case EElementType::LABEL: label.tick(); break;
        }
    }

    void handle_input(float x, float y, float w) {
        switch (type) {
            case EElementType::CHECKBOX: checkbox.handle_input(x, y, w); break;
            case EElementType::SLIDER_INT: slider_int.handle_input(x, y, w); break;
            case EElementType::SLIDER_FLOAT: slider_float.handle_input(x, y, w); break;
            case EElementType::KEYBIND: keybind.handle_input(x, y, w); break;
            case EElementType::DROPDOWN: dropdown.handle_input(x, y, w); break;
            case EElementType::MULTISELECT: multiselect.handle_input(x, y, w); break;
            case EElementType::TEXT_INPUT: text_input.handle_input(x, y, w); break;
            case EElementType::BUTTON: button.handle_input(x, y, w); break;
            case EElementType::COLORPICKER: colorpicker.handle_input(x, y, w); break;
            case EElementType::COGWHEEL: cogwheel.handle_input(x, y, w); break;
            case EElementType::LISTBOX: listbox.handle_input(x, y, w); break;
            case EElementType::TOOLTIP: tooltip.handle_input(x, y, w); break;
            case EElementType::LABEL: label.handle_input(x, y, w); break;
        }
    }

    void render(float x, float y, float w) {
        switch (type) {
            case EElementType::CHECKBOX: checkbox.render(x, y, w); break;
            case EElementType::SLIDER_INT: slider_int.render(x, y, w); break;
            case EElementType::SLIDER_FLOAT: slider_float.render(x, y, w); break;
            case EElementType::KEYBIND: keybind.render(x, y, w); break;
            case EElementType::DROPDOWN: dropdown.render(x, y, w); break;
            case EElementType::MULTISELECT: multiselect.render(x, y, w); break;
            case EElementType::TEXT_INPUT: text_input.render(x, y, w); break;
            case EElementType::BUTTON: button.render(x, y, w); break;
            case EElementType::COLORPICKER: colorpicker.render(x, y, w); break;
            case EElementType::COGWHEEL: cogwheel.render(x, y, w); break;
            case EElementType::LISTBOX: listbox.render(x, y, w); break;
            case EElementType::TOOLTIP: tooltip.render(x, y, w); break;
            case EElementType::LABEL: label.render(x, y, w); break;
        }
    }

    void render_popup(float x, float y, float w, float max_y = 0) {
        switch (type) {
            case EElementType::CHECKBOX: checkbox.render_popup_pass(x, y, w); break;
            case EElementType::SLIDER_INT: slider_int.render_popup_pass(x, y, w); break;
            case EElementType::SLIDER_FLOAT: slider_float.render_popup_pass(x, y, w); break;
            case EElementType::KEYBIND: keybind.render_popup_pass(x, y, w); break;
            case EElementType::DROPDOWN: dropdown.render_popup_pass(x, y, w, max_y); break;
            case EElementType::MULTISELECT: multiselect.render_popup_pass(x, y, w, max_y); break;
            case EElementType::COLORPICKER: colorpicker.render_popup_pass(x, y, w); break;
            case EElementType::COGWHEEL: cogwheel.render_popup_pass(x, y, w); break;
            case EElementType::TOOLTIP: tooltip.render_popup_pass(x, y, w); break;
            case EElementType::LABEL: label.render_popup_pass(x, y, w); break;
        }
    }

    float get_height() {
        switch (type) {
            case EElementType::CHECKBOX: return checkbox.get_height();
            case EElementType::SLIDER_INT: return slider_int.get_height();
            case EElementType::SLIDER_FLOAT: return slider_float.get_height();
            case EElementType::KEYBIND: return keybind.get_height();
            case EElementType::DROPDOWN: return dropdown.get_height();
            case EElementType::MULTISELECT: return multiselect.get_height();
            case EElementType::TEXT_INPUT: return text_input.get_height();
            case EElementType::BUTTON: return button.get_height();
            case EElementType::COLORPICKER: return colorpicker.get_height();
            case EElementType::COGWHEEL: return cogwheel.get_height();
            case EElementType::LISTBOX: return listbox.get_height();
            case EElementType::TOOLTIP: return tooltip.get_height();
            case EElementType::LABEL: return label.get_height();
        }
        return 0;
    }
}