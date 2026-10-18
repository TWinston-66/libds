cmake_minimum_required(VERSION 3.20)
project(libds LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Criterion
find_path(CRITERION_INCLUDE_DIR criterion/criterion.h REQUIRED)
find_library(CRITERION_LIBRARY criterion REQUIRED)

add_library(criterion::criterion UNKNOWN IMPORTED)
set_target_properties(criterion::criterion PROPERTIES
    IMPORTED_LOCATION "${CRITERION_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${CRITERION_INCLUDE_DIR}"
)

# Core library
add_library(ds STATIC
    libds/src/dlist.c
    libds/src/stack.c
    libds/src/queue.c
)
target_include_directories(ds PUBLIC libds/include)
target_compile_options(ds PRIVATE -Wall -Wextra -Wpedantic)
target_compile_definitions(ds PUBLIC $<$<CONFIG:Debug>:DEBUG=1>)

# Tests
enable_testing()

add_executable(test_dlist libds/test/test_dlist.c)
target_link_libraries(test_dlist PRIVATE ds criterion::criterion)
target_compile_options(test_dlist PRIVATE -Wall -Wextra -Wpedantic)

add_executable(test_stack libds/test/test_stack.c)
target_link_libraries(test_stack PRIVATE ds criterion::criterion)
target_compile_options(test_stack PRIVATE -Wall -Wextra -Wpedantic)

add_executable(test_queue libds/test/test_queue.c)
target_link_libraries(test_queue PRIVATE ds criterion::criterion)
target_compile_options(test_queue PRIVATE -Wall -Wextra -Wpedantic)

add_test(NAME test_dlist COMMAND test_dlist)
add_test(NAME test_stack COMMAND test_stack)
add_test(NAME test_queue COMMAND test_queue)
