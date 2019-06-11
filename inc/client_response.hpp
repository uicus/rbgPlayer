#ifndef CLIENT_RESPONSE
#define CLIENT_RESPONSE

#include"reasoner.hpp"
#include"types.hpp"

#include<variant>

struct client_response{
    std::variant<reasoner::move, game_status_indication> content;
};

#endif
