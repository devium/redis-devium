project(hse-sample-project)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

include_directories(.)

add_library(redis
        protocol/redis.cpp
        protocol/reader.cpp
        protocol/writer.cpp
        server/Listener.cpp
        server/Socket.cpp
        server/Cmd.cpp
        server/HashTable.cpp
        server/Setter.cpp
        server/Getter.cpp
        server/Server.cpp
)

add_executable(redis-server
  main.cpp
)
target_link_libraries(redis-server redis pthread)

include_directories(contrib)

add_executable(run_test
  contrib/gmock-gtest-all.cc
  contrib/gmock_main.cc
  test/redis_test.cpp
)
target_link_libraries(run_test redis pthread)
