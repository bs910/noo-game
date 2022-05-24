set( EXENAME noo_client )

set( CMAKE_C_FLAGS "-Wall" )
set( CMAKE_CXX_FLAGS "-Wall -std=c++17" )

set( CMAKE_BUILD_TYPE "Debug" )

file( GLOB_RECURSE SOURCES *.[c|h]pp *.c *.h )
file( GLOB_RECURSE RES *.vsh *.fsh *.gsh *.tesh *.tcsh )

link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../lib/assimp/lib/ )

add_executable( ${EXENAME} ${SOURCES} )

target_link_libraries( ${EXENAME} glfw3 GL GLU GLEW dl Xinerama Xcursor X11 Xi Xxf86vm Xrandr pthread assimp )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../libs/glm/ )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../libs/assimp/include/ )

add_custom_target( ${EXENAME}.res
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources/ ${CMAKE_BINARY_DIR}/resources/
                   COMMENT "Copying resources..." )
