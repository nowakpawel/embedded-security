# Embedded Security Playground

## WIO Terminal - register level GPIO input handling
Buttons, 5-way switch and passive buzzer on the WIO Terminal (ATSAMD51P19A, Cortex-M4F), handled directly through PORT registers instead of the Arduino `pinMode()`/ `digitalRead()` layer. 
Part of a longer embedded C learning path. The goal here was not "make it work", but to understand what actually happens between C code and the physical pin.

### What it does?
* Reads the three top buttons(A/B/C -> PC26/PC27//PC28), and the 5-way switch X direction (PD08) via direct register access: `DIRCLR`, `PINCFG.INEN`, `PINCFG.PULLEN`, `OUTSET`, `IN`.
* Drives a passive buzzer (PD11) by generating square wave in software. 
* Count button A presses using edge detection (press events).
* Logs button state and the press counter over UART (115200)
* Polling loop with a 1 ms delayy


### Hardware Mapping
Pin assignments were traced from original schematic and crooss checked against `variant.cpp` (`g_APinDescription`) in the Seeed Arduino Core.

| Input/Output | Net label | MCU pin | Port/bit | Notes |
| :------------: | :---------: | :-------: | :--------: | :-----: |
| Button A     | BUTTON1   | PC26    | Group[2], 26 | 4.7K ext. pull-up (R49) |
| Button B     | BUTTON2   | PC27    | Group[2], 27 | 4.7K ext. pull-up (R3) |
| Button C     | BUTTON3   | PC28    | Group[2], 28 | 4.7K ext. pull-up (R4) |
| 5-way X      | SWITCH_X  | PD08    | Group[3], 8  | 100K ext. pull-up (R37) |
| Buzzer       | BUZZER_CTRL   | PD11    | Group[3],  11 | via R9, alt. func. TCC0.4 |

All inputs are active-low. The switch shorts the pin to the GND, the pull-up holds it at 3.3V whenn idle.
The buzzer is an output. Driven high/low directly.

### What I learned
* **Memory-mapped I/O is the whole trick**: `PORT` is a pointer to a fixed address `0x41008000`. The CMSIS headers map C structs field-by-field onto the register layout from the datasheet. The struct offsets in `component/port.h` literally are the hardware addresses. Writing a bit flips a transistor.
* **SET/CLR helper registers exist for atomicity**: Writing `1` to a bit in `DIRCLR` means "clear this bit in DIR, leave the rest". One bus write no read-modify-write window. I learned that in hard way, writing directky to `DIR.reg` overwrites the direction of all 32 pins of the port. This reconfigured unrelated pins (LCD, SD lines) on port C.
* **A disabled input buffer reads as 0, no as "unknown"**: With `INEN=0` the `IN` register simply holds zero, regardless of the actuall pin volatge. Combine with _active-low_ logic, a missing configuration shows up as _constant false positive_ (button pressed forever).
* **The board matters as much as the chip**: The schematic revealed external pull-ups on all button lines, which makes the internal `PULLEN`/`OUT`pull-up configuration redundant on this board. The minimal  correct setup is just `DIRCLR` + `INEN`. Same code on a board without external resistors would fail.  (Documented in `wio-terminal/src/main.cpp` as a comments for teaching purpouse).
* **The chip does not know what a "button" is**: The mapping PC26-> button A exists only on this PCB. It lives in the schematic and in the board vendor's `variant.h/cpp` no in the silicon.
* **A passive buzzer has no oscillator of its onw**: Driving PD11 to a steady high produces one click, then silence. A piezo element only moves when the voltagage changes. To get a tone, the pin has to be toggled high/low in software. The toggle rate is the pitch. Half period in microseconds is:
```c
HALF_PERIOD_BASE = 500000;
```
One second is 1 000 000 microseconds. A square wave toggles twice per cycle. So the wait between toggles is `1 000 000/2 / freq_hz`.

* **State is not the same as an event**: `button_a_pressed()` returns the current state. Held for a second, it reads `true` hundrets of times. Counting prsesses means detecting the moment the state changes from released to pressed. (a rising edge). Comparing the current read against the previos iteration's gives four cases. Only one is press:

| prev | now | meaning |
| :----: | :----: | :----: |
| false | false | idle | 
| false | true | **press event** |
| true | true | still held |
| true | false | rerlease event |

This collapses to `if now && !prev) counter++;`.

### Known limitations
* Polling with `delay(1)`  coarse timinig. Short presses between samples can still be missed.
* Tone generation is blocking. `delayMicroseconds` inside the buzzer loop halts everything else while tone is playing. Fine for a short beep, but the "do two thing at once is the problem I'll be facing in the next phases (**FreeRTOS** and **TCC PWM**)
* No software debouncing yet. The counter currently reads clean presses. But that is a side effect of good tact switches plus 1 ms delay, not a designed filter. 
* No edge detection via interrupts yet. EIC - the `Ixx` alternate function visible on the schematic pin labels is the natural next step.
* Only the `X` direction of the 5-way switch is handled so far.

### Build & flash
```bash
pio run -t upload
pio device monitor -b 115200
```
PlatformIO project targeting the Seeed SAMD Framework. User must be in  `uucp` group.

### References
* [Wio Terminal Schematic (PDF)](https://files.seeedstudio.com/wiki/Wio-Terminal/res/Wio-Terminal-Schematics.pdf)
* ATSAMD51 datasheet
* Seeed Arduino core `variant/wio-terminal/variant.{h,cpp}`