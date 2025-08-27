cmake_minimum_required(VERSION 3.20)

project(jdb2
  LANGUAGES CXX
  VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
  Release
  Debug
)

add_library(${PROJECT_NAME}_core
        INTERFACE
)

target_link_libraries(${PROJECT_NAME}_core
        INTERFACE
)

target_include_directories(${PROJECT_NAME}_core
        INTERFACE
            ${PROJECT_SOURCE_DIR}/include
)

add_library(jdb1::core ALIAS ${PROJECT_NAME}_core)

add_subdirectory(thirdparty)
add_subdirectory(tests)
