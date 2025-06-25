################################################################################
# @file CMakeLists.txt
# @author Josepablo C (josepablo134@gmail.com)
################################################################################
set(PACKAGE_NAME App)
set(COMPONENT_NAME SysS)
set(SUB_COMPONENT_NAME Scheduler)

add_library(${PACKAGE_NAME}_${COMPONENT_NAME}_${SUB_COMPONENT_NAME} STATIC
		${CMAKE_CURRENT_LIST_DIR}/src/Scheduler.c
)

target_include_directories(${PACKAGE_NAME}_${COMPONENT_NAME}_${SUB_COMPONENT_NAME} PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/inc
)

add_library(${PACKAGE_NAME}::${COMPONENT_NAME}::${SUB_COMPONENT_NAME} ALIAS ${PACKAGE_NAME}_${COMPONENT_NAME}_${SUB_COMPONENT_NAME})

target_link_libraries( ${PACKAGE_NAME}_${COMPONENT_NAME}_${SUB_COMPONENT_NAME} 
		BoardConfig::Board
		Utils
		BSP::Timer0
)
