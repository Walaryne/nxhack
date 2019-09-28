cmake_minimum_required (VERSION 3.10)

project (nxhack)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_BUILD_TYPE})

add_executable(nxhack src/nxhack.cpp)
target_link_libraries(nxhack procps /home/walaryne/c++/StringExplosion/build/libstringexplosion.so)
set_property(TARGET nxhack PROPERTY CXX_EXTENSIONS OFF)
set_property(TARGET nxhack PROPERTY CXX_STANDARD 17)