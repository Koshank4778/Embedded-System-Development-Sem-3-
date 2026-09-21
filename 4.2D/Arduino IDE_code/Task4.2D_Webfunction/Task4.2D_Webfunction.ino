#include <WiFiNINA.h>

// Wi-Fi network used by the Arduino and laptop.
char ssid[] = "vivo";
char pass[] = "tetrabond";

// Create the Arduino web server on port 80.
WiFiServer server(80);

// Arduino pins connected to the three room LEDs.
const int livingRoomLED = 2;
const int bathroomLED = 3;
const int closetLED = 4;

// Control a specific room LED using its room name and required state.
void controlRoom(String room, String state) {

  // Control the living room LED.
  if (room == "livingroom") {

    if (state == "ON") {
      digitalWrite(livingRoomLED, HIGH);
    }
    else if (state == "OFF") {
      digitalWrite(livingRoomLED, LOW);
    }
  }

  // Control the bathroom LED.
  else if (room == "bathroom") {

    if (state == "ON") {
      digitalWrite(bathroomLED, HIGH);
    }
    else if (state == "OFF") {
      digitalWrite(bathroomLED, LOW);
    }
  }

  // Control the closet LED.
  else if (room == "closet") {

    if (state == "ON") {
      digitalWrite(closetLED, HIGH);
    }
    else if (state == "OFF") {
      digitalWrite(closetLED, LOW);
    }
  }
}


void setup() {

  // Start serial communication for monitoring.
  Serial.begin(9600);

  // Set the LED pins as outputs.
  pinMode(livingRoomLED, OUTPUT);
  pinMode(bathroomLED, OUTPUT);
  pinMode(closetLED, OUTPUT);

  // Turn all LEDs off when the Arduino starts.
  digitalWrite(livingRoomLED, LOW);
  digitalWrite(bathroomLED, LOW);
  digitalWrite(closetLED, LOW);

  // Connect the Arduino to the Wi-Fi network.
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {

    Serial.println("Connecting to Wi-Fi...");
    delay(3000);
  }

  // Start the Arduino web server.
  server.begin();

  // Display the Arduino IP address.
  Serial.print("Arduino IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {

  // Check for an incoming connection from server.js.
  WiFiClient client = server.available();

  if (client) {

    // Read the first line of the HTTP request.
    String request = client.readStringUntil('\n');

    // Display the request in Serial Monitor.
    Serial.println(request);


    // Turn the living room LED ON or OFF.
    if (request.indexOf("GET /toggle?room=livingroom&state=ON") >= 0) {

      controlRoom("livingroom", "ON");
    }

    else if (request.indexOf("GET /toggle?room=livingroom&state=OFF") >= 0) {

      controlRoom("livingroom", "OFF");
    }


    // Turn the bathroom LED ON or OFF.
    else if (request.indexOf("GET /toggle?room=bathroom&state=ON") >= 0) {

      controlRoom("bathroom", "ON");
    }

    else if (request.indexOf("GET /toggle?room=bathroom&state=OFF") >= 0) {

      controlRoom("bathroom", "OFF");
    }


    // Turn the closet LED ON or OFF.
    else if (request.indexOf("GET /toggle?room=closet&state=ON") >= 0) {

      controlRoom("closet", "ON");
    }

    else if (request.indexOf("GET /toggle?room=closet&state=OFF") >= 0) {

      controlRoom("closet", "OFF");
    }


    // Send a response back to server.js.
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println("LED command completed");

    // Close the connection.
    client.stop();
  }
}