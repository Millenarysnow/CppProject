cmake_minimum_required(VERSION 3.13)

project(shell-starter-cpp)


add_library(linenoise STATIC
    vendor/linenoise-2.0/linenoise.c
    vendor/linenoise-2.0/linenoise.h
)

target_include_directories(linenoise PUBLIC vendor/linenoise-2.0)


file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

add_executable(shell ${SOURCE_FILES})

target_include_directories(shell PRIVATE 
    src/public
    src/public/Command
) 

if(WIN32)
    target_link_libraries(shell PRIVATE linenoise)
else()
    target_link_libraries(shell PRIVATE readline)
endif() 