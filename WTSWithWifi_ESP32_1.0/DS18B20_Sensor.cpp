#include "DS18B20_Sensor.h"  // Include the header file for the DS18B20_Sensor class

// Constructor to initialize OneWire and DallasTemperature
DS18B20_Sensor::DS18B20_Sensor(int pin)
  : oneWire(pin), sensors(&oneWire) {
  // The constructor initializes the OneWire object with the provided pin.
  // It also initializes the DallasTemperature object using the OneWire object.
}

// Function to begin communication with the sensor
void DS18B20_Sensor::begin() {
  sensors.begin();  // Initialize the DallasTemperature object, setting up communication with the DS18B20 sensor
}

// Function to read the temperature from the DS18B20 sensor
float DS18B20_Sensor::readTemperature() {
  sensors.requestTemperatures();      // Request a temperature reading from the sensor
  return sensors.getTempCByIndex(0);  // Return the temperature in Celsius from the first (and only) sensor
}
