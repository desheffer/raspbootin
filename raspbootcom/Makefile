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

SOURCES := $(wildcard *.cc)

OBJS := $(patsubst %.cc, %.o, $(SOURCES))

CXXFLAGS := -O2 -W -Wall -g

all: raspbootcom

raspbootcom: raspbootcom.o
	$(CXX) -o $@ $+

clean:
	$(RM) -f $(OBJS) raspbootcom

%.o: %.cc Makefile
	g++ $(CXXFLAGS) -c $< -o $@
