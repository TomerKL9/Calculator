#include "func.h"

void Data::serialize(uint8_t* buffer) const {
    std::memcpy(buffer, &x, sizeof(x));
    std::memcpy(buffer + sizeof(x), &y, sizeof(y));
    std::memcpy(buffer + sizeof(x) + sizeof(y), &opp, sizeof(opp));
    std::memcpy(buffer + sizeof(x) + sizeof(y) + sizeof(opp), &result, sizeof(result));
}

void Data::deserialize(const uint8_t* buffer) {
    std::memcpy(&x, buffer, sizeof(x));
    std::memcpy(&y, buffer + sizeof(x), sizeof(y));
    std::memcpy(&opp, buffer + sizeof(x) + sizeof(y), sizeof(opp));
    std::memcpy(&result, buffer + sizeof(x) + sizeof(y) + sizeof(opp), sizeof(result));
}
Data::Data() : x(0), y(0), opp(0), result(0) {};
Data::Data(int x, int y, uint8_t opp, uint32_t result) 
        : x(x), y(y), opp(opp), result(result) {};

void Data::print() const {
    std::cout << "x = " << x << ", y = " << y 
              << ", opp = " << static_cast<char>(opp) 
              << ", result = " << result << std::endl;
}

bool uart_openDevice(uart_t* uart, const char* path) {
    uart->fd = open(path, O_RDWR | O_NOCTTY);
    if (uart->fd == -1) {
        uart->isOpen = false;
        return false;
    }
    uart->isOpen = true;
    return true;
}

bool uart_closeDevice(uart_t* uart) {
    if (uart_isOpen(uart) && close(uart->fd) == 0) {
        uart->isOpen = false;
        return true;
    }
    return false;
}

bool uart_initDevice(uart_t* uart) {
    if (tcgetattr(uart->fd, &uart->uart) != 0) {
        return false;
    }
    cfsetispeed(&uart->uart, uart->baudrate);
    cfsetospeed(&uart->uart, uart->baudrate);

    uart->uart.c_cflag &= ~PARENB;  // No parity
    uart->uart.c_cflag &= ~CSTOPB;  // 1 stop bit
    uart->uart.c_cflag &= ~CSIZE;
    uart->uart.c_cflag |= CS8;      // 8 data bits

    uart->uart.c_iflag = IGNPAR;    // Ignore parity errors
    uart->uart.c_lflag = 0;         // Raw input
    uart->uart.c_oflag = 0;         // Raw output

    uart->uart.c_lflag &= ~(IXON | IXOFF | IXANY);  // Disable flow control

    uart->uart.c_cc[VTIME] = uart->timeout;
    uart->uart.c_cc[VMIN] = uart->vmin;

    uart->uart.c_cflag |= (CLOCAL | CREAD);  // Local connection, enable receiver

    if (tcsetattr(uart->fd, TCSANOW, &uart->uart) != 0) {
        return false;
    }
    return true;
}

ssize_t uart_writeData(uart_t* uart, const char* buff, size_t length) {
    return write(uart->fd, buff, length);
}

ssize_t uart_readData(uart_t* uart, char* buff, size_t sizeRead) {
    return read(uart->fd, buff, sizeRead);
}

bool uart_isOpen(uart_t* uart) {
    return uart->isOpen;
}

void sendData(uart_t* uart, const Data& data) {
    size_t dataSize = sizeof(data);

    // Use char for the buffer to match the uart_writeData function signature
    char buffer[dataSize];
    data.serialize(reinterpret_cast<uint8_t*>(buffer));  // Serialize data

    ssize_t written = uart_writeData(uart, buffer, dataSize);
    if (written < 0) {
        std::cerr << "Failed to write data to UART" << std::endl;
    } else {
        std::cout << "Sent data: ";
        data.print();
    }
}

Data receiveData(uart_t* uart, size_t length) {
    // Use char for the buffer to match the uart_readData function signature
    char buffer[length];
    ssize_t readBytes = uart_readData(uart, buffer, length);
    if (readBytes < 0) {
        std::cerr << "Failed to read data from UART" << std::endl;
        return {};  // Return an empty Data structure
    } else {
        Data receivedData;
        receivedData.deserialize(reinterpret_cast<uint8_t*>(buffer));  // Deserialize data
        std::cout << "Received data: ";
        receivedData.print();
        return receivedData;
    }
}