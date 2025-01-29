cmake_minimum_required(VERSION 3.30)
project(identification)

set(CMAKE_CXX_STANDARD 20)

# Tự động thêm tất cả các file .cpp vào executable
file(GLOB SOURCES "*.cpp")

add_executable(identification ${SOURCES})