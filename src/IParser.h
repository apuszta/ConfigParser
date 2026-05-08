#pragma once

#include "Node.h"

class IParser {
public:
    virtual Node parse(std::string_view input) = 0;
};