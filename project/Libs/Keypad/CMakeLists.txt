################################################################################
# @file CMakeLists.txt
# @author Josepablo C (josepablo134@gmail.com)
################################################################################
set(COMPONENT_NAME Keypad)

add_library(${COMPONENT_NAME} STATIC
		${CMAKE_CURRENT_LIST_DIR}/src/Keypad.c
)

target_include_directories(${COMPONENT_NAME} PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries( ${COMPONENT_NAME}
	BoardConfig::Board
	Utils
)

