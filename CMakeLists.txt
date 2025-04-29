cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()
include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdlib -Wall -Wextra -Wpedantic -O3 -std=gnu99")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostdlib")

project(setdip LANGUAGES C ASM)

include_directories(
  include
)

add_executable(setdip
  src/kernel.c
)

target_link_libraries(setdip
  SceKblForKernel_365_stub
)

vita_create_self(setdip.skprx setdip
  CONFIG exports.yml
  UNSAFE
)

add_custom_target(copy_skprx ALL
  COMMAND ${CMAKE_COMMAND} -E copy
  "${CMAKE_CURRENT_BINARY_DIR}/setdip.skprx"
  "${CMAKE_BINARY_DIR}/../bin/setdip.skprx"
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/setdip.skprx"
)