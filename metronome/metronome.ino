/*
  Metronome

 one button on pin 2 to GND
 two LEDs on pin 3 and pin 4

 Tap in the rate.
 The LEDs will blink.

 Uses Bounce2.h with LOCK-OUT algorithm.

  The MIT License (MIT)

  Copyright (c) 2018 Simon Kern

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
#include "Bounce2.h"

// set pin numbers:
#define buttonPin  2
#define ledPin1    3
#define ledPin2    4
// state
#define LEARNING   1
#define PLAYING    2

#define TIMEOUT   3000

int state = 0;

unsigned long interval;
unsigned long start_time;
unsigned long now;

int number_pulses;
int current_pulse;

Bounce button;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);
  button = Bounce();
  button.attach(buttonPin);
  button.interval(15);

  // initialize for PLAYING
  state = PLAYING;
  interval = 1000;
  number_pulses = 4;
  start_playing();
}

void play(){
     if ((now - start_time) > interval / 2) {
       // after 20% of interval: switch LEDs off
       digitalWrite(ledPin1, LOW);
       digitalWrite(ledPin2, LOW);
     }

     if ((now - start_time) > interval) {
       // after inverval: start new interval, switch on LED(s)
       start_time = now;
       current_pulse += 1;
       if (current_pulse >= number_pulses) {
         current_pulse = 0;
       }
       if (current_pulse == 0) {
         digitalWrite(ledPin1, HIGH);
       }
       digitalWrite(ledPin2, HIGH);
     }
  }


void start_learning() {
  state = LEARNING;
  interval = 0;
  number_pulses = 1;
  start_time = now;
  // LEDs are on during learning:
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);

  Serial.println("LEARNING...");
}


void start_playing() {
  state = PLAYING;
  current_pulse =  number_pulses - 2;
  start_time = millis();

  Serial.print("PLAY: pulses = ");
  Serial.print(number_pulses);
  Serial.print(", interval = ");
  Serial.print(interval);
  Serial.println(" ms");
}


void learn_now() {
  if (number_pulses == 1) {
    interval = now - start_time;
  }
  else {
    interval = ((now - start_time) + interval * (number_pulses - 1)) / number_pulses;
  }
  number_pulses += 1;
  start_time = now;

  Serial.print("LEARN:  ");
  Serial.print(number_pulses);
  Serial.print(" ");
  Serial.println(interval);
}


void end_learning() {
  if (number_pulses == 1) {
    interval = 1000;
    number_pulses = 4;
  }

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);

  start_playing();
}


void loop(){
  // read the state of the pushbutton value:
  button.update();
  int buttonPressed = button.fell();

  now = millis();

  switch (state) {
    case PLAYING:    // PLAYING
      if (!buttonPressed) {
        play();
      }
      else
      {
        start_learning();
      }
      break;
    case LEARNING:    // LEARNING
      if (buttonPressed == 1) {
        learn_now();
      }
      else if (now - start_time > TIMEOUT) {
        Serial.print("TIMEOUT\n");
        end_learning();
      }
      break;
  }  
}


