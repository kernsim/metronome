/*
  Metronome
 
 one button on pin 2 to GND
 two LEDs on pin 3 and pin 4

 Tap in the rate.
 The LEDs will blink.

 */

// set pin numbers:
#define buttonPin  2
#define ledPin1    3
#define ledPin2    4
// state 
#define MAINSTATE_LEARNING   1
#define MAINSTATE_PLAYING    2

#define TIMEOUT   3000

int mainState = 0;

unsigned long duration;
unsigned long startTime;
unsigned long now;

int n_pulses;
int cur_pulse;
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int lastButtonState = 0;         // variable for reading the pushbutton status
int buttonPressed;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin1, OUTPUT);      
  pinMode(ledPin2, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP); 
  //Serial.begin(38400);  

  mainState = MAINSTATE_PLAYING;
  duration = 750;
  n_pulses = 4;
  cur_pulse = 3;
  startTime = millis();
}


void play(){
     if ((now-startTime) > duration/5) {
       digitalWrite(ledPin1, LOW); 
       digitalWrite(ledPin2, LOW); 
     } 
     if ((now-startTime) > duration) {
       startTime = now;
       cur_pulse += 1;
       if (cur_pulse >= n_pulses) {
         cur_pulse = 0;
       }
       if (cur_pulse == 0) {
         digitalWrite(ledPin1, HIGH); 
       }
       digitalWrite(ledPin2, HIGH); 
     }
  }


void start_learning() {
  mainState = MAINSTATE_LEARNING;
  duration = 0;
  n_pulses = 1;
  startTime = now;  
  digitalWrite(ledPin1, HIGH); 
  digitalWrite(ledPin2, HIGH); 
}

void learn_now() {
  if (n_pulses == 1) {
    duration = now - startTime;
  }
  else {
    duration = ((now - startTime) + duration * (n_pulses - 1)) / n_pulses;
  }
  n_pulses += 1;
  //Serial.print("LEARN  ");
  //Serial.print(duration);
  //Serial.print(" ");
  //Serial.println(n_pulses);
  startTime = now;
}

void end_learning() {
  if (n_pulses == 0) {
    duration = 500;
    n_pulses = 3;
  }
  cur_pulse =  n_pulses - 2;
  mainState = MAINSTATE_PLAYING;
  digitalWrite(ledPin1, LOW); 
  digitalWrite(ledPin2, LOW); 
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && lastButtonState == HIGH) {
    buttonPressed = 1;
    //Serial.print("buttonPressed\n");
  }
  else {
    buttonPressed = 0;
  }
  lastButtonState = buttonState;

  //Serial.print('M');
  //Serial.print(mainState);
  //Serial.print(' L');
  //Serial.print(learnState);
  //Serial.print(' P');
  //Serial.println(buttonPressed);
  now = millis();

  if (mainState == MAINSTATE_PLAYING) {
    if (buttonPressed == 0) {
      play();
    }
    else
    {
      start_learning();
    }
    return;
  }
  
  if (mainState == MAINSTATE_LEARNING) {
    if (now-startTime < 30) {
      // debounce
      return;
    }
    if (buttonPressed == 1) {
       learn_now();
       return;
    }
    if (now-startTime > TIMEOUT) {
      end_learning();
      //Serial.print("timeout\n");
    }
  }
}


