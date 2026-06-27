namespace window {
    uint64 find_by_pid(uint pid) {
        array<WindowInfo>@ windows = get_all_hwnds();
        if (windows is null) return 0;

        for (uint i = 0; i < windows.length(); i++) {
            if (windows[i].pid == pid)
                return windows[i].hwnd;
        }

        return 0;
    }

    bool get_rect(uint64 hwnd, int &out x, int &out y, int &out w, int &out h) {
        return get_window_rect(hwnd, x, y, w, h);
    }

    vector2 get_viewport() {
        float w, h;
        get_view(w, h);
        return vector2(w, h);
    }
}
