

#include "helpers/base.hpp"
#include "helpers/parser.hpp"
#include "helpers/utility.hpp"

#include <algorithm>
#include <cstdlib>
#include <errno.h>
#include <memory>
#include <utility>
#include <variant>

namespace Day03 {
    enum class CharClass { Period, Symbol, Gear };

} // namespace Day03
// helper type for the visitor
template<class... Ts>
struct Overloaded : Ts... {
    using Ts::operator()...;
};

struct AoCDay03 : AoCDay {

    AoCDay03() : AoCDay(3) {
        //
    }

    using VectorContent = std::variant<Day03::CharClass, std::shared_ptr<ResultType>>;

    ResultType solvePart1(const std::string& input, [[maybe_unused]] const bool is_example) const override {

        ResultType result = 0;

        std::vector<std::vector<VectorContent>> rows{};

        for (const auto& line : splitByNewLine(input)) {

            std::size_t pos = 0;
            std::vector<VectorContent> inner_vec{};

            while (pos < line.size()) {
                auto num = get_number<ResultType>(line.substr(pos));
                if (num.has_value()) {

                    auto value = std::make_shared<ResultType>(num.value());

                    const auto num_size = std::to_string(num.value()).size();
                    for (std::size_t i = 0; i < num_size; ++i) {
                        inner_vec.emplace_back(value);
                    }

                    pos += num_size;
                } else {
                    const char given = line.at(pos);
                    if (given == '.') {
                        inner_vec.emplace_back(Day03::CharClass::Period);
                    } else {
                        inner_vec.emplace_back(Day03::CharClass::Symbol);
                    }
                    ++pos;
                }
            }

            rows.push_back(inner_vec);
        }

        auto is_symbol = [](const VectorContent& cont) {
            return std::visit(
                    Overloaded{ [](const Day03::CharClass& c) { return c == Day03::CharClass::Symbol; },
                                [](const std::shared_ptr<ResultType>&) { return false; } },
                    cont
            );
        };

        auto get_number_at = [&rows](const std::size_t i,
                                     const std::size_t j) -> std::optional<std::shared_ptr<ResultType>> {
            return std::visit(
                    Overloaded{ [](const Day03::CharClass&) -> std::optional<std::shared_ptr<ResultType>> {
                                   return std::nullopt;
                               },
                                [](const std::shared_ptr<ResultType>& s
                                ) -> std::optional<std::shared_ptr<ResultType>> { return s; } },
                    rows.at(i).at(j)
            );
        };

        auto get_pos = [&rows](std::size_t i, std::size_t j) {
            std::vector<std::pair<std ::size_t, std::size_t>> result{};

            const auto size_i = rows.size() - 1;
            const auto size_j = rows.at(i).size() - 1;

            if (i > 0) {
                if (j > 0) {
                    result.emplace_back(i - 1, j - 1);
                }

                result.emplace_back(i - 1, j);
                if (j < size_j) {
                    result.emplace_back(i - 1, j + 1);
                }
            }

            if (i < size_i) {
                if (j > 0) {
                    result.emplace_back(i + 1, j - 1);
                }
                result.emplace_back(i + 1, j);
                if (j < size_j) {
                    result.emplace_back(i + 1, j + 1);
                }
            }

            if (j > 0) {
                result.emplace_back(i, j - 1);
            }

            if (j < size_j) {
                result.emplace_back(i, j + 1);
            }

            return result;
        };

        for (std::size_t i = 0; i < rows.size(); ++i) {

            const auto columns = rows.at(i);

            for (std::size_t j = 0; j < columns.size(); ++j) {

                const auto contained = columns.at(j);

                if (is_symbol(contained)) {
                    std::vector<std::shared_ptr<ResultType>> added{};
                    for (const auto& [pos_i, pos_j] : get_pos(i, j)) {

                        auto res = get_number_at(pos_i, pos_j);
                        if (res.has_value()) {
                            const auto res_val = res.value();
                            bool already_included = false;
                            for (const auto& inc : added) {
                                if (inc.get() == res_val.get()) {
                                    already_included = true;
                                }
                            }

                            if (!already_included) {
                                result += *res_val;
                                added.emplace_back(res_val);
                            }
                        }
                    }
                }
            }
        }

        return result;
    }

    ResultType solvePart2(const std::string& input, [[maybe_unused]] const bool is_example) const override {

        ResultType result = 0;

        std::vector<std::vector<VectorContent>> rows{};

        for (const auto& line : splitByNewLine(input)) {

            std::size_t pos = 0;
            std::vector<VectorContent> inner_vec{};

            while (pos < line.size()) {
                auto num = get_number<ResultType>(line.substr(pos));
                if (num.has_value()) {

                    auto value = std::make_shared<ResultType>(num.value());

                    const auto num_size = std::to_string(num.value()).size();
                    for (std::size_t i = 0; i < num_size; ++i) {
                        inner_vec.emplace_back(value);
                    }

                    pos += num_size;
                } else {
                    const char given = line.at(pos);
                    if (given == '.') {
                        inner_vec.emplace_back(Day03::CharClass::Period);
                    } else if (given == '*') {
                        inner_vec.emplace_back(Day03::CharClass::Gear);
                    } else {
                        inner_vec.emplace_back(Day03::CharClass::Symbol);
                    }
                    ++pos;
                }
            }

            rows.push_back(inner_vec);
        }

        auto is_gear = [](const VectorContent& cont) {
            return std::visit(
                    Overloaded{ [](const Day03::CharClass& c) { return c == Day03::CharClass::Gear; },
                                [](const std::shared_ptr<ResultType>&) { return false; } },
                    cont
            );
        };

        auto get_number_at = [&rows](const std::size_t i,
                                     const std::size_t j) -> std::optional<std::shared_ptr<ResultType>> {
            return std::visit(
                    Overloaded{ [](const Day03::CharClass&) -> std::optional<std::shared_ptr<ResultType>> {
                                   return std::nullopt;
                               },
                                [](const std::shared_ptr<ResultType>& s
                                ) -> std::optional<std::shared_ptr<ResultType>> { return s; } },
                    rows.at(i).at(j)
            );
        };

        auto get_pos = [&rows](std::size_t i, std::size_t j) {
            std::vector<std::pair<std ::size_t, std::size_t>> result{};

            const auto size_i = rows.size() - 1;
            const auto size_j = rows.at(i).size() - 1;

            if (i > 0) {
                if (j > 0) {
                    result.emplace_back(i - 1, j - 1);
                }

                result.emplace_back(i - 1, j);
                if (j < size_j) {
                    result.emplace_back(i - 1, j + 1);
                }
            }

            if (i < size_i) {
                if (j > 0) {
                    result.emplace_back(i + 1, j - 1);
                }
                result.emplace_back(i + 1, j);
                if (j < size_j) {
                    result.emplace_back(i + 1, j + 1);
                }
            }

            if (j > 0) {
                result.emplace_back(i, j - 1);
            }

            if (j < size_j) {
                result.emplace_back(i, j + 1);
            }

            return result;
        };

        for (std::size_t i = 0; i < rows.size(); ++i) {

            const auto columns = rows.at(i);

            for (std::size_t j = 0; j < columns.size(); ++j) {

                const auto contained = columns.at(j);

                if (is_gear(contained)) {
                    std::vector<std::shared_ptr<ResultType>> added{};
                    for (const auto& [pos_i, pos_j] : get_pos(i, j)) {

                        auto res = get_number_at(pos_i, pos_j);
                        if (res.has_value()) {
                            const auto res_val = res.value();
                            bool already_included = false;
                            for (const auto& inc : added) {
                                if (inc.get() == res_val.get()) {
                                    already_included = true;
                                }
                            }

                            if (!already_included) {
                                added.emplace_back(res_val);
                            }
                        }
                    }

                    if (added.size() == 2) {

                        result += (*added.at(0)) * (*added.at(1));
                    }
                }
            }
        }

        return result;
    }
};

DayRegister<AoCDay03> day03{ Input::SameInput("input.txt") >> Input::SameExample("example.txt", 4361, 467835) };
