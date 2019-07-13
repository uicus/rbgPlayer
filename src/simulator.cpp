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
                        std::vector<reasoner::move>& moves_container,
                        std::mt19937& mt){
    moves_container.clear();
    state.get_all_moves(cache, moves_container);
    if(not moves_container.empty()){
        std::uniform_int_distribution<> random_distribution(0, moves_container.size()-1);
        state.apply_move(moves_container[random_distribution(mt)]);
        return true;
    }
    else
        return false;
}

bool handle_move(reasoner::game_state& state,
                 reasoner::resettable_bitarray_stack& cache,
                 std::vector<reasoner::move>& moves_container,
                 std::mt19937& mt){
    if(state.get_current_player() == KEEPER)
        return handle_keeper_move(state, cache);
    else
        return handle_player_move(state, cache, moves_container, mt);
}
}

simulation_result perform_simulation(reasoner::game_state& state,
                                     reasoner::resettable_bitarray_stack& cache,
                                     std::vector<reasoner::move>& moves_container,
                                     std::mt19937& mt){
    while(handle_move(state, cache, moves_container, mt));
    return simulation_result(state);
}

