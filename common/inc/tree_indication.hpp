#ifndef TREE_INDICATION
#define TREE_INDICATION

#include"simulation_response.hpp"
#include"reasoner.hpp"

#include<variant>

struct move_request{};
struct status_request{};

struct tree_indication{
    std::variant<simulation_response, reasoner::move, move_request, status_request> content;
};

#endif
