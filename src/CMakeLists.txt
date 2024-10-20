
# Source Files
file( GLOB sources *.c *.cc *.cpp *.h *.hh *.hpp *.ixx )

add_executable( ${PRJ_NAME} ${sources} )

target_include_directories( ${PRJ_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src" )
  

