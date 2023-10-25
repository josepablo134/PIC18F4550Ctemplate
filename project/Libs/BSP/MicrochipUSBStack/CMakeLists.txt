################################################################################
# @file CMakeLists.txt
# @author Josepablo C (josepablo134@gmail.com)
################################################################################
set(COMPONENT_NAME BSP)
set(SUB_COMPONENT_NAME MicrochipUSBStack)
set(SUB_COMPONENT_VERSION "v2018_11_26")

add_library(${COMPONENT_NAME}_${SUB_COMPONENT_NAME} STATIC
		##
		#	C files
		################################
		${CMAKE_CURRENT_LIST_DIR}/${SUB_COMPONENT_VERSION}/src/usb_device.c
		${CMAKE_CURRENT_LIST_DIR}/${SUB_COMPONENT_VERSION}/src/usb_device_hid.c
		# ${CMAKE_CURRENT_LIST_DIR}/${SUB_COMPONENT_VERSION}/src/usb_device_cdc.c
		##
		#	CFG C files
		################################
		${CMAKE_CURRENT_LIST_DIR}/${SUB_COMPONENT_VERSION}/cfg/system.c
		${CMAKE_CURRENT_LIST_DIR}/${SUB_COMPONENT_VERSION}/cfg/usb_descriptors.c
		${CMAKE_CURRENT_LIST_DIR}/${SUB_COMPONENT_VERSION}/cfg/usb_device_hid.c
		${CMAKE_CURRENT_LIST_DIR}/${SUB_COMPONENT_VERSION}/cfg/usb_events.c
)

target_include_directories(${COMPONENT_NAME}_${SUB_COMPONENT_NAME} PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/${SUB_COMPONENT_VERSION}/include
		${CMAKE_CURRENT_LIST_DIR}/${SUB_COMPONENT_VERSION}/cfg
)

add_library(${COMPONENT_NAME}::${SUB_COMPONENT_NAME}::${SUB_COMPONENT_VERSION} ALIAS ${COMPONENT_NAME}_${SUB_COMPONENT_NAME})

add_library(${COMPONENT_NAME}::${SUB_COMPONENT_NAME}::latest ALIAS ${COMPONENT_NAME}_${SUB_COMPONENT_NAME})

target_link_libraries( ${COMPONENT_NAME}_${SUB_COMPONENT_NAME} BoardConfig )

