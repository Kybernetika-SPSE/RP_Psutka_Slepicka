import serial
import json

# amount of scans performed per location
amount = 3

# Parse JSON file into Python object (list of dictionaries)
with open("networks_raw.json") as f:
    try:
        networks = json.load(f)
    except json.JSONDecodeError as e:
        print("Error: Could not parse JSON from file.")
        print(e)
        exit(1)


# Open serial port
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=10) 

# Send command to ESP32
def send_command(command):
    ser.write((command + "\n").encode())  


# Read response from ESP32
def read_response():
    response = ser.readline().decode().strip()
    return response


# Scan for networks and store in Python object
def scan():
    send_command(f"{amount}")  # Tell ESP32 to scan
    
    for i in range(amount):
        print(f"Scan {i+1}/{amount}")
            
        # Read JSON response
        raw_data = read_response()
        try:
            wifi_data = json.loads(raw_data)  # Convert JSON to Python list
        except json.JSONDecodeError:
            print(f"Received: {raw_data}")
            print("Error: Could not parse JSON from ESP32.")
            return

        # Store networks in Python object
        networks.append({
            "location": location,
            "name": name,
            "networks": wifi_data
        })
        
        # Print networks
        for net in wifi_data:
            print(f"BSSID: {net['bssid']}, RSSI: {net['rssi']} SSID: {net['ssid']}")
          
        print()




# Main loop
with open("networks_raw.json", "w") as f:
    while True:
        name = input("Enter name: ")
        #location = input("Enter location: ")
        location = "placeholder" # Placeholder for location
        if name == "exit":
            break

        if name == "":
            print("Please enter a valid name.")
            print("Type 'exit' to quit.")
            print()
            continue

        print(f"name: {name}")

        # Scan for networks
        scan()

        #write to file
        json.dump(networks, f, indent=4)

        print("Scanning complete.")
        print("Type 'exit' to quit.")
        print()