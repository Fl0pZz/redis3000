project(redis3000)
cmake_minimum_required(VERSION 3.3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

include_directories(.)

add_library(redis
        protocol/redis.cpp
        protocol/reader.cpp
        protocol/writer.cpp
        server/Socket.cpp
        server/Commands.cpp
        server/Listener.cpp
        server/Server.cpp
        server/Storage.cpp)

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
