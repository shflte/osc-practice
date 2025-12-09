#include "peripherals/timer.h"
#include "mm.h"
#include "exception.h"
#include "utils.h"
#include "timer.h"

timer_node_t* timer_list = 0;

void timer_init(void) {
    asm volatile("msr cntp_ctl_el0, %0" :: "r"(1));
}

void add_timer(timer_cb_t callback, void* arg, unsigned int expire) {
    disable_cpu_irq();
    unsigned long cntpct, freq;

    asm volatile("mrs %0, cntpct_el0" : "=r"(cntpct));
    asm volatile("mrs %0, cntfrq_el0" : "=r"(freq));

    timer_node_t* new_node = startup_malloc(sizeof(timer_node_t));
    new_node->callback = callback;
    new_node->arg = arg;
    new_node->expire = cntpct + expire * freq;

    timer_node_t** indirect = &timer_list;
    while (*indirect && (*indirect)->expire < new_node->expire) {
        indirect = &((*indirect)->next);
    }

    new_node->next = *indirect;
    *indirect = new_node;

    asm volatile("msr cntp_cval_el0, %0" :: "r"(timer_list->expire));
    put32(CORE0_TIMER_IRQ_CTL, 2);
    enable_cpu_irq();
}

void timer_handler(trap_frame_t* regs) {
    timer_list->callback(timer_list->arg);
    timer_list = timer_list->next;
    if (!timer_list) put32(CORE0_TIMER_IRQ_CTL, 0);
    else asm volatile("msr cntp_cval_el0, %0" :: "r"(timer_list->expire));
}
