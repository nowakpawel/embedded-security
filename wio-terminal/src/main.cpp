#include "TFT_eSPI.h"
#include <Wire.h>
#include <math.h> //atan2

TFT_eSPI tft;
const uint8_t LIS3DH_ADDR       = 0x18;
const uint8_t REG_CTRL_REG1     = 0x20;
const int8_t REG_OUT_X_L        = 0x28;



void setup() {
    Serial.begin(115200);
    delay(1000);

    tft.begin();
    tft.setRotation(3);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.fillScreen(TFT_BLACK);

    Wire1.begin();

    Wire1.beginTransmission(LIS3DH_ADDR);
    Wire1.write(REG_CTRL_REG1);
    Wire1.write(0x47);
    Wire1.endTransmission(true);

    Serial.println("LIS3DH init done");
}

void loop() {
    Wire1.beginTransmission(LIS3DH_ADDR);
    Wire1.write(0x80 | REG_OUT_X_L);
    Wire1.endTransmission(true);
    Wire1.requestFrom(LIS3DH_ADDR, 6); //all 6 bytes (3 axis x 2 bytes)

    uint8_t xl = Wire1.read();
    uint8_t xh = Wire1.read();
    uint8_t yl = Wire1.read();
    uint8_t yh = Wire1.read();
    uint8_t zl = Wire1.read();
    uint8_t zh = Wire1.read();

    int16_t x = (xh << 8) | xl;
    int16_t y = (yh << 8) | yl;
    int16_t z = (zh << 8) | zl;

    float pitch = atan2((float)x, sqrt((float)y * y + (float)z*z)) * 180.0 / PI;
    float roll = atan2((float)y, sqrt((float)x * x + (float)z*z)) * 180.0 / PI;

    const char* position;
    int16_t ax = abs(x), ay = abs(y), az = abs(z);

    if (az > ax && az > ay) {
        position = (z > 0) ? "UPSIDE DOWN": "FLAT";
    } else if (ax > ay) {
        position = (x > 0) ? "VERTICAL (front)" : "VERTICAL (back)";
    } else {
        position = (y > 0) ? "ON SIDE (L)": "ON SIDE (R)";
    }

    //LCD
    tft.setTextSize(3);
    tft.setCursor(10, 15); tft.print("pitch: "); tft.print(pitch, 0); tft.print((char)247); tft.print("      ");
    tft.setCursor(10, 55); tft.print("roll: "); tft.print(roll, 0); tft.print((char)247); tft.print("      ");

    tft.setTextSize(2);
    tft.setCursor(10, 110); tft.print("Position:        ");
    tft.setCursor(10, 135); tft.print(position); tft.print("        ");

    //Serial log
    Serial.print("X: ");            Serial.print(x);
    Serial.print("\nY: ");            Serial.print(y);
    Serial.print("\nZ: ");            Serial.print(z);
    Serial.print("\npitch: ");        Serial.print(pitch, 1);
    Serial.print("\nroll: ");         Serial.print(roll, 1);
    Serial.print("\nposition: ");     Serial.println(position);

    Serial.print("\n\n");

    delay(1000);
}