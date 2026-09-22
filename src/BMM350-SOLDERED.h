/**
 **************************************************
 *
 * @file        BMM350-SOLDERED.h
 * @brief       Header file for the Soldered BMM350 Geomagnetic Sensor
 *              breakout board, providing a simplified I2C-only library
 *              built on top of Bosch Sensortec's BMM350_SensorAPI.
 *
 *
 * @copyright   GNU General Public License v3.0
 * @authors     Soldered Electronics
 ***************************************************/

#ifndef __BMM350_SOLDERED__
#define __BMM350_SOLDERED__

#include "Arduino.h"
#include "Wire.h"
#include "libs/bmm350-api/bmm350.h"

/**
 * @brief                   Default I2C address of the Soldered BMM350
 *                          breakout (ADSEL pin tied low).
 */
#define BMM350_SOLDERED_I2C_ADDR BMM350_I2C_ADSEL_SET_LOW

class Soldered_BMM350
{
  public:
    /**
     * @brief                       Soldered BMM350 breakout constructor.
     */
    Soldered_BMM350()
    {
    }

    /**
     * @brief                       Initializes the sensor over I2C, applying
     *                              a sane default configuration (100 Hz
     *                              output data rate, 4x averaging, all axes
     *                              enabled, normal power mode). The Soldered
     *                              BMM350 breakout only breaks out the I2C
     *                              pins, so I3C is not supported by this
     *                              wrapper.
     *
     * @param uint8_t address       I2C address of the sensor, defaults to
     *                              0x14 (ADSEL tied low on this breakout).
     * @param TwoWire &wirePort     I2C bus to use, defaults to Wire.
     *
     * @return                      BMM350_OK (0) on success, error code otherwise.
     */
    int8_t begin(uint8_t address = BMM350_SOLDERED_I2C_ADDR, TwoWire &wirePort = Wire);

    /**
     * @brief                       Performs a soft reset of the sensor.
     *
     * @return                      BMM350_OK (0) on success, error code otherwise.
     */
    int8_t softReset();

    /**
     * @brief                       Sets the sensor's power mode.
     *
     * @param bmm350_power_modes mode   One of BMM350_SUSPEND_MODE,
     *                                  BMM350_NORMAL_MODE, BMM350_FORCED_MODE
     *                                  or BMM350_FORCED_MODE_FAST.
     *
     * @return                      BMM350_OK (0) on success, error code otherwise.
     */
    int8_t setMode(enum bmm350_power_modes mode);

    /**
     * @brief                       Sets output data rate and averaging
     *                              (performance) configuration.
     *
     * @param bmm350_data_rates odr             One of the BMM350_DATA_RATE_*
     *                                          enum values.
     * @param bmm350_performance_parameters avg One of BMM350_NO_AVERAGING,
     *                                          BMM350_AVERAGING_2,
     *                                          BMM350_AVERAGING_4 or
     *                                          BMM350_AVERAGING_8.
     *
     * @return                      BMM350_OK (0) on success, error code otherwise.
     */
    int8_t setODRPerformance(enum bmm350_data_rates odr, enum bmm350_performance_parameters avg);

    /**
     * @brief                       Enables or disables individual measurement axes.
     *
     * @param bool enableX          Enable the X axis.
     * @param bool enableY          Enable the Y axis.
     * @param bool enableZ          Enable the Z axis.
     *
     * @return                      BMM350_OK (0) on success, error code otherwise.
     */
    int8_t enableAxes(bool enableX, bool enableY, bool enableZ);

    /**
     * @brief                       Reads the data-ready interrupt status.
     *                              Useful whether you're polling it directly
     *                              or checking it after the sensor's physical
     *                              interrupt pin fires.
     *
     * @param uint8_t *drdyStatus   Pointer to store the data-ready interrupt status.
     *
     * @return                      BMM350_OK (0) on success, error code otherwise.
     */
    int8_t getInterruptStatus(uint8_t *drdyStatus);

    /**
     * @brief                       Enables or disables the data-ready interrupt.
     *
     * @param bool enable           True to enable, false to disable.
     *
     * @return                      BMM350_OK (0) on success, error code otherwise.
     */
    int8_t enableInterrupt(bool enable);

    /**
     * @brief                       Configures the behavior of the sensor's
     *                              physical interrupt pin. Does not enable
     *                              the interrupt itself, see enableInterrupt().
     *
     * @param bmm350_intr_latch latching    BMM350_PULSED or BMM350_LATCHED.
     * @param bmm350_intr_polarity polarity BMM350_ACTIVE_LOW or BMM350_ACTIVE_HIGH.
     * @param bmm350_intr_drive drive       BMM350_INTR_OPEN_DRAIN or BMM350_INTR_PUSH_PULL.
     * @param bool mapToPin                 True to map the data-ready interrupt to the
     *                                      physical pin, false to unmap it.
     *
     * @return                      BMM350_OK (0) on success, error code otherwise.
     */
    int8_t configureInterrupt(enum bmm350_intr_latch latching, enum bmm350_intr_polarity polarity,
                              enum bmm350_intr_drive drive, bool mapToPin);

    /**
     * @brief                       Reads compensated magnetometer and
     *                              temperature data and stores them in the
     *                              magX/magY/magZ/temperature members.
     *
     * @return                      BMM350_OK (0) on success, error code otherwise.
     */
    int8_t getSensorData();

    /**
     * @brief                       Runs the sensor's built-in self-test.
     *
     * @param bmm350_self_test *result  Pointer to store the self-test result.
     *
     * @return                      BMM350_OK (0) on success, error code otherwise.
     */
    int8_t performSelfTest(struct bmm350_self_test *result);

    /**
     * @brief                       Last compensated magnetometer X reading in
     *                              microtesla, updated by getSensorData().
     */
    float magX = 0;

    /**
     * @brief                       Last compensated magnetometer Y reading in
     *                              microtesla, updated by getSensorData().
     */
    float magY = 0;

    /**
     * @brief                       Last compensated magnetometer Z reading in
     *                              microtesla, updated by getSensorData().
     */
    float magZ = 0;

    /**
     * @brief                       Last temperature reading in deg C,
     *                              updated by getSensorData().
     */
    float temperature = 0;

  private:
    struct bmm350_dev _dev;

    struct I2CContext
    {
        uint8_t address;
        TwoWire *wire;
    } _i2cContext;

    static BMM350_INTF_RET_TYPE i2cRead(uint8_t regAddr, uint8_t *readData, uint32_t len, void *intfPtr);
    static BMM350_INTF_RET_TYPE i2cWrite(uint8_t regAddr, const uint8_t *data, uint32_t len, void *intfPtr);
    static void delayUs(uint32_t period, void *intfPtr);
};

#endif
