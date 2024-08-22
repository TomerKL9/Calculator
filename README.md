MCU Calculator Project
Overview
This project implements a calculator using an STM32F746 microcontroller (MCU) and a BeagleBone Green (BBG) as an interface. The system supports two optional communication modes: UART and SPI. The MCU processes mathematical operations and returns the result to the BBG.

Components
STM32F746 MCU: Acts as a calculator, performing operations based on received data.
BeagleBone Green (BBG): Serves as the interface for sending and receiving data to/from the MCU.
UART: One of the communication options between the BBG and the MCU.
SPI: An alternative communication method supported by the system.
