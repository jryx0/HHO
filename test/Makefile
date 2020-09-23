########################################################################
####################### Makefile Template ##############################
########################################################################

# Makefile settings - Can be customized.
TESTAPPNAME = $(APPNAME)_$(TEST)
#EXT = .c
#SRCDIR = .
#OBJDIR = obj

############## Do not change anything from here downwards! #############

SRC = $(wildcard $(SRCDIR)/*$(EXT)) \
				$(wildcard $(SHARESRC)/*$(EXT)) \
				$(wildcard $(SHARELISTSRC)/*$(EXT)) \
				$(wildcard $(SHAREPINYINSRC)/*$(EXT))   #添加src查找目录
SRCFILE= $(notdir $(SRC)) 
SRCFILE= $(notdir $(SRC))
OBJ = $(SRCFILE:%$(EXT)=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=$(OBJDIR)/%.d)

# UNIX-based OS variables & settings
RM = -rm
DELOBJ = $(OBJ)
# Windows OS variables & settings
DEL = del
EXE = .exe
WDELOBJ = $(subst /,\,$(OBJ))
WDELDEP = $(subst /,\,$(DEP))
WDELAPP = $(subst /,\, $(OUTPUTDIR)/$(TESTAPPNAME).exe)

$(SHOW):	 
	@echo ------------------$(PATIENT)---------------------------------
	@echo SRC = $(wildcard $(SRC)) 
	@echo OBJ = $(OBJ)
	@echo DEP = $(DEP)
	@echo WDELOBJ = $(WDELOBJ)
	@echo WDELEDP = $(WDELDEP)
	@echo WDELAPP = $(subst /,\, $(OUTPUTDIR)/$(TESTAPPNAME).exe)

$(BUILD): $(TESTAPPNAME)
# Builds the app
$(TESTAPPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $(OUTPUTDIR)/$@ $^ $(LDFLAGS)

# Creates the dependecy rules
$(OBJDIR)/%.d: $(wildcard $(SRC)) 
	@$(CC) $(CFLAGS)  -I $(INCDIR) $< -MM -MT $(@:%.d=%.o) > $@

# Includes all .h files
-include $(DEP)


# Building rule for .o files and its .c/.cpp in combination with all .h
#share/
$(OBJDIR)/%.o: $(SHARESRC)/%$(EXT)
	$(CC) $(CXXFLAGS)  -I $(INCDIR) -o $@ -c $<
#share/list
$(OBJDIR)/%.o: $(SHARELISTSRC)/%$(EXT)
	$(CC) $(CXXFLAGS)  -I $(INCDIR) -o $@ -c $<

#share/pinyin
$(OBJDIR)/%.o: $(SHAREPINYINSRC)/%$(EXT)
	$(CC) $(CXXFLAGS)  -I $(INCDIR) -o $@ -c $<

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -I $(INCDIR) -o $@ -c $<

################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: $(CLEANW) $(BUILD) $(SHOW)  $(CLEAN)
clean:
	$(RM) $(DELOBJ) $(DEP) $(OUTPUTDIR)/*.exe
	$(RM) $(OBJDIR)/*.OBJ  $(OBJDIR)/*.exe
	@echo

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP)

#################### Cleaning rules for Windows OS #####################
# Cleans complete project
.PHONY: cleanw
cleanw:
	$(DEL) $(WDELOBJ) $(WDELDEP) 
	$(DEL) $(WDELAPP)
	$(DEL) $(OBJDIR)\*.OBJ  $(OBJDIR)\*.EXE

# Cleans only all files with the extension .d
.PHONY: cleandepw
cleandepw:
	$(DEL) $(DEP)
 