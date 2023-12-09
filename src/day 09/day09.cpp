

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

namespace Day09 {
    std::vector<ResultType> parse_numbers(const std::string& content) {
        std::vector<ResultType> vec{};
        for (const auto& num : splitByRegex(content, R"( )")) {
            if (num.empty()) {
                continue;
            }
            const auto num_value = get_number<ResultType>(num);
            assert_has_value(num_value, "This has to be a number");
            vec.push_back(num_value.value());
        }

        return vec;
    };


} // namespace Day09


struct AoCDay09 : AoCDay {

    AoCDay09() : AoCDay(9) {
        //
    }

    ResultType solvePart1(const std::string& input, [[maybe_unused]] const bool is_example) const override {

        ResultType result = 0;

        for (const auto& line : splitByNewLine(input)) {

            const auto& start_numbers = Day09::parse_numbers(line);

            std::vector<std::vector<ResultType>> numbers{ start_numbers };
            for (std::size_t i = 0; i < numbers.size(); ++i) {

                const auto& current_numbers = numbers.back();


                const auto have_all_0 = std::all_of(
                        current_numbers.begin(), current_numbers.end(), [](const auto& number) { return number == 0; }
                );
                if (!have_all_0) {


                    assert_greater<std::size_t>(
                            current_numbers.size(), 0, "At least one element expected in current_numbers list!"
                    );

                    std::vector<ResultType> difference{};
                    for (std::size_t j = 1; j < current_numbers.size(); ++j) {
                        difference.push_back(current_numbers.at(j) - current_numbers.at(j - 1));
                    }


                    numbers.push_back(difference);
                }
            }

            assert_greater<std::size_t>(numbers.size(), 1, "At least two element expected in numbers list!");

            numbers.back().push_back(0);

            for (std::size_t i = numbers.size() - 1; i != 0; --i) {

                const auto new_element = numbers.at(i).back() + numbers.at(i - 1).back();

                numbers.at(i - 1).push_back(new_element);
            }

            result += numbers.front().back();
        }


        return result;
    }


    ResultType solvePart2(const std::string& input, [[maybe_unused]] const bool is_example) const override {

        ResultType result = 0;

        result += input.size();

        return result;
    }
};

DayRegister<AoCDay09> day09{ Input::SameInput("input.txt") >> Input::SameExample("example.txt", 114, -1) };
