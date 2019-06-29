#include"own_moves_sender.hpp"
#include"reasoner.hpp"
#include"types.hpp"
#include<sstream>
#include<iomanip>

own_moves_sender::own_moves_sender(int socket_descriptor)
  : socket_descriptor(socket_descriptor){}

namespace{
std::string create_header(const std::string& t){
    std::stringstream header;
    header<<std::setw(8)<<std::setfill(' ')<<t.size();
    return header.str()+"\0\0\0\0\0\0\0\0";
}

std::string move_to_string(const reasoner::move& m){
    std::stringstream result;
    for(uint i=0;i<m.mr.size();++i){
        if(i!=0)
            result<<' ';
        result<<m.mr[i].index<<' '<<m.mr[i].cell;
    }
    return result.str();
}
}

void own_moves_sender::write_all(const std::string& t){
    uint len = t.size();
    const char* str = t.data();
    while(len>0){
        int n = write(socket_descriptor, str, len);
        assert(n>=1); // why not?
        str += n;
        len -= n;
    }
}

void own_moves_sender::send_text(const std::string& t){
    std::string pdu = create_header(t) + t;
    write_all(t);
}


void own_moves_sender::send_move(const reasoner::move& m){
    send_text(move_to_string(m));
}
