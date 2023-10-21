################################################################################
# @file CMakeLists.txt
# @author Josepablo C (josepablo134@gmail.com)
# @version 1.0
################################################################################

set(CMAKE_C_OUTPUT_EXTENSION ".p1")# Fixed/defined by XC8
set(CMAKE_ASM_OUTPUT_EXTENSION ".p1")# Fixed/defined by XC8

set(CMAKE_C_COMPILE_OBJECT)
string(APPEND CMAKE_C_COMPILE_OBJECT
    "<CMAKE_C_COMPILER> <FLAGS> <DEFINES> <INCLUDES>"
    "   -o<OBJECT> <SOURCE>"
)

set(CMAKE_C_LINK_EXECUTABLE)
string(APPEND CMAKE_C_LINK_EXECUTABLE
    "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS>"
    "   <OBJECTS> <LINK_LIBRARIES>"
    "   -o<TARGET>"
)

set(CMAKE_STATIC_LIBRARY_SUFFIX_C ".a")

set(CMAKE_C_CREATE_STATIC_LIBRARY)
string(APPEND CMAKE_C_CREATE_STATIC_LIBRARY
    "<CMAKE_AR> -r "
    "<TARGET> "
    "<OBJECTS> <LINK_LIBRARIES> "
)
