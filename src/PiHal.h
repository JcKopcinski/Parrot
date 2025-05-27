// PiHal_modernized.h
#ifndef PI_HAL_MODERNIZED_H
#define PI_HAL_MODERNIZED_H

#include <RadioLib.h>
#include <chrono>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <gpiod.h>
#include <cstring>
#include <cerrno>
#include "Timer.h"
#include "DuckError.h"
#include "DuckLogger.h"

#define PI_INPUT         0
#define PI_OUTPUT        1
#define PI_RISING        1
#define PI_FALLING       2
#define PI_MAX_USER_GPIO 31

class PiHal : public RadioLibHal {
  public:
    PiHal(const char* gpiochip = "/dev/gpiochip0", uint32_t spiSpeed = 2000000)
      : RadioLibHal(PI_INPUT, PI_OUTPUT, 0, 1, PI_RISING, PI_FALLING),
        _gpiochip_path(gpiochip), _spiSpeed(spiSpeed), chip(nullptr), spiFd(-1), lastError(DUCK_ERR_NONE) {
    }

    void init() override {
      chip = gpiod_chip_open(_gpiochip_path);
      if (!chip) {
        logerr_ln("PiHal ERROR: Failed to open GPIO chip: %s", strerror(errno));
        lastError = DUCK_ERR_SETUP;
        return;
      }

      spiFd = open("/dev/spidev0.0", O_RDWR);
      if (spiFd < 0) {
        logerr_ln("PiHal ERROR: Failed to open SPI device: %s", strerror(errno));
        lastError = DUCK_ERR_SETUP;
        return;
      }

      uint8_t mode = SPI_MODE_0;
      ioctl(spiFd, SPI_IOC_WR_MODE, &mode);
      ioctl(spiFd, SPI_IOC_WR_MAX_SPEED_HZ, &_spiSpeed);
    }

    void term() override {
      if (chip) gpiod_chip_close(chip);
      if (spiFd >= 0) close(spiFd);
    }

    void pinMode(uint32_t pin, uint32_t mode) override {
      auto line = gpiod_chip_get_line(chip, pin);
      if (!line) {
        logerr_ln("PiHal ERROR: Failed to get GPIO line %u", pin);
        lastError = DUCK_ERR_SETUP;
        return;
      }
      int rc = (mode == PI_OUTPUT) ?
        gpiod_line_request_output(line, "radiolib", 0) :
        gpiod_line_request_input(line, "radiolib");
      if (rc < 0) {
        logerr_ln("PiHal ERROR: Failed to request line %u", pin);
        lastError = DUCK_ERR_SETUP;
      }
    }

    void digitalWrite(uint32_t pin, uint32_t value) override {
      auto line = gpiod_chip_get_line(chip, pin);
      if (!line) {
        logerr_ln("PiHal ERROR: Failed to get GPIO line for write: %u", pin);
        lastError = DUCK_ERR_SETUP;
        return;
      }
      gpiod_line_set_value(line, value);
    }

    uint32_t digitalRead(uint32_t pin) override {
      auto line = gpiod_chip_get_line(chip, pin);
      if (!line) {
        logerr_ln("PiHal ERROR: Failed to get GPIO line for read: %u", pin);
        lastError = DUCK_ERR_SETUP;
        return 0;
      }
      return gpiod_line_get_value(line);
    }

    void delay(unsigned long ms) override {
      std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    void delayMicroseconds(unsigned long us) override {
      std::this_thread::sleep_for(std::chrono::microseconds(us));
    }

    void yield() override {
      std::this_thread::yield();
    }

    unsigned long millis() override {
      return ::millis();
    }

    unsigned long micros() override {
      return ::micros();
    }

    long pulseIn(uint32_t pin, uint32_t state, unsigned long timeout) override {
      auto start = micros();
      while (digitalRead(pin) == state) {
        if (micros() - start > timeout) return 0;
      }
      return micros() - start;
    }

    void spiBegin() override {}
    void spiBeginTransaction() override {}
    void spiEndTransaction() override {}
    void spiEnd() override {}

    void spiTransfer(uint8_t* out, size_t len, uint8_t* in) override {
      struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)out,
        .rx_buf = (unsigned long)in,
        .len = static_cast<uint32_t>(len),
        .speed_hz = _spiSpeed,
        .bits_per_word = 8,
      };

      if (ioctl(spiFd, SPI_IOC_MESSAGE(1), &tr) < 0) {
        logerr_ln("PiHal ERROR: SPI transfer failed: %s", strerror(errno));
        lastError = DUCKLORA_ERR_TRANSMIT;
      }
    }

    void attachInterrupt(uint32_t, void (*)(void), uint32_t) override {
      // Optional: use thread-based polling or epoll with gpiod_line_event_wait
    }

    void detachInterrupt(uint32_t) override {}

    void tone(uint32_t, unsigned int, unsigned long = 0) override {}
    void noTone(uint32_t) override {}

    int getLastError() const {
      return lastError;
    }

  private:
    const char* _gpiochip_path;
    uint32_t _spiSpeed;
    struct gpiod_chip* chip;
    int spiFd;
    int lastError;
};

#endif // PI_HAL_MODERNIZED_H
