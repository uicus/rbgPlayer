#include"label_type.hpp"

std::optional<uint> matches(const label_type& lhs, const reasoner::move& rhs, const uint from){
    const auto& actions = lhs.get_actions()
    for(uint i=0;i<actions.size();++i)
        if(from+i >= rhs.mr.size() or actions[i] != rhs.mr[from+i])
            return std::nullopt;
    return from+actions.size();
}
