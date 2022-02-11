cmake_minimum_required(VERSION 3.15)
project(SoftBody)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts/cmake")
include(scripts)
conan_init(cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

string(APPEND CMAKE_EXE_LINKER_FLAGS " -fuse-ld=lld -flto")
string(APPEND CMAKE_SHARED_LINKER_FLAGS " -fuse-ld=lld")
string(APPEND CMAKE_MODULE_LINKER_FLAGS " -fuse-ld=lld")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# finding this is not very c++20 compat
# find_program(CPPCHECK cppcheck)
# if(CPPCHECK)
#   set(CMAKE_CXX_CPPCHECK
#     ${CPPCHECK}
#     --suppress=missingInclude
#     --enable=all
#     --inline-suppr)
# endif()

string(APPEND CMAKE_CXX_FLAGS_DEBUG          " -fsanitize=address,undefined,leak")
string(APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO " -fsanitize=address,undefined,leak")

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wextra-semi -Wmissing-noreturn -msse4.2)
endif()

# if we want shared libs for some reason
# option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

find_package(Threads REQUIRED)
add_executable(softbody main.cpp)
target_include_directories(softbody PRIVATE include)
target_link_libraries(softbody PRIVATE sfml-graphics sfml-window sfml-system tbb Threads::Threads)

# add_executable(median median.cpp)
# target_include_directories(median PRIVATE include)

# find_package(benchmark REQUIRED)
# add_executable(median_bench median_bench.cpp)
# target_link_libraries(median_bench benchmark::benchmark)

# add_executable(comp comp.cpp)
