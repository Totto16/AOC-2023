

#include "helpers/base.hpp"
#include "helpers/parser.hpp"
#include "helpers/utility.hpp"

#include <algorithm>

enum BagColor { Red = 0, Green = 1, Blue = 2 };

struct AoCDay02 : AoCDay {

    AoCDay02() : AoCDay(2) {
        //
    }

    ResultType solvePart1(const std::string& input, [[maybe_unused]] const bool is_example) const override {

        ResultType result = 0;

        const std::array<ResultType, 3> upper_bound{ 12, 13, 14 };

        for (const auto& game : splitByNewLine(input)) {

            if (game.empty() || game == "\n") {
                continue;
            }

            std::array<ResultType, 3> maximum{
                std::numeric_limits<ResultType>::min(),
                std::numeric_limits<ResultType>::min(),
                std::numeric_limits<ResultType>::min(),
            };

            auto split1 = splitByRegex(game, R"(:)");
            assert_equal<std::size_t>(split1.size(), 2u, "only one ':' expected!");
            auto split2 = splitByRegex(split1.at(0), R"( )");
            assert_equal<std::size_t>(split2.size(), 2u, "expected: game <num>");

            const auto game_num = assert_has_value(get_number<ResultType>(split2.at(1)), "Number required here");

            for (auto& outer : splitByRegex(split1.at(1), R"(;)")) {

                trim(outer);
                for (auto& single : splitByRegex(outer, R"(,)")) {

                    trim(single);

                    auto parsed = splitByRegex(single, R"( )");
                    assert_equal<std::size_t>(parsed.size(), 2u, "expected color <num>");

                    const auto num = assert_has_value(get_number<ResultType>(parsed.at(0)), "Number required here");

                    const auto& color = parsed.at(1);

                    std::uint8_t index{};

                    if (color == "blue") {
                        index = std::to_underlying(BagColor::Blue);
                    } else if (color == "green") {
                        index = std::to_underlying(BagColor::Green);
                    } else if (color == "red") {
                        index = std::to_underlying(BagColor::Red);
                    } else {
                        throw std::runtime_error(std::format("Unrecognized color {}", color));
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

    ResultType solvePart2(const std::string& input, [[maybe_unused]] const bool is_example) const override {

        ResultType result = 0;

        for (const auto& game : splitByNewLine(input)) {

            if (game.empty() || game == "\n") {
                continue;
            }

            std::array<ResultType, 3> minimum{ std::numeric_limits<ResultType>::min(),
                                               std::numeric_limits<ResultType>::min(),
                                               std::numeric_limits<ResultType>::min() };

            auto split1 = splitByRegex(game, R"(:)");
            assert_equal<std::size_t>(split1.size(), 2u, "only one ':' expected!");

            for (auto& outer : splitByRegex(split1.at(1), R"(;)")) {

                trim(outer);
                for (auto& single : splitByRegex(outer, R"(,)")) {

                    trim(single);

                    auto parsed = splitByRegex(single, R"( )");
                    assert_equal<std::size_t>(parsed.size(), 2u, "expected color <num>");

                    const auto num = assert_has_value(get_number<ResultType>(parsed.at(0)), "Number required here");

                    const auto& color = parsed.at(1);

                    std::uint8_t index{};

                    if (color == "blue") {
                        index = std::to_underlying(BagColor::Blue);
                    } else if (color == "green") {
                        index = std::to_underlying(BagColor::Green);
                    } else if (color == "red") {
                        index = std::to_underlying(BagColor::Red);
                    } else {
                        throw std::runtime_error(std::format("Unrecognized color {}", color));
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

DayRegister<AoCDay02> day02{ Input::SameInput("input.txt") >> Input::SameExample("example.txt", 8, 2286) };
