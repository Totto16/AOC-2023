

#include "helpers/base.hpp"
#include "helpers/utility.hpp"
struct AoCDay01 : AoCDay {

  AoCDay01() : AoCDay(1) {
    //
  }

  ResultType solve(std::string input,
                   std::vector<std::string> numLiterals) const {

    ResultType result = 0;

    for (const auto &temp : splitByNewLine(input)) {

      std::vector<std::uint8_t> numbers{};

      if (temp.empty() || temp == "\n") {
        continue;
      }

      for (std::size_t i = 0; i < temp.size(); ++i) {

        const char &c = temp.at(i);

        if (isdigit(c)) {
          numbers.push_back(c - '0');
        }

        for (std::uint8_t j = 0; j < numLiterals.size(); ++j) {
          const auto &mapping = numLiterals.at(j);
          if (temp.rfind(mapping, i) == i) {
            numbers.push_back(j + 1);
          }
        }
      }
      assert(numbers.size() >= 1 && "at least one numbers expected");
      result += numbers.at(0) * 10 + numbers.back();
    }

    return result;
  }

  ResultType solvePart1(std::string input,
                        [[maybe_unused]] const bool is_example) const override {

    return solve(input, {});
  }

  ResultType solvePart2(std::string input,
                        [[maybe_unused]] const bool is_example) const override {

    const std::vector<std::string> mappings = {
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    return solve(input, mappings);
  }
};

DayRegister<AoCDay01> day01{Input::SameInput("input.txt") >>
                            Input::ExampleInput("example_1.txt", 142) >>
                            Input::ExampleInput("example_2.txt", 281)};
