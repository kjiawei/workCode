.global _start
.global IRQ_handle

_start:
	ldr	sp, =0x40000000		

	mov r0, #0x53			
	msr CPSR, r0	

	@bl clock_init	
	b main	

IRQ_handle:
	ldr sp, =0xD0037F80
	sub lr, lr, #4				
	stmfd sp!, {r0-r12, lr}
	bl	Irq_Isr		
	ldmfd sp!, {r0-r12, pc}^

