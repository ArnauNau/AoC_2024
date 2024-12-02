cmake_minimum_required(VERSION 3.30)
project(AoC_2024 C)

set(CMAKE_C_STANDARD 23)

# Add sanitizer flags
set(SANITIZER_FLAGS "-fsanitize=address,undefined" "-fno-omit-frame-pointer")
# Warning flags
set(WARNING_FLAGS "-Wall" "-Wextra" "-Wpedantic")

# Apply the sanitizer flags to the compiler
add_compile_options(${SANITIZER_FLAGS} ${WARNING_FLAGS})
add_link_options(${SANITIZER_FLAGS})

add_executable(Day_1
        Day_1/main.c)

add_executable(Day_2
        Day_2/main.c)
