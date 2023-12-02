

#include "helpers/base.hpp"
#include "helpers/utility.hpp"

#include <algorithm>

enum Color { Red = 0, Green = 1, Blue = 2 };

struct AoCDay02 : AoCDay {

  AoCDay02() : AoCDay(2) {
    //
  }

  ResultType solvePart1(const std::string &input,
                        [[maybe_unused]] const bool is_example) const override {

    ResultType result = 0;

    const std::array<ResultType, 3> upper_bound{12, 13, 14};

    for (const auto &game : splitByNewLine(input)) {

      if (game.empty() || game == "\n") {
        continue;
      }

      std::array<ResultType, 3> maximum{
          std::numeric_limits<ResultType>::min(),
          std::numeric_limits<ResultType>::min(),
          std::numeric_limits<ResultType>::min(),
      };

      auto split1 = splitByRegex(game, R"(:)");
      assert(split1.size() == 2 && "only one ':' expected!");
      auto split2 = splitByRegex(split1.at(0), R"( )");
      assert(split2.size() == 2 && "expected: game <num>");

      std::stringstream temp(split2.at(1));

      ResultType game_num;
      temp >> game_num;

      for (auto &outer : splitByRegex(split1.at(1), R"(;)")) {

        trim(outer);
        for (auto &single : splitByRegex(outer, R"(,)")) {

          trim(single);

          auto parsed = splitByRegex(single, R"( )");
          assert(parsed.size() == 2 && "expected color <num>");

          std::stringstream temp(parsed.at(0));

          ResultType num;
          temp >> num;

          const auto &color = parsed.at(1);

          std::uint8_t index;

          if (color == "blue") {
            index = Color::Blue;
          } else if (color == "green") {
            index = Color::Green;
          } else if (color == "red") {
            index = Color::Red;
          } else {
            throw std::runtime_error(
                std::format("Unrecognized color {}", color));
          }

          maximum.at(index) = std::max(maximum.at(index), num);
        }
      }

      bool correct = true;

      for (std::uint8_t i = 0; i < 3; ++i) {
        if (maximum.at(i) > upper_bound.at(i)) {
          correct = false;
          break;
        }
      }

      if (correct) {
        result += game_num;
      }
    }

    return result;
  }

  ResultType solvePart2(const std::string &input,
                        [[maybe_unused]] const bool is_example) const override {

    ResultType result = 0;

    for (const auto &game : splitByNewLine(input)) {

      if (game.empty() || game == "\n") {
        continue;
      }

      std::array<ResultType, 3> minimum{std::numeric_limits<ResultType>::min(),
                                        std::numeric_limits<ResultType>::min(),
                                        std::numeric_limits<ResultType>::min()};

      auto split1 = splitByRegex(game, R"(:)");
      assert(split1.size() == 2 && "only one ':' expected!");
      auto split2 = splitByRegex(split1.at(0), R"( )");
      assert(split2.size() == 2 && "expected: game <num>");

      std::stringstream temp(split2.at(1));

      ResultType game_num;
      temp >> game_num;

      for (auto &outer : splitByRegex(split1.at(1), R"(;)")) {

        trim(outer);
        for (auto &single : splitByRegex(outer, R"(,)")) {

          trim(single);

          auto parsed = splitByRegex(single, R"( )");
          assert(parsed.size() == 2 && "expected color <num>");

          std::stringstream temp(parsed.at(0));

          ResultType num;
          temp >> num;

          const auto &color = parsed.at(1);

          std::uint8_t index;

          if (color == "blue") {
            index = Color::Blue;
          } else if (color == "green") {
            index = Color::Green;
          } else if (color == "red") {
            index = Color::Red;
          } else {
            throw std::runtime_error(
                std::format("Unrecognized color {}", color));
          }

          minimum.at(index) = std::max(minimum.at(index), num);
        }
      }

      ResultType power = 1;

      for (std::uint8_t i = 0; i < 3; ++i) {
        power *= minimum.at(i);
      }

      result += power;
    }

    return result;
  }
};

DayRegister<AoCDay02> day02{Input::SameInput("input.txt") >>
                            Input::SameExample("example.txt", 8, 2286)};
