class color_t {
    uint8 r = 255, g = 255, b = 255, a = 255;

    color_t() {}

    color_t(uint8 _r, uint8 _g, uint8 _b, uint8 _a = 255) {
        r = _r; g = _g; b = _b; a = _a;
    }

    color_t scaled() {
        return color_t(r, g, b, uint8((a * globals::menu_alpha) / 255));
    }
}