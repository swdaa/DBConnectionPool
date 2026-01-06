cmake_minimum_required(VERSION 3.16)


project(MyCppMysqlProject LANGUAGES C CXX)

# ------------------------------
# clang/clang++
# ------------------------------
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# generate compile_commands.json（ 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE PROJECT_SOURCES
    src/*.cpp
)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/Include
)

# ------------------------------
# MySQL client (Homebrew)
# ------------------------------

set(MYSQL_CLIENT_ROOT "/opt/homebrew/Cellar/mysql-client/9.5.0")

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${MYSQL_CLIENT_ROOT}/include
)

target_link_directories(${PROJECT_NAME}
    PRIVATE
        ${MYSQL_CLIENT_ROOT}/lib
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        mysqlclient
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall -Wextra -Wpedantic
)
