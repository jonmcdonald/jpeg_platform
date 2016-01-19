.equ MAX_CORES, 4
.equ IRQ_STACK_SIZE, 2*1024
.equ FIQ_STACK_SIZE, 2*1024

.equ USR_MODE, 0x10
.equ SVC_MODE, 0x13
.equ IRQ_MODE, 0x12
.equ FIQ_MODE, 0x11
        
.MACRO proc_SwitchMode mode, stack_pointer
            MRS     R0, CPSR
            MOV     R1, R0
            AND     R1, R1, #0x1F       @ Store the previous mode.
            BIC     R0, R0, #0x1F       @ Clear the mode bits.
            ORR     R0, R0, #\mode      @ Set the new mode bits.
            MSR     CPSR_csfx, R0       @ Set the processor mode.
            MOV     R13, \stack_pointer @ Set the stack.
            BIC     R0, R0, #0x1F       @ Clear the mode bits again.
            ORR     R0, R0, R1          @ Restore the mode.
            MSR     CPSR_csfx, R0       @ Set the processor mode back.
.endm
        
.section .text
.code 32

    .global __cs3_isr_irq
    .global __cs3_isr_fiq
    .global __cs3_reset
        
     .global enable_interrupts
     .type enable_interrupts, %function

     .global disable_interrupts
     .type disable_interrupts, %function

     .global board_init
     .type board_init, %function
                
__cs3_reset:
     B	_start

__cs3_isr_irq:
    STMDB   R13!,{R0-R12,R14}    
    BL do_irq
    LDMIA   R13!,{R0-R12,R14}
    SUBS    PC, R14, #0x4

__cs3_isr_fiq:
    STMDB   R13!,{R0-R12,R14}
    BL do_fiq
    LDMIA   R13!,{R0-R12,R14}
    SUBS    PC, R14, #0x4

    .global get_core_id
    .type get_core_id, %function
get_core_id:
    mrc		p15, 0, r0, c0, c0, 5		@ MPIDR (ARMv7 only)
    and		r0, r0, #15			@ CPU number
    bx		lr

     .global enable_dcache
     .type enable_dcache, %function
enable_dcache:
    MRC      p15, 0, R0, c1, c0, 0 @ Read control register
    LDR      R1, =0x4
    ORR      R0, R0, R1
    MCR      p15, 0, R0, c1, c0, 0 @ Write control register
    bx lr
 
board_init:
    mrc		p15, 0, r2, c0, c0, 5		@ MPIDR (ARMv7 only)
    and		r2, r2, #15			@ CPU number
    lsl		r2, r2, #2
    
    @ Switch the processor to the FIQ mode & set the stack pointer.
    ldr		r3, =fiq_stacks
    ldr		r3, [r3, r2]
    proc_SwitchMode FIQ_MODE, r3

    @ Switch the processor to the IRQ mode & set the stack pointer.
    ldr		r3, =irq_stacks
    ldr		r3, [r3, r2]
    proc_SwitchMode IRQ_MODE, r3

    @ Enable Interrupts
    MRS     R0, CPSR
    BIC     R0, R0, #0xC0
    @ Change the mode.
    MSR     CPSR_csfx, R0

    @ Enable the instruction cache
    MRC      p15, 0, R0, c1, c0, 0 @ Read control register
    LDR      R1, =0x00001000
    ORR      R0, R0, R1
    MCR      p15, 0, R0, c1, c0, 0 @ Write control register

    bx lr
        
enable_interrupts:
    @ Enable Interrupts
    MRS     R0, CPSR
    BIC     R0, R0, #0xC0
    @ Change the mode.
    MSR     CPSR_csfx, R0
    bx lr
                
disable_interrups:
     @ Disable Interrupts
    MRS     R0, CPSR
    ORR     R0, R0, #0xC0
    @ Change the mode.
    MSR     CPSR, R0
    bx lr
        
.data

IRQ_STACK_AREA:
.space IRQ_STACK_SIZE*MAX_CORES
irq_stacks:
	.word IRQ_STACK_AREA + 1*IRQ_STACK_SIZE
	.word IRQ_STACK_AREA + 2*IRQ_STACK_SIZE
	.word IRQ_STACK_AREA + 3*IRQ_STACK_SIZE
	.word IRQ_STACK_AREA + 4*IRQ_STACK_SIZE

FIQ_STACK_AREA:
.space FIQ_STACK_SIZE*MAX_CORES
fiq_stacks:
	.word FIQ_STACK_AREA + 1*FIQ_STACK_SIZE
	.word FIQ_STACK_AREA + 2*FIQ_STACK_SIZE
	.word FIQ_STACK_AREA + 3*FIQ_STACK_SIZE
	.word FIQ_STACK_AREA + 4*FIQ_STACK_SIZE

.end
