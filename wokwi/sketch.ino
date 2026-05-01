/**
 * Care Plus – Health Tracker (ESP32)
 * Sprint 2 – Edge Computing & Computer Systems – FIAP 2026
 *
 * Integrantes:
 *   Nicolas Forcione de Oliveira e Souza – RM566998
 *   Alexandre Constantino Furtado Junior – RM567188
 *   Enrico Dellatorre da Fonseca         – RM566824
 *   Leonardo Batista de Souza            – RM568558
 *   Matheus Freitas dos Santos           – RM567337
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// WiFi
const char* ssid     = "Wokwi-GUEST";
const char* password = "";

// MQTT – atualize com seu TCP Proxy do Railway
const char* mqtt_server = "switchyard.proxy.rlwy.net";
const int   mqtt_port   = 28405;

// FIWARE
const char* apikey    = "4jggokgpepnvsb2uv4s40d59ov";
const char* device_id = "sensor001";

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pinos
#define PIN_BPM   34
#define PIN_SPO2  35
#define PIN_TEMP  32

WiFiClient   espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED não encontrado");
    while (true);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  Serial.print("Conectando WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");

  client.setServer(mqtt_server, mqtt_port);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    if (client.connect("ESP32_CarePlus")) {
      Serial.println("conectado!");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  // Leitura dos sensores
  int   bpm  = map(analogRead(PIN_BPM),  0, 4095, 50, 120);
  int   spo2 = map(analogRead(PIN_SPO2), 0, 4095, 90, 100);
  float temp = map(analogRead(PIN_TEMP), 0, 4095, 360, 390) / 10.0;

  // Payload Ultralight 2.0
  char payload[64];
  snprintf(payload, sizeof(payload), "bpm|%d|spo2|%d|t|%.1f", bpm, spo2, temp);

  // Tópico FIWARE
  char topic[80];
  snprintf(topic, sizeof(topic), "/%s/%s/attrs", apikey, device_id);

  client.publish(topic, payload);
  Serial.print("Publicado: ");
  Serial.println(payload);

  // Display OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Care Plus - Health");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 14);
  display.print("BPM: ");
  display.println(bpm);

  display.setTextSize(1);
  display.setCursor(0, 38);
  display.print("SpO2: ");
  display.print(spo2);
  display.println(" %");

  display.setCursor(0, 50);
  display.print("Temp: ");
  display.print(temp, 1);
  display.println(" C");

  display.display();
  delay(3000);
}
