# AIS Simulator (Automatic Identification System)

- [Overview](#overview)
- [Purpose](#purpose)
- [Features](#features)
- [Supported AIS Message Types](#supported-ais-message-types)
- [Simulation Logic](#simulation-logic)
- [Target Management](#target-management)
- [Additional Resources](#additional-resources)

## Overview

The AIS (Automatic Identification System) simulator is designed to emulate the output of AIS transponders. It generates NMEA AIVDM/AIVDO sentences containing vessel identification, position, course, speed, and other navigational information.

## Purpose

The simulator simulates the operation of Class A and Class B AIS transponders, allowing you to:
- Generate customizable NMEA AIVDM/AIVDO messages for various vessel types.
- Simulate Class A vessels, Class B vessels, shore stations, SAR aircraft, and navigation aids.
- Test AIS data processing in navigation systems without physical AIS equipment.
- Create training scenarios for maritime navigation and collision avoidance.
- Configure vessel parameters, such as MMSI, position, course, speed, destination, and vessel dimensions.

## Features

- **Multi-Vessel Simulation**: Simultaneous simulation of multiple AIS targets:
  - Class A vessels
  - Class B vessels
  - Base stations with VTS targets
  - SAR aircrafts
  - Aids to Navigation (AtoN)
  - AIS-SART (Search and Rescue Transponder)
  - Long-range vessels

- **AIS Message Generation**: Supports standard AIS message types (1, 4, 5, 9, 14, 18, 19, 21, 24).
- **Dynamic Vessel Movement**: Configurable vessel movement patterns with realistic navigation behavior.
- **Random Traffic**: Option to add randomly controlled AIS objects for realistic traffic scenarios.

## Supported AIS Message Types

- **Message 1 (Position Report Class A)** — Dynamic position, course, speed, and navigation status
- **Message 4 (Base Station Report)** — Base station position and time
- **Message 5 (Static and Voyage Related Data Class A)** — Vessel name, callsign, dimensions, destination, ETA
- **Message 9 (Standard SAR Aircraft Position Report)** — SAR aircraft position and altitude
- **Message 14 (Safety Related Broadcast Message)** — Safety text broadcasts
- **Message 18 (Standard Class B Equipment Position Report)** — Class B vessel position and movement
- **Message 19 (Extended Class B Equipment Position Report)** — Class B with additional vessel data
- **Message 21 (Aid-to-Navigation Report)** — AtoN position and status
- **Message 24 (Class B CS Static Data Report)** — Class B static vessel information

## Simulation Logic

- **Vessel Movement**:
  - Each vessel moves according to its own course and speed vector.
  - Position updates calculated using geodesic methods (WGS84 ellipsoid).
  - Optional vessel maneuvering (turns, speed changes, status changes).

- **Message Encoding**:
  - AIS binary data encoded into NMEA AIVDM/AIVDO sentences.
  - Proper NMEA checksum calculation and formatting.
  - Configurable transmission intervals (2-10 seconds for dynamic data, 6 minutes for static data).

- **Navigation Status**:
  - Simulated vessel states: Under way, at anchor, moored, etc.
  - Automatic status changes based on vessel behavior.

- **Safety Messages**:
  - Generation of safety-related broadcast messages (Message 14).
  - Configurable safety text content.

## Target Management

- **Vessel Parameters**:
  - MMSI: Maritime Mobile Service Identity (9 digits)
  - Name: Vessel name (up to 20 characters)
  - Callsign: Radio callsign (up to 7 characters)
  - Position: Latitude and longitude (WGS84)
  - Course: True course over ground (0-360 degrees)
  - Speed: Speed over ground (0-102 knots)
  - Heading: True heading (0-360 degrees)
  - Dimensions: Length and beam (meters)
  - Destination: Port of destination
  - ETA: Estimated Time of Arrival

- **Dynamic Updates**:
  - Vessels can change course and speed during simulation.
  - Random variations to simulate real-world navigation.
  - Vessel deletion when out of coverage area or manually removed.

## Additional Resources

- [AIVDM/AIVDO protocol decoding](https://gpsd.gitlab.io/gpsd/AIVDM.html)
----

# Симулятор АИС (Автоматическая Идентификационная Система)

- [Обзор](#обзор)
- [Назначение](#назначение)
- [Функциональные возможности](#функциональные-возможности)
- [Поддерживаемые типы сообщений АИС](#поддерживаемые-типы-сообщений-айс)
- [Логика симуляции](#логика-симуляции)
- [Управление целями](#управление-целями)
- [Дополнительные ресурсы](#дополнительные-ресурсы)

## Обзор

Симулятор АИС (Автоматическая Идентификационная Система) — предназначен для эмуляции выходных данных АИС-транспондеров. Он генерирует NMEA-предложения AIVDM/AIVDO, содержащие данные об идентификации судна, позиции, курсе, скорости и другой навигационной информации.

## Назначение

Симулятор имитирует работу АИС-транспондеров класса A и класса B, позволяя:
- Генерировать настраиваемые NMEA-сообщения AIVDM/AIVDO для различных типов судов.
- Симулировать суда класса A, суда класса B, береговых станций, авиации САР и навигационных средств.
- Тестировать обработку АИС-данных в навигационных системах без физического АИС-оборудования.
- Создавать учебные сценарии для морской навигации и предотвращения столкновений.
- Настраивать параметры судов, такие как MMSI, позиция, курс, скорость, пункт назначения и габариты судна.

## Функциональные возможности

- **Многоканальная симуляция судов**: Одновременная симуляция нескольких АИС-целей:
  - Судна класса A
  - Судна класса B
  - Береговые станции с целями VTS
  - Самолеты SAR (поисково-спасательной авиации)
  - Навигационные средства (AtoN)
  - АИС-САРТ (поисково-спасательный транспондер)
  - Суда дальнего следования

- **Генерация сообщений АИС**: Поддержка стандартных типов сообщений АИС (1, 4, 5, 9, 14, 18, 19, 21, 24).
- **Динамическое движение судов**: Настраиваемые паттерны движения с реалистичным навигационным поведением.
- **Случайный трафик**: Возможность добавления случайно управляемых АИС-объектов для реалистичных сценариев движения.

## Поддерживаемые типы сообщений АИС

- **Сообщение 1 (Отчет о позиции класса A)** — Динамические данные о позиции, курсе, скорости и навигационном статусе
- **Сообщение 4 (Отчет береговой станции)** — Позиция и время береговой станции
- **Сообщение 5 (Статические данные и данные о рейсе класса A)** — Название судна, позывной, габариты, пункт назначения, ETA
- **Сообщение 9 (Стандартный отчет о позиции авиации САР)** — Позиция и высота самолета САР
- **Сообщение 14 (Широковещательное сообщение о безопасности)** — Текстовые сообщения о безопасности
- **Сообщение 18 (Стандартный отчет о позиции оборудования класса B)** — Позиция и движение судна класса B
- **Сообщение 19 (Расширенный отчет о позиции оборудования класса B)** — Класс B с дополнительными данными о судне
- **Сообщение 21 (Отчет о навигационном средстве)** — Позиция и статус навигационного средства
- **Сообщение 24 (Статический отчет класса B CS)** — Статическая информация о судне класса B

## Логика симуляции

- **Движение судов**:
  - Каждое судно движется по собственному вектору курса и скорости.
  - Обновление позиций выполняется с использованием геодезических методов (эллипсоид WGS84).
  - Опциональное маневрирование судов (повороты, изменение скорости, изменение статуса).

- **Кодирование сообщений**:
  - Двоичные данные АИС кодируются в NMEA-предложения AIVDM/AIVDO.
  - Правильный расчет контрольной суммы NMEA и форматирование.
  - Настраиваемые интервалы передачи (2-10 секунд для динамических данных, 6 минут для статических).

- **Навигационный статус**:
  - Симулируемые состояния судов: В пути, на якоре, пришвартовано и др.
  - Автоматическое изменение статуса в зависимости от поведения судна.

- **Сообщения о безопасности**:
  - Генерация широковещательных сообщений о безопасности (Сообщение 14).
  - Настраиваемое содержание текста безопасности.

## Управление целями

- **Параметры судов**:
  - MMSI: Идентификатор морской мобильной службы (9 цифр)
  - Название: Имя судна (до 20 символов)
  - Позывной: Радиопозывной (до 7 символов)
  - Позиция: Широта и долгота (WGS84)
  - Курс: Истинный курс относительно земли (0-360 градусов)
  - Скорость: Скорость относительно земли (0-102 узла)
  - Направление: Истинный курс (0-360 градусов)
  - Габариты: Длина и ширина (метры)
  - Пункт назначения: Порт назначения
  - ETA: Расчетное время прибытия

- **Динамическое обновление**:
  - Суда могут менять курс и скорость во время симуляции.
  - Случайные вариации для имитации реальной навигации.
  - Удаление судов при выходе из зоны покрытия или при ручном удалении.

## Дополнительные ресурсы
- [AIVDM/AIVDO protocol decoding](https://gpsd.gitlab.io/gpsd/AIVDM.html)