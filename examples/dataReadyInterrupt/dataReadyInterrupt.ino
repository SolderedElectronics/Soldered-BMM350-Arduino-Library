/**
 **************************************************
 *
 * @file        dataReadyInterrupt.ino
 *
 * @brief       Example showing how to use the BMM350's physical interrupt
 *              pin to know when a new magnetometer reading is ready,
 *              instead of polling the interrupt status register
 *
 * @link        solde.red/333359 : Soldered BMM350 Geomagnetic Sensor breakout
 *
 * @authors     Soldered Electronics
 ***************************************************/
#include <BMM350-SOLDERED.h>

// Create an instance of the BMM350 sensor
Soldered_BMM350 magSensor;

// Pin connected to the sensor's INT pin, change to match your wiring
int interruptPin = 2;

// Flag to know when an interrupt occurs
volatile bool interruptOccurred = false;

void bmm350InterruptHandler()
{
    interruptOccurred = true;
}

void setup()
{
    // Initialize serial for output
    Serial.begin(115200);
    Serial.println("BMM350 Example - Data Ready Interrupt");

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

    // Configure the interrupt pin as push/pull, active high, pulsed and
    // mapped to the physical pin
    magSensor.configureInterrupt(BMM350_PULSED, BMM350_ACTIVE_HIGH, BMM350_INTR_PUSH_PULL, true);

    // Enable the data-ready interrupt
    magSensor.enableInterrupt(true);

    // Attach the interrupt handler to the microcontroller pin wired to INT
    attachInterrupt(digitalPinToInterrupt(interruptPin), bmm350InterruptHandler, RISING);
}

void loop()
{
    if (interruptOccurred)
    {
        interruptOccurred = false;

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
            Serial.println(" uT");
        }
    }
}
