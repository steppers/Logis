cmake_minimum_required(VERSION 3.0.0)
project(Logis VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_FILES
	src/components/counter.cpp
	src/components/buffer.cpp
	src/components/xor_gate.cpp
	src/components/not_gate.cpp
	src/components/or_gate.cpp
	src/components/and_gate.cpp
	src/components/register.cpp
	src/components/adder.cpp
	src/components/clock.cpp
	src/semaphore.cpp
	src/pin.cpp
	src/executor.cpp
	src/main.cpp
)

add_executable(logis ${SRC_FILES})
