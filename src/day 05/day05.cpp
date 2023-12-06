

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

using Range = std::pair<ResultType, ResultType>;

enum class RangeState : std::uint8_t {
  FullyInclusive,
  NoOverlap,
  ThreeWayOverlap,
  RightOverlap,
  LeftOverlap
};

RangeState ranges_overlap(const Range &source, const Range &dest) {

  const auto &[start, end] = dest;
  const auto &[own_start, own_end] = source;

  if (own_start > end) {
    return RangeState::NoOverlap;
  }

  if (own_end < start) {
    return RangeState::NoOverlap;
  }

  if (start >= own_start && end <= own_end) {
    return RangeState::FullyInclusive;
  }

  if (start < own_start && end > own_end) {
    return RangeState::ThreeWayOverlap;
  }

  if (start >= own_start && end > own_end) {
    return RangeState::RightOverlap;
  }

  if (start < own_start && end <= own_end) {
    return RangeState::LeftOverlap;
  }

  assert_unreachable("didn't handle range case!");
}

struct MapEntry {

public:
  ResultType dest_start;
  Range source;

  MapEntry(ResultType dest_start, ResultType source_start, ResultType length)
      : dest_start{dest_start},
        source{source_start, source_start + length - 1} {
    //
  }
  bool in_source_range(const ResultType input) const {
    return input >= source.first && input <= source.second;
  }

  ResultType map(const ResultType input) const {
    assert_true(in_source_range(input), "only mappable, if in source range");
    return input - source.first + dest_start;
  }

  RangeState range_state(const Range &range) const {
    return ranges_overlap(source, range);
  }

  Range map_fully(const Range &input) const {

    assert_equal_enum(range_state(input), RangeState::FullyInclusive,
                      "only fully mappable, if 'FullyInclusive'");

    const auto &[start, end] = input;
    const auto shift_amount = dest_start - source.first;
    return {start + shift_amount, end + shift_amount};
  }

  std::pair<Range, std::vector<Range>> map_partly(const Range &input) const {

    const auto &[start, end] = input;
    const auto &[own_start, own_end] = source;
    const auto shift_amount = dest_start - source.first;
    switch (range_state(input)) {
    case RangeState::LeftOverlap:
      return {Range{own_start + shift_amount, end + shift_amount},
              {Range{start, own_start - 1}}};

    case RangeState::RightOverlap:
      return {Range{start + shift_amount, own_end + shift_amount},
              {Range{own_end + 1, end}}};

    case RangeState::ThreeWayOverlap:
      return {Range{own_start + shift_amount, own_end + shift_amount},
              {Range{start, own_start - 1}, Range{own_end + 1, end}}};

    default:
      assert_unreachable("only partly mappable, if overlapping");
    }
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

  std::vector<Range> map_range(const Range &input) const {

    std::vector<Range> result{};

    std::vector<Range> remaining_ranges{input};

    for (const auto &entry : entries) {

      std::vector<Range> unmapped_temp{};

      for (const auto &range : remaining_ranges) {

        switch (entry.range_state(range)) {
        case RangeState::FullyInclusive:
          result.push_back(entry.map_fully(range));
          break;
        case RangeState::NoOverlap:
          unmapped_temp.push_back(range);
          break;

        case RangeState::LeftOverlap:
        case RangeState::RightOverlap:
        case RangeState::ThreeWayOverlap: {
          const auto res = entry.map_partly(range);
          result.push_back(res.first);
          for (const auto &elem : res.second) {
            unmapped_temp.push_back(elem);
          }

          break;
        }
        default:
          assert_unreachable("RangeState not covered completely");
        }
      }

      remaining_ranges.clear();
      for (const auto &elem : unmapped_temp) {
        remaining_ranges.push_back(elem);
      }
    }

    // add remaining ranges to the result, here we have are mapped and unmapped
    // ranges
    for (const auto &remaining_range : remaining_ranges) {
      result.push_back(remaining_range);
    }

    return result;
  }
};

std::vector<Range> merge_ranges(const std::vector<Range> &input) {

  // not done: since it doesn't improve the performance by a lot
  return input;
}

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

    assert_equal<std::size_t>(seed_list.size(), 2u,
                              "seed list has to be the correct length");

    for (const auto &seed : splitByRegex(seed_list.at(1), R"( )")) {

      if (seed.empty()) {
        continue;
      }

      const auto num = Day05::get_number(seed);
      assert_has_value(num, "seed has to be a number");
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
        assert_has_value(num, "entry has to be a number");
        numbers.push_back(num.value());
      }

      assert_equal<std::size_t>(numbers.size(), 3u,
                                "A map has to have 3 entries");

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
    auto lines = splitByNewLine(input);

    std::vector<ResultType> seeds_raw = {};

    const auto seed_list = splitByRegex(lines.at(0), R"(:)");

    assert_equal<std::size_t>(seed_list.size(), 2u,
                              "seed list has to be the correct length");

    for (const auto &seed : splitByRegex(seed_list.at(1), R"( )")) {

      if (seed.empty()) {
        continue;
      }

      const auto num = Day05::get_number(seed);
      assert_has_value(num, "seed has to be a number");
      seeds_raw.push_back(num.value());
    }

    assert_equal<std::size_t>(seeds_raw.size() % 2, 0u,
                              "Seed amount has to be even!");

    std::vector<Day05::Range> seeds{};

    for (std::size_t i = 0; i < seeds_raw.size() / 2; ++i) {
      const auto start = seeds_raw.at(i * 2);
      const auto length = seeds_raw.at(i * 2 + 1);
      seeds.emplace_back(start, start + length - 1);
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
        assert_has_value(num, "entry has to be a number");
        numbers.push_back(num.value());
      }

      assert_equal<std::size_t>(numbers.size(), 3u,
                                "A map has to have 3 entries");

      maps.at(index).add(
          Day05::MapEntry{numbers.at(0), numbers.at(1), numbers.at(2)});
    }

    for (const auto &map : maps) {

      std::vector<Day05::Range> temp{};

      for (std::size_t i = 0; i < seeds.size(); ++i) {

        const auto res = map.map_range(seeds.at(i));
        for (const auto &elem : res) {
          temp.push_back(elem);
        }
      }

      seeds.clear();
      const auto merged_ranges = Day05::merge_ranges(temp);
      for (const auto &elem : merged_ranges) {
        seeds.push_back(elem);
      }
    }

    std::sort(seeds.begin(), seeds.end());

    return seeds.at(0).first;
  }
};

DayRegister<AoCDay05> day05{Input::SameInput("input.txt") >>
                            Input::SameExample("example.txt", 35, 46)};
