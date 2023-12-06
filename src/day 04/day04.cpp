

#include "helpers/base.hpp"
#include "helpers/utility.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <deque>
#include <errno.h>
#include <memory>
#include <unordered_map>
#include <utility>
#include <variant>

namespace Day04 {

std::optional<std::uint64_t> get_number(const std::string &inp) {
  char *end = nullptr;
  const char *c_str = inp.c_str();

  // don't allow -  or + or similar!
  if (!isdigit(*c_str)) {
    return std::nullopt;
  }

  // this function is bad, use better ones in the future!
  std::uint64_t result = std::strtoull(c_str, &end, 10);

  if (errno != 0) {
    return std::nullopt;
  }
  if (end == c_str) {
    return std::nullopt;
  }

  return result;
}
} // namespace Day04

struct AoCDay04 : AoCDay {

  AoCDay04() : AoCDay(4) {
    //
  }

  ResultType solvePart1(const std::string &input,
                        [[maybe_unused]] const bool is_example) const override {

    ResultType result = 0;

    auto parse_numbers = [](const std::string &content) {
      std::vector<ResultType> vec{};
      for (const auto &num : splitByRegex(content, R"( )")) {
        if (num.empty()) {
          continue;
        }
        const auto num_value = Day04::get_number(num);
        assert(num_value.has_value() && "This has to be a number");
        vec.push_back(num_value.value());
      }

      return vec;
    };

    for (const auto &line : splitByNewLine(input)) {

      if (line.empty() || line == "\n") {
        continue;
      }

      const auto parts = splitByRegex(line, R"(\|)");

      assert_equal<std::size_t>(parts.size(), 2u, "expected two parts");

      const auto gameParts = splitByRegex(parts.at(0), R"(:)");

      assert_equal<std::size_t>(gameParts.size(), 2u,
                                "expected two parts of the game line!");

      const auto winningNumbers = parse_numbers(gameParts.at(1));

      const auto myNumbers = parse_numbers(parts.at(1));

      std::size_t amount = 0;

      for (const auto myNumber : myNumbers) {

        if (std::find(winningNumbers.cbegin(), winningNumbers.cend(),
                      myNumber) != winningNumbers.cend()) {
          ++amount;
        }
      }

      result += amount == 0 ? 0 : std::pow(2, amount - 1);
    }

    return result;
  }

  ResultType solvePart2(const std::string &input,
                        [[maybe_unused]] const bool is_example) const override {

    ResultType result = 0;

    auto parse_numbers = [](const std::string &content) {
      std::vector<ResultType> vec{};
      for (const auto &num : splitByRegex(content, R"( )")) {
        if (num.empty()) {
          continue;
        }
        const auto num_value = Day04::get_number(num);
        assert(num_value.has_value() && "This has to be a number");
        vec.push_back(num_value.value());
      }

      return vec;
    };

    std::unordered_map<ResultType, ResultType> cardMap{};

    for (const auto &line : splitByNewLine(input)) {

      if (line.empty() || line == "\n") {
        continue;
      }

      const auto parts = splitByRegex(line, R"(\|)");

      assert_equal<std::size_t>(parts.size(), 2u, "expected two parts");

      const auto gameParts = splitByRegex(parts.at(0), R"(:)");

      assert_equal<std::size_t>(gameParts.size(), 2u,
                                "expected two parts of the game line!");

      const auto cardSplit = splitByRegex(gameParts.at(0), R"( )");

      const auto cardNumber = Day04::get_number(cardSplit.back());
      assert(cardNumber.has_value() && "cardNumber has to be a number!");
      const auto winningNumbers = parse_numbers(gameParts.at(1));

      const auto myNumbers = parse_numbers(parts.at(1));

      std::size_t amount = 0;

      for (const auto myNumber : myNumbers) {

        if (std::find(winningNumbers.cbegin(), winningNumbers.cend(),
                      myNumber) != winningNumbers.cend()) {
          ++amount;
        }
      }

      cardMap.insert_or_assign(cardNumber.value(), amount);
    }

    std::deque<ResultType> remainingCards = {};
    for (const auto &[key, _] : cardMap) {
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

DayRegister<AoCDay04> day04{Input::SameInput("input.txt") >>
                            Input::SameExample("example.txt", 13, 30)};
