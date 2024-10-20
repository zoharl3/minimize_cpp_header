
typedef bool _Bool;

/*
 * This file is part of TinyAD and released under the MIT license.
 * Author: Patrick Schmidt
 */
#pragma once

#include <cmath>
#include <Eigen/Dense>

#include <TinyAD/Utils/Out.hh>
#include <TinyAD/Utils/ToPassive.hh>

namespace TinyAD
{

/**
  * Forward-differentiable scalar type with constructors for passive and active variables.
  * Each scalar carries its gradient and Hessian w.r.t. a variable vector.
  *     k: Size of variable vector at compile time. (Eigen::Dynamic is possible, but experimental and slow.)
  *     PassiveT: Internal floating point type, e.g. double.
  *     with_hessian: Set to false for gradient-only mode.
  */
template <int k, typename PassiveT, bool with_hessian = true>
struct Scalar
{
    // Make template arguments available as members
    static constexpr int k_ = k;
    static constexpr bool with_hessian_ = with_hessian;
    static constexpr bool dynamic_mode_ = k == Eigen::Dynamic;

    // Determine derivative data types at compile time. Use 0-by-0 if no Hessian required.
    using GradType = Eigen::Matrix<PassiveT, k, 1>;
    using HessType = typename std::conditional_t<
                with_hessian,
                Eigen::Matrix<PassiveT, k, k>,
                Eigen::Matrix<PassiveT, 0, 0>>;

    // ///////////////////////////////////////////////////////////////////////////
    // TinyAD::Scalar constructors
    // ///////////////////////////////////////////////////////////////////////////

    /// Default constructor, copy, move, assignment
    Scalar() = default;
    Scalar(const Scalar& _rhs) = default;
    Scalar(Scalar&& _rhs) = default;
    Scalar& operator=(const Scalar& _rhs) = default;
    Scalar& operator=(Scalar&& _rhs) = default;

    /// Passive variable a.k.a. constant.
    /// Gradient and Hessian are zero.
    Scalar( PassiveT _val );

    /// Active variable.
    ///     _idx: index in variable vector
    Scalar( PassiveT _val, Eigen::Index _idx );

    /// Initialize scalar with known derivatives
    static Scalar<k, PassiveT, with_hessian> known_derivatives( PassiveT _val, const GradType & _grad, const HessType & _Hess);

    /// Initialize scalar with known derivatives (univariate case)
    static Scalar<k, PassiveT, with_hessian> known_derivatives( PassiveT _val, PassiveT _grad, PassiveT _Hess);

    /// Initialize passive variable a.k.a. constant with zero derivatives of size _k_dynamic.
    /// Only necessary in dynamic mode to pass derivative size at run time.
    /// In static mode, use the Scalar(val) constructor instead.
    static Scalar<k, PassiveT, with_hessian> make_passive( PassiveT _val, Eigen::Index _k_dynamic);

    /// Initialize active variable with derivatives of size _k_dynamic.
    /// Only necessary in dynamic mode to pass derivative size at run time.
    /// In static mode, use the Scalar(val, idx) constructor instead.
    static Scalar<k, PassiveT, with_hessian> make_active( PassiveT _val, Eigen::Index _idx, Eigen::Index _k_dynamic);

    /// Initialize an active variable vector of size k from given values.
    static Eigen::Matrix<Scalar<k, PassiveT, with_hessian>, k, 1> make_active( const Eigen::Matrix<PassiveT, Eigen::Dynamic, 1> & _passive);

    /// Initialize an active variable vector of size k from given values.
    static Eigen::Matrix<Scalar<k, PassiveT, with_hessian>, k, 1> make_active( std::initializer_list<PassiveT> _passive);

    // ///////////////////////////////////////////////////////////////////////////
    // Unary operators
    // ///////////////////////////////////////////////////////////////////////////

    /// Apply chain rule to compute f(a(x)) and its derivatives.
    static Scalar<k, PassiveT, with_hessian> chain( const PassiveT & val, const PassiveT & grad, const PassiveT & Hess, const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> operator-( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> sqrt( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> sqr( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> pow( const Scalar<k, PassiveT, with_hessian> & a, const int & e);

    friend Scalar<k, PassiveT, with_hessian> pow( const Scalar<k, PassiveT, with_hessian> & a, const PassiveT & e);

    friend Scalar<k, PassiveT, with_hessian> fabs( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> abs( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> exp( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> log( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> log2( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> log10( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> sin( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> cos( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> tan( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> asin( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> acos( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> atan( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> sinh( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> cosh( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> tanh( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> asinh( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> acosh( const Scalar<k, PassiveT, with_hessian> & a);

    friend Scalar<k, PassiveT, with_hessian> atanh( const Scalar<k, PassiveT, with_hessian> & a);

    friend _Bool isnan( const Scalar<k, PassiveT, with_hessian> & a);

    friend _Bool isinf( const Scalar<k, PassiveT, with_hessian> & a);

    friend _Bool isfinite( const Scalar<k, PassiveT, with_hessian> & a);

    // ///////////////////////////////////////////////////////////////////////////
    // Binary operators
    // ///////////////////////////////////////////////////////////////////////////

    friend Scalar<k, PassiveT, with_hessian> operator+( const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend Scalar<k, PassiveT, with_hessian> operator+( const Scalar<k, PassiveT, with_hessian> & a, const PassiveT & b);

    friend Scalar<k, PassiveT, with_hessian> operator+( const PassiveT & a, const Scalar<k, PassiveT, with_hessian> & b);

    Scalar<k, PassiveT, with_hessian> & operator+=( const Scalar<k, PassiveT, with_hessian> & b);

    Scalar<k, PassiveT, with_hessian> & operator+=( const PassiveT & b);

    friend Scalar<k, PassiveT, with_hessian> operator-( const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend Scalar<k, PassiveT, with_hessian> operator-( const Scalar<k, PassiveT, with_hessian> & a, const PassiveT & b);

    friend Scalar<k, PassiveT, with_hessian> operator-( const PassiveT & a, const Scalar<k, PassiveT, with_hessian> & b);

    Scalar<k, PassiveT, with_hessian> & operator-=( const Scalar<k, PassiveT, with_hessian> & b);

    Scalar<k, PassiveT, with_hessian> & operator-=( const PassiveT & b);

    friend Scalar<k, PassiveT, with_hessian> operator*( const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend Scalar<k, PassiveT, with_hessian> operator*( const Scalar<k, PassiveT, with_hessian> & a, const PassiveT & b);

    friend Scalar<k, PassiveT, with_hessian> operator*( const PassiveT & a, const Scalar<k, PassiveT, with_hessian> & b);

    Scalar<k, PassiveT, with_hessian> & operator*=( const Scalar<k, PassiveT, with_hessian> & b);

    Scalar<k, PassiveT, with_hessian> & operator*=( const PassiveT & b);

    friend Scalar<k, PassiveT, with_hessian> operator/( const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend Scalar<k, PassiveT, with_hessian> operator/( const Scalar<k, PassiveT, with_hessian> & a, const PassiveT & b);

    friend Scalar<k, PassiveT, with_hessian> operator/( const PassiveT & a, const Scalar<k, PassiveT, with_hessian> & b);

    Scalar<k, PassiveT, with_hessian> & operator/=( const Scalar<k, PassiveT, with_hessian> & b);

    Scalar<k, PassiveT, with_hessian> & operator/=( const PassiveT & b);

    friend Scalar<k, PassiveT, with_hessian> atan2( const Scalar<k, PassiveT, with_hessian> & y, const Scalar<k, PassiveT, with_hessian> & x);

    friend Scalar<k, PassiveT, with_hessian> hypot( const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);
    
    friend _Bool operator==( const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend _Bool operator==( const Scalar<k, PassiveT, with_hessian> & a, const PassiveT & b);

    friend _Bool operator==( const PassiveT & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend _Bool operator!=( const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend _Bool operator!=( const Scalar<k, PassiveT, with_hessian> & a, const PassiveT & b);

    friend _Bool operator!=( const PassiveT & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend _Bool operator<( const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend _Bool operator<( const Scalar<k, PassiveT, with_hessian> & a, const PassiveT & b);

    friend _Bool operator<( const PassiveT & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend _Bool operator<=( const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend _Bool operator<=( const Scalar<k, PassiveT, with_hessian> & a, const PassiveT & b);

    friend _Bool operator<=( const PassiveT & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend _Bool operator>( const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend _Bool operator>( const Scalar<k, PassiveT, with_hessian> & a, const PassiveT & b);

    friend _Bool operator>( const PassiveT & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend _Bool operator>=( const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend _Bool operator>=( const Scalar<k, PassiveT, with_hessian> & a, const PassiveT & b);

    friend _Bool operator>=( const PassiveT & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend Scalar<k, PassiveT, with_hessian> min( const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend Scalar<k, PassiveT, with_hessian> fmin( const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend Scalar<k, PassiveT, with_hessian> max( const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend Scalar<k, PassiveT, with_hessian> fmax( const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);

    friend Scalar<k, PassiveT, with_hessian> clamp( const Scalar<k, PassiveT, with_hessian> & x, const Scalar<k, PassiveT, with_hessian> & a, const Scalar<k, PassiveT, with_hessian> & b);

    // ///////////////////////////////////////////////////////////////////////////
    // std::complex operators (just spell out and differentiate the real case)
    // ///////////////////////////////////////////////////////////////////////////

    friend std::complex<Scalar<k, PassiveT, with_hessian> > operator+( const std::complex<Scalar<k, PassiveT, with_hessian> > & a, const std::complex<Scalar<k, PassiveT, with_hessian> > & b);

    friend std::complex<Scalar<k, PassiveT, with_hessian> > operator+( const std::complex<PassiveT> & a, const std::complex<Scalar<k, PassiveT, with_hessian> > & b);

    friend std::complex<Scalar<k, PassiveT, with_hessian> > operator+( const std::complex<Scalar<k, PassiveT, with_hessian> > & a, const std::complex<PassiveT> & b);

    friend std::complex<Scalar<k, PassiveT, with_hessian> > operator-( const std::complex<Scalar<k, PassiveT, with_hessian> > & a, const std::complex<Scalar<k, PassiveT, with_hessian> > & b);

    friend std::complex<Scalar<k, PassiveT, with_hessian> > operator-( const std::complex<PassiveT> & a, const std::complex<Scalar<k, PassiveT, with_hessian> > & b);

    friend std::complex<Scalar<k, PassiveT, with_hessian> > operator-( const std::complex<Scalar<k, PassiveT, with_hessian> > & a, const std::complex<PassiveT> & b);

    friend std::complex<Scalar<k, PassiveT, with_hessian> > operator*( const std::complex<Scalar<k, PassiveT, with_hessian> > & a, const std::complex<Scalar<k, PassiveT, with_hessian> > & b);

    friend std::complex<Scalar<k, PassiveT, with_hessian> > operator*( const std::complex<PassiveT> & a, const std::complex<Scalar<k, PassiveT, with_hessian> > & b);

    friend std::complex<Scalar<k, PassiveT, with_hessian> > operator*( const std::complex<Scalar<k, PassiveT, with_hessian> > & a, const std::complex<PassiveT> & b);

    friend std::complex<Scalar<k, PassiveT, with_hessian> > sqr( const std::complex<Scalar<k, PassiveT, with_hessian> > & a);

    friend std::complex<Scalar<k, PassiveT, with_hessian> > operator/( const std::complex<Scalar<k, PassiveT, with_hessian> > & a, const std::complex<Scalar<k, PassiveT, with_hessian> > & b);

    friend std::complex<Scalar<k, PassiveT, with_hessian> > operator/( const std::complex<Scalar<k, PassiveT, with_hessian> > & a, const std::complex<PassiveT> & b);

    friend std::complex<Scalar<k, PassiveT, with_hessian> > conj( const std::complex<Scalar<k, PassiveT, with_hessian> > & a);

    friend Scalar<k, PassiveT, with_hessian> abs( const std::complex<Scalar<k, PassiveT, with_hessian> > & a);

    friend Scalar<k, PassiveT, with_hessian> arg( const std::complex<Scalar<k, PassiveT, with_hessian> > & a);

    // ///////////////////////////////////////////////////////////////////////////
    // Stream Operators
    // ///////////////////////////////////////////////////////////////////////////

    friend std::ostream & operator<<( std::ostream & s, const Scalar<k, PassiveT, with_hessian> & a);

    // ///////////////////////////////////////////////////////////////////////////
    // Data
    // ///////////////////////////////////////////////////////////////////////////

    PassiveT val = 0.0;                // Scalar value of this (intermediate) variable.
    GradType grad = GradType::Zero(    // Gradient (first derivative) of val w.r.t. the active variable vector.
                dynamic_mode_ ? 0 : k);
    HessType Hess = HessType::Zero(    // Hessian (second derivative) of val w.r.t. the active variable vector.
                dynamic_mode_ ? 0 : (with_hessian ? k : 0),
                dynamic_mode_ ? 0 : (with_hessian ? k : 0));
}; // struct Scalar

// ///////////////////////////////////////////////////////////////////////////
// Overloads (Fails to build on windows otherwise)
// ///////////////////////////////////////////////////////////////////////////
template  <typename T1, typename T2>
T1 pow( const T1 & a, const T2 & e);

template <typename PassiveT>
PassiveT atan2( const PassiveT & _y, const PassiveT & _x);

// ///////////////////////////////////////////////////////////////////////////
// Explicit conversion to passive types
// ///////////////////////////////////////////////////////////////////////////

template <int k, typename PassiveT, bool with_hessian>
PassiveT to_passive( const Scalar<k, PassiveT, with_hessian> & a);

template <int k, int rows, int cols, typename ScalarT, bool with_hessian>
Eigen::Matrix<ScalarT, rows, cols> to_passive( const Eigen::Matrix<Scalar<k, ScalarT, with_hessian>, rows, cols> & A);

// ///////////////////////////////////////////////////////////////////////////
// TinyAD::Scalar typedefs
// ///////////////////////////////////////////////////////////////////////////

template <int k, bool with_hessian = true> using Float = Scalar<k, float, with_hessian>;
template <int k, bool with_hessian = true> using Double = Scalar<k, double, with_hessian>;
template <int k, bool with_hessian = true> using LongDouble = Scalar<k, long double, with_hessian>;

} // namespace TinyAD

// ///////////////////////////////////////////////////////////////////////////
// Eigen3 traits
// ///////////////////////////////////////////////////////////////////////////
namespace Eigen
{

/**
 * See https://eigen.tuxfamily.org/dox/TopicCustomizing_CustomScalar.html
 * and https://eigen.tuxfamily.org/dox/structEigen_1_1NumTraits.html
 */
template<int k, typename PassiveT, bool with_hessian>
struct NumTraits<TinyAD::Scalar<k, PassiveT, with_hessian>>
        : NumTraits<PassiveT>
{
    typedef TinyAD::Scalar<k, PassiveT, with_hessian> Real;
    typedef TinyAD::Scalar<k, PassiveT, with_hessian> NonInteger;
    typedef TinyAD::Scalar<k, PassiveT, with_hessian> Nested;

    enum
    {
        IsComplex = 0,
        IsInteger = 0,
        IsSigned = 1,
        RequireInitialization = 1,
        ReadCost = 1,
        AddCost = k == Eigen::Dynamic ? 1 : 1 + k + (with_hessian ? k * k : 0),
        MulCost = k == Eigen::Dynamic ? 1 : 1 + k + (with_hessian ? k * k : 0),
    };
};

/*
 * Let Eigen know that binary operations between TinyAD::Scalar and T are allowed,
 * and that the return type is TinyAD::Scalar.
 */
template<typename BinaryOp, int k, typename PassiveT, bool with_hessian>
struct ScalarBinaryOpTraits<TinyAD::Scalar<k, PassiveT, with_hessian>, PassiveT, BinaryOp>
{
    typedef TinyAD::Scalar<k, PassiveT, with_hessian> ReturnType;
};

template<typename BinaryOp, int k, typename PassiveT, bool with_hessian>
struct ScalarBinaryOpTraits<PassiveT, TinyAD::Scalar<k, PassiveT, with_hessian>, BinaryOp>
{
    typedef TinyAD::Scalar<k, PassiveT, with_hessian> ReturnType;
};

} // namespace Eigen
