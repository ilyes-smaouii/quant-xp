#include <cmath>


#include "option_pricing.hpp"

BlackScholesInputs::BlackScholesInputs() { *this = BlackScholesInputs::generateRandomInstance(); }

BlackScholesInputs::BlackScholesInputs(price_t curr_price, price_t strike_price,
                                       my_time_t maturity,
                                       real_number_t volatility,
                                       real_number_t risk_free_interest_rate)
    : _asset_price(curr_price), _strike_price(strike_price),
      _maturity(maturity), _volatility(volatility),
      _risk_free_interest_rate(risk_free_interest_rate) {}

price_t getOptionPrice(const BlackScholesInputs &bs_inputs) {
  const auto &s = bs_inputs._asset_price;
  const auto &k = bs_inputs._strike_price;
  const auto &sigma = bs_inputs._volatility;
  const auto t = bs_inputs._maturity.yearly();
  const auto &r = bs_inputs._risk_free_interest_rate;
  const auto d1 = (std::log(s / k) + (r + (sigma * sigma / 2)) * t) /
                  (sigma * std::sqrt(t));
  // const auto d2 = (std::log(s / k) + (r - (sigma * sigma / 2)) * t) /
  //                 (sigma * std::sqrt(t));
  const auto d2 = d1 - sigma * std::sqrt(t); // quicker to compute
  const auto n_d1 = normalCdf(d1);
  const auto n_d2 = normalCdf(d2);
  //
  // S*N(d1) - K*N(d2)*exp(-rt)
  return (s * n_d1) - (k * std::exp((-r) * t) * n_d2);
}