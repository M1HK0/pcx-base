void on_render(int id, int data_index) {
    set_thread_to_highest_priority();
    int64 t = thread::begin_timing();

    int target_streamproof = gui::config::settings::streamproof_mode.get() + 1;
    if (globals::streamproof.get() != target_streamproof)
        //globals::streamproof.set(target_streamproof);

    cursor::update();
    gui::tick();
    gui::update();
    widgets::update();
    notifications::update();

    crosshair::render();

    widgets::render();
    gui::render();
    notifications::render();

    thread::end_timing("on_render", t);
}