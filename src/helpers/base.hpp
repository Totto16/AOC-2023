

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

#include "helpers/assert.hpp"
#include "helpers/color.hpp"
#include "helpers/measure.hpp"

using DayType = std::uint8_t;
using ResultType = std::int64_t;

namespace internals {

template <typename T, typename S>
concept IsBaseOf = std::is_base_of<T, S>::value;

std::expected<std::string, std::string> read(std::filesystem::path name);

enum class Part { Part1, Part2 };

} // namespace internals

struct Example {
  std::string input_name;
  ResultType result;
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
  InputTag,
  ExampleTag

};

struct InputDescriptionGeneratorSingle {
  InputDescriptionGenerateTag tag;
  std::string state;
  ResultType value = 0;
};

struct InputDescriptionGeneratorMultiple {
  std::vector<InputDescriptionGeneratorSingle> all;

  InputDescriptionGeneratorMultiple(
      const std::initializer_list<InputDescriptionGeneratorSingle> &list)
      : all{list} {
    //
  }

  operator InputDescription() {
    std::string input_1_name;
    std::string input_2_name;
    std::uint8_t input_i = 0;

    std::optional<Example> example_1;
    std::optional<Example> example_2;
    std::uint8_t example_i = 0;
    for (const auto &elem : this->all) {
      if (elem.tag == InputDescriptionGenerateTag::InputTag) {
        if (input_i >= 2) {
          throw std::runtime_error(std::format("To much inputs given!"));
        }
        if (input_i == 0) {
          input_1_name = elem.state;
        } else {
          input_2_name = elem.state;
        }

        ++input_i;
      } else {
        if (example_i >= 2) {
          throw std::runtime_error(std::format("To much Examples given!"));
        }
        if (example_i == 0) {
          example_1 = Example{elem.state, elem.value};
        } else {
          example_2 = Example{elem.state, elem.value};
        }

        ++example_i;
      }
    }

    return InputDescription{input_1_name, input_2_name, example_1, example_2};
  }
};

InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorSingle first,
           const InputDescriptionGeneratorSingle second);

InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorSingle first,
           const InputDescriptionGeneratorMultiple second);

InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorMultiple first,
           const InputDescriptionGeneratorSingle second);
InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorMultiple first,
           const InputDescriptionGeneratorMultiple second);
namespace Input {

InputDescriptionGeneratorMultiple SameInput(std::string name);

InputDescriptionGeneratorSingle Input(std::string name);

InputDescriptionGeneratorMultiple
SameExample(std::string name, ResultType result1, ResultType result2);

InputDescriptionGeneratorSingle ExampleInput(std::string name,
                                             ResultType result);
} // namespace Input

struct SuccessResult {};

// Rust like ? operator for std::expected, it throws an exception with
// ex.error() if it has an error, it is used in the implementation, see there on
// how to use it properly
template <typename T> void operator~(const std::expected<T, std::string> &ex) {
  if (!ex.has_value()) {
    throw std::runtime_error(ex.error());
  }
}

struct AoCDay {
private:
  std::filesystem::path getFilePath(std::string name) {
    return std::filesystem::path{std::format("src/day {:02}/{}", day, name)};
  }

  std::expected<SuccessResult, std::string> executeExample(internals::Part part,
                                                           Example example) {

    const uint8_t num = part == internals::Part::Part1 ? 1 : 2;

    try {

      const auto read_file = internals::read(getFilePath(example.input_name));
      if (!read_file.has_value()) {
        throw std::runtime_error(read_file.error());
      }

      const auto result = num == 1 ? this->solvePart1(read_file.value(), true)
                                   : this->solvePart2(read_file.value(), true);

      if (result == example.result) {
        std::cout << std::format(
            "{}Solved Example for part {}{}{}{} successfully{}\n",
            ForegroundColor::Green,
            Color::color(ForegroundColor::Cyan, Modifier::Bold), num,
            Color::reset(), ForegroundColor::Green, Color::reset());
      } else {
        const auto brackets = ForegroundColor::Cyan;
        const auto content = ForegroundColor::Red;
        throw std::runtime_error(std::format(
            "{}expected {}'{}{}{}'{} but "
            "got {}'{}{}{}'{}!{}",
            content, brackets, ForegroundColor::Green, example.result, brackets,
            content, brackets, ForegroundColor::Blue, result, brackets, content,
            Color::reset()));
      }

      return SuccessResult{};

    } catch (std::exception &ex) {
      return std::unexpected(std::format(
          "{}In example for part {}{}{}{}: {}", ForegroundColor::Red,
          Color::color(ForegroundColor::Cyan, Modifier::Bold), num,
          Color::reset(), ForegroundColor::Red, ex.what()));
    }
  }

  std::expected<SuccessResult, std::string>
  executePart(internals::Part part, const std::string &name,
              const bool measureTime) {
    const uint8_t num = part == internals::Part::Part1 ? 1 : 2;

    try {

      const auto read_file = internals::read(getFilePath(name));
      ~read_file;

      const std::function<ResultType()> execute = [this, &read_file, &num]() {
        return num == 1 ? this->solvePart1(read_file.value(), false)

                        : this->solvePart2(read_file.value(), false);
      };

      const auto result =
          measureTime
              ? measure(std::format(
                            "{}Day {}{:02}{}{} part {}{}{}",
                            ForegroundColor::Green,
                            Color::color(ForegroundColor::Cyan, Modifier::Bold),
                            day, Color::reset(), ForegroundColor::Green,
                            ForegroundColor::Cyan, num, Color::reset()),
                        execute)
              : execute();

      std::cout << std::format(
          "{}The solution for part {}{}{}{} is: {}'{}{}{}'{}\n",
          ForegroundColor::Blue,
          Color::color(ForegroundColor::Cyan, Modifier::Bold), num,
          Color::reset(), ForegroundColor::Blue, ForegroundColor::Cyan,
          ForegroundColor::Green, result, ForegroundColor::Cyan, Color::reset()

      );

      return SuccessResult{};

    } catch (std::exception &ex) {
      return std::unexpected(std::format(

          "{}Error while running part {}{}{}{}: {}{}\n", ForegroundColor::Red,
          Color::color(ForegroundColor::Cyan, Modifier::Bold), num,
          Color::reset(), ForegroundColor::Red, ex.what(), Color::reset()));
    }
  }

public:
  DayType day;

  AoCDay(DayType day) : day{day} {
    //
  }

  virtual ResultType solvePart1(const std::string &input,
                                bool is_example) const = 0;

  virtual ResultType solvePart2(const std::string &input,
                                bool is_example) const = 0;

  std::expected<SuccessResult, std::string> start(InputDescription description,
                                                  const bool measureTime) {

    try {

      if (description.example_1.has_value()) {
        ~executeExample(internals::Part::Part1, description.example_1.value());
      }

      ~executePart(internals::Part::Part1, description.input_1_name,
                   measureTime);

      if (description.example_2.has_value()) {
        ~executeExample(internals::Part::Part2, description.example_2.value());
      }

      ~executePart(internals::Part::Part2, description.input_2_name,
                   measureTime);

      return SuccessResult{};
    } catch (std::exception &ex) {
      return std::unexpected(ex.what());
    }
  }
};

using AoCDayStorageType = std::pair<InputDescription, std::shared_ptr<AoCDay>>;

namespace internals {

struct global_init {

  static std::shared_ptr<std::unordered_map<DayType, AoCDayStorageType>>
  available_days();
};

} // namespace internals

template <typename Day>
  requires internals::IsBaseOf<AoCDay, Day>
struct DayRegister {

  DayRegister(InputDescription description) {

    std::shared_ptr<Day> instance = std::make_shared<Day>();
    if (internals::global_init::available_days()->contains(instance->day)) {
      throw std::runtime_error(std::format(
          "{}Can't register Day {}{:02}{}{}, since it's already registered!{}",
          ForegroundColor::Red,
          Color::color(ForegroundColor::Yellow, Modifier::Bold), instance->day,
          Color::reset(), ForegroundColor::Red, Color::reset()));
    }

    internals::global_init::available_days()->insert_or_assign(
        instance->day, AoCDayStorageType{description, instance});
  }
};

std::optional<AoCDayStorageType> get_registered_day(DayType day);

std::vector<DayType> get_available_days();
