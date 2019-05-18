#ifndef CONSTANTS
#define CONSTANTS

#include"types.hpp"

constexpr int KEEPER = 0;
constexpr double EXPECTED_MAX_SCORE = 100.0;
constexpr double EXPLORATION_CONSTANT = 1.0; // TODO: find value
constexpr double INF = 1000000000.0;
constexpr uint MIN_REQUESTS_IN_CHANNEL = 20;
constexpr uint MAX_NEW_REQUESTS_PER_ITERATION = 40;

#endif
