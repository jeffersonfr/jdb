cmake_minimum_required(VERSION 3.20)

project(jdb
  LANGUAGES CXX
  VERSION 1.0
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
  Release
  Debug
)

add_subdirectory(thirdparty)

add_library(${PROJECT_NAME}
        INTERFACE
)

target_link_libraries(${PROJECT_NAME}
        INTERFACE
)

target_include_directories(${PROJECT_NAME}
        INTERFACE
            include
)

add_library(jdb::core ALIAS jdb)

add_subdirectory(tests)
