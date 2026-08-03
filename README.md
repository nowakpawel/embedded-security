# embedded-security

Bare-metal and low-level embedded work on ARM Cortex-M hardware, written from the register level. 
This repository documents a deliberate move into embedded systems, with a long-term focus on ***embedded/product security***. 

## Where I am?
Building a solid foundation in embedded C/C++ on real hardware (peripherals, buses, and RTOS, verified against datasheets and schematics. Not only tutorials). 

## Where I'm heading?
Embedded product security (memory safety, SAST for C/C++, secure boot) in the Automotive /IoT space, including MISRA C/C++, ISO 21434 and  AUTOSAR Context. 

# Background
Coming from an enterprise software background (Java, QA, cybersecurity homelab), now applying that engineering maturity to embedded systems. The security instinct that shaped the earlier work is what makes the embedded-security niche the natural target.

## What's here

### Hardware platform(s)

* ***WIO Terminal*** - ATSAMD51P19A (Cortex-M4F), 120MHz) . Primary board for foundational work.

### Projects
* ***IMU orientation over I2C*** - `week3-imu-orientation` -> Live orientation readout (pitch/ roll orientation labels + raw X/Y/Z) from the on-board LIS3DHTR accelerometer, on the LCD and streamed over Serial. 
Written against the datasheet. No high-level driver library.
Register level I2C: multi-byte reads with autoincrement, data from three axis, `atan2`- based angle computation.

## Approach

* ***Verify at source***: Datasheets, schematics and framework headers over tutorial assumptions. This method caught several real bugs like *wrong I2C bus*, *address mismatch*, *schematic label vs. measured pin state*.
* ***Predict before running***: Reason about expected behaviour first. Then check against  reality.
* ***Register level first***: Understand what the hardware does underneath the abstraction, not just that it works.

## Tech
C, C++, ARM Cortex-M (M4F), I2C/SPI/UART, register level peripherial access, PlatformIO, CMSIS.
FREERTOS and STM32 planned in future as the work progress.

## Roadmap
Foundation (C/C++ on real hardware, peripherals, RTOS) <- ***currently here*** -> security layer (static analysis, memory-safety work, secure boot) on top of that foundation.

The security layer is the destination. The embedded fundamentals are the ground it stands on.
Both are the same path. Built in order.