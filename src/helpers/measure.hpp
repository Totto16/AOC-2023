#include <functional>
#include <string>

#pragma once

std::ostream &operator<<(std::ostream &os, const std::chrono::nanoseconds &dur);

template <typename T>
T measure(const std::string &name, std::function<T()> execute) {
  std::cout << "measuring '" << name << "':\n";

  auto start = std::chrono::steady_clock::now();

  const T result = execute();

  auto end = std::chrono::steady_clock::now();

  const std::chrono::nanoseconds duration = end - start;

  std::cout << "It took: " << duration << "\n";
  return result;
}
