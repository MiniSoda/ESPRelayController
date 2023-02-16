#include <Arduino.h>
// #include <AsyncElegantOTA.h>
#include <Esp.h>
#include <WiFiManager.h> 

#include "Timer.hpp"

#define PIN_LED         5
#define PIN_LED_ON_CHIP 2
#define PIN_RELAY_A     16
#define PIN_RELAY_B     16
#define PIN_RELAY_C     16
#define PIN_RELAY_D      16
#define PIN_BUTTON      4


bool Relay1 = false;
unsigned int uDelay = 1000;
int WIFILEDState = LOW;
int ActiveLEDState = LOW;

WiFiManager wifiManager;
bool portalRunning      = false;
bool relayA = false;

const std::string LED_WIFI_NAME = "wifi";
const std::string LED_ACTIVE_NAME = "active";

// const char* ssid = "George";
// const char* password = "zhengjian";

// AsyncWebServer server(80);

struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

Button button1 = {PIN_BUTTON, 0, false};

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

void IRAM_ATTR buttonInput() {
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    button1.numberKeyPresses++;
    button1.pressed = true;
    last_button_time = button_time;
  }
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void RunPortal() {
  if(portalRunning)
    wifiManager.process(); // do processing
}

void setup() {
  Serial.begin(9600);
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


  // put your setup code here, to run once:
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_RELAY_A, OUTPUT);
  pinMode(PIN_LED_ON_CHIP, OUTPUT);
  pinMode(PIN_LED, HIGH);
  pinMode(PIN_RELAY_A, HIGH);
  pinMode(PIN_LED_ON_CHIP, HIGH);

  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);
  // Serial.println("");

  // Wait for connection
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.print("Connected to ");
  // Serial.println(ssid);
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());

  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   request->send(200, "text/plain", "Hi! I am ESP8266.");
  // });

  // AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  // server.begin();
  // Serial.println("HTTP server started");
  wifiManager.autoConnect("帮帮奔奔", "zhengjian");
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.startWebPortal();
}

void loop() {
  Timer::update();
  RunPortal();
  
  if (button1.pressed) {
    Serial.printf("Button has been pressed %u times\n", button1.numberKeyPresses);
    button1.pressed = false;
    Serial.println("Button Pressed haha, Starting Web Portal");
    //wifiManager.startWebPortal();
    portalRunning = true;

    if (relayA)
    {
      digitalWrite(PIN_RELAY_A, LOW);
      Timer::SetInterval(LED_WIFI_NAME, 500);
      relayA = false;
    }
    else
    {
      digitalWrite(PIN_RELAY_A, HIGH);
      Timer::SetInterval(LED_WIFI_NAME, 200);
      relayA = true;
    }
  }
  
}

