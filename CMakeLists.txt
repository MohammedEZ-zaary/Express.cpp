
cmake_minimum_required(VERSION 3.12)

project(Express.cpp VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the output directory for the built library
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)


# Add the library target
add_library(expressCpp STATIC
    src/core/helpFunctions.cpp
    src/core/httpParsing.cpp
    src/core/platformImp.cpp
    src/core/contentTypeMap.cpp
    src/core/fileSystemManagment.cpp
    src/core/multiPartFormDataHandler.cpp
    src/core/response.cpp
    src/windowsImp.cpp
    src/expressCpp.cpp
   
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

