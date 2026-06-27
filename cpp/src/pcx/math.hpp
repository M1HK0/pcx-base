#pragma once

#include <algorithm>
#include <cmath>

namespace pcx {

template<typename T>
inline T clamp(T v, T lo, T hi) {
    return std::max(lo, std::min(v, hi));
}

inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

inline float smoothstep(float edge0, float edge1, float x) {
    float t = clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
    return t * t * (3.f - 2.f * t);
}

} // namespace pcx
