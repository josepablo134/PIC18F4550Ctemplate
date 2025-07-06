# xsBootloader

xsBootloader (extra small Bootloader) is a bootloader solution designed to reduce the complexity of the bootloader logic (allowing this to work without interrupts) and therefore reducing the resources consumption. This bootloader implementation uses UART instead of USB due to several reasons:

 1. Software complexity. If the base product is intrinsically complex, then the consequent updates will be complex to add, verify and maintain. UART is simple and good enough for a firmware update process.
 2. Resources. xsBootloader is designed to work on the PIC18F4550 which comes with hardware limitations, like the vector interrupt remapping, or the code protection regions. This means that the software shall be small and simple.
 3. Reusability. The hypotesis is that, if the software works on the PIC18F4550 then it may also work on a more complex devices mantaining the communication specifications. This will help me to reuse the python script for this implementation and others in the future (on other microcontrollers from my collection).
 4. Compatibility. By using UART there is chance to connect bridge devices, like bluetooth modules, USB-to-serial adaptres and more. At the same time, using UART allows OTA updates by moving the OTA logic to an external device using more complex communication stacks, like ETHERNET, CAN, WIFI or any other radio or wired technology without the need to redesign the bootloader.

## Overview

This is a bootloader project for the PIC18F4550 microcontroller using the UART peripheral instead of the USB. This is to reduce the complexity of the bootloader and increase the reusability of the interface since UART allows connecting bluetooth, USB-to-serial adapters and more. At the same time, using UART also allows OTA updates by the use of external devices like an ESP32 with BLE or wifi.

The process is very simple. The device waits up to 50 ms for an ACK signal to be send by the programmer, if the timeout is reached, the device then passes the control to the application, but if the signal is received, the device confirms with another ACK and then enters to reprogramming mode, this mode is then never leaved unless the programmer requests a reset of the device.

## Bootloader Requirements and Solution Description

The bootloader uses polling for all the processes, but it requires a mechanism to detect timeouts and avoid getting blocked on any of the processes, this without using any kind of watchdog. The reason for this is, first, the interrupt vector of the PIC18F4550 is shared with application but application's interrupts shall not be affected by the bootloader, the trivial solution is not using any interrupt and just map application ISRs with a simple GOTO instruction, saving time, memory and complexity for the application. The second reason is that the watchdog is initialized by the configuration bits, and once set, the watchdog can not be disabled unless a reconfiguration happens, which requires a flash overwrite, again, the trivial solution is not using the watchdog at all.

This bootloader requires a communication mechanism for two things, commands and packets. The commands are basically a group of bytes sent by the programmer and confirmed or rejected by the device. In the other hand, the packets are bulk of bytes which represents the content of the flash either from a read request or to a write request. When reading or writing to the memory, the amount of data transmited is greater than the needed for a simple command, this means that the communication requires a flow control.

Instead of using a complex flow control, the flow is controlled by very simple commands: **read**, **erase**, **write** with a limit of bytes defined by the software implementation.

For example, the PIC18F4550 have a page size of 32 bytes (this is the smallest amount of data that can be written to the flash), and a sector size of 64 bytes (this is the smallest amount of data that can be erased), so the packet shall be multiple of these values, in order to save space in the device, the maximum amount of data that can be transmited is 128 bytes (can be increased in the configuration to up to 255). Then the read and write operations are limited to up to 128 bytes per command, leaving the flow control to the command it self, since the command is not expecting a transmission greater than 128 bytes, if the programmer requires more data then the programmer can request the same command for another memory location.

This means that the only flow control required is the transmission of a single packet of up to 128 bytes at a time. For serial there is a very known protocol to transmit files, the xModem protocol, this protocol allows transmision of several blocks of 128 bytes each. This protocol is very simple and useful, but overkill for this project. So in this project a smaller version of it is presented, it is called xsModem (extra small modem) which is not meant to transmit more than 128 bytes, and it transmits or receives a single block only. This protocol is not compatible with xModem, but it is inspired by it.

For the timeout control, the software requires a timer, this timer shall be big enough to hold several milliseconds, and the timeouts shall be small enough to be detected by the timer. Using the timer and non blocking functions is enough to detect timeouts in a polling based software.

In this project, the Timer0 is used, configured to the smallest possible frequency, giving enough room for up to 1000ms (1 second) with 1ms per every 46.875 (about 47) counts. UART is configured to work on 115200 (no parity, 8 bits, etc, a very common configuration). To react on time withing the software timeouts the main clock frequency is configured to 48Mhz using a 20Mhz crystal as input. The configuration bits are set propetly to enable the USB's clock as well, but this are not meant to be reconfigured by the programmer.

Finally, the flash driver is configured to limit its capabilities to the memory range from 0x2000 to 0x8000 using virtual addressing, the flash driver only accepts commands within the range 0x00-0x6000 which are software mapped to the range 0x2000-0x8000.

### Communication

#### Commands

The format of a command packet is simple, it requires a byte for the command identifier, followed by the known parameters, each parameter sent twice, the first time is sent normally and the second time its ones' complement is sent. The device confirms the command by sending the ACK character (0x06 from the ascii table), or rejects it by sending the CAN character (0x18 from the ascii table) + the error number.

The commands are the following:

 1. READ (id: 'R'): Identified by the character 'R', receives an uint16 as address (little endian format) and an uint8 as len. After command confirmation, an xsModem transmission shall happen where the device sends the memory content.
 2. ERASE (id: 'E' ): Identified by the character 'E', receives an uint16 as address (little endian format) and an uint8 as len.
 3. PROGRAM (id: 'P' ): Identified by the character 'P, receives an uint16 as address (little endian format). After command confirmation, an xsModem transmission shall happen where the programmer sends the memory conent, the len of the xsModem packet is the len of the memory affected. The operation is confirmed with an extra ACK or the error is reported with the CAN character + the error number. **There is no need to erase before program, this command handles the erase, write and verify operations when accepted and confirmed**.
 4. RESET (id: 0x0A): Identified by the byte 0x0A. Does not receive more parameters.
 5. PING (id: 'A'): Identified by the character 'A' Does not receive more parameters. Echoes back the same character. **This is the only command that doesn't confirm with an ACK**.

Example, a read of 64 bytes from the address 0x00 shall look like this:

 - Programmer sends: 'R' 0x00 0x00 0xFF 0xFF 0x40 0xBF ->
 - Device response (accepts the command): <- 0x06
 - Device response: <- xsModem Packet with a len of 64 bytes.

Example of bad request:

In this example the programmer requests a "block program" operation without an xsModem packet, the timeout happens and the device reports several errors.

 - Programmer sends: 'P' 0x00 0x00 0xFF 0xFF ->
 - Device response (accepts the command): <- 0x06
 - Device sends (signals that is ready for an xsModem transaction): <- 0x15
 - Programmer never sends the xsModem.
 - Device after 50ms (xsModem timeout happens): <- 0x15
 - Device sends (xsModem NACK due to timeout): <- 0x15
 - Device sends (command not confirmed due to an error on the xsModem transaction): <- 0x19  = (0x18 + 1)

#### xsModem

The xsModem protocol is based on xModem in the following things:

1. Is receiver based, the receiver shall confirm it is ready to receive by sending an NACK char (0x15 from the ascii table).
2. The packet begins with the character SOH (0x01 from the ascii table) followed by the block number format (1 byte to identify the block and it's complement), but in this implementation these bytes are not used to identify the block number, but the len of the packet payload.
3. The packet ends with an 8 bit CRC value.

The difference is that the block number is now used to identify the len of the payload block, which can be from 1 to 128 (in this implementation, but can be configured in the source code to up to 255), the CRC is calculated using only the payload content, by setting the byte 0xCE as initial value and calculating the XOR of each byte in the payload block. This serves as error checking and compatibility checking as well.
Another big difference is that the xsModem has a configurable timeout in milliseconds instead of seconds, this implementation is configured to wait up to 50ms.

An example of data transaction of two bytes of payload (0xCA 0xFE):

 - Receiver sends (signals that is ready for an xsModem transaction with an NACK char): <- 0x15
 - Sender sends the packet (SOH + LEN + ~LEN + PAYLOAD_BLOCK + CRC): 0x01 0x02 0xFD 0xCA 0xFE 0xFA ->
 - Reciver confirms reception (with an ACK char): <- 0x06

## Schematic

This is the schematic of the platform based on the pinguino 18F4550 platform, which has a LED connected to the RA4 pin and the RE3 pin connected to a reset button.

![platform\_base](pic18f4550\_platform\_sch.png?raw=true "PICNEO image")

## Programmer implemented in python

Under the folder `tools/programmer` the script `xsBldrProgrammer.py`, a script for python 3.12 using intelhex and pyserial as only dependencies can be found. This script uses argparse to receive 3 parameters, the serial port, the hex file and the json configuration of the device.

The JSON file describes basic configurations of the device, like the baudrate, the flash offset and the page, sector and block size. Where the page is the smallest writtable block in memory (in bytes), the sector is the smallest erasable block in memory (in bytes), and the block is the maximum number of bytes that can be sent to the device using xsModem.

Assuming that COM5 is the device's serial port, then the following command installs the `blinky_app.hex` image (found at `tools/programmer/examples`):

```
python .\xsBldrProgrammer.py -d .\configs\pic18f4550.json -i .\examples\blinky_app.hex -p COM5
```
