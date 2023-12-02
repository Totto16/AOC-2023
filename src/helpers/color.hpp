

#pragma once

#include <cstdint>
#include <format>
#include <string>
#include <utility>

enum class ColorForeground : std::uint8_t {
  Default = 39,
  Black = 30,
  Red = 31,
  Green = 32,
  Yellow = 33,
  Blue = 34,
  Magenta = 35,
  Cyan = 36,
  Light_Gray = 37,
  Dark_Gray = 90,
  Light_Red = 91,
  Light_Green = 92,
  Light_Yellow = 93,
  Light_Blue = 94,
  Light_Magenta = 95,
  Light_Cyan = 96,
  White = 97
};

enum class ColorBackground : std::uint8_t {
  Default = 49,
  Black = 40,
  Red = 41,
  Green = 42,
  Yellow = 43,
  Blue = 44,
  Megenta = 45,
  Cyan = 46,
  Light_Gray = 47,
  Dark_Gray = 100,
  Light_Red = 101,
  Light_Green = 102,
  Light_Yellow = 103,
  Light_Blue = 104,
  Light_Magenta = 105,
  Light_Cya = 106,
  White = 107,
};

enum class Modifier : std::uint8_t {
  Default = 0,
  Bold = 1,
  Dim = 2,
  Underlined = 4,
  Blink = 5,
  Reverse = 7,
  Hidden = 8
};

enum class ResetModifier : std::uint8_t {
  All = 0,
  Bold = 21,
  Dim = 22,
  Underlined = 24,
  Blink = 25,
  Reverse = 27,
  Hidden = 28,
};

struct Color {

  static constexpr const char *control = "\033";

  static constexpr std::string reset(ResetModifier modifier) {

    const auto md = std::to_string(std::to_underlying(modifier));

    return std::format("{}[{}m", control, md);
  }

  static constexpr std::string reset() { return reset(ResetModifier::All); }

  static constexpr std::string color(ColorForeground foreground) {

    const auto fg = std::to_string(std::to_underlying(foreground));

    return std::format("{}[{}m", control, fg);
  }

  static constexpr std::string color(ColorForeground foreground,
                                     ColorBackground background) {

    const auto fg = std::to_string(std::to_underlying(foreground));

    const auto bg = std::to_string(std::to_underlying(background));

    return std::format("{}[{};{}m", control, bg, fg);
  }

  static constexpr std::string color(ColorForeground foreground,
                                     ColorBackground background,
                                     Modifier modifier) {

    const auto fg = std::to_string(std::to_underlying(foreground));

    const auto bg = std::to_string(std::to_underlying(background));

    const auto md = std::to_string(std::to_underlying(modifier));

    return std::format("{}[{};{};{}m", control, md, bg, fg);
  }

  static constexpr std::string color(ColorForeground foreground,
                                     Modifier modifier) {

    return color(foreground, ColorBackground::Default, modifier);
  }

  static constexpr std::string color(Modifier modifier) {

    return color(ColorForeground::Default, ColorBackground::Default, modifier);
  }
};
