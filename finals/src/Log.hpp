#pragma once

#include <cstring>
#include <string>

template<class... T>
void log(const std::string& str, const T&... ts) {
#ifndef CALMARE_DISABLE_LOG
    std::string line = str + "\n";
    std::printf(line.c_str(), ts...);
#endif
}
