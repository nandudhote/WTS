#include <sys/_intsup.h>
#include "Config.h"

String SSID = "";
String PASSWORD = "";

byte MAC[6] = { 0x8C, 0x4C, 0xAD, 0xF0, 0xBF, 0x30 };
// const char* ServerMQTT = "203.109.124.70";
const char* ServerMQTT = "evoluzn.org";
int MqttPort = 18889;
const char* mqttUserName = "evzin_led";
const char* mqttUserPassword = "63I9YhMaXpa49Eb";

String devNamePrefix = "WTS";
String alertTopic = "WTSAlert";
String subTopic = "/control";  // devName + 6 digits of MAC + /Control
String pubTopic = "/status";   // // devName + 6 digits of MAC + /Status
String globalTopic = "WTSGlobal";
String apSSID = "WTSAp";
String apPassword = "123456789";
String deviceId = "";
String deviceAlertId = "";

const byte ssidEEPROMAdd = 00;
const byte passEEPROMAdd = 21;
const byte ssidLenghtEEPROMAdd = 41;
const byte passwordLenghtEEPROMAdd = 42;
const byte firstThresholdTempForAlertEEPROMAdd = 43;
const byte buzzerBeepEEPROMAdd = 44;
const byte publishIntervalEEPROMAdd = 45;
const byte maxtThresholdTempForAlertEEPROMAdd = 46;
const byte activeSensorEEPROMAdd = 47;

// const char _wifiStatusLED = 27;
const char _wifiStatusLED = 4;
const char _buzzerPin = 15;

bool responseOn200Request = false;
bool ledState = false;
bool alertFlag = false;
bool enterInAPMode = false;
bool globallySSIDAndPasswordChange = false;
bool espRestartFlag = false;
bool isApStarted = false;
bool isWifiConnected = false;
bool buzzerBeepFlag = true;
bool maxTempAlertFlag = false;

unsigned int previousTime = 0;

byte firstThresholdTempForAlert = 90;
byte secondThresholdTempForAlert = 110;
byte ssidLength = 0;
byte passwordLength = 0;
byte wiFiRetryCounter = 0;
byte mqttCounter = 0;
byte responseLength = 2;
byte interval = 5;
byte activeSensor = 10;

String publishData = "";
String sendingTempData = "";
String alertMsg = "";
