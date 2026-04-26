#pragma once

#include <string>
#include <variant>
#include <optional>
#include <unordered_map>

// ConfigMap maps configuration keys to ConfigValue entries.
//using ConfigMap = std::unordered_map<std::string, struct ConfigValue>;

// ConfigValue stores a single configuration entry using one of
// the supported types: string, int, bool, or double.
struct ConfigValue{
    using Map = std::unordered_map<std::string, ConfigValue>;
    
    std::variant<std::string, int, bool, double, Map> value;
};

// Config is a simple in-memory container for parsed configuration values.
// It owns a map of keys to typed values and provides typed accessors.
struct Config {
    using Map = std::unordered_map<std::string, ConfigValue>;
    private:
        Map root;

    public:
        // Returns the stored value for `key` if it exists and can be converted
        // to the requested type T. Otherwise returns std::nullopt.
        template<typename T>
        std::optional<T> get(std::string_view key) const;

        // Sets the value for `key` to the specified ConfigValue.
        // If the key already exists, it is replaced.
        void set(std::string_view key, ConfigValue value);

    private:
        // Helper function to navigate to the nested map for a given key path.
        // Returns a pointer to the map where the final value should be stored.
        const ConfigValue* navigateToMap(std::string_view key) const;
};
