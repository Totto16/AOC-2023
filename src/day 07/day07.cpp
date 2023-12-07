

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

namespace Day07 {

    struct Card {
        std::uint8_t value;

        static Card parseCard(const char value) {

            Card result{ 0 };

            if (isdigit(value) != 0) {
                assert_greater_eq<std::uint8_t>(value, '2', "A card value must be greater or equal than '2'");
                assert_less_eq<std::uint8_t>(value, '9', "A card value must be less or equal than '9'");
                result.value = value - '2';
                return result;
            }

            switch (value) {
                case 'T':
                    result.value = 10;
                    break;
                case 'J':
                    result.value = 11;
                    break;
                case 'Q':
                    result.value = 12;
                    break;
                case 'K':
                    result.value = 13;
                    break;
                case 'A':
                    result.value = 14;
                    break;
                default:
                    assert_unreachable(std::format("Not allowed value for card: '{}'", value));
            }


            return result;
        }
    };


    enum class HandType : std::uint8_t {
        HighCard = 0,
        OnePair,
        TwoPairs,
        ThreeSame,
        FullHouse,
        FourSame,
        FiveSame,

    };


    struct Hand {
        std::array<Card, 5> cards;

        static Hand parseHand(const std::string& input) {

            assert_equal<std::size_t>(input.size(), 5u, "A Hand has to have 5 cards");

            std::array<Card, 5> cards{};
            for (std::size_t i = 0; i < input.size(); ++i) {
                cards.at(i) = Card::parseCard(input.at(i));
            }

            return Hand{ cards };
        }

        HandType getHandType() const {

            std::unordered_map<std::uint8_t, std::uint8_t> frequencies{};
            for (const auto& card : cards) {
                std::uint8_t last_value = 0;
                if (frequencies.contains(card.value)) {
                    last_value = frequencies.at(card.value);
                }
                frequencies.insert_or_assign(card.value, last_value + 1);
            }

            if (frequencies.size() == 1) {
                return HandType::FiveSame;
            }

            if (frequencies.size() == 5) {
                return HandType::HighCard;
            }

            if (frequencies.size() == 2) {

                bool fourSame = false;
                for (const auto& [key, val] : frequencies) {
                    if (val == 4) {
                        fourSame = true;
                    }
                }

                return fourSame ? HandType::FourSame : HandType::FullHouse;
            }

            if (frequencies.size() == 3) {

                bool threeSame = false;
                for (const auto& [key, val] : frequencies) {
                    if (val == 3) {
                        threeSame = true;
                    }
                }

                return threeSame ? HandType::ThreeSame : HandType::TwoPairs;
            }

            if (frequencies.size() == 4) {
                return HandType::OnePair;
            }

            assert_unreachable("All cases should be handled");
        }

        bool operator<(const Hand& hand) const {
            const auto type = std::to_underlying(getHandType());
            const auto other_type = std::to_underlying(hand.getHandType());

            if (type < other_type) {
                return true;
            }

            if (type > other_type) {
                return false;
            }

            for (std::size_t i = 0; i < cards.size(); ++i) {
                if (cards.at(i).value != hand.cards.at(i).value) {
                    return cards.at(i).value < hand.cards.at(i).value;
                }
            }

            // both are equal
            return false;
        }
    };


} // namespace Day07

struct AoCDay07 : AoCDay {

    AoCDay07() : AoCDay(7) {
        //
    }

    ResultType solvePart1(const std::string& input, [[maybe_unused]] const bool is_example) const override {

        std::vector<std::pair<Day07::Hand, ResultType>> parsed{};

        for (const auto& line : splitByNewLine(input)) {

            if (line.empty()) {
                continue;
            }

            const auto split = splitByRegex(line, R"( )");
            assert_equal<std::size_t>(split.size(), 2u, "each line has two entries");

            const auto num = get_number<ResultType>(split.at(1));
            assert_has_value(num, "Expected number");

            const auto hand = Day07::Hand::parseHand(split.at(0));

            parsed.emplace_back(hand, num.value());
        }

        std::sort(parsed.begin(), parsed.end(), [](const auto& lhs, const auto& rhs) { return lhs.first < rhs.first; });

        ResultType result = 0;

        for (std::size_t i = 0; i < parsed.size(); ++i) {
            const auto& val = parsed.at(i);
            result += val.second * (i + 1);
        }


        return result;
    }

    ResultType solvePart2(const std::string& input, [[maybe_unused]] const bool is_example) const override {
        ResultType result = 0;
        result += input.size();

        return result;
    }
};

DayRegister<AoCDay07> day07{ Input::SameInput("input.txt") >> Input::SameExample("example.txt", 6440, -1) };
