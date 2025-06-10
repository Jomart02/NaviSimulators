cmake_minimum_required(VERSION 3.5)

project(SNSPlugin)

set(CMAKE_AUTOMOC ON)  
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

INCLUDE_DIRECTORIES(./include/)

set(SRCS
    ./include/SNS.h
    ./src/SNS.cpp
)

# Файлы ресурсов
set(QRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/tr${PROJECT_NAME}.qrc
)
file(WRITE ${QRCS}
    "<!DOCTYPE RCC>\n"
    "<RCC version=\"1.0\">\n"
    "    <qresource prefix=\"/translations\">\n"
    "        <file alias=\"en${PROJECT_NAME}\">${PROJECT_NAME}_en.qm</file>\n"
    "        <file alias=\"ru${PROJECT_NAME}\">${PROJECT_NAME}_ru.qm</file>\n"
    "    </qresource>\n"
    "</RCC>\n"
)
# Путь для поиска .ui файлов
set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ./ui/
)

add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")

# Переводы
set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/${PROJECT_NAME}_en.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/${PROJECT_NAME}_ru.ts
)

set(QM_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/${PROJECT_NAME}_en.qm
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/${PROJECT_NAME}_ru.qm
)

# Сбор всех файлов для lupdate
file(GLOB_RECURSE UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui")

# Поиск Qt и утилит для переводов

find_package(Qt6 COMPONENTS Widgets REQUIRED)

find_program(LUPDATE_EXECUTABLE lupdate)
find_program(LRELEASE_EXECUTABLE lrelease)
if(NOT LUPDATE_EXECUTABLE OR NOT LRELEASE_EXECUTABLE)
    message(FATAL_ERROR "lupdate or lrelease not found")
endif()

# Цель для обновления .ts файлов
add_custom_target(${PROJECT_NAME}_update_ts
    COMMAND ${LUPDATE_EXECUTABLE} ${SRCS} ${HDRS} ${UI_FILES} -ts ${TS_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Updating translation files for ${PROJECT_NAME}"
)

# Цель для компиляции .ts в .qm
add_custom_command(
    OUTPUT ${QM_FILES}
    COMMAND ${LRELEASE_EXECUTABLE} ${TS_FILES}
    DEPENDS ${TS_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Compiling translation files to .qm for ${PROJECT_NAME}"
)
add_custom_target(${PROJECT_NAME}_translations ALL DEPENDS ${QM_FILES})


add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS} ${QRCS})

find_package(Qt6 COMPONENTS Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets BaseNaviWidgetLib CircularValueWidgets nmeabuilder GeoMatch CoordinateEdit )

# Зависимость библиотеки от переводов
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_translations)


if(BUILD_TESTS)
    message("Building tests ${PROJECT_NAME} ")
    target_include_directories(${PROJECT_NAME} PUBLIC ./include)
endif()