# Controlling 4 Relays with ESP8266


This project aims to control 4 relays using the ESP8266 microcontroller. The project uses the popular WiFiManager, PubSubClient, and ArduinoJson libraries to connect the ESP8266 to a MQTT broker and receive commands to control the relays.

## Hardware Requirements
    ESP8266 microcontroller
    4 channel relay module
    Power supply for the relay module
    Jumper wires
    Breadboard

## Wiring Instructions

Connect the relay module to the ESP8266 as follows:
| Relay Module Pin | ESP8266 Pin |
| -------- | --------- |
| IN1      | D16       |
| IN2      | D14       |
| IN3      | D12       |
| IN4      | D13       |
| VCC      | 12v       |
| GND      | GND       |
	
	
## MQTT Message Format

To control the relays, a MQTT message must be sent to the server/ topic with the following JSON format:

```json
{
  "relayA": true,
  "relayB": false,
  "relayC": true,
  "relayD": true,
}
```

Each relay is represented by a letter (A, B, C, D) and its value is either true (to turn it on) or false (to turn it off).

## Libraries Used

    WiFiManager: a library for managing WiFi connection on ESP8266 devices
    PubSubClient: a lightweight MQTT library for ESP8266
    ArduinoJson: a library for working with JSON on Arduino and ESP8266 platforms

## Usage

    Flash the ESP8266 with the provided sketch.
    Power on the relay module and connect it to the ESP8266 according to the wiring instructions.
    Connect the ESP8266 to a WiFi network using the WiFiManager library.
    Connect the ESP8266 to a MQTT broker using the PubSubClient library.
    Send a MQTT message to the server/ topic with the desired relay states using the message format described above.

## License

This project is licensed under the MIT License.

![ESP8266_4Relay_module](ESP8266_4Relay_module.jpg)

I hope this helps! Let me know if you have any further questions.
