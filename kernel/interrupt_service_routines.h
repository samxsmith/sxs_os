#ifndef ISR_H
#define ISR_H

#include "types.h"

// extern -- implemented in assembly
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// IRQ definitions
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

typedef struct {
	word data_segment_selector;
	word edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushed by pusha
	word interrupt_number, error_code;
	word eip, cs, eflags, useresp, ss; // pushed by cpu -- not sure what they all are
} Register_Table;

void isr_install();
void isr_handler(Register_Table r);

// define port numbers for PIC
#define PRIMARY_PIC_COMMAND_PORT 0x20
#define SECONDARY_PIC_COMMAND_PORT 0xA0
#define PRIMARY_PIC_DATA_PORT 0x21
#define SECONDARY_PIC_DATA_PORT 0xA1


// vvv
// Defines a function type
// Which returns void
// Takes Register_Table as its only param
// The function type is called ISR_Table
// And ISR_Table type is a pointer to such a function
// vvv
typedef void (*ISR_Table)(Register_Table);

void register_interrupt_handler(byte n, ISR_Table handler);

#endif
