#pragma once

#include "AppConfig.h"

template<typename T, typename Member>
struct Field {
    const char* name;
    Member T::* member;
    bool required;
};

template<typename T>
struct Descriptor;

template<typename T>
concept Bindable = requires {
    Descriptor<T>::fields;
};

template<>
struct Descriptor<AppConfig>
{
    inline static const auto fields = std::make_tuple(
        Field<AppConfig, DbConfig>{"db", &AppConfig::db, true},
        Field<AppConfig, bool>{"debug", &AppConfig::debug, false}
    );
};

template<>
struct Descriptor<DbConfig>
{
    inline static const auto fields = std::make_tuple(
        Field<DbConfig, std::string>{"host", &DbConfig::host, true},
        Field<DbConfig, int>{"port", &DbConfig::port, true}
    );
};
