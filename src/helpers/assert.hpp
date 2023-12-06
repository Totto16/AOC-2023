

#pragma once

#define PPK_ASSERT_DEFAULT_LEVEL Fatal
#define PPK_ASSERT_EXCEPTION_MESSAGE_BUFFER_SIZE 0x4000

#include <ppk_assert.h>

#include "helpers/color.hpp"
#include <format>
#include <functional>
#include <optional>
#include <stdexcept>

// TODO: this doesn't work that goof atm, assertions show the function name of
// this file, fix that!

#if defined(_MSC_VER)
#define ALWAYS_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define ALWAYS_INLINE [[gnu::always_inline]] inline __attribute__((always_inline)) __attribute__((flatten))
#else
#define ALWAYS_INLINE inline
#endif

namespace {

    template<typename T>
    struct Operation {
        std::string representation;
        std::function<bool(const T& a, const T& b)> compare;
    };

    template<typename T>
    constexpr Operation<T> EqualOp() {
        return Operation<T>{ "==", std::equal_to<T>{} };
    }

    template<typename T>
    constexpr Operation<T> GreaterEqOp() {
        return Operation<T>{ ">=", std::greater_equal<T>{} };
    }

    ALWAYS_INLINE void assert_raw(const bool expr, const std::string& message) {

        std::string internal_message = std::format("{}UNREACHABLE{}", ForegroundColor::Red, Color::reset());
        if (!message.empty()) {
            internal_message +=
                    std::format("{}: {}{}{}", ForegroundColor::Red, ForegroundColor::Cyan, message, Color::reset());
        }

        PPK_ASSERT(expr, "%s", internal_message.c_str());
    }

    template<typename T>
    ALWAYS_INLINE void assert_op(const T& first, const T& second, const std::string& message, const Operation<T>& op) {

        std::string internal_message = std::format(
                "{}not satisfied: {}{}{} {}{} {}{}{}", ForegroundColor::Red,
                Color::color(ForegroundColor::Yellow, Modifier::Bold), first, Color::reset(), ForegroundColor::Red,
                op.representation, Color::color(ForegroundColor::Yellow, Modifier::Bold), second, Color::reset()
        );
        if (!message.empty()) {
            internal_message +=
                    std::format("{}: {}{}{}", ForegroundColor::Red, ForegroundColor::Cyan, message, Color::reset());
        }

        PPK_ASSERT(op.compare(first, second), "%s", internal_message.c_str());
    }
    template<typename T>
    ALWAYS_INLINE void assert_has_val(const std::optional<T>& option, const std::string& message = "") {

        std::string internal_message = std::format("{}optional has nop value{}", ForegroundColor::Red, Color::reset());
        if (!message.empty()) {
            internal_message +=
                    std::format("{}: {}{}{}", ForegroundColor::Red, ForegroundColor::Cyan, message, Color::reset());
        }

        PPK_ASSERT(option.has_value(), "%s", internal_message.c_str());
    }

} // namespace

template<typename T>
ALWAYS_INLINE void assert_equal(const T& first, const T& second, const std::string& message = "") {

    assert_op<T>(first, second, message, EqualOp<T>());
}

// TODO use concepts and std::is_enum!
template<typename T>
ALWAYS_INLINE void assert_equal_enum(const T& first, const T& second, const std::string& message = "") {

    using Type = std::underlying_type_t<T>;

    assert_op<Type>(std::to_underlying(first), std::to_underlying(second), message, EqualOp<Type>());
}

template<typename T>
ALWAYS_INLINE void assert_greater_eq(const T& first, const T& second, const std::string& message = "") {

    assert_op<T>(first, second, message, GreaterEqOp<T>());
}

template<typename T>
ALWAYS_INLINE void assert_has_value(const std::optional<T>& option, const std::string& message = "") {

    assert_has_val<T>(option, message);
}

ALWAYS_INLINE void assert_true(const bool assertion, const std::string& message = "") {
    assert_raw(assertion, message);
}

[[noreturn]] ALWAYS_INLINE void assert_unreachable(const std::string& message) {

    assert_raw(false, message);
    throw std::runtime_error("UNREACHABLE caught: (assertions are disabled): " + message);
}
