cmake_minimum_required(VERSION 3.5)
project(thor_example VERSION 1.0.1 LANGUAGES CXX )


set(CMAKE_CXX_STANDARD 11)

include_directories(/usr/include/freetype2/)


file(GLOB_RECURSE SOURCES src/*.cpp lib/*.cpp )
file(GLOB_RECURSE HEADERS include/*.h lib/*.h)

find_package(Protobuf REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(thor_simple thor_simple.cpp)
target_link_libraries(thor_simple thor ${Protobuf_LIBRARY})

add_executable(tests tests.cpp)
target_link_libraries(tests thor ${Protobuf_LIBRARY} ${OpenCV_LIBS})