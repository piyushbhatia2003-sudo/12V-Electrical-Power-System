//uart.c: UART communication: reading commands, converting voltage and current values, evaluating power system condition and printing status

#include "eps.h"

void UART_Initialize(unsigned long baud_rate)
{
	unsigned long ubrr_value = (F_CPU / (16UL * baud_rate)) - 1; //get value for baud rate 
	UBRR0H = (ubrr_value >> 8); //Baud rate high
	UBRR0L = ubrr_value; //Baud rate low 

	UCSR0B = (1 << TXEN0) | (1 << RXEN0); //transmitting and receiving enabled for UART 
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

}

void UART_SendChar(char c) {
	
	while(!(UCSR0A & (1 << UDRE0))); //loop while Data Register is not empty 
	UDR0 = c; //character is sent 
}

void UART_SendString(const char *str) {

	while(*str)
	{
		UART_SendChar(*str++); //send each character, character by character
	}
}

eps_condition_t protection_check(float volt, float curr) {

	if(volt < VBUS_UV && curr > IBUS_OC) { //undervoltage and overcurrent
		return SHORT_CIRCUIT;
	} else if (volt > VBUS_OV && curr > IBUS_OC) { //overvoltage and overcurrent
		return SURGE;
	} else if (volt > VBUS_OV) { //overvoltage
		return OVERVOLTAGE;
	}
	  else if (curr > IBUS_OC) { 
			return OVERLOAD; //Overcurrent
	}
	  else if (volt < VBUS_UV) { //Undervoltage
			return UNDERVOLTAGE;
	} else {
		return NOMINAL; //normal
	}
	
}

//read line character by character
void UART_read_line(char *buffer) {
	char c;
	int i = 0;

	while (1) {
		while (!(UCSR0A & (1 << RXC0)));
		c = UDR0;

		if (c == '\n' || c == '\r') {
			buffer[i] = '\0';
			return;
		}

		buffer[i++] = c;
	}
}

//convert value to number
void uart_convert_2_num(uint32_t number) {
	
	char buf[BIT_STR];
	
	uint8_t index = 0;
	
	if(number == 0) {
		UART_SendChar('0'); //print 0 if number is 0
		return;
	}
	while(number > 0) { //if number is greater than 0
		buf[index++] = (number % 10) + '0';  //last digit is converted to ASCII text
		number /= 10; //every digit gets removed
	}
	
	while(index > 0) {
		UART_SendChar(buf[--index]); //send digits in correct order over UART
	}
	
}

//print voltage
void uart_volt(float volt) {
	
	uint32_t voltage; //scaling voltage to print it 
	uint32_t volt_int; //split into integer
	uint32_t volt_dec; //split into decimal 
	voltage = volt * 100; //scaled voltage
	volt_int = voltage / 100; //voltage integer
	volt_dec = voltage % 100; //voltage decimal 

	//printing voltage 
	UART_SendString("VOLTAGE: "); 
	uart_convert_2_num(volt_int);
	UART_SendString(".");
	uart_convert_2_num(volt_dec);
	UART_SendString(" V\r\n");

}

//print current
void uart_curr(float curr) {
	//printing current
	uint32_t current; //scaling current to print it
	uint32_t curr_int; //split into integer
	uint32_t curr_dec; //split into decimal
	current = curr * 100; //scaled current
	curr_int = current / 100; //voltage integer
	curr_dec = current % 100; //voltage decimal
	UART_SendString("CURRENT: ");
	uart_convert_2_num(curr_int);
	UART_SendString(".");
	uart_convert_2_num(curr_dec);
	UART_SendString(" A\r\n");
}

//print condition
void uart_faulthandling(eps_condition_t cond) {
		//switching between protection checks
		switch(cond) {
			case OVERVOLTAGE:
			UART_SendString("OVERVOLTAGE\n");
			break;
			case UNDERVOLTAGE:
			UART_SendString("UNDERVOLTAGE\n");
			break;
			case OVERLOAD:
			UART_SendString("OVERCURRENT\n");
			break;
			case SHORT_CIRCUIT:
			UART_SendString("SHORT-CIRCUIT\n");
			break;
			case SURGE:
			UART_SendString("SURGE\n");
			break;
			default:
			UART_SendString("NOMINAL\n");
			break;
		}
}

