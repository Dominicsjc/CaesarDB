cmake_minimum_required(VERSION 3.10)
project(CaesarDB)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/include/mysql)

link_libraries(/var/lib/mysql)

add_executable(CaesarDB main.cpp dbInstance.cpp dbInstance.h userSession.cpp userSession.h)

target_link_libraries(CaesarDB mysqlclient)
