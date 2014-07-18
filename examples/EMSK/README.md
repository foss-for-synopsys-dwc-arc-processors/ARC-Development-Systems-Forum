Examples for EMSK board
=======================

## Timer IRQ examples

The example demostrates how to work with IRQ and timers for EM4/6 CPU.
Detaled description locates here.

If built as 'make EM=6' then all sections are mapped to the beginning of RAM (0x10000000) of EM6 configurations.
It produces additional binary, irq_test.img.

## Bootloader example

Very simple example for ARC EM6 cores of EMSK. It shows basic idea how application can be 'loaded' into RAM and 'started'.
User's application should be built to fit 1M and mapped to 0x10000000, having interrupt vector at the beginning.

Bootloader code is loaded at 0x00000000. Once started, it copies 1M of memory from 'SOURCE' section (0x17f00000) into beginning of RAM (0x10000000), 
writes new address of interrupt vector into AUX_REG 0x25, reads address of new reset handler and jumps there to perform 'soft reset'.

Steps to run this demo:
 * build irq demo app: ```make EM=6```
 * build bootloader demo: ```make```
 * start openocd: ```openocd -f <config_file>  -c init -c halt -c "reset halt"```
 * start GDB: ```arc-elf32-gdb bootloader.exe```
 * connect to openocd: ```target remote <IP> :3333```
 * load irq demo to the end of RAM: ```restore <path>/irq_test.img binary 0x17f00000```
 * load bootloader: ```load```
 * run the demo: ```continue```
