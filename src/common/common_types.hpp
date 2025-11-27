#include <concepts>
#include <cstdint>

// Assumptions/rules for the price :
// price is always > 0
// ?
struct Price {
  // Now, the question is, how do we store the price ?
};

struct MyTime {
  using tick_t = std::uint64_t;

  tick_t _ticks;
  constexpr static tick_t TICKS_PER_SECONDS{1'000'000'000};

  MyTime() : _ticks(0) {};
  MyTime(tick_t ticks) : _ticks(ticks) {}

  tick_t ticks() const { return _ticks; }

  MyTime operator+(const MyTime &other) const {
    return MyTime{_ticks + other._ticks};
  }

  MyTime operator+(const tick_t &other_ticks) const {
    return MyTime{_ticks + other_ticks};
  }

  void operator+=(const MyTime &other) { _ticks += other._ticks; }
};

using default_number_t = double;

// using price_t = Price;
using price_t = double;
using interest_t = double;
using volatility_t = double;
using my_time_t = MyTime;