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
boolean mainRoadGreenToggle = 0;
boolean secondRoadGreenToggle = 0;
boolean mainRoadYellowToggle = 0;
boolean secondRoadYellowToggle = 0;
boolean mainRoadRedToggle = 0;
boolean secondRoadRedToggle = 0;
boolean mainRoadWillOpen = 0;
int count = 0;
int timeCompare = 5;
int mainRoadTimeOpen = 12;
int mainRoadTimeTransition = 1;
int mainRoadTimeClose = 4;
int bothRoadTimeClose = 1;

boolean initializeStatus = 0;
int initializationTime = 5;

int redOutputMainRoad = 11;
int yellowOutputMainRoad = 12;
int greenOutputMainRoad = 13;

int redOutputSecondRoad = 8;
int yellowOutputSecondRoad = 9;
int greenOutputSecondRoad = 10;

void setup() {
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  timeCompare = initializationTime;
  initializeStatus = true;
  
  //set pins as outputs
  pinMode(redOutputMainRoad, OUTPUT);
  pinMode(yellowOutputMainRoad, OUTPUT);
  pinMode(greenOutputMainRoad, OUTPUT);
  pinMode(redOutputSecondRoad, OUTPUT);
  pinMode(yellowOutputSecondRoad, OUTPUT);
  pinMode(greenOutputSecondRoad, OUTPUT);

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

  if (initializeStatus) {
    mainRoadYellowToggle = !mainRoadYellowToggle;
    secondRoadYellowToggle = !secondRoadYellowToggle;
    Serial.print("Yellow toggled");
    if (count >= timeCompare) {
        count = 0;
        initializeStatus = false;
      	mainRoadYellowToggle = false;
      	secondRoadYellowToggle = false;
        mainRoadRedToggle = true;
        secondRoadRedToggle = true;
        timeCompare = bothRoadTimeClose;
    } else {
      return;
  	}
  }

  if (count >= timeCompare) {
    count = 0;
    if (mainRoadRedToggle && secondRoadRedToggle) {
      if (mainRoadWillOpen) {
        mainRoadWillOpen = false;
        mainRoadGreenToggle = true;
        mainRoadRedToggle = false;
        timeCompare = mainRoadTimeOpen;
        return;
      } else {
        mainRoadWillOpen = true;
        secondRoadGreenToggle = true;
        secondRoadRedToggle = false;
        timeCompare = mainRoadTimeClose;
        return;
      }
    }

    if (mainRoadGreenToggle) {
      mainRoadYellowToggle = true;
      mainRoadGreenToggle = false;
      timeCompare = mainRoadTimeTransition;
      return;
    }

    if (mainRoadYellowToggle) {
      mainRoadRedToggle = true;
      mainRoadYellowToggle = false;
      timeCompare = bothRoadTimeClose;
    } 
   
    if (secondRoadGreenToggle) {
      secondRoadGreenToggle = false;
      secondRoadYellowToggle = true;
      timeCompare = mainRoadTimeTransition;
      return;
    }

    if (secondRoadYellowToggle) {
      secondRoadRedToggle = true;
      secondRoadYellowToggle = false;
      timeCompare = bothRoadTimeClose;
      return;
    }
  }
}
  
void loop() { 

  if (initializeStatus) {
    if (mainRoadYellowToggle) {
      digitalWrite(yellowOutputMainRoad, HIGH);
      digitalWrite(yellowOutputSecondRoad, HIGH);
    } else {
      digitalWrite(yellowOutputMainRoad, LOW);
      digitalWrite(yellowOutputSecondRoad, LOW);
    }
    return;
  }

  if (mainRoadRedToggle) {
	digitalWrite(redOutputMainRoad, HIGH);
    digitalWrite(yellowOutputMainRoad, LOW);
    digitalWrite(greenOutputMainRoad, LOW);
  }

  if (secondRoadRedToggle) {
    digitalWrite(redOutputSecondRoad, HIGH);
    digitalWrite(yellowOutputSecondRoad, LOW);
    digitalWrite(greenOutputSecondRoad, LOW);
  }
  
  if (mainRoadYellowToggle) {
	digitalWrite(redOutputMainRoad, LOW);
    digitalWrite(yellowOutputMainRoad, HIGH);
    digitalWrite(greenOutputMainRoad, LOW);
  }

  if (secondRoadYellowToggle) {
    digitalWrite(redOutputSecondRoad, LOW);
    digitalWrite(yellowOutputSecondRoad, HIGH);
    digitalWrite(greenOutputSecondRoad, LOW);
  }

  if (mainRoadGreenToggle) {
	digitalWrite(redOutputMainRoad, LOW);
    digitalWrite(yellowOutputMainRoad, LOW);
    digitalWrite(greenOutputMainRoad, HIGH);
  }

  if (secondRoadGreenToggle) {
    digitalWrite(redOutputSecondRoad, LOW);
    digitalWrite(yellowOutputSecondRoad, LOW);
    digitalWrite(greenOutputSecondRoad, HIGH);
  }
}
