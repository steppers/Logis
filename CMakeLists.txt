cmake_minimum_required(VERSION 3.0.0)
project(Logis VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_FILES
	src/ui/ui_menu_bar.cpp
	src/ui/ui_manager.cpp
	src/ui/ui_element.cpp
	src/ui/ui_workspace.cpp
	src/ui/ui_tool_panel.cpp
	src/circuit.cpp
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

execute_process(
	COMMAND "simple2d" "--libs"
	OUTPUT_VARIABLE SIMPLE2D_LIBS
)
string(REGEX REPLACE "\n" " " SIMPLE2D_LIBS "${SIMPLE2D_LIBS}")
string(REGEX REPLACE " $" "" SIMPLE2D_LIBS "${SIMPLE2D_LIBS}")

include_directories(/usr/local/include)

add_executable(logis ${SRC_FILES})
target_link_libraries(logis ${SIMPLE2D_LIBS})