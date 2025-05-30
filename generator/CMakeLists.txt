cmake_minimum_required(VERSION 3.14)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the DIM library
find_library(DIM_LIBRARY NAMES dim REQUIRED)

if(NOT DIM_LIBRARY)
    message(FATAL_ERROR "DIM library not found. Please install DIM library and try again.")
else()
    message(STATUS "DIM library found: ${DIM_LIBRARY}")
endif()

# Find Boost
find_package(Boost 1.80 REQUIRED COMPONENTS system log log_setup program_options)

# Set the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/shared/include)
include_directories(${PROJECT_SOURCE_DIR}/tomlplusplus/include)

# Add the source files
file(GLOB SOURCES "src/*.cpp")

# Create the executable
add_executable(generator ${SOURCES})

# Link to the DIM library
target_link_libraries(generator ${DIM_LIBRARY} pthread shared Boost::system Boost::log_setup Boost::log Boost::program_options)

# Specify the output directory for the generator executable
set_target_properties(generator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
