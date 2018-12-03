# ==========================================================================
#
# Copyright (C) 2018 Giancarlo Zollino, Leonardo Baroncelli
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# ==========================================================================

################################################################################
# Shell
#
SHELL = /bin/sh

################################################################################
# External dependencies
#
ifndef REDIS
$(error REDIS is not set.)
endif
ifndef MYSQL_CXX_CNT
$(error MYSQL_CXX_CNT is not set.)
endif
ifndef BOOST_PATH
$(error BOOST_PATH is not set.)
endif
ifndef CTA_ALGORITHMS
$(error CTA_ALGORITHMS is not set.)
endif
ifndef CTA_UTILS
$(error CTA_UTILS is not set.)
endif
ifndef GTEST_DIR
	$(error GTEST_DIR is not set.)
endif


################################################################################
# Utilities
#
AR       = ar cqs
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f -r
COPY_FILE= $(COPY) -p
COPY_DIR = $(COPY) -pR
DEL_FILE = rm -f
DEL_FOLDER = rm -r
SYMLINK  = ln -sf
DEL_DIR  = rm -rf
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p	# -p



################################################################################
# Project name and system
#
PROJECT=CxxRTAlib
SYSTEM= $(shell gcc -dumpmachine)




################################################################################
#	Sources (Main() )
#
SOURCE_DIR_CXX_SIM_TEST = TestEnvironment/src_cpp/rtaLibSim.cpp
SOURCE_DIR_CXX_UNIT_TEST = TestEnvironment/src_cpp/unitTest.cpp
SOURCE_DIR_CXX_PERF_TEST = TestEnvironment/src_cpp/performanceTest.cpp
SOURCE_DIR_CXX_PROF_TEST = TestEnvironment/src_cpp/profiling_src/RTA_DLTEST_DB_prof.cpp

################################################################################
#	Sources (C++)
#
SOURCE_DIR_CXX_DBC = CxxRTAlib/DBConnectors/src_cpp
SOURCE_DIR_CXX_DM  = CxxRTAlib/DataModels/src_cpp
SOURCE_DIR_CXX_RTA = CxxRTAlib/RTAInterface/src_cpp
SOURCE_DIR_CXX_U   = CxxRTAlib/Utils/src_cpp

################################################################################
#	Sources (C)
#
SOURCE_DIR_C_DBC = CxxRTAlib/DBConnectors/src_c
SOURCE_DIR_C_U = CxxRTAlib/Utils/src_c


################################################################################
#	Headers (C++)
#
INCLUDE_DIR_CXX_DBC = CxxRTAlib/DBConnectors/include_cpp
INCLUDE_DIR_CXX_DM  = CxxRTAlib/DataModels/include_cpp
INCLUDE_DIR_CXX_RTA = CxxRTAlib/RTAInterface/include_cpp
INCLUDE_DIR_CXX_U   = CxxRTAlib/Utils/include_cpp


################################################################################
#	Headers (C)
#
INCLUDE_DIR_C_DBC = CxxRTAlib/DBConnectors/include_c
INCLUDE_DIR_C_U = CxxRTAlib/Utils/include_c



################################################################################
# Objects
#
OBJECTS_DIR = CxxRTAlib/obj_cpp
OBJECTS_DIR_C = CxxRTAlib/obj_c



################################################################################
# Outputs
#
LIB_DESTDIR = lib
LIB_NAME = libcxxrtalib.a

TESTING_EXE_DESTDIR = TestEnvironment/bin
UNITTEST_EXE = unitTest
SIMTEST_EXE = rtaLibSim
PERFORMANCE_EXE = performanceTest
PROFILING_EXE = rtaDlTestDbProf

################################################################################
# Compiler and compilation flags (C++)
#
CXX = g++

CXXFLAGS = -funroll-loops -std=c++11 -lpthread

CXXFLAGS += -I$(INCLUDE_DIR_CXX_DBC) -I$(INCLUDE_DIR_CXX_DM) -I$(INCLUDE_DIR_CXX_RTA) -I$(INCLUDE_DIR_CXX_U) -I$(INCLUDE_DIR_C_U) -I$(INCLUDE_DIR_C_DBC)

CXXFLAGS += -I$(REDIS)

CXXFLAGS += -I$(CTA_ALGORITHMS)/code

CXXFLAGS += -I$(CTA_UTILS)/code

CXXFLAGS += -I$(MYSQL_CXX_CNT)/include

CXXFLAGS += -I$(BOOST_PATH)

CXXFLAGS += -Wl,-Bdynamic

CXXFLAGS += -I/usr/local/include/cppconn

GTEST_HEADERS =   $(GTEST_DIR)/include/ \
							  -I$(GTEST_DIR)/include/gtest/internal/*.h

# Flags passed to the preprocessor. Set Google Test's header directory as a system directory, such that the compiler doesn't generate warnings in Google Test headers.
GTEST_SYSTEM += -isystem $(GTEST_DIR)/include

CXXFLAGS += $(GTEST_SYSTEM)

CXXFLAGS += -I$(GTEST_HEADERS)


################################################################################
# Compiler and compilation flags (C)
#
C = gcc

CFLAGS = -std=c99 -I$(INCLUDE_DIR_C_DBC) -I$(INCLUDE_DIR_C_U)

CFLAGS += -I$(REDIS)


################################################################################
# Linking
#
LIBS += -L$(MYSQL_CXX_CNT)/lib64/ -lmysqlcppconn8

LIBS += -L$(REDIS)/ -lhiredis

LIBS += -L$(CTA_ALGORITHMS)/lib -lCTAAlgorithms

LIBS += -L$(CTA_UTILS)/lib -lCTAUtils

LIBS += -L$(REDIS) -lhiredis

LIBS += -L$(GTEST_DIR)/lib64 -lgtest



################################################################################
# Debugging configuration
#
ifeq ($(DEBUG),1)
	CXXFLAGS += -DDEBUG
endif



################################################################################
# Profiling or optimizations
#
ifeq ($(PROFILING), 1)
	CXXFLAGS += -pg -O0
else
	CXXFLAGS += -O3
endif


################################################################################
# VPATH
#
VPATH=$(SOURCE_DIR_CXX_DBC):$(INCLUDE_DIR_CXX_DBC):$(SOURCE_DIR_CXX_DM):$(INCLUDE_DIR_CXX_DM):$(SOURCE_DIR_CXX_RTA):$(INCLUDE_DIR_CXX_RTA):$(SOURCE_DIR_CXX_U):$(INCLUDE_DIR_CXX_U):$(SOURCE_DIR_C_DBC):$(INCLUDE_DIR_C_DBC):$(SOURCE_DIR_C_U):$(INCLUDE_DIR_C_U)
vpath %.o $(OBJECTS_DIR)


################################################################################
# Objects generation (C++)
#
INCLUDE_CXX=$(foreach dir,$(INCLUDE_DIR_CXX_DBC), $(wildcard $(dir)/*))
INCLUDE_CXX+=$(foreach dir,$(INCLUDE_DIR_CXX_DM), $(wildcard $(dir)/*))
INCLUDE_CXX+=$(foreach dir,$(INCLUDE_DIR_CXX_RTA), $(wildcard $(dir)/*))
INCLUDE_CXX+=$(foreach dir,$(INCLUDE_DIR_CXX_U), $(wildcard $(dir)/*))
SOURCE_CXX=$(foreach dir,$(SOURCE_DIR_CXX_DBC), $(wildcard $(dir)/*.cpp))
SOURCE_CXX+=$(foreach dir,$(SOURCE_DIR_CXX_DM), $(wildcard $(dir)/*.cpp))
SOURCE_CXX+=$(foreach dir,$(SOURCE_DIR_CXX_RTA), $(wildcard $(dir)/*.cpp))
SOURCE_CXX+=$(foreach dir,$(SOURCE_DIR_CXX_U), $(wildcard $(dir)/*.cpp))

OBJECTS=$(addsuffix .o, $(basename $(notdir $(SOURCE_CXX))))

################################################################################
# Objects generation (C)
#
INCLUDE_C=$(foreach dir,$(INCLUDE_DIR_C_DBC), $(wildcard $(dir)/*.h))
INCLUDE_C+=$(foreach dir,$(INCLUDE_DIR_C_U), $(wildcard $(dir)/*.h))
SOURCE_C=$(foreach dir,$(SOURCE_DIR_C_DBC), $(wildcard $(dir)/*.c))
SOURCE_C+=$(foreach dir,$(SOURCE_DIR_C_U), $(wildcard $(dir)/*.c))

OBJECTS_C=$(addsuffix .o, $(basename $(notdir $(SOURCE_C))))


################################################################################
# Logs
#
#$(info $$INCLUDE_CXX is [${INCLUDE_CXX}])
#$(info $$SOURCE_CXX is [${SOURCE_CXX}])
#$(info $$INCLUDE_C is [${INCLUDE_C}])
#$(info $$SOURCE_C is [${SOURCE_C}])
$(info $$OBJECTS is [${OBJECTS}])
$(info $$OBJECTS_C is [${OBJECTS_C}])


################################################################################
# Pattern rules
#
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(OBJECTS_DIR)/$@

%.o : %.c
	$(C) $(CFLAGS) -c $< -o $(OBJECTS_DIR_C)/$@


################################################################################
# Rules
#
all: staticlib tests

makeobjdir:
	test -d $(OBJECTS_DIR) || mkdir -p $(OBJECTS_DIR)
	test -d $(OBJECTS_DIR_C) || mkdir -p $(OBJECTS_DIR_C)

makelibdir:
	test -d $(LIB_DESTDIR) || mkdir -p $(LIB_DESTDIR)

makeexedir:
	test -d $(TESTING_EXE_DESTDIR) || mkdir -p $(TESTING_EXE_DESTDIR)


staticlib: makelibdir makeobjdir $(OBJECTS_C) $(OBJECTS)
		test -d $(LIB_DESTDIR) || mkdir -p $(LIB_DESTDIR)
		$(AR) $(LIB_DESTDIR)/$(LIB_NAME) $(OBJECTS_DIR)/*.o $(OBJECTS_DIR_C)/*.o

tests: makeexedir unittest sim performancetest profiletest

unittest:
	$(CXX) $(SOURCE_DIR_CXX_UNIT_TEST) $(CXXFLAGS) -o $(TESTING_EXE_DESTDIR)/$(UNITTEST_EXE) $(OBJECTS_DIR)/*.o  $(OBJECTS_DIR_C)/*.o $(LIBS)

sim:
	$(CXX) $(SOURCE_DIR_CXX_SIM_TEST)  $(CXXFLAGS) -o $(TESTING_EXE_DESTDIR)/$(SIMTEST_EXE) $(OBJECTS_DIR)/*.o  $(OBJECTS_DIR_C)/*.o $(LIBS)

performancetest:
	$(CXX) $(SOURCE_DIR_CXX_PERF_TEST) $(CXXFLAGS) -o $(TESTING_EXE_DESTDIR)/$(PERFORMANCE_EXE) $(OBJECTS_DIR)/*.o  $(OBJECTS_DIR_C)/*.o $(LIBS)

profiletest:
	$(CXX) $(SOURCE_DIR_CXX_PROF_TEST) $(CXXFLAGS) -o $(TESTING_EXE_DESTDIR)/$(PROFILING_EXE) $(OBJECTS_DIR)/*.o  $(OBJECTS_DIR_C)/*.o $(LIBS)


clean:
	$(DEL_FILE) $(OBJECTS_DIR)/*.o
	$(DEL_FILE) $(OBJECTS_DIR_C)/*.o
	$(DEL_FILE) $(LIB_DESTDIR)/*
	$(DEL_FILE) $(TESTING_EXE_DESTDIR)/*
