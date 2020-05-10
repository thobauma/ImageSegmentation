cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(imageSegmentation LANGUAGES CXX)



# setting warning compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "(C|c?)lang")
    add_compile_options(-fdiagnostics-color=always --pedantic -Wextra -Wall -std=c++14)
else()
    add_compile_options(-fdiagnostics-color=always --pedantic -Wextra -Wall -std=c++14)
endif()

#---CREATE EXE & LINK LIBS---
file(GLOB CPP_FILES *.cpp)
file(GLOB H_FILES *.{h, hpp})
add_executable(main main.cpp ${CPP_FILES} ${H_FILES})




