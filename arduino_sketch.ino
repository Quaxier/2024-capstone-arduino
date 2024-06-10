#include <SPI.h>
#include <WiFiNINA.h> #WiFiNINA를 사용해야 WiFi Rev 2 모델에서 통신 가능
#include <PubSubClient.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* mqtt_server = "your_MQTT_broker_address";

WiFiClient espClient;
PubSubClient client(espClient);

const int pirPin = 2;
int pirState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);

  // WiFi 연결
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  client.setServer(mqtt_server, 1883);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ArduinoClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int sensorValue = digitalRead(pirPin);
  if (sensorValue == HIGH) {
    if (pirState == LOW) {
      Serial.println("Motion detected!");
      client.publish("sensor", "Motion detected");
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH) {
      Serial.println("Motion ended!");
      client.publish("sensor", "Motion ended");
      pirState = LOW;
    }
  }
  delay(100);
}
