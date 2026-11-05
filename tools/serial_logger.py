import csv
import re
import serial

SERIAL_PORT = "COM9"
BAUD_RATE = 115200

OUTPUT_FILE = "data/sensor_data.csv"

HEADER = [
    "time_ms",
    "thumb",
    "index",
    "middle",
#    "pitch_deg",
#    "roll_deg",
]

NUMBER_PATTERN = re.compile(
    r"-?\d+(?:\.\d+)?"
)

with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as serial_port:
    print(f"Reading from {SERIAL_PORT}")
    print(f"Saving data to {OUTPUT_FILE}")
    print("Press Ctrl+C to stop.\n")

    with open(OUTPUT_FILE, "w", newline="") as csv_file:
        writer = csv.writer(csv_file)
        writer.writerow(HEADER)

        try:
            while True:
                raw_line = serial_port.readline()

                if not raw_line:
                    continue

                line = raw_line.decode(
                    "utf-8",
                    errors="ignore"
                ).strip()

                # skip blank lines
                if not line:
                    continue

                fields = line.split(",")

                # require six values
                if len(fields) != len(HEADER):
                    continue

                # skip debug messages
                if not all(
                    NUMBER_PATTERN.fullmatch(field)
                    for field in fields
                ):
                    continue

                writer.writerow(fields)
                csv_file.flush()

                print(line)

        except KeyboardInterrupt:
            print(f"\nData saved to {OUTPUT_FILE}")