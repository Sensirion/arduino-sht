# arduino-sht
Repository for Sensirion humidity and temperature sensor support on Arduino

## Supported sensors:
- SHTC1
- SHTW1
- SHTW2
- SHT3x-DIS (I2C)
- SHT3x-ARP (ratiometric analog voltage output)

## Installation

Download arduino-sht either via git or from the releases page and place it in
your Arduino/libraries directory. After restarting the Arduino IDE, you will see
the new SHTSensor menu items under libraries and examples.

## Integrating it into your sketch

Assuming you installed the library as described above, the following steps are
necessary:

1. Import the Wire library like this: From the menu bar, select Sketch > Import
   Library > Wire
1. Import the arduino-sht library: From the menu bar, select Sketch >
   Import Library > arduino-sht
1. Create an instance of the `SHTSensor` class (`SHTSensor sht;`)
2. In `setup()`, make sure to init the Wire library with `Wire.begin()`
3. If you want to use the serial console, remember to initialize the Serial
   library with `Serial.begin(9600)`
1. Call `sht.readSample()` in the `loop()` function, which reads a temperature
   and humidity sample from the sensor
2. Use `sht.getHumidity()` and `sht.getTemperature()` to get the values from
   the last sample

*Important:* `getHumidity()` and `getTemperature()` do *not* read a new sample
from the sensor, but return the values read last. To read a new sample, make
sure to call `readSample()`

### Sample code
```c++
#include <Wire.h>

#include <SHTSensor.h>

SHTSensor sht;

void setup() {
  // put your setup code here, to run once:

  Wire.begin();
  Serial.begin(9600);
  sht.init();
}

void loop() {
  // put your main code here, to run repeatedly:

  sht.readSample();
  Serial.print("SHT:\n");
  Serial.print("  RH: ");
  Serial.print(sht.getHumidity(), 2);
  Serial.print("\n");
  Serial.print("  T:  ");
  Serial.print(sht.getTemperature(), 2);
  Serial.print("\n");

  delay(1000);
}
```

### Usage with two SHT31 sensors

```c++
#include <Wire.h>
#include "SHTSensor.h"

// Sensor with normal i2c address
// Sensor 1 with address pin pulled to GND
SHTSensor sht1(SHTSensor::SHT3X);

// Sensor with alternative i2c address
// Sensor 2 with address pin pulled to Vdd
SHTSensor sht2(SHTSensor::SHT3X_ALT);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  delay(1000); // let serial console settle

  // init on a specific sensor type (i.e. without auto detecting), 
  // does not check if the sensor is responding and will thus always succeed.

  // initialize sensor with normal i2c-address
  sht1.init();

  // initialize sensor with alternative i2c-address
  sht2.init();
}
void loop() {
  // put your main code here, to run repeatedly:
  // read from first sensor
  if (sht1.readSample()) {
    Serial.print("SHT1 :\n");
    Serial.print("  RH: ");
    Serial.print(sht1.getHumidity(), 2);
    Serial.print("\n");
    Serial.print("  T:  ");
    Serial.print(sht1.getTemperature(), 2);
    Serial.print("\n");
  } else {
    Serial.print("Sensor 1: Error in readSample()\n");
  }
  // read from second sensor
  if (sht2.readSample()) {
    Serial.print("SHT2:\n");
    Serial.print("  RH: ");
    Serial.print(sht2.getHumidity(), 2);
    Serial.print("\n");
    Serial.print("  T:  ");
    Serial.print(sht2.getTemperature(), 2);
    Serial.print("\n");
  } else {
    Serial.print("Sensor 2: Error in readSample()\n");
  }
  delay(1000);
}
```
