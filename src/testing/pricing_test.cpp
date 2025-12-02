#include "../pricing/black_scholes.hpp"
#include "../simulation/misc.hpp"
#include <cmath>
#include <cstddef>
#include <iostream>

int main(int argc, char *argv[]) {
  // Parameters - pt. 1
  constexpr std::size_t STEP_COUNT{5 * TRADING_DAYS_PER_YEAR * 4 / 100};
  // Parameters - pt. 2
  std::size_t inputs_sample_size{1}, simulations_per_input{1};
  if (argc > 2) {
    inputs_sample_size = std::stoi(argv[1]);
    simulations_per_input = std::stoi(argv[2]);
  }
  bool display_info_about_each_input{false};

  // setup
  // std::vector<BlackScholesInputs> inputs;
  // inputs.reserve(inputs_sample_size);
  NormalSampler norm_sampler{};

  constexpr auto TPY =
      my_time_t::TICKS_PER_TRADING_YEAR; // ticks per trading year
  constexpr auto TPD = my_time_t::TICKS_PER_SECONDS * 86400; // ticks per day
  constexpr auto TP4D =
      my_time_t::TICKS_PER_SECONDS * 21600; // ticks per 1/4 day (i.e. 6 hours)

  // Loop over random(-ish) inputs
  real_number_t total_log_error{0};
  for (std::size_t i{0}; i < inputs_sample_size; i++) {
    price_t total_pnl{0};
    // auto curr_input = BlackScholesInputs::generateRandomInstance();
    auto curr_input = BlackScholesInputs::generateRandomInstance(
        {100, 80, my_time_t{my_time_t::TICKS_PER_TRADING_YEAR * 5}, 0.20, 0.05},
        0, .1);
    // BlackScholesInputs curr_input{
    //     100, 80, my_time_t{my_time_t::TICKS_PER_TRADING_YEAR * 5}, 0.20,
    //     0.05};
    auto time_step = my_time_t{curr_input._maturity / STEP_COUNT};
    if (display_info_about_each_input) {
      std::cout << "Current input :\n"
                << curr_input.toStr() << "\n"
                << std::endl;
    }
    // inputs.push_back(curr_input);
    // Try several simulations for each input
    for (std::size_t j{0}; j < simulations_per_input; j++) {
      auto [timestamps, prices] = simulateAssetPrices(
          curr_input._asset_price, curr_input._risk_free_interest_rate,
          curr_input._volatility, time_step, STEP_COUNT * 1.2, norm_sampler);
      // auto [timestamps, prices] = simulateAssetPrices_eff(
      //     std::log(curr_input._asset_price),
      //     curr_input._risk_free_interest_rate, curr_input._volatility,
      //     time_step, STEP_COUNT * 1.2, norm_sampler);
      // compute PnL w/o including price of option
      std::size_t ts_idx{
          static_cast<std::size_t>(curr_input._maturity / time_step)};
      while (timestamps.at(ts_idx) < curr_input._maturity) {
        ts_idx++;
      }
      auto diff = prices.at(ts_idx) - curr_input._strike_price;
      // auto diff = std::exp(prices.at(ts_idx)) - curr_input._strike_price;
      if (diff > 0) {
        total_pnl += diff;
      }
    }
    auto avg_pnl = total_pnl / simulations_per_input;
    auto avg_pnl_discount =
        avg_pnl * std::exp(-curr_input._risk_free_interest_rate *
                           curr_input._maturity.toTradingYears());
    auto theoretical_price = assessOptionPrice(curr_input);
    auto theoretical_price_forward =
        theoretical_price * std::exp(curr_input._risk_free_interest_rate *
                                     curr_input._maturity.toTradingYears());
    if (theoretical_price > 0 && avg_pnl > 0) {
      total_log_error +=
          abs(std::log(avg_pnl_discount) - std::log(theoretical_price));
    }

    if (display_info_about_each_input) {
      std::cout << "Average PnL on current input (discounted) : "
                << avg_pnl_discount << std::endl;
      std::cout << "Theoretical price of option for current input : "
                << theoretical_price << std::endl;
      std::cout << "Average PnL on current input : " << avg_pnl << std::endl;
      std::cout << "Theoretical price of option for current input (forward) : "
                << theoretical_price_forward << std::endl;
    }
  }
  std::cout << "Total log error : " << total_log_error << std::endl;
  std::cout << "Average log error : " << (total_log_error / inputs_sample_size)
            << std::endl;
}