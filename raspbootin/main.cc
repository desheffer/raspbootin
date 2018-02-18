/* main.cc - the entry point for the kernel */
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

#include <stdint.h>
#include "uart.h"

extern "C" {
    // kernel_main gets called from boot.S. Declaring it extern "C" avoid
    // having to deal with the C++ name mangling.
    void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags);
}

#define LOADER_ADDR 0x2000000

const char hello[] = "\r\n*** system booting ***\r\n";
const char halting[] = "\r\n*** system halting ***\r\n";

typedef void (*entry_fn)(uint32_t r0, uint32_t r1, uint32_t atags);

// kernel main function, it all begins here
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    UART::init();
    UART::puts(hello);

    // request kernel by sending 3 breaks
    UART::puts("\x03\x03\x03");

    // wait for start character
    uint8_t start;
    do {
        start = UART::getc();
    } while (start != '\x03');

    // get kernel size
    uint32_t size = UART::getc();
    size |= UART::getc() << 8;
    size |= UART::getc() << 16;
    size |= UART::getc() << 24;

    if (0x80000 + size > LOADER_ADDR) {
        UART::puts("SE");
        return;
    } else {
        UART::puts("OK");
    }

    uint8_t *kernel = (uint8_t*)0x80000;
    entry_fn fn = (entry_fn)kernel;

    // get kernel
    while (size-- > 0) {
        *kernel++ = UART::getc();
    }

    // call the kernel
    fn(r0, r1, atags);

    // say goodbye and halt
    UART::puts(halting);
}
