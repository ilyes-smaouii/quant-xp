#pragma once

#include <compare>
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

constexpr real_number_t EPSILON{0.001};

struct MyTime {
public:
  using tick_t = std::uint64_t;
  static_assert(std::integral<tick_t>,
                "MyTime static_assert error : tick_t is not integral !");

  constexpr static tick_t TICKS_PER_SECONDS{1'000'000'000};
  constexpr static tick_t TICKS_PER_DAY{TICKS_PER_SECONDS * 86400};
  constexpr static tick_t TICKS_PER_FULL_YEAR{
      static_cast<tick_t>(TICKS_PER_DAY * 365.22)};
  constexpr static tick_t TICKS_PER_TRADING_YEAR{TICKS_PER_DAY *
                                                 TRADING_DAYS_PER_YEAR};

  constexpr MyTime() : _ticks(0) {};

  constexpr explicit MyTime(tick_t ticks) : _ticks(ticks) {}

  tick_t ticks() const;
  /*
  Convert time to trading years
  */
  real_number_t toTradingYears() const;

  /*
  Convert time to calendar years
  */
  real_number_t toCalendarYears() const;

  void setTo(tick_t new_ticks);
  void setTo(const MyTime &other);

  MyTime operator+(const MyTime &other) const;
  // MyTime operator+(const tick_t &other_ticks) const;
  MyTime &operator+=(const MyTime &other);
  template <typename T>
    requires std::is_arithmetic_v<T>
  MyTime operator*(T factor) const {
    return MyTime{static_cast<tick_t>(_ticks * factor)};
  }
  template <typename T>
    requires std::is_arithmetic_v<T>
  MyTime &operator*=(T factor) {
    _ticks = static_cast<tick_t>(_ticks * factor);
    return *this;
  }
  real_number_t operator/(const MyTime &other) const;
  MyTime operator/(real_number_t denom) const;
  MyTime &operator/=(real_number_t denom);
  std::strong_ordering operator<=>(const MyTime &other) const;

protected:
  // ATTRIBUTES
  tick_t _ticks;
};

using my_time_t = MyTime;