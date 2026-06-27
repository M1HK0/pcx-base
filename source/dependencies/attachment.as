class CAttachment {
    uint pid;
    uint64 peb;
    uint64 module_base;
    uint64 module_size;
    proc_t proc;
    string proc_name;
    string module_name;

    bool attach(const string &in target_proc, const string &in target_module = "") {
        proc = process::reference(target_proc);
        if (!process::is_alive(proc)) {
            process::dereference(proc);
            return false;
        }

        pid = process::get_pid(proc);
        peb = process::get_peb(proc);
        proc_name = target_proc;
        module_name = target_module.length() > 0 ? target_module : target_proc;

        if (!process::get_module(proc, module_name, module_base, module_size)) {
            process::dereference(proc);
            return false;
        }

        return true;
    }

    void detach() {
        if (process::is_alive(proc))
            process::dereference(proc);
    }
}