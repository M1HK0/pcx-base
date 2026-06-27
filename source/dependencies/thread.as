class CThread {
    string name;
    int id;
    int interval;
    double timing = 0.0;

    CThread(const string &in _name, int _id, int _interval) {
        name = _name;
        id = _id;
        interval = _interval;
    }
}

namespace thread {
    array<CThread@> registered;
    int64 frequency = perf_frequency();

    void register(const string &in name, const __Internal_CallbackFn@ callback, int interval) {
        int callback_id = register_callback(callback, interval, 0);
        registered.insertLast(CThread(name, callback_id, interval));
        log_console(string::format("[+] thread: %s | interval: %dms").s(name).d(interval).str());
    }

    void unregister_all() {
        for (uint i = 0; i < registered.length(); i++)
            unregister_callback(registered[i].id);
        registered.resize(0);
    }

    int64 begin_timing() {
        return perf_time();
    }

    void end_timing(const string &in name, int64 start) {
        int64 elapsed = perf_time() - start;
        double seconds = double(elapsed) / double(frequency);
        for (uint i = 0; i < registered.length(); i++) {
            if (registered[i].name == name) {
                registered[i].timing = seconds;
                break;
            }
        }
    }

    double get_timing(const string &in name) {
        for (uint i = 0; i < registered.length(); i++) {
            if (registered[i].name == name)
                return registered[i].timing;
        }
        return 0.0;
    }
}