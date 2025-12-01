#pragma once

#include "../common/common_types.hpp"
#include "../misc.hpp"


struct BlackScholesInputs {
  BlackScholesInputs();
  BlackScholesInputs(price_t curr_price, price_t strike_price,
                     my_time_t maturity, real_number_t volatility,
                     real_number_t risk_free_interest_rate);

  static BlackScholesInputs
  generateRandomInstance(const BlackScholesInputs &hint,
                         real_number_t shift_mean = 0,
                         real_number_t shift_sigma = 1) {
    NormalSampler my_sampler{shift_mean, shift_sigma};
    auto asset_price_shift{my_sampler.sample()},
        strike_price_shift{my_sampler.sample()},
        maturity_shift{my_sampler.sample()},
        volatility_shift{my_sampler.sample()},
        interest_shift{my_sampler.sample()};
    return BlackScholesInputs{hint._asset_price * std::exp(asset_price_shift),
                              hint._asset_price * std::exp(strike_price_shift),
                              hint._maturity * std::exp(maturity_shift),
                              hint._volatility * std::exp(volatility_shift),
                              hint._risk_free_interest_rate *
                                  std::exp(interest_shift)};
  }
  /*
  Generates random inputs (but still keeps values within certain reasonable
  ranges ?)
  */
  static BlackScholesInputs generateRandomInstance() {
    price_t typical_price{100}, strike_price{100};
    my_time_t typical_maturity = {my_time_t::TICKS_PER_TRADING_YEAR};
    typical_maturity *= 1;
    real_number_t typical_volatility{0.20}, typical_interest{0.07};
    auto typical_inputs =
        BlackScholesInputs{typical_price, strike_price, typical_maturity,
                           typical_volatility, typical_interest};
    return BlackScholesInputs::generateRandomInstance(typical_inputs);
  }

  price_t _asset_price{};
  price_t _strike_price{};
  my_time_t _maturity{};
  real_number_t _volatility{};
  real_number_t _risk_free_interest_rate{};
};

price_t getOptionPrice(const BlackScholesInputs &bs_inputs);