#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace pcx {

bool create_directory(const std::string& path);
bool create_file(const std::string& path, const std::string& content);
bool read_file(const std::string& path, std::string& content);
bool delete_file(const std::string& path);
bool does_file_exist(const std::string& path);
bool query_directory(const std::string& path, const std::vector<std::string>& extensions, std::vector<std::string>& files);
bool copy_to_clipboard(const std::string& text);
bool copy_from_clipboard(std::string& text);
std::string get_username();

} // namespace pcx
