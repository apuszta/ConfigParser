#include "Binder.h"

template<>
int Binder::convertScalar<int>(const Node& node, const std::string& path) const {
    const auto* str = node.as<std::string>();

    if (!str) {
        throw ConfigError(path, "expected string for integer conversion");
    }

    try {
        size_t pos = 0;

        int value = std::stoi(*str, &pos);

        if (pos != str->size()) {
            throw ConfigError(path, "invalid integer: '" + *str + "'");
        }

        return value;
    } catch (const std::invalid_argument&) {
        throw ConfigError(path, "invalid integer: '" + *str + "'");
    } catch (const std::out_of_range&) {
        throw ConfigError(path, "integer out of range: '" + *str + "'");
    }
}

template<>
double Binder::convertScalar<double>(const Node& node, const std::string& path) const {
    const auto* str = node.as<std::string>();

    if (!str) {
        throw ConfigError(path, "expected string for double conversion");
    }

    try {
        size_t pos = 0;

        double value = std::stod(*str, &pos);

        if (pos != str->size()) {
            throw ConfigError(path, "invalid double: '" + *str + "'");
        }

        return value;
    }
    catch (const std::invalid_argument&) {
        throw ConfigError(path, "invalid double: '" + *str + "'");
    }
    catch (const std::out_of_range&) {
        throw ConfigError(path, "double out of range: '" + *str + "'");
    }
}

template<>
bool Binder::convertScalar<bool>(const Node& node, const std::string& path) const {
    const auto* str = node.as<std::string>();

    if (!str) {
        throw ConfigError(path, "expected string for boolean conversion");
    }

    if (*str == "true" || *str == "1") {
        return true;
    } else if (*str == "false" || *str == "0") {
        return false;
    } else {
        throw ConfigError(path, "invalid boolean: '" + *str + "'");
    }
}

template<>
std::string Binder::convertScalar<std::string>(const Node& node, const std::string& path) const {
    const auto* str = node.as<std::string>();

    if (!str) {
        throw ConfigError(path, "expected string");
    }

    return *str;
}