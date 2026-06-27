int main() {
    globals::username = get_username();
    gui::initialize("Perception.cx", vector2(500, 480));

    globals::streamproof = find_single_select(2, "Settings", "General", "Stream Proof Mode");
    //globals::streamproof.set(gui::config::settings::streamproof_mode.get() + 1); // default to perceptproof

    if (!globals::process.attach(globals::target_process)) {
        log_console(string::format("[!] failed to attach to '%s'").s(globals::target_process).str());
        return 0;
    }

    log_console(string::format("[+] %s | pid: %d | base: %x | size: %x")
        .s(globals::process.proc_name)
        .d(globals::process.pid)
        .x(globals::process.module_base)
        .x(globals::process.module_size)
        .str());

    globals::hwnd = window::find_by_pid(globals::process.pid);
    if (globals::hwnd == 0) {
        log_console(string::format("[!] failed to find window for '%s'").s(globals::target_process).str());
        globals::process.detach();
        return 0;
    }

    int x, y, w, h;
    if (!window::get_rect(globals::hwnd, x, y, w, h)) {
        log_console(string::format("[!] failed to get window rect for '%s'").s(globals::target_process).str());
        globals::process.detach();
        return 0;
    }

    log_console(string::format("[+] hwnd: %x | x: %d y: %d w: %d h: %d")
        .x(globals::hwnd)
        .d(x).d(y).d(w).d(h)
        .str());

    if (renderer::fonts::initialize())
        log_console(string::format("[+] fonts initialized | %d total fonts").d(globals::fonts.length()).str());

    widget_system::initialize();

    thread::register("on_render", on_render, 0);

    return 1;
}

void on_unload() {
    globals::process.detach();
}