#include "misc.hpp"
#include "../common/common_types.hpp"
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
  const real_number_t annualized_time_step = time_step.toTradingYears();

  my_time_t curr_ts{0};
  price_t curr_price{initial_price};

  std::vector<my_time_t> timestamps{curr_ts};
  std::vector<price_t> prices{curr_price};
  timestamps.reserve(step_count + 1);
  prices.reserve(step_count + 1);

  for (std::size_t i{0}; i < step_count; i++) {
    curr_ts += time_step;
    curr_price *= std::exp((annualized_time_step) *
                               (drift - 0.5 * volatility * volatility) +
                           (volatility * std::sqrt(annualized_time_step) *
                            normal_sampler.sample()));
    // curr_price *=
    //     (1 + ((annualized_time_step) * (drift - 0.5 * volatility *
    //     volatility) +
    //           (volatility * std::sqrt(annualized_time_step) *
    //            normal_sampler.sample())));
    timestamps.push_back(curr_ts);
    prices.push_back(curr_price);
  }

  return std::make_pair(std::move(timestamps), std::move(prices));
}

std::pair<std::vector<my_time_t>, std::vector<price_t>>
simulateAssetPrices_eff(price_t initial_price_log, interest_t drift,
                        volatility_t volatility, my_time_t time_step,
                        std::size_t step_count, NormalSampler &normal_sampler) {
  const real_number_t annualized_time_step = time_step.toTradingYears();

  my_time_t curr_ts{0};
  price_t curr_price_log{initial_price_log};

  std::vector<my_time_t> timestamps{curr_ts};
  std::vector<price_t> prices{curr_price_log};
  timestamps.reserve(step_count + 1);
  prices.reserve(step_count + 1);

  for (std::size_t i{0}; i < step_count; i++) {
    curr_ts += time_step;
    curr_price_log +=
        (annualized_time_step) * (drift - 0.5 * volatility * volatility) +
        (volatility * std::sqrt(annualized_time_step) *
         normal_sampler.sample());
    // curr_price *=
    //     (1 + ((annualized_time_step) * (drift - 0.5 * volatility *
    //     volatility) +
    //           (volatility * std::sqrt(annualized_time_step) *
    //            normal_sampler.sample())));
    timestamps.push_back(curr_ts);
    prices.push_back(curr_price_log);
  }

  return std::make_pair(std::move(timestamps), std::move(prices));
}
