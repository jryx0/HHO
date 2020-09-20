 ########################################################################
#########################    HHO Project  ##############################
########################################################################

export CC = gcc
export CXXFLAGS = -std=c99 -Wall -g  

export SHOW = show
export BUILD = build
export CLEAN = clean

export INCDIR = $(CURDIR)/include
export SHARELISTSRC = $(CURDIR)/Share/List
export SHARESRC = $(CURDIR)/Share
export EXT = .c
export SRCDIR = .
export OBJDIR = obj
export APPPATH = $(CURDIR)/
export OUTPUTDIR = $(CURDIR)/build

export PATIENT = patient
export DOCTOR = doctor
export TEST = test

MAKE = mingw32-make

# UNIX-based OS variables & settings
RM = rm
# Windows OS variables & settings
DEL = del
EXE = .exe


###############################################################################
$(SHOW):
	@echo this is main makefile
	@echo CC = $(CC)
	$(MAKE) $(SHOW) -C $(TEST)  

$(BUILD):
	@echo --------------$(TEST) Makefile-----------------------
	$(MAKE) $(BUILD) -C $(TEST)
	

$(CLEAN):
	$(MAKE) $(CLEAN) -C $(TEST)

.PHONY: $(CLEAN) $(BUILD) $(SHOW)
