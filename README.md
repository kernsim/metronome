# Arduino Metronome

## Hardware

* Arduino Nano
* One button on pin 2 to GND
* Two LEDs on pin 3 and pin 4
* Buzzer on pin 9 (pwm)

Tap in the rate and number of pulses, wait for three seconds and the LEDs will blink.

The button is debounced using Bounce2.h with LOCK-OUT algorithm and 15ms interval.
