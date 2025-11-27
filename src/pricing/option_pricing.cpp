#include "option_pricing.hpp"

BlackScholesInputs::BlackScholesInputs() { *this = generateRandomInputs(); }

BlackScholesInputs::BlackScholesInputs(price_t curr_price, price_t strike_price,
                                       my_time_t maturity,
                                       default_number_t volatility,
                                       default_number_t risk_free_interest_rate)
    : _curr_price(curr_price), _strike_price(strike_price), _maturity(maturity),
      _volatility(volatility),
      _risk_free_interest_rate(risk_free_interest_rate) {}

static BlackScholesInputs generateInputs() {
  // - generate random current price
  // - generate strike_price not foo far from current price (within 1, 2, more
  // standard deviations ?)
  // - generate volatility(, within reasonable range ?)
  // - generate risk-free interest rate(, within reasonable range ?)
  // TO-DO : implement all this
}

price_t getOptionPrice(BlackScholesInputs &bs_inputs) {
  price_t final_price{};
  // S*N(d1) - K*N(d2)*exp(-rt)
  // TO-DO : implement and add parameters
  //
  return final_price;
}