
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
    if (sensor.programAddress(newAddress)) printf("Sensor address %i set successfully!\n", newAddress);
    else printf("Sensor address %i failed!\n", newAddress);
}

void processSensor(ALS31300::Sensor& sensor)
{
    if (sensor.update())
    {
        printf("Sensor %i angle: %03d, Depth: %05d\n", sensor.address, sensor.getAngle(), (int) sensor.z);
    }
}

extern "C" void app_main()
{
    Pins::init();
    I2C::init();

    // Setup I2C callbacks
    ALS31300::Sensor::setCallbacks(I2C::registerDevice, I2C::unregisterDevice, I2C::changeAddress, I2C::write, I2C::read);

    uint8_t oldAddress = 0;
    uint8_t newAddress = 4;

    if (I2C::devicePresent(oldAddress))
    {
        // BAC Compatible addresses should be 96-111
        setAddress(oldAddress, newAddress);
        printf("Please power cycle to ensure new address took.\n"); // Power cycle after this
    }
    else
    {
        printf("Sensor address 0 not found, skipping programming!\n");
    }

    ALS31300::Sensor sensor1(1);
    ALS31300::Sensor sensor2(2);
    ALS31300::Sensor sensor3(3);
    ALS31300::Sensor sensor4(4);

    while (1)
    {
        processSensor(sensor1);
        processSensor(sensor2);
        processSensor(sensor3);
        processSensor(sensor4);

        i2cScan();
    }
}
