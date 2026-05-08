#include "ConfigParser.h"

#include <ranges>
#include <string>
#include <charconv>
#include <algorithm>

// Trim whitespace from the beginning and end of a token.
// This helper is internal to parsing and keeps keys/values normalized.
static std::string_view trim(std::string_view sv) {
    auto is_space = [](unsigned char c) { return std::isspace(c); };

    auto first = std::ranges::find_if_not(sv, is_space);
    auto last_rev = std::ranges::find_if_not(sv | std::views::reverse, is_space);
    auto last = last_rev.base();

    if (first >= last) {
        return std::string_view{};
    }

    return std::string_view(first, last - first);
}

Node ConfigParser::parseConfig(std::string_view text) const {
    Node root;
    root.value = Node::Map{};

    auto& rootMap = std::get<Node::Map>(root.value);

    for (auto lineView : text | std::views::split('\n')) {

        auto parts = lineView | std::views::split('=');

        auto it = parts.begin();
        if (it == parts.end()) continue;

        std::string key((*it).begin(), std::ranges::distance(*it));
        key = trim(key);

        ++it;
        if (it == parts.end()) continue;

        std::string value((*it).begin(), std::ranges::distance(*it));
        value = trim(value);

        insert(rootMap, key, value);
    }

    return root;
}

void ConfigParser::insert(
    Node::Map& root,
    std::string_view path,
    const std::string& value
) const {
    size_t start = 0;
    Node::Map* current = &root;

    while (true) {
        size_t dot = path.find('.', start);
        std::string key = std::string(path.substr(start, dot - start));

        if (dot == std::string_view::npos) {
            (*current)[key].value = value;
            return;
        }

        Node& next = (*current)[key];

        if (!next.isObject()) {
            next.value = Node::Map{};
        }

        current = std::get_if<Node::Map>(&next.value);
        start = dot + 1;
    }
}
