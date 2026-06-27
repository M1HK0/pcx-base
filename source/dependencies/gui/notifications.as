enum ENotificationType {
    NORMAL,
    SUCCESS,
    ERROR
}

class CNotification {
    string message;
    bool dismissable;
    float lifetime;
    float elapsed = 0.0f;
    float slide_anim = 0.0f;
    float fade_anim = 0.0f;
    bool closing = false;
    int64 start_time;
    ENotificationType type;

    CNotification() {}

    CNotification(const string &in _message, float _lifetime = 5.0f, bool _dismissable = false, ENotificationType _type = ENotificationType::NORMAL) {
        message = _message;
        lifetime = _lifetime;
        dismissable = _dismissable;
        type = _type;
        start_time = perf_time();
    }
}

namespace notifications {
    array<CNotification@> active;
    float padding = 10;
    float width = 220;
    float min_height = 30;

    void create(const string &in message, float lifetime = 5.0f) {
        active.insertLast(CNotification(message, lifetime, false, ENotificationType::NORMAL));
    }

    void create_success(const string &in message, float lifetime = 5.0f) {
        active.insertLast(CNotification(message, lifetime, false, ENotificationType::SUCCESS));
    }

    void create_error(const string &in message, float lifetime = 5.0f) {
        active.insertLast(CNotification(message, lifetime, false, ENotificationType::ERROR));
    }

    void create_persistent(const string &in message, ENotificationType type = ENotificationType::NORMAL) {
        active.insertLast(CNotification(message, 0, true, type));
    }

    color_t get_accent(CNotification@ n, uint8 a) {
        switch (n.type) {
            case ENotificationType::SUCCESS: return color_t(50, 200, 80, a);
            case ENotificationType::ERROR: return color_t(220, 50, 50, a);
        }
        return color_t(gui::colors::accent_primary.r, gui::colors::accent_primary.g, gui::colors::accent_primary.b, a);
    }

    float get_notification_height(CNotification@ n) {
        vector2 text_size = renderer::text::get_size(n.message, EFont::TAHOMA);
        float content_w = width - 16;
        if (n.dismissable) content_w -= 16;
        int lines = int(text_size.x / content_w) + 1;
        float text_h = text_size.y * lines;
        float h = text_h + 12;
        if (h < min_height) h = min_height;
        return h;
    }

    void update() {
        for (int i = int(active.length()) - 1; i >= 0; i--) {
            CNotification@ n = active[i];

            if (!n.closing)
                n.slide_anim = lerp(n.slide_anim, 1.0f, 0.06f);
            else
                n.slide_anim = lerp(n.slide_anim, 0.0f, 0.06f);

            if (n.slide_anim > 0.99f) n.slide_anim = 1.0f;
            if (n.closing && n.slide_anim < 0.01f) n.slide_anim = 0.0f;

            if (!n.closing) {
                n.fade_anim = lerp(n.fade_anim, 1.0f, 0.08f);
                if (n.fade_anim > 0.99f) n.fade_anim = 1.0f;
            } else {
                n.fade_anim = lerp(n.fade_anim, 0.0f, 0.08f);
                if (n.fade_anim < 0.01f) n.fade_anim = 0.0f;
            }

            if (!n.dismissable && n.lifetime > 0) {
                int64 now = perf_time();
                n.elapsed = float(now - n.start_time) / float(perf_frequency());
                if (n.elapsed >= n.lifetime)
                    n.closing = true;
            }

            if (n.dismissable && !n.closing) {
                vector2 vp = window::get_viewport();
                float h = get_notification_height(n);
                float x = vp.x - (width + padding) * n.slide_anim;

                float current_y = padding;
                for (int j = 0; j < i; j++) {
                    if (active[j].slide_anim > 0.01f)
                        current_y += get_notification_height(active[j]) * active[j].slide_anim + 6;
                }

                float close_x = x + width - 18;
                float close_y = current_y + 4;

                if (cursor::m1_pressed && cursor::is_hovered(vector2(close_x, close_y), vector2(14, 14)))
                    n.closing = true;
            }

            if (n.closing && n.slide_anim <= 0.0f && n.fade_anim <= 0.0f)
                active.removeAt(i);
        }
    }

    void render() {
        if (active.length() == 0) return;

        vector2 vp = window::get_viewport();
        float current_y = padding;

        for (uint i = 0; i < active.length(); i++) {
            CNotification@ n = active[i];
            if (n.slide_anim < 0.01f && n.fade_anim < 0.01f) continue;

            float h = get_notification_height(n);
            float animated_h = h * n.slide_anim;
            float x = vp.x - (width + padding) * n.slide_anim;
            uint8 a = uint8(255 * n.fade_anim);

            color_t accent = get_accent(n, a);

            // glow
            uint8 glow_a = uint8(gui::colors::glow.a * n.fade_anim);
            renderer::rect::draw_glow(
                vector2(x, current_y),
                vector2(width, animated_h),
                color_t(gui::colors::glow.r, gui::colors::glow.g, gui::colors::glow.b, glow_a),
                4.0f, 3
            );

            // background
            renderer::rect::draw_filled_rounded(
                vector2(x, current_y),
                vector2(width, animated_h),
                color_t(gui::colors::background_secondary.r, gui::colors::background_secondary.g, gui::colors::background_secondary.b, a),
                4.0f
            );

            renderer::rect::draw(
                vector2(x + 1, current_y + 1),
                vector2(width - 2, animated_h - 2),
                color_t(gui::colors::border.r, gui::colors::border.g, gui::colors::border.b, a),
                1.0f, 4.0f, RR_TOP_LEFT | RR_TOP_RIGHT | RR_BOTTOM_LEFT | RR_BOTTOM_RIGHT
            );

            // accent line
            renderer::line::draw(
                vector2(x + 4, current_y + 4),
                vector2(x + 4, current_y + animated_h - 4),
                accent
            );

            if (n.fade_anim > 0.3f) {
                float text_x = x + 10;
                float text_y = current_y + (animated_h - renderer::text::get_size(n.message, EFont::TAHOMA).y) / 2;

                float content_w = width - 18;
                if (n.dismissable) content_w -= 16;

                clip_push(int(x + 6), int(current_y + 2), int(content_w), int(animated_h - 4));
                renderer::text::draw_shadowed(
                    n.message,
                    vector2(text_x, text_y),
                    color_t(gui::colors::text_active.r, gui::colors::text_active.g, gui::colors::text_active.b, a),
                    color_t(0, 0, 0, a),
                    EFont::TAHOMA
                );
                clip_pop();

                if (n.dismissable) {
                    float close_x = x + width - 18;
                    float close_y = current_y + (animated_h - 10) / 2;
                    bool hover_close = cursor::is_hovered(vector2(close_x, close_y - 2), vector2(14, 14));

                    color_t close_col = hover_close ? accent : color_t(gui::colors::text_inactive.r, gui::colors::text_inactive.g, gui::colors::text_inactive.b, a);

                    renderer::line::draw(vector2(close_x, close_y), vector2(close_x + 10, close_y + 10), close_col);
                    renderer::line::draw(vector2(close_x + 10, close_y), vector2(close_x, close_y + 10), close_col);
                }
            }

            current_y += animated_h + 8;
        }
    }
}