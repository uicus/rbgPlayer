#include"label_type.hpp"

bool matches(const label_type& lhs, const reasoner::move& rhs){
    return lhs == rhs;
}
