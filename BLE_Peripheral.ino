#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

BLEService imuService("180F");
BLECharacteristic imuChar("2A60", BLENotify, 48);

const int LED_PIN = LED_BUILTIN;

unsigned long lastSend = 0;
const unsigned long sendInterval = 200;  // 5 Hz

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  if (!BLE.begin()) {
    Serial.println("Failed to start BLE!");
    while (1);
  }

  BLE.setLocalName("IMUPeripheral");
  BLE.setAdvertisedService(imuService);
  imuService.addCharacteristic(imuChar);
  BLE.addService(imuService);
  BLE.advertise();

  Serial.println("BLE IMU Peripheral started...");
}

void loop() {
  BLE.poll();

  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    digitalWrite(LED_PIN, HIGH);
    BLE.stopAdvertise();  // Important for stability

    while (central.connected()) {
      BLE.poll();

      unsigned long now = millis();
      if (now - lastSend >= sendInterval) {
        lastSend = now;

        float ax, ay, az, gx, gy, gz;
        if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
          IMU.readAcceleration(ax, ay, az);
          IMU.readGyroscope(gx, gy, gz);

          char buffer[48];
          snprintf(buffer, sizeof(buffer),
                   "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f",
                   ax, ay, az, gx, gy, gz);

          // ✅ only send if subscribed
          if (imuChar.subscribed()) {
            imuChar.writeValue(buffer);
            Serial.println(buffer);
          }
        }
      }
    }

    Serial.println("Disconnected from central.");
    digitalWrite(LED_PIN, LOW);
    BLE.advertise();  // Resume advertising for next connection
  }

  // LED blink while waiting
  static unsigned long lastBlink = 0;
  static bool ledState = false;
  if (!BLE.connected() && millis() - lastBlink > 500) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    lastBlink = millis();
  }
}
