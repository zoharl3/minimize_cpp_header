
// explicit template instantiation
template
struct A<T>;

// (dummy) function usage to encourage the compiler not to discard it
void INST_FUNC() {
    T *var0 = nullptr;
    A<T> *cls_var1 = nullptr;
    cls_var1->f( *var0 );
}

