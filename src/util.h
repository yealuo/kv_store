//
// Created by Administrator on 2024/6/25.
//

#ifndef KV_STORE_UTIL_H
#define KV_STORE_UTIL_H

#include <string>

bool is_valid_string(const std::string &str, const char delimiter) {
    return !str.empty() && str.find(delimiter) != std::string::npos;
}

std::pair<std::string, std::string> get_kay_value_from_string(const std::string &str, const char delimiter) {
    if (!is_valid_string(str, delimiter))
        return {};

    std::string::size_type deli_pos = str.find_first_of(delimiter);
    return {str.substr(0, deli_pos), str.substr(deli_pos + 1)};
}

#endif //KV_STORE_UTIL_H
