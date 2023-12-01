

#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <argparse/argparse.hpp>

#define STRINGIFY(a) STR_IMPL(a)
#define STR_IMPL(a) #a

struct Arguments {
  std::uint8_t day;
};

[[nodiscard]] const Arguments
parse_arguments(const std::vector<std::string> &arguments);
