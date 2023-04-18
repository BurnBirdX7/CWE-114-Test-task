cmake_minimum_required(VERSION 3.8)
project(CWE-114-Test)

if (WIN32)
    set(SRC "test Task/WinSource.cpp")
elseif (LINUX OR ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(SRC "Test task/LinuxSource.cpp")
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

add_executable(test ${SRC})
target_include_directories(test PUBLIC "Test task")  # To make std_testcase.h available in *.cpp

if (LINUX OR ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(test PUBLIC ${CMAKE_DL_LIBS})
endif ()
