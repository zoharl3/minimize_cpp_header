
// class instantiation
template
struct Scalar<k, PassiveT, with_hessian>;

// function instantiation
template
T1 pow( const T1 & a, const T2 & e);

template
PassiveT atan2( const PassiveT & _y, const PassiveT & _x);

template
PassiveT to_passive( const Scalar<k, PassiveT, with_hessian> & a);

template
Eigen::Matrix<ScalarT, rows, cols> to_passive( const Eigen::Matrix<Scalar<k, ScalarT, with_hessian>, rows, cols> & A);



