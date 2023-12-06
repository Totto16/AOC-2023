

#pragma once

#define PPK_ASSERT_DEFAULT_LEVEL Fatal
#define PPK_ASSERT_EXCEPTION_MESSAGE_BUFFER_SIZE 0x4000

#include <ppk_assert.h>

#include "helpers/color.hpp"
#include <format>
#include <functional>

namespace {

template <typename T> struct Operation {
  std::string representation;
  std::function<bool(const T &a, const T &b)> compare;
};

template <typename T> constexpr Operation<T> EqualOp() {
  return Operation<T>{"==", std::less<T>{}};
}

template <typename T> constexpr Operation<T> GreaterEqOp() {
  return Operation<T>{">=", std::greater_equal<T>{}};
}

template <typename T>
inline constexpr void assert_op(const T &first, const T &second,
                                const std::string &message,
                                const Operation<T> &op) {

  std::string internal_message =
      std::format("{}not satisfied: {}{}{} {}{} {}{}{}", ForegroundColor::Red,
                  Color::color(ForegroundColor::Yellow, Modifier::Bold), first,
                  Color::reset(), ForegroundColor::Red, op.representation,
                  Color::color(ForegroundColor::Yellow, Modifier::Bold), second,
                  Color::reset());
  if (!message.empty()) {
    internal_message +=
        std::format("{}: {}{}{}", ForegroundColor::Red, ForegroundColor::Cyan,
                    message, Color::reset());
  }

  PPK_ASSERT(first == second, "%s", internal_message.c_str());
}

} // namespace

template <typename T>
inline constexpr void assert_equal(const T &first, const T &second,
                                   const std::string &message = "") {

  assert_op<T>(first, second, message, EqualOp<T>());
}

template <typename T>
inline constexpr void assert_greater_eq(const T &first, const T &second,
                                        const std::string &message = "") {

  assert_op<T>(first, second, message, GreaterEqOp<T>());
}
