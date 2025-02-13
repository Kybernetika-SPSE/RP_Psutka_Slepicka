import os
import re
import csv

alloved_SSIDs = ["pf-gosl", "eduroam"]

#path to this files dir
dir_path = os.path.dirname(os.path.realpath(__file__))
all_logs = os.listdir(f"{dir_path}/logs")

#get all log names
print(dir_path)
all_logs = list(all_logs)
print(f"logs: {all_logs}")

#get all scan names
all_places = os.listdir(f"{dir_path}/places")
all_places = list(all_places)
print(f"places: {all_places}")


#get name of file
def get_name(file_name):
    file_name = file_name.split(".")
    return file_name[0]

all_logs = list(map(get_name, all_logs))
all_places = list(map(get_name, all_places))

#compare logs to places
missing_logs = []
for log in all_logs:
    if not log in all_places:
        missing_logs.append(log)

print(f"mising places: {missing_logs}")




#to file
for place in missing_logs:
    print(place)
    wifi_map = {}
    with open(f"{dir_path}/logs/{place}.log") as f:
        log = f.read()

        #seperate log into individual scans
        log = log.split("scan number: ")[1:-1]
        for scan in log:

            if scan[-11:-1] == "Setup done":
                continue

            #scan index
            print(re.findall("[0-9]+", scan)[0])

            #reset after ESP restart
            if scan[:2] == "0\n":
                wifi_map = {}

            #splits scan into individual elements
            scan = scan.split("\n")[1:-2]

            #group and save data for each router
            for router in scan:
                router = router.split(" - ")
                BSSID = router[0]
                RSSI = int(router[1])
                SSID = router[2]

                #ignore not aloved SSIDs
                if not SSID in alloved_SSIDs:
                    continue
           
                print(f"{BSSID}\t{RSSI}\t{SSID}")


                #if it is the first time the router is scanned
                if not BSSID in wifi_map:
                    wifi_map[BSSID] = [SSID, []]

                #append the data into the dict
                wifi_map[BSSID][1].append(RSSI)

    print(wifi_map)

    #write data to file
    fields = []
    rows = [[], [], []]
    for router in wifi_map:
        fields.append(router)
        rows[0].append(wifi_map[router][0])                                     #SSID
        rows[1].append(sum(wifi_map[router][1]) / len(wifi_map[router][1]))     #AVG RSSI
        rows[2].append(wifi_map[router][1])                                     #RSSI

    with open(f"{dir_path}/places/{place}.csv", "w") as csvfile:
        # creating a csv writer object
        csvwriter = csv.writer(csvfile)
        # writing the fields
        csvwriter.writerow(fields)
        # writing the data rows
        csvwriter.writerows(rows)
