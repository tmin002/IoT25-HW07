#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

void setup() {
  Serial.begin(115200);
  BLEDevice::init("IoT25-Team1-Server");

  BLEServer *pServer = BLEDevice::createServer();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x12);

  // iPhone connection test
  pAdvertising->setMinPreferred(0x06);

  BLEDevice::setPower(ESP_PWR_LVL_P9); // +9 dBm if needed
  BLEDevice::startAdvertising();
  Serial.println("BLE Advertising started");
}

void loop() {
  delay(1000);
}
