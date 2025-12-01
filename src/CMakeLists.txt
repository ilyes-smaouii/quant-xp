cmake_minimum_required(VERSION 3.10)
project(quant-xp LANGUAGES CXX)

# ------------------------------------------------------------
# Global configuration
# ------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Put binaries in bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ------------------------------------------------------------
# Sources
# ------------------------------------------------------------
set(SRC_COMMON
    common/common_types.cpp
)

set(SRC_MISC
    misc.cpp
)

set(SRC_PRICING
    pricing/option_pricing.cpp
)

add_library(quantxp_lib
    ${SRC_COMMON}
    ${SRC_MISC}
    ${SRC_PRICING}
)

# Correct include path (CMakeLists is already inside src/)
target_include_directories(quantxp_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ------------------------------------------------------------
# Tests
# ------------------------------------------------------------

add_executable(test_gnb
    testing/gnb_simulation_test.cpp
)
target_link_libraries(test_gnb PRIVATE quantxp_lib)

add_executable(test_pricing
    testing/pricing_test.cpp
)
target_link_libraries(test_pricing PRIVATE quantxp_lib)

# ------------------------------------------------------------
# Optional: Enable warnings
# ------------------------------------------------------------
if (MSVC)
    target_compile_options(quantxp_lib PRIVATE /W4)
else()
    target_compile_options(quantxp_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()