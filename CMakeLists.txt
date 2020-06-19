cmake_minimum_required(VERSION 3.16)
project(collisions)

add_definitions(-mavx2)
set(CMAKE_CXX_STANDARD 17)

include_directories(.)

set(CMAKE_CXX_FLAGS "-Wall -mavx2")
message("AAAAAaaa ${CMAKE_BUILD_TYPE} ;; ${CMAKE_CXX_FLAGS_RELEASE} ;; ${CMAKE_CXX_FLAGS_DEBUG} ;; ${CMAKE_CXX_FLAGS}")

add_executable(collisions
        box.h
        collide_two_particles.cpp
        collide_two_particles.h
        default.h
        eps_close.cpp
        eps_close.h
        myrandom.cpp
        myrandom.h
        particle.cpp
        particle.h time_queue.cpp time_queue.h main.cpp model.h model_base.cpp model_parallel_collide.cpp thread_pool.cpp thread_pool.h hashing.cpp hashing.h model_dump.cpp main_time_interval.h)
