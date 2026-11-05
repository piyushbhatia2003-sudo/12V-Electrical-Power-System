//main.c : GPIO, UART and ADC initialization with UART command handling 

#include "eps.h"

int main() {
	char buffer[32];
	uint32_t last_print = 0;
	
	gpio_init();
	UART_Initialize(9600); //16 MHz oscillator baud rate
	ADC_Initialize();

	if(timer_ms - last_print >= 1000) { //timer starts at 1s (1000ms) and increases every 1s
		//UART command handling
		while (1) {
			UART_read_line(buffer);

			buffer[strcspn(buffer, "\r\n")] = 0;

			handle_command(buffer);
		}
	}
}

		
