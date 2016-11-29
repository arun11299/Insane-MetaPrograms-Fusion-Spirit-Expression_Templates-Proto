project(linker_error_example)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp example.cpp types.cpp)
add_executable(linker_error_example ${SOURCE_FILES})
