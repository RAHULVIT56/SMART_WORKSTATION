import serial
import json
import requests
import time

# ---------------- FIREBASE CONFIG ---------------- #
FIREBASE_URL = "https://smartgarmentsystem-default-rtdb.asia-southeast1.firebasedatabase.app"

# ---------------- SERIAL CONNECTIONS ---------------- #
try:
    arduino_a = serial.Serial('COM4', 9600, timeout=2)  # Operator board
    arduino_b = serial.Serial('COM3', 9600, timeout=2)  # Sensor board
    print("✅ Connected to Arduino A (COM4) and Arduino B (COM3)")
except Exception as e:
    print("❌ Serial connection error:", e)
    exit()

print("🔥 Uploading real-time data to Firebase...\n")

# ---------------- MAIN LOOP ---------------- #
while True:
    try:
        # ----- Arduino A (Operator Data) ----- #
        if arduino_a.in_waiting > 0:
            line_a = arduino_a.readline().decode('utf-8', errors='ignore').strip()
            if line_a.startswith("{") and line_a.endswith("}"):
                try:
                    data_a = json.loads(line_a)
                    operator_payload = {
                        "name": data_a.get("name", "Rahul"),
                        "garments": data_a.get("garments", 0),
                        "efficiency": data_a.get("efficiency", 0),
                        "timestamp": time.strftime("%Y-%m-%d %H:%M:%S")
                    }
                    requests.put(f"{FIREBASE_URL}/operator_data/Rahul.json", json=operator_payload)
                    print("👕 Operator Data:", operator_payload)
                except json.JSONDecodeError:
                    print("⚠️ Invalid JSON from Arduino A:", line_a)

        # ----- Arduino B (Sensor Data) ----- #
        if arduino_b.in_waiting > 0:
            line_b = arduino_b.readline().decode('utf-8', errors='ignore').strip()
            if line_b.startswith("{") and line_b.endswith("}"):
                try:
                    data_b = json.loads(line_b)
                    sensor_payload = {
                        "presence": data_b.get("presence", "Unknown"),
                        "machine": data_b.get("machine", "Unknown"),
                        "timestamp": time.strftime("%Y-%m-%d %H:%M:%S")
                    }
                    requests.put(f"{FIREBASE_URL}/sensor_data.json", json=sensor_payload)
                    print("⚙️ Sensor Data:", sensor_payload)
                except json.JSONDecodeError:
                    print("⚠️ Invalid JSON from Arduino B:", line_b)

        time.sleep(1)

    except KeyboardInterrupt:
        print("\n🛑 Exiting uploader safely...")
        break
    except Exception as e:
        print("⚠️ Error:", e)
        time.sleep(2)
