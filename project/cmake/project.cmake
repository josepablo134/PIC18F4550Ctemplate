################################################################################
# @file CMakeLists.txt
# @author Josepablo C (josepablo134@gmail.com)
# @version 1.0
################################################################################

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_EXECUTABLE_SUFFIX .elf)

if(WIN32)
    # Referenced from: https://github.com/AymenLA/LedBlinker.X
	message("Configuring for win")
    set( MPLABX_BASE_PATH "C:/Program Files/Microchip/MPLABX/v6.20/" CACHE STRING "MPLAB X")
    set( MPLABX_JAVA "${MPLABX_BASE_PATH}/sys/java/zulu8.64.0.19-ca-fx-jre8.0.345-win_x64/bin/java" CACHE STRING "MPLAB X Java")
    set( MPLABX_IPE_CLI "${MPLABX_BASE_PATH}/mplab_platform/mplab_ipe/ipecmd.jar" CACHE STRING "MPLAB X IPE")
else()
    # Referenced from: https://github.com/AymenLA/LedBlinker.X
	message("Configuring for linux")
    set( MPLABX_BASE_PATH "/opt/microchip/mplabx/v6.25" CACHE STRING "MPLAB X")
    set( MPLABX_JAVA "${MPLABX_BASE_PATH}/sys/java/zulu8.80.0.17-ca-fx-jre8.0.422-linux_x64/bin/java" CACHE STRING "MPLAB X Java")
    set( MPLABX_IPE_CLI "${MPLABX_BASE_PATH}/mplab_platform/mplab_ipe/ipecmd.jar" CACHE STRING "MPLAB X IPE")
endif()

set( MCU_DEVICE "18F4550" )
# tool selection could be done by 
# P : programmer name
# S : serial number
set(PIC_PROGRAMMER_TOOL_SELECTION "P")

# use programmer name or serial number
# PK4 : PICKIT4
# PK3 : PICKIT3
# BURxxxxxxxxx : serial number
set(PIC_PROGRAMMER_IDENTIFICATION "PK3")

set( IPE_HEX_FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXEC_NAME}.hex )
