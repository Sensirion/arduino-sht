# arduino-sht
Repository for Sensirion humidity and temperature sensor support on Arduino

> [!WARNING]
> arduino-sht is scheduled to be archived in the near future. We recommend users to transition to the dedicated Arduino libraries https://github.com/Sensirion/arduino-i2c-sht3x and https://github.com/Sensirion/arduino-i2c-sht4x

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

For <code><a href="https://github.com/Sensirion/arduino-i2c-sht3x">sht3x</a></code> and <code><a href="https://github.com/Sensirion/arduino-i2c-sht4x">sht4x</a></code> there are specific drivers available in separate repositories.

## User visible changes in 2.0.0

### mSensorType changed to private, added getSensorType()

`mSensorType` was never meant to be exposed to users of the library, but has been in the past
few releases, with users mentioning it's use in their application(s). This release fixes the
exposed member variable and adds a getter to retain the functionality.

If you're using mSensorType today, simply replace with a call to `getSensorType()`

### getHumidity() and getTemperature() behavior

In the past, calls to getHumidity() and getTemperature() calls returned the last valid
measurement - assuming one had taken place - even if a readSample() call failed. The new
behavior is such that those two functions return `NAN` after when readSample() fails.

Applications that used the caching behavior will need to be updated to implement local
caching.


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
3. Also in `setup()`, call `sht.init()` 
5. If you want to use the serial console, remember to initialize the Serial
   library with `Serial.begin(9600)`
1. Call `sht.readSample()` in the `loop()` function, which reads a temperature
   and humidity sample from the sensor
2. Use `sht.getHumidity()` and `sht.getTemperature()` to get the values from
   the last sample

*Important:* `getHumidity()` and `getTemperature()` do *not* read a new sample
from the sensor, but return the values read last. To read a new sample, make
sure to call `readSample()`

### Using an custom or alternative I2C port/Wire instance

Some Arduino boards have multiple predefined I2C ports; generally, the second port will be called `Wire1`.

The `arduino-sht` library allows to use an alternative interface; to do so, pass the port you want to use as an argument to `sht.init()`, like this:
```
  if (sht.init(Wire1)) {
      Serial.print("init(): success\n");
  } else {
      Serial.print("init(): failed\n");
  }
```

## Example projects

See example project
[sht-autodetect](examples/sht-autodetect/sht-autodetect.ino)

### Usage with multiple SHT31 sensors

See example project
[multiple-sht-sensors](examples/multiple-sht-sensors/multiple-sht-sensors.ino)
