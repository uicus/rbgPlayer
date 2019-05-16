#ifndef SIMULATION_REQUEST
#define SIMULATION_REQUEST

#include"reasoner.hpp"
#include<types.hpp>

struct simulation_request{
    reasoner::game_state state;
    uint id;
};

#endif
