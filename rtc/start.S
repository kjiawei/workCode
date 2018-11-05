.text
.global _start
.global IRQ_handle

_start:
	@diable watchdog
	ldr	r0, =0xE2700000			
	mov	r1, #0
	str	r1, [r0]

	@set sp
	ldr	sp, =0x50000000	

	mov r1, #0x53			
	msr CPSR_cxsf, r1	
	

	@initialize clock		
	bl clock_init	


	@to main
	b main	

IRQ_handle:
	ldr sp, =0xD0037F80
	sub lr, lr, #4				
	stmfd sp!, {r0-r12, lr}
	bl	Irq_Isr		
	ldmfd sp!, {r0-r12, pc}^