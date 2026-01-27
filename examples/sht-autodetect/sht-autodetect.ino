#include <Wire.h>

#include "SHTSensor.h"

SHTSensor sht;
// To use a specific sensor instead of probing the bus use this command:
// SHTSensor sht(SHTSensor::SHT3X);

String getSensorName(SHTSensor::SHTSensorType);

void setup() {
  // put your setup code here, to run once:

  Wire.begin();
  Serial.begin(9600);
  delay(1000); // let serial console settle

  if (sht.init()) {
      Serial.print("init(): success\n");
      Serial.println("Sensor detected: " + getSensorName(sht.getSensorType()));
  } else {
      Serial.print("init(): failed\n");
  }
  sht.setAccuracy(SHTSensor::SHT_ACCURACY_MEDIUM); // only supported by SHT3x

}

void loop() {
  // put your main code here, to run repeatedly:

  if (sht.readSample()) {
      Serial.print("SHT:\n");
      Serial.print("  RH: ");
      Serial.print(sht.getHumidity(), 2);
      Serial.print("\n");
      Serial.print("  T:  ");
      Serial.print(sht.getTemperature(), 2);
      Serial.print("\n");
  } else {
      Serial.print("Error in readSample()\n");
  }

  delay(1000);
}

String getSensorName(SHTSensor::SHTSensorType sensorType) {
  switch (sensorType) {
    case SHTSensor::SHTSensorType::SHT2X:
      return "SHT2x";

    case SHTSensor::SHTSensorType::SHT3X:
    case SHTSensor::SHTSensorType::SHT85:
      return "SHT3x/SHT85 (I2C address 0x44)";

    case SHTSensor::SHTSensorType::SHT3X_ALT:
      return "SHT3x (I2C address 0x44)";

    case SHTSensor::SHTSensorType::SHTW1:
    case SHTSensor::SHTSensorType::SHTW2:
    case SHTSensor::SHTSensorType::SHTC1:
    case SHTSensor::SHTSensorType::SHTC3:
      return "SHTC1/SHTC3/SHTW1/SHTW2\n";

    case SHTSensor::SHTSensorType::SHT4X:
      return "SHT4x";
  }

  return "Unknown sensor type (" + String(sensorType) + "); please report on https://github.com/Sensirion/arduino-sht";
}
