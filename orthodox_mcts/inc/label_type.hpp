#ifndef LABEL_TYPE
#define LABEL_TYPE

#include"reasoner.hpp"

typedef reasoner::move label_type;

bool matches(const label_type& lhs, const reasoner::move& rhs);

#endif
