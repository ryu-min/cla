cmake_minimum_required(VERSION 3.5)

project(cla_test LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cla_test cla_test.cpp)
add_test(NAME cla_test COMMAND cla_test)

target_link_libraries(cla_test PRIVATE Qt${QT_VERSION_MAJOR}::Test)

