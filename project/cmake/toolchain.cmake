################################################################################
# @file CMakeLists.txt
# @author Josepablo C (josepablo134@gmail.com)
# @version 1.0
################################################################################

set(CMAKE_SYSTEM_NAME "generic")
set(CMAKE_SYSTEM_PROCESSOR "PIC18")

set(TOOLCHAIN xc8)

if(WIN32)
	set(TOOLCHAIN_VERSION v2.41)
	set(TOOLCHAIN_PREFIX "C:/Program Files/Microchip/${TOOLCHAIN}/${TOOLCHAIN_VERSION}")
	set(TOOLCHAIN_EXEC_SUFFIX ".exe")
elseif( UNIX )
	set(TOOLCHAIN_VERSION v2.45)
	set(TOOLCHAIN_PREFIX "/opt/microchip/${TOOLCHAIN}/${TOOLCHAIN_VERSION}")
	set(TOOLCHAIN_EXEC_SUFFIX "")
endif()

set(TOOLCHAIN_BIN_DIR ${TOOLCHAIN_PREFIX}/bin)

# Perform compiler test with static library
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)

#---------------------------------------------------------------------------------------
# Set compiler/linker flags
#---------------------------------------------------------------------------------------

# Object build options
set(OBJECT_GEN_FLAGS "-mcpu=18F4550 -c -fno-short-double -fno-short-float -memi=wordwrite -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=default -D__18F4550 -msummary=-psect,-class,+mem,-hex,-file -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto")
set(LINK_FLAGS "-mcpu=18F4550 -DXPRJ_default=default -Wl,--defsym=__MPLAB_BUILD=1 -fno-short-double -fno-short-float -memi=wordwrite -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto -mrom=default,-7dc0-7fff -mram=default,-3f4-3ff,-f9c-f9c,-fd4-fd4,-fdb-fdf,-fe3-fe7,-feb-fef,-ffd-fff")

set(CMAKE_C_FLAGS "${OBJECT_GEN_FLAGS} -Os" CACHE INTERNAL "C Compiler options")
set(CMAKE_EXE_LINKER_FLAGS "${LINK_FLAGS} -Os" CACHE INTERNAL "Linker options")

#---------------------------------------------------------------------------------------
# Set debug/release build configuration Options
#---------------------------------------------------------------------------------------

# Options for DEBUG build
set(CMAKE_C_FLAGS_DEBUG "-Og -g -D__DEBUG=1" CACHE INTERNAL "C Compiler options for debug build type")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${LINK_FLAGS} -O0 -D__DEBUG=1" CACHE INTERNAL "Linker options for debug build type")

# Options for RELEASE build
set(CMAKE_C_FLAGS_RELEASE "-Os" CACHE INTERNAL "C Compiler options for release build type")
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "ASM Compiler options for release build type")


#---------------------------------------------------------------------------------------
# Set compilers
#---------------------------------------------------------------------------------------
set(CMAKE_C_COMPILER ${TOOLCHAIN_BIN_DIR}/${TOOLCHAIN}-cc${TOOLCHAIN_EXEC_SUFFIX}   CACHE INTERNAL "C Compiler")
set(CMAKE_AR ${TOOLCHAIN_BIN_DIR}/${TOOLCHAIN}-ar${TOOLCHAIN_EXEC_SUFFIX}   CACHE INTERNAL "Archiver")

set(CMAKE_FIND_ROOT_PATH)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_USER_MAKE_RULES_OVERRIDE_C ${CMAKE_CURRENT_LIST_DIR}/compiler.cmake)
