#pragma once

#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;
    bool operator()(const std::string &lhs, const std::string &rhs) const { return lhs < rhs; }
    bool operator()(const std::string &lhs, std::string_view rhs) const { return lhs < rhs; }
    bool operator()(std::string_view lhs, const std::string &rhs) const { return lhs < rhs; }
};

struct TransparentStringEqual {
    using is_transparent = void;
    bool operator()(const std::string &lhs, const std::string &rhs) const { return lhs == rhs; }
    bool operator()(const std::string &lhs, std::string_view rhs) const { return lhs == rhs; }
    bool operator()(std::string_view lhs, const std::string &rhs) const { return lhs == rhs; }
};

struct TransparentStringHash {
    using is_transparent = void;
    std::size_t operator()(const std::string &s) const { return std::hash<std::string>{}(s); }
    std::size_t operator()(std::string_view sv) const { return std::hash<std::string_view>{}(sv); }
};

}  // namespace bookdb
