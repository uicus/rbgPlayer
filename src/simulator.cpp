#include"simulator.hpp"
#include"reasoner.hpp"
#include"simulation_result.hpp"
#include"constants.hpp"

namespace{
bool handle_keeper_move(reasoner::game_state& state, reasoner::resettable_bitarray_stack& cache){
    return state.apply_any_move(cache);
}

bool handle_player_move(reasoner::game_state& state, reasoner::resettable_bitarray_stack& cache, std::mt19937& mt){
    auto legal_moves = state.get_all_moves(cache);
    if(not legal_moves.empty()){
        std::uniform_int_distribution<> random_distribution(0, legal_moves.size()-1);
        state.apply_move(legal_moves[random_distribution(mt)]);
        return true;
    }
    else
        return false;
}

bool handle_move(reasoner::game_state& state, reasoner::resettable_bitarray_stack& cache, std::mt19937& mt){
    if(state.get_current_player() == KEEPER)
        return handle_keeper_move(state, cache);
    else
        return handle_player_move(state, cache, mt);
}
}

simulation_result perform_simulation(reasoner::game_state& state,
                                     reasoner::resettable_bitarray_stack& cache,
                                     std::mt19937& mt){
    while(handle_move(state, cache, mt));
    return simulation_result(state);
}

