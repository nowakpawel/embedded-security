//
// Created by pawel on 8/7/26.
//
#include "Button.h"
#include <Arduino.h>

Button::Button(uint8_t group, uint8_t bit)
    : group_(group), bit_(bit), prev_(false), count_(0) {
    PORT->Group[group_].DIRCLR.reg = (1 << bit_);
    PORT->Group[group_].PINCFG[bit_].bit.INEN = 1;
}

void Button::update() {
    bool now = !(PORT->Group[group_].IN.reg & (1 << bit_));
    if (now && !prev_) {
        count_++;
    }

    prev_ = now;
}

uint32_t Button::count() const {
    return count_;
}