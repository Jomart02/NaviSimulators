cmake_minimum_required(VERSION 3.5)

project(AISPlugin)

set(CMAKE_AUTOMOC ON)  
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

INCLUDE_DIRECTORIES(./include/)

set(HDRS
    ./include/SimulatorAIS.h
    ./include/Type123Simulator.h
    ./include/Type5Simulator.h
    ./include/BaseAISSimulator.h
    ./include/AisStructures.h
)

set(SRCS
    ./src/SimulatorAIS.cpp
    ./src/Type123Simulator.cpp
    ./src/Type5Simulator.cpp
    ./src/AisStructures.cpp
)

set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ./ui/
)

add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})

find_package(Qt6 COMPONENTS Widgets REQUIRED)
# Указываем зависимости
add_dependencies(${PROJECT_NAME} ToolBoxWidget)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets BaseNaviWidgetLib ToolBoxWidget)

if(BUILD_TESTS)
    message("Building tests ${PROJECT_NAME} ")
    target_include_directories(${PROJECT_NAME} PUBLIC ./include)
endif()