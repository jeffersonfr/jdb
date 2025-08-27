# Find Git executable
find_package(Git REQUIRED)

# Get the full commit hash
execute_process(
    COMMAND "${GIT_EXECUTABLE}" rev-parse HEAD
    OUTPUT_VARIABLE COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get a short commit hash
execute_process(
    COMMAND "${GIT_EXECUTABLE}" rev-parse --short HEAD
    OUTPUT_VARIABLE SHORT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the current branch
execute_process(
    COMMAND "${GIT_EXECUTABLE}" rev-parse --abbrev-ref HEAD
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest tag
execute_process(
    COMMAND "${GIT_EXECUTABLE}" describe --tags --always
    OUTPUT_VARIABLE GIT_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
    "${CMAKE_BINARY_DIR}/config.h"
)

add_library(info INTERFACE)

target_include_directories(info
    INTERFACE
        ${CMAKE_BINARY_DIR}
)