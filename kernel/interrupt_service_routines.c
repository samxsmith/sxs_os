#include "interrupt_service_routines.h"
#include "../drivers/screen.h"
#include "interrupt_descriptor_table.h"
#include "util.h"
#include "../drivers/ports.h"

ISR_Table interrupt_handlers[256];

// can't use loop
// need the address of functions
// functions defined in asm
void isr_install() {
	// interrupt service routines (ISRs)
	// handle typically-fatal interrupts
	// i.e. they are CPU exceptions
	set_idt_entry(0, (word)isr0);
	set_idt_entry(1, (word)isr1);
	set_idt_entry(2, (word)isr2);
	set_idt_entry(3, (word)isr3);
	set_idt_entry(4, (word)isr4);
	set_idt_entry(5, (word)isr5);
	set_idt_entry(6, (word)isr6);
	set_idt_entry(7, (word)isr7);
	set_idt_entry(8, (word)isr8);
	set_idt_entry(9, (word)isr9);
	set_idt_entry(10, (word)isr10);
	set_idt_entry(11, (word)isr11);
	set_idt_entry(12, (word)isr12);
	set_idt_entry(13, (word)isr13);
	set_idt_entry(14, (word)isr14);
	set_idt_entry(15, (word)isr15);
	set_idt_entry(16, (word)isr16);
	set_idt_entry(17, (word)isr17);
	set_idt_entry(18, (word)isr18);
	set_idt_entry(19, (word)isr19);
	set_idt_entry(20, (word)isr20);
	set_idt_entry(21, (word)isr21);
	set_idt_entry(22, (word)isr22);
	set_idt_entry(23, (word)isr23);
	set_idt_entry(24, (word)isr24);
	set_idt_entry(25, (word)isr25);
	set_idt_entry(26, (word)isr26);
	set_idt_entry(27, (word)isr27);
	set_idt_entry(28, (word)isr28);
	set_idt_entry(29, (word)isr29);
	set_idt_entry(30, (word)isr30);
	set_idt_entry(31, (word)isr31);


	// idt entries 0 -> 31 are taken
	// so for these interrupt requests (IRQs)
	// we map them onto 32-47
	// we need to tell the PIC where to go to find the handlers
	// as it will expect them to start from 0
	// we do this by reconfiguring the PIC, changing the offsets to 32+

	// the Programmable Interrupt Controller (PIC) is used to program interrupt handlers
	// when an interrupt is triggered, the PIC chip will send the interrupt to the CPU
	// PIC primary's command port: 0x20
	// PIC primary's data port:    0x21
	// PIC secondary's command port: 0xA0
	// PIC secondary's data port:    0xA1
	// Details of PIC and how to interface with it:
	// https://wiki.osdev.org/PIC

	// 0x11 is the initiliase command
	// initialise primary and secondary
	write_byte_to_port(PRIMARY_PIC_COMMAND_PORT, 0x11);
    write_byte_to_port(SECONDARY_PIC_COMMAND_PORT, 0x11);

    // primary will use offsets 0x20 -> 0x27 (32 -> 39)
	write_byte_to_port(PRIMARY_PIC_DATA_PORT, 0x20);
    // secondary will use offsets 0x28 -> 0x2F (40 -> 47)
	write_byte_to_port(SECONDARY_PIC_DATA_PORT, 0x28);
    // tells primary that secondary is available on IRQ line 2 (represented as #4 for some reason?)
	write_byte_to_port(PRIMARY_PIC_DATA_PORT, 0x04);
    // tells secondary that it is the secondary (its "cascade identity" because the two PICs are setup as a cascade)
	write_byte_to_port(SECONDARY_PIC_DATA_PORT, 0x02);

	// 0x01 means intel 8086/88 mode -- i.e. x86 architecture
    write_byte_to_port(PRIMARY_PIC_DATA_PORT, 0x01);
    write_byte_to_port(SECONDARY_PIC_DATA_PORT, 0x01);

	// I think you're supposed to restore whatever was originally on the port
	// but here we'll just write them to 0
    write_byte_to_port(PRIMARY_PIC_DATA_PORT, 0x0);
    write_byte_to_port(SECONDARY_PIC_DATA_PORT, 0x0);

	// now the PIC knows the range where IRQ handlers will be
	// we can install the interrupt request (IRQ) handlers
	set_idt_entry(32, (word)irq0);
	set_idt_entry(33, (word)irq1);
	set_idt_entry(34, (word)irq2);
	set_idt_entry(35, (word)irq3);
	set_idt_entry(36, (word)irq4);
	set_idt_entry(37, (word)irq5);
	set_idt_entry(38, (word)irq6);
	set_idt_entry(39, (word)irq7);
	set_idt_entry(40, (word)irq8);
	set_idt_entry(41, (word)irq9);
	set_idt_entry(42, (word)irq10);
	set_idt_entry(43, (word)irq11);
	set_idt_entry(44, (word)irq12);
	set_idt_entry(45, (word)irq13);
	set_idt_entry(46, (word)irq14);
	set_idt_entry(47, (word)irq15);

	set_idt(); // load with asm
}

// message at index of its exception number
char *exception_messages[] = {
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",

	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",

	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"};

void isr_handler(Register_Table rt) {
	screen_print("received interrupt...\n\0");
	char err_code[3];
	itoa(rt.interrupt_number, err_code);
	screen_print(err_code);
	screen_print("\n");

	screen_print(exception_messages[rt.interrupt_number]);
	screen_print("\n");
}

// takes an interrupt index and records its handler
void register_interrupt_handler(byte n, ISR_Table handler) {
	interrupt_handlers[n] = handler;
}

void irq_handler(Register_Table rt) {
	// after each interrupt we have to tell the PIC it's been handled
	// this is called an EOI (end of interrupt)
	// if we don't do this no more interrupts can be sent
	if (rt.interrupt_number >= 40) {
		// also send to secondary
		write_byte_to_port(SECONDARY_PIC_COMMAND_PORT, 0x20);
	}
	// send to master
	write_byte_to_port(PRIMARY_PIC_COMMAND_PORT, 0x20);

	if (interrupt_handlers[rt.interrupt_number] == 0) {
		char interruptNumString[3];
		itoa(rt.interrupt_number, interruptNumString);
		screen_print("unable to find handler for interrupt");
		screen_print(interruptNumString);
		screen_print("\n");
		return;
	}
	ISR_Table handler = interrupt_handlers[rt.interrupt_number];
	handler(rt);
}
