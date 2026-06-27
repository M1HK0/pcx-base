namespace process {
    proc_t reference(uint pid) {
        return ref_process(pid);
    }

    proc_t reference(const string &in proc_name) {
        return ref_process(proc_name);
    }

    void dereference(proc_t proc) {
        proc.deref();
    }

    bool is_alive(proc_t proc) {
        return proc.alive();
    }

    uint get_pid(proc_t proc) {
        return proc.pid();
    }

    uint64 get_peb(proc_t proc) {
        return proc.peb();
    }

    uint64 get_base_address(proc_t proc) {
        return proc.base_address();
    }

    bool get_module(proc_t proc, const string &in name, uint64 &out base, uint64 &out size) {
        return proc.get_module(name, base, size);
    }

    bool is_valid_address(proc_t proc, uint64 address) {
        return proc.is_valid_address(address);
    }

    // unsigned reads

    uint8 read_uint8(proc_t proc, uint64 address) {
        return proc.ru8(address);
    }

    uint16 read_uint16(proc_t proc, uint64 address) {
        return proc.ru16(address);
    }

    uint32 read_uint32(proc_t proc, uint64 address) {
        return proc.ru32(address);
    }

    uint64 read_uint64(proc_t proc, uint64 address) {
        return proc.ru64(address);
    }

    // signed reads

    int8 read_int8(proc_t proc, uint64 address) {
        return proc.r8(address);
    }

    int16 read_int16(proc_t proc, uint64 address) {
        return proc.r16(address);
    }

    int32 read_int32(proc_t proc, uint64 address) {
        return proc.r32(address);
    }

    int64 read_int64(proc_t proc, uint64 address) {
        return proc.r64(address);
    }

    // float reads

    float read_float(proc_t proc, uint64 address) {
        return proc.rf32(address);
    }

    double read_double(proc_t proc, uint64 address) {
        return proc.rf64(address);
    }

    // string reads

    string read_string(proc_t proc, uint64 address, int chars = 32) {
        return proc.rs(address, chars);
    }

    string read_wstring(proc_t proc, uint64 address, int chars = 32) {
        return proc.rws(address, chars);
    }

    // raw reads

    void read_vm(proc_t proc, uint64 address, uint size, array<uint8>@ buffer) {
        proc.rvm(address, size, buffer);
    }

    // unsigned writes

    bool write_uint8(proc_t proc, uint64 address, uint8 value) {
        return proc.wu8(address, value);
    }

    bool write_uint16(proc_t proc, uint64 address, uint16 value) {
        return proc.wu16(address, value);
    }

    bool write_uint32(proc_t proc, uint64 address, uint32 value) {
        return proc.wu32(address, value);
    }

    bool write_uint64(proc_t proc, uint64 address, uint64 value) {
        return proc.wu64(address, value);
    }

    // signed writes

    bool write_int8(proc_t proc, uint64 address, int8 value) {
        return proc.w8(address, value);
    }

    bool write_int16(proc_t proc, uint64 address, int16 value) {
        return proc.w16(address, value);
    }

    bool write_int32(proc_t proc, uint64 address, int32 value) {
        return proc.w32(address, value);
    }

    bool write_int64(proc_t proc, uint64 address, int64 value) {
        return proc.w64(address, value);
    }

    // float writes

    bool write_float(proc_t proc, uint64 address, float value) {
        return proc.wf32(address, value);
    }

    bool write_double(proc_t proc, uint64 address, double value) {
        return proc.wf64(address, value);
    }

    // string writes

    bool write_string(proc_t proc, uint64 address, const string &in text) {
        return proc.ws(address, text);
    }

    bool write_wstring(proc_t proc, uint64 address, const string &in text) {
        return proc.wws(address, text);
    }

    // raw writes

    bool write_vm(proc_t proc, uint64 address, const array<uint8> &in buffer) {
        return proc.wvm(address, buffer);
    }

    // pattern scanning

    uint64 find_pattern(proc_t proc, uint64 start, uint64 size, const string &in signature) {
        return proc.find_code_pattern(start, size, signature);
    }

    void find_all_patterns(proc_t proc, uint64 start, uint64 size, const string &in signature, array<uint64> &out result) {
        proc.find_all_code_patterns(start, size, signature, result);
    }

    // value scanning

    array<uint64>@ scan_uint32(proc_t proc, uint value, bool heap_only = false) {
        return proc.scan_u32(value, heap_only);
    }

    array<uint64>@ scan_uint64(proc_t proc, uint64 value, bool heap_only = false) {
        return proc.scan_u64(value, heap_only);
    }

    array<uint64>@ scan_float(proc_t proc, float value, bool heap_only = false) {
        return proc.scan_float(value, heap_only);
    }

    array<uint64>@ scan_double(proc_t proc, double value, bool heap_only = false) {
        return proc.scan_double(value, heap_only);
    }

    array<uint64>@ scan_string(proc_t proc, const string &in text, bool heap_only = false) {
        return proc.scan_string(text, heap_only);
    }

    array<uint64>@ scan_wstring(proc_t proc, const string &in text, bool heap_only = false) {
        return proc.scan_wstring(text, heap_only);
    }

    array<uint64>@ scan_pointer(proc_t proc, uint64 target, bool heap_only = false) {
        return proc.scan_pointer(target, heap_only);
    }

    // ue helpers

    string read_fstring(proc_t proc, uint64 address, int length = 0) {
        uint64 text = proc.ru64(address);
        if (text == 0) return "";
        if (length == 0)
            length = proc.ru32(address + 0x8);
        if (length <= 0 || length >= 10000) return "";
        return proc.rws(text, length);
    }

    string read_ftext(proc_t proc, uint64 address) {
        uint64 data = proc.ru64(address);
        if (data == 0) return "";
        uint64 name = proc.ru64(data + 0x20);
        if (name == 0) return "";
        int length = proc.ru32(data + 0x28);
        if (length <= 0 || length >= 512) return "";
        return proc.rws(name, length);
    }

    // vector helpers

    vector3 read_vector3f(proc_t proc, uint64 address) {
        vector3 vec;
        vec.readas_float(proc, address);
        return vec;
    }

    vector3 read_vector3d(proc_t proc, uint64 address) {
        vector3 vec;
        vec.readas_double(proc, address);
        return vec;
    }

    bool write_vector3f(proc_t proc, uint64 address, const vector3 &in vec) {
        return vec.writeas_float(proc, address);
    }

    bool write_vector3d(proc_t proc, uint64 address, const vector3 &in vec) {
        return vec.writeas_double(proc, address);
    }

    // matrix helpers

    matrix4x4 read_mat4(proc_t proc, uint64 address) {
        matrix4x4 mat;
        mat.readas_double(proc, address);
        return mat;
    }

    // quaternion helpers

    quaternion read_quatf(proc_t proc, uint64 address) {
        quaternion q;
        q.readas_float(proc, address);
        return q;
    }

    quaternion read_quatd(proc_t proc, uint64 address) {
        quaternion q;
        q.readas_double(proc, address);
        return q;
    }

    bool write_quatf(proc_t proc, uint64 address, const quaternion &in q) {
        return q.writeas_float(proc, address);
    }

    bool write_quatd(proc_t proc, uint64 address, const quaternion &in q) {
        return q.writeas_double(proc, address);
    }
}