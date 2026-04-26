#include "Config.h"

#include <utility>

void Config::set(std::string_view key, ConfigValue value) {
    auto* current = &root;
    size_t start = 0;
    while (true) {
        size_t dotPos = key.find('.', start);
        std::string_view part = key.substr(start, dotPos - start);

        if (dotPos == std::string_view::npos) {
            // Last part of the key, set the value here.
            (*current)[std::string(part)] = std::move(value);
            break;
        }

        // Intermediate part of the key, navigate or create nested map.
        auto& nextValue = (*current)[std::string(part)];
        if (!std::holds_alternative<ConfigValue::Map>(nextValue.value)) {
            // If it's not already a map, replace it with an empty map.
            nextValue.value = ConfigValue::Map{};
        }
        current = &std::get<ConfigValue::Map>(nextValue.value);

        start = dotPos + 1;
    }
}

const ConfigValue* Config::navigateToMap(std::string_view key) const {
    const Map* current = &root;
    size_t start = 0;
    while (true) {
        size_t dotPos = key.find('.', start);
        std::string_view part = key.substr(start, dotPos - start);

        auto it = current->find(std::string(part));
        if(it == current->end()) {
            return nullptr;
        }

        const ConfigValue& node = it->second;

        if (dotPos == std::string_view::npos) {
            return &node;
        }

        const Map* next = &std::get<Map>(node.value);
        if (!next) return nullptr;

        current = next;

        start = dotPos + 1;
    }
}

template<typename T>
std::optional<T> Config::get(std::string_view key) const {
    const ConfigValue* node = navigateToMap(key);
    if (!node) {
        return std::nullopt;
    }

    if (auto val = std::get_if<T>(&node->value)) {
        return *val;
    }

    return std::nullopt;
}

// Explicit instantiations for the types currently used by the project.
template std::optional<int> Config::get<int>(const std::string_view key) const;
template std::optional<bool> Config::get<bool>(const std::string_view key) const;
template std::optional<std::string> Config::get<std::string>(const std::string_view key) const;
template std::optional<double> Config::get<double>(const std::string_view key) const;
template std::optional<ConfigValue::Map> Config::get<ConfigValue::Map>(const std::string_view key) const;
