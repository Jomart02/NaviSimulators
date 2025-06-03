

# SNS Simulator

- [Overview](#overview)
- [Purpose](#purpose)
- [Features](#features)
- [Supported NMEA Messages](#supported-nmea-messages)
- [Simulation Logic](#simulation-logic)
- [Additional Resources](#additional-resources)

## Overview

The SNS (Satellite Navigation System) Simulator is designed to emulate a satellite navigation system. It generates NMEA messages, which are a standard format for transmitting position, velocity, and time data in GPS and other navigation systems. This tool is ideal for testing navigation software, debugging NMEA parsers, or simulating vessel movement in a controlled environment.

## Purpose

The simulator mimics the operation of a real satellite navigation system, allowing:
- Generation of customizable NMEA messages (e.g., RMC, VTG, ZDA, GGA, GLL, GSV).
- Simulation of vessel movement by updating position based on speed and course.
- Testing of navigation data processing in applications without a physical GPS device.
- Adjustment of parameters such as latitude, longitude, speed, course, and satellite data for realistic scenarios.

## Features

- **Customizable Source Identifier**: Select standard sources (e.g., "GP" for GPS, "GL" for GLONASS) or input a custom two-character code.
- **Position Simulation**: Optional simulation of vessel movement using geodesic calculations based on speed (SOG) and course (COG).
- **NMEA Message Generation**: Supports multiple NMEA message types with customizable fields.
- **Time Updates**: Automatically increments time in UTC format for realistic message output.
- **Random Variations**: Simulates natural variations in course and speed with periodic "freezes" for realism.
- **User-Friendly Interface**: Built on Qt, providing intuitive controls.

## Supported NMEA Messages

- **RMC (Recommended Minimum Data)**
- **VTG (Course and Speed Over Ground)**
- **ZDA (Time and Date)**
- **GGA (GPS Fix Data)**
- **GLL (Geographic Position — Latitude/Longitude)**
- **GSV (Satellites in View)**
  - (up to 4 satellites)

## Simulation Logic

- **Position Update**:
  - Uses `PositionByDistanceBearing` from `GeoMatchFunc.h` to calculate the new position.
  - Based on the WGS84 ellipsoid and geodesic method for accurate movement simulation.

- **Course and Speed (updateCOGandSOG)**:
  - Randomly adjusts course (COG) and speed (SOG) using distributions (`cogChangeDist`, `sogChangeDist`).
  - 20% chance of a "freeze" for 3-10 seconds.
  - 10% chance to slightly increase speed (if < 30 m/s) or decrease (if > 0.5 m/s).
  - Ensures speed remains non-negative and course stays within 0-360 degrees.

- **Time**:
  - Increments by 1 second with each call to `getNavigationData()`.

## Additional Resources

- [GNSS DEVICE](https://navmarine.ru/wa-data/public/site/documents/gnss/%D0%A0%D0%AD_Gamma_10_2019_.pdf)
- [NMEA Revealed](https://gpsd.gitlab.io/gpsd/NMEA.html#_rmc_recommended_minimum_navigation_information)
----

# Cимулятор СНС

- [Обзор](#обзор)
- [Назначение](#назначение)
- [Функциональные возможности](#функциональные-возможности)
- [Поддерживаемые сообщения NMEA](#поддерживаемые-сообщения-nmea)
- [Логика симуляции](#логика-симуляции)
- [Дополнительные ресурсы](#дополнительные-ресурсы)

## Обзор

Симулятор СНС (Спутниковая Навигационная Система) —  предназначен для эмуляции спутниковой навигационной системы. Оно генерирует сообщения NMEA , которые являются стандартным форматом для передачи данных о положении, скорости и времени в GPS и других навигационных системах. Этот инструмент идеально подходит для тестирования навигационного ПО, отладки парсеров NMEA или имитации движения судна в контролируемой среде.

## Назначение

Симулятор имитирует работу реальной спутниковой навигационной системы, позволяя:
- Генерировать настраиваемые сообщения NMEA (например, RMC, VTG, ZDA, GGA, GLL, GSV).
- Симулировать движение судна, обновляя позицию на основе скорости и курса.
- Тестировать обработку навигационных данных в приложениях без физического GPS-устройства.
- Настраивать параметры, такие как широта, долгота, скорость, курс и данные о спутниках для реалистичных сценариев.

## Функциональные возможности

- **Настраиваемый идентификатор источника**: Выбор стандартных источников (например, "GP" для GPS, "GL" для ГЛОНАСС) или ввод пользовательского двухсимвольного кода.
- **Симуляция положения**: Опциональная симуляция движения судна с использованием геодезических вычислений на основе скорости (SOG) и курса (COG).
- **Генерация сообщений NMEA**: Поддержка нескольких типов сообщений NMEA с настраиваемыми полями.
- **Обновление времени**: Автоматическое увеличение времени в формате UTC для реалистичного вывода сообщений.
- **Случайные изменения**: Имитация естественных вариаций курса и скорости с периодическими "заморозками" для реализма.
- **Удобный интерфейс**: Построен на Qt, предоставляет интуитивно понятные элементы управления.

## Поддерживаемые сообщения NMEA

- **RMC (Рекомендуемые минимальные данные)**
- **VTG (Курс и скорость относительно земли)**
- **ZDA (Время и дата)**
- **GGA (Данные фиксации GPS)**
- **GLL (Географическое положение — широта/долгота)**
- **GSV (Спутники в поле зрения)**
  - (до 4 спутников)

## Логика симуляции

- **Обновление положения**:
  - Использует `PositionByDistanceBearing` из `GeoMatchFunc.h` для вычисления новой позиции.
  - Основано на эллипсоиде WGS84 и геодезическом методе для точной симуляции движения.

- **Курс и скорость (updateCOGandSOG)**:
  - Случайно изменяет курс (COG) и скорость (SOG) с использованием распределений (`cogChangeDist`, `sogChangeDist`).
  - 20% шанс "заморозки" на 3-10 секунд
  - 10% шанс слегка увеличить скорость (если < 30 м/с) или уменьшить (если > 0.5 м/с).
  - Гарантирует, что скорость остается неотрицательной, а курс — в пределах 0-360 градусов.

- **Время**:
  - Увеличивается на 1 секунду при каждом вызове `getNavigationData()`.

## Дополнительные ресурсы
- [GNSS DEVICE](https://navmarine.ru/wa-data/public/site/documents/gnss/%D0%A0%D0%AD_Gamma_10_2019_.pdf)
- [NMEA Revealed](https://gpsd.gitlab.io/gpsd/NMEA.html#_rmc_recommended_minimum_navigation_information)

----
