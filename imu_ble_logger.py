import asyncio
import os
import csv
from datetime import datetime
from bleak import BleakScanner, BleakClient

TARGET_NAME = "Arduino"  # Match BLE.setLocalName()
CHAR_UUID = "2A60"

# Create the data folder if it doesn't exist
DATA_DIR = "data"
os.makedirs(DATA_DIR, exist_ok=True)

# Generate timestamped filename
timestamp_str = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
filename = os.path.join(DATA_DIR, f"imu_log_{timestamp_str}.csv")

async def run():
    print("Scanning for BLE devices...")
    devices = await BleakScanner.discover(timeout=5.0)

    imu_device = None
    for d in devices:
        print(f"Found: {d.name or 'Unnamed'} | Address: {d.address}")
        if d.name == TARGET_NAME:
            imu_device = d
            break

    if not imu_device:
        print("❌ IMUPeripheral not found.")
        return

    print(f"✅ Connecting to {imu_device.name} at {imu_device.address}...")

    async with BleakClient(imu_device.address) as client:
        print(f"Logging to: {filename}")

        # Open CSV file
        with open(filename, mode='w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            # Write CSV header
            writer.writerow(["timestamp", "ax", "ay", "az", "gx", "gy", "gz"])

            # Notification handler
            async def notification_handler(_, data):
                decoded = data.decode("utf-8").strip()
                try:
                    ax, ay, az, gx, gy, gz = map(float, decoded.split(","))
                    now = datetime.now().strftime("%H:%M:%S")
                    writer.writerow([now, ax, ay, az, gx, gy, gz])
                    print(f"{now} | {decoded}")
                except ValueError:
                    print(f"Skipped malformed data: {decoded}")

            await client.start_notify(CHAR_UUID, notification_handler)
            print("Receiving and logging IMU data (Ctrl+C to stop)...")

            while True:
                await asyncio.sleep(1)

if __name__ == "__main__":
    try:
        asyncio.run(run())
    except KeyboardInterrupt:
        print("Logging stopped by user.")
