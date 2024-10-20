
cmake_minimum_required(VERSION 3.13.4)

set( PRJ_NAME "min_header" )
project( ${PRJ_NAME} )

#set(CMAKE_VERBOSE_MAKEFILE on)

if (MSVC)
    #set( CMAKE_CONFIGURATION_TYPES "Debug" "RelWithDebInfo" )
endif()

# src
add_subdirectory(src)

set_property( TARGET ${PRJ_NAME} PROPERTY FOLDER ${PRJ_NAME} )
set_property( TARGET ${PRJ_NAME} PROPERTY CXX_STANDARD 20 )

target_precompile_headers( ${PRJ_NAME} PRIVATE src/stdafx.h )

# static lib
if( 0 )
    if( MSVC )
        set( CMAKE_FIND_LIBRARY_SUFFIXES ".lib" )
        target_compile_options( ${PRJ_NAME} PRIVATE /MT )
        target_link_options( ${PRJ_NAME} PRIVATE /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT )
        #set( CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" )
    else()
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
        set(BUILD_SHARED_LIBS OFF)
        target_link_libraries( ${PRJ_NAME} -static )
    endif()
endif()
    
# clang
set( SUPPORTS_BREPRO OFF ) # avoid warning
#find_package( LLVM CONFIG REQUIRED )
find_package( Clang CONFIG REQUIRED )
include( HandleLLVMOptions )
message( "LLVM_DEFINITIONS: ${LLVM_DEFINITIONS}" )
target_compile_definitions( ${PRJ_NAME} PRIVATE ${LLVM_DEFINITIONS} )
list( APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}" )
target_include_directories( ${PRJ_NAME} PRIVATE ${LLVM_INCLUDE_DIRS} )
if( 0 )
    execute_process( COMMAND llvm-config --components OUTPUT_VARIABLE llvm_comp OUTPUT_STRIP_TRAILING_WHITESPACE )
    string(REPLACE " " ";" llvm_comp ${llvm_comp})
    message( "llvm_comp='${llvm_comp}'/n" )
    llvm_map_components_to_libnames( llvm_libs ${llvm_comp} )
    list( REMOVE_ITEM llvm_libs LLVMWindowsManifest LLVMall-targets LLVMjit LLVMx86 )
    message( "llvm_libs='${llvm_libs}'" )
    target_link_libraries( ${PRJ_NAME} PRIVATE ${llvm_libs} )
else()
    target_link_libraries(${PRJ_NAME} PRIVATE
        clangAST
        clangBasic
        clangDriver
        clangFrontend
        clangRewriteFrontend
        clangSerialization
        clangStaticAnalyzerFrontend
        clangTooling
        clangToolingSyntax
    )
endif()
    
# boost
find_package( Boost REQUIRED COMPONENTS thread serialization )
if(Boost_FOUND)
    message( "Boost found: ${Boost_INCLUDE_DIRS}" )
    target_include_directories( ${PRJ_NAME} PRIVATE ${Boost_INCLUDE_DIRS} )
    target_link_libraries( ${PRJ_NAME} PRIVATE ${Boost_LIBRARIES} )
endif()

# VS properties
if (MSVC)
    set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PRJ_NAME} )
    set_property( TARGET ${PRJ_NAME} PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS --help )
endif()

