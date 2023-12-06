#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <string>

#include "color.hpp"

#pragma once

namespace internals {

    struct PrintOptions {
        bool leading;
        std::uint8_t maximum_precision;
        bool useColor;
    };

    using namespace std::literals::chrono_literals;

    struct Duration {
        std::chrono::nanoseconds duration;
        std::chrono::nanoseconds unit;
        std::string singular;
        std::string plural;
        std::int64_t count;

        template<typename INT, typename T>
        Duration(
                const std::chrono::duration<INT, T> duration,
                const std::chrono::duration<INT, T> unit,
                const std::string singular,
                const std::string plural
        )
            : duration{ std::chrono::duration_cast<std::chrono::nanoseconds>(duration) },
              unit{ std::chrono::duration_cast<std::chrono::nanoseconds>(unit) },
              singular{ singular },
              plural{ plural },
              count{ duration.count() } {
            //
        }
    };

    const constexpr std::uint8_t durationsAmount = 6;

    std::string to_string(const std::chrono::nanoseconds& dur, const PrintOptions& options);

} // namespace internals

/** @brief format for std::chrono::nanoseconds
 *  format options for this are
 *  {:<flags>:<maximum amount of precision>}
 * everything is optional, standard is {:Lc:3}
 * the flags section defines these arguments:
 * <lL>: leading units, print all units, ignores precision and prints all units
 * every time, lower case is true, uppercase is false, the last specifier is
 * taken
 * <cC>: use Color, defined, if the result uses color or not,lower case is true,
 * uppercase is false, the last specifier is taken
 * <maximum amount of precision>: a single digit between 1 and 6, it describes
 * the precision, that is maximally used, if this is e.g. 1, only the highest
 * order is used, standard is 3
 */
template<typename CharT>
struct std::formatter<std::chrono::nanoseconds, CharT> : std::formatter<std::string, CharT> {

    internals::PrintOptions options{ false, 3 };

    constexpr auto parse(std::format_parse_context& ctx) {
        auto pos = ctx.begin();
        std::array<std::string, 2> specifier{};
        std::size_t array_idx = 0;
        while (pos != ctx.end() && *pos != '}') {
            if (*pos == ':') {
                ++array_idx;
                if (array_idx >= 2) {
                    throw std::format_error("Format specifier can only have two specifiers!");
                }
                ++pos;
                continue;
            }
            specifier.at(array_idx) += *pos;
            ++pos;
        }

        for (const auto& c : specifier.at(0)) {
            switch (c) {
                case 'l':
                    options.leading = true;
                    break;
                case 'L':
                    options.leading = false;
                    break;
                case 'c':
                    options.useColor = true;
                    break;
                case 'C':
                    options.useColor = false;
                    break;
                default:
                    throw std::format_error("Unknown Format specifier in first section: allowed are 'cClL'");
            }
        }

        if (!specifier.at(1).empty()) {
            if (specifier.at(1).size() != 1) {
                throw std::format_error("Only one digit is allowed in the precision specifier");
            }

            const char digit = specifier.at(1).at(0);

            if (digit < '1' || digit > '6') {
                throw std::format_error("Only digits from 1-6 are allowed in the precision specifier!");
            }

            options.maximum_precision = digit - '0';
        }

        if (*pos != '}') {
            throw std::format_error("Format specifier doesn't end with '}'");
        }
        return pos;
    }

    auto format(const std::chrono::nanoseconds& nanoseconds, std::format_context& ctx) const {

        return std::format_to(ctx.out(), "{}", internals::to_string(nanoseconds, options));
    }
};

template<typename T>
T measure(const std::string& name, std::function<T()> execute) {
    std::cout << std::format(
            "{}measuring {}'{}{}{}'{}:{}\n", ForegroundColor::Blue, ForegroundColor::Cyan, ForegroundColor::Green, name,
            ForegroundColor::Cyan, ForegroundColor::Blue, Color::reset()
    );

    auto start = std::chrono::steady_clock::now();

    const T result = execute();

    auto end = std::chrono::steady_clock::now();

    const std::chrono::nanoseconds duration = end - start;

    std::cout << std::format(
            "{}It took: {}{:cL:3}{}\n", ForegroundColor::Yellow, ForegroundColor::Cyan, duration, Color::reset()
    );
    return result;
}
