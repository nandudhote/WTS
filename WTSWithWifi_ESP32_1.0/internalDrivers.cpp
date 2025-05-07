#include "internalDrivers.h"
#include "Config.h"
#include "DS18B20_Sensor.h"


internalDrivers::internalDrivers() {
}

const uint8_t _temperatureSensorPins[] = { 32, 33, 25, 26 };
const byte maxSensors = sizeof(_temperatureSensorPins) / sizeof(_temperatureSensorPins[0]);
float temperatures[maxSensors] = { 0.0 };


DS18B20_Sensor sensors[maxSensors] = {
  DS18B20_Sensor(_temperatureSensorPins[0]),
  DS18B20_Sensor(_temperatureSensorPins[1]),
  DS18B20_Sensor(_temperatureSensorPins[2]),
  DS18B20_Sensor(_temperatureSensorPins[3]),
};

void internalDrivers::gpioInit() {
  pinMode(_wifiStatusLED, OUTPUT);
  pinMode(_buzzerPin, OUTPUT);
  for (int i = 0; i < maxSensors; i++) {
    sensors[i].begin();
    // pinMode(_temperatureSensorPins[i], INPUT);
    delay(1);
  }
}

void internalDrivers::readDataFromEEPROM() {
  EEPROM.begin(512);
  ssidLength = EEPROM.read(ssidLenghtEEPROMAdd);
  passwordLength = EEPROM.read(passwordLenghtEEPROMAdd);
  SSID = loadStringFromEEPROM(ssidEEPROMAdd, ssidLength);
  PASSWORD = loadStringFromEEPROM(passEEPROMAdd, passwordLength);
  byte firstThresholdTempForAlertTemp = EEPROM.read(firstThresholdTempForAlertEEPROMAdd);
  if (firstThresholdTempForAlertTemp != 0) {
    firstThresholdTempForAlert = firstThresholdTempForAlertTemp;
  }
  buzzerBeepFlag = EEPROM.read(buzzerBeepEEPROMAdd);
  byte intervalTemp = EEPROM.read(publishIntervalEEPROMAdd);
  if (intervalTemp != 0) {
    interval = intervalTemp;
  }
  byte secondThresholdTempForAlertTemp = EEPROM.read(maxThresholdTempForAlertEEPROMAdd);
  if (secondThresholdTempForAlertTemp != 0) {
    secondThresholdTempForAlert = secondThresholdTempForAlertTemp;
  }
}

void internalDrivers::sensorsReadAndValidateData() {
  for (int i = 0; i < maxSensors; i++) {
    float temp = sensors[i].readTemperature();
    if (validateAndProcessSensorData(temp)) {
      temperatures[i] = temp;
    } else {
      temperatures[i] = 0.0;
    }
    // Serial.print(i);
    // Serial.print(" : ");
    // Serial.println(temperatures[i]);
    delay(1);
  }
}

bool internalDrivers::validateAndProcessSensorData(float SensorData) {
  return (SensorData > 0.0 && SensorData < 199.9);
}

void internalDrivers::checkIfSensorValueOverThresholdAndGenerateAlert() {
  // for (int i = 0; i < maxSensors; i++) {
  if ((temperatures[0] > firstThresholdTempForAlert) || (temperatures[1] > firstThresholdTempForAlert) || (temperatures[2] > firstThresholdTempForAlert) || (temperatures[3] > firstThresholdTempForAlert)) {
    alertFlag = true;
    if (buzzerBeepFlag) {
      buzzerBeep(true);
      // Serial.println("Hooter ON");
    } else {
      buzzerBeep(false);
    }
  } else {
    alertFlag = false;
    buzzerBeep(false);
    // Serial.println("Hooter OFF");
  }
  delay(1);
  // }
}

void internalDrivers::checkIfSensorValueOverThresholdAndGenerateBoth() {
  maxTempAlertFlag = false;
  // for (int i = 0; i < maxSensors; i++) {
  if ((temperatures[0] > secondThresholdTempForAlert) || (temperatures[1] > secondThresholdTempForAlert) || (temperatures[2] > secondThresholdTempForAlert) || (temperatures[3] > secondThresholdTempForAlert)) {
    maxTempAlertFlag = true;
    buzzerBeep(maxTempAlertFlag);
  }
  delay(1);
  // }
}

void internalDrivers::buzzerBeep(bool state) {
  Serial.print("state : ");
  Serial.println(state);
  digitalWrite(_buzzerPin, state);
}

void internalDrivers::publishingDataFormat() {
  publishData = "";
  for (int i = 0; i < maxSensors; i++) {
    publishData += String(temperatures[i]);
    if (i < maxSensors - 1) {
      publishData += ":";
    }
    // sendingData += ":" + String(buzzerBeepFlag);
    delay(1);
  }
}

void internalDrivers::writeOneByteInEEPROM(int Add, byte data) {
  EEPROM.write(Add, data);
  EEPROM.commit();
}

void internalDrivers::storeStringInEEPROM(String data, byte Addr) {
  for (int i = 0; i < data.length(); i++) {
    EEPROM.write(Addr + i, data.charAt(i));
  }
  EEPROM.commit();
}

String internalDrivers::loadStringFromEEPROM(byte Addr, byte Length) {
  String readData = "";
  for (int i = Addr; i < (Addr + Length); i++) {
    readData += char(EEPROM.read(i));
  }
  return readData;
}

SplitData internalDrivers::splitStringByColon(const String& data) {
  SplitData mqttMsg;
  int firstIndex = data.indexOf(':');
  if (firstIndex != -1) {
    mqttMsg.indexOneData = data.substring(0, firstIndex);
    int secondIndex = data.indexOf(':', firstIndex + 1);
    if (secondIndex != -1) {
      mqttMsg.indexTwoData = data.substring(firstIndex + 1, secondIndex);
      mqttMsg.indexThreeData = data.substring(secondIndex + 1);
      if (mqttMsg.indexThreeData.length() > 0) {
      }
    } else {
      mqttMsg.indexTwoData = data.substring(firstIndex + 1);
    }
  } else {
    mqttMsg.indexOneData = data.substring(firstIndex + 1);
  }
  return mqttMsg;
}
