cmake_minimum_required(VERSION 3.5)

project(AISPlugin)

set(CMAKE_AUTOMOC ON)  
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/ClassA)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/BasePage)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/ClassB)

set(HDRS
    ./include/SimulatorAIS.h
    ./include/BasePage/BaseAisPage.h
    ./include/BaseAISSimulator.h
    ./include/AisStructures.h
    ./include/ClassA/Type5Simulator.h 
    ./include/ClassA/Type123Simulator.h
    ./include/ClassA/ClassAPage.h
    ./include/ClassB/ClassBPage.h
    ./include/ClassB/Type18Simulator.h
    ./include/ClassB/Type19Simulator.h
)

set(SRCS
    ./src/SimulatorAIS.cpp
    ./src/BasePage/BaseAisPage.cpp
    ./src/ClassA/Type123Simulator.cpp
    ./src/ClassA/Type5Simulator.cpp
    ./src/ClassA/ClassAPage.cpp
    ./src/ClassB/ClassBPage.cpp
    ./src/ClassB/Type18Simulator.cpp
    ./src/ClassB/Type19Simulator.cpp
    ./src/AisStructures.cpp
    
)

set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ./ui/
    ./ui/ClassA/
    ./ui/ClassB/
)




add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})


find_package(Qt6 COMPONENTS Widgets REQUIRED)
# Указываем зависимости
add_dependencies(${PROJECT_NAME} ToolBoxWidget)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets BaseNaviWidgetLib ToolBoxWidget CoordinateEdit)

if(BUILD_TESTS)
    message("Building tests ${PROJECT_NAME} ")
    target_include_directories(${PROJECT_NAME} PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ClassA
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ClassB
        ${CMAKE_CURRENT_SOURCE_DIR}/include/BasePage
    )
endif()