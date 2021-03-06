# 
# 	Copyright 2012 Torbjorn Tyridal <ttyridal@gmail.com>
#
#	This file is part of simavr.
#
#	simavr is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	simavr is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with simavr.  If not, see <http://www.gnu.org/licenses/>.

target=	simusb
firmware_src = ./output/test.ino
firmware = ./output/test.ino.hex
simavr = ../../

arduino_builder = ./arduino-1.8.5/arduino-builder

IPATH = .
IPATH += ../parts
IPATH += ../vhci/include
IPATH += ../vhci
IPATH += ${simavr}/include
IPATH += ${simavr}/simavr/sim
#IPATH += /usr/local/include

VPATH = .
VPATH += ../parts

LDFLAGS += -lpthread -lusb_vhci -L../vhci/lib

all: obj ${firmware} ${target}

include ${simavr}/Makefile.common

board = ${OBJ}/${target}.elf

${board} : ${OBJ}/vhci_usb.o
${board} : ${OBJ}/${target}.o

../vhci/lib/libusb_vhci.a:
	@echo -n "BUILDIN $@ "
	@{ export CC=;export CFLAGS=; $(MAKE) -j -C ../vhci >/tmp/vhci.build.log 2>&1 || \
		{ echo "ERROR check /tmp/vhci.build.log"; exit 1; }; } && \
			echo " Done" 

vhci : ../vhci/lib/libusb_vhci.a 

${arduino_builder}:
	@echo please download arduino-1.8.5 from https://www.arduino.cc/en/Main/Software
	exit 1

${firmware}: ${arduino_builder}
	./arduino-1.8.5/arduino-builder -build-options-file ./build.options.json -build-path ${CURDIR}/output test/test.ino

modules:
	-sudo insmod ../vhci/vhci-hcd-1.14/usb-vhci-hcd.ko
	-sudo insmod ../vhci/vhci-hcd-1.14/usb-vhci-iocifc.ko
	sleep 1

rmmodules:
	-sudo rmmod usb-vhci-iocifc
	-sudo rmmod usb-vhci-hcd

test: modules
	@echo "-----------------------------------"
	@echo "start something listening on the new serial port: miniterm.py /dev/ttyACM0 115200"
	sudo ${board}

clean-vhci:
	$(MAKE) -C ../vhci clean

${target}: vhci ${board}
	@echo $@ done

clean: clean-${OBJ} clean-vhci
	rm -rf *.a *.axf *.hex ${target} *.vcd

.PHONY: test
