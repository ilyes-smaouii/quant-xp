#pragma once

#include <chrono>
#include <cstddef>
// #include <iostream>
#include <random>
#include <utility>
#include <vector>

#include "../common/common_types.hpp"

// price_t sampleFromNormal();

real_number_t normalCdf(real_number_t x);

class NormalSampler {
public:
  using num_t = real_number_t;
  using seed_t = std::chrono::system_clock::rep;

  NormalSampler();
  NormalSampler(real_number_t mean, real_number_t sigma);

  num_t sample();

protected:
  // ATTRIBUTES
  std::random_device _rd;
  std::mt19937_64 _generator;
  std::normal_distribution<num_t> _nd;
};

template <std::size_t STEP_COUNT>
std::pair<std::array<my_time_t, STEP_COUNT + 1>,
          std::array<price_t, STEP_COUNT + 1>>
simulateAssetPrices(price_t initial_price, interest_t drift,
                    volatility_t volatility, my_time_t time_step,
                    NormalSampler &normal_sampler) {
  using tick_t = my_time_t::tick_t;
  constexpr tick_t TPS = my_time_t::TICKS_PER_SECONDS;
  constexpr tick_t TPY = TPS * 86400 * TRADING_DAYS_PER_YEAR;

  const real_number_t annualized_time_step = time_step.toTradingYears();

  my_time_t curr_ts{0};
  price_t curr_price{initial_price};

  std::array<my_time_t, STEP_COUNT + 1> timestamps({curr_ts});
  std::array<price_t, STEP_COUNT + 1> prices{curr_price};

  for (std::size_t i{1}; i < STEP_COUNT + 1; i++) {
    curr_ts += time_step;
    curr_price *= std::exp((annualized_time_step) *
                               (drift - 0.5 * volatility * volatility) +
                           (volatility * std::sqrt(annualized_time_step) *
                            normal_sampler.sample()));
    timestamps.at(i) = curr_ts;
    prices.at(i) = curr_price;
  }
  return std::make_pair(std::move(timestamps), std::move(prices));
}

std::pair<std::vector<my_time_t>, std::vector<price_t>>
simulateAssetPrices(price_t initial_price, interest_t drift,
                    volatility_t volatility, my_time_t time_step,
                    std::size_t step_count, NormalSampler &normal_sampler);

std::pair<std::vector<my_time_t>, std::vector<price_t>>
simulateAssetPrices_eff(price_t initial_price, interest_t drift,
                        volatility_t volatility, my_time_t time_step,
                        std::size_t step_count, NormalSampler &normal_sampler);