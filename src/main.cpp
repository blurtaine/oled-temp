#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DallasTemperature.h>
#include <Fonts/TomThumb.h>
#include <OneWire.h>

// ===================================================================
// Blinks an LED in an endless loop
// ===================================================================
static void ledTrap(const int pin) {
    pinMode(pin, OUTPUT);
    while (true) {
        digitalWrite(pin, HIGH);
        delay(300);
        digitalWrite(pin, LOW);
        delay(300);
    }
}

// ===================================================================
// Takes a DallasTemperature instance
// Returns a temperature in °C
// ===================================================================
static float getTemperature(DallasTemperature& sensor) {
    sensor.requestTemperatures();
    const float tempC = sensor.getTempCByIndex(0);
    if (tempC == DEVICE_DISCONNECTED_C) {
        Serial.println("Error: Could not read temperature data");
        return 0;
    }
    return tempC;
}

// ===================================================================
// Returns an instance of a display with the important
// properties set
// ===================================================================
static Adafruit_SSD1306 createDisplay() {
    static constexpr int SCREEN_WIDTH = 128;
    static constexpr int SCREEN_HEIGHT = 64;

    // Reset pin or -1 if sharing Arduino reset pin
    static constexpr int RESET_PIN = -1;
    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, RESET_PIN);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        // Failed to initialize, signalize that with LED
        ledTrap(5);
    }

    // display.setFont(&TomThumb);
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.cp437(true);         // Use full 256 char 'Code Page 437' font

    return display;
}

// ===================================================================
// Returns an instance of a Dallas Temperature sensor
// ===================================================================
static DallasTemperature createTemperatureSensor() {
    static constexpr int TEMP_SENSOR_PIN = 2;
    OneWire oneWire(TEMP_SENSOR_PIN);
    DallasTemperature sensor(&oneWire);
    sensor.begin();

    return sensor;
}

#define DEGREE_SYMBOL "\xF8"

int main() {
    init();

    Adafruit_SSD1306 display = createDisplay();
    DallasTemperature sensor = createTemperatureSensor();

    while (true) {
        static constexpr int MEASURING_TIME = 1000;
        const unsigned long start = millis();
        float averageTemperature = getTemperature(sensor);
        while (millis() - start < MEASURING_TIME) {
            averageTemperature = (averageTemperature + getTemperature(sensor)) / 2;
        }
        display.clearDisplay();
        display.setCursor(0, 0);     // Start at top-left corner
        display.print("Temperature: ");
        display.print(averageTemperature);
        display.print(" " DEGREE_SYMBOL "C");
        
    }
    return 0;
}

