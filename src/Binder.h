#pragma once

#include "Config.h"
#include "Descriptor.h"

#include <stdexcept>

struct BindOptions {
    bool strict = true;
    bool allow_missing = false;
};

class Binder{
    private:
        const Config& config;
    public:
        explicit Binder(const Config& config) : config(config) {}

        template<Bindable T>
        T bind(BindOptions options = BindOptions{}) const{
            ConfigValue root;
            root.value = config.getRoot();
            return convert<T>(root);
        }

    protected:
        template<typename T>
        T convert(const ConfigValue& v) const{
            if constexpr (Bindable<T>) {
                if (!std::holds_alternative<ConfigValue::Map>(v.value)) {
                    throw std::runtime_error("Expected a map for binding to struct");
                }

                const auto& fieldMap = std::get<ConfigValue::Map>(v.value);
                const auto& fields = Descriptor<T>::fields;

                T obj{};

                std::apply([&](auto&&... field) {
                    (
                        [&]() {
                            const auto& f = field;
                            const char* fieldName = f.name;
                            auto it = fieldMap.find(fieldName);
                            if (it == fieldMap.end()) {
                                if (f.required) {
                                    throw std::runtime_error("Missing required config key: " + std::string(fieldName));
                                }
                                return;
                            }

                            const ConfigValue& fieldValue = it->second;
                            auto convertedValue = convert<std::remove_reference_t<decltype(obj.*(f.member))>>(fieldValue);
                            obj.*(f.member) = convertedValue;
                        }(),
                        ...
                    );
                }, fields);

                return obj;
            }else {
                if (std::holds_alternative<T>(v.value)) {
                    return std::get<T>(v.value);
                }
                throw std::runtime_error("Cannot convert ConfigValue to requested type");
            }
        }
};