#pragma once

#include <string>

namespace pcx {

std::string base64_encode(const std::string& input);
bool base64_decode(const std::string& input, std::string& output);

} // namespace pcx
