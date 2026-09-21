#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

// Pins
const int PIR_PIN = 2;
const int SLIDER_PIN = 3;

const int LED1_PIN = 6;
const int LED2_PIN = 7;

// Dark-light threshold 
const float DARK_THRESHOLD = 20.0;

// Interrupt flags
volatile bool motionDetected = false;
volatile bool sliderPressed = false;

// PIR interrupt
void motionISR() {
  motionDetected = true;
}

// Slider interrupt
void sliderISR() {
  sliderPressed = true;
}

void setup() {

  Serial.begin(9600);

  // Inputs
  pinMode(PIR_PIN, INPUT);
  pinMode(SLIDER_PIN, INPUT_PULLUP);

  // LEDs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  // Start BH1750
  Wire.begin();

  if (lightMeter.begin()) {
    Serial.println("BH1750 started successfully.");
  }
  else {
    Serial.println("BH1750 error.");
  }

  // Attach interrupts
  attachInterrupt(
    digitalPinToInterrupt(PIR_PIN),
    motionISR,
    RISING
  );

  attachInterrupt(
    digitalPinToInterrupt(SLIDER_PIN),
    sliderISR,
    FALLING
  );

  Serial.println("System ready.");
}

void loop() {

  // PIR MOTION INTERRUPT

  if (motionDetected) {

    motionDetected = false;

    float lux = lightMeter.readLightLevel();

    Serial.print("Motion detected. Light level: ");
    Serial.print(lux);
    Serial.println(" lux");

    if (lux < DARK_THRESHOLD) {

      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, HIGH);

      Serial.println("It is dark.");
      Serial.println("Porch and hallway lights ON.");

    }
    else {

      Serial.println("It is bright.");
      Serial.println("Lights state unchanged.");
    }
  }


  // SLIDER INTERRUPT

  if (sliderPressed) {

    sliderPressed = false;

    // Toggle both LEDs
    digitalWrite(
      LED1_PIN,
      !digitalRead(LED1_PIN)
    );

    digitalWrite(
      LED2_PIN,
      !digitalRead(LED2_PIN)
    );

    Serial.println("Slider switch activated.");
    Serial.println("LED1 and LED2 toggled.");
  }
}