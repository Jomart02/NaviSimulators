cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME ToolBoxWidget)
project(${PROJECT_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(./include)


set(HDRS
    ./include/ToolItem.h
    ./include/ToolBox.h
)
set(SRC
    ./src/ToolItem.cpp
    ./src/ToolBox.cpp
)

set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ./ui/
)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_library(${PROJECT_NAME} STATIC ${HDRS} ${SRC})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets )
# Указываем, что заголовочные файлы библиотеки должны быть доступны для других проектов
target_include_directories(${PROJECT_NAME} PUBLIC ./include)