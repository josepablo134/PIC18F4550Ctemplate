################################################################################
# @file CMakeLists.txt
# @author Josepablo C (josepablo134@gmail.com)
# @version 1.0
################################################################################

cmake_minimum_required( VERSION 3.18.4 )

set(CMAKE_TOOLCHAIN_FILE cmake/toolchain.cmake)

project( PIC18F4550CTemplate
		DESCRIPTION "Project template for PIC18F4550"
		VERSION 1.0.0
		LANGUAGES C ASM
)

# ===================================================================
# Project Set-Up
# ===================================================================

set( PROJECT_NAME template )
set( EXEC_NAME ${PROJECT_NAME} )
set( WORK_DIR ${PROJECT_SOURCE_DIR} )

include( cmake/project.cmake)

# ===================================================================
# Project content
# ===================================================================

add_executable( ${EXEC_NAME} ${WORK_DIR}/main.c )

add_subdirectory( ${WORK_DIR}/Libs )
add_subdirectory( ${WORK_DIR}/BoardConfig )

target_link_libraries( ${EXEC_NAME}
		BSP::MicrochipUSBStack::latest
		BoardConfig::Board
		BoardConfig::USB
)

target_include_directories(${EXEC_NAME} PRIVATE
		${WORK_DIR}
)

target_link_options(${EXEC_NAME}
		PUBLIC
		-Wl,-Map=${EXEC_NAME}.map
)

add_custom_target(flash
	DEPENDS ${EXEC_NAME}
	DEPENDS ${MPLABX_IPE_CLI}
	COMMAND ${MPLABX_JAVA} -jar ${MPLABX_IPE_CLI} -P${MCU_DEVICE} -T${PIC_PROGRAMMER_TOOL_SELECTION}${PIC_PROGRAMMER_IDENTIFICATION} -M -F\"${IPE_HEX_FILE}\" -OL
)
