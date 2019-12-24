const int ledPin =  LED_BUILTIN;
int ledState = LOW;


void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {  
  if (millis() % 5000 == 0 ) {

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    digitalWrite(ledPin, ledState);
    
    Serial.print("*2\r\n$3\r\nGET\r\n$13\r\narduino:pin13\r\n");
  }
}
