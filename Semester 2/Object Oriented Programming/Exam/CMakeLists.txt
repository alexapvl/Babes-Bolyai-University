cmake_minimum_required(VERSION 3.23)
project(myproject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# Enable Qt's automatic tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set Qt prefix path
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")

# Find required Qt6 components
find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Recursively find header files
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")

# Recursively find source files (assuming 'main.cpp' and other sources are in the 'src' directory)
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Specify the executable and include sources and headers
add_executable(main ${SOURCES} ${HEADERS})

# Link Qt libraries to the executable
target_link_libraries(main
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
