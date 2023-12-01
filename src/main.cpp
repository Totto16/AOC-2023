#include <format>
#include <iostream>
#include <string>
#include <vector>

#include "helpers/arguments.hpp"
#include "helpers/base.hpp"
#include "helpers/parser.hpp"

int main(const int argc, char const *argv[]) {

  try {

    std::vector<std::string> arguments{};
    arguments.reserve(argc);
    for (int i = 0; i < argc; ++i) {
      arguments.emplace_back(argv[i]);
    }

    const Arguments program_args = parse_arguments(arguments);

    const auto day = program_args.day;

    const std::optional<AoCDayStorageType> registered_day =
        get_registered_day(day);

    if (!registered_day.has_value()) {

      std::cerr << std::format("No such registered Day {:02} !\n", day);
      std::exit(2);
    }

    const auto &[description, day_class] = registered_day.value();

    const auto result = day_class->start(description);

    if (!result.has_value()) {
      std::cerr << std::format("Error in executing day {:02}: {}\n", day,
                               result.error());
      std::exit(3);
    }

  } catch (const std::exception &err) {
    std::cerr << "Error: " << err.what() << std::endl;
    std::exit(1);
  }
}
