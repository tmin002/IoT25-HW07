#include <WiFi.h>
#include <WebServer.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

const char* ssid = "song2g";
const char* password = "11111aaaaa";

WebServer server(80);
BLEScan* pBLEScan;

float lastDistance = 0.0;
const int LED_PIN = 2;
const float THRESHOLD = 1.0;

// model constants
const float txPower = -58.0;
const float pathLossExp = 2.2;

float calculateDistance(int rssi) {
  return pow(10.0, (txPower - rssi) / (10.0 * pathLossExp));
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == "Iot25-Team1-Client") {
      int rssi = advertisedDevice.getRSSI();
      lastDistance = calculateDistance(rssi);
      Serial.printf("RSSI: %d dBm, Distance: %.2f m\n", rssi, lastDistance);
      digitalWrite(LED_PIN, (lastDistance <= THRESHOLD) ? HIGH : LOW);
    }
  }
};

// serve webpage with dynamic updates
void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>BLE Distance Monitor</title>
      <meta charset="utf-8">
      <style>
        body {
          font-family: Arial, sans-serif;
          text-align: center;
          transition: background-color 0.5s;
          margin-top: 20vh;
        }
        h1 {
          font-size: 3em;
        }
      </style>
    </head>
    <body>
      <h1 id="distance">Fetching data please wait</h1>
      <script>
        function getColor(distance) {
          // Clamp distance between 0 and 5
          let d = Math.min(Math.max(distance, 0), 5);
          let intensity = Math.floor(255 - (d / 5) * 255);
          return `rgb(255, ${intensity}, ${intensity})`; // red to white
        }

        async function updateDistance() {
          try {
            const res = await fetch("/distance");
            const text = await res.text();
            const distance = parseFloat(text).toFixed(2);
            document.getElementById("distance").innerText = `Distance: ${distance} m`;
            document.body.style.backgroundColor = getColor(distance);
          } catch (e) {
            console.error("Fetch failed", e);
          }
        }

        setInterval(updateDistance, 1000);
        updateDistance();
      </script>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

// api endpoint
void handleDistance() {
  server.send(200, "text/plain", String(lastDistance, 2));
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  // wifi connect
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  // server start
  server.on("/", handleRoot);
  server.on("/distance", handleDistance);
  server.begin();

  // start BLE scanning
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(1, false);
}

void loop() {
  pBLEScan->start(1, false); // restart scan
  server.handleClient();
  delay(1000);
}
