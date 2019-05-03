#include"edge.hpp"
#include"node.hpp"

edge::edge(const reasoner::move& label, std::unique_ptr<node>&& target)
  : label(label)
  , target(std::move(target)){}
