# Baseline project for platform Microchip PIC18F4550 (MPLAB X project with XC8 compiler)

## Overview

This is a baseline project for the PIC18F4550 microcontroller with the following ready to use drivers:

 * __UART__ with dynamic configurable baudrate.
 * __ADC__ with static configurable number of channels.
 * __PWM__ (Pulse Width Modulation) with static configurable number of channels, using the highest possible frequency with the highest possible resolution (10 bits).
 * __DIO__ (Digital Input/Output) with static configurable number of channels.
 * Master mode __SPI__ (Serial Peripheral Interface) with static configurable clock frequency.
 * Master mode __IIC__ (Inter-Integrated Circuit) with static configurable clock frequency ( :construction: work in progress)
 * __Mcu__ (Micro-controller Unit) with static configurable clock signals to handle low-power modes (Sleep instruction for example).
 * __Timer 1__ with external 32.768 Khz crystal for real-time applications.
 * __Flash__ with static configurable memory map to read/erase/write data or code on the code flash memory.

Most of the base drivers come with __Sync__ and __Async__ interfaces to set requests to the peripheral and letting the ISRs (interrupt service routines) handle the responses. These ISRs (per driver) come with a static configurable priority.

This project also comes with an example of bootloader over UART ( :construction: work in progress) that shows the basic design and implementation of a bootloader software, control pass mechanism to application software, and modifying code flash routines.

## Schematic

This is the schematic of the platform based on the pinguino 18f4550 platform, which has a LED connected to the RA4 pin and the RE3 pin connected to a reset button.

![platform\_base](pic18f4550\_platform\_sch.png?raw=true "PICNEO image")
