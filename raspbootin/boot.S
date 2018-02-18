/* boot.S - assembly startup code */
/* Copyright (C) 2018 Doug Sheffer <desheffer@gmail.com>

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

.section ".text.boot"

.globl _start

// Entry point. Preserve x0, x1, and x2 for kernel_main.
_start:
    // Set up the stack.
    mov sp, #0x80000

    // Relocate from 0x80000 to __start.
    mov x3, #0x80000
    ldr x4, =__start
    ldr x9, =__data_end
loop_relocate:
    // Load from x3 and store at x4.
    ldp x5, x6, [x3], #16
    stp x5, x6, [x4], #16

    // Loop until __data_end.
    cmp x4, x9
    blo loop_relocate

    // Clear out bss.
    ldr x4, =__bss_start
    ldr x9, =__bss_end
    mov x5, #0
    mov x6, #0
loop_clear_bss:
    // Store zero at x4.
    stp x5, x6, [x4], #16

    // Loop until bss_end.
    cmp x4, x9
    blo loop_clear_bss

    // Call kernel_main.
    ldr x3, =kernel_main
    blr x3

halt:
    wfe
    b halt
