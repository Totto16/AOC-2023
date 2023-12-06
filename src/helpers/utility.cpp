

#include "utility.hpp"

std::vector<std::string> splitByRegex(const std::string& content, const std::regex& pattern) {
    std::vector<std::string> split_content;
    copy(std::sregex_token_iterator(content.begin(), content.end(), pattern, -1), std::sregex_token_iterator(),
         back_inserter(split_content));
    return split_content;
}

std::vector<std::string> splitByRegex(const std::string& content, const std::string& in_pattern) {
    const std::regex pattern(in_pattern);
    return splitByRegex(content, pattern);
}

std::vector<std::string> splitByNewLine(const std::string& content) {
    return splitByRegex(content, R"(\n)");
}

// from: https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring

std::string& rtrim(std::string& s, const char* what) {
    s.erase(s.find_last_not_of(what) + 1);
    return s;
}

std::string& ltrim(std::string& s, const char* what) {
    s.erase(0, s.find_first_not_of(what));
    return s;
}

std::string& trim(std::string& s, const char* what) {
    return ltrim(rtrim(s, what), what);
}
