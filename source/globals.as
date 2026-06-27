namespace globals {
    string target_process = "notepad.exe";
    CAttachment process;
    uint64 hwnd;
    string username;

    single_select_t streamproof;

    string config_path = "configs\\base";
    string themes_path = "themes\\base";

    uint8 menu_alpha = 255;
    float menu_scale = 1.0f;

    array<uint64> fonts(EFont::COUNT);
}