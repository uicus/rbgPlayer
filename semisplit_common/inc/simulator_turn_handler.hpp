#ifndef SIMULATOR_TURN_HANDLER
#define SIMULATOR_TURN_HANDLER

#include<random>

#include"moves_container.hpp"

namespace reasoner{
    class game_state;
    class move;
    class resettable_bitarray_stack;
}

bool handle_move(reasoner::game_state& state,
                 reasoner::resettable_bitarray_stack& cache,
                 moves_container& legal_semimoves,
                 std::mt19937& mt);

#endif
