cmake_minimum_required(VERSION 3.5)

project(ARPAPlugin)

set(CMAKE_AUTOMOC ON)  
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

INCLUDE_DIRECTORIES(./include/)

set(HDRS
    ./include/ARPA.h
    ./include/TargetModel.h
)

set(SRCS
    ./src/ARPA.cpp
    ./src/TargetModel.cpp
)

set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ./ui/
)

add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})

find_package(Qt6 COMPONENTS Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets BaseNaviWidgetLib CoordinateEdit nmeabuilder)

if(BUILD_TESTS)
    message("Building tests ${PROJECT_NAME} ")
    target_include_directories(${PROJECT_NAME} PUBLIC ./include)
endif()