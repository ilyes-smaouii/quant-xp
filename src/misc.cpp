#include "misc.hpp"
#include <cmath>
#include <utility>
#include <vector>

price_t getNormal() { return {}; }

std::pair<std::vector<my_time_t>, std::vector<price_t>>
simulateAssetPrices(price_t initial_price, interest_t drift,
                    volatility_t volatility, my_time_t timestep,
                    std::size_t sample_size) {
  using tick_t = my_time_t::tick_t;
  constexpr tick_t TPS = my_time_t::TICKS_PER_SECONDS;
  price_t t_step = static_cast<price_t>(timestep.ticks()) / TPS;

  my_time_t curr_ts{0};
  price_t curr_price{initial_price};
  auto prices = std::vector<price_t>(sample_size);
  auto timestamps = std::vector<my_time_t>(sample_size);
  prices.push_back(curr_price);
  timestamps.push_back(curr_ts);

  for (std::size_t i{0}; i < sample_size; i++) {
    curr_ts += timestep;
    curr_price += std::exp((t_step) * (drift - 0.5 * volatility * volatility) +
                           (volatility * std::sqrt(t_step) * getNormal()));
    prices.push_back(curr_price);
  }

  return std::make_pair(timestamps, prices);
}
