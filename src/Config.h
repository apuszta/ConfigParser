#pragma once

#include "Node.h"
#include <optional>

// Config is a simple in-memory container for parsed configuration values.
// It owns a map of keys to typed values and provides typed accessors.
struct Config {
    private:
        Node rootNode;

    public:
        const Node& root() const {
            return rootNode;
        }

        template<typename T>
        std::optional<T> get(std::string_view path) const {
            const Node* node = rootNode.find(path);

            if (!node) {
                return std::nullopt;
            }

            if (const auto* ptr = node->as<T>()) {
                return *ptr;
            }

            return std::nullopt;
        }
};
