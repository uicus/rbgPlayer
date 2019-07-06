#include"remote_moves_receiver.hpp"
#include"reasoner.hpp"
#include<cassert>
#include<string>
#include<sstream>
#include<vector>

remote_moves_receiver::remote_moves_receiver(int socket_descriptor)
  : socket_descriptor(socket_descriptor){}

namespace{
std::vector<std::string> split_string(const std::string& t, char delimiter){
    std::vector<std::string> result;
    std::string current_part;
    for(const auto el: t){
        if(el == delimiter){
            if(not current_part.empty()){
                result.push_back(std::move(current_part));
                current_part.clear();
            }
        }
        else
            current_part.push_back(el);
    }
    if(not current_part.empty())
        result.push_back(std::move(current_part));
    return result;

}

reasoner::move string_to_move(const std::string& t){
    auto split_text = split_string(t, ' ');
    reasoner::move result;
    assert(split_text.size() % 2 == 0);
    for(uint i=0;i<split_text.size();i+=2){
        uint index = std::stoi(split_text[i]);
        uint cell = std::stoi(split_text[i+1]);
        result.mr.emplace_back(index, cell);
    }
    return result;
}
}

std::string remote_moves_receiver::read_all(uint len){
    std::vector<char> buffer(len,'\0');
    char* str = &buffer[0];
    while (len>0){
        int n = read(socket_descriptor, str, len);
        assert(n>=1); // why not?
        len -= n;
        str += n;
    }
    return std::string(buffer.begin(), buffer.end());
}

uint remote_moves_receiver::read_length(void){
    std::stringstream length_text(read_all(5));
    uint result;
    length_text>>result;
    return result;
}

reasoner::move remote_moves_receiver::receive_move(void){
    uint length = read_length();
    auto sdu = read_all(length);
    sdu.pop_back();
    return string_to_move(sdu);
}
