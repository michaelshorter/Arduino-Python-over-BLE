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


## Known issues
One thing to watch out for is the name that the peripheral device broadcasts versus what the python script is looking for. My Arduino was broadcasting as 'arduino' even though it was 'IMUPeripheral' in the arduino sketch. It is easy to see what the python script is looking for in the scripts inital setup phase - it prints the BLE list in terminal.
