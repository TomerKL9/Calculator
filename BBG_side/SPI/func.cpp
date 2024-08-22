// func.cpp
#include "func.h"

Data::Data(int x, int y, uint8_t opp, uint32_t result)
    : x(x), y(y), opp(opp), result(result) {}

Data::Data() : x(0), y(0), opp(0), result(0) {}

void Data::serialize(uint8_t* buffer) const {
    memcpy(buffer, &x, sizeof(x));
    memcpy(buffer + sizeof(x), &y, sizeof(y));
    memcpy(buffer + sizeof(x) + sizeof(y), &opp, sizeof(opp));
    memcpy(buffer + sizeof(x) + sizeof(y) + sizeof(opp), &result, sizeof(result));
}

void Data::deserialize(const uint8_t* buffer) {
    memcpy(&x, buffer, sizeof(x));
    memcpy(&y, buffer + sizeof(x), sizeof(y));
    memcpy(&opp, buffer + sizeof(x) + sizeof(y), sizeof(opp));
    memcpy(&result, buffer + sizeof(x) + sizeof(y) + sizeof(opp), sizeof(result));
}

bool spi_initDevice(spi_t* spi) {
    if (!spi->isOpen) {
        std::cerr << "SPI device is not open." << std::endl;
        return false;
    }

    // Set SPI mode
    if (ioctl(spi->fd, SPI_IOC_WR_MODE, &spi->mode) == -1) {
        std::cerr << "Failed to set SPI mode: " << strerror(errno) << std::endl;
        return false;
    }

    // Set SPI bits per word
    if (ioctl(spi->fd, SPI_IOC_WR_BITS_PER_WORD, &spi->bits) == -1) {
        std::cerr << "Failed to set SPI bits per word: " << strerror(errno) << std::endl;
        return false;
    }

    // Set SPI max speed
    if (ioctl(spi->fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi->speed) == -1) {
        std::cerr << "Failed to set SPI speed: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}
bool spi_openDevice(spi_t* spi, const char* path) {
    spi->fd = open(path, O_RDWR);
    if (spi->fd < 0) {
        std::cerr << "Failed to open SPI device " << path << ": " << strerror(errno) << std::endl;
        spi->isOpen = false;
        return false;
    }
    spi->isOpen = true;
    return true;
}

bool spi_closeDevice(spi_t* spi) {
    if (spi->isOpen && close(spi->fd) == 0) {
        spi->isOpen = false;
        return true;
    }
    return false;
}

bool sendReceiveSPI(spi_t* spi, const uint8_t* tx_buf, uint8_t* rx_buf, size_t len) {
    struct spi_ioc_transfer transfer;
    std::memset(&transfer, 0, sizeof(transfer));
    
    transfer.tx_buf = reinterpret_cast<uintptr_t>(tx_buf);
    transfer.rx_buf = reinterpret_cast<uintptr_t>(rx_buf);
    transfer.len = len;
    transfer.speed_hz = spi->speed;
    transfer.bits_per_word = spi->bits;

    if (ioctl(spi->fd, SPI_IOC_MESSAGE(1), &transfer) < 0) {
        std::cerr << "SPI transfer failed: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}
