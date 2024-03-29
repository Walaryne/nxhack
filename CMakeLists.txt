cmake_minimum_required (VERSION 3.10)

project (nxhack)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_BUILD_TYPE})

add_subdirectory(StringExplosion)

add_executable(nxhack src/nxhack.cpp src/CommandRegistry.cpp)
target_link_libraries(nxhack procps dl stringexplosion pthread xcb xcb-keysyms)
target_compile_options(nxhack PUBLIC -pedantic -g)

add_library(teleport SHARED src/teleport.cpp src/CommandRegistry.cpp src/ProcessMemoryEditor.cpp)
target_include_directories(nxhack PUBLIC StringExplosion/src)
target_compile_options(teleport PUBLIC -pedantic)

set_property(TARGET nxhack PROPERTY CXX_EXTENSIONS OFF)
set_property(TARGET nxhack PROPERTY CXX_STANDARD 17)

set_property(TARGET teleport PROPERTY CXX_EXTENSIONS OFF)
set_property(TARGET teleport PROPERTY CXX_STANDARD 17)
set_property(TARGET teleport PROPERTY LIBRARY_OUTPUT_DIRECTORY ./${CMAKE_BUILD_TYPE}/plugins)