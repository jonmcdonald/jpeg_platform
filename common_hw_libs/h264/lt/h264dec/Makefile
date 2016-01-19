#--------------------------------------------------------------------------
# Global UniChip
# TLM IP Library Makefile
#--------------------------------------------------------------------------
# Source: tlm/*, tlm/ldecod/src/*, tlm_ldecod/inc/*
# Target: tlm/ocb_h264d_top.a
# MB: Modified for ESL to RTL flow 
#--------------------------------------------------------------------------

#--------------------------------------------------------------------------
# Make Args
#--------------------------------------------------------------------------
# TLM-defs
include ./Makefile.defs

CFLAGS  += -DESLCPP -DSTATIC_ALLOC -DLT_MODEL -DEN_TLM -DT_RGB
CFLAGS  += $(GCC_SC_MVAR)



#--------------------------------------------------------------------------
# Make Paths
#--------------------------------------------------------------------------

OBJDIR  = obj
SRCDIR  = ldecod/src
OBJINC  = -I ldecod/inc
TAR     = libguc_h264d.a

OBJ     = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.cpp))

ifeq ($(MARCH),32)
    M32_CC   = -m32
else
    M32_CC   = # blank
endif

#--------------------------------------------------------------------------
# Make Rules
#--------------------------------------------------------------------------

all: $(TAR)

$(TAR): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(M32_CC) $(CFLAGS) $(OBJINC) -c $< -o $@ 

$(OBJDIR):
	mkdir -p $(OBJDIR)


.PHONY: clean cleanall clean_all
clean: 
	rm -rf core.* spsccLog
	rm -rf $(OBJDIR) *.nsco

cleanall clean_all: clean
	rm -f $(TAR) *.nsca

