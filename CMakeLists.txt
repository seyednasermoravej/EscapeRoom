# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(EscapeRoom)

target_include_directories(app PRIVATE ${ZEPHYR_BASE}/drivers/input)


target_include_directories(app PRIVATE inc)
FILE(GLOB app_sources src/*.c src/*.cpp)
# target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE ${app_sources})
# target_sources(app PRIVATE src/usart.c src/queues.c)

#optimizain disable begin
add_library(set-app-flags INTERFACE)
target_compile_options(set-app-flags INTERFACE -O0)
target_link_libraries(app PRIVATE set-app-flags)
#optimizain disable end

#begin for dns resolver
include(${ZEPHYR_BASE}/samples/net/common/common.cmake)
#end for dns resolver
#begin for usb
include(${ZEPHYR_BASE}/samples/subsys/usb/common/common.cmake)
#end for usb
# list(REMOVE_ITEM app_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/unwanted_file.cpp)

#begin rgb driver

# zephyr_library_sources_ifdef(CONFIG_TCS34725		${CMAKE_CURRENT_SOURCE_DIR}/drivers/tcs34725.c)
# zephyr_include_directories(include)

add_subdirectory(drivers)
#end rgb driver
