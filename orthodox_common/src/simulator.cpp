#include"simulator.hpp"
#include"reasoner.hpp"
#include"simulation_result.hpp"
#include"constants.hpp"

namespace{
bool handle_keeper_move(reasoner::game_state& state, reasoner::resettable_bitarray_stack& cache){
    return state.apply_any_move(cache);
}

bool handle_player_move(reasoner::game_state& state,
                        reasoner::resettable_bitarray_stack& cache,
                        moves_container& mc,
                        std::mt19937& mt){
    mc.clear();
    state.get_all_moves(cache, mc);
    if(not mc.empty()){
        std::uniform_int_distribution<> random_distribution(0, mc.size()-1);
        state.apply_move(mc[random_distribution(mt)]);
        return true;
    }
    else
        return false;
}

bool handle_move(reasoner::game_state& state,
                 reasoner::resettable_bitarray_stack& cache,
                 moves_container& mc,
                 std::mt19937& mt){
    if(state.get_current_player() == KEEPER)
        return handle_keeper_move(state, cache);
    else
        return handle_player_move(state, cache, mc, mt);
}
}

simulation_result perform_simulation(reasoner::game_state& state,
                                     reasoner::resettable_bitarray_stack& cache,
                                     moves_container& mc,
                                     std::mt19937& mt){
    while(handle_move(state, cache, mc, mt));
    return simulation_result(state);
}

