#pragma once

#include <driver/gpio.h>

extern const gpio_num_t RSVD_IO0;
extern const gpio_num_t GPIO_1;
extern const gpio_num_t GPIO_2;
extern const gpio_num_t RSVD_IO3;
extern const gpio_num_t GPIO_4;
extern const gpio_num_t GPIO_5;
extern const gpio_num_t GPIO_6;
extern const gpio_num_t GPIO_7;
extern const gpio_num_t GPIO_8;
extern const gpio_num_t GPIO_9;
extern const gpio_num_t GPIO_10;
extern const gpio_num_t GPIO_11;
extern const gpio_num_t GPIO_12;
extern const gpio_num_t GPIO_13;
extern const gpio_num_t GPIO_14;
extern const gpio_num_t GPIO_15;
extern const gpio_num_t GPIO_16;
extern const gpio_num_t GPIO_17;
extern const gpio_num_t GPIO_18;
extern const gpio_num_t _INT_USB_DPOS;
extern const gpio_num_t _INT_USB_DNEG;
extern const gpio_num_t GPIO_21;
extern const gpio_num_t NONEXISTENT22;
extern const gpio_num_t NONEXISTENT23;
extern const gpio_num_t NONEXISTENT24;
extern const gpio_num_t NONEXISTENT25;
extern const gpio_num_t _INT_SPI_CS1;
extern const gpio_num_t _INT_SPI_HD;
extern const gpio_num_t _INT_SPI_WP;
extern const gpio_num_t _INT_SPI_CS0;
extern const gpio_num_t _INT_SPI_CLK;
extern const gpio_num_t _INT_SPI_Q;
extern const gpio_num_t _INT_SPI_D;
extern const gpio_num_t GPIO_33;
extern const gpio_num_t GPIO_34;
extern const gpio_num_t GPIO_35;
extern const gpio_num_t GPIO_36;
extern const gpio_num_t GPIO_37;
extern const gpio_num_t GPIO_38;
extern const gpio_num_t GPIO_39;
extern const gpio_num_t GPIO_40;
extern const gpio_num_t GPIO_41;
extern const gpio_num_t GPIO_42;
extern const gpio_num_t UART0_TX;
extern const gpio_num_t UART0_RX;
extern const gpio_num_t RSVD_IO45;
extern const gpio_num_t RSVD_IO46;
extern const gpio_num_t GPIO_47;
extern const gpio_num_t GPIO_48;

#define I2C_SCL GPIO_NUM_1
#define I2C_SDA GPIO_NUM_2

namespace Pins
{
    void init();
    void initPin(gpio_num_t pin, gpio_mode_t mode, bool defaultHigh);

    void setOutput(gpio_num_t pin, bool high);
    bool getOutput(gpio_num_t pin);
    bool getInput(gpio_num_t pin);

    class DebouncedInput
    {
        static const uint8_t undefinedRead = 0xFF;

        uint8_t debounceCount;
        uint8_t previousRead = undefinedRead;
        uint8_t settledRead = undefinedRead;

        static const uint8_t DEBOUNCE_FILTER = 5;

    public:
        bool debounce(gpio_num_t pin);
        bool get();
    };
}