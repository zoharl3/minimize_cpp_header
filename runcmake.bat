
@echo off
cls

@SETLOCAL

mkdir build
cd build

cmake ^
-DCMAKE_TOOLCHAIN_FILE=d:/vcpkg/scripts/buildsystems/vcpkg.cmake ^ -DVCPKG_TARGET_TRIPLET=x64-windows ^
-Wno-dev -G "Visual Studio 17 2022" ^
..

rem -T ClangCL ^

cd ..

pause
