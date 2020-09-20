########################################################################
####################### Makefile Template ##############################
########################################################################

# Makefile settings - Can be customized.
APPNAME = $(TEST)
#EXT = .c
#SRCDIR = .
#OBJDIR = obj

############## Do not change anything from here downwards! #############

SRC = $(wildcard $(SRCDIR)/*$(EXT)) \
				$(wildcard $(SHARESRC)/*$(EXT)) \
				$(wildcard $(SHARELISTSRC)/*$(EXT))
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
WDELOBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)\\%.o)


$(SHOW):	 
	@echo ------------------$(PATIENT)---------------------------------
	@echo SRC = $(wildcard $(SRC)) 
	@echo OBJ = $(OBJ)
	@echo DEP = $(DEP)
	@echo 

$(BUILD): $(APPNAME)
# Builds the app
$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $(OUTPUTDIR)/$@ $^ $(LDFLAGS)

# Creates the dependecy rules
$(OBJDIR)/%.d: $(wildcard $(SRC)) 
	@$(CC) $(CFLAGS)  -I $(INCDIR) $< -MM -MT $(@:%.d=%.o) > $@

# Includes all .h files
-include $(DEP)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SHARESRC)/%$(EXT)
	$(CC) $(CXXFLAGS)  -I $(INCDIR) -o $@ -c $<

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SHARELISTSRC)/%$(EXT)
	$(CC) $(CXXFLAGS)  -I $(INCDIR) -o $@ -c $<

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -I $(INCDIR) -o $@ -c $<

################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: $(CLEAN) $(BUILD) $(SHOW)
clean:
	$(RM) $(DELOBJ) $(DEP) $(OUTPUTDIR)/$(APPNAME)
	$(RM) $(OBJDIR)/*.OBJ  $(OBJDIR)/*.EXE
	@echo

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP)

#################### Cleaning rules for Windows OS #####################
# Cleans complete project
.PHONY: cleanw
cleanw:
	$(DEL) $(WDELOBJ) $(DEP) $(OUTPUTDIR)/$(APPNAME)$(EXE)

# Cleans only all files with the extension .d
.PHONY: cleandepw
cleandepw:
	$(DEL) $(DEP)
 