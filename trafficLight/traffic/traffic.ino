// Traffic light contro
//
//
//  Attribute that control all traffic light state
//
// +--------+-----------------------------------------+
// + State  + Status                                  + 
// +--------+-----------------------------------------+
// +  00    + Initialize Status                       + 
// +  01    + Main Road Traffic Open                  + 
// +  02    + Second Road Traffic Open                + 
// +  03    + Main Road Traffic Attention             + 
// +  04    + Second Road Traffic Attention           + 
// +  05    + Traffic Closed - Main Road Will Open    + 
// +  06    + Traffic Closed - Second Road Will Open  + 
// +  07    + Traffic Attention Both Roads            + 
// +        +                                         +
// +--------+-----------------------------------------+
//
//  Attribute that control all pedestrian light state
//
// +--------+-----------------------------------------+
// + State  + Status                                  + 
// +--------+-----------------------------------------+
// +  00    + Initialize Status                       + 
// +  01    + Main Road Pedestrian Open               + 
// +  02    + Main Road Pedestrian Blinking           + 
// +  03    + Second Road Pedestrian Open             + 
// +  04    + Second Road Pedestrian Blinking         + 
// +--------+-----------------------------------------+ 
//
//
//
//storage variables
boolean blinkOutput = 0;

int trafficLightStatus = 0;
int pedestrianLightStatus = 0;
int count = 0;
int timeCompare = 5;
int initializationTime = 5;

int mainRoadTimeOpen = 12;
int mainRoadTimeTransition = 2;
int mainRoadTimeClose = 8;
int bothRoadTimeClose = 1;

int redPedestrianMainRoad = 4;
int greenPedestrianMainRoad = 5;
int redPedestrianSecondRoad = 6;
int greenPedestrianSecondRoad = 7;

int redOutputSecondRoad = 8;
int yellowOutputSecondRoad = 9;
int greenOutputSecondRoad = 10;
int redOutputMainRoad = 11;
int yellowOutputMainRoad = 12;
int greenOutputMainRoad = 13;

void setup() {
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  timeCompare = initializationTime;
  trafficLightStatus = 0;
  
  //set pins as outputs
  pinMode(redOutputMainRoad, OUTPUT);
  pinMode(yellowOutputMainRoad, OUTPUT);
  pinMode(greenOutputMainRoad, OUTPUT);
  pinMode(redOutputSecondRoad, OUTPUT);
  pinMode(yellowOutputSecondRoad, OUTPUT);
  pinMode(greenOutputSecondRoad, OUTPUT);

  pinMode(redPedestrianMainRoad, OUTPUT);
  pinMode(greenPedestrianMainRoad, OUTPUT);
  pinMode(redPedestrianSecondRoad, OUTPUT);
  pinMode(greenPedestrianSecondRoad, OUTPUT);

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

  if (trafficLightStatus == 0) {
    if (count >= timeCompare) {
        count = 0;
        trafficLightStatus = 5;
        timeCompare = bothRoadTimeClose;
    } else {
      blinkOutput = !blinkOutput;
      return;
  	}
  }

  if (count >= timeCompare) {
    count = 0;
    if (trafficLightStatus == 5) {
      trafficLightStatus = 1;
      pedestrianLightStatus = 3;
      timeCompare = mainRoadTimeOpen;
      return;
    }

    if (trafficLightStatus == 6) {
      trafficLightStatus = 2;
      pedestrianLightStatus = 1;
      timeCompare = mainRoadTimeClose;
      return;
    }

    if (trafficLightStatus == 1) {
      trafficLightStatus = 3;
      timeCompare = mainRoadTimeTransition;
      return;
    }

    if (trafficLightStatus == 2) {
      trafficLightStatus = 4;
      timeCompare = mainRoadTimeTransition;
      return;
    } 
   
    if (trafficLightStatus == 3) {
      trafficLightStatus = 6;
      timeCompare = bothRoadTimeClose;
      return;
    }

    if (trafficLightStatus == 4) {
      trafficLightStatus = 5;
      timeCompare = bothRoadTimeClose;
      return;
    }
  }
}
  
void loop() { 

  if (trafficLightStatus == 0) {
    digitalWrite(yellowOutputMainRoad, blinkOutput ? HIGH : LOW);
    digitalWrite(yellowOutputSecondRoad, blinkOutput ? HIGH : LOW);
    return;
  }

  if (trafficLightStatus == 1) {
  	digitalWrite(redOutputMainRoad, LOW);
    digitalWrite(yellowOutputMainRoad, LOW);
    digitalWrite(greenOutputMainRoad, HIGH);
    
    digitalWrite(redOutputSecondRoad, HIGH);
    digitalWrite(yellowOutputSecondRoad, LOW);
    digitalWrite(greenOutputSecondRoad, LOW);
  }

  if (trafficLightStatus == 2) {
    digitalWrite(redOutputMainRoad, HIGH);
    digitalWrite(yellowOutputMainRoad, LOW);
    digitalWrite(greenOutputMainRoad, LOW);
    
    digitalWrite(redOutputSecondRoad, LOW);
    digitalWrite(yellowOutputSecondRoad, LOW);
    digitalWrite(greenOutputSecondRoad, HIGH);
  }

  if (trafficLightStatus == 3) {
    digitalWrite(redOutputMainRoad, LOW);
    digitalWrite(yellowOutputMainRoad, HIGH);
    digitalWrite(greenOutputMainRoad, LOW);
    
    digitalWrite(redOutputSecondRoad, HIGH);
    digitalWrite(yellowOutputSecondRoad, LOW);
    digitalWrite(greenOutputSecondRoad, LOW);
  }

  if (trafficLightStatus == 4) {
    digitalWrite(redOutputMainRoad, HIGH);
    digitalWrite(yellowOutputMainRoad, LOW);
    digitalWrite(greenOutputMainRoad, LOW);
    
    digitalWrite(redOutputSecondRoad, LOW);
    digitalWrite(yellowOutputSecondRoad, HIGH);
    digitalWrite(greenOutputSecondRoad, LOW);
  }

  if (trafficLightStatus == 5 || trafficLightStatus == 6) {
    digitalWrite(redOutputMainRoad, HIGH);
    digitalWrite(yellowOutputMainRoad, LOW);
    digitalWrite(greenOutputMainRoad, LOW);
    
    digitalWrite(redOutputSecondRoad, HIGH);
    digitalWrite(yellowOutputSecondRoad, LOW);
    digitalWrite(greenOutputSecondRoad, LOW);
  }

  if (pedestrianLightStatus == 1) {
    digitalWrite(redPedestrianMainRoad, LOW);
    digitalWrite(greenPedestrianMainRoad, HIGH);
    digitalWrite(redPedestrianSecondRoad, HIGH);
    digitalWrite(greenPedestrianSecondRoad, LOW);
  }

  if (pedestrianLightStatus == 3) {
    digitalWrite(redPedestrianMainRoad, HIGH);
    digitalWrite(greenPedestrianMainRoad, LOW);
    digitalWrite(redPedestrianSecondRoad, LOW);
    digitalWrite(greenPedestrianSecondRoad, HIGH);
  }
}
