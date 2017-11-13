/* vim: set sts=4:sw=4:ts=4:noexpandtab
	simusb.c

	Copyright 2012 Torbjorn Tyridal <ttyridal@gmail.com>

 	This file is part of simavr.

	simavr is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	simavr is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with simavr.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libgen.h>

#include <pthread.h>

#include "sim_avr.h"
#include "avr_ioport.h"
#include "sim_elf.h"
#include "sim_hex.h"
#include "sim_gdb.h"
#include "vhci_usb.h"
#include "sim_vcd_file.h"

struct vhci_usb_t vhci_usb;
avr_t * avr = NULL;
avr_vcd_t vcd_file;



int main(int argc, char *argv[])
{
//		elf_firmware_t f;
    fprintf(stderr, "stage %d\n" , 0);

	avr = avr_make_mcu_by_name("atmega32u4");
	if (!avr) {
		fprintf(stderr, "%s: Error creating the AVR core\n", argv[0]);
		exit(1);
	}
    fprintf(stderr, "stage %d\n" , 1);
	avr_init(avr);
	avr->frequency = 16000000;
    fprintf(stderr, "stage %d\n" , 2);

	// this trick creates a file that contains /and keep/ the flash
	// in the same state as it was before. This allow the bootloader
	// app to be kept, and re-run if the bootloader doesn't get a
	// new one
	{
		char path[1024];
		uint32_t base, size;

		uint8_t * boot = read_ihex_file("/tmp/arduino-sketch-DB5E8967374B107190C82CEEB8DD4891/test.ino.hex", &size, &base);
		if (!boot) {
			fprintf(stderr, "%s: Unable to load %s\n", argv[0], path);
			exit(1);
		}
		printf("Bootloader %04x: %d\n", base, size);
		memcpy(avr->flash + base, boot, size);
		free(boot);
		avr->pc = base;
		avr->codeend = avr->flashend;
	}

    fprintf(stderr, "stage %d\n" , 3);
	// even if not setup at startup, activate gdb if crashing
	avr->gdb_port = 1234;
	if (0) {
		//avr->state = cpu_Stopped;
		avr_gdb_init(avr);
	}

    fprintf(stderr, "stage %d\n" , 4);
	vhci_usb_init(avr, &vhci_usb);
    fprintf(stderr, "stage %d\n" , 5);
	vhci_usb_connect(&vhci_usb, '0');
    fprintf(stderr, "stage %d\n" , 6);


	while (1) {
		avr_run(avr);
	}
}
