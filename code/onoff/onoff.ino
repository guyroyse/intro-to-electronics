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

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    digitalWrite(ledPin, ledState);
    Serial.print("*2\r\n$3\r\nGET\r\n$13\r\narduino:pin13\r\n");
  }
}
