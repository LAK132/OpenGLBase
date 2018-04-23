#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <vector>
using std::vector;
#include <unordered_map>
using std::unordered_map;
#include <stdint.h>
#include <exception>
using std::exception;
#include <type_traits>
using std::remove_reference;
#include "laktype.hpp"

#ifndef LAKTRIE_H
#define LAKTRIE_H

typedef lakType (*lakOperator)(lakType ...);

struct lakNode
{
    lakOperator operation;
    shared_ptr<unordered_map<size_t, lakNode>> child;
};

#include "laktrie.inl"

#endif