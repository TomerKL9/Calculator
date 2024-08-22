// func.h
#ifndef FUNC_H
#define FUNC_H

#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

/**
 * @brief SPI device configuration structure.
 * 
 * Contains the file descriptor and configuration settings for an SPI device.
 */
typedef struct {
    int fd; ///< File descriptor for the SPI device.
    bool isOpen; ///< Status of the device (open or closed).
    uint8_t mode; ///< SPI mode.
    uint8_t bits; ///< Number of bits per SPI word.
    uint32_t speed; ///< Maximum SPI speed in Hertz.
} spi_t;

/**
 * @brief Data structure for SPI communication.
 * 
 * Holds data to be transmitted and received over SPI.
 */
struct Data {
    int x; ///< X coordinate.
    int y; ///< Y coordinate.
    uint8_t opp; ///< Operation code (e.g., '+', '-', '*', '/').
    uint32_t result; ///< Result of the operation.

    /**
     * @brief Constructs a Data object with specified values.
     * 
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param opp Operation code.
     * @param result Result value (default is 0).
     */
    Data(int x, int y, uint8_t opp, uint32_t result = 0);

    /**
     * @brief Default constructor initializes all values to zero.
     */
    Data();

    /**
     * @brief Serializes the Data object into a buffer.
     * 
     * Converts the Data object into a byte buffer that can be transmitted over SPI.
     * 
     * @param buffer Pointer to the buffer where the data will be serialized.
     */
    void serialize(uint8_t* buffer) const;

    /**
     * @brief Deserializes the Data object from a buffer.
     * 
     * Converts a byte buffer received from SPI into a Data object.
     * 
     * @param buffer Pointer to the buffer containing the serialized data.
     */
    void deserialize(const uint8_t* buffer);
};

/**
 * @brief Opens the SPI device file.
 * 
 * Attempts to open the SPI device file specified by the path.
 * 
 * @param spi Pointer to the spi_t structure that will be initialized with the file descriptor.
 * @param path Path to the SPI device file (e.g., "/dev/spidev1.0").
 * @return True if the device was successfully opened, false otherwise.
 */
bool spi_openDevice(spi_t* spi, const char* path);

/**
 * @brief Closes the SPI device file.
 * 
 * Closes the SPI device file and updates the status in the spi_t structure.
 * 
 * @param spi Pointer to the spi_t structure containing the file descriptor.
 * @return True if the device was successfully closed, false otherwise.
 */
bool spi_closeDevice(spi_t* spi);

/**
 * @brief Initializes the SPI device.
 * 
 * Configures the SPI device with the mode, bits per word, and speed specified in the spi_t structure.
 * 
 * @param spi Pointer to the spi_t structure containing the configuration settings.
 * @return True if initialization is successful, false otherwise.
 */
bool spi_initDevice(spi_t* spi);

/**
 * @brief Sends and receives data over SPI.
 * 
 * Transfers data to and from an SPI device using the specified buffers.
 * 
 * @param spi Pointer to the spi_t structure containing SPI configuration.
 * @param tx_buf Pointer to the buffer with data to be transmitted.
 * @param rx_buf Pointer to the buffer where received data will be stored.
 * @param len Length of the data to be transferred.
 * @return True if the transfer is successful, false otherwise.
 */
bool sendReceiveSPI(spi_t* spi, const uint8_t* tx_buf, uint8_t* rx_buf, size_t len);

#endif // FUNC_H
