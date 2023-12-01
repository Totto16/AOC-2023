#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "helpers/parser.hpp"

int main(const int argc, char const *argv[]) {

  try {

    std::vector<std::string> arguments{};
    arguments.reserve(argc);
    for (int i = 0; i < argc; ++i) {
      arguments.emplace_back(argv[i]);
    }

    const Arguments program_args = parse_arguments(arguments);

  } catch (const std::exception &err) {
    std::cerr << "Error: " << err.what() << std::endl;
    std::exit(1);
  }
}
