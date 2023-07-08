#pragma once

#include <type_aliases.hpp>

#include <vector>

template<class T> struct ParentComponent_t
{
    std::vector<ECS::Handle_t<T>> leafs {  };
};
