
#include <iostream>

//#include "header.h"
#include "header_min_lean.hpp"

using namespace std;

int main() {
    A<int> a( 3 );
    f_fri_A( a );
    // f2_fri_A( 3 );
    // f3_fri_A( a );
    a.g_A( 4 );
    a.j_A();
    auto v = a.i_A();
    for ( auto i : v )
        cout << i << endl;
    f_indep( a );

    B<int> b;
    f_fri_B( b );
    cout << b + 1;

    B<double> b2;
    f_fri_B( b2 );
    add_fri_B( b2, 1 );
    cout << b2 + 1;

    C c;
    c.g_C( 9 );
    c.h_C( 1.1 );
    c.get_x();
    
    return 0;
}

