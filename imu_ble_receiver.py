import asyncio
from bleak import BleakScanner, BleakClient

TARGET_NAME = "Arduino"  # must match BLE.setLocalName()
CHAR_UUID = "2A60"

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
        async def notification_handler(_, data):
            print(f"{data.decode('utf-8')}")

        print("Subscribing to IMU characteristic...")
        await client.start_notify(CHAR_UUID, notification_handler)

        print("Receiving notifications (press Ctrl+C to stop)...")
        while True:
            await asyncio.sleep(1)

if __name__ == "__main__":
    try:
        asyncio.run(run())
    except KeyboardInterrupt:
        print("Stopped by user.")
