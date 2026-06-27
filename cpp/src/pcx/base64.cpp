#include "pcx/base64.hpp"

#include <array>
#include <vector>

namespace pcx {
namespace {

constexpr char kEncodeTable[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int decode_char(unsigned char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

} // namespace

std::string base64_encode(const std::string& input) {
    std::string out;
    out.reserve(((input.size() + 2) / 3) * 4);

    for (size_t i = 0; i < input.size(); i += 3) {
        const uint32_t b0 = static_cast<unsigned char>(input[i]);
        const uint32_t b1 = (i + 1 < input.size()) ? static_cast<unsigned char>(input[i + 1]) : 0;
        const uint32_t b2 = (i + 2 < input.size()) ? static_cast<unsigned char>(input[i + 2]) : 0;
        const uint32_t triple = (b0 << 16) | (b1 << 8) | b2;

        out.push_back(kEncodeTable[(triple >> 18) & 0x3F]);
        out.push_back(kEncodeTable[(triple >> 12) & 0x3F]);
        out.push_back((i + 1 < input.size()) ? kEncodeTable[(triple >> 6) & 0x3F] : '=');
        out.push_back((i + 2 < input.size()) ? kEncodeTable[triple & 0x3F] : '=');
    }

    return out;
}

bool base64_decode(const std::string& input, std::string& output) {
    output.clear();
    std::vector<unsigned char> bytes;
    bytes.reserve(input.size());

    int val = 0;
    int valb = -8;
    for (unsigned char c : input) {
        if (c == '=') break;
        const int d = decode_char(c);
        if (d < 0) continue;
        val = (val << 6) + d;
        valb += 6;
        if (valb >= 0) {
            bytes.push_back(static_cast<unsigned char>((val >> valb) & 0xFF));
            valb -= 8;
        }
    }

    output.assign(bytes.begin(), bytes.end());
    return true;
}

} // namespace pcx
