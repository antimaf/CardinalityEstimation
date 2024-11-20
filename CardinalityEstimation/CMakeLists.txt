cmake_minimum_required(VERSION 3.10)
project(CardinalityEstimation)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

# Detect operating system
if(WIN32)
    message("Your OS: Windows")
elseif(UNIX)
    message("Your OS: Unix")
endif()

# Add include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/third_party
)

# Add source files
set(SOURCES
    src/main.cpp
    src/CEEngine.cpp
    src/CardinalityEstimation.cpp
)

# Add XXHash library
add_subdirectory(third_party/xxhash)

# Create main executable
add_executable(main ${SOURCES})
target_link_libraries(main PRIVATE xxhash)
