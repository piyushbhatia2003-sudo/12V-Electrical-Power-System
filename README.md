# 12V-Electrical-Power-System

This repository contains the embedded C firmware developed for a 12V Electrical Power System. This includes GPIO power rail enabling/disabling, UART communication (command and telemetry) and ADC voltage and current monitoring and fault handling.

It was tested on an ATmega328P/ Arduino Undo (16 MHz), using Microchip Studio and PuTTy. 

What it accomplishes:

Enables and disables three power rails (12V, 5V, 3.3V) and 2 correspondiong load outputs named A and B for each rail using GPIO. 

Monitors voltage and current using ATmega ADC

Receives commands over UART from a PC emulating an OBC and responds with the appropriate response

Provides condition of Electrical Power System like Overvoltage, Undervoltage, Overcurrent
