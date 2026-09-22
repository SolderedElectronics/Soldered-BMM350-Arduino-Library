/**
 **************************************************
 *
 * @file        basicReadings.ino
 *
 * @brief       Example showing how to read compensated magnetometer and
 *              temperature data from the Soldered BMM350 Geomagnetic
 *              Sensor breakout over I2C
 *
 * @link        solde.red/333359 : Soldered BMM350 Geomagnetic Sensor breakout
 *
 * @authors     Soldered Electronics
 ***************************************************/
#include <BMM350-SOLDERED.h>

// Create an instance of the BMM350 sensor
Soldered_BMM350 magSensor;

void setup()
{
    // Initialize serial for output
    Serial.begin(115200);
    Serial.println("BMM350 Example - Basic Readings");

    // Initialize I2C bus
    Wire.begin();

    // Check if sensor is connected and initialize it.
    // Address is optional, defaults to 0x14 (ADSEL tied low on this breakout)
    while (magSensor.begin() != BMM350_OK)
    {
        Serial.println("Error: BMM350 not connected, check wiring and I2C address!");
        delay(1000);
    }

    Serial.println("BMM350 connected!");
}

void loop()
{
    // Get new measurements from the sensor. This must be called before
    // accessing the magX/magY/magZ/temperature data, otherwise it will never update
    if (magSensor.getSensorData() == BMM350_OK)
    {
        Serial.print("X: ");
        Serial.print(magSensor.magX, 2);
        Serial.print(" uT\t");
        Serial.print("Y: ");
        Serial.print(magSensor.magY, 2);
        Serial.print(" uT\t");
        Serial.print("Z: ");
        Serial.print(magSensor.magZ, 2);
        Serial.print(" uT\t");
        Serial.print("Temperature: ");
        Serial.print(magSensor.temperature, 2);
        Serial.println(" degC");
    }

    // Print 10x per second
    delay(100);
}
