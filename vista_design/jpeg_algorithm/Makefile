#
# Makefile for jpeg algorithm
#

OBJS              = bitstream.o  bmp.o  huff.o  jpeg.o
ARCHIVE           = libjpeg.a
INCLUDE           = -I../../common_hw_libs/catapult_ac_datatypes
CFLAGS            = -fPIC

SCCOM_FLAGS       = -incr -DSC_INCLUDE_MTI_AC
SC_WORK_SCCOM     = work/_sc/linux_gcc-4.3.4

#######################################
### Calculated and 'stable' variables
#######################################

CC                = vista_c++
SCCOM             = vista_sccom
SHARED_LIB_SCCOM  =  $(SC_WORK_SCCOM)/systemc.so

OBJS_SCCOM        =  $(patsubst %.o,$(SC_WORK_SCCOM)/%.o,$(OBJS))
SC_WORK_SCCOM     := $(patsubst %/,%,$(SC_WORK_SCCOM))
WORK_SCCOM        =  $(patsubst %/,%,$(call stripdir,$(SC_WORK_SCCOM)))
stripdir          =  $(dir $(patsubst %/,%,$(dir $1))) # function to strip the rightmost directory name

ifeq ($(MARCH),32)
    M32_CC        = -m32
    M32_SCCOM     = -32 -m32  # vista_sccom may only recognize -m32
else
    M32_CC        = # blank
    M32_SCCOM     = # blank
endif

#######################
###   Targets
#######################

.PHONY: archive sccom
archive: $(ARCHIVE)
sccom : $(SHARED_LIB_SCCOM)

#-----------------
#    recipes
#-----------------
$(ARCHIVE): $(OBJS)
	ar rcs $@ $^

%.o: %.cpp
	$(CC) $(M32_CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

#-
$(SHARED_LIB_SCCOM): $(OBJS_SCCOM)
	$(SCCOM) $(M32_SCCOM) -linkshared -work $(WORK_SCCOM)

$(OBJS_SCCOM): $(SC_WORK_SCCOM)/%.o : %.cpp | $(WORK_SCCOM)/_info
	$(SCCOM) $(M32_SCCOM) $(SCCOM_FLAGS) $(CFLAGS) $(INCLUDE) -c $<

$(WORK_SCCOM)/_info: 
	vlib $(WORK_SCCOM)

#-
.PHONY: clean cleanall clean_all
clean:
	rm -f *.o
	rm -rf $(SC_WORK_SCCOM)/*.o

cleanall clean_all: clean
	rm -f $(ARCHIVE)
	rm -rf $(WORK_SCCOM)
	rm -rf vista_modelsim.ini


