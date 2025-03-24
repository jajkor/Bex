cmake_minimum_required(VERSION 3.10)

project(bex)

file(GLOB bex_SRC CONFIGURE_DEPENDS "*.h" "*.cpp")
add_executable(bex ${bex_SRC})
