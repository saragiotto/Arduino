
void setup() {
  
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  Serial.begin(9600);
  while (!Serial) ;
  
  // Initialize the client library
  HttpClient client;

  // Make a HTTP request:
  client.get("https://shrouded-headland-71343.herokuapp.com/settings");

  // if there are incominxg bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  Serial.flush();
  Serial.print("Setup finished");  
}

  
void loop() {

}

