#include <cmath>
#include <format>

#include "black_scholes.hpp"

BlackScholesInputs::BlackScholesInputs() {
  *this = BlackScholesInputs::generateRandomInstance();
}

BlackScholesInputs::BlackScholesInputs(price_t asset_price,
                                       price_t strike_price, my_time_t maturity,
                                       real_number_t volatility,
                                       real_number_t risk_free_interest_rate)
    : _asset_price(asset_price), _strike_price(strike_price),
      _maturity(maturity), _volatility(volatility),
      _risk_free_interest_rate(risk_free_interest_rate) {}

std::string BlackScholesInputs::toStr() const {
  return std::format("Asset price : {},\tStrike price : {}\n"
                     "Risk-free interest rate : {} %,\tVolatility : {} %\n"
                     "Maturity : {} years",
                     _asset_price, _strike_price,
                     _risk_free_interest_rate * 100, _volatility * 100,
                     _maturity.toTradingYears());
}

price_t assessOptionPrice(const BlackScholesInputs &bs_inputs) {
  // Setup
  const auto &s = bs_inputs._asset_price;
  const auto &k = bs_inputs._strike_price;
  const auto &sigma = bs_inputs._volatility;
  const auto t = bs_inputs._maturity.toTradingYears();
  const auto &r = bs_inputs._risk_free_interest_rate;
  //
  // Actual calculations
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