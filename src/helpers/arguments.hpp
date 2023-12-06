

#pragma once

#include <argparse/argparse.hpp>
#include <filesystem>
#include <string>
#include <vector>

#include "helpers/base.hpp"

#define STRINGIFY(a) STR_IMPL(a)
#define STR_IMPL(a) #a

struct Arguments {
    DayType day;
    bool measureTime;
};

[[nodiscard]] Arguments parse_arguments(const std::vector<std::string>& arguments);
