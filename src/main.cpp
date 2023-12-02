#include <format>
#include <iostream>
#include <string>
#include <vector>

#include "helpers/arguments.hpp"
#include "helpers/base.hpp"
#include "helpers/color.hpp"
#include "helpers/parser.hpp"

int main(const int argc, char const *argv[]) {

  try {

    std::vector<std::string> arguments{};
    arguments.reserve(argc);
    for (int i = 0; i < argc; ++i) {
      arguments.emplace_back(argv[i]);
    }

    const Arguments program_args = parse_arguments(arguments);

    const auto parsed_day = program_args.day;

    std::vector<DayType> days{};
    if (parsed_day == 0) {

      for (const auto &available_day : get_available_days()) {
        days.push_back(available_day);
      }

    } else {
      days.push_back(parsed_day);
    }

    for (const auto &day : days) {
      std::cout << std::format(
          "{}Running day {}{:02}{}\n", Color::color(ColorForeground::Blue),
          Color::color(ColorForeground::White, Modifier::Bold), day,
          Color::reset());

      const std::optional<AoCDayStorageType> registered_day =
          get_registered_day(day);

      if (!registered_day.has_value()) {
        std::cerr << std::format("No such registered Day {:02}!\n", day);
        std::exit(2);
      }

      const auto &[description, day_class] = registered_day.value();

      const auto result = day_class->start(description);

      if (!result.has_value()) {
        std::cerr << std::format("Error in executing day {:02}: {}\n", day,
                                 result.error());
        std::exit(3);
      }
      std::cout << "\n";
    }
  } catch (const std::exception &err) {
    std::cerr << "Error: " << err.what() << std::endl;
    std::exit(1);
  }
}
