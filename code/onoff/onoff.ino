const int ledPin =  LED_BUILTIN;
int ledState = LOW;

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
    Serial.print("*2\r\n$3\r\nGET\r\n$13\r\narduino:pin:13\r\n");
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
