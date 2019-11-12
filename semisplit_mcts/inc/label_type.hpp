#ifndef LABEL_TYPE
#define LABEL_TYPE

#include"reasoner.hpp"
#include<optional>

typedef reasoner::semimove label_type;

std::optional<uint> matches(const label_type& lhs, const reasoner::move& rhs, const uint from);

#endif
