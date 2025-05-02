cmake_minimum_required(VERSION 3.2)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	if(DEFINED ENV{VITASDK})
		set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
	else()
		message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
	endif()
endif()
include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

project(diphwbkpt)
set(CMAKE_C_FLAGS "-Wl,-q -Wall -Wextra -Wpedantic -O3 -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

add_custom_target(build_payload ALL
		COMMAND ${CMAKE_MAKE_PROGRAM} -C ${CMAKE_CURRENT_SOURCE_DIR}/payload all
		BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/payload/payload.h
		SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/payload/Makefile ${CMAKE_CURRENT_SOURCE_DIR}/payload/payload.c
		VERBATIM
)

link_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(diphwbkpt
	main.c
)

add_dependencies(diphwbkpt build_payload)

target_link_libraries(diphwbkpt
	SKPLForKernel_stub
	SceKernelSuspendForDriver_stub
)

set_target_properties(diphwbkpt
	PROPERTIES LINK_FLAGS "-nostdlib"
	COMPILE_FLAGS "-D__VITA_KERNEL__"
)

vita_create_self(diphwbkpt.skprx diphwbkpt
	CONFIG exports.yml
	UNSAFE
)

add_custom_target(copy_skprx ALL
	COMMAND ${CMAKE_COMMAND} -E copy
	"${CMAKE_CURRENT_BINARY_DIR}/diphwbkpt.skprx"
	"${CMAKE_BINARY_DIR}/../bin/diphwbkpt.skprx"
	DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/diphwbkpt.skprx"
)