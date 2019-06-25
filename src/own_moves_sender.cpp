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
}

void own_moves_sender::write_all(const char *str, uint len){
    int n;
    while(len>0){
        n = write(socket_descriptor, str, len);
        assert(n>=0); // why not?
        str += n;
        len -= n;
    }
}

void own_moves_sender::send_text(const std::string& t){
    std::string pdu = create_header(t) + t;
    write_all(pdu.c_str(), pdu.size());
}

void own_moves_sender::send_move(const reasoner::move& m){
    std::stringstream sdu;
    for(uint i=0;i<m.mr.size();++i)
    {
        if(i!=0)
            sdu<<' ';
        sdu<<m.mr[i].index<<' '<<m.mr[i].cell;
    }
    send_text(sdu.str());
}
