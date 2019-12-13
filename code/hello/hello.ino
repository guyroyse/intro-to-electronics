#include "WiFi.h"

const char* WIFI_SSID     = "Virus Distribution Network";
const char* WIFI_PASSWORD = "givemeyourdata";
const char* REDIS_HOST    = "192.168.0.1";
const int   REDIS_PORT    = 6379;

const int LED_PIN = 15;
int ledState = LOW;


void setup() {

  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(50);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}
 
void loop() {

  if (ledState == LOW) {
    ledState = HIGH;
  } else {
    ledState = HIGH;
  }

  digitalWrite(LED_PIN, ledState);
  
  WiFiClient client;
  
  if (!client.connect(REDIS_HOST, REDIS_PORT)) {
    Serial.println("connection failed");
    return;
  }

  client.print(String("*2\r\n$3\r\nGET\r\n$3\r\nfoo\r\n"));
               
  unsigned long timeout = millis();
  
  while (client.available() == 0) {
      if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
      }
  }

  while(client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");

}
