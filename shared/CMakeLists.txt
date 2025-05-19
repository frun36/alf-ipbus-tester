cmake_minimum_required(VERSION 3.14)

# Add include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/tomlplusplus/include)

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Create a shared library
add_library(shared STATIC ${SOURCES})

find_package(Boost REQUIRED COMPONENTS system log)
target_link_libraries(shared Boost::system Boost::log)


# Specify the output directory for the shared library
set_target_properties(shared PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
