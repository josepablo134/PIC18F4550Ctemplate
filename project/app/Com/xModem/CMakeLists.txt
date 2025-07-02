################################################################################
# @file CMakeLists.txt
# @author Josepablo C (josepablo134@gmail.com)
################################################################################
set(COMPONENT_NAME Com)
set(SUB_COMPONENT_NAME xModem)

add_library(${COMPONENT_NAME}_${SUB_COMPONENT_NAME} STATIC
		${CMAKE_CURRENT_LIST_DIR}/src/xModem.c
		${CMAKE_CURRENT_LIST_DIR}/src/xModem_internal.c
)

target_include_directories(${COMPONENT_NAME}_${SUB_COMPONENT_NAME} PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/inc
)

add_library(${COMPONENT_NAME}::${SUB_COMPONENT_NAME} ALIAS ${COMPONENT_NAME}_${SUB_COMPONENT_NAME})

target_link_libraries( ${COMPONENT_NAME}_${SUB_COMPONENT_NAME} 
		BoardConfig::Board
		Utils
		BSP::UART
		SysS::Scheduler
)
