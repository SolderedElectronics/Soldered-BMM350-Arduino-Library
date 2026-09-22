/**
 **************************************************
 *
 * @file        BMM350-SOLDERED.cpp
 * @brief       Source file for the Soldered BMM350 Geomagnetic Sensor
 *              breakout board.
 *
 *
 * @copyright   GNU General Public License v3.0
 * @authors     Soldered Electronics
 ***************************************************/

#include "BMM350-SOLDERED.h"

int8_t Soldered_BMM350::begin(uint8_t address, TwoWire &wirePort)
{
    _i2cContext.address = address;
    _i2cContext.wire = &wirePort;

    _dev.intf_ptr = &_i2cContext;
    _dev.read = i2cRead;
    _dev.write = i2cWrite;
    _dev.delay_us = delayUs;

    int8_t result = bmm350_init(&_dev);
    if (result != BMM350_OK)
    {
        return result;
    }

    result = setODRPerformance(BMM350_DATA_RATE_100HZ, BMM350_AVERAGING_4);
    if (result != BMM350_OK)
    {
        return result;
    }

    result = enableAxes(true, true, true);
    if (result != BMM350_OK)
    {
        return result;
    }

    return setMode(BMM350_NORMAL_MODE);
}

int8_t Soldered_BMM350::softReset()
{
    return bmm350_soft_reset(&_dev);
}

int8_t Soldered_BMM350::setMode(enum bmm350_power_modes mode)
{
    return bmm350_set_powermode(mode, &_dev);
}

int8_t Soldered_BMM350::setODRPerformance(enum bmm350_data_rates odr, enum bmm350_performance_parameters avg)
{
    return bmm350_set_odr_performance(odr, avg, &_dev);
}

int8_t Soldered_BMM350::enableAxes(bool enableX, bool enableY, bool enableZ)
{
    return bmm350_enable_axes(enableX ? BMM350_X_EN : BMM350_X_DIS, enableY ? BMM350_Y_EN : BMM350_Y_DIS,
                              enableZ ? BMM350_Z_EN : BMM350_Z_DIS, &_dev);
}

int8_t Soldered_BMM350::getInterruptStatus(uint8_t *drdyStatus)
{
    return bmm350_get_interrupt_status(drdyStatus, &_dev);
}

int8_t Soldered_BMM350::enableInterrupt(bool enable)
{
    return bmm350_enable_interrupt(enable ? BMM350_ENABLE_INTERRUPT : BMM350_DISABLE_INTERRUPT, &_dev);
}

int8_t Soldered_BMM350::configureInterrupt(enum bmm350_intr_latch latching, enum bmm350_intr_polarity polarity,
                                           enum bmm350_intr_drive drive, bool mapToPin)
{
    return bmm350_configure_interrupt(latching, polarity, drive, mapToPin ? BMM350_MAP_TO_PIN : BMM350_UNMAP_FROM_PIN,
                                      &_dev);
}

int8_t Soldered_BMM350::getSensorData()
{
    struct bmm350_mag_temp_data data = {0};

    int8_t result = bmm350_get_compensated_mag_xyz_temp_data(&data, &_dev);
    if (result != BMM350_OK)
    {
        return result;
    }

    magX = data.x;
    magY = data.y;
    magZ = data.z;
    temperature = data.temperature;

    return BMM350_OK;
}

int8_t Soldered_BMM350::performSelfTest(struct bmm350_self_test *result)
{
    return bmm350_perform_self_test(result, &_dev);
}

BMM350_INTF_RET_TYPE Soldered_BMM350::i2cRead(uint8_t regAddr, uint8_t *readData, uint32_t len, void *intfPtr)
{
    I2CContext *ctx = (I2CContext *)intfPtr;

    ctx->wire->beginTransmission(ctx->address);
    ctx->wire->write(regAddr);
    if (ctx->wire->endTransmission(false) != 0)
    {
        return BMM350_E_COM_FAIL;
    }

    uint8_t bytesRead = ctx->wire->requestFrom(ctx->address, (uint8_t)len);
    if (bytesRead != len)
    {
        return BMM350_E_COM_FAIL;
    }

    for (uint32_t i = 0; i < len; i++)
    {
        readData[i] = ctx->wire->read();
    }

    return BMM350_OK;
}

BMM350_INTF_RET_TYPE Soldered_BMM350::i2cWrite(uint8_t regAddr, const uint8_t *data, uint32_t len, void *intfPtr)
{
    I2CContext *ctx = (I2CContext *)intfPtr;

    ctx->wire->beginTransmission(ctx->address);
    ctx->wire->write(regAddr);
    ctx->wire->write(data, (size_t)len);

    return (ctx->wire->endTransmission() == 0) ? BMM350_OK : BMM350_E_COM_FAIL;
}

void Soldered_BMM350::delayUs(uint32_t period, void *intfPtr)
{
    (void)intfPtr;

    if (period >= 1000)
    {
        delay(period / 1000);
        period %= 1000;
    }
    delayMicroseconds(period);
}
