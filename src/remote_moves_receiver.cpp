#include"remote_moves_receiver.hpp"
#include"reasoner.hpp"
#include<cassert>

remote_moves_receiver::remote_moves_receiver(int socket_descriptor)
  : socket_descriptor(socket_descriptor){}

void remote_moves_receiver::fill_buffer(void){
    buffer.resize(SOCKET_BUFFER_SIZE);
    uint bytes = read(socket_descriptor, &buffer[0], SOCKET_BUFFER_SIZE);
    assert(bytes>0); // can server close the connection?
    buffer.resize(bytes);
    next_data_index = 0;
}

reasoner::move remote_moves_receiver::receive_move(void){
    // TODO: implement when the interface is released
    return {};
}
