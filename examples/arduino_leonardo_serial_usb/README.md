# Arduino Leonardo (atmega32u4) USB serial port example

Based on the extra_board_usb example.

Uses the arduino enviroment to build the [test sketch](test/test.ino)

# Requirements

download Arduino-1.8.5 to this directory

run `make` to build the virtual usb drivers

then run `make test` to start the simulation. If all goes well you will see a virtual serial port detected in `dmesg`.

Output for me looks like:

    sudo obj-x86_64-linux-gnu/simusb.elf
    Bootloader 0000: 4040
    avr attached: 1
    Created virtual usb host with 1 port at usb_vhci_hcd.0 (bus# 10)
    port disabled
     STALL
     STALL
     STALL

At this point start something to read the terminal:

    miniterm.py /dev/ttyACM0 115200       

And you should see numbers being printed from the simulated sketch via the virtual com port!
