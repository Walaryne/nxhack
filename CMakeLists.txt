cmake_minimum_required (VERSION 3.10)

project (nxhack)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(nxhack nxhack.cpp)
target_link_libraries(nxhack procps /home/walaryne/eclipse-workspace/StringExplosion/build/default/libstringexplosion.so)
set_property(TARGET nxhack PROPERTY CXX_EXTENSIONS OFF)
set_property(TARGET nxhack PROPERTY CXX_STANDARD 17)