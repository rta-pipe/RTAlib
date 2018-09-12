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

ifndef MYSQL_CXX_CNT
$(error MYSQL_CXX_CNT is not set.)
endif
ifndef BOOST_PATH
$(error BOOST_PATH is not set.)
endif


####### 1) Project names and system
SYSTEM= $(shell gcc -dumpmachine)

# Applications
EXE = testCXXRTAlib

####### 2) Directories for the compiler
OBJECTS_DIR = obj
SOURCE_DIR = src
INCLUDE_DIR = include
PROJECTS_DIR = projects
EXE_DESTDIR  = bin
LIB_DESTDIR = lib

OBJECTS_DIR_C = obj_c
SOURCE_DIR_C = src_c
INCLUDE_DIR_C = include_c

####### 3) Compiler, tools and options

CXX = g++

CXXFLAGS = -O3 -funroll-loops -std=c++11 -I $(INCLUDE_DIR) -I $(INCLUDE_DIR_C)

LIBS += -L$(MYSQL_CXX_CNT)/lib64 -lmysqlcppconn	#NEW ADDED

CXXFLAGS += -I$(MYSQL_CXX_CNT)/include	#NEW ADDED

CXXFLAGS += -I$(BOOST_PATH)	#NEW ADDED

CXXFLAGS += -Wl,-Bdynamic	#NEW ADDED

CXXFLAGS += -I/usr/local/include/cppconn	#NEW ADDED


######### C COMPILING
C = gcc
CFLAGS = -std=c99 -I $(INCLUDE_DIR_C)


ifeq ($(DEBUG),1)
	CXXFLAGS += -DDEBUG
endif

ifeq ($(MULTITHREAD),1)
	CXXFLAGS += -DMULTITHREAD
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
VPATH=$(SOURCE_DIR):$(INCLUDE_DIR):$(SOURCE_DIR_C):$(INCLUDE_DIR_C)
vpath %.o $(OBJECTS_DIR)

INCLUDE=$(foreach dir,$(INCLUDE_DIR), $(wildcard $(dir)/*))
SOURCE=$(foreach dir,$(SOURCE_DIR), $(wildcard $(dir)/*.cpp))
OBJECTS=$(addsuffix .o, $(basename $(notdir $(SOURCE))))


# INI PARSER
INCLUDE_C=$(foreach dir,$(INCLUDE_DIR_C), $(wildcard $(dir)/*.h))
SOURCE_C=$(foreach dir,$(SOURCE_DIR_C), $(wildcard $(dir)/*.c))
OBJECTS_C=$(addsuffix .o, $(basename $(notdir $(SOURCE_C))))

$(info $$INCLUDE is [${INCLUDE}])
$(info $$SOURCE is [${SOURCE}])
$(info $$OBJECTS is [${OBJECTS}])

$(info $$INCLUDE_C is [${INCLUDE_C}])
$(info $$SOURCE_C is [${SOURCE_C}])
$(info $$OBJECTS_C is [${OBJECTS_C}])

# Pattern rule
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(OBJECTS_DIR)/$@

%.o : %.c
	$(C) $(CFLAGS) -c $< -o $(OBJECTS_DIR_C)/$@

#all: compile the entire program.	agextspot performance bayesian
all: exe
	#only if conf directory is present:
	#$(SYMLINK) $(CONF_DIR) $(CONF_DEST_DIR)

exe: makeobjdir $(OBJECTS_C) $(OBJECTS)




	test -d $(EXE_DESTDIR) || mkdir -p $(EXE_DESTDIR)
	$(CXX) $(CXXFLAGS) -o $(EXE_DESTDIR)/$(EXE) $(OBJECTS_DIR)/*.o $(LIBS) $(OBJECTS_DIR_C)/*.o

makeobjdir:
	test -d $(OBJECTS_DIR) || mkdir -p $(OBJECTS_DIR)


#clean: delete all files from the current directory that are normally created by building the program.
clean:
	$(DEL_FILE) $(OBJECTS_DIR)/*.o
	$(DEL_FILE) $(EXE_DESTDIR)/*
