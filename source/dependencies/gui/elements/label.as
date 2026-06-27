class CLabel {
    string name;
    string config_key;
    bool visible = true;
    VisibilityFn@ visibility_fn;
    array<CElement@> attachments;

    CLabel() {}

    CLabel(const string &in _name) {
        name = _name;
    }

    void set_visible(VisibilityFn@ fn) {
        @visibility_fn = fn;
    }

    bool is_visible() {
        if (visibility_fn !is null)
            return visibility_fn();
        return visible;
    }

    CCogWheel@ create_cogwheel(const string &in cog_name) {
        CCogWheel@ cog = CCogWheel(cog_name);
        if (config_key.length() > 0)
            cog.config_key = config_key + "." + cog_name;
        attachments.insertLast(CElement(cog));
        return cog;
    }

    CTooltip@ create_tooltip(const string &in description) {
        CTooltip@ tip = CTooltip(description);
        attachments.insertLast(CElement(tip));
        return tip;
    }

    void tick() {
        for (uint i = 0; i < attachments.length(); i++)
            attachments[i].tick();
    }

    void handle_input(float x, float y, float w) {
        float right_edge = x + w;

        for (uint i = 0; i < attachments.length(); i++) {
            float att_w = get_attachment_width(attachments[i]);
            right_edge -= att_w;
            attachments[i].handle_input(right_edge, y, att_w);
            right_edge -= 4;
        }
    }

    void render(float x, float y, float w) {
        vector2 text_size = renderer::text::get_size(name, EFont::TAHOMA);
        float text_y = y + (12 - text_size.y) / 2;

        renderer::text::draw_shadowed(name, vector2(x, text_y), gui::colors::text_active.scaled(), gui::colors::outline.scaled(), EFont::TAHOMA);

        float right_edge = x + w;

        for (uint i = 0; i < attachments.length(); i++) {
            float att_w = get_attachment_width(attachments[i]);
            right_edge -= att_w;
            attachments[i].render(right_edge, y, att_w);
            right_edge -= 4;
        }
    }

    void render_popup_pass(float x, float y, float w) {
        float right_edge = x + w;

        for (uint i = 0; i < attachments.length(); i++) {
            float att_w = get_attachment_width(attachments[i]);
            right_edge -= att_w;
            attachments[i].render_popup(right_edge, y, att_w);
            right_edge -= 4;
        }
    }

    float get_attachment_width(CElement@ el) {
        if (el.type == EElementType::COGWHEEL) return 12;
        if (el.type == EElementType::TOOLTIP) return 12;
        return 12;
    }

    float get_height() {
        return 12;
    }
}
