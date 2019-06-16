//timer interrupts
//by Amanda Ghassaei
//June 2012
//https://www.instructables.com/id/Arduino-Timer-Interrupts/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
*/

//timer setup for timer0, timer1, and timer2.
//For arduino uno or any board with ATMEL 328/168.. diecimila, duemilanove, lilypad, nano, mini...

//this code will enable all three arduino timer interrupts.
//timer0 will interrupt at 2kHz
//timer1 will interrupt at 1Hz
//timer2 will interrupt at 8kHz

//storage variables
boolean redToggle = 0;
boolean yellowToggle = 0;
boolean greenToggle = 0;
int count = 0;
int timeCompare = 5;
int mainRoadTimeOpen = 3;
int mainRoadTimeTransition = 1;
int mainRoadTimeClose = 2;

void setup() {
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  timeCompare = mainRoadTimeClose;
  redToggle = true;
  
  //set pins as outputs
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);

  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  //allow interrupts
  sei();
}

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  
  count += 1;
  if (count >= timeCompare) {
    count = 0;
    if (redToggle) {
      redToggle = false;
      greenToggle = true;
      timeCompare = mainRoadTimeOpen;
      return;
    }
    
    if (greenToggle) {
      greenToggle = false;
      yellowToggle = true;
      timeCompare = mainRoadTimeTransition;
      return;
    }
    
    if (yellowToggle) {
      yellowToggle = false;
      redToggle = true;
      timeCompare = mainRoadTimeClose;
      return;
    }
  }
}
  
void loop(){
  //do other things here
  if (redToggle) {
	digitalWrite(13, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
  }
  
  if (greenToggle) {
	digitalWrite(13, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, HIGH);
  }
  
  if (yellowToggle) {
	digitalWrite(13, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);
  }
}
