#include "day01.hpp"
#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> splitter(std::string in_pattern,
                                  std::string &content) {
  std::vector<std::string> split_content;

  std::regex pattern(in_pattern);
  copy(std::sregex_token_iterator(content.begin(), content.end(), pattern, -1),
       std::sregex_token_iterator(), back_inserter(split_content));
  return split_content;
}

std::string read(std::filesystem::path name) {

  if (!std::filesystem::exists(name)) {
    std::cerr << "File " << name << " doesn't exist!\n";
    std::exit(3);
  }

  std::ifstream myfile(name);

  if (!myfile.is_open()) { // always check whether the file is open
    std::cerr << "File " << name << " couldn't be opened!\n";
    std::exit(4);
  }

  std::stringstream strStream;
  strStream << myfile.rdbuf();
  return strStream.str();
}

void part2() {

  std::filesystem::path sample{"src/day 01/sample.txt"};

  std::string input = read(sample);

  const std::vector<std::string> mappings = {
      "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

  std::uint64_t result = 0;

  for (const auto &temp : splitter(R"(\n)", input)) {

    std::vector<std::uint8_t> numbers{};

    if (temp.empty() || temp == "\n") {
      continue;
    }

    for (std::size_t i = 0; i < temp.size(); ++i) {

      const char &c = temp.at(i);

      if (isdigit(c)) {
        numbers.push_back(c - '0');
      }

      for (std::uint8_t j = 0; j < mappings.size(); ++j) {
        const auto &mapping = mappings.at(j);
        if (temp.rfind(mapping, i) == i) {
          numbers.push_back(j + 1);
        }
      }
    }
    assert(numbers.size() >= 1 && "at least one numbers expected");
    result += numbers.at(0) * 10 + numbers.back();
  }

  std::cout << result << "\n";
}

void day01() {

  std::filesystem::path sample{"src/day 01/sample.txt"};

  std::string mystring = read(sample);

  std::uint64_t result = 0;

  for (const auto &temp : splitter(R"(\n)", mystring)) {

    std::vector<std::uint8_t> numbers{};

    if (temp.empty() || temp == "\n") {
      continue;
    }

    for (const auto &c : temp) {
      if (isdigit(c)) {
        numbers.push_back(c - '0');
      }
    }
    assert(numbers.size() >= 1 && "at least one numbers expected");
    result += numbers.at(0) * 10 + numbers.back();
  }

  std::cout << "part1: " << result << "\n";

  part2();
}
