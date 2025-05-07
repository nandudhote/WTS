#ifndef INTERNALDRIVERS_H
#define INTERNALDRIVERS_H

#include "Arduino.h"
#include <EEPROM.h>
#include <OneWire.h>            // Include the OneWire library to communicate with the DS18B20 sensor
#include <DallasTemperature.h>  // Include the DallasTemperature library to easily manage the DS18B20 sensor


struct SplitData {
  String indexOneData;
  String indexTwoData;
  String indexThreeData;
};

class internalDrivers {
public:
  internalDrivers();
  void gpioInit();
  void readDataFromEEPROM();
  void sensorsReadAndValidateData();
  bool validateAndProcessSensorData(float SensorData);
  void checkIfSensorValueOverThresholdAndGenerateAlert();
  void checkIfSensorValueOverThresholdAndGenerateBoth();
  void buzzerBeep(bool state);
  void publishingDataFormat();
  void writeOneByteInEEPROM(int Add, byte data);
  void storeStringInEEPROM(String data, byte Addr);
  String loadStringFromEEPROM(byte Addr, byte Length);
  SplitData splitStringByColon(const String& data);
};

#endif
