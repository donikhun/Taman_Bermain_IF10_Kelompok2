#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc.exe
CCC=g++.exe
CXX=g++.exe
FC=gfortran.exe
AS=as.exe

# Macros
CND_PLATFORM=MinGW-Windows
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/282466788/vec3f.o \
	${OBJECTDIR}/_ext/282466788/imageloader.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lglu32 -lglut32 -lopengl32

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/taman_bermain_if10_kelompok2.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/taman_bermain_if10_kelompok2.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/taman_bermain_if10_kelompok2 ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/282466788/vec3f.o: /C/Documents/NetBeansProjects/OpenGL/Taman_Bermain_IF10_Kelompok2/vec3f.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/282466788
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/282466788/vec3f.o /C/Documents/NetBeansProjects/OpenGL/Taman_Bermain_IF10_Kelompok2/vec3f.cpp

${OBJECTDIR}/_ext/282466788/imageloader.o: /C/Documents/NetBeansProjects/OpenGL/Taman_Bermain_IF10_Kelompok2/imageloader.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/282466788
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/282466788/imageloader.o /C/Documents/NetBeansProjects/OpenGL/Taman_Bermain_IF10_Kelompok2/imageloader.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/taman_bermain_if10_kelompok2.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
