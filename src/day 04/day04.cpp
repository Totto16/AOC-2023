

#include "helpers/base.hpp"
#include "helpers/parser.hpp"
#include "helpers/utility.hpp"

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <cerrno>
#include <memory>
#include <unordered_map>
#include <utility>
#include <variant>

struct AoCDay04 : AoCDay {

    AoCDay04() : AoCDay(4) {
        //
    }

    ResultType solvePart1(const std::string& input, [[maybe_unused]] const bool is_example) const override {

        ResultType result = 0;

        auto parse_numbers = [](const std::string& content) {
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

        for (const auto& line : splitByNewLine(input)) {

            if (line.empty() || line == "\n") {
                continue;
            }

            const auto parts = splitByRegex(line, R"(\|)");

            assert_equal<std::size_t>(parts.size(), 2u, "expected two parts");

            const auto gameParts = splitByRegex(parts.at(0), R"(:)");

            assert_equal<std::size_t>(gameParts.size(), 2u, "expected two parts of the game line!");

            const auto winningNumbers = parse_numbers(gameParts.at(1));

            const auto myNumbers = parse_numbers(parts.at(1));

            std::size_t amount = 0;

            for (const auto myNumber : myNumbers) {

                if (std::find(winningNumbers.cbegin(), winningNumbers.cend(), myNumber) != winningNumbers.cend()) {
                    ++amount;
                }
            }

            result += amount == 0 ? 0 : 0x01u << (amount - 1);
        }

        return result;
    }

    ResultType solvePart2(const std::string& input, [[maybe_unused]] const bool is_example) const override {

        ResultType result = 0;

        auto parse_numbers = [](const std::string& content) {
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

        std::unordered_map<ResultType, ResultType> cardMap{};

        for (const auto& line : splitByNewLine(input)) {

            if (line.empty() || line == "\n") {
                continue;
            }

            const auto parts = splitByRegex(line, R"(\|)");

            assert_equal<std::size_t>(parts.size(), 2u, "expected two parts");

            const auto gameParts = splitByRegex(parts.at(0), R"(:)");

            assert_equal<std::size_t>(gameParts.size(), 2u, "expected two parts of the game line!");

            const auto cardSplit = splitByRegex(gameParts.at(0), R"( )");

            const auto cardNumber = get_number<ResultType>(cardSplit.back());
            assert_has_value(cardNumber, "cardNumber has to be a number!");
            const auto winningNumbers = parse_numbers(gameParts.at(1));

            const auto myNumbers = parse_numbers(parts.at(1));

            std::size_t amount = 0;

            for (const auto myNumber : myNumbers) {

                if (std::find(winningNumbers.cbegin(), winningNumbers.cend(), myNumber) != winningNumbers.cend()) {
                    ++amount;
                }
            }

            cardMap.insert_or_assign(cardNumber.value(), amount);
        }

        std::deque<ResultType> remainingCards = {};
        for (const auto& [key, _] : cardMap) {
            remainingCards.push_back(key);
        }

        while (remainingCards.size() != 0) {

            const auto currentCard = remainingCards.front();
            remainingCards.pop_front();
            ++result;
            const auto amount = cardMap.at(currentCard);
            if (amount == 0) {
                continue;
            }

            for (ResultType i = currentCard + 1; i <= currentCard + amount; ++i) {
                remainingCards.push_back(i);
            }
        }

        return result;
    }
};

DayRegister<AoCDay04> day04{ Input::SameInput("input.txt") >> Input::SameExample("example.txt", 13, 30) };
