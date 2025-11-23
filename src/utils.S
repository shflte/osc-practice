.globl put32
put32:
	str w1, [x0]
	ret

.globl get32
get32:
	ldr w0, [x0]
	ret

.globl delay
delay:
	subs x0, x0, #1
	bne delay
	ret

.global enable_cpu_irq
enable_cpu_irq:
	msr daifclr, #2
	ret

.global disable_cpu_irq
disable_cpu_irq:
	msr daifset, #2
	ret
