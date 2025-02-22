import serial
import json

amount = 3


with open("networks_raw.json") as f:
    try:
        networks = json.load(f)
    except json.JSONDecodeError as e:
        print("Error: Could not parse JSON from file.")
        print(e)
        exit(1)


# Open serial port
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=10) 

def send_command(command):
    ser.write((command + "\n").encode())  # Send command


def read_response():
    response = ser.readline().decode().strip()  # Read one line
    return response


def scan():
    send_command(f"{amount}")  # Tell ESP32 to scan
    
    for i in range(amount):
        print(f"Scan {i+1}/{amount}")
            
        # Read JSON response
        raw_data = read_response()
        print(f"Received: {raw_data}")
        try:
            wifi_data = json.loads(raw_data)  # Convert JSON to Python list
        except json.JSONDecodeError:
            print("Error: Could not parse JSON from ESP32.")
            return

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
        location = "test"
        if name == "exit":
            break

        if name == "":
            print("Please enter a valid name.")
            print("Type 'exit' to quit.")
            print()
            continue

        print(f"name: {name}")
        print("=====================================")
        print(networks)
        print("=====================================")

        scan()

        print("=====================================")
        print(networks)
        print("=====================================")

        #write to file
        json.dump(networks, f, indent=4)

        print("Scanning complete.")
        print("Type 'exit' to quit.")
        print()

