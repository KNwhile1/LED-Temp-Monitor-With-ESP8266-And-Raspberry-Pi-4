// =====================================================================================================================
// Name: Client.imo
// Description: This program allows the control and sending of the state of an LED, as well as the acquisition and sending of temperature data from a sensor.
// Target: ESP8266
// Compiler: Arduino IDE
// Usage: Control a red LED and get the current temperature
// Restriction(s): None.
// History: 12/05/2024 | E. Zoukou / C. Courtemanche / K. Niamba / Creation;
//          1/28/2025 | K. Niamba / Modifications ---> Documentation translation (FR to ENG);
//                                                ---> The HTML page for the "handleRoot" function has been changed to simplify the page look;
// =====================================================================================================================

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "***";                            // Network name
const char* password = "***";                        // Password
const int serverPort = 80;                           // Web server port

int sensorPin = A0;                                  // Analog pin where the temperature sensor is connected
int ledPin = D1;                                     // Pin where the LED is connected

ESP8266WebServer server(80);                         // Create a web server instance on port 80

unsigned long previousMillis = 0;
const long interval = 5000;                          // Temperature update interval (5 seconds)

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.print("Connecting to network ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(ledPin, OUTPUT);                           // Set LED pin as output

  server.on("/", handleRoot);                        // Route to Root
  server.on("/temp", handleTemp);                    // Route to temperature
  server.on("/led/on", handleLedOn);                 // Route to turn on the LED
  server.on("/led/off", handleLedOff);               // Route to turn off the LED
  server.begin();                                    // Start the web server
}

void loop() {
  server.handleClient();                             // Handle client requests

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {  // Record the last time the temperature was updated
    previousMillis = currentMillis;
    updateTemperature();
  }
}

void handleRoot() {
  if (WiFi.status() == WL_CONNECTED) {
    String page = "<html><head><title>ESP8266 Connection</title>";
    page += "<h1>Connected! &#12411;&#40;&#8807;&#9643;&#8806;&#10083;*)o </h1>";
    page += "</head><body>";
    page += "</body></html>";
    server.send(200, "text/html", page);
  } else {
    String page = "<html><head><title>ESP8266 Connection</title>";
    page += "<h1>We are no longer connected... </h1>";
    page += "</head><body>";
    page += "</body></html>";
    server.send(200, "text/html", page);
  }
}

void handleTemp() {
  updateTemperature();
}

void updateTemperature() {
  int sensorValue = analogRead(sensorPin);
  float voltage = (sensorValue / 1024.0) * 3300.0;
  float temperature = voltage / 10.0;
  printf("%f\n", temperature);
  String page = "<html><head><title>Temperature</title>";
  page += "<meta http-equiv='refresh' content='5'>"; // Refresh the page every 5 sec
  page += "</head><body>";
  page += "<h1>Temperature</h1>";
  page += "<p><span>";
  page += temperature;
  page += "</span></p>";
  page += "</body></html>";

  server.send(200, "text/html", page);
}

void handleLedOn() {
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/plain", "LED On");
}

void handleLedOff() {
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "LED Off");
}