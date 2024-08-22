#include "func.h"


const char* const SPI_DEVICE_PATH = "/dev/spidev1.0"; // SPI device file path
const uint8_t SPI_MODE = SPI_MODE_0; // SPI mode
const uint8_t SPI_BITS_PER_WORD = 8; // Number of bits per SPI word
const uint32_t SPI_MAX_SPEED = 500000; // Max SPI speed in Hz (500 kHz)
const useconds_t USLEEP_TIME = 6000; // Delay between SPI transactions in microseconds

/**
 * @brief Main program to perform SPI communication.
 * 
 * This program initializes the SPI device, takes user input for two integers
 * and an operator, sends this data to the SPI device, and receives the result.
 * The received data is then printed to the console.
 * 
 * The SPI device is configured with parameters specified in the code.
 * The data is sent twice with a short delay between the transmissions.
 * 
 * @return Returns 0 on successful execution, 1 on failure.
 */
int main() {
    spi_t spi;
    spi.mode = SPI_MODE;
    spi.bits = SPI_BITS_PER_WORD;
    spi.speed = SPI_MAX_SPEED;

    // Initialize SPI device
    if (!spi_openDevice(&spi, SPI_DEVICE_PATH) || !spi_initDevice(&spi)) {
        std::cerr << "Failed to initialize SPI device." << std::endl;
        return 1;
    }

    // Get user input
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
            spi_closeDevice(&spi);
            return 1;
    }

    // Prepare Data
    Data data_to_send = {x, y, opp, 0}; // data to send with result as defualt 0
    size_t data_size = sizeof(Data);
    uint8_t tx_buf[data_size];
    uint8_t rx_buf[data_size];

    // Serialize Data into tx_buf
    data_to_send.serialize(tx_buf);

    // Send Data on SPI device
    if (!sendReceiveSPI(&spi, tx_buf, rx_buf, data_size)) {
        spi_closeDevice(&spi);
        return 1;
    }

    usleep(USLEEP_TIME);

    if (!sendReceiveSPI(&spi, tx_buf, rx_buf, data_size)) {
        spi_closeDevice(&spi);
        return 1;
    }

    // Deserialize received Data
    Data received_data;
    received_data.deserialize(rx_buf);

    // Print received data
    std::cout << "Received Data: x = " << received_data.x 
              << ", y = " << received_data.y 
              << ", opp = " << (char)received_data.opp 
              << ", result = " << received_data.result 
              << std::endl;

    // Close SPI device
    spi_closeDevice(&spi);

    return 0;
}