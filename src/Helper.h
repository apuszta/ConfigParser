#pragma once

#include <string>
#include <string_view>

namespace PathHelper {
    // Joins two parts of a configuration key path with a dot.
    // If the base is empty, returns the next part as is.
    inline std::string join_path(std::string_view base, std::string_view next) {
        if (base.empty()) return std::string(next);
        return std::string(base) + "." + std::string(next);
    }

}