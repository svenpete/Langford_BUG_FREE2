cmake_minimum_required(VERSION 3.23)
project(Langford_BUG_FREE)

set(CMAKE_CXX_STANDARD 20)

add_executable(Langford_BUG_FREE main.cpp LangfordSequentiell.h LangfordParrallel.h LangfordParrallelOMP.h)
find_package("TBB")
target_link_libraries(Langford_BUG_FREE TBB::tbb)

FIND_PACKAGE( OpenMP)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
include_directories(SYSTEM ${OpenMP_INCLUDE_PATH})