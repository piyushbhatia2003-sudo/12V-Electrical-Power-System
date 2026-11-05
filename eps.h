//header file

#ifndef EPS_H
#define EPS_H

#define F_CPU 16000000UL

//libraries
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include <string.h>

#define SCL_CLK 100000UL
#define PRESCALER_128 1024
#define ADC_REF_VOLT 5.0
#define ADC_MAX 1023.0
#define SHUNT_RES 0.1
#define CURR_GAIN 50.0
#define MAX_CHARGE_CURRENT 1.5
#define MAX_DISCHARGE_CURRENT 6.5
#define VOLTAGE_BUS_GAIN_DIV 4.0
#define VBUS_UV 6.5
#define VBUS_OV 8.6
#define IBUS_OC 1.5
#define BIT_STR 17

#define ADC_VOLTAGE_CHANNEL 0 //A0
#define ADC_CURRENT_CHANNEL 1 //A1

//Protection labels
typedef enum {
	NOMINAL, //NORMAL
	SHORT_CIRCUIT, //Under-voltage & Over-current
	OVERLOAD, //Over-current
	UNDERVOLTAGE, //Battery Low
	OVERVOLTAGE, //Over-voltage
	SURGE //Over-voltage & Over-current
} eps_condition_t;

extern volatile uint32_t timer_ms;

//function prototypes

//ADC Functions
void ADC_Initialize(void);
unsigned long ADC_ReadChannel(uint8_t channel_id);
float Convert_ADC_to_Voltage(uint16_t raw_adc);
float Convert_ADC_to_Current(uint16_t raw_adc);

//GPIO functions
void gpio_init(void);
void power_rail_off(uint8_t power_rail);
void power_rail_on(uint8_t power_rail);
void switch_gpio(eps_condition_t cond);

//UART functions
void UART_Initialize(unsigned long baud_rate);
void UART_SendChar(char c);
void UART_SendString(const char *str);
eps_condition_t protection_check(float volt, float curr);
void uart_convert_2_num(uint32_t number);
void uart_volt(float volt);
void uart_curr(float curr);
void UART_read_line(char *buffer);
void handle_command(char *cmd);
float read_voltage(void);
float read_current(void);
void uart_faulthandling(eps_condition_t cond);

//Timer function
void Timer_Initialize();

#define BAUD 9600
#define UBRR_VAL ((F_CPU/16/BAUD)-1)

//ADC Channels (Port C)

#define ADC_CH0   0   // PC0
#define ADC_CH1   1   // PC1
#define ADC_CH2   2   // PC2
#define ADC_CH3   3   // PC3

#endif