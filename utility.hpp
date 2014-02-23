#pragma once

#include <string>
#include <sstream>

template <typename T>
T to_numeric(const std::string& v)
{
    std::istringstream ss(v);
    T t;
    ss >> t;
    return t;
}

