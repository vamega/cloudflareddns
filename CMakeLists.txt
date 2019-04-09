cmake_minimum_required(VERSION 3.13)
project(cloudflareddns
        VERSION 0.1
        DESCRIPTION "C++ Dynamic DNS Client for Cloudflare"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package (cares REQUIRED MODULE)

add_executable(cloudflareddns main.cpp)
target_link_libraries(cloudflareddns cares)