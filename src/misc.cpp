#include "misc.hpp"
#include "common/common_types.hpp"
#include <cmath>
#include <random>
#include <utility>
#include <vector>

real_number_t normalCdf(real_number_t x) {
  return (1 + std::erf(x / std::sqrt(2))) / 2;
}

NormalSampler::NormalSampler() : _rd(), _generator(_rd()), _nd(0, 1) {}

NormalSampler::NormalSampler(real_number_t mean, real_number_t sigma)
    : _rd(), _generator(_rd()), _nd(mean, sigma) {}

NormalSampler::num_t NormalSampler::sample() { return _nd(_generator); }

std::pair<std::vector<my_time_t>, std::vector<price_t>>
simulateAssetPrices(price_t initial_price, interest_t drift,
                    volatility_t volatility, my_time_t time_step,
                    std::size_t step_count, NormalSampler &normal_sampler) {
  using tick_t = my_time_t::tick_t;
  constexpr tick_t TPS = my_time_t::TICKS_PER_SECONDS;
  constexpr tick_t TPY = TPS * 86400 * TRADING_DAYS_PER_YEAR;

  const price_t annualized_time_step =
      static_cast<price_t>(time_step.ticks()) / TPY;

  my_time_t curr_ts{0};
  price_t curr_price{initial_price};

  std::vector<my_time_t> timestamps{curr_ts};
  std::vector<price_t> prices{curr_price};
  timestamps.reserve(step_count);
  prices.reserve(step_count);

  for (std::size_t i{1}; i < step_count; i++) {
    curr_ts += time_step;
    curr_price *= std::exp((annualized_time_step) *
                               (drift - 0.5 * volatility * volatility) +
                           (volatility * std::sqrt(annualized_time_step) *
                            normal_sampler.sample()));
    timestamps.push_back(curr_ts);
    prices.push_back(curr_price);
  }

  return std::make_pair(timestamps, prices);
}
