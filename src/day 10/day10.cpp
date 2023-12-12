

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

    enum class Direction { North, East, South, West };

    Pos dirToPos(const Pos& start, const Direction& dir) {
        const ResultType y_off = dir == Direction::North ? -1 : (dir == Direction::South ? 1 : 0);
        const ResultType x_off = dir == Direction::West ? -1 : (dir == Direction::East ? 1 : 0);

        return { start.first + y_off, start.second + x_off };
    }

    std::pair<Direction, Direction> get_directions(const std::vector<std::vector<Tile>>& tiles, const Pos& pos) {
        const auto& [y, x] = pos;
        const auto& tile = tiles.at(pos.first).at(pos.second);
        switch (tile) {
            case Tile::StartPosition: {
                std::vector<std::pair<Pos, Direction>> next_pos{};
                next_pos.push_back({
                        {y, x + 1},
                        Direction::East
                });
                next_pos.push_back({
                        {y, x - 1},
                        Direction::West
                });
                next_pos.push_back({
                        {y + 1, x},
                        Direction::South
                });
                next_pos.push_back({
                        {y - 1, x},
                        Direction::North
                });


                std::vector<Direction> result{};

                for (const auto& [next_p, dir_to_dest] : next_pos) {
                    const auto& [y_p, x_p] = next_p;

                    if (y_p < 0 || x_p < 0) {
                        continue;
                    }

                    if (y_p >= static_cast<ResultType>(tiles.size())) {
                        continue;
                    }

                    if (x_p >= static_cast<ResultType>(tiles.at(y_p).size())) {
                        continue;
                    }


                    if (const auto& tile = tiles.at(y_p).at(x_p); tile == Tile::Ground) {
                        continue;
                    }

                    const auto& [dir_o, dir_t] = get_directions(tiles, next_p);

                    const auto pos_o = dirToPos(next_p, dir_o);
                    const auto pos_t = dirToPos(next_p, dir_t);

                    if (pos == pos_o || pos == pos_t) {
                        result.push_back(dir_to_dest);
                    }
                }

                assert_equal<std::size_t>(result.size(), 2u, "Exactly two directions from the start have to be valid");

                return { result.at(0), result.at(1) };
            }
            case Tile::Vertical: {
                return { Direction::South, Direction::North };
            }
            case Tile::Horizontal: {
                return { Direction::East, Direction::West };
            }
            case Tile::NorthEast: {
                return { Direction::East, Direction::North };
            }
            case Tile::NorthWest: {
                return { Direction::West, Direction::North };
            }
            case Tile::SouthWest: {
                return { Direction::West, Direction::South };
            }
            case Tile::SouthEast: {
                return { Direction::East, Direction::South };
            }


            case Tile::Ground: {
                assert_unreachable("No Ground should be visited");
                break;
            }

            default: {
                assert_unreachable(std::format("Not handled a tile: {}", std::to_underlying(tile)));
            }
        }
    };

    Pos walk(const std::vector<std::vector<Tile>>& tiles, const Pos& pos, const Pos& origin) {

        const auto& [dir_o, dir_t] = get_directions(tiles, pos);

        const auto pos_o = dirToPos(pos, dir_o);
        const auto pos_t = dirToPos(pos, dir_t);

        assert_true(pos_o == origin || pos_t == origin, "Expected connected pipes!");

        return pos_o == origin ? pos_t : pos_o;
    }


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
                    assert_equal(startPos, { 0, 0 }, "Start has to occur only once!");
                    startPos = { tiles.size(), row.size() - 1 };
                }
            }
            tiles.push_back(row);
        }


        const std::pair<Day10::Direction, Day10::Direction> startDirections = Day10::get_directions(tiles, startPos);

        ResultType length = 0;
        Day10::Pos currentPos = startPos;
        Day10::Pos lastVisited = Day10::dirToPos(startPos, startDirections.first);
        while (true) {
            const Day10::Pos newPos = Day10::walk(tiles, currentPos, lastVisited);
            lastVisited = currentPos;
            currentPos = newPos;
            ++length;
            if (tiles.at(currentPos.first).at(currentPos.second) == Day10::Tile::StartPosition) {
                break;
            }
        };


        return length / 2;
    }


    ResultType solvePart2(const std::string& input, [[maybe_unused]] const bool is_example) const override {
        ResultType result = 0;

        result += static_cast<ResultType>(input.size());

        return result;
    }
};

DayRegister<AoCDay10> day10{ Input::SameInput("input.txt") >> Input::ExampleInput("example.txt", 8)
                             >> Input::ExampleInput("example_2.txt", 10) };
