

#include "arguments.hpp"

const Arguments parse_arguments(const std::vector<std::string> &arguments) {

  argparse::ArgumentParser program{arguments.at(0), STRINGIFY(VERSION_STRING),
                                   argparse::default_arguments::all};

  program.add_argument("day")
      .help("The day to execute")
      .required()
      .scan<'u', std::uint8_t>();
  ;

  try {
    program.parse_args(arguments);

    const std::uint8_t day = program.get<std::uint8_t>("day");

    return Arguments{day};

  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(4);
  }
}
