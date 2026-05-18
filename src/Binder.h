#pragma once

#include "Config.h"
#include "Descriptor.h"
#include "Node.h"
#include "Helper.h"
#include "ConfigError.h"

#include <stdexcept>

struct BindOptions {
    bool strict = true;
    bool allow_missing = false;
};

class Binder{
    private:
        const Node& root;
    public:
        explicit Binder(const Node& root) : root(root) {}

        template<Bindable T>
        T bind(BindOptions options = BindOptions{}) const{
            return convert<T>(root, "config");
        }

        template<typename T>
        T convertPublic(const Node& node, const std::string& path) const {
            return convert<T>(node, path);
        }

    protected:
        template<typename T>
        T convert(const Node& node, const std::string& path) const {
            if constexpr (Bindable<T>) {
                if (!node.isObject()) {
                    throw ConfigError(path, "expected object");
                }

                return bind_fields<T>(node, path);
            } else {
                return convertScalar<T>(node, path);
            }
        }

        template<typename T, typename Tuple>
        T bind_fields_impl(
            const Node& node,
            const std::string& path,
            const Tuple& fields
        ) const {
            T obj{};

            const auto* map = node.asObject();

            std::apply([&](auto&&... field) {((
                    [&]() {
                        const auto& f = field;
                        const Node* child = node.get(f.name);
                        std::string child_path = PathHelper::join_path(path, f.name);

                        if (!child) {
                            if (f.required) {
                                throw ConfigError(child_path, "missing required field");
                            }
                            return;
                        }

                        using MemberType = std::remove_reference_t<decltype(obj.*(f.member))>;

                        obj.*(f.member) = convert<MemberType>(*child, child_path);
                    }()
                ), ...);
            }, fields);

            return obj;
        }

        template<typename T>
        T bind_fields(const Node& node, const std::string& path) const {
            constexpr auto& fields = Descriptor<T>::fields;

            return bind_fields_impl<T>(
                node,
                path,
                fields
            );
        }

        template<typename T>
        T convertScalar(const Node&, const std::string&) const;
};