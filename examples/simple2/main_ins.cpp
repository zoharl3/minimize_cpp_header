
#if defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
    #define MIN_HEADER_EXPORT __declspec( dllexport )
#else
    #define MIN_HEADER_EXPORT __attribute__((used))
#endif
#include "header_min.hpp"

#define S double

#define INST_FUNC fins1
#define T int
#define TYPE int
#include "header_min_ins.hpp"
#undef T
#undef TYPE
#undef INST_FUNC

#define INST_FUNC fins2
#define T double
#define TYPE double
#include "header_min_ins.hpp"
#undef T
#undef TYPE
#undef INST_FUNC
