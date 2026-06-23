cmake_minimum_required(VERSION 3.16)
project(WnfHijackFramework VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT WIN32)
    message(FATAL_ERROR "This project is Windows-only")
endif()

set(SOURCES
    src/main.cpp
    src/sa.cpp
    src/core/wnf_client.cpp
    src/core/tp_hijacker.cpp
    src/core/alpc_client.cpp
    src/utils/memory.cpp
    src/utils/process.cpp
)

set(HEADERS
    include/undoc/ntwrapper.h
    include/undoc/wnf_structs.h
    src/sa.h
    src/core/wnf_client.h
    src/core/tp_hijacker.h
    src/core/alpc_client.h
    src/utils/memory.h
    src/utils/process.h
)

add_executable(wnf_hijack ${SOURCES} ${HEADERS})

target_include_directories(wnf_hijack PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(wnf_hijack PRIVATE
    ntdll.lib
    kernel32.lib
    user32.lib
)

if(MSVC)
    target_compile_options(wnf_hijack PRIVATE /W4 /EHsc)
else()
    target_compile_options(wnf_hijack PRIVATE -Wall -Wextra)
endif()

add_executable(test_wnf tests/test_wnf.cpp src/core/wnf_client.cpp src/utils/memory.cpp src/utils/process.cpp)
target_include_directories(test_wnf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(test_wnf PRIVATE ntdll.lib kernel32.lib user32.lib)
