cmake_minimum_required(VERSION 3.15)
project(SoftBody)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

string(APPEND CMAKE_EXE_LINKER_FLAGS " ")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(APPEND CMAKE_CXX_FLAGS_DEBUG          " -fsanitize=address,undefined,leak")
string(APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO " -fsanitize=address,undefined,leak")

add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wextra-semi -Wmissing-noreturn -march=sandybridge)

add_library(imgui INTERFACE)
target_include_directories(imgui INTERFACE ../imgui-1.87)

add_library(sfml INTERFACE)
target_include_directories(sfml INTERFACE ../SFML/include)
target_link_directories(sfml INTERFACE ../SFML/build/lib)
target_link_libraries(sfml INTERFACE sfml-graphics sfml-window sfml-system)

add_library(imgui-sfml INTERFACE)
target_include_directories(imgui-sfml INTERFACE ../imgui-sfml-2.5)
target_link_directories(imgui-sfml INTERFACE ../imgui-sfml-2.5/build)
target_link_libraries(imgui-sfml INTERFACE ImGui-SFML sfml imgui)

find_package(Threads REQUIRED)
add_executable(softbody main.cpp)
target_include_directories(softbody PRIVATE include)
target_link_libraries(softbody PRIVATE imgui-sfml Threads::Threads)

# add_executable(median median.cpp)
# target_include_directories(median PRIVATE include)

# add_executable(cr2 cr2.cpp)

find_package(benchmark REQUIRED)
add_executable(bench bench.cpp)
target_link_libraries(bench PRIVATE sfml benchmark::benchmark)

# not working
# add_executable(bench_poly bench_poly.cpp)
# target_link_libraries(bench_poly PRIVATE benchmark::benchmark)

# add_executable(so_immutable so_immutable.cpp)
# target_link_libraries(so_immutable PRIVATE sfml benchmark::benchmark)
