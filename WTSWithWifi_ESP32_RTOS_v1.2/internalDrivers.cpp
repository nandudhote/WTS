#include "internalDrivers.h"
#include "Config.h"
#include "DS18B20_Sensor.h"

internalDrivers iiiiDrivers;

internalDrivers::internalDrivers() {
}

const uint8_t _temperatureSensorPins[] = { 32, 33, 25, 26, 27, 14, 13, 23, 22, 21 };
const byte maxSensors = sizeof(_temperatureSensorPins) / sizeof(_temperatureSensorPins[0]);
float temperatures[maxSensors] = { 0.0 };

DS18B20_Sensor sensors[maxSensors] = {
  DS18B20_Sensor(_temperatureSensorPins[0]),
  DS18B20_Sensor(_temperatureSensorPins[1]),
  DS18B20_Sensor(_temperatureSensorPins[2]),
  DS18B20_Sensor(_temperatureSensorPins[3]),
  DS18B20_Sensor(_temperatureSensorPins[4]),
  DS18B20_Sensor(_temperatureSensorPins[5]),
  DS18B20_Sensor(_temperatureSensorPins[6]),
  DS18B20_Sensor(_temperatureSensorPins[7]),
  DS18B20_Sensor(_temperatureSensorPins[8]),
  DS18B20_Sensor(_temperatureSensorPins[9])
};

void internalDrivers::gpioInit() {
  pinMode(_wifiStatusLED, OUTPUT);
  pinMode(_buzzerPin, OUTPUT);
  digitalWrite(_buzzerPin, LOW);
}

void internalDrivers::DS18B20SensorInit() {
  for (int i = 0; i < maxSensors; i++) {
    sensors[i].begin();
    delay(10);
  }
}

void internalDrivers::readDataFromEEPROM() {
  EEPROM.begin(512);
  if (((SSID != "") && (PASSWORD != "")) || ((ssidLength != 255) || (passwordLength != 255))) {
    ssidLength = EEPROM.read(ssidLenghtEEPROMAdd);
    passwordLength = EEPROM.read(passwordLenghtEEPROMAdd);
    SSID = loadStringFromEEPROM(ssidEEPROMAdd, ssidLength);
    PASSWORD = loadStringFromEEPROM(passEEPROMAdd, passwordLength);
  }
  byte firstThresholdTempForAlertTemp = EEPROM.read(firstThresholdTempForAlertEEPROMAdd);
  if (firstThresholdTempForAlertTemp != 0) {
    firstThresholdTempForAlert = firstThresholdTempForAlertTemp;
  }
  byte secondThresholdTempForAlertTemp = EEPROM.read(maxtThresholdTempForAlertEEPROMAdd);
  if (secondThresholdTempForAlertTemp != 0) {
    secondThresholdTempForAlert = secondThresholdTempForAlertTemp;
  }
  buzzerBeepFlag = EEPROM.read(buzzerBeepEEPROMAdd);
  byte intervalTemp = EEPROM.read(publishIntervalEEPROMAdd);
  if (intervalTemp != 0) {
    interval = intervalTemp;
  }
  byte activeSensorTemp = EEPROM.read(activeSensorEEPROMAdd);
  if (activeSensorTemp != 0) {
    activeSensor = activeSensorTemp;
  }
}

void internalDrivers::sensorsReadAndValidateData() {
  for (int i = 0; i < activeSensor; i++) {
    float temp = sensors[i].readTemperature();
    if (validateAndProcessSensorData(temp)) {
      temperatures[i] = temp;
    } else {
      temperatures[i] = 0.0;
    }
    // //Serial.print(i);
    // //Serial.print(" : ");
    // //Serial.println(temperatures[i]);
    delay(1);
  }
}

bool internalDrivers::validateAndProcessSensorData(float SensorData) {
  return (SensorData > 0.0 && SensorData < 199.9);
}

void internalDrivers::checkIfSensorValueOverThresholdAndGenerateAlert() {
  alertFlag = false;  // Start assuming there is no alert.
  for (int i = 0; i < activeSensor; i++) {
    if (temperatures[i] > firstThresholdTempForAlert) {
      alertFlag = true;  // Found one sensor above threshold.
      break;             // No need to check more, break the loop.
    }
    delay(1);  // Small delay
  }
  buzzerBeep(alertFlag && buzzerBeepFlag);  // Turn buzzer ON only if alert is true and buzzerBeepFlag is true.
}

void internalDrivers::checkIfSensorValueOverThresholdAndGenerateBoth() {
  maxTempAlertFlag = false;
  for (int i = 0; i < activeSensor; i++) {
    if (temperatures[i] > secondThresholdTempForAlert) {
      maxTempAlertFlag = true;
      buzzerBeep(maxTempAlertFlag);  // Always ON if max threshold crossed
      break;
    }
    delay(1);
  }
}

void internalDrivers::buzzerBeep(bool state) {
  digitalWrite(_buzzerPin, state);
}

String internalDrivers::publishingDataFormat() {
  publishData = "";
  for (int i = 0; i < activeSensor; i++) {
    publishData += String(temperatures[i]);
    if (i < activeSensor - 1) {
      publishData += ":";
    }
    delay(1);
  }
  return publishData;
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
