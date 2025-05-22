cmake_minimum_required(VERSION 3.5)

project(LagAbsPlugin)

set(CMAKE_AUTOMOC ON)  
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

INCLUDE_DIRECTORIES(./include/)

set(HDRS
    ./include/LagAbs.h
)

set(SRCS
    ./src/LagAbs.cpp
)

set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ./ui/
)

add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})

find_package(Qt6 COMPONENTS Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets BaseNaviWidgetLib nmeabuilder)

if(BUILD_TESTS)
    message("Building tests ${PROJECT_NAME} ")
    target_include_directories(${PROJECT_NAME} PUBLIC ./include)
endif()