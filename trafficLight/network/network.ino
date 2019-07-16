#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>

Ticker blinker;

#define LED 2  //On board LED

void ICACHE_RAM_ATTR onTimerISR(){
    digitalWrite(LED,!(digitalRead(LED)));  //Toggle LED Pin
    timer1_write(600000);//12us
}

void setup() {
    Serial.begin(115200);
    Serial.println("");

    pinMode(LED,OUTPUT);

    //Initialize Ticker every 0.5s
    timer1_attachInterrupt(onTimerISR);
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
    timer1_write(600000); //120000 us
}

  
void loop() {

}

String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
