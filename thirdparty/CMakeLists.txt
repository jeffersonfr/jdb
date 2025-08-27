include(FetchContent)

find_package(Boost CONFIG REQUIRED
    COMPONENTS
        date_time
        filesystem
        iostreams
        thread
)

# jmixin
FetchContent_Declare(
    jmixin
    GIT_REPOSITORY https://github.com/jeffersonfr/jmixin.git
)
FetchContent_MakeAvailable(jmixin)

# jinject
FetchContent_Declare(
    jinject
    GIT_REPOSITORY https://github.com/jeffersonfr/jinject.git
    GIT_TAG main
)
FetchContent_MakeAvailable(jinject)

# fmt
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
)
FetchContent_MakeAvailable(fmt)

# SQLiteCpp
FetchContent_Declare(
    SQLiteCpp
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
)
FetchContent_MakeAvailable(SQLiteCpp)

# expected
FetchContent_Declare(
    expected
    GIT_REPOSITORY https://github.com/TartanLlama/expected.git
)
FetchContent_MakeAvailable(expected)

FetchContent_GetProperties(expected)
if (expected_POPULATED)
    message("SOURCE_DIR: ${expected_SOURCE_DIR}")
    message("BINARY_DIR: ${expected_BINARY_DIR}")
endif ()

# create a target to use in src/
add_library(${PROJECT_NAME}_thirdparty INTERFACE)

target_link_libraries(${PROJECT_NAME}_thirdparty
    INTERFACE
        jmixin
        jinject
        fmt
        SQLiteCpp
        sqlite3
        expected
        crypt
        Boost::date_time
        Boost::filesystem
        Boost::iostreams
        Boost::thread
        z
)

add_library(jdb::thirdparty ALIAS ${PROJECT_NAME}_thirdparty)
