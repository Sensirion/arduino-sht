# arduino-sht
Repository for Sensirion humidity and temperature sensor support on Arduino

## Supported sensors:
- SHTC1
- SHTC3
- SHTW1
- SHTW2
- SHT2x (SHT20, SHT21, SHT25)
- SHT3x-DIS (I2C)
- SHT3x-ARP (ratiometric analog voltage output)
- SHT85
- SHT4x

## Installation

The recommended way to install ```arduino-sht``` is through the Library
Manager of the Arduino IDE. To access it, go to the ```Tools``` menu and
select ```Manage Libraries...```, and search for the library name there.

If you prefer to install it manually, you can download either via git or from
the releases page and place it in your Arduino/libraries directory. After
restarting the Arduino IDE, you will see the new SHTSensor menu items under
libraries and examples.

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

## Example projects

See example project
[sht-autodetect](examples/sht-autodetect/sht-autodetect.ino)

### Usage with multiple SHT31 sensors

See example project
[multiple-sht-sensors](examples/multiple-sht-sensors/multiple-sht-sensors.ino)
