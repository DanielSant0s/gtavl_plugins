.phony: clean

#----------------------------#
# Directories
#----------------------------#

OBJDIR := build
RELDIR := release

INCDIR := include
SRCDIR := . fourier libspectrice

#----------------------------#
# Cross-compilation, compile flags
#----------------------------#

# Alternatively, try "-march=native" for ARCHFLAGS
#ARCHCROSS := i686-pc-cygwin-
ARCHFLAGS :=

CCFLAGS := $(ARCHFLAGS) -fno-math-errno -O2 -Wall -Wextra $(foreach dir, $(INCDIR), -I$(dir))
LDFLAGS := -static

#----------------------------#
# Tools
#----------------------------#

CC := $(ARCHCROSS)gcc
LD := $(ARCHCROSS)gcc

#----------------------------#
# Files
#----------------------------#

SRC := $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
OBJ := $(addprefix $(OBJDIR)/, $(notdir $(SRC:.c=.o)))
EXE := wav2vag.exe # Change this for other platforms

VPATH := $(SRCDIR)

#----------------------------#
# General rules
#----------------------------#

$(OBJDIR)/%.o : %.c
	@echo $(notdir $<)
	@$(CC) $(CCFLAGS) -c -o $@ $<

#----------------------------#
# make all
#----------------------------#

all : $(EXE)

$(OBJDIR) $(RELDIR) :; mkdir -p $@
$(OBJ) : $(SRC) $(OBJDIR)
$(EXE) : $(OBJ) 
	$(LD) -o $(RELDIR)/$@ $^ $(LDFLAGS)

#----------------------------#
# make clean
#----------------------------#

clean :; rm -rf $(OBJDIR) $(RELDIR)

#----------------------------#
