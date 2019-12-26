const int ledPin =  LED_BUILTIN;
int ledState = LOW;
bool connected = false;

const long interval = 5000;
unsigned long previousMillis = 0;


void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;

    if (Serial.available() >= 3) {

      char buffer[4];
      Serial.readBytes(buffer, 3);
      buffer[3] = 0;

      if (strcmp("+++", buffer) == 0) {
        connected = true;
      }
    }
    
    if (connected == true) {      
      Serial.print("*2\r\n$3\r\nGET\r\n$13\r\narduino:pin13\r\n");
    }
    
  }
}

void blink_code(int count, int pause) {  
  for (int i = 0; i < count; i++) {
    digitalWrite(ledPin, HIGH);
    delay(pause);
    digitalWrite(ledPin, LOW);
    delay(pause);
  }
}
