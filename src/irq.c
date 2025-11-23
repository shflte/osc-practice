#include "peripherals/irq.h"
#include "irq.h"
#include "utils.h"

void irq_enable(unsigned int irq_number) {
    if (irq_number < 32) {
        put32(ENABLE_IRQS_1, (1 << irq_number));
    } else if (irq_number < 64) {
        put32(ENABLE_IRQS_2, (1 << (irq_number - 32)));
    } else {
        // Basic IRQs
    }
}
