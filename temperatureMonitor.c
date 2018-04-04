//Leonardo Saragiotto 04/04/2018
//timer1 will interrupt at 1Hz

//storage variables
boolean toggle = 0;
boolean toggleTemp = 0;
int ledPin = 8;

//this count will decrease every second and fire temperature function at 5 minute interval
int tempCount = 300;

void setup(){
  
  //set pins as outputs
  pinMode(ledPin, OUTPUT);

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

ISR(TIMER1_COMPA_vect) {//timer1 interrupt 1Hz
  tempCount --;
  if (tempCount == 0) {
      tempCount = 300;
      toggleTemp = 1;
  }
  toggleLed();
}

void toggleLed() {
  if (toggle){
    digitalWrite(13,HIGH);
  } else {
    digitalWrite(13,LOW);
  }
  toggle = !toggle
}

void loop(){
  //do other things here
  if (toggleTemp) {
    temperature();
  }
}

void temperature() {
    
}