################################################################################
# @file CMakeLists.txt
# @author Josepablo C (josepablo134@gmail.com)
################################################################################

set(COMPONENT_NAME BoardConfig)
set(SUB_COMPONENT_NAME USB)

add_library(${COMPONENT_NAME}_${SUB_COMPONENT_NAME} STATIC
		${CMAKE_CURRENT_LIST_DIR}/cfg/system.c
		${CMAKE_CURRENT_LIST_DIR}/cfg/usb_descriptors.c
		${CMAKE_CURRENT_LIST_DIR}/cfg/usb_device_hid.c
		${CMAKE_CURRENT_LIST_DIR}/cfg/usb_events.c
)

target_include_directories(${COMPONENT_NAME}_${SUB_COMPONENT_NAME} PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/cfg
)

add_library(${COMPONENT_NAME}::${SUB_COMPONENT_NAME} ALIAS ${COMPONENT_NAME}_${SUB_COMPONENT_NAME})

target_link_libraries( ${COMPONENT_NAME}_${SUB_COMPONENT_NAME}
		BSP::Mcu
		BSP::MicrochipUSBStack::latest
)

