# Arduino-> Python over BLE
Arduino sketch and Python script to transmit Gyroscope and Accelerometer data via BLE to a python script. I have also included a version that writes the data to a new file everytime a new BLE connection has been made with the Arduino Nano 33 BLE Sesne Peripheral device.

## Python Set up:

Make sure Python3 is installed then run:
   ```
pip3 install bleak
   ```
   ```
python imu_ble_receiver.py
   ```


