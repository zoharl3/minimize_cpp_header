
// class instantiation
template
struct A<T>;

template
struct B<T>;

// function instantiation
template
void A<T>::h_A( T t, S s ) ;

template
void f_indep( A<T> & s ) ;

template
void f_fri_B( B<T> & s ) ;

template
void C::h_C( S s ) ;



