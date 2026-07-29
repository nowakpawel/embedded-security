#include "TFT_eSPI.h"
#include <Wire.h>

TFT_eSPI tft;

void setUpCursor(int i, int fontHeigh) {
	tft.setCursor(50, i*fontHeigh);
}

void setup() {
	tft.begin();
	tft.setRotation(3);
	tft.fillScreen(TFT_BLUE);

	tft.setTextColor(TFT_BLACK);
	tft.setTextSize(3);
	// tft.setCursor(10,10);

	Wire.begin();
	uint8_t addresses[2] = {0x18, 0x19};
	size_t length = sizeof(addresses) / sizeof(addresses[0]);

	for (int i = 0; i < length; i++) {
		setUpCursor(i, tft.fontHeight());
		Wire.beginTransmission(addresses[i]);
		uint8_t result = Wire.endTransmission();

		tft.printf("0x%02X: %d\n", addresses[i], result);
	}
}

void loop() {

}