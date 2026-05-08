#pragma once

#include <string>
#include <variant>
#include <unordered_map>
#include <vector>

struct Node {
    using Map = std::unordered_map<std::string, Node>;
    using Vector = std::vector<Node>;

    std::variant<
        std::monostate,
        int,
        double,
        bool,
        std::string,
        Map,
        Vector
    > value;

    // ==========================
    // TYPE CHECK
    // ==========================

    bool isObject() const noexcept {
        return std::holds_alternative<Map>(value);
    }

    bool isVector() const noexcept {
        return std::holds_alternative<Vector>(value);
    }

    bool isScalar() const noexcept {
        return !isObject() && !isVector() &&
               !std::holds_alternative<std::monostate>(value);
    }

    // ==========================
    // ACCESSORS
    // ==========================

    const Map* asObject() const {
        return std::get_if<Map>(&value);
    }

    const Vector* asVector() const {
        return std::get_if<Vector>(&value);
    }

    template<typename T>
    const T* as() const {
        return std::get_if<T>(&value);
    }

    // ==========================
    // SINGLE KEY LOOKUP
    // ==========================

    const Node* get(std::string_view key) const {
        const Map* map = asObject();
        if (!map) return nullptr;

        auto it = map->find(std::string(key));
        if (it == map->end()) return nullptr;

        return &it->second;
    }

    // ==========================
    // PATH LOOKUP (db.host.port)
    // ==========================

    const Node* find(std::string_view path) const {
        const Node* current = this;

        size_t start = 0;

        while (true) {
            size_t dot = path.find('.', start);
            std::string_view part = path.substr(start, dot - start);

            current = current->get(part);
            if (!current) return nullptr;

            if (dot == std::string_view::npos)
                return current;

            start = dot + 1;
        }
    }
};