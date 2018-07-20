
SHELL = /bin/sh

####### 1) Project names and system
SYSTEM= $(shell gcc -dumpmachine)

# Applications
LIB_NAME = libRTA.a

####### 2) Directories for the compiler
OBJECTS_DIR = obj
SOURCE_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib

####### 3) Compiler, tools and options

CXX = g++

CXXFLAGS = -std=c++11 -g -O2 -pipe -I $(INCLUDE_DIR)


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
all: staticlib

staticlib: clean makelibdir makeobjdir $(OBJECTS)
	test -d $(LIB_DIR) || mkdir -p $(LIB_DIR)
	$(AR) $(LIB_DIR)/$(LIB_NAME) $(OBJECTS_DIR)/*.o

makeobjdir:
	test -d $(OBJECTS_DIR) || mkdir -p $(OBJECTS_DIR)

makelibdir:
	test -d $(LIB_DIR) || mkdir -p $(LIB_DIR)

#clean: delete all files from the current directory that are normally created by building the program.
clean:
	$(DEL_FILE) $(LIB_DIR)/$(LIB_NAME)
	$(DEL_FILE) $(OBJECTS_DIR)/*.o


#install: compile the program and copy the executables, libraries,
#and so on to the file names where they should reside for actual use.
install:

	# For conf files installation
	$(COPY_FILE) $(INCLUDE_DIR)/* ./../dl2Sim/include/

	test -d ./../dl2Sim/lib || mkdir -p ./../dl2Sim/lib
	$(COPY_FILE) $(LIB_DIR)/$(LIB_NAME) ./../dl2Sim/lib


#uninstall: delete all the installed files--the copies that the `install' target creates.
uninstall:
	# For exe uninstall
	$(DEL_FILE) ./../dl2Sim/lib/$(LIB_NAME)
