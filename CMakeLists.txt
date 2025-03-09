
cmake_minimum_required(VERSION 3.12)

project(Handsome VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the output directory for the built library
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

# vcpkg toolchain
# set(CMAKE_TOOLCHAIN_FILE "E:/development/handsome/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)

# Add the library target
add_library(expressCpp STATIC
    src/core/httpSocketImp.cpp
    src/core/platformImp.cpp
    src/windowsImp.cpp
   
)

# Find Boost with asio and system components
# find_package(Boost REQUIRED COMPONENTS asio system)

# Specify include directories for the library
target_include_directories(expressCpp PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Add platform-specific socket libraries and definitions
if(WIN32)
    target_link_libraries(expressCpp PRIVATE  ws2_32)
    target_compile_definitions(expressCpp PRIVATE _WIN32_WINNT=0x0A00) # Windows 10 compatibility
else()
    target_compile_definitions(expressCpp PRIVATE __linux__)
endif()

