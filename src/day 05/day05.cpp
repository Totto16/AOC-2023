

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

namespace Day05 {

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

struct MapEntry {

public:
  ResultType dest_start;
  std::pair<ResultType, ResultType> source;

  MapEntry(ResultType dest_start, ResultType source_start, ResultType length)
      : dest_start{dest_start},
        source{source_start, source_start + length - 1} {
    //
  }
  bool in_source_range(const ResultType input) const {
    return input >= source.first && input <= source.second;
  }

  ResultType map(const ResultType input) const {
    assert(in_source_range(input) && "only mappable, if in source range");
    return input - source.first + dest_start;
  }
};

struct Map {
  std::vector<MapEntry> entries;

  void add(const MapEntry &entry) { entries.push_back(entry); }

  ResultType map(const ResultType input) const {

    for (const auto &entry : entries) {
      if (entry.in_source_range(input)) {
        return entry.map(input);
      }
    }

    return input;
  }
};

} // namespace Day05

struct AoCDay05 : AoCDay {

  AoCDay05() : AoCDay(5) {
    //
  }

  ResultType solvePart1(const std::string &input,
                        [[maybe_unused]] const bool is_example) const override {

    auto lines = splitByNewLine(input);

    std::vector<ResultType> seeds = {};

    const auto seed_list = splitByRegex(lines.at(0), R"(:)");

    assert(seed_list.size() == 2 && "seed list has to be the correct length");

    for (const auto &seed : splitByRegex(seed_list.at(1), R"( )")) {

      if (seed.empty()) {
        continue;
      }

      const auto num = Day05::get_number(seed);
      assert(num.has_value() && "seed has to be a number");
      seeds.push_back(num.value());
    }

    // delete the first two lines!
    lines.erase(lines.begin());
    lines.erase(lines.begin());

    std::vector<Day05::Map> maps{};
    maps.push_back(Day05::Map{});

    std::size_t index = 0;

    for (const auto &line : lines) {

      if (line.empty()) {
        maps.push_back(Day05::Map{});
        ++index;
        continue;
      }

      if (line.back() == ':') {
        continue;
      }

      std::vector<ResultType> numbers = {};

      for (const auto &number_str : splitByRegex(line, R"( )")) {

        const auto num = Day05::get_number(number_str);
        assert(num.has_value() && "entry has to be a number");
        numbers.push_back(num.value());
      }

      assert(numbers.size() == 3 && "A map has to have 3 entries");

      maps.at(index).add(
          Day05::MapEntry{numbers.at(0), numbers.at(1), numbers.at(2)});
    }

    for (const auto &map : maps) {
      for (std::size_t i = 0; i < seeds.size(); ++i) {

        seeds.at(i) = map.map(seeds.at(i));
      }
    }

    std::sort(seeds.begin(), seeds.end());

    return seeds.at(0);
  }

  ResultType solvePart2(const std::string &input,
                        [[maybe_unused]] const bool is_example) const override {

    ResultType result = 0;

    result += input.size();

    return result;
  }
};

DayRegister<AoCDay05> day05{Input::SameInput("input.txt") >>
                            Input::SameExample("example.txt", 35, -1)};
