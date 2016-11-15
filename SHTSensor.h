/*
 *  Copyright (c) 2016, Sensirion AG <andreas.brauchli@sensirion.com>
 *  Copyright (c) 2015, Johannes Winkelmann <jw@smts.ch>
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *      * Neither the name of the <organization> nor the
 *        names of its contributors may be used to endorse or promote products
 *        derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef SHTSENSOR_H
#define SHTSENSOR_H

#include <inttypes.h>

// Forward declaration
class SHTSensorDriver;

/**
 * Official interface for Sensirion SHT Sensors
 */
class SHTSensor
{
public:
  /**
   * Enum of the supported Sensirion SHT Sensors.
   * Using the special AutoDetect sensor causes all i2c sensors to be
   * probed. The first matching sensor will then be used.
   */
  enum SHTSensorType {
    /** Automatically detect the sensor type (only i2c sensors listed above) */
    AutoDetect,
    // i2c Sensors:
    /** SHT3x-DIS with ADDR (sensor pin 2) connected to VSS (default) */
    SHT3x,
    /** SHT3x-DIS with ADDR (sensor pin 2) connected to VDD */
    SHT3xAlt,
    SHTC1,
    SHTW1,
    SHTW2,
    // Analog sensors:
    SHT3xAnalog
  };

  /**
   * Accuracy setting of measurement.
   * Not all sensors support changing the sampling accuracy.
   */
  enum SHTAccuracy {
    /** Highest repeatability at the cost of slower measurement */
    SHT_ACCURACY_HIGH,
    /** Balanced repeatability and speed of measurement */
    SHT_ACCURACY_MEDIUM,
    /** Fastest measurement but lowest repeatability */
    SHT_ACCURACY_LOW
  };

  /** Value reported by getHumidity() when the sensor is not initialized */
  static const float HUMIDITY_INVALID;
  /** Value reported by getTemperature() when the sensor is not initialized */
  static const float TEMPERATURE_INVALID;
  /**
   * Auto-detectable sensor types.
   * Note that the SHTW1 and SHTW2 share exactly the same driver as the SHTC1
   * and are thus not listed individually.
   */
  static const SHTSensorType AUTO_DETECT_SENSORS[];

  /**
   * Instantiate a new SHTSensor
   * By default, the i2c bus is queried for known SHT Sensors. To address
   * a specific sensor, set the `sensorType'.
   *
   * Some sensor drivers require a specific parameter set and cannot be
   * auto-detected. Those must be instantiated separately and be passed to
   * `sensor'. In any other case sensor must be NULL.
   *
   * `ownSensor' describes whether the SHTSensor is responsible for freeing the
   * memory pointed to by `sensor'. This should never need to be set explicitly.
   */
  SHTSensor(SHTSensorType sensorType = AutoDetect,
            SHTSensorDriver *sensor = NULL,
            bool ownSensor = false)
      : mTemperature(SHTSensor::TEMPERATURE_INVALID),
        mHumidity(SHTSensor::HUMIDITY_INVALID),
        mSensorType(sensorType),
        mSensor(sensor),
        mOwnSensor(ownSensor)
  {
  }

  virtual ~SHTSensor() {
    cleanup();
  }

  /**
   * Initialize a new Temperature and Humidity sensor driver
   * To read out the sensor use readSample(), then use getTemperature() and
   * getHumidity() to retrieve the values from the sample
   */
  bool init(SHTSensorDriver *driver = NULL);

  /**
   * Read new values from the sensor
   * After the call, use getTemperature() and getHumidity() to retrieve the
   * values
   * Returns true if the sample was read and the values are cached
   */
  bool readSample();

  /**
   * Get the relative humidity in percent read from the last sample
   * Use readSample() to trigger a new sensor reading
   */
  float getHumidity() const {
    return mHumidity;
  }

  /**
   * Get the humidity in percent read from the last sample
   * Use readSample() to trigger a new sensor reading
   */
  float getTemperature() const {
    return mTemperature;
  }

  /**
   * Change the sensor accurancy, if supported by the sensor
   * Returns true if the accuracy was changed
   */
  bool setAccuracy(SHTAccuracy newAccuracy);

  /**
   * Set the i2c address the driver uses to communicate with the sensor,
   * if supported by the driver
   * Note: this will only cause the communication to address a different i2c
   * device but not cause the sensor to change it's listening address.
   * Returns true if the sensor address was changed
   */
  bool setI2cAddress(uint8_t newAddress);

private:
  void cleanup();

  bool mOwnSensor;
  SHTSensorDriver *mSensor;
  SHTSensorType mSensorType;
  float mTemperature;
  float mHumidity;
};

// I2c addresses used by the SHT3x-DIS
const uint8_t SHT3x_I2C_ADDRESS_44 = 0x44;
const uint8_t SHT3x_I2C_ADDRESS_45 = 0x45;

/** Abstract class for an SHT Sensor driver */
class SHTSensorDriver
{
public:
  virtual ~SHTSensorDriver() = 0;

  /**
   * Set the sensor accuracy.
   * Returns false if the sensor does not support changing the accuracy
   */
  virtual bool setAccuracy(SHTSensor::SHTAccuracy newAccuracy) {
    return false;
  }

  /**
   * Set the i2c communication address.
   * Returns false if the sensor driver does not support different addresses
   */
  virtual bool setI2cAddress(uint8_t newAddress) {
    return false;
  }

  /** Returns true if the next sample was read and the values are cached */
  virtual bool readSample();

  /**
   * Get the relative humidity in percent read from the last sample
   * Use readSample() to trigger a new sensor reading
   */
  float getHumidity() const {
    return mHumidity;
  }

  /**
   * Get the humidity in percent read from the last sample
   * Use readSample() to trigger a new sensor reading
   */
  float getTemperature() const {
    return mTemperature;
  }

  float mTemperature;
  float mHumidity;
};

/** Base class for i2c SHT Sensor drivers */
class SHTI2cSensor : public SHTSensorDriver {
public:
  /** Size of i2c commands to send */
  static const uint8_t CMD_SIZE;

  /** Size of i2c replies to expect */
  static const uint8_t EXPECTED_DATA_SIZE;

  /**
   * Constructor for i2c SHT Sensors
   * Takes the `i2cAddress' to read, the `i2cCommand' issues when sampling
   * the sensor and the values `a', `b', `c' to convert the fixed-point
   * temperature value received by the sensor to a floating point value using
   * the formula: temperature = a + b * (rawTemperature / c)
   * and the values `x' and `y' to convert the fixed-point humidity value
   * received by the sensor to a floating point value using the formula:
   * humidity = x * (rawHumidity / y)
   */
  SHTI2cSensor(uint8_t i2cAddress, uint16_t i2cCommand,
               float a, float b, float c,
               float x, float y)
      : mI2cAddress(i2cAddress), mI2cCommand(i2cCommand),
        mA(a), mB(b), mC(c), mX(x), mY(y)
  {
  }

  virtual ~SHTI2cSensor()
  {
  }

  virtual bool readSample();

  virtual bool setI2cAddress(uint8_t i2cAddress) {
    mI2cAddress = i2cAddress;
    return true;
  }

  uint8_t mI2cAddress;
  uint16_t mI2cCommand;
  float mA;
  float mB;
  float mC;
  float mX;
  float mY;

private:
  static const uint8_t MAX_I2C_READ_TRIES;
  static uint8_t crc8(const uint8_t *data, uint8_t len);
  static bool readFromI2c(uint8_t i2cAddress,
                          const uint8_t *i2cCommand,
                          uint8_t commandLength, uint8_t *data,
                          uint8_t dataLength);
};

class SHT3xAnalogSensor : public SHTSensorDriver
{
public:

  /**
   * Instantiate a new Sensirion SHT3x Analog sensor driver instance.
   * The required paramters are `humidityPin` and `temperaturePin`
   * An optional `readResolutionBits' can be set since the Arduino/Genuino Zero
   * support 12bit precision analog readings. By default, 10 bit precision is
   * used.
   * The analog instance can then (a) be used directly or (b) be passed to the
   * SHTDriver instance for transparent usage accross SHT drivers:
   *
   * Example (a):
   * SHT3xAnalogSensor sht3xAnalog(HUMIDITY_PIN, TEMPERATURE_PIN);
   * float humidity = sht.readHumidity();
   *
   * Example (b):
   * SHT3xAnalogSensor shtDriver(HUMIDITY_PIN, TEMPERATURE_PIN);
   * SHTDriver sht(SHTDriver::SHT3xAnalog);
   * sht.init(&shtDriver)
   * sht.readSample();
   * float temperature = sht.getTemperature();
   */
  SHT3xAnalogSensor(uint8_t humidityPin, uint8_t temperaturePin,
                    uint8_t readResolutionBits = 10)
      : mHumidityAdcPin(humidityPin), mTemperatureAdcPin(temperaturePin),
        mReadResolutionBits(readResolutionBits)
  {
  }

  virtual ~SHT3xAnalogSensor()
  {
  }

  virtual bool readSample();

  float readHumidity();
  float readTemperature();

  uint8_t mHumidityAdcPin;
  uint8_t mTemperatureAdcPin;
  uint8_t mReadResolutionBits;
};

#endif /* SHTSENSOR_H */