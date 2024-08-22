#ifndef UART_H
#define UART_H

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <ostream>

/**
 * @struct Data
 * @brief Holds data for communication.
 * 
 * Stores two integers, an operator, and a result.
 */
struct Data {
public:
    int x; ///< First integer value.
    int y; ///< Second integer value.
    uint8_t opp; ///< Operator as an 8-bit unsigned integer.
    uint32_t result; ///< Result of the operation.

    /**
     * @brief Default constructor for Data.
     * Initializes all members to zero.
     */
    Data();

    /**
     * @brief Parameterized constructor for Data.
     * @param x First integer value.
     * @param y Second integer value.
     * @param opp Operator as an 8-bit unsigned integer.
     * @param result Result of the operation.
     */
    Data(int x, int y, uint8_t opp, uint32_t result);

    /**
     * @brief Serialize the Data object into a buffer.
     * @param buffer Pointer to the buffer where the data will be serialized.
     */
    void serialize(uint8_t* buffer) const;

    /**
     * @brief Deserialize Data from a buffer.
     * @param buffer Pointer to the buffer from which data will be deserialized.
     */
    void deserialize(const uint8_t* buffer);

    /**
     * @brief Print the contents of the Data object.
     */
    void print() const;
};

/**
 * @struct uart_t
 * @brief Structure to hold UART configuration and state.
 * 
 * This structure encapsulates UART configuration parameters and state information.
 */
typedef struct {
    int fd; ///< File descriptor for the UART device.
    struct termios uart; ///< UART configuration settings.
    speed_t baudrate; ///< Baud rate for UART communication.
    tcflag_t parity; ///< Parity bit setting.
    tcflag_t stopBits; ///< Number of stop bits.
    tcflag_t dataBits; ///< Number of data bits.
    int timeout; ///< Timeout for UART operations.
    int vmin; ///< Minimum number of characters to read.
    bool isOpen; ///< Flag indicating if the UART device is open.
} uart_t;

/**
 * @brief Open the UART device.
 * @param uart Pointer to the uart_t structure with device configuration.
 * @param path Path to the UART device.
 * @return True if the device was opened successfully, false otherwise.
 */
bool uart_openDevice(uart_t* uart, const char* path);

/**
 * @brief Close the UART device.
 * @param uart Pointer to the uart_t structure with device configuration.
 * @return True if the device was closed successfully, false otherwise.
 */
bool uart_closeDevice(uart_t* uart);

/**
 * @brief Initialize the UART device with the given configuration.
 * @param uart Pointer to the uart_t structure with device configuration.
 * @return True if the device was initialized successfully, false otherwise.
 */
bool uart_initDevice(uart_t* uart);

/**
 * @brief Write data to the UART device.
 * @param uart Pointer to the uart_t structure with device configuration.
 * @param buff Pointer to the data buffer to be written.
 * @param length Length of the data to be written.
 * @return Number of bytes written, or -1 on error.
 */
ssize_t uart_writeData(uart_t* uart, const char* buff, size_t length);

/**
 * @brief Read data from the UART device.
 * @param uart Pointer to the uart_t structure with device configuration.
 * @param buff Pointer to the buffer where the read data will be stored.
 * @param sizeRead Length of the data to be read.
 * @return Number of bytes read, or -1 on error.
 */
ssize_t uart_readData(uart_t* uart, char* buff, size_t sizeRead);

/**
 * @brief Check if the UART device is open.
 * @param uart Pointer to the uart_t structure with device configuration.
 * @return True if the device is open, false otherwise.
 */
bool uart_isOpen(uart_t* uart);

/**
 * @brief Send data over UART.
 * @param uart Pointer to the uart_t structure with device configuration.
 * @param data Reference to the Data object to be sent.
 */
void sendData(uart_t* uart, const Data& data);

/**
 * @brief Receive data from UART.
 * @param uart Pointer to the uart_t structure with device configuration.
 * @param length Length of the data to be received.
 * @return Data object containing the received data.
 */
Data receiveData(uart_t* uart, size_t length);

#endif // UART_H