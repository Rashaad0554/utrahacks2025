import cv2
import requests
import numpy as np
import serial
import time
from ultralytics import YOLO

# Load trained YOLO model
model = YOLO("C:/Users/Sherv/Desktop/Utra Hacks/best.pt")

# ESP32-CAM Stream URL (Replace with your ESP32's IP)
ESP32_URL = "http://192.168.92.79/capture"

# Connect to Arduino (Change COM port as needed, e.g., "COM3" or "/dev/ttyUSB0")
arduino = serial.Serial("COM8", 9600, timeout=1)
time.sleep(2)  # Wait for the connection to establish

# Initialize detection count
detection_count = 0  
max_detections = 3  # Keep LED on for 3 cycles after detection

while True:
    try:
        # Fetch image from ESP32-CAM
        response = requests.get(ESP32_URL, timeout=5)
        if response.status_code != 200:
            print(f"Error: Unable to fetch image. Status code: {response.status_code}")
            continue

        # Decode image
        img_array = np.asarray(bytearray(response.content), dtype=np.uint8)
        frame = cv2.imdecode(img_array, cv2.IMREAD_COLOR)

        # Check if the frame is valid
        if frame is None or frame.size == 0:
            print("Error: Invalid frame received from ESP32-CAM.")
            continue

        # Run object detection
        results = model(frame)

        # Default to no detection
        helipad_detected = False

        # Draw detection results
        for r in results:
            for box in r.boxes:
                x1, y1, x2, y2 = map(int, box.xyxy[0])
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.putText(frame, "Helipad", (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
                helipad_detected = True  # Helipad detected

        # Update detection count
        if helipad_detected:
            detection_count = max_detections  # Reset counter to 3 when detection occurs
        else:
            detection_count = max(0, detection_count - 1)  # Decrease count but keep at least 0

        # Send signal to Arduino
        if detection_count > 0:
            arduino.write(b'1')  # Send '1' to turn LED ON
            print(f"Helipad detected - LED ON (remaining cycles: {detection_count})")
        else:
            arduino.write(b'0')  # Send '0' to turn LED OFF
            print("No helipad - LED OFF")

        # Display video feed
        cv2.imshow("ESP32-CAM Helipad Detection", frame)

        # Exit if 'q' is pressed
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

        time.sleep(0.5)  # Prevent spamming the serial port

    except Exception as e:
        print(f"Error: {e}")

cv2.destroyAllWindows()
arduino.close()
