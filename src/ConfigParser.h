#pragma once

#include "Config.h"
#include <string_view>

// ConfigParser is responsible for parsing plain text configuration input
// into a Config object containing typed values.
class ConfigParser {
    public:
        // Parses the contents of `text` as configuration lines and returns a
        // Config object with typed values for each parsed key.
        //
        // Expected format for each line: key=value
        // Supported value types are int, double, bool, and string.
        Config parseConfig(std::string_view text) const;
};
