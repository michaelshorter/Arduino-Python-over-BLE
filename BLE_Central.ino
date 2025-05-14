#include <ArduinoBLE.h>

const char* TARGET_NAME = "IMUPeripheral";
const char* SERVICE_UUID = "180F";
const char* CHAR_UUID = "2A60";

BLEDevice peripheral;
BLECharacteristic imuChar;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while (1);
  }

  Serial.println("Scanning for peripheral...");
  BLE.scan();
}

void loop() {
  BLE.poll();

  if (!peripheral) {
    BLEDevice dev = BLE.available();
    if (dev && dev.localName() == TARGET_NAME) {
      BLE.stopScan();
      peripheral = dev;
      Serial.print("Found: ");
      Serial.println(peripheral.address());
    }
    return;
  }

  if (!peripheral.connected()) {
    Serial.println("Connecting...");
    if (peripheral.connect()) {
      Serial.println("Connected!");

      if (peripheral.discoverAttributes()) {
        imuChar = peripheral.characteristic(CHAR_UUID);
        if (imuChar && imuChar.canSubscribe()) {
          imuChar.subscribe();
          Serial.println("Subscribed to IMU data.");
        }
      }
    } else {
      Serial.println("Connection failed. Rescanning...");
      peripheral = BLEDevice();
      BLE.scan();
    }
    return;
  }

  if (imuChar && imuChar.valueUpdated()) {
    char buffer[64];
    int len = imuChar.readValue(buffer, sizeof(buffer) - 1);
    buffer[len] = '\0';
    Serial.println(buffer);
  }

  if (!peripheral.connected()) {
    Serial.println("Disconnected. Restarting scan...");
    peripheral = BLEDevice();
    BLE.scan();
  }
}
