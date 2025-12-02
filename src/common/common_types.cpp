#include "common_types.hpp"

MyTime::tick_t MyTime::ticks() const { return _ticks; }

real_number_t MyTime::toTradingYears() const {
  return static_cast<real_number_t>(_ticks) / TICKS_PER_TRADING_YEAR;
}

real_number_t MyTime::toCalendarYears() const {
  return static_cast<real_number_t>(_ticks) / TICKS_PER_FULL_YEAR;
}

void MyTime::setTo(tick_t new_ticks) { _ticks = new_ticks; }

void MyTime::setTo(const MyTime &other) { _ticks = other._ticks; }

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

real_number_t MyTime::operator/(const MyTime &other) const {
  return static_cast<real_number_t>(_ticks) /
         static_cast<real_number_t>(other._ticks);
}

MyTime MyTime::operator/(real_number_t denom) const {
  return MyTime{static_cast<tick_t>(_ticks / denom)};
}

MyTime &MyTime::operator/=(real_number_t denom) {
  _ticks /= denom;
  return *this;
}

std::strong_ordering MyTime::operator<=>(const MyTime &other) const {
  return _ticks <=> other._ticks;
}