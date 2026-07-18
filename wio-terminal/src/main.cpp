#include "TFT_eSPI.h"

TFT_eSPI tft;

void setup() {
	Serial.begin(115200);

	tft.begin();
	tft.setRotation(3);
	tft.fillScreen(TFT_WHITE);
	tft.setTextColor(TFT_BLACK);
	tft.setTextSize(3);
}

void loop() {
	static int x = 0;
	const char* msg = "UART log";

	int w = tft.textWidth(msg);
	int h = tft.fontHeight();

	tft.fillRect(x, 100, w, h, TFT_WHITE);
	x -= 2;

	if (x + w < 0) {
		x = tft.width();
	}

	tft.drawString(msg, x, 100);
	Serial.print("x = ");
	Serial.print(x);
	delay(20);
}
