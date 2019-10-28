#ifndef TREE_INDICATION
#define TREE_INDICATION

#include"simulation_response.hpp"
#include"reasoner.hpp"

#include<variant>

struct move_request{};
struct reset_tree{};

struct tree_indication{
    std::variant<simulation_response, reasoner::move, move_request, reset_tree> content;
};

#endif
