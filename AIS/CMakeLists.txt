cmake_minimum_required(VERSION 3.5)

project(AISPlugin)

set(CMAKE_AUTOMOC ON)  
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Опция для выбора типа сборки
option(BUILD_TEST "Build for testing (source) instead of release (library)" OFF)
if(BUILD_TEST)
    add_definitions(-DBUILD_TEST)
endif()

INCLUDE_DIRECTORIES(./include/)


#Подключаем директорию базового класса 
if(BUILD_SINGLE)
    add_subdirectory(
        ${CMAKE_SOURCE_DIR}/../BaseNaviWidget
        ${CMAKE_BINARY_DIR}/BaseNaviWidget
    )
    add_subdirectory(
        ${CMAKE_SOURCE_DIR}/../ToolBoxWidget
        ${CMAKE_BINARY_DIR}/ToolBoxWidget
    )
endif()



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

# # Указываем выходную директорию
# if(BUILD_TEST)
#     set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_test)
# else()
#     set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
# endif()

find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Сборка для тестов или релиза
if(BUILD_TEST)
    message(STATUS "Building for testing (source)")
    add_executable(${PROJECT_NAME}_test ${HDRS} ${SRCS} ./src/main.cpp)
    target_link_libraries(${PROJECT_NAME}_test Qt6::Widgets BaseNaviWidgetLib ToolBoxWidget)
else()
    message(STATUS "Building for release (library)")
    add_library(${PROJECT_NAME} SHARED ${HDRS} ${SRCS})
    target_link_libraries(${PROJECT_NAME} Qt6::Widgets BaseNaviWidgetLib ToolBoxWidget)
endif()
