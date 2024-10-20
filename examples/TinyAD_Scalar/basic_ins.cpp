
#define MIN_HEADER_EXPORT __declspec( dllexport )
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

#define with_hessian 1
#include "Scalar_min_ins.hpp"
#undef with_hessian

} // namespace TinyAD

