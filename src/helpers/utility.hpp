

#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

[[nodiscard]] std::vector<std::string>
splitByRegex(const std::string &content, const std::string in_pattern);

[[nodiscard]] std::vector<std::string> splitByRegex(const std::string &content,
                                                    const std::regex pattern);

[[nodiscard]] std::vector<std::string>
splitByNewLine(const std::string &content);

static const char *whitespace = " \t\n\r\f\v";

// trim from end of string (right)
std::string &rtrim(std::string &s, const char *what = whitespace);

// trim from beginning of string (left)
std::string &ltrim(std::string &s, const char *what = whitespace);

// trim from both ends of string (right then left)
std::string &trim(std::string &s, const char *what = whitespace);
