cmake_minimum_required(VERSION 3.5)

project(NaviSimulators VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Опция для включения тестов
option(BUILD_TESTS "Build test executable" OFF)

# Указываем папку для всех DLL и исполняемых файлов
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/output" CACHE PATH "Path for Dll")

# Устанавливаем выходные директории
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})#Только для библиотек - исполняемые будут тесты

# add_subdirectory(./thirdparty/qtgamepadlegacy)
add_subdirectory(./BaseNaviWidget)
add_subdirectory(./NavyComponents)
add_subdirectory(./ToolBoxWidget)
add_subdirectory(./SNS)
add_subdirectory(./Compas)
add_subdirectory(./Weather)
add_subdirectory(./EchoSounder)
add_subdirectory(./LagRelative)
add_subdirectory(./LagAbs)
add_subdirectory(./AIS)
add_subdirectory(./ARPA)


if(BUILD_TESTS)
    add_executable(TestRunner main.cpp)

    target_link_libraries(TestRunner PRIVATE
        BaseNaviWidgetLib
        nmeabuilder
        CoordinateEdit
        CircularValueWidgets
        ToolBoxWidget
        SNSPlugin
        CompassPlugin
        WeatherPlugin
        EchoSounderPlugin
        LagRelPlugin
        LagAbsPlugin
        AISPlugin
        ARPAPlugin
    )
endif()


# Опция для создания архива
option(CREATE_ARCHIVE "Создать архив папки с DLL" OFF)

# Команда для создания архива
if(CREATE_ARCHIVE)
    find_program(ZIP_TOOL zip)  # Ищем утилиту zip (Linux/macOS)
    if(NOT ZIP_TOOL)
        find_program(ZIP_TOOL 7z)  # Ищем утилиту 7z (Windows)
    endif()

    if(ZIP_TOOL)
        # Создаем кастомную цель для архивации
        add_custom_target(create_archive ALL
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/temp_archive_dir
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${OUTPUT_DIR} ${CMAKE_BINARY_DIR}/temp_archive_dir
            COMMAND ${ZIP_TOOL} -r ${CMAKE_BINARY_DIR}/output.zip -j ${CMAKE_BINARY_DIR}/temp_archive_dir/*
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/temp_archive_dir
            COMMENT "Создание архива ${CMAKE_BINARY_DIR}/output.zip"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )

        # Указываем зависимости для create_archive
        add_dependencies(create_archive
            BaseNaviWidgetLib
            nmeabuilder
            CoordinateEdit
            CircularValueWidgets
            ToolBoxWidget
            SNSPlugin
            CompassPlugin
            WeatherPlugin
            EchoSounderPlugin
            LagRelPlugin
            LagAbsPlugin
            AISPlugin
        )

        if(BUILD_TESTS)
            add_dependencies(create_archive TestRunner)
        endif()
    else()
        message(WARNING "Утилита для создания архива (zip или 7z) не найдена. Архив не будет создан.")
    endif()
endif()