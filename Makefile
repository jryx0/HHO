########################################################################
#########################    HHO Project  ##############################
########################################################################

export CC = gcc
export CXXFLAGS = -std=c99 -Wall -g

export SHOW = show
export BUILD = build
export CLEANW = cleanw
export CLEAN = clean

#添加新增文件夹
export INCDIR = $(CURDIR)/include
export SHARELISTSRC = $(CURDIR)/Share/List
export SHAREPINYINSRC = $(CURDIR)/Share/Pinyin
export SHARESRC = $(CURDIR)/Share
export APPNAME = HHO

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

ifeq ($(OS),Windows_NT)
	os = windows
else
	os = linux
endif

###############################################################################
$(SHOW):
	@echo this is main makefile
	@echo CC = $(CC)
	@echo os = $(os)
	@echo OS = $(OS)
	@echo shell = $(OSTYPE)
	$(MAKE) $(SHOW) -C $(TEST)  

$(TEST):
	@echo --------------$(TEST) Makefile-----------------------
	$(MAKE) $(BUILD) -C $(TEST)
	
#cmd,powershell
$(CLEANW):
	$(MAKE) $(CLEANW) -C $(TEST)

#bash
$(CLEAN):
	$(MAKE) $(CLEAN) -C $(TEST)

.PHONY: $(CLEANW)   $(SHOW) $(CLEAN) $(TEST)
