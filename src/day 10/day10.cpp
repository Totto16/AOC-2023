

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

namespace Day10 {


    enum class Tile : char {
        Vertical = '|',
        Horizontal = '-',
        NorthEast = 'L',
        NorthWest = 'J',
        SouthWest = '7',
        SouthEast = 'F',
        Ground = '.',
        StartPosition = 'S'
    };


    Tile get_tile(const char c) {
        switch (c) {
            case std::to_underlying(Tile::Vertical):
                return Tile::Vertical;
            case std::to_underlying(Tile::Horizontal):
                return Tile::Horizontal;
            case std::to_underlying(Tile::NorthEast):
                return Tile::NorthEast;
            case std::to_underlying(Tile::NorthWest):
                return Tile::NorthWest;
            case std::to_underlying(Tile::SouthWest):
                return Tile::SouthWest;
            case std::to_underlying(Tile::SouthEast):
                return Tile::SouthEast;
            case std::to_underlying(Tile::Ground):
                return Tile::Ground;
            case std::to_underlying(Tile::StartPosition):
                return Tile::StartPosition;
            default:
                assert_unreachable(std::format("No such tile: {}", c));
        }
    }

    using Pos = std::pair<ResultType, ResultType>;


} // namespace Day10


struct AoCDay10 : AoCDay {

    AoCDay10() : AoCDay(10) {
        //
    }

    ResultType solvePart1(const std::string& input, [[maybe_unused]] const bool is_example) const override {


        std::vector<std::vector<Day10::Tile>> tiles{};

        Day10::Pos startPos{ 0, 0 };

        for (const auto& line : splitByNewLine(input)) {
            if (line.empty()) {
                continue;
            }

            std::vector<Day10::Tile> row{};
            for (const auto c : line) {
                row.push_back(Day10::get_tile(c));
                if (row.back() == Day10::Tile::StartPosition) {
                    startPos = { tiles.size(), row.size() - 1 };
                }
            }
            tiles.push_back(row);
        }


        using RecursionType = std::pair<Day10::Tile, Day10::Pos>;

        const std::function<
                std::vector<RecursionType>(const Day10::Pos pos, const Day10::Tile tile, const Day10::Pos origin)>
                find_loop =
                        [&find_loop, &tiles](const Day10::Pos pos, const Day10::Tile tile, const Day10::Pos origin) {
                            std::cout << "scanning pos: " << pos.first << ", " << pos.second << "\n";

                            std::vector<Day10::Pos> next_pos{};
                            const auto& [y, x] = pos;
                            switch (tile) {
                                case Day10::Tile::StartPosition: {
                                    next_pos.emplace_back(y, x + 1);
                                    next_pos.emplace_back(y, x - 1);
                                    next_pos.emplace_back(y + 1, x);
                                    next_pos.emplace_back(y - 1, x);
                                    break;
                                }
                                case Day10::Tile::Vertical: {
                                    next_pos.emplace_back(y + 1, x);
                                    next_pos.emplace_back(y - 1, x);
                                    break;
                                }
                                case Day10::Tile::Horizontal: {
                                    next_pos.emplace_back(y, x + 1);
                                    next_pos.emplace_back(y, x - 1);
                                    break;
                                }
                                case Day10::Tile::NorthEast: {
                                    next_pos.emplace_back(y, x + 1);
                                    next_pos.emplace_back(y - 1, x);
                                    break;
                                }
                                case Day10::Tile::NorthWest: {
                                    next_pos.emplace_back(y, x - 1);
                                    next_pos.emplace_back(y - 1, x);
                                    break;
                                }
                                case Day10::Tile::SouthWest: {
                                    next_pos.emplace_back(y, x - 1);
                                    next_pos.emplace_back(y + 1, x);
                                    break;
                                }
                                case Day10::Tile::SouthEast: {
                                    next_pos.emplace_back(y, x + 1);
                                    next_pos.emplace_back(y + 1, x);
                                    break;
                                }


                                case Day10::Tile::Ground: {
                                    break;
                                }

                                default: {
                                    assert_unreachable("Not handled a tile: " + std::to_underlying(tile));
                                }
                            }


                            std::vector<RecursionType> rec_result{};

                            for (const auto& [y_p, x_p] : next_pos) {

                                if (y_p < 0 || x_p < 0) {
                                    continue;
                                }

                                if (y_p >= static_cast<ResultType>(tiles.size())) {
                                    continue;
                                }

                                if (x_p >= static_cast<ResultType>(tiles.at(y_p).size())) {
                                    continue;
                                }

                                const auto next_tile = tiles.at(y_p).at(x_p);
                                const Day10::Pos next_p = { y_p, x_p };

                                if (origin == next_p) {
                                    continue;
                                }


                                if (next_tile == Day10::Tile::StartPosition) {
                                    rec_result.emplace_back(next_tile, next_p);
                                    break;
                                }

                                const auto temp_res = find_loop(next_p, next_tile, pos);

                                if (temp_res.size() != 0) {
                                    if (temp_res.back().first == Day10::Tile::StartPosition) {
                                        rec_result.emplace_back(tile, pos);
                                        for (const auto& res : temp_res) {
                                            rec_result.push_back(res);
                                        }
                                        break;
                                    }
                                }
                            }

                            return rec_result;
                        };


        const auto loop = find_loop(startPos, Day10::Tile::StartPosition, startPos);

        for (const auto& t : loop) {
            std::cout << std::to_underlying(t.first) << " - " << t.second.first << ", " << t.second.second << "\n";
        }

        std::cout << loop.size() << "\n";


        return loop.size() / 2;
    }


    ResultType solvePart2(const std::string& input, [[maybe_unused]] const bool is_example) const override {
        ResultType result = 0;

        result += input.size();

        return result;
    }
};

DayRegister<AoCDay10> day10{ Input::SameInput("input.txt") >> Input::SameExample("example.txt", 8, -1) };
