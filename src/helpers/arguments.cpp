

#include "arguments.hpp"


const Arguments parse_arguments(const std::vector<std::string> &arguments) {

  argparse::ArgumentParser program{arguments.at(0), STRINGIFY(VERSION_STRING),
                                   argparse::default_arguments::all};

  program.add_argument("day")
      .help("The day to execute (0-25 -> 0 means all)")
      .required()
      .scan<'u', DayType>();
  ;

  try {
    program.parse_args(arguments);

    const DayType day = program.get<DayType>("day");

    return Arguments{day};

  } catch (const std::exception &err) {
    std::cerr << err.what() << "\n";
    std::cerr << program;
    std::exit(4);
  }
}
