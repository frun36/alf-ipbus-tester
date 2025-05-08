#pragma once

#include <expected>
#include <format>
#include <string>

template <typename T>
using Result = std::expected<T, std::string>;

template <typename... Args>
std::unexpected<std::string> Err(std::format_string<Args...> fmt,
                                           Args&&... args)
{
    return std::unexpected<std::string>(
        std::format(fmt, std::forward<Args>(args)...));
}

#define TRY(_expr)                                                \
    {                                                             \
        auto _expected = _expr;                                   \
        if (!_expected) {                                         \
            return std::unexpected(std::move(_expected.error())); \
        }                                                         \
    }

#define TRY_ASSIGN(_expr, _var)                                   \
    {                                                             \
        auto _expected = _expr;                                   \
        if (!_expected) {                                         \
            return std::unexpected(std::move(_expected.error())); \
        }                                                         \
        _var = std::move(*_expected);                             \
    }
