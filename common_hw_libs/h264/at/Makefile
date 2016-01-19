include ./Makefile.config
OBJ_NAME = simple_vp

SRCDIR      = ./

INCDIR =    -I./ \
			-I$(SYSTEMC_INC_DIR)\
			-I$(H264_DIR) \
			-I$(H264_DIR)/ldecod/inc \
			-I$(TLM_INC_DIR)

CFLAGS = $(FLAGS) $(INCDIR)

OBJS   =  main.o              \
          obj/init_wrapper.o  \
          obj/sc_h264d_top.o  \
          obj/h264_top.o	

.PHONY: clean all check 
all: announce obj $(OBJ_NAME)
obj:
	mkdir obj
	
announce:
	@if  ( test ! -d $(FLAG_BATCH) ); \
	 	then echo; echo "$(OBJ_NAME):"; echo; \
	 fi
	
check: announce $(OBJ_NAME)
	@$(VALGRIND) ./$(OBJ_NAME) > run.log
	@cat run.log | grep -v "stopped by user" | awk '{if($$0!="") print $$0}' > run_trimmed.log
	@cat ../results/expected.log | grep -v "stopped by user" | awk '{if($$0!="") print $$0}' > ./expected_trimmed.log
	@diff ./run_trimmed.log ./expected_trimmed.log > diff.log 2>&1;\
	if [ -s diff.log ]; then echo "***ERROR:"; cat diff.log; else echo "OK"; fi

run: announce $(OBJ_NAME)
	@.$(OBJ_NAME)

$(OBJ_NAME): $(OBJS)
	$(LD) -o $@ $(OBJS) $(LDFLAGS)

main.o: $(SRCDIR)/main.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

obj/init_wrapper.o: $(SRCDIR)/init_wrapper.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

obj/sc_h264d_top.o: $(SRCDIR)/sc_h264d_top.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

obj/h264_top.o: $(SRCDIR)/h264_top.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f obj/*.o obj/*.log $(OBJ_NAME) .bin/$(OBJ_NAME) *~ core.*

# DO NOT DELETE

