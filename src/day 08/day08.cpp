

#include "helpers/base.hpp"
#include "helpers/parser.hpp"
#include "helpers/utility.hpp"

#include <algorithm>
#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <utility>
#include <variant>

namespace Day08 {

    using MapKey = std::string;
    using MapValue = std::pair<std::string, std::string>;


    struct ParsedLine {
        MapKey key;
        MapValue value;
    };

    struct NodeName {
        static constexpr auto rule = [] { return lexy::dsl::times<3>(lexy::dsl::capture(lexy::dsl::ascii::upper)); }();
        static constexpr auto value = lexy::callback<std::string>(


                [](lexy::lexeme<lexy::_prd> a, lexy::lexeme<lexy::_prd> b, lexy::lexeme<lexy::_prd> c) {
                    std::string result{};

                    result += std::string{ a.begin(), a.end() };
                    result += std::string{ b.begin(), b.end() };
                    result += std::string{ c.begin(), c.end() };

                    return result;
                },
                []() {
                    std::string result{};
                    assert_unreachable("TODO");

                    return result;
                }
        );
    };

    struct LineParser {

        static constexpr auto rule = [] {
            const auto whitespace = lexy::dsl::ascii::space;
            return lexy::dsl::p<NodeName> + whitespace + LEXY_LIT("=") + whitespace + LEXY_LIT("(")
                   + lexy::dsl::p<NodeName> + LEXY_LIT(",") + whitespace + lexy::dsl::p<NodeName> + LEXY_LIT(")");
        }();
        static constexpr auto value =
                lexy::callback<ParsedLine>([](std::string name, std::string name_1, std::string name_2) {
                    return ParsedLine{
                        name,
                        {name_1, name_2}
                    };
                });
    };


} // namespace Day08


struct AoCDay08 : AoCDay {

    AoCDay08() : AoCDay(8) {
        //
    }

    ResultType solvePart1(const std::string& input, [[maybe_unused]] const bool is_example) const override {

        std::unordered_map<Day08::MapKey, Day08::MapValue> map{};
        std::string directions{};

        for (const auto& line : splitByNewLine(input)) {

            if (line.empty()) {
                continue;
            }

            if (directions.empty()) {
                directions = line;
                continue;
            }

            const auto parseResult = parse<Day08::LineParser, Day08::ParsedLine>(line);
            assert_has_value(parseResult, "Expected successful parsing");
            const auto value = parseResult.value();

            assert_true(!map.contains(value.key), "This has to be a new node!");

            map.insert_or_assign(value.key, value.value);
        }


        ResultType result = 0;

        std::string currentNode = "AAA";
        ResultType i = 0;

        while (true) {

            assert_true(map.contains(currentNode), "Current node must be in the network!");

            if (currentNode == "ZZZ") {
                break;
            }

            const auto node = map.at(currentNode);

            if (directions.at(i) == 'L') {
                currentNode = node.first;
            } else if (directions.at(i) == 'R') {
                currentNode = node.second;
            } else {
                assert_unreachable("Direction has to be Either L or R!");
            }


            i = (i + 1) % directions.size();
            ++result;
        }


        return result;
    }

    ResultType solvePart2(const std::string& input, [[maybe_unused]] const bool is_example) const override {


        ResultType result = 0;
        result += input.size();


        return result;
    }
};

DayRegister<AoCDay08> day08{ Input::SameInput("input.txt") >> Input::SameExample("example.txt", 2, -1) };
