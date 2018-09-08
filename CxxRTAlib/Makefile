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

####### 3) Compiler, tools and options

CXX = g++


MYSQL_INC=`mysql_config --cflags`

MYSQL_LIB=`mysql_config --libs`

CXXFLAGS = -O3 -funroll-loops -std=c++11 -I $(INCLUDE_DIR) $(MYSQL_INC) -L $(MYSQL_LIB)

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
VPATH=$(SOURCE_DIR):$(INCLUDE_DIR):
vpath %.o $(OBJECTS_DIR)

INCLUDE=$(foreach dir,$(INCLUDE_DIR), $(wildcard $(dir)/*.h))
SOURCE=$(foreach dir,$(SOURCE_DIR), $(wildcard $(dir)/*.cpp))
OBJECTS=$(addsuffix .o, $(basename $(notdir $(SOURCE))))

$(info $$INCLUDE is [${INCLUDE}])
$(info $$SOURCE is [${SOURCE}])
$(info $$OBJECTS is [${OBJECTS}])


# Pattern rule
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(OBJECTS_DIR)/$@


#all: compile the entire program.	agextspot performance bayesian
all: exe
	#only if conf directory is present:
	#$(SYMLINK) $(CONF_DIR) $(CONF_DEST_DIR)

exe: makeobjdir $(OBJECTS)
	test -d $(EXE_DESTDIR) || mkdir -p $(EXE_DESTDIR)
	$(CXX) $(CXXFLAGS) -o $(EXE_DESTDIR)/$(EXE) $(OBJECTS_DIR)/*.o $(LIBS)

makeobjdir:
	test -d $(OBJECTS_DIR) || mkdir -p $(OBJECTS_DIR)


#clean: delete all files from the current directory that are normally created by building the program.
clean:
	$(DEL_FILE) $(OBJECTS_DIR)/*.o
	$(DEL_FILE) $(EXE_DESTDIR)/*
