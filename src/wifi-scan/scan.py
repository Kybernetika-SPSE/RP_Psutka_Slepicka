import serial
import json

# amount of scans performed per location
amount = 3

# Load networks from JSON file
def load_networks(file_path):
    try:
        with open(file_path) as f:
            return json.load(f)
    except json.JSONDecodeError as e:
        print("Error: Could not parse JSON from file.")
        print(e)
        exit(1)
    except FileNotFoundError:
        print(f"Error: {file_path} not found.")
        print("do you want to proceed with an empty list and create a new file? (y/n)")
        if input() == "y":
            return []
        else:
            exit(1)


# Save networks to JSON file
def save_networks(file_path, networks):
    with open(file_path, "w") as f:
        json.dump(networks, f, indent=4)


# Open serial port
def open_serial_port(port, baudrate, timeout):
    try:
        return serial.Serial(port, baudrate, timeout=timeout)
    except serial.SerialException as e:
        print("Error: Could not open serial port.")
        print(e)
        exit(1)


# Send command to ESP32
def send_command(ser, command):
    ser.write((command + "\n").encode())


# Read response from ESP32
def read_response(ser):
    response = ser.readline().decode().strip()
    return response


# Scan for networks and store in Python object
def scan(ser, amount, networks, location, name):
    send_command(ser, f"{amount}")  # Tell ESP32 to scan
    
    for i in range(amount):
        print(f"Scan {i+1}/{amount}")
            
        # Read JSON response
        raw_data = read_response(ser)
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


def main():
    networks = load_networks("scanData/networks_raw.json")
    ser = open_serial_port('/dev/ttyUSB0', 115200, timeout=10)

    try:
        while True:
            name = input("Enter name: ")
            location = "placeholder"  # Placeholder for location
            if name == "exit":
                break

            if name == "":
                print("Please enter a valid name.")
                print("Type 'exit' to quit.")
                print()
                continue

            print(f"name: {name}")

            # Scan for networks
            scan(ser, amount, networks, location, name)

            # Save networks to file
            save_networks("scanData/networks_raw.json", networks)

            print("Scanning complete.")
            print("Type 'exit' to quit.")
            print()
    finally:
        ser.close()
        

if __name__ == "__main__":
    main()