import json

# read raw data
with open('networks_raw.json') as f:
    data_raw = json.load(f)

data = {}

# group by name
for scan in data_raw:
    name = scan["name"]
    location = scan["location"]

    # add name if not exists
    if name not in data:
        data[name] = {"location": location, "networks": {}}

    for network in scan["networks"]:
        bssid = network["bssid"]
        ssid = network["ssid"]
        rssi = network["rssi"]

        # add network if not exists
        if bssid not in data[name]["networks"]:
            data[name]["networks"][bssid] = []

        # add rssi
        data[name]["networks"][bssid].append(rssi)


# calculate average rssi
for name in data:
    for bssid in data[name]["networks"]:
        data[name]["networks"][bssid] = sum(data[name]["networks"][bssid]) // len(data[name]["networks"][bssid])


# save formatted data
with open('networks.json', 'w') as f:
    json.dump(data, f, indent=4)