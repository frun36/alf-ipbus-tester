cmake_minimum_required(VERSION 3.14)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/IPbus/inc)
include_directories(${PROJECT_SOURCE_DIR}/shared/include)

# Add the source files
file(GLOB SOURCES "src/*.cpp")

# Create the executable
add_executable(mock ${SOURCES})

# Link to the IPbus library
target_link_libraries(mock ipbus pthread shared)

# Specify the output directory for the mock executable
set_target_properties(mock PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
