#include "measure.hpp"

using namespace std::literals::chrono_literals;

std::string internals::to_string(const std::chrono::nanoseconds &dur,
                                 const PrintOptions &options) {

  // TODO use options
  (void)options;

  std::stringstream ss;

  auto hours = duration_cast<std::chrono::hours>(dur);

  auto mins = duration_cast<std::chrono::minutes>(dur) % 1h;

  auto secs = duration_cast<std::chrono::seconds>(dur) % 1min;

  auto msecs = duration_cast<std::chrono::milliseconds>(dur) % 1s;

  auto usecs = duration_cast<std::chrono::microseconds>(dur) % 1ms;

  auto nsecs = duration_cast<std::chrono::nanoseconds>(dur) % 1us;

  bool printed_earlier = false;

  if (printed_earlier || hours >= 1h) {
    printed_earlier = true;
    ss << hours.count() << ' ' << (1h != hours ? "hours" : "hour") << ' ';
  }
  if (printed_earlier || mins >= 1min) {
    printed_earlier = true;
    ss << mins.count() << ' ' << (1min != mins ? "minutes" : "minute") << ' ';
  }
  if (printed_earlier || secs >= 1s) {
    printed_earlier = true;
    ss << secs.count() << ' ' << (1s != secs ? "seconds" : "second") << ' ';
  }
  if (printed_earlier || msecs >= 1ms) {
    printed_earlier = true;
    ss << msecs.count() << ' '
       << (1ms != msecs ? "milliseconds" : "millisecond") << ' ';
  }

  if (printed_earlier || usecs >= 1us) {
    printed_earlier = true;
    ss << usecs.count() << ' '
       << (1us != usecs ? "microseconds" : "microsecond") << ' ';
  }

  if (printed_earlier || nsecs >= 1ns) {
    printed_earlier = true;
    ss << nsecs.count() << ' ' << (1ns != nsecs ? "nanoseconds" : "nanosecond");
  }

  return ss.str();
}
