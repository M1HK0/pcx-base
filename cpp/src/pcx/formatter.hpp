#pragma once

#include <sstream>
#include <string>

namespace pcx::string {

class Formatter {
    std::string buf_;

public:
    explicit Formatter(const std::string& fmt) : buf_(fmt) {}

    Formatter& replace(const std::string& token, const std::string& val) {
        const auto pos = buf_.find(token);
        if (pos != std::string::npos)
            buf_.replace(pos, token.size(), val);
        return *this;
    }

    Formatter& s(const std::string& val) { return replace("%s", val); }

    Formatter& d(int val) {
        return replace("%d", std::to_string(val));
    }

    Formatter& u(unsigned int val) {
        return replace("%u", std::to_string(val));
    }

    Formatter& x(int val) {
        std::ostringstream oss;
        oss << "0x" << std::hex << val;
        return replace("%x", oss.str());
    }

    Formatter& f(float val, int precision = 2) {
        std::ostringstream oss;
        oss.precision(precision);
        oss << std::fixed << val;
        return replace("%f", oss.str());
    }

    std::string str() const { return buf_; }
};

inline Formatter format(const std::string& fmt) {
    return Formatter(fmt);
}

} // namespace pcx::string
