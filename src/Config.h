#pragma once

#include <string>
#include <variant>
#include <optional>
#include <unordered_map>

// ConfigValue stores a single configuration entry using one of
// the supported types: string, int, bool, or double.
using ConfigValue = std::variant<std::string, int, bool, double>;

// ConfigMap maps configuration keys to ConfigValue entries.
using ConfigMap = std::unordered_map<std::string, ConfigValue>;

// Config is a simple in-memory container for parsed configuration values.
// It owns a map of keys to typed values and provides typed accessors.
struct Config {
    ConfigMap values;

    // Returns the stored value for `key` if it exists and can be converted
    // to the requested type T. Otherwise returns std::nullopt.
    template<typename T>
    std::optional<T> get(const std::string& key) const;

    // Sets the value for `key` to the specified ConfigValue.
    // If the key already exists, it is replaced.
    void set(std::string key, ConfigValue value);
};
