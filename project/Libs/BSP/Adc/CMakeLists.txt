################################################################################
# @file CMakeLists.txt
# @author Josepablo C (josepablo134@gmail.com)
################################################################################
set(COMPONENT_NAME BSP)
set(SUB_COMPONENT_NAME Adc)

add_library(${COMPONENT_NAME}_${SUB_COMPONENT_NAME} STATIC
		${CMAKE_CURRENT_LIST_DIR}/src/Adc.c
)

target_include_directories(${COMPONENT_NAME}_${SUB_COMPONENT_NAME} PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/include
)

add_library(${COMPONENT_NAME}::${SUB_COMPONENT_NAME} ALIAS ${COMPONENT_NAME}_${SUB_COMPONENT_NAME})

target_link_libraries( ${COMPONENT_NAME}_${SUB_COMPONENT_NAME} BoardConfig Utils )

