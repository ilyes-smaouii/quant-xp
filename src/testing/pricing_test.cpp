#include "../misc.hpp"
#include "../pricing/option_pricing.hpp"
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>

int main(int argc, char *argv[]) {
  // Parameters - pt. 1
  my_time_t time_step;
  constexpr std::size_t max_steps_per_asset{1 << 20};
  // Parameters - pt. 2
  std::size_t inputs_sample_size{1}, simulations_per_input{1};
  if (argc > 2) {
    inputs_sample_size = std::stoi(argv[1]);
    simulations_per_input = std::stoi(argv[2]);
  }

  // setup
  std::vector<BlackScholesInputs> inputs;
  inputs.reserve(inputs_sample_size);
  NormalSampler norm_sampler{};

  auto TPY = my_time_t::TICKS_PER_TRADING_YEAR;
  auto TPD = my_time_t::TICKS_PER_SECONDS * 86400;
  auto TP4D = my_time_t::TICKS_PER_SECONDS * 21600;

  // Loop over random(-ish) inputs
  for (std::size_t i{0}; i < inputs_sample_size; i++) {
    price_t total_pnl{0};
    inputs.emplace_back();
    auto &curr_input = inputs[i];
    std::size_t steps_per_asset{curr_input._maturity.ticks() / TP4D};
    if (steps_per_asset > max_steps_per_asset) {
      throw std::runtime_error("Error : maturity too large !");
    }
    // Try several simulations for each input
    for (std::size_t j{0}; j < simulations_per_input; j++) {
      auto timestamps_and_prices = simulateAssetPrices(
          curr_input._asset_price, curr_input._risk_free_interest_rate,
          curr_input._volatility, time_step, steps_per_asset, norm_sampler);
      auto &timestamps = timestamps_and_prices.first;
      auto &prices = timestamps_and_prices.second;
      // compute PnL w/o including price of option
      auto diff = prices[steps_per_asset - 1] - curr_input._strike_price;
      if (diff > 0) {
        total_pnl += diff;
      }
    }
    auto avg_pnl = total_pnl / simulations_per_input;
    auto theoretical_price = getOptionPrice(curr_input);
    std::cout << "Average PnL on current input : " << avg_pnl << std::endl;
    std::cout << "Theoretical price of option for current input : "
              << theoretical_price << std::endl;
  }
}