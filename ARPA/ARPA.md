
# ARPA Simulator

- [Overview](#overview)
- [Purpose](#purpose)
- [Supported NMEA Messages](#supported-nmea-messages)
- [Target Management](#target-management)
- [Additional Resources](#additional-resources)

## Overview

The ARPA simulator is designed to emulate radar signal processing systems that generate data on tracked targets. It generates NMEA messages containing secondary radar information—processed target data received from marine radar stations.

## Purpose

The simulator mimics the operation of ARPA processors that receive primary radar signals and output tracked target information, allowing:
- Generation of customizable NMEA messages for tracked targets .
- Simulation of multiple moving targets with configurable parameters (vessels, buoys, etc.).
- Testing of collision avoidance algorithms and decision-making systems.
- Training scenarios for maritime navigation using processed radar data.
- Adjustment of target parameters such as range, bearing, course, speed, CPA, and TCPA.

## Features

- **Multi-Target Tracking**: Simultaneous tracking of up to 10+ radar targets.
- **NMEA Message Generation**: Supports ARPA-specific NMEA message types with customizable fields.
- **Target Simulation**: Configurable target movement patterns (linear, turning, accelerating).
- **CPA/TCPA Calculation**: Automatic calculation of Closest Point of Approach and Time to CPA.
- **Target Acquisition**: Simulation of manual and automatic target acquisition modes.
- **Target Status**: Simulated target states (acquired, tracked, lost, dangerous).
- **User-Friendly Interface**: Built on Qt, providing intuitive controls for target management.
- **Network Output**: Transmits NMEA messages via UDP/TCP for integration with ECDIS/INS.

## Supported NMEA Messages

- **TLL - Target Latitude and Longitude** 
- **TTM - Tracked Target Message** 

## Simulation Logic

- **Target Data Processing**:
  - Simulates ARPA processor output based on primary radar returns.
  - Each target has independent movement vector (course and speed).
  - Position updates calculated using geodesic methods (WGS84 ellipsoid).

- **Relative Motion Calculation**:
  - Calculates target position relative to own ship.
  - Updates range and bearing based on relative motion vectors.
  - Simulates ARPA tracking cycles and data refresh rates.

- **CPA/TCPA Computation**:
  - Continuously calculates Closest Point of Approach for each tracked target.
  - Computes Time to CPA based on relative velocity vectors.
  - Marks targets as dangerous when CPA falls below safety threshold.

- **Target Acquisition Simulation**:
  - Simulates ARPA acquisition process from raw radar video.
  - Configurable acquisition gates and tracking windows.
  - Lost target simulation when target exits tracking area or becomes obscured.

## Target Management

- **Target Parameters**:
  - Range: Distance from own ship (0.1 - 96 nautical miles)
  - Bearing: Relative bearing to target (0-360 degrees)
  - Course: Target's true course (0-360 degrees)
  - Speed: Target's speed (0-100 knots)
  - Name: Optional target identification (up to 9 characters)

- **Dynamic Behavior**:
  - Targets can change course and speed during simulation.
  - Random variations to simulate real-world vessel movement.
  - Target deletion when out of ARPA range or manually removed.

## Additional Resources

- [NMEA 0183 Standard](https://gpsd.gitlab.io/gpsd/NMEA.html#_ttm_tracked_target_message)
----

# Симулятор АРПА

- [Обзор](#обзор)
- [Назначение](#назначение)
- [Поддерживаемые сообщения NMEA](#поддерживаемые-сообщения-nmea)
- [Управление целями](#управление-целями)
- [Дополнительные ресурсы](#дополнительные-ресурсы)

## Обзор

Симулятор АРПА  — предназначен для эмуляции систем обработки радиолокационных сигналов, выдающих данные о сопровождаемых целях. Он генерирует сообщения NMEA, содержащие вторичную радиолокационную информацию — обработанные данные о целях, поступающие от морских радиолокационных станций.

## Назначение

Симулятор имитирует работу АРПА -процессоров, которые получают первичные радиолокационные сигналы и выдают информацию о сопровождаемых целях, позволяя:
- Генерировать настраиваемые сообщения NMEA для сопровождаемых целей .
- Симулировать несколько движущихся целей с настраиваемыми параметрами (суда, буи и т.д.).
- Тестировать алгоритмы предотвращения столкновений и системы поддержки принятия решений.
- Создавать учебные сценарии для морской навигации с использованием обработанных радарных данных.
- Настраивать параметры целей, такие как дальность, пеленг, курс, скорость, CPA и TCPA.

## Поддерживаемые сообщения NMEA

- **TLL - Целевая широта и долгота**
- **TTM - Отслеживаемое целевое сообщение**

## Управление целями

- **Параметры целей**:
  - Дальность: Расстояние от своего судна (0.1 - 96 морских миль)
  - Пеленг: Относительный пеленг на цель (0-360 градусов)
  - Курс: Истинный курс цели (0-360 градусов)
  - Скорость: Скорость цели (0-100 узлов)
  - Название: Опциональная идентификация цели (до 9 символов)

- **Динамическое поведение**:
  - Цели могут менять курс и скорость во время симуляции.
  - Случайные вариации для имитации реального движения судов.
  - Удаление целей при выходе из зоны действия АРПА или при ручном удалении.

## Дополнительные ресурсы

- [Стандарт NMEA 0183](https://gpsd.gitlab.io/gpsd/NMEA.html#_ttm_tracked_target_message)

----