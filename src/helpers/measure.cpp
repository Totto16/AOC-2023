#include "measure.hpp"

#include <vector>

using namespace std::literals::chrono_literals;

std::string internals::to_string(const std::chrono::nanoseconds &dur,
                                 const PrintOptions &options) {

  std::stringstream ss;

  std::array<Duration, internals::durationsAmount> durations = {
      Duration{std::chrono::duration_cast<std::chrono::hours>(dur), 1h, "hour",
               "hours"},
      Duration{std::chrono::duration_cast<std::chrono::minutes>(dur) % 1h, 1min,
               "minute", "minutes"},
      Duration{std::chrono::duration_cast<std::chrono::seconds>(dur) % 1min, 1s,
               "second", "seconds"},
      Duration{std::chrono::duration_cast<std::chrono::milliseconds>(dur) % 1s,
               1ms, "millisecond", "milliseconds"},
      Duration{std::chrono::duration_cast<std::chrono::microseconds>(dur) % 1ms,
               1us, "microsecond", "microseconds"},
      Duration{std::chrono::duration_cast<std::chrono::nanoseconds>(dur) % 1us,
               1ns, "nanosecond", "nanoseconds"},

  };

  const auto timeColor =
      options.useColor ? Color::color(ForegroundColor::Blue, Modifier::Bold)
                       : "";

  bool printed_earlier = false;
  for (const auto &[duration, unit, singular, plural, count] : durations) {

    if (printed_earlier || duration >= unit) {
      printed_earlier = true;
      ss << timeColor << count << Color::reset() << ' '
         << (unit == duration ? singular : plural) << ' ';
    }
  }

  return ss.str();
}
