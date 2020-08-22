#include "timer.h"
#include "../drivers/screen.h"
#include "interrupt_service_routines.h"
#include "ports.h"

const word SPEED_OF_PIT_HZ = 1193182;
word tick = 0;

// static means visible to its object file only
static void timer_callback(Register_Table rt) {
	tick++;
	// screen_print("Tick: ");
	// char tick_ascii[256];
	// itoa(tick, tick_ascii);
	// screen_print(tick_ascii);
	// screen_print("\n");
}

// Takes a frequency in HZ (times per second) you want the interrupt to run.
// Beware that on an emulator, the frequencies may be quite a bit faster.
// Can be modified to accept a callback instead of using timer_callback
void init_timer(word desiredFreqencyInHz) {
	// register our first interrupt handler
	register_interrupt_handler(IRQ0, timer_callback);

	// the Programmable Interval Timer (PIT) is a circuit that allows you to work with time
	// PIT has 2 modes
	// 1) periodic interrupt mode (0x36) -- interrupt signal emitted at frequency
	// 2) one shot mode (0x34) -- PIT will decrease a counter at its top speed (1.19318 MHz), down to 0

	// we will be using mode 1 for to get an interrupt at the provided frequency
	// this mode works by setting a counter to a number e.g. 10
	// it will count down from that number to 0
	// when it hits 0, an interrupt signal is sent
	// then the conter is reset to the original number, e.g. 10
	// and the decrementing begins again
	
	// so the number you provide to the PIT is the number of cycles before an interrupt
	// it divides the actual frequency of the PIT by that number
	// if the PIT is 200HZ & you give the number 10
	// the interrupt will be generated at 20HZ (200/10)

	// equally if you provide a desired freqency in HZ
	// and divide the PIT frequency by your desired frequency
	// it gives you the correct number of cycles before each interrupt
	// that's what we'll do here
	// As stated above, in an emulator, the time will probably run faster than real life

	word cycles_per_interrupt = SPEED_OF_PIT_HZ / desiredFreqencyInHz;

	// the data has to be split into two bytes
	// bottom half, masking the top half
	byte low = (byte)(cycles_per_interrupt & 0xFF);
	// top half, masking the rest
	byte high = (byte)((cycles_per_interrupt >> 8) & 0xFF);

	// we communicate with the PIT through command port 0x43 & data port 0x40

	// 0x36 is periodic interrupt mode for the PIT -- see above ^^
	write_byte_to_port(0x43, 0x36); // write to command port
	write_byte_to_port(0x40, low); // write low data
	write_byte_to_port(0x40, high); // write high data
}