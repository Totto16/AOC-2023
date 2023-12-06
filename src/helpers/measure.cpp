#include "measure.hpp"

#include <vector>

using namespace std::literals::chrono_literals;

std::string internals::to_string(const std::chrono::nanoseconds& dur, const PrintOptions& options) {

    std::string result{};

    std::array<Duration, internals::durationsAmount> durations = {
        Duration{             std::chrono::duration_cast<std::chrono::hours>(dur),   1h,        "hour","hours"                                                                                                       },
        Duration{      std::chrono::duration_cast<std::chrono::minutes>(dur) % 1h, 1min,      "minute",      "minutes"},
        Duration{    std::chrono::duration_cast<std::chrono::seconds>(dur) % 1min,   1s,      "second",      "seconds"},
        Duration{ std::chrono::duration_cast<std::chrono::milliseconds>(dur) % 1s,  1ms, "millisecond", "milliseconds"},
        Duration{std::chrono::duration_cast<std::chrono::microseconds>(dur) % 1ms,  1us, "microsecond",
                 "microseconds"                                                                                       },
        Duration{ std::chrono::duration_cast<std::chrono::nanoseconds>(dur) % 1us,  1ns,  "nanosecond",  "nanoseconds"},
    };

    const auto timeColor = options.useColor ? Color::color(ForegroundColor::Blue, Modifier::Bold) : "";

    const auto normalColor = options.useColor ? Color::color(ForegroundColor::Green) : "";
    const auto reset = options.useColor ? Color::reset() : "";

    bool printed_earlier = options.leading;
    std::uint8_t max = options.leading ? durationsAmount : options.maximum_precision;

    for (std::size_t i = 0; i < durations.size(); ++i) {
        const auto& [duration, unit, singular, plural, count] = durations.at(i);

        const auto* trailingWhiteSpace = i + 1u != durations.size() ? " " : "";
        const bool shouldPrint = (printed_earlier || duration >= unit) && max > 0;

        if (shouldPrint) {
            --max;
            printed_earlier = true;
            result += std::format(
                    "{}{}{}{} {}{}{}", timeColor, count, reset, normalColor, (unit == duration ? singular : plural),
                    trailingWhiteSpace, reset
            );
        }
    }

    return result;
    ;
}
