

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
#include <vector>

// TODO: using for all standard types: e.g day type = u8, solution type u64 etc.

namespace internals {

template <typename T, typename S>
concept IsBaseOf = std::is_base_of<T, S>::value;

std::expected<std::string, std::string> read(std::filesystem::path name) {

  if (!std::filesystem::exists(name)) {
    return std::unexpected(
        std::format("File '{}' doesn't exist!", name.string()));
  }

  std::ifstream fileInputStream(name);

  if (!fileInputStream.is_open()) {
    return std::unexpected(
        std::format("File '{}' couldn't be opened!", name.string()));
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
                   std::optional<Example> example_1,
                   std::optional<Example> example_2)
      : input_1_name{input_1_name}, input_2_name{input_2_name},
        example_1{example_1}, example_2{example_2} {
    //
  }
};

enum InputDescriptionGenerateTag : std::uint8_t {
  Input1 = 0b1,
  Input2 = 0b10,
  InputSingle = 0b100,
  ExampleSingle = 0b1000

};

struct InputDescriptionGeneratorSingle {
  std::uint8_t tag;
  std::string state;
  std::uint64_t value = 0;
};

struct InputDescriptionGeneratorMultiple {
  std::vector<InputDescriptionGeneratorSingle> all;

  InputDescriptionGeneratorMultiple(
      const std::initializer_list<InputDescriptionGeneratorSingle> &list)
      : all{list} {
    //
  }

  operator InputDescription() {
    return InputDescription{"", "", std::nullopt, std::nullopt};
  }
};

InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorSingle first,
           const InputDescriptionGeneratorSingle second) {

  return InputDescriptionGeneratorMultiple{first, second};
}

InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorSingle first,
           const InputDescriptionGeneratorMultiple second) {

  InputDescriptionGeneratorMultiple result{first};

  for (const auto &elem : second.all) {
    result.all.push_back(elem);
  }

  return result;
}

InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorMultiple first,
           const InputDescriptionGeneratorSingle second) {

  InputDescriptionGeneratorMultiple result{};

  for (const auto &elem : first.all) {
    result.all.push_back(elem);
  }
  result.all.push_back(second);

  return result;
}

InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorMultiple first,
           const InputDescriptionGeneratorMultiple second) {
  InputDescriptionGeneratorMultiple result{};

  for (const auto &elem : first.all) {
    result.all.push_back(elem);
  }

  for (const auto &elem : second.all) {
    result.all.push_back(elem);
  }

  return result;
}

namespace Input {

InputDescriptionGeneratorSingle SameInput(std::string name) {

  return InputDescriptionGeneratorSingle{
      InputDescriptionGenerateTag::Input1 | InputDescriptionGenerateTag::Input2,
      name};
}

InputDescriptionGeneratorSingle Input(std::string name) {

  return InputDescriptionGeneratorSingle{
      InputDescriptionGenerateTag::InputSingle, name};
}

InputDescriptionGeneratorMultiple
SameExample(std::string name, std::uint64_t result1, std::uint64_t result2) {

  return InputDescriptionGeneratorMultiple{
      InputDescriptionGeneratorSingle{
          InputDescriptionGenerateTag::ExampleSingle, name, result1},
      InputDescriptionGeneratorSingle{
          InputDescriptionGenerateTag::ExampleSingle, name, result2}};
}

InputDescriptionGeneratorSingle ExampleInput(std::string name,
                                             std::uint64_t result) {

  return InputDescriptionGeneratorSingle{
      InputDescriptionGenerateTag::ExampleSingle, name, result};
}
} // namespace Input

struct AoCDay {
private:
  std::filesystem::path getFilePath(std::string name) {
    return std::filesystem::path{std::format("src/day {:02}/{}", day, name)};
  }

  void executeExample(std::uint8_t part, Example example) {
    // TODO
    (void)example;
    (void)part;
  }

  void executePart(std::uint8_t part, std::string name) {
    // TODO
    (void)name;
    (void)part;
  }

public:
  std::uint8_t day;

  AoCDay(std::uint8_t day) : day{day} {
    //
  }

  virtual std::uint64_t solvePart1(std::string input,
                                   bool is_example) const = 0;

  virtual std::uint64_t solvePart2(std::string input,
                                   bool is_example) const = 0;

  std::expected<bool, std::string> start(InputDescription description) {

    if (description.example_1.has_value()) {
      executeExample(1, description.example_1.value());
    }

    executePart(1, description.input_1_name);

    if (description.example_2.has_value()) {
      executeExample(2, description.example_2.value());
    }

    executePart(2, description.input_2_name);

    return true;
  }
};

using AoCDayStorageType = std::pair<InputDescription, std::shared_ptr<AoCDay>>;

namespace internals {
std::unordered_map<std::uint8_t, AoCDayStorageType> available_days;

}

template <typename Day>
  requires internals::IsBaseOf<AoCDay, Day>
struct DayRegister {

  DayRegister(InputDescription description) {
    std::shared_ptr<Day> instance = std::make_shared<Day>();
    if (internals::available_days.contains(instance->day)) {
      throw std::runtime_error(std::format(
          "Can't register Day {:02}, since it's already registered!",
          instance->day));
    }

    internals::available_days.insert_or_assign(
        instance->day, AoCDayStorageType{description, std::move(instance)});
  }
};

std::optional<AoCDayStorageType> get_registered_day(std::uint8_t day) {

  if (!internals::available_days.contains(day)) {
    return std::nullopt;
  }

  return internals::available_days.at(day);
}
