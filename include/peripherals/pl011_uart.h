#ifndef _PERIPHERAL_PL011_UART_H
#define _PERIPHERAL_PL011_UART_H

#include "peripherals/base.h"

#define PU_DR        ((volatile unsigned int *)(MMIO_BASE + 0x00201000))
#define PU_RSRECR    ((volatile unsigned int *)(MMIO_BASE + 0x00201004))
#define PU_FR        ((volatile unsigned int *)(MMIO_BASE + 0x00201018))
#define PU_ILPR      ((volatile unsigned int *)(MMIO_BASE + 0x00201020))
#define PU_IBRD      ((volatile unsigned int *)(MMIO_BASE + 0x00201024))
#define PU_FBRD      ((volatile unsigned int *)(MMIO_BASE + 0x00201028))
#define PU_LCRH      ((volatile unsigned int *)(MMIO_BASE + 0x0020102C))
#define PU_CR        ((volatile unsigned int *)(MMIO_BASE + 0x00201030))
#define PU_IFLS      ((volatile unsigned int *)(MMIO_BASE + 0x00201034))
#define PU_IMSC      ((volatile unsigned int *)(MMIO_BASE + 0x00201038))
#define PU_RIS       ((volatile unsigned int *)(MMIO_BASE + 0x0020103C))
#define PU_MIS       ((volatile unsigned int *)(MMIO_BASE + 0x00201040))
#define PU_ICR       ((volatile unsigned int *)(MMIO_BASE + 0x00201044))
#define PU_DMACR     ((volatile unsigned int *)(MMIO_BASE + 0x00201048))
#define PU_ITCR      ((volatile unsigned int *)(MMIO_BASE + 0x00201080))
#define PU_ITIP      ((volatile unsigned int *)(MMIO_BASE + 0x00201084))
#define PU_ITOP      ((volatile unsigned int *)(MMIO_BASE + 0x00201088))
#define PU_TDR       ((volatile unsigned int *)(MMIO_BASE + 0x0020108C))

#endif
