#pragma once

#include <string>

struct DbConfig {
    std::string host;
    int port;
};

struct AppConfig {
    DbConfig db;
    bool debug;
};