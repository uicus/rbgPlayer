#ifndef OWN_MOVES_SENDER
#define OWN_MOVES_SENDER

namespace reasoner{
    class move;
}

class own_moves_sender{
        int socket_descriptor;
    public:
        own_moves_sender(void)=delete;
        own_moves_sender(const own_moves_sender&)=delete;
        own_moves_sender& operator=(const own_moves_sender&)=delete;
        own_moves_sender(own_moves_sender&&)=default;
        own_moves_sender& operator=(own_moves_sender&&)=default;
        own_moves_sender(int socket_descriptor);
        void send_move(const reasoner::move& m);
};

#endif
