#ifndef DS18B20_SENSOR_H  // Check if DS18B20_SENSOR_H is not defined
#define DS18B20_SENSOR_H  // Define DS18B20_SENSOR_H to prevent multiple inclusions of this header file

#include <OneWire.h>            // Include the OneWire library to communicate with the DS18B20 sensor
#include <DallasTemperature.h>  // Include the DallasTemperature library to easily manage the DS18B20 sensor

// Define the DS18B20_Sensor class
class DS18B20_Sensor {
private:
  OneWire oneWire;            // OneWire object to handle communication with the sensor
  DallasTemperature sensors;  // DallasTemperature object to interact with the DS18B20 sensor

public:
  DS18B20_Sensor(int pin);  // Constructor to initialize the sensor with the GPIO pin number
  void begin();             // Method to initialize the sensor and prepare for reading temperature
  float readTemperature();  // Method to read the temperature from the sensor and return it in Celsius
};

#endif  // End of the #ifndef preprocessor directive
