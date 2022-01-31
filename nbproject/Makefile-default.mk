#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F4550Ctemplate.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F4550Ctemplate.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=project/source/bldrCtl/bldrCtl.c project/source/Board/flashCtl/flash.c project/source/Board/Timer1/Timer1.c project/source/Board/UART/UART.c project/source/Board/board_config.c project/source/Keypad/keypad.c project/source/utils/utils.c project/source/project_macros.c project/main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/project/source/bldrCtl/bldrCtl.p1 ${OBJECTDIR}/project/source/Board/flashCtl/flash.p1 ${OBJECTDIR}/project/source/Board/Timer1/Timer1.p1 ${OBJECTDIR}/project/source/Board/UART/UART.p1 ${OBJECTDIR}/project/source/Board/board_config.p1 ${OBJECTDIR}/project/source/Keypad/keypad.p1 ${OBJECTDIR}/project/source/utils/utils.p1 ${OBJECTDIR}/project/source/project_macros.p1 ${OBJECTDIR}/project/main.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/project/source/bldrCtl/bldrCtl.p1.d ${OBJECTDIR}/project/source/Board/flashCtl/flash.p1.d ${OBJECTDIR}/project/source/Board/Timer1/Timer1.p1.d ${OBJECTDIR}/project/source/Board/UART/UART.p1.d ${OBJECTDIR}/project/source/Board/board_config.p1.d ${OBJECTDIR}/project/source/Keypad/keypad.p1.d ${OBJECTDIR}/project/source/utils/utils.p1.d ${OBJECTDIR}/project/source/project_macros.p1.d ${OBJECTDIR}/project/main.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/project/source/bldrCtl/bldrCtl.p1 ${OBJECTDIR}/project/source/Board/flashCtl/flash.p1 ${OBJECTDIR}/project/source/Board/Timer1/Timer1.p1 ${OBJECTDIR}/project/source/Board/UART/UART.p1 ${OBJECTDIR}/project/source/Board/board_config.p1 ${OBJECTDIR}/project/source/Keypad/keypad.p1 ${OBJECTDIR}/project/source/utils/utils.p1 ${OBJECTDIR}/project/source/project_macros.p1 ${OBJECTDIR}/project/main.p1

# Source Files
SOURCEFILES=project/source/bldrCtl/bldrCtl.c project/source/Board/flashCtl/flash.c project/source/Board/Timer1/Timer1.c project/source/Board/UART/UART.c project/source/Board/board_config.c project/source/Keypad/keypad.c project/source/utils/utils.c project/source/project_macros.c project/main.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F4550Ctemplate.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F4550
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/project/source/bldrCtl/bldrCtl.p1: project/source/bldrCtl/bldrCtl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source/bldrCtl" 
	@${RM} ${OBJECTDIR}/project/source/bldrCtl/bldrCtl.p1.d 
	@${RM} ${OBJECTDIR}/project/source/bldrCtl/bldrCtl.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/bldrCtl/bldrCtl.p1 project/source/bldrCtl/bldrCtl.c 
	@-${MV} ${OBJECTDIR}/project/source/bldrCtl/bldrCtl.d ${OBJECTDIR}/project/source/bldrCtl/bldrCtl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/bldrCtl/bldrCtl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/source/Board/flashCtl/flash.p1: project/source/Board/flashCtl/flash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source/Board/flashCtl" 
	@${RM} ${OBJECTDIR}/project/source/Board/flashCtl/flash.p1.d 
	@${RM} ${OBJECTDIR}/project/source/Board/flashCtl/flash.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/Board/flashCtl/flash.p1 project/source/Board/flashCtl/flash.c 
	@-${MV} ${OBJECTDIR}/project/source/Board/flashCtl/flash.d ${OBJECTDIR}/project/source/Board/flashCtl/flash.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/Board/flashCtl/flash.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/source/Board/Timer1/Timer1.p1: project/source/Board/Timer1/Timer1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source/Board/Timer1" 
	@${RM} ${OBJECTDIR}/project/source/Board/Timer1/Timer1.p1.d 
	@${RM} ${OBJECTDIR}/project/source/Board/Timer1/Timer1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/Board/Timer1/Timer1.p1 project/source/Board/Timer1/Timer1.c 
	@-${MV} ${OBJECTDIR}/project/source/Board/Timer1/Timer1.d ${OBJECTDIR}/project/source/Board/Timer1/Timer1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/Board/Timer1/Timer1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/source/Board/UART/UART.p1: project/source/Board/UART/UART.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source/Board/UART" 
	@${RM} ${OBJECTDIR}/project/source/Board/UART/UART.p1.d 
	@${RM} ${OBJECTDIR}/project/source/Board/UART/UART.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/Board/UART/UART.p1 project/source/Board/UART/UART.c 
	@-${MV} ${OBJECTDIR}/project/source/Board/UART/UART.d ${OBJECTDIR}/project/source/Board/UART/UART.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/Board/UART/UART.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/source/Board/board_config.p1: project/source/Board/board_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source/Board" 
	@${RM} ${OBJECTDIR}/project/source/Board/board_config.p1.d 
	@${RM} ${OBJECTDIR}/project/source/Board/board_config.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/Board/board_config.p1 project/source/Board/board_config.c 
	@-${MV} ${OBJECTDIR}/project/source/Board/board_config.d ${OBJECTDIR}/project/source/Board/board_config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/Board/board_config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/source/Keypad/keypad.p1: project/source/Keypad/keypad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source/Keypad" 
	@${RM} ${OBJECTDIR}/project/source/Keypad/keypad.p1.d 
	@${RM} ${OBJECTDIR}/project/source/Keypad/keypad.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/Keypad/keypad.p1 project/source/Keypad/keypad.c 
	@-${MV} ${OBJECTDIR}/project/source/Keypad/keypad.d ${OBJECTDIR}/project/source/Keypad/keypad.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/Keypad/keypad.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/source/utils/utils.p1: project/source/utils/utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source/utils" 
	@${RM} ${OBJECTDIR}/project/source/utils/utils.p1.d 
	@${RM} ${OBJECTDIR}/project/source/utils/utils.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/utils/utils.p1 project/source/utils/utils.c 
	@-${MV} ${OBJECTDIR}/project/source/utils/utils.d ${OBJECTDIR}/project/source/utils/utils.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/utils/utils.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/source/project_macros.p1: project/source/project_macros.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source" 
	@${RM} ${OBJECTDIR}/project/source/project_macros.p1.d 
	@${RM} ${OBJECTDIR}/project/source/project_macros.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/project_macros.p1 project/source/project_macros.c 
	@-${MV} ${OBJECTDIR}/project/source/project_macros.d ${OBJECTDIR}/project/source/project_macros.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/project_macros.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/main.p1: project/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project" 
	@${RM} ${OBJECTDIR}/project/main.p1.d 
	@${RM} ${OBJECTDIR}/project/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/main.p1 project/main.c 
	@-${MV} ${OBJECTDIR}/project/main.d ${OBJECTDIR}/project/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/project/source/bldrCtl/bldrCtl.p1: project/source/bldrCtl/bldrCtl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source/bldrCtl" 
	@${RM} ${OBJECTDIR}/project/source/bldrCtl/bldrCtl.p1.d 
	@${RM} ${OBJECTDIR}/project/source/bldrCtl/bldrCtl.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/bldrCtl/bldrCtl.p1 project/source/bldrCtl/bldrCtl.c 
	@-${MV} ${OBJECTDIR}/project/source/bldrCtl/bldrCtl.d ${OBJECTDIR}/project/source/bldrCtl/bldrCtl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/bldrCtl/bldrCtl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/source/Board/flashCtl/flash.p1: project/source/Board/flashCtl/flash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source/Board/flashCtl" 
	@${RM} ${OBJECTDIR}/project/source/Board/flashCtl/flash.p1.d 
	@${RM} ${OBJECTDIR}/project/source/Board/flashCtl/flash.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/Board/flashCtl/flash.p1 project/source/Board/flashCtl/flash.c 
	@-${MV} ${OBJECTDIR}/project/source/Board/flashCtl/flash.d ${OBJECTDIR}/project/source/Board/flashCtl/flash.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/Board/flashCtl/flash.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/source/Board/Timer1/Timer1.p1: project/source/Board/Timer1/Timer1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source/Board/Timer1" 
	@${RM} ${OBJECTDIR}/project/source/Board/Timer1/Timer1.p1.d 
	@${RM} ${OBJECTDIR}/project/source/Board/Timer1/Timer1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/Board/Timer1/Timer1.p1 project/source/Board/Timer1/Timer1.c 
	@-${MV} ${OBJECTDIR}/project/source/Board/Timer1/Timer1.d ${OBJECTDIR}/project/source/Board/Timer1/Timer1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/Board/Timer1/Timer1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/source/Board/UART/UART.p1: project/source/Board/UART/UART.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source/Board/UART" 
	@${RM} ${OBJECTDIR}/project/source/Board/UART/UART.p1.d 
	@${RM} ${OBJECTDIR}/project/source/Board/UART/UART.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/Board/UART/UART.p1 project/source/Board/UART/UART.c 
	@-${MV} ${OBJECTDIR}/project/source/Board/UART/UART.d ${OBJECTDIR}/project/source/Board/UART/UART.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/Board/UART/UART.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/source/Board/board_config.p1: project/source/Board/board_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source/Board" 
	@${RM} ${OBJECTDIR}/project/source/Board/board_config.p1.d 
	@${RM} ${OBJECTDIR}/project/source/Board/board_config.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/Board/board_config.p1 project/source/Board/board_config.c 
	@-${MV} ${OBJECTDIR}/project/source/Board/board_config.d ${OBJECTDIR}/project/source/Board/board_config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/Board/board_config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/source/Keypad/keypad.p1: project/source/Keypad/keypad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source/Keypad" 
	@${RM} ${OBJECTDIR}/project/source/Keypad/keypad.p1.d 
	@${RM} ${OBJECTDIR}/project/source/Keypad/keypad.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/Keypad/keypad.p1 project/source/Keypad/keypad.c 
	@-${MV} ${OBJECTDIR}/project/source/Keypad/keypad.d ${OBJECTDIR}/project/source/Keypad/keypad.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/Keypad/keypad.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/source/utils/utils.p1: project/source/utils/utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source/utils" 
	@${RM} ${OBJECTDIR}/project/source/utils/utils.p1.d 
	@${RM} ${OBJECTDIR}/project/source/utils/utils.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/utils/utils.p1 project/source/utils/utils.c 
	@-${MV} ${OBJECTDIR}/project/source/utils/utils.d ${OBJECTDIR}/project/source/utils/utils.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/utils/utils.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/source/project_macros.p1: project/source/project_macros.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project/source" 
	@${RM} ${OBJECTDIR}/project/source/project_macros.p1.d 
	@${RM} ${OBJECTDIR}/project/source/project_macros.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/source/project_macros.p1 project/source/project_macros.c 
	@-${MV} ${OBJECTDIR}/project/source/project_macros.d ${OBJECTDIR}/project/source/project_macros.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/source/project_macros.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/project/main.p1: project/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/project" 
	@${RM} ${OBJECTDIR}/project/main.p1.d 
	@${RM} ${OBJECTDIR}/project/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/project/main.p1 project/main.c 
	@-${MV} ${OBJECTDIR}/project/main.d ${OBJECTDIR}/project/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/project/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F4550Ctemplate.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F4550Ctemplate.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -mrom=default,-7dc0-7fff -mram=default,-3f4-3ff,-f9c-f9c,-fd4-fd4,-fdb-fdf,-fe3-fe7,-feb-fef,-ffd-fff  $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F4550Ctemplate.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F4550Ctemplate.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F4550Ctemplate.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F4550Ctemplate.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=0 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/PIC18F4550Ctemplate.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
