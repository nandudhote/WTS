/*
   Name Of Project: WTS with esp32 for wifi based
   MCU used: ESP32
   Functions: Read Temperature and publish Also generate alert if temprature is more then threshold temprature
   Programmer: Nandkishor Dhote
   Author: EVOLUZN INDIA PRIVATE LIMITED
   Firmware Status: 
*/

#include "Config.h"
#include "internalDrivers.h"
#include "myWIFI.h"
#include "accessPoint.h"

ACCESSPOINT AP;
myWIFI wiFI;
internalDrivers iDrivers;
subPubTopics Topic;

void setup() {
  Serial.begin(9600);  // for debugging
  iDrivers.gpioInit();
  iDrivers.readDataFromEEPROM();
  wiFI.wiFiSetup(SSID, PASSWORD);
  apSSID = wiFI.prepareDevID(MAC, apSSID);
  deviceId = wiFI.prepareDevID(MAC, devNamePrefix);
  deviceAlertId = wiFI.prepareDevID(MAC, alertTopic);
  AP.accessPointSetup();
  wiFI.mqttSetup(ServerMQTT, MqttPort);
  Topic = wiFI.createSubPubTopics(deviceId, subTopic, pubTopic, globalTopic, deviceAlertId);  // establish the connections with Mqtt
  wiFI.reconnectToMqtt(Topic.Publish, Topic.Subscribe, Topic.Global, Topic.Alert);
}

void loop() {
  wiFI.clientLoop();
  iDrivers.sensorsReadAndValidateData();
  iDrivers.checkIfSensorValueOverThresholdAndGenerateAlert();
  iDrivers.checkIfSensorValueOverThresholdAndGenerateBoth();

  AP.updateTheSSIDAndPASSFromMqttIfAvailable();
  if (enterInAPMode) {
    mqttCounter = 0;
    wiFI.earasWiFiCredentialsFromEEPROM();
    SSID = "";
    PASSWORD = "";
    AP.accessPointSetup();
    enterInAPMode = false;
  }
  iDrivers.publishingDataFormat();
  /* One Mints Millis loop */
  unsigned long currentTime = millis();
  if (currentTime - previousTime >= (interval * 1000) || responseOn200Request || !isWifiConnected || alertFlag || alertMsg != "" || maxTempAlertFlag) {
    if (wiFI.wiFiLinkCheck()) {
      isWifiConnected = true;
      digitalWrite(_wifiStatusLED, HIGH);
      if (isApStarted) {
        wiFiRetryCounter = 0;
        isApStarted = false;
        AP.stopApServer();
        AP.stopApWiFi();
      }
      if (wiFI.CheckMQTTConnection()) {
        if (alertMsg != "") {
          wiFI.publishMqttMsg_Alert(Topic.Publish, deviceId, alertMsg);
          alertMsg = "";
        }
        if (alertFlag) {
          wiFI.publishMqttMsg_Alert(Topic.Alert, deviceAlertId, "Temprature is > " + String(firstThresholdTempForAlert));
          alertFlag = false;
        }
        if (maxTempAlertFlag) {
          wiFI.publishMqttMsg_Alert(Topic.Alert, deviceAlertId, "Temprature is > " + String(secondThresholdTempForAlert));
          maxTempAlertFlag = false;
        }
        if ((currentTime - previousTime >= (interval * 1000)) || responseOn200Request) {
          if (responseOn200Request) {
            wiFI.publishMqttMsg(Topic.Publish, "200", publishData);
            responseOn200Request = false;
          } else {
            wiFI.publishMqttMsg(Topic.Publish, deviceId, publishData);
            previousTime = currentTime;
          }
        }
      } else {
        if (!wiFI.CheckMQTTConnection()) {
          wiFI.reconnectToMqtt(Topic.Publish, Topic.Subscribe, Topic.Global, Topic.Alert);
        } else {
          delay(100);
        }
      }
    } else {
      wiFI.wiFiSetup(SSID, PASSWORD);
      wiFiRetryCounter += 1;
      if (wiFiRetryCounter >= 2 && (!isApStarted)) {
        AP.readSsidAndPasswordFromAP(apSSID.c_str(), apPassword.c_str());
        isApStarted = true;
      }
    }
  }
  delay(10);
}
