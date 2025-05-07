#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

extern String SSID;
extern String PASSWORD;

extern byte MAC[6];
extern const char* ServerMQTT;
extern int MqttPort;
extern const char* mqttUserName;
extern const char* mqttUserPassword;

extern String devNamePrefix;
extern String alertTopic;
extern String subTopic;  // devName + 6 digits of MAC + /Control
extern String pubTopic;
extern String globalTopic;
extern String apSSID;
extern String apPassword;
extern String deviceId;
extern String deviceAlertId;

extern const byte ssidEEPROMAdd;
extern const byte passEEPROMAdd;
extern const byte ssidLenghtEEPROMAdd;
extern const byte passwordLenghtEEPROMAdd;
extern const byte firstThresholdTempForAlertEEPROMAdd;
extern const byte buzzerBeepEEPROMAdd;
extern const byte publishIntervalEEPROMAdd;
extern const byte maxtThresholdTempForAlertEEPROMAdd;

extern const char _wifiStatusLED;
extern const char _buzzerPin;

extern bool responseOn200Request;
extern bool ledState;
extern bool alertFlag;
extern bool enterInAPMode;
extern bool globallySSIDAndPasswordChange;
extern bool espRestartFlag;
extern bool isApStarted;
extern bool isWifiConnected;
extern bool buzzerBeepFlag;
extern bool maxTempAlertFlag;

extern unsigned int previousTime;

extern byte firstThresholdTempForAlert;
extern byte secondThresholdTempForAlert;
extern byte ssidLength;
extern byte passwordLength;
extern byte wiFiRetryCounter;
extern byte mqttCounter;
extern byte responseLength;
extern byte interval;

extern String publishData;
extern String sendingTempData;

extern String alertMsg;

#endif