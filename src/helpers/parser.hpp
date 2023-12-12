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

#include <limits>
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


// This is hack atm xD, but it wasn't that easily possible by using std::numeric_limits or other generic types, since constexpr conversion to string from a number isn't that easy, there is a method : https://stackoverflow.com/questions/23999573/convert-a-number-to-a-string-literal-with-constexpr but it doesn't cover the edge case -MAX -1 !
namespace conversion {


    template<Signed T>
    constexpr auto lit_rule() {
        static_assert(false, "not specialized for this type");
        return LEXY_LIT("ERROR");
    }

    template<>
    constexpr auto lit_rule<std::int64_t>() {
        //LLONG_MIN | LONG_MIN
        return LEXY_LIT("-9223372036854775808");
    }

    template<>
    constexpr auto lit_rule<std::int32_t>() {
        //INT_MIN
        return LEXY_LIT("-2147483648");
    }

    template<>
    constexpr auto lit_rule<std::int16_t>() {
        // SHRT_MIN
        return LEXY_LIT("-32768");
    }

    template<>
    constexpr auto lit_rule<std::int8_t>() {
        //SCHAR_MIN
        return LEXY_LIT("-128");
    }


} // namespace conversion

template<Signed T>
struct MinNumber {
    static constexpr auto rule = [] { return conversion::lit_rule<T>(); }();

    static constexpr auto value = lexy::callback<T>([]() { return std::numeric_limits<T>::min(); });
};


template<Signed T>
struct NegativeNumber {
    static constexpr auto rule = [] {
        auto digits = lexy::dsl::digits<lexyd::decimal>.no_leading_zero();
        auto number = lexy::dsl::integer<T>(digits);
        auto negative_number =
                (

                        lexy::dsl::peek(lexy::dsl::minus_sign)
                        >> (lexy::dsl::p<MinNumber<T>> | (lexy::dsl::else_ >> (lexy::dsl::minus_sign + number)))
                )

                | number;

        return negative_number;
    }();

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
        const auto res = parse<NegativeNumber<T>, T>(input);
        // tackle an edge case, where lexy parses the number, gets an overflow, but returns a number, that isnÄ't correct!
        if (!res.has_value()) {
            return res;
        }

        if (std::to_string(res.value()) != input) {
            return std::nullopt;
        }
        return res;
    } else {
        return parse<Number<T>, T>(input);
    }
}
