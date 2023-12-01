

#include "utility.hpp"

std::vector<std::string> splitByRegex(std::string &content,
                                      std::regex pattern) {
  std::vector<std::string> split_content;
  copy(std::sregex_token_iterator(content.begin(), content.end(), pattern, -1),
       std::sregex_token_iterator(), back_inserter(split_content));
  return split_content;
}

std::vector<std::string> splitByRegex(std::string &content,
                                      std::string in_pattern) {
  std::regex pattern(in_pattern);
  return splitByRegex(content, pattern);
}

std::vector<std::string> splitByNewLine(std::string &content) {
  return splitByRegex(content, R"(\n)");
}
