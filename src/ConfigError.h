#pragma once

#include <stdexcept>
#include <string>

struct ConfigError : std::runtime_error {
    std::string path;

    ConfigError(std::string p, std::string msg)
        : std::runtime_error(p + ": " + msg),
          path(std::move(p)) {}
};