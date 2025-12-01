#include "common_types.hpp"

MyTime::MyTime() : _ticks(0) {};

MyTime::MyTime(tick_t ticks) : _ticks(ticks) {}

MyTime::tick_t MyTime::ticks() const { return _ticks; }

real_number_t MyTime::yearly() const {
  return static_cast<real_number_t>(_ticks) / TICKS_PER_TRADING_YEAR;
}

MyTime MyTime::operator+(const MyTime &other) const {
  return MyTime{_ticks + other._ticks};
}

// MyTime MyTime::operator+(const tick_t &other_ticks) const {
//   return MyTime{_ticks + other_ticks};
// }

MyTime &MyTime::operator+=(const MyTime &other) {
  _ticks += other._ticks;
  return *this;
}
