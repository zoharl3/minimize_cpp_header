
// explicit template instantiation
template
struct A<T>;

template
void A<T>::h_A( T t, S s );

template
void f_indep( A<T> & s );

template
struct B<T>;

template
void f_fri_B( B<T> & s );

template
void C::h_C( S s );

// (dummy) function usage to encourage the compiler not to discard it
void INST_FUNC() {    A<T> *var0 = nullptr;
    f_fri_A( *var0 );

    T *var1 = nullptr;
    A<T> *cls_var2 = nullptr;
    cls_var2->g_A( *var1 );

    A<T> *cls_var3 = nullptr;
    cls_var3->i_A(  );

    A<T> *cls_var4 = nullptr;
    cls_var4->j_A(  );

    const B<TYPE> *var5 = nullptr;
    TYPE *var6 = nullptr;
    add_fri_B( *var5, *var6 );

    const B<TYPE> *var7 = nullptr;
    TYPE *var8 = nullptr;
    operator+( *var7, *var8 );

    int *var9 = nullptr;
    C *cls_var10 = nullptr;
    cls_var10->g_C( *var9 );

    C *cls_var11 = nullptr;
    cls_var11->get_x(  );



}