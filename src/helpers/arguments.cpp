

#include "arguments.hpp"

const Arguments parse_arguments(const std::vector<std::string>& arguments) {

    argparse::ArgumentParser program{ arguments.at(0), STRINGIFY(VERSION_STRING), argparse::default_arguments::all };

    program.add_argument("day").help("The Day to execute (0-25 -> 0 means all)").required().scan<'u', DayType>();
    program.add_argument("-m", "--measure")
            .help("Measure the time of solutions")
            .default_value(false)
            .implicit_value(true);

    try {
        program.parse_args(arguments);

        const DayType day = program.get<DayType>("day");
        const bool measureTime = program.get<bool>("--measure");
        return Arguments{ day, measureTime };

    } catch (const std::exception& err) {
        std::cerr << err.what() << "\n";
        std::cerr << program;
        std::exit(4);
    }
}
