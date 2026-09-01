#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>

// ---------------- Wi-Fi ----------------
char ssid[] = "ZTE_2.4G_53f5pm";
char pass[] = "3pcdcd5x";

// ---------------- MQTT ----------------
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.emqx.io";
const int port = 1883;

const char waveTopic[] = "ES/Wave";
const char patTopic[] = "ES/Pat";

const char name[] = "Koshank";

// ---------------- Pins ----------------
const int trigPin = 2;
const int echoPin = 3;

const int greenLED = 6;
const int whiteLED = 7;

// ---------------- Gesture variables ----------------
float distance;

int waveCount = 0;
bool inWaveArea = false;
bool patDetected = false;

unsigned long lastGestureTime = 0;
const unsigned long cooldown = 1000;


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(9600);

  while (!Serial) {
    ;
  }

  // LEDs
  pinMode(greenLED, OUTPUT);
  pinMode(whiteLED, OUTPUT);

  digitalWrite(greenLED, LOW);
  digitalWrite(whiteLED, LOW);

  // Ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // ---------------- Wi-Fi ----------------

  Serial.println("Connecting to WiFi...");

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.println("Trying again...");
    delay(5000);
  }

  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());


  // ---------------- MQTT ----------------

  Serial.println("Connecting to MQTT broker...");

  while (!mqttClient.connect(broker, port)) {
    Serial.println("MQTT connection failed. Trying again...");
    delay(5000);
  }

  Serial.println("Connected to MQTT broker!");

  mqttClient.subscribe(waveTopic);
  mqttClient.subscribe(patTopic);

  Serial.println("Subscribed to ES/Wave");
  Serial.println("Subscribed to ES/Pat");

  Serial.println("System ready!");
}


// =====================================================
// LOOP
// =====================================================

void loop() {

  // Keep MQTT connection active
  mqttClient.poll();


  // ===================================================
  // CHECK MQTT MESSAGES
  // ===================================================

  int messageSize = mqttClient.parseMessage();

  if (messageSize) {

    String topic = mqttClient.messageTopic();

    Serial.print("Received on ");
    Serial.println(topic);

    Serial.print("Message: ");

    while (mqttClient.available()) {
      Serial.print((char)mqttClient.read());
    }

    Serial.println();


    // ES/Wave → LEDs ON
    if (topic == waveTopic) {

      digitalWrite(greenLED, HIGH);
      digitalWrite(whiteLED, HIGH);

      Serial.println("Both LEDs ON");
    }


    // ES/Pat → LEDs OFF
    if (topic == patTopic) {

      digitalWrite(greenLED, LOW);
      digitalWrite(whiteLED, LOW);

      Serial.println("Both LEDs OFF");
    }

    Serial.println();
  }


  // ===================================================
  // ULTRASONIC SENSOR
  // ===================================================

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    delay(100);
    return;
  }

  distance = duration * 0.034 / 2;


  // ===================================================
  // DISPLAY DISTANCE
  // ===================================================

  static unsigned long lastPrint = 0;

  if (millis() - lastPrint >= 500) {

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    lastPrint = millis();
  }


  // ===================================================
  // PAT DETECTION
  // Pat = 8 cm or less
  // ===================================================

  if (distance <= 8) {

    if (!patDetected &&
        millis() - lastGestureTime > cooldown) {

      Serial.println("PAT DETECTED");

      // Publish
      mqttClient.beginMessage(patTopic);
      mqttClient.print(name);
      mqttClient.endMessage();

      Serial.println("Published Koshank to ES/Pat");

      // Immediately turn LEDs OFF
      digitalWrite(greenLED, LOW);
      digitalWrite(whiteLED, LOW);

      Serial.println("Both LEDs OFF");

      lastGestureTime = millis();

      patDetected = true;

      waveCount = 0;
      inWaveArea = false;
    }
  }


  // Hand moved away from pat area
  if (distance > 8) {
    patDetected = false;
  }


  // ===================================================
  // WAVE DETECTION
  // Wave = greater than 8 cm and up to 35 cm
  // ===================================================

  if (distance > 8 && distance <= 35) {

    if (!inWaveArea) {

      inWaveArea = true;
      waveCount++;
    }
  }


  // Hand moved outside wave area
  if (distance > 35) {

    if (inWaveArea) {
      inWaveArea = false;
    }

    // Two entries into wave area = wave
    if (waveCount >= 2 &&
        millis() - lastGestureTime > cooldown) {

      Serial.println("WAVE DETECTED");

      // Publish
      mqttClient.beginMessage(waveTopic);
      mqttClient.print(name);
      mqttClient.endMessage();

      Serial.println("Published Koshank to ES/Wave");

      // Immediately turn LEDs ON
      digitalWrite(greenLED, HIGH);
      digitalWrite(whiteLED, HIGH);

      Serial.println("Both LEDs ON");

      lastGestureTime = millis();

      waveCount = 0;
    }
  }

  delay(100);
}