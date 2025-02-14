cmake_minimum_required(VERSION 3.5)
project(simulators)

# Включение Automoc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Добавление поддиректорий
add_subdirectory(./BaseNaviWidget)
add_subdirectory(./ToolBoxWidget)
add_subdirectory(./SNS)
add_subdirectory(./AIS)

# Опция для тестирования
option(BUILD_TEST "Build for testing (source) instead of release (library)" OFF)
if(BUILD_TEST)
    add_definitions(-DBUILD_TEST)
endif()
