namespace cursor {
    float x, y;
    bool m1_down = false;
    bool m1_pressed = false;
    bool was_m1_down = false;

    void update() {
        get_mouse_pos(x, y);
        m1_down = key_down(0x01);
        m1_pressed = m1_down && !was_m1_down;
        was_m1_down = m1_down;
    }

    vector2 pos() {
        return vector2(x, y);
    }

    bool is_hovered(vector2 position, vector2 size) {
        return x >= position.x && x <= position.x + size.x &&
               y >= position.y && y <= position.y + size.y;
    }
}