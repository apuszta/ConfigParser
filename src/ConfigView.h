#pragma once

#include "Config.h"
#include "Binder.h"

class ConfigView {
    private:
        const Node& root;
        Binder binder;
    public:
        ConfigView(const Node& root)
            : root(root), binder(root)
        {}

        template<typename T>
        std::optional<T> get(std::string_view path) const {
            const Node* node = root.find(path);

            if (!node) {
                return std::nullopt;
            }

            return binder.convertPublic<T>(*node, std::string(path));
        }
};