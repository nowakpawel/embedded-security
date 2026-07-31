#include "TFT_eSPI.h"
#include <Wire.h>

TFT_eSPI tft;
const uint8_t LIS3DH_ADDR = 0x18;
const uint8_t WHOAIM_LIS3DHTR_REGISTER = 0x0F;




void setup() {
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    // tft.setCursor(50, 0);

    Wire1.begin();

    Wire1.beginTransmission(LIS3DH_ADDR);
    Wire1.write(WHOAIM_LIS3DHTR_REGISTER);
    uint8_t tx = Wire1.endTransmission(true);
    uint8_t got = Wire1.requestFrom(LIS3DH_ADDR, 1);

    uint8_t whoami = Wire1.read();

    tft.print("tx: "); tft.println(tx);
    tft.print("got "); tft.println(got);
    tft.print("0x"); tft.println(whoami, HEX);

}


void loop() {

}