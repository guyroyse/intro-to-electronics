const int ledPin =  LED_BUILTIN;
int ledState = LOW;

const long interval = 2500;
unsigned long previousMillis = 0;


void setup() {
  for (int i = 2; i <= 13; i++)
    pinMode(i, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    Serial.print("*13\r\n");
    Serial.print("$4\r\n");
    Serial.print("MGET\r\n");
    Serial.print("$13\r\n");
    Serial.print("arduino:pin:2\r\n");
    Serial.print("$13\r\n");
    Serial.print("arduino:pin:3\r\n");
    Serial.print("$13\r\n");
    Serial.print("arduino:pin:4\r\n");
    Serial.print("$13\r\n");
    Serial.print("arduino:pin:5\r\n");
    Serial.print("$13\r\n");
    Serial.print("arduino:pin:6\r\n");
    Serial.print("$13\r\n");
    Serial.print("arduino:pin:7\r\n");
    Serial.print("$13\r\n");
    Serial.print("arduino:pin:8\r\n");
    Serial.print("$13\r\n");
    Serial.print("arduino:pin:9\r\n");
    Serial.print("$14\r\n");
    Serial.print("arduino:pin:10\r\n");
    Serial.print("$14\r\n");
    Serial.print("arduino:pin:11\r\n");
    Serial.print("$14\r\n");
    Serial.print("arduino:pin:12\r\n");
    Serial.print("$14\r\n");
    Serial.print("arduino:pin:13\r\n");

    char results[1024];
    char buffer[128];
    
    memset(results, NULL, 1024);

    if (Serial.available()) {
      
      while(Serial.available()) {
        memset(buffer, NULL, 128);
        Serial.readBytes(buffer, 127);
        strcat(results, buffer);
      }

      Serial.print(results);

//      char *token = strtok(results, "\r\n");
//      for (int pin = 2; pin <= 13; pin++) {
//        token = strtok(NULL, "\r\n");
//        int pinValue = atoi(strtok(NULL, "\r\n"));
//        digitalWrite(pin, pinValue == 0 ? LOW : HIGH);
//      }
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
