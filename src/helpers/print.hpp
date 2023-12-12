


#pragma once

#include <format>
#include <string>
#include <utility>


template<typename A, typename B, typename CharT>
struct std::formatter<std::pair<A, B>, CharT> : std::formatter<std::string, CharT> {

    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const std::pair<A, B>& value, std::format_context& ctx) const {

        return std::format_to(ctx.out(), "({}, {})", value.first, value.second);
    }
};
