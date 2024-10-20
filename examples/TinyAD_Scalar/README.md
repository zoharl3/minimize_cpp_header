
# TinyAD

This example is for the Scalar class from [TinyAD](https://github.com/patr-schm/TinyAD).

The input files are in `input`.

I ran 

`min_header Scalar.hh -Ic:/prj-external-libs/TinyAD/include/ -IC:/prj/vcpkg_installed/x64-windows/include` 

to generate

- `Scalar_min.hpp`
- `Scalar_min_ins.hpp`
- `Scalar_min_lean.hpp`

I manually wrote `basic_ins.cpp` and modified `basic.cc` to include the lean header.
