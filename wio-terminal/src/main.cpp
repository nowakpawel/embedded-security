#include "TFT_eSPI.h"

static const uint32_t HALF_PERIOD_BASE = 500000;

static bool prev_a = false;
static uint32_t counter = 0;

TFT_eSPI tft;

void setup_button_a() {
	PORT->Group[2].DIRCLR.reg = (1 << 26);
	PORT->Group[2].PINCFG[26].bit.INEN = 1;

}

void setup_button_b() {
	PORT->Group[2].DIRCLR.reg = (1 << 27);
	PORT->Group[2].PINCFG[27].bit.INEN = 1;
	// external pull-up on board (R3, 4.7K) — PULLEN not needed
}

//Stup Buzzer
void buzzer_setup() {
	PORT->Group[3].OUTCLR.reg = (1 << 11);
	PORT->Group[3].DIRSET.reg = (1 << 11);
}

//BUZZER trigger
void setup_button_c() {
	PORT->Group[2].DIRCLR.reg = (1 << 28);
	PORT->Group[2].PINCFG[28].bit.INEN = 1;
}

void setup_switch() {
	PORT->Group[3].DIRCLR.reg = (1 << 8);
	PORT->Group[3].PINCFG[8].bit.INEN = 1;
	// PORT->Group[3].PINCFG[8].bit.PULLEN = 1;
	// PORT->Group[3].OUTSET.reg = (1 << 8); => last two lines for transparency only
}

void yell_buzzer(uint32_t freq_hz) {
	PORT->Group[3].OUTSET.reg = (1 << 11);
	delayMicroseconds(HALF_PERIOD_BASE / freq_hz);
	PORT->Group[3].OUTCLR.reg = (1 << 11);
	delayMicroseconds(HALF_PERIOD_BASE / freq_hz);
}


bool button_a_pressed() {
	return !(PORT->Group[2].IN.reg & (1 << 26));
}

bool button_b_pressed() {
	return !(PORT->Group[2].IN.reg & (1 << 27));
}

bool button_c_pressed() {
	return !(PORT->Group[2].IN.reg & (1 << 28));
}

bool button_switch_pressed() {
	return !(PORT->Group[3].IN.reg & (1 << 8));
}

void setup() {
	Serial.begin(115200);
	setup_button_a();
	setup_button_b();
	setup_button_c();
	setup_switch();

	//buzzer
	buzzer_setup();

	tft.begin();
	tft.setRotation(1);
	tft.fillScreen(TFT_BLACK);

}

void loop() {
	if (button_b_pressed()) {
		Serial.print("Button B pressed\t");
	}

	if (button_switch_pressed()) {
		Serial.print("Switch Pressed\t");
	}

	while (button_c_pressed()) {
		yell_buzzer(1000);
	}

	bool now_buttonA = button_a_pressed();


	//button A counter
	if (now_buttonA && !prev_a) {
		counter++;
		Serial.printf("Button A pressed %lu times\n", counter);
	}

	prev_a = now_buttonA;
	// delay(1);
}
