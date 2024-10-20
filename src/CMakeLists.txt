
# Source Files
#file( GLOB_RECURSE sources *.c *.cpp *.h *.hpp *.ixx
file( GLOB sources *.c *.cpp *.h *.hpp *.ixx )

# source2
set( dir c:/prj/confstruct/src )
set( sources2
    ${dir}/debug/logger.cpp
    ${dir}/debug/logging.cpp
    )
    
add_executable( ${PRJ_NAME} ${sources} ${sources2} )

target_include_directories( ${PRJ_NAME} PRIVATE "${dir}/debug" )
target_include_directories( ${PRJ_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src" )
  

