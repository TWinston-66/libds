cmake_minimum_required(VERSION 3.20)
project(libds LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_library(ds STATIC
    libds/src/list.c
)
target_include_directories(ds PUBLIC libds/include)
target_compile_options(ds PRIVATE -Wall -Wextra -Wpedantic)
target_compile_definitions(ds PUBLIC $<$<CONFIG:Debug>:DEBUG=1>)

add_executable(libds libds/main.c)
target_link_libraries(libds PRIVATE ds)
target_compile_options(libds PRIVATE -Wall -Wextra -Wpedantic)
