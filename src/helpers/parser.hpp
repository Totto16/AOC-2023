#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wparentheses"
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-local-typedefs"
#pragma clang diagnostic ignored "-Wparentheses"
#endif

#include <lexy/action/parse.hpp> // lexy::parse
#include <lexy/callback.hpp>
#include <lexy/dsl.hpp> // lexy::dsl::*
#include <lexy/input/string_input.hpp>
#include <lexy_ext/report_error.hpp> // lexy_ext::report_error

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include <optional>
#include <string>


template<typename T>
struct Number {
    static constexpr auto rule = [] {
        auto digits = lexy::dsl::digits<lexyd::decimal>.no_leading_zero();
        auto normal_digit = lexy::dsl::integer<T>(digits);

        return normal_digit;
    }();

    static constexpr auto value = lexy::forward<T>;
};

template<typename T>
inline std::optional<T> get_number(const std::string& input) {
    lexy::string_input string_input{ input };
    auto result = lexy::parse<Number<T>>(string_input, lexy::noop);
    if (result.has_value()) {
        return result.value();
    }

    return std::nullopt;
}
