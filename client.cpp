#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

BLEScan* pBLEScan;

float lastDistance = 0.0;
const int LED_PIN = 2;
const float THRESHOLD = 0.;

const float txPower = -58.0;
const float pathLossExp = 2.2;

float calculateDistance(int rssi) {
  return pow(10.0, (txPower - rssi) / (10.0 * pathLossExp));
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == "IoT25-Team1-Server") {
      int rssi = advertisedDevice.getRSSI();
      lastDistance = calculateDistance(rssi);
      Serial.printf("RSSI: %d dBm, Distance: %.2f m\n", rssi, lastDistance);
      digitalWrite(LED_PIN, (lastDistance <= THRESHOLD) ? HIGH : LOW);
    }
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(1, false);  // 1초 동안 비동기 스캔
}

void loop() {
  pBLEScan->start(1, false); // 매 루프마다 스캔 재시작
  delay(100);
}