

#include "helpers/base.hpp"
#include "helpers/utility.hpp"

struct AoCDayXX : AoCDay {

    AoCDayXX() : AoCDay(1) {
        //
    }

    ResultType solvePart1(const std::string& input, [[maybe_unused]] const bool is_example) const override {

        ResultType result = 0;

        return result;
    }

    ResultType solvePart2(const std::string& input, [[maybe_unused]] const bool is_example) const override {

        ResultType result = 0;

        return result;
    }
};

DayRegister<AoCDayXX> dayxx{ Input::SameInput("input.txt") >> Input::ExampleInput("example_1.txt", -1)
                             >> Input::ExampleInput("example_2.txt", -1) };
