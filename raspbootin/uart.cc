/* uart.cc - UART initialization & communication */
/* Copyright (C) 2013 Goswin von Brederlow <goswin-v-b@web.de>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/* Reference material:
 * http://www.raspberrypi.org/wp-content/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
 * Chapter 13: UART
 */

#include <stdint.h>
#include "uart.h"

#define MMIO_BASE       0x3F000000

#define GPFSEL0         ((volatile unsigned int*)(MMIO_BASE + 0x00200000))
#define GPFSEL1         ((volatile unsigned int*)(MMIO_BASE + 0x00200004))
#define GPFSEL2         ((volatile unsigned int*)(MMIO_BASE + 0x00200008))
#define GPFSEL3         ((volatile unsigned int*)(MMIO_BASE + 0x0020000C))
#define GPFSEL4         ((volatile unsigned int*)(MMIO_BASE + 0x00200010))
#define GPFSEL5         ((volatile unsigned int*)(MMIO_BASE + 0x00200014))
#define GPSET0          ((volatile unsigned int*)(MMIO_BASE + 0x0020001C))
#define GPSET1          ((volatile unsigned int*)(MMIO_BASE + 0x00200020))
#define GPCLR0          ((volatile unsigned int*)(MMIO_BASE + 0x00200028))
#define GPLEV0          ((volatile unsigned int*)(MMIO_BASE + 0x00200034))
#define GPLEV1          ((volatile unsigned int*)(MMIO_BASE + 0x00200038))
#define GPEDS0          ((volatile unsigned int*)(MMIO_BASE + 0x00200040))
#define GPEDS1          ((volatile unsigned int*)(MMIO_BASE + 0x00200044))
#define GPHEN0          ((volatile unsigned int*)(MMIO_BASE + 0x00200064))
#define GPHEN1          ((volatile unsigned int*)(MMIO_BASE + 0x00200068))
#define GPPUD           ((volatile unsigned int*)(MMIO_BASE + 0x00200094))
#define GPPUDCLK0       ((volatile unsigned int*)(MMIO_BASE + 0x00200098))
#define GPPUDCLK1       ((volatile unsigned int*)(MMIO_BASE + 0x0020009C))

#define AUX_ENABLE      ((volatile unsigned int*)(MMIO_BASE + 0x00215004))
#define AUX_MU_IO       ((volatile unsigned int*)(MMIO_BASE + 0x00215040))
#define AUX_MU_IER      ((volatile unsigned int*)(MMIO_BASE + 0x00215044))
#define AUX_MU_IIR      ((volatile unsigned int*)(MMIO_BASE + 0x00215048))
#define AUX_MU_LCR      ((volatile unsigned int*)(MMIO_BASE + 0x0021504C))
#define AUX_MU_MCR      ((volatile unsigned int*)(MMIO_BASE + 0x00215050))
#define AUX_MU_LSR      ((volatile unsigned int*)(MMIO_BASE + 0x00215054))
#define AUX_MU_MSR      ((volatile unsigned int*)(MMIO_BASE + 0x00215058))
#define AUX_MU_SCRATCH  ((volatile unsigned int*)(MMIO_BASE + 0x0021505C))
#define AUX_MU_CNTL     ((volatile unsigned int*)(MMIO_BASE + 0x00215060))
#define AUX_MU_STAT     ((volatile unsigned int*)(MMIO_BASE + 0x00215064))
#define AUX_MU_BAUD     ((volatile unsigned int*)(MMIO_BASE + 0x00215068))

namespace UART {
    /*
     * Initialize UART1.
     */
    void init(void) {
        register unsigned int r;

        /* initialize UART */
        *AUX_ENABLE |= 1;       // enable UART1, AUX mini uart
        *AUX_MU_IER = 0;
        *AUX_MU_CNTL = 0;
        *AUX_MU_LCR = 3;       // 8 bits
        *AUX_MU_MCR = 0;
        *AUX_MU_IER = 0;
        *AUX_MU_IIR = 0xc6;    // disable interrupts
        *AUX_MU_BAUD = 270;    // 115200 baud
        /* map UART1 to GPIO pins */
        r = *GPFSEL1;
        r &= ~((7 << 12) | (7 << 15)); // gpio14, gpio15
        r |= (2 << 12) | (2 << 15);    // alt5
        *GPFSEL1 = r;
        *GPPUD = 0;            // enable pins 14 and 15
        for (r = 150; r > 0; --r) {
            asm volatile("nop");
        }
        *GPPUDCLK0 = (1 << 14) | (1 << 15);
        for (r = 150; r > 0; --r) {
            asm volatile("nop");
        }
        *GPPUDCLK0 = 0;        // flush GPIO setup
        *AUX_MU_CNTL = 3;      // enable Tx, Rx
    }

    /*
     * Transmit a byte via UART1.
     * uint8_t Byte: byte to send.
     */
    void putc(uint8_t byte) {
        do {
            asm volatile("nop");
        } while (!(*AUX_MU_LSR & 0x20));

        *AUX_MU_IO = byte;
    }

    /*
     * Receive a byte via UART1.
     *
     * Returns:
     * uint8_t: byte received.
     */
    uint8_t getc(void) {
        do {
            asm volatile("nop");
        } while (!(*AUX_MU_LSR & 0x01));

        return (char)(*AUX_MU_IO);
    }

    /*
     * print a string to the UART one character at a time
     * const char *str: 0-terminated string
     */
    void puts(const char *str) {
        while (*str) {
            putc(*str++);
        }
    }
}
