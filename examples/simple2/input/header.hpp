
#pragma once

#include <vector>

template <class T>
struct A;

template <class T>
struct B;

// a forward declaration is required for ADL
// https://en.cppreference.com/w/cpp/language/adl
// https://stackoverflow.com/questions/54416490/friend-function-defined-inside-a-template-class
void f2_fri_A( int x = 5 );
void f3_fri_A( A<int> &s );

// A
template <class T>
struct A {
    A() = default;

    A( T x_ ) : x( x_ ) {
    }

    ~A() {
    }

    // https://stackoverflow.com/questions/79063965/instantiate-a-friend-function-in-a-template-class
    friend void f_fri_A( A &s ) { 
        s.x = 1; 
    }

    // since there are no template specifiers in this declaration, we can instantiate A only once for a certain type; ignoring this case
    // friend void f2_fri_A( int x ) {
    //     A<int> a;
    //     a.x = x;
    // }
    //
    // friend void f3_fri_A( A<int> &s ) {
    //     s.x = 1;
    // }

    void g_A( T t ) {
        x += t;
    }

    template<class S>
    void h_A( T t, S s ) {
        x += t + s;
    }

    std::vector<int> i_A() {
        return std::vector<int>( 3, x );
    }

    bool j_A() {
        return true;
    }

    T y;

private:
    T x;
};

template <class T>
void f_indep( A<T> &s ) { 
    s.y = 1; 
}

// B
template <class TYPE>
struct B {
    static constexpr int c = 123;
    using Int = int;

    template <class T>
    friend void f_fri_B( B<T> &s );

    friend TYPE add_fri_B( const B<TYPE> &b, TYPE y ) {
        Int z = 7;
        return b.x + y + c + z;
    }

    friend TYPE operator+( const B<TYPE> &b, TYPE y ) {
        return b.x + y;
    }

private:
    TYPE x;
};

template <class T>
void f_fri_B( B<T> &s ) { 
    s.x = 1; 
} 

// C
struct C {
    void g_C( int t ) {
        x = t;
    }

    template<class S>
    void h_C( S s ) {
        x += s;
    }

    int get_x() const {
        return x;
    }

    int x;
};



