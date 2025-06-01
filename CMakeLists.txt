cmake_minimum_required(VERSION 3.20)
project(Chatapp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(client)
add_subdirectory(server)
target_link_libraries(client PRIVATE pthread)
target_link_libraries(server PRIVATE pthread)