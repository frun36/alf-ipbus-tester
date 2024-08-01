cmake_minimum_required(VERSION 3.14)
project(generator)

# Set the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add the source files
file(GLOB SOURCES "src/*.cpp")

# Create the executable
add_executable(generator ${SOURCES})

# Link to the IPbus library
target_link_libraries(generator)

# Specify the output directory for the generator executable
set_target_properties(generator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
