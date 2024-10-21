
# Extract an interface from a C++ header to ease template instantiation for reducing compilation time
Given a C++ header, this tool creates a similar lean header file without definitions (function bodies) and that includes only declarations.
The purpose is to reduce compilation time of a critical cpp file that includes it by moving template instantiation to a separate file.

The tool is built using clang LLVM. Using libtooling, the input file goes through the first part of the compilation, and an AST is created.
The command-line options are similar to clang:

`min_header --help`

For example, you can supply an additional include directory with an argument `-I<dir>`.
The input file must pass compilation successfully.
It should have an extension that clang associates with a C++ file (e.g., .hh, .hpp).

I'll demonstrate the tool on the following example step-by-step.

The app consists of two input files, `header.hpp`:

```cpp
template <class T>
struct A {
    void f( T t ) {
        x = t;
    }

    T x;
};
```

and `main.cpp`:

```cpp
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

I execute

`min_header header.hpp` 

to generate three header files:

1. `header_min.hpp`

```cpp
template <class T>
struct A {
    MIN_HEADER_EXPORT void f( T t ) {
        x = t;
    }

    T x;
};
```

> This header differs from the original header by the addition of the macro `MIN_HEADER_EXPORT` before declaration of instantiated functions.

2. `header_min_ins.hpp`

```cpp
// explicit template instantiation
template
struct A<T>;

// (dummy) function usage to encourage the compiler not to discard it
void INST_FUNC() {
    T *var0 = nullptr;
    A<T> *cls_var1 = nullptr;
    cls_var1->f( *var0 );
}
```

> This header explicitly instantiates template classes and functions.

3. `header_min_lean.hpp`

```cpp
typedef bool _Bool;

template <class T>
struct A {
    void f( T t );

    T x;
};
```

> This is the _lean_ header that includes only declarations.

Using two of these generated headers, I (manually) write `main_ins.cpp`:

```cpp
if defined( _MSC_VER ) && !defined( __clang__ ) && !defined( __INTEL_COMPILER )
    #define MIN_HEADER_EXPORT __declspec( dllexport )
#else
    #define MIN_HEADER_EXPORT __attribute__( ( used ) )
#endif
#include "header_min.hpp"

#define INST_FUNC fins1
#define T int
#include "header_min_ins.hpp"
#undef T
#undef INST_FUNC

#define INST_FUNC fins2
#define T double
#include "header_min_ins.hpp"
#undef T
#undef INST_FUNC
```

The macro `MIN_HEADER_EXPORT` is to encourage the compiler not to discard a nontemplate, inline function. gcc and clang also require the `INST_FUNC`. For details, see [Instantiate a friend function in a template class](https://stackoverflow.com/questions/79063965/instantiate-a-friend-function-in-a-template-class).

Finally, I modify `main.cpp` to include the third lean header:

```cpp
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

For the background story and further motivation see [eigen_wrapper_cpp](https://github.com/zoharl3/eigen_wrapper_cpp), which takes a different approach and is dedicated to Eigen (which contains many headers).


# Building instructions

Check the release section for pre-built binaries.

Building the source requires `boost` and `llvm`.
If you are using `vcpkg` on Windows, `llvm` requires 170GB (and it's slow to build), and it contains debug libraries. It might be simpler instead to download pre-built binaries, which don't include them.
On Linux, I chose to build the sources.



