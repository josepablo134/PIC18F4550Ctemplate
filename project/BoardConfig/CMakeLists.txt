################################################################################
# @file CMakeLists.txt
# @author Josepablo C (josepablo134@gmail.com)
################################################################################

set(COMPONENT_NAME BoardConfig)

add_library(${COMPONENT_NAME} STATIC
		${CMAKE_CURRENT_LIST_DIR}/src/board_config.c
)

target_include_directories(${COMPONENT_NAME} PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries( ${COMPONENT_NAME}
		BSP::Adc
		BSP::IIC
		BSP::Mcu
		BSP::SPI
		BSP::Timer1
		BSP::UART
		BSP::MicrochipUSBStack::latest
)

