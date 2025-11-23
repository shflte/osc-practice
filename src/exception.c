#include "peripherals/irq.h"
#include "peripherals/timer.h"
#include "exception.h"
#include "irq.h"
#include "timer.h"
#include "uart.h"
#include "utils.h"

void el1_sync_router(trap_frame_t* regs) {
    unsigned long esr;
    asm volatile("mrs %0, esr_el1" : "=r"(esr));
}

void el0_sync_router(trap_frame_t* regs) {
    unsigned long spsr, elr, esr;

    asm volatile("mrs %0, spsr_el1" : "=r"(spsr));
    asm volatile("mrs %0, elr_el1" : "=r"(elr));
    asm volatile("mrs %0, esr_el1" : "=r"(esr));

    uart_send_f("Exception detected!\n");
    uart_send_f("SPSR_EL1: 0x%x\n", spsr);
    uart_send_f("ELR_EL1:  0x%x\n", elr);
    uart_send_f("ESR_EL1:  0x%x\n", esr);
}

void irq_router(trap_frame_t* regs) {
    unsigned long core_irq, irq_pend1;

    core_irq = get32(CORE0_INTERRUPT_SOURCE);
    irq_pend1 = get32(IRQ_PENDING_1);

    if (core_irq & (1 << 1)) {
        timer_handler(regs);
    } else if (irq_pend1 & (1 << AUX_IRQ)) {
        uart_handler(regs);
    }
}
