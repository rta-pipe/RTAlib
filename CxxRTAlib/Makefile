# ==========================================================================
#
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


####### 1) Project names and system
SYSTEM= $(shell gcc -dumpmachine)

# Applications
EXE = testRTAlib


####### 2) Directories for the compiler

### Object directories
OBJECTS_DIR = obj_cpp
OBJECTS_DIR_C = obj_c

#### Source Cxx directories
SOURCE_DIR_CXX_DBC = DBConnectors/src_cpp
SOURCE_DIR_CXX_DM = DataModels/src_cpp
SOURCE_DIR_CXX_RTA = RTAInterface/src_cpp
SOURCE_DIR_CXX_U = Utils/src_cpp

### Include Cxx directories
INCLUDE_DIR_CXX_DBC = DBConnectors/include_cpp
INCLUDE_DIR_CXX_DM = DataModels/include_cpp
INCLUDE_DIR_CXX_RTA = RTAInterface/include_cpp
INCLUDE_DIR_CXX_U = Utils/include_cpp


#### Source C directories
SOURCE_DIR_C_DBC = DBConnectors/src_c
SOURCE_DIR_C_U = Utils/src_c

### Include C directories
INCLUDE_DIR_C_DBC = DBConnectors/include_c
INCLUDE_DIR_C_U = Utils/include_c



PROJECTS_DIR = projects
EXE_DESTDIR  = bin
LIB_DESTDIR = lib

####### 3) Compiler, tools and options

CXX = g++

CXXFLAGS = -O3 -funroll-loops -std=c++11 -lpthread

LIBS += -L$(MYSQL_CXX_CNT)/lib64 -lmysqlcppconn	#NEW ADDED

LIBS += -L$(REDIS)/ -lhiredis	# NEW NEW NEW ADDED

LIBS += -L$(CTA_ALGORITHMS)/lib -lCTAAlgorithms

LIBS += -L$(CTA_UTILS)/lib -lCTAUtils

CXXFLAGS += -I$(INCLUDE_DIR_CXX_DBC) -I$(INCLUDE_DIR_CXX_DM) -I$(INCLUDE_DIR_CXX_RTA) -I$(INCLUDE_DIR_CXX_U) -I$(INCLUDE_DIR_C_U) -I$(INCLUDE_DIR_C_DBC)

CXXFLAGS += -I$(REDIS)

CXXFLAGS += -I$(CTA_ALGORITHMS)/code

CXXFLAGS += -I$(CTA_UTILS)/code

CXXFLAGS += -I$(MYSQL_CXX_CNT)/include	#NEW ADDED

CXXFLAGS += -I$(BOOST_PATH)	#NEW ADDED

CXXFLAGS += -Wl,-Bdynamic	#NEW ADDED

CXXFLAGS += -I/usr/local/include/cppconn	#NEW ADDED


######### C COMPILING
C = gcc

CFLAGS = -std=c99 -I$(INCLUDE_DIR_C_DBC) -I$(INCLUDE_DIR_C_U)

CFLAGS += -I$(REDIS)

LIBS += -L$(REDIS) -lhiredis


ifeq ($(DEBUG),1)
	CXXFLAGS += -DDEBUG
endif
ifeq ($(VALGRIND),1)
	CXXFLAGS += -g -O0
endif


AR       = ar cqs
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f -r
COPY_FILE= $(COPY) -p
COPY_DIR = $(COPY) -pR
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rm -rf
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p	# -p


#######  4) VPATH
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

$(info $$INCLUDE_CXX is [${INCLUDE_CXX}])
$(info $$SOURCE_CXX is [${SOURCE_CXX}])
$(info $$OBJECTS is [${OBJECTS}])

$(info $$INCLUDE_C is [${INCLUDE_C}])
$(info $$SOURCE_C is [${SOURCE_C}])
$(info $$OBJECTS_C is [${OBJECTS_C}])

# Pattern rule
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(OBJECTS_DIR)/$@

%.o : %.c
	$(C) $(CFLAGS) -c $< -o $(OBJECTS_DIR_C)/$@


all: exe	#staticlib
	#only if conf directory is present:
	#$(SYMLINK) $(CONF_DIR) $(CONF_DEST_DIR)

exe: makeobjdir $(OBJECTS_C) $(OBJECTS)
	test -d $(EXE_DESTDIR) || mkdir -p $(EXE_DESTDIR)
	$(CXX) $(CXXFLAGS) -o $(EXE_DESTDIR)/$(EXE) $(OBJECTS_DIR)/*.o $(LIBS) $(OBJECTS_DIR_C)/*.o
	# $(C) $(CFLAGS) -o $(EXE_DESTDIR)/$(EXE) $(LIBS) $(OBJECTS_DIR_C)/*.o

makeobjdir:
	test -d $(OBJECTS_DIR) || mkdir -p $(OBJECTS_DIR)
	test -d $(OBJECTS_DIR_C) || mkdir -p $(OBJECTS_DIR_C)

staticlib: makelibdir makeobjdir $(OBJECTS_DIR) $(OBJECTS_DIR_C)
		test -d $(LIB_DESTDIR) || mkdir -p $(LIB_DESTDIR)
		$(AR) $(LIB_DESTDIR)/$(LIB_NAME) $(OBJECTS_DIR)/*.o $(OBJECTS_DIR_C)/*.o

makelibdir:
	test -d $(LIB_DESTDIR) || mkdir -p $(LIB_DESTDIR)



#clean: delete all files from the current directory that are normally created by building the program.
clean:
	$(DEL_FILE) $(OBJECTS_DIR)/*.o
	# $(DEL_FILE) $(OBJECTS_DIR_C)/*.o
	$(DEL_FILE) $(LIB_DESTDIR)/$(LIB_NAME)
	$(DEL_FILE) $(EXE_DESTDIR)/*
