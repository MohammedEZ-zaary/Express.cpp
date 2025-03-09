
cmake_minimum_required(VERSION 3.12)

project(Handsome VERSION 1.2.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable target
add_executable(mainTT main.cpp)

# Specify include directories for the executable
target_include_directories(mainTT PUBLIC
    ${PROJECT_SOURCE_DIR}/../include
)

# Link the external static library to the executable
target_link_libraries(mainTT PRIVATE ${PROJECT_SOURCE_DIR}/../lib/Debug/expressCpp.lib)
