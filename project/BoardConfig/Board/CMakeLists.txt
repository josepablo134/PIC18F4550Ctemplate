################################################################################
# @file CMakeLists.txt
# @author Josepablo C (josepablo134@gmail.com)
################################################################################

set(COMPONENT_NAME BoardConfig)
set(SUB_COMPONENT_NAME Board)

add_library(${COMPONENT_NAME}_${SUB_COMPONENT_NAME} STATIC
		${CMAKE_CURRENT_LIST_DIR}/cfg/board_config.c
)

target_include_directories(${COMPONENT_NAME}_${SUB_COMPONENT_NAME} PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/cfg
)

add_library(${COMPONENT_NAME}::${SUB_COMPONENT_NAME} ALIAS ${COMPONENT_NAME}_${SUB_COMPONENT_NAME})

target_link_libraries( ${COMPONENT_NAME}_${SUB_COMPONENT_NAME}
		BSP::Mcu
		BSP::MicrochipUSBStack::latest
)

