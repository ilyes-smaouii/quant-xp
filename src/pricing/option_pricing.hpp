#pragma once

#include "../common/common_types.hpp"

struct BlackScholesInputs {
  BlackScholesInputs();
  BlackScholesInputs(price_t curr_price, price_t strike_price,
                     my_time_t maturity, default_number_t volatility,
                     default_number_t risk_free_interest_rate);

  /*
  Generates random inputs (but still keeps values within certain reasonable
  ranges ?)
   */
  static BlackScholesInputs generateRandomInputs();

  price_t _curr_price{};
  price_t _strike_price{};
  my_time_t _maturity{};
  default_number_t _volatility{};
  default_number_t _risk_free_interest_rate{};
};

price_t getOptionPrice(const BlackScholesInputs &bs_inputs);