#ifndef TYPES
#define TYPES

#include<tuple>

typedef unsigned int uint;
typedef std::vector<uint> node_address;
typedef std::tuple<double,uint> priority;
enum game_status_indication{
    own_turn,
    opponent_turn,
    end_game
};

#endif
