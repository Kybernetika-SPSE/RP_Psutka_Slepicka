import json

# Load raw data from JSON file
def load_raw_data(file_path):
    try:
        with open(file_path) as f:
            return json.load(f)
    except FileNotFoundError:
        print(f"Error: {file_path} not found.")
        exit(1)
    except json.JSONDecodeError as e:
        print("Error: Could not parse JSON from file.")
        print(e)
        exit(1)

# Group data by name
def group_by_name(data_raw):
    grouped_data = {}
    for scan in data_raw:
        name = scan["name"]
        location = scan["location"]

        if name not in grouped_data:
            grouped_data[name] = {"location": location, "networks": {}}

        for network in scan["networks"]:
            bssid = network["bssid"]
            rssi = network["rssi"]

            if bssid not in grouped_data[name]["networks"]:
                grouped_data[name]["networks"][bssid] = []

            grouped_data[name]["networks"][bssid].append(rssi)
    return grouped_data

# Calculate the average RSSI for each network
def calculate_average_rssi(grouped_data):
    for name in grouped_data:
        for bssid in grouped_data[name]["networks"]:
            rssi_list = grouped_data[name]["networks"][bssid]
            grouped_data[name]["networks"][bssid] = sum(rssi_list) // len(rssi_list)
    return grouped_data

# Save formatted data to JSON file
def save_formatted_data(file_path, data):
    with open(file_path, 'w') as f:
        json.dump(data, f, indent=4)

# Main function to load, process, and save data
def main():
    raw_data = load_raw_data('scanData/networks_raw.json')
    grouped_data = group_by_name(raw_data)
    averaged_data = calculate_average_rssi(grouped_data)
    save_formatted_data('scanData/networks.json', averaged_data)

# Entry point of the script
if __name__ == "__main__":
    main()