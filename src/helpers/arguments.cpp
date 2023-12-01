

#include "arguments.hpp"

const Arguments parse_arguments(const std::vector<std::string> &arguments) {

  argparse::ArgumentParser program{arguments.at(0), STRINGIFY(VERSION_STRING),
                                   argparse::default_arguments::all};

  program.add_argument("day").help("The file to scan").required().scan<'i'>;

  try {
    program.parse_args(arguments);

    const int = program.get<int>("day");

    if (day < 0 && day > std::limits<std::uint8_t>) {
      throw std::runtime_error("Day value to large!");
    }

    return Arguments{day};

  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(4);
  }
}
