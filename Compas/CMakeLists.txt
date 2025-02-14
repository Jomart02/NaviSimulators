cmake_minimum_required(VERSION 3.5)

project(CompassPlugin)

set(CMAKE_AUTOMOC ON)  
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Опция для выбора типа сборки
option(BUILD_TEST "Build for testing (source) instead of release (library)" OFF)
if(BUILD_TEST)
    add_definitions(-DBUILD_TEST)
endif()

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/)

#Подключаем директорию базового класса 
add_subdirectory(
    ${CMAKE_SOURCE_DIR}/../BaseNaviWidget
    ${CMAKE_BINARY_DIR}/BaseNaviWidget
)

set(HDRS
    ./include/Compas.h
)

set(SRCS
    ./src/Compas.cpp
)



set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ./ui/
)

# Указываем выходную директорию
if(BUILD_TEST)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_test)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
endif()

find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Сборка для тестов или релиза
if(BUILD_TEST)
    message(STATUS "Building for testing (source)")
    add_executable(${PROJECT_NAME}_test ${HDRS} ${SRCS} ./src/main.cpp)
    target_link_libraries(${PROJECT_NAME}_test Qt6::Widgets BaseNaviWidgetLib )
else()
    message(STATUS "Building for release (library)")
    add_library(${PROJECT_NAME} SHARED ${HDRS} ${SRCS})
    target_link_libraries(${PROJECT_NAME} Qt6::Widgets BaseNaviWidgetLib )
endif()


if(NOT BUILD_TEST)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Creating output directory"
)
endif()
