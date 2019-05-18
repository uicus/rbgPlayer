#ifndef SIMULATION_RESPONSE
#define SIMULATION_RESPONSE

#include"simulation_result.hpp"

struct simulation_response{
    simulation_result result;
    node_address address;
    uint game_turn;
};

#endif

