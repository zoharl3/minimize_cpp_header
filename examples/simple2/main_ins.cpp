
#define MIN_HEADER_EXPORT __declspec( dllexport )
#include "header_min.hpp"

#define S double

#define T int
#define TYPE int
#include "header_min_ins.hpp"
#undef T
#undef TYPE

#define T double
#define TYPE double
#include "header_min_ins.hpp"
#undef T
#undef TYPE
