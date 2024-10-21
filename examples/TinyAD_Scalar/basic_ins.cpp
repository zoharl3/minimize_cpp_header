
#if defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
    #define MIN_HEADER_EXPORT __declspec( dllexport )
#else
    #define MIN_HEADER_EXPORT __attribute__((used))
#endif
#include "Scalar_min.hpp"

namespace TinyAD {

typedef double PassiveT;

constexpr int k = 2;
using PassiveT = double;
using T1 = double;
using T2 = double;
using ScalarT = double;
constexpr int rows = 2;
constexpr int cols = 2;

#define INST_FUNC fins1
#define with_hessian 1
#include "Scalar_min_ins.hpp"
#undef with_hessian
#undef INST_FUNC

} // namespace TinyAD

