#include <cstddef>
#include <iostream>
#include <vector>

#include "common/common_types.hpp"

price_t getNormal();

std::pair<std::vector<my_time_t>, std::vector<price_t>>
simulateAssetPrices(price_t initial_price, interest_t drift,
                    volatility_t volatility, my_time_t timestep,
                    std::size_t sample_size);