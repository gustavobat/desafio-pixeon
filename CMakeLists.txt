cmake_minimum_required(VERSION 2.9)
project(desafio-pixeon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(PixeonApp main.cpp PXMainWindow.cpp PXMainWindow.h)

target_link_libraries(PixeonApp Qt${QT_VERSION_MAJOR}::Widgets)
