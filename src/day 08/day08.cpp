

#include "helpers/base.hpp"
#include "helpers/parser.hpp"
#include "helpers/utility.hpp"

#include <algorithm>
#include <cstdlib>
#include <memory>
#include <numeric>
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
                        std::move(name),
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
            const auto& value = assert_has_value(parseResult, "Expected successful parsing");

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


            i = (i + 1) % static_cast<ResultType>(directions.size());
            ++result;
        }


        return result;
    }


    ResultType solvePart2(const std::string& input, [[maybe_unused]] const bool is_example) const override {


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
            const auto& value = assert_has_value(parseResult, "Expected successful parsing");


            assert_true(!map.contains(value.key), "This has to be a new node!");

            map.insert_or_assign(value.key, value.value);
        }

        std::vector<std::string> currentNodes{};
        for (const auto& [key, value] : map) {
            if (key.back() == 'A') {
                currentNodes.push_back(key);
            }
        }

        std::vector<ResultType> loops{};

        for (const auto& startNode : currentNodes) {

            auto currentNode = startNode;

            ResultType i = 0;
            ResultType loop_count = 0;
            std::vector<ResultType> loop_counts{};
            std::vector<std::string_view> visited{};

            while (true) {

                assert_true(map.contains(currentNode), "Current node must be in the network!");

                if (currentNode.back() == 'Z') {
                    loop_counts.push_back(loop_count);
                    loop_count = 0;

                    if (std::find(visited.begin(), visited.end(), currentNode) != visited.end()) {
                        if (i == 0) {
                            // The input seems to have this property, that every loop_count is the same, this makes the math easy, since lcm can be used without any restrictions, I simply assert that here, so that no invalid solution can be calculated!

                            const auto loop_count_cmp = loop_counts.front();

                            const auto has_equal_distance = std::all_of(
                                    loop_counts.begin(), loop_counts.end(),
                                    [loop_count_cmp](const auto& count) { return count == loop_count_cmp; }
                            );
                            assert_true(has_equal_distance, "All loop have to have the same distance!");
                            loops.push_back(loop_count_cmp);
                            break;
                        }
                    }
                    visited.push_back(currentNode);
                }


                const auto node = map.at(currentNode);

                if (directions.at(i) == 'L') {
                    currentNode = node.first;
                } else if (directions.at(i) == 'R') {
                    currentNode = node.second;
                } else {
                    assert_unreachable("Direction has to be Either L or R!");
                }

                ++loop_count;
                i = (i + 1) % static_cast<ResultType>(directions.size());
            }
        }

        // use lcm of all loops
        return std::accumulate(loops.begin(), loops.end(), 1LL, std::lcm<ResultType, ResultType>);
    }
};

DayRegister<AoCDay08> day08{ Input::SameInput("input.txt") >> Input::ExampleInput("example.txt", 2)
                             >> Input::ExampleInput("example_2.txt", 6) };
