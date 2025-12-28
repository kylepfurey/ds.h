cmake_minimum_required(VERSION 4.0)
project(ds C)

set(CMAKE_C_STANDARD 99)

add_executable(ds
        ds/std.h
        ds/vector.h
        ds/list.h
        ds/queue.h
        ds/set.h
        ds/map.h
        ds/unique.h
        ds/shared.h
        ds/weak.h
        ds/signal.h
        ds/slab.h
        ds.h
        main.c
)
