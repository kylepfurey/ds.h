cmake_minimum_required(VERSION 4.0)
project(ds C)

set(CMAKE_C_STANDARD 99)

add_executable(ds
        ds/ds_std.h
        ds/ds_def.h
        ds/ds_allocator.h
        ds/ds_vector.h
        ds/ds_string.h
        ds/ds_list.h
        ds/ds_queue.h
        ds/ds_set.h
        ds/ds_map.h
        ds/ds_unique.h
        ds/ds_shared.h
        ds/ds_weak.h
        ds/ds_slab.h
        ds/ds_signal.h
        ds.h
        main.c
)
