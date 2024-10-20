
#define MIN_HEADER_EXPORT __declspec( dllexport )
#include "header_min.hpp"

#define T int
#include "header_min_ins.hpp"
#undef T

#define T double
#include "header_min_ins.hpp"
#undef T
