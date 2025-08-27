#pragma once

#include "database/DataClass.hpp"

#include <fmt/format.h>

inline std::string format_currency(Data const &value) {
  if (value.is_null()) {
    return "<null>";
  }

  return fmt::format("{:>10.2F}", value.get_decimal().value_or(NAN));
}

template<typename Clock>
std::string format_timestamp(std::chrono::time_point<Clock> timePoint) {
  char buffer[32];
  std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
  std::strftime(std::data(buffer), std::size(buffer), "%F %T.000",
  std::gmtime(&time));

  return buffer;
}
