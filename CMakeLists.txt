cmake_minimum_required(VERSION 3.0.0)
project(Logis VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_FILES
	src/main.cpp	
)

add_executable(logis ${SRC_FILES})
