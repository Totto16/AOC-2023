

#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

namespace internals {

template <typename T, typename S>
concept IsBaseOf = std::is_base_of<T, S>::value;

std::expected<std::string, std::string> read(std::filesystem::path name) {

  if (!std::filesystem::exists(name)) {
    return std::unexpected(std::format("File '{}' doesn't exist!", name));
  }

  std::ifstream fileInputStream(name);

  if (!fileInputStream.is_open()) {
    return std::unexpected(std::format("File '{}' couldn't be opened!", name));
  }

  std::stringstream result;
  result << fileInputStream.rdbuf();
  return result.str();
}

} // namespace internals

struct Example {
  std::uint64_t result;
  std::string input_name;
};

struct InputDescription {
  std::string input_1_name;
  std::string input_2_name;

  std::optional<Example> example_1;
  std::optional<Example> example_2;

  InputDescription(std::string input_1_name, std::string input_2_name,
                   std::optional<Example> example_1_name,
                   std::optional<Example> example_2_name)
      : input_1_name{input_1_name}, input_2_name{input_2_name},
        example_1{example_1}, example_2{example_2} {
    //
  }

  static InputDescription SameInputSeperateexamples
};

struct AoCDay {

  std::uint8_t day;

  AoCDay(std::uint8_t day) : day{day} {
    //
  }

  virtual std::uint64_t solvePart1(const std::string &input,
                                   const bool is_example) = 0;

  virtual std::uint64_t solvePart2(const std::string &input,
                                   const bool is_example) = 0;

  std::expected<bool, std::string> start(InputDescription description) {

    const std::filesystem::path example_part1_name{
        std::format("src/day {:02}/{}", day, example_part1_name)};

    const std::uint64_t
  }
};

using AoCDayStorageType = std::pair<InputDescription, std::shared_ptr<AoCDay>>;

namespace internals {
std::unordered_map<std::uint8_t, AoCDayStorageType> available_days;

}

template <typename Day>
  requires internals::IsBaseOf<Day, AoCDay>
constexpr inline void register_day(InputDescription description) {
  std::shared_ptr<Day> instance = std::make_shared<Day>{};
  if (internals::available_days.contains(instance->day)) {
    throw std::runtime_error(
        std::format("Can't register Day {:02}, since it's already registered!",
                    instance->day));
  }

  internals::available_days.emplace(description, std::move(instance));
}

std::optional<AoCDayStorageType> get_registered_day(std::uint8_t day) {

  if (!internals::available_days.contains(day)) {
    return std::nullopt;
  }

  return internals::available_days.at(day);
}
