/***************************************************************************
 * This is an Arduino UNO control for the purpose of humidity sensing using
 * the BME280 to turn on/off a relay based on set humidity HIGH and LOW
 * thesholds 
 * 
 * Developed to control the humidity of a home meat curing chamber 
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

// Relay port 
#define RELAY 13

// Rel. Humidity threshold to turn humidifier/fan OFF
#define HUMIDITY_HIGH 80 

// Rel. Humidity threshold to turn humidifier/fan ON 
#define HUMIDITY_LOW 75  // Humidity turn ON

// Amount of time to wait between sensor measurments in milliseconds
#define DELAY_DURATION 1000 // 1hz

// Global bool to keep track if the relay is known to be in the ON state 
bool relayIsOn; 

Adafruit_BME280 bme; // I2C

void setup() {
    Serial.begin(9600);
    
    // RELAY Setup
    pinMode(RELAY, OUTPUT); 
    relayIsOn = false; 

    // BME280 Setup
    if (!bme.begin()) {
        Serial.println("Failed to initilalize BME280");
        while (1);  
    }

    // Set BME280 parameters for humidity sampling 
    bme.setSampling(Adafruit_BME280::MODE_FORCED,
                    Adafruit_BME280::SAMPLING_X1,   // temperature
                    Adafruit_BME280::SAMPLING_NONE, // pressure
                    Adafruit_BME280::SAMPLING_X1,   // humidity
                    Adafruit_BME280::FILTER_OFF );

    Serial.println("Setup complete..."); 
}

void loop() { 
    delay(DELAY_DURATION);
    
    bme.takeForcedMeasurement();
    float humidity = bme.readHumidity();

    if (humidity <= HUMIDITY_LOW) {
      if (!relayIsOn) {
        digitalWrite(RELAY, HIGH);
        relayIsOn = true; 
      }
    } else if (humidity >= HUMIDITY_HIGH){
      if (relayIsOn) {
        digitalWrite(RELAY, LOW);
        relayIsOn = false;
      }
    }
    Serial.print(humidity);
    Serial.print("\t");
    Serial.print(bme.readTemperature());
    Serial.println(); 
}

