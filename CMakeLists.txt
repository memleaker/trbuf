cmake_minimum_required(VERSION 3.0)

# project
project(trbuf)

# cpp standard
set(CMAKE_CXX_STANDARD 17)

# headers
include_directories(include)

# source
file(GLOB SOURCES "src/*.cpp")

# compiler flag
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-fPIC -Wall")

# shared library
add_library(trbuf SHARED ${SOURCES})

# shared library version
set_target_properties(trbuf PROPERTIES VERSION 1.1 SOVERSION 1)

# install library
install(TARGETS trbuf DESTINATION /usr/lib)

# install headers
install(FILES include/RingBuffer.h include/Trbuf.h DESTINATION /usr/include/trbuf)
