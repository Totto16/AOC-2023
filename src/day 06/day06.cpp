

#include "helpers/base.hpp"
#include "helpers/parser.hpp"
#include "helpers/utility.hpp"

#include <algorithm>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <deque>
#include <memory>
#include <unordered_map>
#include <utility>
#include <variant>

namespace Day06 {


    void parseNumberArray(std::vector<ResultType>& vec, const std::string& inp) {

        for (const auto& num : splitByRegex(inp, R"( )")) {

            if (num.empty()) {
                continue;
            }

            const auto num_value = get_number<ResultType>(num);

            assert_has_value(num_value, "Expected number!");

            vec.push_back(num_value.value());
        }
    }

    void parseNumber(std::vector<ResultType>& vec, const std::string& inp) {

        std::string temp{};
        for (const auto& num : splitByRegex(inp, R"( )")) {

            if (num.empty()) {
                continue;
            }

            temp += num;
        }

        const auto prev_size = vec.size();

        parseNumberArray(vec, temp);

        assert_equal<std::size_t>(vec.size() - 1, prev_size, "Expected exactly one number!");
    }

} // namespace Day06

struct AoCDay06 : AoCDay {

    AoCDay06() : AoCDay(6) {
        //
    }

    ResultType solvePart1(const std::string& input, [[maybe_unused]] const bool is_example) const override {

        ResultType result = 1;

        std::vector<ResultType> times;
        std::vector<ResultType> distances;

        for (const auto& line : splitByNewLine(input)) {
            if (line.empty()) {
                continue;
            }

            if (times.empty()) {
                Day06::parseNumberArray(times, splitByRegex(line, R"(:)").at(1));
            } else {
                Day06::parseNumberArray(distances, splitByRegex(line, R"(:)").at(1));
            }
        }

        assert_equal<std::size_t>(times.size(), distances.size(), "Times and distances have to equally long!");

        for (std::size_t i = 0; i < times.size(); ++i) {
            const auto& time = times.at(i);
            const auto distance = distances.at(i);

            ResultType temp = 0;

            for (ResultType i = 1; i < time; ++i) {
                if (i * (time - i) > distance) {
                    ++temp;
                }
            }

            if (temp >= 1) {
                result *= temp;
            }
        }

        return result;
    }

    ResultType solvePart2(const std::string& input, [[maybe_unused]] const bool is_example) const override {
        ResultType result = 1;

        std::vector<ResultType> times;
        std::vector<ResultType> distances;

        for (const auto& line : splitByNewLine(input)) {
            if (line.empty()) {
                continue;
            }

            if (times.empty()) {
                Day06::parseNumber(times, splitByRegex(line, R"(:)").at(1));
            } else {
                Day06::parseNumber(distances, splitByRegex(line, R"(:)").at(1));
            }
        }

        assert_equal<std::size_t>(
                times.size(), distances.size(),
                "Times and distances have to equally "
                "long!"
        );
        assert_equal<std::size_t>(times.size(), 1u, "Times and distances have to have only oen element!");

        for (std::size_t i = 0; i < times.size(); ++i) {
            const auto& time = times.at(i);
            const auto distance = distances.at(i);

            ResultType temp = 0;

            for (ResultType i = 1; i < time; ++i) {
                if (i * (time - i) > distance) {
                    ++temp;
                }
            }

            if (temp >= 1) {
                result *= temp;
            }
        }

        return result;
    }
};

DayRegister<AoCDay06> day06{ Input::SameInput("input.txt") >> Input::SameExample("example.txt", 288, 71503) };
