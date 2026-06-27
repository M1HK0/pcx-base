#include "pcx/file_util.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

namespace pcx {

bool create_directory(const std::string& path) {
    std::error_code ec;
    return fs::create_directories(path, ec) || fs::exists(path, ec);
}

bool create_file(const std::string& path, const std::string& content) {
    const auto parent = fs::path(path).parent_path();
    if (!parent.empty())
        create_directory(parent.string());
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;
    out.write(content.data(), static_cast<std::streamsize>(content.size()));
    return out.good();
}

bool read_file(const std::string& path, std::string& content) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return false;
    std::ostringstream ss;
    ss << in.rdbuf();
    content = ss.str();
    return true;
}

bool delete_file(const std::string& path) {
    std::error_code ec;
    return fs::remove(path, ec);
}

bool does_file_exist(const std::string& path) {
    std::error_code ec;
    return fs::exists(path, ec);
}

bool query_directory(const std::string& path, const std::vector<std::string>& extensions, std::vector<std::string>& files) {
    files.clear();
    std::error_code ec;
    if (!fs::exists(path, ec) || !fs::is_directory(path, ec))
        return false;

    for (const auto& entry : fs::directory_iterator(path, ec)) {
        if (!entry.is_regular_file(ec)) continue;
        const auto filename = entry.path().filename().string();
        for (const auto& ext : extensions) {
            if (filename.size() >= ext.size() &&
                filename.compare(filename.size() - ext.size(), ext.size(), ext) == 0) {
                files.push_back(filename);
                break;
            }
        }
    }
    return true;
}

bool copy_to_clipboard(const std::string& text) {
    if (!OpenClipboard(nullptr)) return false;
    EmptyClipboard();
    const size_t size = text.size() + 1;
    HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!mem) {
        CloseClipboard();
        return false;
    }
    void* ptr = GlobalLock(mem);
    memcpy(ptr, text.c_str(), size);
    GlobalUnlock(mem);
    SetClipboardData(CF_TEXT, mem);
    CloseClipboard();
    return true;
}

bool copy_from_clipboard(std::string& text) {
    text.clear();
    if (!OpenClipboard(nullptr)) return false;
    HANDLE data = GetClipboardData(CF_TEXT);
    if (!data) {
        CloseClipboard();
        return false;
    }
    const char* ptr = static_cast<const char*>(GlobalLock(data));
    if (ptr) text = ptr;
    GlobalUnlock(data);
    CloseClipboard();
    return !text.empty();
}

std::string get_username() {
    char buf[256] = {};
    DWORD size = sizeof(buf);
    if (GetUserNameA(buf, &size))
        return std::string(buf);
    return "user";
}

} // namespace pcx
