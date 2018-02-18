# Makefile - build script */
# Copyright (C) 2013 Goswin von Brederlow <goswin-v-b@web.de>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

TOOLCHAIN ?= aarch64-none-elf

SOURCES_ASM := $(wildcard *.S)
SOURCES_CC  := $(wildcard *.cc)

OBJS := $(patsubst %.S, %.o, $(SOURCES_ASM))
OBJS += $(patsubst %.cc, %.o, $(SOURCES_CC))

BASEFLAGS := -O2 -fpic -nostdlib
BASEFLAGS += -nostartfiles -ffreestanding -nodefaultlibs
BASEFLAGS += -fno-builtin -fomit-frame-pointer -mcpu=cortex-a53
WARNFLAGS := -Wall -Wextra -Wshadow -Wcast-align -Wwrite-strings
WARNFLAGS += -Wredundant-decls -Winline
WARNFLAGS += -Wno-attributes -Wno-deprecated-declarations
WARNFLAGS += -Wno-div-by-zero -Wno-endif-labels -Wfloat-equal
WARNFLAGS += -Wformat=2 -Wno-format-extra-args -Winit-self
WARNFLAGS += -Winvalid-pch -Wmissing-format-attribute
WARNFLAGS += -Wmissing-include-dirs -Wno-multichar
WARNFLAGS += -Wredundant-decls -Wshadow
WARNFLAGS += -Wno-sign-compare -Wswitch -Wsystem-headers
WARNFLAGS += -Wno-pragmas -Wno-unused-but-set-parameter
WARNFLAGS += -Wno-unused-but-set-variable -Wno-unused-result
WARNFLAGS += -Wwrite-strings -Wdisabled-optimization -Wpointer-arith
ASFLAGS   := -D__ASSEMBLY__
CXXFLAGS  := $(BASEFLAGS) $(WARNFLAGS)
CXXFLAGS  += -fno-exceptions -std=gnu++11
LDFLAGS   := $(BASEFLAGS)

all: kernel8.img

kernel8.elf: $(OBJS) link-aarch64-elf.ld
	$(TOOLCHAIN)-g++ $(LDFLAGS) $(OBJS) -lgcc -Tlink-aarch64-elf.ld -o $@

kernel8.img: kernel8.elf
	$(TOOLCHAIN)-objcopy kernel8.elf -O binary kernel8.img

clean:
	$(RM) -f $(OBJS) kernel8.elf kernel8.img

%.o: %.cc Makefile
	$(TOOLCHAIN)-g++ $(CXXFLAGS) -c $< -o $@

%.o: %.S Makefile
	$(TOOLCHAIN)-g++ $(ASFLAGS) -c $< -o $@
