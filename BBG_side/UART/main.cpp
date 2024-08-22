#include "func.h"


const char* uartDevice = "/dev/ttyS1";
const int UART_BAUDRATE = B115200;
const int UART_PARITY = 0;
const int UART_STOPBITS = 0;
const int UART_DATABITS = CS8;
const int UART_TIMEOUT = 5;
const int UART_VMIN = 1;

 // UART device path
/**
 * @brief Main function to initialize UART device, get user input, send and receive data.
 * 
 * This function initializes the UART device, gets user input for two numbers and an operator,
 * prepares the data, serializes it, sends it over UART, receives the response, deserializes it,
 * and prints the received data.
 * 
 * @return int Returns 0 on success, 1 on invalid operator.
 */
int main() {
    uart_t uart;

    uart.baudrate = UART_BAUDRATE;
    uart.parity = UART_PARITY;
    uart.stopBits = UART_STOPBITS;
    uart.dataBits = UART_DATABITS;
    uart.timeout = UART_TIMEOUT;
    uart.vmin = UART_VMIN;

    if (!uart_openDevice(&uart, uartDevice)) {
        std::cerr << "Failed to open UART device" << std::endl;
        return 1;
    }

    if (!uart_initDevice(&uart)) {
        std::cerr << "Failed to initialize UART device" << std::endl;
        uart_closeDevice(&uart);
        return 1;
    }

    int x, y;
    char opp_char;
    
    std::cout << "Enter first number (x): ";
    std::cin >> x;
    std::cout << "Enter second number (y): ";
    std::cin >> y;
    std::cout << "Enter operator (+, -, *, /): ";
    std::cin >> opp_char;

    // Convert operator to uint8_t
    uint8_t opp;
    switch (opp_char) {
        case '+': opp = '+'; break;
        case '-': opp = '-'; break;
        case '*': opp = '*'; break;
        case '/': opp = '/'; break;
        default:
            std::cerr << "Invalid operator!" << std::endl;
            uart_closeDevice(&uart);
            return 1;
    }

    // Create Data object with user input
    Data dataToSend{x, y, opp, 0};  // You can set result to 0 or compute it later

    // Send data
    sendData(&uart, dataToSend);

    std::cout << "Waiting to receive a message..." << std::endl;
    Data receivedData = receiveData(&uart, sizeof(Data));  // Adjust length to sizeof(Data)

    uart_closeDevice(&uart);

    return 0;
}