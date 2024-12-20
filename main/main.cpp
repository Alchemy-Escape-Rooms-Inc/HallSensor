
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "allegro/als31300.h"

#include "peripherals/i2c.h"
#include "peripherals/pins.h"

void i2cScan()
{
    // I2C Scanner
    for (int i = 0; i < 128; i++)
    {
        if (I2C::devicePresent(i))
        {
            printf("Device found at %i\n", i);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void setAddress(uint8_t previousAddress, uint8_t newAddress)
{
    ALS31300::Sensor sensor(previousAddress);
    if (sensor.programAddress(newAddress)) printf("Sensor address %i set successfully!", newAddress);
    else printf("Sensor address %i failed!", newAddress);
}

extern "C" void app_main()
{
    Pins::init();
    I2C::init();

    // Setup I2C callbacks
    ALS31300::Sensor::setCallbacks(I2C::registerDevice, I2C::unregisterDevice, I2C::changeAddress, I2C::write, I2C::read);

    if (I2C::devicePresent(0))
    {
        // BAC Compatible addresses should be 96-111
        setAddress(0, 96);
        // Power cycle after this
    }

    ALS31300::Sensor sensor(96);

    while (1)
    {
        if (sensor.update())
        {
            printf("Sensor %i angle: %03d, Depth: %05d\n", sensor.address, sensor.getAngle(), (int) sensor.z);
        }

        //i2cScan();
    }
}
