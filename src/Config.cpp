#include "Config.h"

#include <utility>

void Config::set(std::string key, ConfigValue value) {
    values[std::move(key)] = std::move(value);
}

template<typename T>
std::optional<T> Config::get(const std::string& key) const {
    auto it = values.find(key);
    if (it == values.end()) {
        return std::nullopt;
    }

    if (auto val = std::get_if<T>(&it->second)) {
        return *val;
    }

    return std::nullopt;
}

// Explicit instantiations for the types currently used by the project.
template std::optional<int> Config::get<int>(const std::string& key) const;
template std::optional<bool> Config::get<bool>(const std::string& key) const;
template std::optional<std::string> Config::get<std::string>(const std::string& key) const;
template std::optional<double> Config::get<double>(const std::string& key) const;
