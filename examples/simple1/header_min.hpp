
#pragma once

template <class T>
struct A {
    MIN_HEADER_EXPORT void f( T t ) {
        x = t;
    }

    T x;
};
