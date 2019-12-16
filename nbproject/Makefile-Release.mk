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
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/69fa279c/grep.o \
	${OBJECTDIR}/_ext/69fa279c/main.o \
	${OBJECTDIR}/_ext/69fa279c/main_window.o \
	${OBJECTDIR}/_ext/69fa279c/result.o \
	${OBJECTDIR}/_ext/69fa279c/resulttable.o \
	${OBJECTDIR}/_ext/69fa279c/searchcontroller.o


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
LDLIBSOPTIONS=-L/home/martin/Projects/cpp/fltk-1.4.x.20191115/lib -L/home/martin/Projects/cpp/my/My/dist/Release/GNU-Linux -lfltk -lfltk_forms -lmy

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/searchinfiles

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/searchinfiles: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/searchinfiles ${OBJECTFILES} ${LDLIBSOPTIONS} -lXrender -lXcursor -lXfixes -lXext -lXft -lfontconfig -lXinerama -lpthread -ldl -lm -lX11

${OBJECTDIR}/_ext/69fa279c/grep.o: /home/martin/Projects/cpp/SearchInFiles/grep.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/69fa279c
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/home/martin/Projects/cpp/fltk-1.4.x.20191115 -I/home/martin/Projects/cpp/my -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/69fa279c/grep.o /home/martin/Projects/cpp/SearchInFiles/grep.cpp

${OBJECTDIR}/_ext/69fa279c/main.o: /home/martin/Projects/cpp/SearchInFiles/main.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/69fa279c
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/home/martin/Projects/cpp/fltk-1.4.x.20191115 -I/home/martin/Projects/cpp/my -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/69fa279c/main.o /home/martin/Projects/cpp/SearchInFiles/main.cpp

${OBJECTDIR}/_ext/69fa279c/main_window.o: /home/martin/Projects/cpp/SearchInFiles/main_window.cxx
	${MKDIR} -p ${OBJECTDIR}/_ext/69fa279c
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/home/martin/Projects/cpp/fltk-1.4.x.20191115 -I/home/martin/Projects/cpp/my -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/69fa279c/main_window.o /home/martin/Projects/cpp/SearchInFiles/main_window.cxx

${OBJECTDIR}/_ext/69fa279c/result.o: /home/martin/Projects/cpp/SearchInFiles/result.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/69fa279c
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/home/martin/Projects/cpp/fltk-1.4.x.20191115 -I/home/martin/Projects/cpp/my -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/69fa279c/result.o /home/martin/Projects/cpp/SearchInFiles/result.cpp

${OBJECTDIR}/_ext/69fa279c/resulttable.o: /home/martin/Projects/cpp/SearchInFiles/resulttable.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/69fa279c
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/home/martin/Projects/cpp/fltk-1.4.x.20191115 -I/home/martin/Projects/cpp/my -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/69fa279c/resulttable.o /home/martin/Projects/cpp/SearchInFiles/resulttable.cpp

${OBJECTDIR}/_ext/69fa279c/searchcontroller.o: /home/martin/Projects/cpp/SearchInFiles/searchcontroller.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/69fa279c
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/home/martin/Projects/cpp/fltk-1.4.x.20191115 -I/home/martin/Projects/cpp/my -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/69fa279c/searchcontroller.o /home/martin/Projects/cpp/SearchInFiles/searchcontroller.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
