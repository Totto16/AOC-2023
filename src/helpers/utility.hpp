

#pragma once

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

std::vector<std::string> splitByRegex(std::string &content,
                                      std::string in_pattern);

std::vector<std::string> splitByRegex(std::string &content, std::regex pattern);

std::vector<std::string> splitByNewLine(std::string &content);
