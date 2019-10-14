#ifndef SIMULATION_REQUEST
#define SIMULATION_REQUEST

#include"reasoner.hpp"
#include"types.hpp"
#include"node_address.hpp"

struct simulation_request{
    reasoner::game_state state;
    node_address address;
    uint game_turn;
};

#endif
