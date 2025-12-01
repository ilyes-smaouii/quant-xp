#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>

// Assumptions/rules for the price :
// price is always > 0
// ?
struct Price {
  // Now, the question is, how do we store the price ?
};

using real_number_t = double;

// using price_t = Price;
using price_t = real_number_t;
using interest_t = real_number_t;
using volatility_t = real_number_t;

constexpr std::size_t TRADING_DAYS_PER_YEAR = 252;

struct MyTime {
  using tick_t = std::uint64_t;
  static_assert(std::integral<tick_t>,
                "MyTime static_assert error : tick_t is not integral !");

  tick_t _ticks;
  constexpr static tick_t TICKS_PER_SECONDS{1'000'000'000};
  constexpr static tick_t TICKS_PER_TRADING_YEAR{TICKS_PER_SECONDS * 86400 *
                                                 TRADING_DAYS_PER_YEAR};

  MyTime();
  MyTime(tick_t ticks);

  tick_t ticks() const;
  real_number_t yearly() const;

  MyTime operator+(const MyTime &other) const;
  // MyTime operator+(const tick_t &other_ticks) const;
  MyTime &operator+=(const MyTime &other);
  template <typename T>
    requires std::is_arithmetic_v<T>
  MyTime operator*(T factor) const {
    return {static_cast<tick_t>(_ticks * factor)};
  }
  template <typename T>
    requires std::is_arithmetic_v<T>
  MyTime &operator*=(T factor) {
    _ticks = static_cast<tick_t>(_ticks * factor);
    return *this;
  }
};

using my_time_t = MyTime;