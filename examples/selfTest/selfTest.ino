/**
 **************************************************
 *
 * @file        selfTest.ino
 *
 * @brief       Example showing how to run the BMM350's built-in self-test.
 *              The self-test generates an internal ~130 uT magnetic field on
 *              the X and Y channels and reports the resulting field
 *              difference; per the datasheet (section 5.1.6), a channel is
 *              considered working if its reported value is >= 130 uT.
 *
 * @link        solde.red/333359 : Soldered BMM350 Geomagnetic Sensor breakout
 *
 * @authors     Soldered Electronics
 ***************************************************/
#include <BMM350-SOLDERED.h>

// Create an instance of the BMM350 sensor
Soldered_BMM350 magSensor;

// Minimum self-test field difference to consider a channel passing,
// per the datasheet's self-test section
const float SELF_TEST_THRESHOLD_UT = 130.0;

void setup()
{
    // Initialize serial for output
    Serial.begin(115200);
    Serial.println("BMM350 Example - Self Test");

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

    // Run the self-test. The sensor automatically returns to normal mode
    // afterwards since begin() already left it there.
    struct bmm350_self_test result;
    if (magSensor.performSelfTest(&result) == BMM350_OK)
    {
        Serial.print("X axis field: ");
        Serial.print(result.out_ust_x, 2);
        Serial.print(" uT -> ");
        Serial.println(result.out_ust_x >= SELF_TEST_THRESHOLD_UT ? "PASS" : "FAIL");

        Serial.print("Y axis field: ");
        Serial.print(result.out_ust_y, 2);
        Serial.print(" uT -> ");
        Serial.println(result.out_ust_y >= SELF_TEST_THRESHOLD_UT ? "PASS" : "FAIL");
    }
    else
    {
        Serial.println("Self-test failed to run.");
    }
}

void loop()
{
}
