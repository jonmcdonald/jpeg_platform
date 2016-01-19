
#tool_chain := /home/zamir/apps/CodeSourcery/Sourcery_CodeBench_for_ARM_EABI
#gcc := $(tool_chain)/bin/arm-none-eabi-gcc
gcc := arm-none-eabi-gcc

VARIANT = PAR

# This is the primary target
executable := image-$(VARIANT).elf

c_sources := \
	uart.c \
	menu.c \
	gic.c \
	dhry_1.c \
	dhry_2.c \
	jpeg_program.c \
	retarget.c \
	page_table.c

s_sources := \
	board.s \
	init_mmu.s

S_sources := \
	start.S

cc_flags := -mcpu=cortex-a9 -DMSC_CLOCK -D__CLK_TCK=1000
ld_flags := -Wl,--wrap=_read,--wrap=_write -T qemu-arm-hosted.ld  -T board.ld

build_dir := build-$(VARIANT)

objects := \
	$(addprefix $(build_dir)/,$(subst .c,.o,$(c_sources))) \
	$(addprefix $(build_dir)/,$(subst .s,.o,$(s_sources))) \
	$(addprefix $(build_dir)/,$(subst .S,.o,$(S_sources))) \
	jpeg-6a/libcjpeg.a

compile_command = \
	mkdir -p $(dir $@) ; \
	$(gcc) -DVARIANT_$(VARIANT) -I $(MODEL_BUILDER_HOME)/include -O0 -g -c -MMD -o $@ $< $(cc_flags)

link_command = \
	@mkdir -p $(dir $@) ; \
	$(gcc) -g -o $@ $^ $(cc_flags) $(ld_flags) -lm

$(executable) : $(objects)
	@echo "LD $<"
	$(link_command)

$(build_dir)/%.o : %.c
	@echo "CC $<"
	$(compile_command)

$(build_dir)/%.o : %.s
	@echo "AS $<"
	$(compile_command)

-include $(build_dir)/*.d

clean:
	rm -rf $(build_dir)
	rm -f $(executable)
