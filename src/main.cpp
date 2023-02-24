#include <Arduino.h>
#include <ArduinoJson.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h> 

#include "Timer.hpp"

#define PIN_LED         5
#define PIN_LED_ON_CHIP 2
#define PIN_RELAY_A     16
#define PIN_RELAY_B     14
#define PIN_RELAY_C     12
#define PIN_RELAY_D     13
#define PIN_BUTTON      4


// Update these with values suitable for your network
const char* mqtt_server = "your_server_ip";

#define PIN_RELAY_A     16
#define PIN_RELAY_B     14
#define PIN_RELAY_C     12
#define PIN_RELAY_D     13

WiFiManager wifiManager;
WiFiClient espClient;
PubSubClient client(espClient);

int WIFILEDState = LOW;
int ActiveLEDState = LOW;

bool portalRunning      = false;

const std::string LED_WIFI_NAME = "wifi";
const std::string LED_ACTIVE_NAME = "active";

struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

Button button1 = {PIN_BUTTON, 0, false};

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 


/* MQTT message example
{
  "relayA": true,
  "relayB": false,
  "relayC": true,
  "relayD": true,
}
*/

void callback(char* topic, byte* payload, unsigned int length) {
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload, length);

  if (doc.containsKey("relayA")) {
    bool state = doc["relayA"];
    digitalWrite(PIN_RELAY_A, state);
  }

  if (doc.containsKey("relayB")) {
    bool state = doc["relayB"];
    digitalWrite(PIN_RELAY_B, state);
  }

  if (doc.containsKey("relayC")) {
    bool state = doc["relayC"];
    digitalWrite(PIN_RELAY_C, state);
  }

  if (doc.containsKey("relayD")) {
    bool state = doc["relayD"];
    digitalWrite(PIN_RELAY_D, state);
  }

  if (doc.containsKey("upgrade")) {
    bool state = doc["upgrade"];
    portalRunning = state;
    if (portalRunning)
    {
      Timer::SetInterval(LED_WIFI_NAME, 100);
    }
    else
    {
      Timer::SetInterval(LED_WIFI_NAME, 1000);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Subscribe to the topics
      client.subscribe("auto-feeder");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void IRAM_ATTR buttonInput() {
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    button1.numberKeyPresses++;
    button1.pressed = true;
    last_button_time = button_time;
  }
}

void RunPortal() {
  if(portalRunning)
    wifiManager.process(); // do processing
}

void setup() {
  Serial.begin(115200);

  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, buttonInput, FALLING);

  Timer::Create(LED_ACTIVE_NAME, [](){
    digitalWrite(PIN_LED, ActiveLEDState);
    ActiveLEDState = !ActiveLEDState;
    }, 1000);

  Timer::Create(LED_WIFI_NAME, [](){
    digitalWrite(PIN_LED_ON_CHIP, WIFILEDState);
    WIFILEDState = !WIFILEDState;
    }, 2000);

  pinMode(PIN_RELAY_A, OUTPUT);
  pinMode(PIN_RELAY_B, OUTPUT);
  pinMode(PIN_RELAY_C, OUTPUT);
  pinMode(PIN_RELAY_D, OUTPUT);
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // put your setup code here, to run once:
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_LED_ON_CHIP, OUTPUT);

  wifiManager.autoConnect("ESP8266Relay", "password");
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.startWebPortal();
}

void loop() {
  RunPortal();
  client.loop();
  Timer::update();

  if (!client.connected()) {
    reconnect();
  }

  if (button1.pressed) {
    Serial.printf("Button has been pressed %u times\n", button1.numberKeyPresses);
    button1.pressed = false;
    Serial.println("Button Pressed haha, Starting Web Portal");
  }
}

