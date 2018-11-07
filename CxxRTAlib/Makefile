# ==========================================================================
#
# Copyright (C) 2018 Giancarlo Zollino
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


SHELL = /bin/sh

# Check dependencies
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



# Utilities
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


####### 1) Project names and system
SYSTEM= $(shell gcc -dumpmachine)


####### 2) Directories for the compiler
### Object directories
OBJECTS_DIR = CxxRTAlib/obj_cpp
OBJECTS_DIR_C = CxxRTAlib/obj_c

##### Source executables
SOURCE_DIR_CXX_UNIT_TEST = TestEnvironment/src_cpp/UnitTest.cpp
#SOURCE_DIR_CXX_PERF_TEST = TestEnvironment/src_cpp/PerfTest.cpp


#### Source Cxx directories
SOURCE_DIR_CXX_DBC = CxxRTAlib/DBConnectors/src_cpp
SOURCE_DIR_CXX_DM = CxxRTAlib/DataModels/src_cpp
SOURCE_DIR_CXX_RTA = CxxRTAlib/RTAInterface/src_cpp
SOURCE_DIR_CXX_U = CxxRTAlib/Utils/src_cpp


### Include Cxx directories
INCLUDE_DIR_CXX_DBC = CxxRTAlib/DBConnectors/include_cpp
INCLUDE_DIR_CXX_DM = CxxRTAlib/DataModels/include_cpp
INCLUDE_DIR_CXX_RTA = CxxRTAlib/RTAInterface/include_cpp
INCLUDE_DIR_CXX_U = CxxRTAlib/Utils/include_cpp

#### Source C directories
SOURCE_DIR_C_DBC = CxxRTAlib/DBConnectors/src_c
SOURCE_DIR_C_U = CxxRTAlib/Utils/src_c

### Include C directories
INCLUDE_DIR_C_DBC = CxxRTAlib/DBConnectors/include_c
INCLUDE_DIR_C_U = CxxRTAlib/Utils/include_c


### Output
EXE_DESTDIR = bin

LIB_DESTDIR = lib
LIB_NAME = libCxxRTAlib.a

UNITTEST_EXE_DESTDIR = TestEnvironment/bin
UNITTEST_EXE = unitTest


####### 3) Compiler, tools and options
######### C++ COMPILING
CXX = g++

CXXFLAGS = -O3 -funroll-loops -std=c++11 -lpthread

LIBS += -L$(MYSQL_CXX_CNT)/lib64/debug -lmysqlcppconn8

LIBS += -L$(REDIS)/ -lhiredis

LIBS += -L$(CTA_ALGORITHMS)/lib -lCTAAlgorithms

LIBS += -L$(CTA_UTILS)/lib -lCTAUtils

CXXFLAGS += -I$(INCLUDE_DIR_CXX_DBC) -I$(INCLUDE_DIR_CXX_DM) -I$(INCLUDE_DIR_CXX_RTA) -I$(INCLUDE_DIR_CXX_U) -I$(INCLUDE_DIR_C_U) -I$(INCLUDE_DIR_C_DBC)

CXXFLAGS += -I$(REDIS)

CXXFLAGS += -I$(CTA_ALGORITHMS)/code

CXXFLAGS += -I$(CTA_UTILS)/code

CXXFLAGS += -I$(MYSQL_CXX_CNT)/include

CXXFLAGS += -I$(BOOST_PATH)

CXXFLAGS += -Wl,-Bdynamic

CXXFLAGS += -I/usr/local/include/cppconn


######### C COMPILING
C = gcc

CFLAGS = -std=c99 -I$(INCLUDE_DIR_C_DBC) -I$(INCLUDE_DIR_C_U)

CFLAGS += -I$(REDIS)

LIBS += -L$(REDIS) -lhiredis



################# UNIT TESTING
# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/ \
								-I$(GTEST_DIR)/include/gtest/internal/*.h

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
GTEST_SYSTEM += -isystem $(GTEST_DIR)/include

CXXFLAGS += $(GTEST_SYSTEM)

CXXFLAGS += -I$(GTEST_HEADERS)

LIBS += -L$(GTEST_DIR)/lib64 -lgtest


############ 4) DEBUG CONFIGURATION
ifeq ($(DEBUG),1)
	CXXFLAGS += -DDEBUG
endif
ifeq ($(VALGRIND),1)
	CXXFLAGS += -g -O0
endif




#######  5) VPATH
VPATH=$(SOURCE_DIR_CXX_DBC):$(INCLUDE_DIR_CXX_DBC):$(SOURCE_DIR_CXX_DM):$(INCLUDE_DIR_CXX_DM):$(SOURCE_DIR_CXX_RTA):$(INCLUDE_DIR_CXX_RTA):$(SOURCE_DIR_CXX_U):$(INCLUDE_DIR_CXX_U):$(SOURCE_DIR_C_DBC):$(INCLUDE_DIR_C_DBC):$(SOURCE_DIR_C_U):$(INCLUDE_DIR_C_U)
vpath %.o $(OBJECTS_DIR)

INCLUDE_CXX=$(foreach dir,$(INCLUDE_DIR_CXX_DBC), $(wildcard $(dir)/*))
INCLUDE_CXX+=$(foreach dir,$(INCLUDE_DIR_CXX_DM), $(wildcard $(dir)/*))
INCLUDE_CXX+=$(foreach dir,$(INCLUDE_DIR_CXX_RTA), $(wildcard $(dir)/*))
INCLUDE_CXX+=$(foreach dir,$(INCLUDE_DIR_CXX_U), $(wildcard $(dir)/*))

SOURCE_CXX=$(foreach dir,$(SOURCE_DIR_CXX_DBC), $(wildcard $(dir)/*.cpp))
SOURCE_CXX+=$(foreach dir,$(SOURCE_DIR_CXX_DM), $(wildcard $(dir)/*.cpp))
SOURCE_CXX+=$(foreach dir,$(SOURCE_DIR_CXX_RTA), $(wildcard $(dir)/*.cpp))
SOURCE_CXX+=$(foreach dir,$(SOURCE_DIR_CXX_U), $(wildcard $(dir)/*.cpp))

OBJECTS=$(addsuffix .o, $(basename $(notdir $(SOURCE_CXX))))


# INI PARSER
INCLUDE_C=$(foreach dir,$(INCLUDE_DIR_C_DBC), $(wildcard $(dir)/*.h))
INCLUDE_C+=$(foreach dir,$(INCLUDE_DIR_C_U), $(wildcard $(dir)/*.h))

SOURCE_C=$(foreach dir,$(SOURCE_DIR_C_DBC), $(wildcard $(dir)/*.c))
SOURCE_C+=$(foreach dir,$(SOURCE_DIR_C_U), $(wildcard $(dir)/*.c))

OBJECTS_C=$(addsuffix .o, $(basename $(notdir $(SOURCE_C))))

#$(info $$INCLUDE_CXX is [${INCLUDE_CXX}])
#$(info $$SOURCE_CXX is [${SOURCE_CXX}])

#$(info $$INCLUDE_C is [${INCLUDE_C}])
#$(info $$SOURCE_C is [${SOURCE_C}])

$(info $$OBJECTS is [${OBJECTS}])
$(info $$OBJECTS_C is [${OBJECTS_C}])

# Pattern rule
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(OBJECTS_DIR)/$@

%.o : %.c
	$(C) $(CFLAGS) -c $< -o $(OBJECTS_DIR_C)/$@


all: staticlib unittest

#exe: makeobjdir $(OBJECTS_C) $(OBJECTS)
	#test -d $(EXE_DESTDIR) || mkdir -p $(EXE_DESTDIR)
	#$(CXX) $(CXXFLAGS) -o $(EXE_DESTDIR)/$(EXE) $(OBJECTS_DIR)/*.o $(LIBS) $(OBJECTS_DIR_C)/*.o
	#$(C) $(CFLAGS) -o $(EXE_DESTDIR)/$(EXE) $(LIBS) $(OBJECTS_DIR_C)/*.o

makeobjdir:
	test -d $(OBJECTS_DIR) || mkdir -p $(OBJECTS_DIR)
	test -d $(OBJECTS_DIR_C) || mkdir -p $(OBJECTS_DIR_C)

makelibdir:
	test -d $(LIB_DESTDIR) || mkdir -p $(LIB_DESTDIR)

staticlib: makelibdir makeobjdir $(OBJECTS_C) $(OBJECTS)
		test -d $(LIB_DESTDIR) || mkdir -p $(LIB_DESTDIR)
		$(AR) $(LIB_DESTDIR)/$(LIB_NAME) $(OBJECTS_DIR)/*.o $(OBJECTS_DIR_C)/*.o

test: unittest

unittest:
	test -d $(UNITTEST_EXE_DESTDIR) || mkdir -p $(UNITTEST_EXE_DESTDIR)
	$(CXX) $(SOURCE_DIR_CXX_UNIT_TEST) $(CXXFLAGS) -o $(UNITTEST_EXE_DESTDIR)/$(UNITTEST_EXE) $(OBJECTS_DIR)/*.o  $(OBJECTS_DIR_C)/*.o $(LIBS)



#clean: delete all files from the current directory that are normally created by building the program.
clean:
	$(DEL_FILE) $(OBJECTS_DIR)/*.o
	$(DEL_FILE) $(OBJECTS_DIR_C)/*.o
	$(DEL_FILE) $(LIB_DESTDIR)/*
	$(DEL_FILE) $(UNITTEST_EXE_DESTDIR)/*
