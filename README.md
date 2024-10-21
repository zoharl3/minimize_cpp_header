
# Minimize a C++ header
Given a C++ header, this tool creates a lean header file from it that removes definitions (function bodies) and includes only declarations.
The purpose is to reduce compilation time of a critical file by moving template instantiation to a separate file.

The tool is built using clang LLVM. Using libtooling, the input file goes through the first part of the compilation, and an AST is created.
The command-line options are similar to clang:

`min_header --help`

The input file must pass compilation successfully.
It should have an extension that implies a C++ file (.hh, .hpp).

I'll demonstrate the tool on the following example.

Two input files, `header.hpp`

```
template <class T>
struct A {
    void f( T t ) {
        x = t;
    }

    T x;
};
```

and `main.cpp`

```
#include "header.hpp"

int main() {
    A<int> ai;
    ai.f( 1 );
    
    A<double> ad;
    ad.f( 1.1 );

    return 0;
}
```

I'd like to reduce the compilation time of `main.cpp` by moving the instantiation of the template in `header.hpp` to another file.

I run

`min_header header.hpp` 

to generate

1. `header_min.hpp`

```
template <class T>
struct A {
    MIN_HEADER_EXPORT void f( T t ) {
        x = t;
    }

    T x;
};
```

This header differs from the original header is the addition of the macro `MIN_HEADER_EXPORT` before declaration of instantiated functions.

2. `header_min_ins.hpp`

```
// class instantiation
template
struct A<T>;

// function instantiation
```

This header explicitly instantiates template classes and headers.

3. `header_min_lean.hpp`

```
typedef bool _Bool;

template <class T>
struct A {
    void f( T t );

    T x;
};
```

This is the _lean_ header that includes only declarations.

I (manually) write `main_ins.cpp`

```
#define MIN_HEADER_EXPORT __declspec( dllexport )
#include "header_min.hpp"

#define T int
#include "header_min_ins.hpp"
#undef T

#define T double
#include "header_min_ins.hpp"
#undef T
```

For Visual Studio, I define `MIN_HEADER_EXPORT` as `__declspec( dllexport )`.
The `__declspec( dllexport )` is to force the compiler not to inline or skip unused functions.

Finally, I modify `main.cpp` to include the lean header

```
//#include "header.hpp"
#include "header_min_lean.hpp"

int main() {
    A<int> ai;
    ai.f( 1 );
    
    A<double> ad;
    ad.f( 1.1 );

    return 0;
}
```

For the background story and further motivation see [eigen_wrapper_cpp](https://github.com/zoharl3/eigen_wrapper_cpp), which takes a different approach and is dedicated to Eigen, which contains many headers.


# Building instructions

Check the release section for pre-built binaries.

Building the source requires `boost` and `llvm`.
If you are using `vcpkg`, `llvm` requires 170GB, which includes debug libraries. It might be simpler instead to download pre-built binaries, which don't include them.



