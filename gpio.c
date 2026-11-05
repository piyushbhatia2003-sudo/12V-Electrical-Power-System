// gpio.c - Enabling and disabling of power rails and load outputs, power sequencing logic and command handler

#include "eps.h"

//power rails and loads initialization

int power_rail_12V_on = 0;
int power_rail_5V_on = 0;
int power_rail_3V3_on = 0;

int load_12A_enable = 0;
int load_12B_enable = 0;
int load_5A_enable = 0;
int load_5B_enable = 0;
int load_3V3A_enable = 0;
int load_3V3B_enable = 0;

//GPIO initialization

void gpio_init(void)
{
	//Power Rails (PORTB - PB0 - PB2)
	DDRB |= (1 << 0) | (1 << 1) | (1 << 2);
	
	//Loads (PD2 - PD7)
	DDRD |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);
	
	PORTB &= ~((1 << 0) | (1 << 1) | (1 << 2));
	PORTD &= ~((1 << 2) | (1 << 3)) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);
	
}

void update_outputs(void)
{
	//rails
	if (power_rail_12V_on) {
		PORTB |= (1 << 0); //Sets 12V rail on
	}	else {
		PORTB &= ~(1 << 0); //Sets 12V rail off
	}
	if (power_rail_5V_on) {
		PORTB |= (1 << 1); //Sets 5V rail on
		}	else {
		PORTB &= ~(1 << 1); //Sets 5V rail off
	}
	if (power_rail_3V3_on) { //Sets 3.3V rail on
		PORTB |= (1 << 2);
		}	else {
		PORTB &= ~(1 << 2); //Sets 3.3V rail off
	}
	
	//loads
	
	//load is never turned on if associated power rail is not turned on for A/B load outputs
	if(power_rail_12V_on && load_12A_enable) { 
		PORTD |= (1 << 2);
	} else {
		PORTD |= (1 << 2);
	}
	if(power_rail_12V_on && load_12B_enable) {
		PORTD |= (1 << 3);
		} else {
		PORTD |= (1 << 3);
	}
	if(power_rail_5V_on && load_5A_enable) {
		PORTD |= (1 << 4);
		} else {
		PORTD |= (1 << 4);
	}
	if(power_rail_5V_on && load_5B_enable) {
		PORTD |= (1 << 5);
		} else {
		PORTD |= (1 << 5);
	}
	if(power_rail_3V3_on && load_3V3A_enable) {
		PORTD |= (1 << 6);
		} else {
		PORTD |= (1 << 6);
	}
	if(power_rail_3V3_on && load_3V3B_enable) {
		PORTD |= (1 << 7);
		} else {
		PORTD |= (1 << 7);
	}
}

void handle_command(char *cmd)
{
	//Remove newline
	cmd[strcspn(cmd, "\r\n")] = 0;
	
	//Turning power rails on and off
	if (strcmp(cmd, "PWR 12V ON") == 0)
	{
		power_rail_12V_on = 1;
		UART_SendString("12V ON\r\n");
		
	}
	else if (strcmp(cmd, "PWR 12V OFF") == 0) {
		
		power_rail_12V_on = 0;
		UART_SendString("12V OFF\r\n");
		
	}
	else if (strcmp(cmd, "PWR 5V ON") == 0) {
		
		power_rail_5V_on = 1;
		UART_SendString("5V ON\r\n");
		
	} else if (strcmp(cmd, "PWR 5V OFF") == 0) {
		
		power_rail_5V_on = 0;
		UART_SendString("5V OFF\r\n");
		
	} 
	else if (strcmp(cmd, "PWR 3V3 ON") == 0) {
		
		power_rail_3V3_on = 1;
		UART_SendString("3V3 ON\r\n");
		
	} else if (strcmp(cmd, "PWR 3V3 OFF") == 0) {
		
		power_rail_3V3_on = 0;
		UART_SendString("3V3 OFF\r\n");
		
	}


	//turning load outputs on and off
	else if (strcmp(cmd, "LOAD 12A ON") == 0) {
		
		if (power_rail_12V_on) //only able to if associated 12V rail is on
		{
			load_12A_enable = 1;
			UART_SendString("12A ON\r\n");
		}
		else {
			UART_SendString("ERROR: 12V OFF\r\n");
		}
	}
	
	else if (strcmp(cmd, "LOAD 12B ON") == 0) {
		
		if (power_rail_12V_on)
		{
			load_12B_enable = 1;
			UART_SendString("12B ON\r\n");
		}
		else {
			UART_SendString("ERROR: 12V OFF\r\n");
		}
	}
	
	else if (strcmp(cmd, "LOAD 5A ON") == 0) { //only able to if associated 5V rail is on
		
		if (power_rail_5V_on)
		{
			load_5A_enable = 1;
			UART_SendString("5A ON\r\n");
		}
		else {
			UART_SendString("ERROR: 5V OFF\r\n");
		}
	}
	
	else if (strcmp(cmd, "LOAD 5B ON") == 0) {
		
		if (power_rail_12V_on)
		{
			load_5B_enable = 1;
			UART_SendString("5B ON\r\n");
		}
		else {
			UART_SendString("ERROR: 5V OFF\r\n");
		}
	}
	
	else if (strcmp(cmd, "LOAD 3V3A ON") == 0) { //only able to if associated 3.3V rail is on
		
		if (power_rail_3V3_on)
		{
			load_3V3A_enable = 1;
			UART_SendString("3V3A ON\r\n");
		}
		else {
			UART_SendString("ERROR: 3V3 OFF\r\n");
		}
	}
	
	else if (strcmp(cmd, "LOAD 3V3B ON") == 0) {
		
		if (power_rail_3V3_on)
		{
			load_3V3B_enable = 1;
			UART_SendString("3V3B ON\r\n");
		}
		else {
			UART_SendString("ERROR: 3V3 OFF\r\n");
		}
	}
	
	else if (strcmp(cmd, "GET HEALTH") == 0) { //getting condition of power system
		float volt = read_voltage();
		float curr = read_current();
		uart_volt(volt);
		uart_curr(curr);
		eps_condition_t condition =  protection_check(volt, curr);
		uart_faulthandling(condition);
	}
	
	else {
		UART_SendString("UNKNOWN COMMAND\r\n");
	}
	
	update_outputs(); //update load output status
}