# Arduino-> Python over BLE
Arduino sketch and Python script to transmit Gyroscope and Accelerometer data via BLE to a python script. I have also included a version that writes the data to a new file everytime a new BLE connection has been made with the Arduino Nano 33 BLE Sesne Peripheral device.


## Arduino > Python Set up:

Load the Peripheral code onto an Arduino.
Make sure Python3 is installed on your computer then run:
   ```
pip3 install bleak
   ```
   ```
python imu_ble_receiver.py
   ```
or
   ```
python imu_ble_logger.py
   ```

The range for this is OK - up to about 1m.


## Arduino > Arduino Set up:
Load the Peripheral code onto an Arduino.
Load the Central code onto an Arduino.
The range for this is not so good. Mine stopped working over a distance greater than 30cm or so.


## Arduino > Processing Set up:
Still to be developed

## Known issues
One thing to watch out for is the name that the peripheral device broadcasts versus what the python script is looking for. My Arduino was broadcasting as 'arduino' even though it was 'IMUPeripheral' in the arduino sketch. It is easy to see what the python script is looking for in the scripts inital setup phase - it prints the BLE list in terminal.
