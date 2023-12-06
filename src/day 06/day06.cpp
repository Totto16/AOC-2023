

#include "helpers/base.hpp"
#include "helpers/utility.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <deque>
#include <errno.h>
#include <memory>
#include <unordered_map>
#include <utility>
#include <variant>

namespace Day06 {

std::optional<std::uint64_t> get_number(const std::string &inp) {
  char *end = nullptr;
  const char *c_str = inp.c_str();

  // don't allow -  or + or similar!
  if (!isdigit(*c_str)) {
    return std::nullopt;
  }

  // this function is bad, use better ones in the future!
  std::uint64_t result = std::strtoull(c_str, &end, 10);

  if (errno != 0) {
    return std::nullopt;
  }
  if (end == c_str) {
    return std::nullopt;
  }

  return result;
}

void parseNumberArray(std::vector<ResultType> &vec, const std::string &inp) {

  for (const auto &num : splitByRegex(inp, R"( )")) {

    if (num.empty()) {
      continue;
    }

    const auto num_value = get_number(num);

    assert(num_value.has_value() && "Expected number!");

    vec.push_back(num_value.value());
  }
}

} // namespace Day06

struct AoCDay06 : AoCDay {

  AoCDay06() : AoCDay(6) {
    //
  }

  ResultType solvePart1(const std::string &input,
                        [[maybe_unused]] const bool is_example) const override {

    ResultType result = 1;

    std::vector<ResultType> times;
    std::vector<ResultType> distances;

    for (const auto &line : splitByNewLine(input)) {
      if (line.empty()) {
        continue;
      }

      if (times.empty()) {
        Day06::parseNumberArray(times, splitByRegex(line, R"(:)").at(1));
      } else {
        Day06::parseNumberArray(distances, splitByRegex(line, R"(:)").at(1));
      }
    }

    assert(times.size() == distances.size() &&
           "Times and distances have to equally long!");

    for (std::size_t i = 0; i < times.size(); ++i) {
      const auto &time = times.at(i);
      const auto distance = distances.at(i);

      ResultType temp = 0;

      for (ResultType i = 1; i < time; ++i) {
        if (i * (time - i) > distance) {
          ++temp;
        }
      }

      if (temp >= 1) {
        result *= temp;
      }
    }

    return result;
  }

  ResultType solvePart2(const std::string &input,
                        [[maybe_unused]] const bool is_example) const override {
    ResultType result = 0;

    result += input.size();

    return result;
  }
};

DayRegister<AoCDay06> day06{Input::SameInput("input.txt") >>
                            Input::SameExample("example.txt", 288, -1)};
