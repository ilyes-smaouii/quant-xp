#pragma once

#include <chrono>
#include <cstddef>
// #include <iostream>
#include <random>
#include <vector>

#include "common/common_types.hpp"

price_t sampleFromNormal();

class NormalSampler {
public:
  using num_t = double;
  using seed_t = std::chrono::system_clock::rep;

  NormalSampler();

  num_t sample();

protected:
  std::random_device _rd;
  std::mt19937_64 _generator;
  std::normal_distribution<num_t> _nd;
};

std::pair<std::vector<my_time_t>, std::vector<price_t>>
simulateAssetPrices(price_t initial_price, interest_t drift,
                    volatility_t volatility, my_time_t time_step,
                    std::size_t step_count, NormalSampler &normal_sampler);