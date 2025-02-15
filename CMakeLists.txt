cmake_minimum_required(VERSION 3.5)

project(NaviSimulators VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_subdirectory(./BaseNaviWidget)
add_subdirectory(./ToolBoxWidget)
add_subdirectory(./SNS)
add_subdirectory(./Compas)
add_subdirectory(./Weather)
add_subdirectory(./EchoSounder)
add_subdirectory(./LagRelative)
add_subdirectory(./LagAbs)
add_subdirectory(./AIS)

# Опция для включения тестов
option(BUILD_TESTS "Build test executable" OFF)

if(BUILD_TESTS)
    add_executable(TestRunner main.cpp)

    target_link_libraries(TestRunner PRIVATE
        BaseNaviWidgetLib
        ToolBoxWidget
        SNSPlugin
        CompassPlugin
        WeatherPlugin
        EchoSounderPlugin
        LagRelPlugin
        LagAbsPlugin
        AISPlugin
    )
endif()