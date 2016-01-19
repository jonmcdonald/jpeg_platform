.equ MAX_CORES, 4
.equ STACK_SIZE, 16*1024

	.syntax	unified
	.text

	.globl	_start
_start:
	@
	@ CPU initialisation
	@
	mrc	p15, 0, r0, c0, c0, 5		@ MPIDR (ARMv7 only)
	and	r0, r0, #15			@ CPU number
	lsl	r0, r0, #2
	cmp	r0, #0
	beq	3f

2:
        @ Secondary
	wfe
        ldr	r2, =stacks
        ldr	sp, [r2, r0]
        b main
        
3:
        @ Primary
	ldr	r2, =stacks
	ldr	sp, [r2, r0]
	b	__cs3_start_c

stacks:
	.word STACK_AREA + 1*STACK_SIZE
	.word STACK_AREA + 2*STACK_SIZE
	.word STACK_AREA + 3*STACK_SIZE
	.word STACK_AREA + 4*STACK_SIZE

        .data

STACK_AREA:
.space STACK_SIZE*MAX_CORES
