#ifndef BUTTON_H
#define BUTTON_H

#include <cstdint>

class Button {
private:
    uint8_t group_;
    uint8_t bit_;
    bool prev_;
    uint32_t count_;

public:
    Button(uint8_t group, uint8_t bit);
    void update();
    uint32_t count() const;
};

#endif //BUTTON_H
