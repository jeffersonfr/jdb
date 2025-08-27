#pragma once

#include <string>
#include <algorithm>

template<size_t N>
struct StringLiteral {
  constexpr StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); }

  constexpr std::string to_string() const { return std::string{value, N - 1}; }

  char value[N];
};
