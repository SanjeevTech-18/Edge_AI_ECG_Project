import serial
import csv
import time

# Set your COM port and baud rate correctly (check Arduino IDE for port)
ser = serial.Serial('COM6', 115200, timeout=1)
time.sleep(2)  # Give time for serial to initialize

filename = "ecg_data12k_samples.csv"
num_samples = 12000
sample_count = 0

with open(filename, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(["Time(ms)", "ECG_Value"])  # CSV header

    while sample_count < num_samples:
        try:
            if ser.in_waiting:
                line = ser.readline().decode('utf-8').strip()
                if line and ',' in line:
                    parts = line.split(',')
                    if len(parts) == 2:
                        writer.writerow(parts)
                        print(parts)
                        sample_count += 1
        except Exception as e:
            print("Error:", e)
            break

ser.close()
print("✅ Done: 500 samples saved to", filename)
