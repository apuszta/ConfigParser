#include "ConfigParser.h"

#include <ranges>
#include <string>
#include <charconv>
#include <algorithm>

std::optional<int> try_parse_int(std::string_view sv) {
    int value;
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
    if (ec == std::errc() && ptr == sv.data() + sv.size()) {
        return value;
    }
    return std::nullopt;
}

bool is_double(std::string_view sv) {
    return std::count(sv.begin(), sv.end(), '.') == 1;
}

std::optional<double> try_parse_double(std::string_view sv) {
    if(!is_double(sv)) {
        return std::nullopt;
    }

    double value;
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
    if (ec == std::errc() && ptr == sv.data() + sv.size()) {
        return value;
    }
    return std::nullopt;
}

std::optional<bool> try_parse_bool(std::string_view sv) {
    if (sv == "true") {
        return true;
    } else if (sv == "false") {
        return false;
    }
    return std::nullopt;
}

Config ConfigParser::parseConfig(std::string_view text) const {
    Config config;

    for(auto lineView : text | std::ranges::views::split('\n')) {
        auto parts = lineView | std::ranges::views::split('=');

        auto it = std::ranges::begin(parts);

        if (it == std::ranges::end(parts)) {
            continue;
        }

        auto keyPart = *it;
        ++it;

        if (it == std::ranges::end(parts)) {
            continue;
        }

        auto valuePart = *it;

        auto to_sv = [](auto subrange) {
            return std::string_view(subrange.begin(), std::ranges::distance(subrange));
        };

        std::string_view key = to_sv(keyPart);
        std::string_view value = to_sv(valuePart);

        if (auto intValue = try_parse_int(value)) {
            config.set(std::string(key), *intValue);
        } else if (auto doubleValue = try_parse_double(value)) {
            config.set(std::string(key), *doubleValue);
        } else if (auto boolValue = try_parse_bool(value)) {
            config.set(std::string(key), *boolValue);
        } else {
            config.set(std::string(key), std::string(value));
        }
    }

    return config;
}
