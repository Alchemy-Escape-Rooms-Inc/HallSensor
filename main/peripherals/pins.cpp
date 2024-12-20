#include "pins.h"
#include <soc/gpio_reg.h>
#include <rom/gpio.h>

const gpio_num_t RSVD_IO0		= GPIO_NUM_0;
const gpio_num_t GPIO_1		    = GPIO_NUM_1;
const gpio_num_t GPIO_2		    = GPIO_NUM_2;
const gpio_num_t RSVD_IO3		= GPIO_NUM_3;
const gpio_num_t GPIO_4		    = GPIO_NUM_4;
const gpio_num_t GPIO_5			= GPIO_NUM_5;
const gpio_num_t GPIO_6	        = GPIO_NUM_6;
const gpio_num_t GPIO_7			= GPIO_NUM_7;
const gpio_num_t GPIO_8			= GPIO_NUM_8;
const gpio_num_t GPIO_9			= GPIO_NUM_9;
const gpio_num_t GPIO_10		= GPIO_NUM_10;
const gpio_num_t GPIO_11		= GPIO_NUM_11;
const gpio_num_t GPIO_12		= GPIO_NUM_12;
const gpio_num_t GPIO_13		= GPIO_NUM_13;
const gpio_num_t GPIO_14		= GPIO_NUM_14;
const gpio_num_t GPIO_15		= GPIO_NUM_15;
const gpio_num_t GPIO_16		= GPIO_NUM_16;
const gpio_num_t GPIO_17		= GPIO_NUM_17;
const gpio_num_t GPIO_18		= GPIO_NUM_18;
const gpio_num_t _INT_USB_DPOS	= GPIO_NUM_19;
const gpio_num_t _INT_USB_DNEG	= GPIO_NUM_20;
const gpio_num_t GPIO_21		= GPIO_NUM_21;
const gpio_num_t NONEXISTENT22	= GPIO_NUM_NC;
const gpio_num_t NONEXISTENT23	= GPIO_NUM_NC;
const gpio_num_t NONEXISTENT24	= GPIO_NUM_NC;
const gpio_num_t NONEXISTENT25	= GPIO_NUM_NC;
const gpio_num_t _INT_SPI_CS1	= GPIO_NUM_26;
const gpio_num_t _INT_SPI_HD	= GPIO_NUM_27;
const gpio_num_t _INT_SPI_WP	= GPIO_NUM_28;
const gpio_num_t _INT_SPI_CS0	= GPIO_NUM_29;
const gpio_num_t _INT_SPI_CLK	= GPIO_NUM_30;
const gpio_num_t _INT_SPI_Q		= GPIO_NUM_31;
const gpio_num_t _INT_SPI_D		= GPIO_NUM_32;
const gpio_num_t GPIO_33		= GPIO_NUM_33;
const gpio_num_t GPIO_34		= GPIO_NUM_34;
const gpio_num_t GPIO_35	    = GPIO_NUM_35;
const gpio_num_t GPIO_36		= GPIO_NUM_36;
const gpio_num_t GPIO_37		= GPIO_NUM_37;
const gpio_num_t GPIO_38		= GPIO_NUM_38;
const gpio_num_t GPIO_39		= GPIO_NUM_39;
const gpio_num_t GPIO_40		= GPIO_NUM_40;
const gpio_num_t GPIO_41		= GPIO_NUM_41;
const gpio_num_t GPIO_42		= GPIO_NUM_42;
const gpio_num_t UART0_TX		= GPIO_NUM_43;
const gpio_num_t UART0_RX		= GPIO_NUM_44;
const gpio_num_t RSVD_IO45		= GPIO_NUM_45;
const gpio_num_t RSVD_IO46		= GPIO_NUM_46;
const gpio_num_t GPIO_47		= GPIO_NUM_47;
const gpio_num_t GPIO_48		= GPIO_NUM_48;

namespace Pins
{
    void initPin(gpio_num_t pin, gpio_mode_t mode, bool defaultHigh = false)
    {
        gpio_reset_pin(pin); // Some pins are default configured and need to be reset to be used, it's not enough to just set pin direction and level
        gpio_set_direction(pin, mode);
        if (mode == GPIO_MODE_OUTPUT) gpio_set_level(pin, defaultHigh);
    }

    void init()
    {
        // Init pins here
        //initPin(pin, mode);
    }

    void setOutput(gpio_num_t pin, bool high)
    {
        gpio_set_level(pin, high);
    }

    bool getOutput(gpio_num_t pin)
    {
        if (pin >= 32) return GPIO_REG_READ(GPIO_OUT1_REG) & 1 << (pin - 32);
                  else return GPIO_REG_READ(GPIO_OUT_REG)  & 1 << pin;
    }

    bool getInput(gpio_num_t pin)
    {
        return gpio_get_level(pin);
    }

    bool DebouncedInput::debounce(gpio_num_t pin)
    {
        uint8_t currentRead = getInput(pin);

        debounceCount = (currentRead != settledRead && currentRead == previousRead) ? debounceCount + 1 : 0;
        previousRead = currentRead;
        
        if (debounceCount == DEBOUNCE_FILTER)
        {
            settledRead = currentRead;
            return true;
        }
        
        return false;
    }

    bool DebouncedInput::get()
    {
        return settledRead;
    }
}