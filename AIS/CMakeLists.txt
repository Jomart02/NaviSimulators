cmake_minimum_required(VERSION 3.5)

project(AISPlugin)

set(CMAKE_AUTOMOC ON)  
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/ClassA)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/BasePage)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/RemoteAis)

set(HDRS
    ./include/SimulatorAIS.h
    ./include/BasePage/BaseAisPage.h
    ./include/BaseAISSimulator.h
    ./include/AisStructures.h
    ./include/ClassA/Type5Simulator.h 
    ./include/ClassA/Type123Simulator.h
    ./include/ClassA/ClassAPage.h
    ./include/RemoteAis/RemoteAis.h
)

set(SRCS
    ./src/SimulatorAIS.cpp
    ./src/BasePage/BaseAisPage.cpp
    ./src/ClassA/Type123Simulator.cpp
    ./src/ClassA/Type5Simulator.cpp
    ./src/ClassA/ClassAPage.cpp

    ./src/AisStructures.cpp
    ./src/RemoteAis/RemoteAis.cpp
)

set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ./ui/
    ./ui/ClassA/
    ./ui/RemoteAis/
)

add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})

find_package(Qt6 COMPONENTS Widgets Network REQUIRED)
# Указываем зависимости
add_dependencies(${PROJECT_NAME} ToolBoxWidget)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Widgets Qt6::Network BaseNaviWidgetLib ToolBoxWidget CoordinateEdit)

if(BUILD_TESTS)
    message("Building tests ${PROJECT_NAME} ")
    target_include_directories(${PROJECT_NAME} PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ClassA
        ${CMAKE_CURRENT_SOURCE_DIR}/include/BasePage
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RemoteAis
    )
endif()