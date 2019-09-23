cmake_minimum_required (VERSION 2.6)

project (nxhack)

add_executable(nxhack nxhack.cpp)
target_link_libraries(nxhack procps /home/walaryne/eclipse-workspace/StringExplosion/build/default/libstringexplosion.so)