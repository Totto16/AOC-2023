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
        auto number = lexy::dsl::integer<T>(digits);

        return number;
    }();

    static constexpr auto value = lexy::forward<T>;
};

template<typename T>
concept Signed = std::is_signed_v<T>;

template<Signed T>
struct NegativNumber {
    static constexpr auto rule = [] {
        auto digits = lexy::dsl::digits<lexyd::decimal>.no_leading_zero();
        auto negative_number = lexy::dsl::minus_sign + lexy::dsl::integer<T>(digits);

        return negative_number;
    }();

    //TODO: this doesn't cover one value, MIN -> -(MAX) != MIN, since MIN = -(MAX)-1
    static constexpr auto value =
            lexy::callback<T>([](const lexy::minus_sign, const T& s) { return -s; }, [](const T& s) { return s; });
};


template<typename T>
concept production = lexy::is_production<T>;


template<production T, typename S>
inline std::optional<S> parse(const std::string& input) {
    lexy::string_input string_input{ input };
    const lexy::parse_result<S, lexy::_noop> result = lexy::parse<T>(string_input, lexy::noop);
    if (result.has_value()) {
        return result.value();
    }

    return std::nullopt;
}


template<typename T>
inline std::optional<T> get_number(const std::string& input) {

    if constexpr (std::is_signed_v<T>) {
        return parse<NegativNumber<T>, T>(input);
    } else {
        return parse<Number<T>, T>(input);
    }
}
