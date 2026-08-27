#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <BH1750.h>

// light sensor
BH1750 sensor;

// WIFI and MQTT
WiFiClient wifi;
PubSubClient mqtt(wifi);

// WIFI name and password
const char* wifi_name = "ZTE_2.4G_53f5pm";
const char* wifi_password = "3pcdcd5x";

// MQTT broker address
const char* mqtt_broker = "192.168.1.2";

// light level needed to start sunlight
const float light_limit = 130.0;

// this stores the current sunlight state
bool sunlight_on = false;

void setup() {
  Serial.begin(9600);

  // this starts the light sensor
  Wire.begin();
  sensor.begin();

  // helps in connecting to WIFI
  WiFi.begin(wifi_name, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WIFI connected.");

  // Sets MQTT broker and port
  mqtt.setServer(mqtt_broker, 1883);
}

void connect_mqtt() {
  // this keeps trying until MQTT is connected
  while (!mqtt.connected()) {

    if (mqtt.connect("TerrariumLightSensor")) {
      Serial.println("MQTT connected.");
    } 
    else {
      delay(5000);
    }
  }
}

void loop() {

  // Checks WIFI connection
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  // Connects to MQTT
  if (!mqtt.connected()) {
    connect_mqtt();
  }

  mqtt.loop();

  // Reads light level from BH1750
  float light = sensor.readLightLevel();

  Serial.print("Light: ");
  Serial.print(light);
  Serial.println(" lux");

  // Checks if the light is above the limit
  if (light > light_limit && !sunlight_on) {

    mqtt.publish("terrarium/light", "SUNLIGHT_START");

    Serial.println("Sunlight started.");

    sunlight_on = true;
  }

  // Checks if the light goes below the limit
  else if (light <= light_limit && sunlight_on) {

    mqtt.publish("terrarium/light", "SUNLIGHT_STOP");

    Serial.println("Sunlight stopped.");

    sunlight_on = false;
  }

  // Waits before taking the next reading
  delay(2000);
}