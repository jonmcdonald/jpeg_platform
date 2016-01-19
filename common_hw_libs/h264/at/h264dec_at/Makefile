#--------------------------------------------------------------------------
# Global UniChip
# TLM IP Library Makefile
#--------------------------------------------------------------------------
# Source: tlm/*, tlm/ldecod/src/*, tlm_ldecod/inc/*
# Target: tlm/ocb_h264d_top.a
#--------------------------------------------------------------------------

#--------------------------------------------------------------------------
# Make Args
#--------------------------------------------------------------------------
# TLM-defs
include ./Makefile.defs
#SYSCLIB = $(SYSTEMC)/lib-linux/libsystemc.a
#GUCLIB  = $(GUCINC)/guc_amba20_wrap.a

# pre-defs
#CFLAGS  += -D H264D_ENTRY_ENA -D H264D_STDOUT_DIS -D H264D_DPBOUT_DIS
CFLAGS  += -DESLCPP -DSTATIC_ALLOC -DLT_MODEL -DEN_TLM -DT_RGB
CFLAGS  += #-D DEBUG_SHOW #-D VCD_DEBUG


#CC=${COWAREHOME}/gnu/${COWARE_CXX_COMPILER}/bin/g++
#SYSCINC=${COWAREHOME}/common/include/



#--------------------------------------------------------------------------
# Make Paths
#--------------------------------------------------------------------------
# library
#LIBINC  = -I $(GUCINC) -I $(SYSCINC)
#LIB     = $(GUCLIB) $(SYSCLIB)

# object
#OBJINC  = -I . \
#          -I ldecod/inc
OBJINC  = -I ldecod/inc
#OBJ     = $(patsubst %.cpp,obj/%.o,$(wildcard *.cpp)) \
#          $(patsubst ldecod/src/%.cpp,obj/%.o,$(wildcard ldecod/src/*.cpp))
OBJ     = $(patsubst ldecod/src/%.cpp,obj/%.o,$(wildcard ldecod/src/*.cpp))

# target
TAR     = libguc_h264d.a

#--------------------------------------------------------------------------
# Make Rules
#--------------------------------------------------------------------------
# all
all: obj $(TAR)

obj:
	mkdir obj

# target
$(TAR): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

#obj/%.o: %.cpp
#	$(CC) $(CFLAGS) $(OBJINC) $(LIBINC) -c -o $@ $<

obj/%.o: ldecod/src/%.cpp
	$(CC) $(CFLAGS) $(OBJINC) -c -o $@ $<

# new
new: clean all

# clean
clean: clean_obj clean_lib
	rm -rf core.* spsccLog

clean_obj:
	rm -f $(OBJ) *.nsco

clean_lib:
	rm -f $(TAR) *.nsca

