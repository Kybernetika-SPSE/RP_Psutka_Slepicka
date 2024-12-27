import os
import re


#path to this files dir
dir_path = os.path.dirname(os.path.realpath(__file__))

#filters out files that are not logs
def islog(name):
    return name[:3] == "log"
all_logs = filter(islog, os.listdir(dir_path))
print(dir_path)
all_logs = list(all_logs)

print(all_logs)

#returns the time of each log from its name
def get_time(name):
    return re.findall("[0-9]+", name)[0]
time = []
for i in range(len(all_logs)):
    time.append(get_time(all_logs[i]))

#last logs name
print(f"{dir_path}/log.scan.{max(time)}.txt")






#read from log
wifi_map = {}
with open(f"{dir_path}/log.scan.{max(time)}.txt") as f:
    log = f.read()

    #seperate log into individual scans
    log = log.split("scan number: ")
    for scan in log:

        #scan index
        print(re.findall("[0-9]+", scan)[0])

        #reset after ESP restart
        if scan[:2] == "1\x1b":
            wifi_map = {}

        #splits scan into individual elements
        scan = scan.split("\n")[1:-1]

        #group and save data for each router
        if len(scan) % 3 == 0:

            for j in range(0, len(scan), 3):
                SSID = scan[j].split("\t\t")[-1][:-4]
                BSSID = scan[j+1].split("\t\t")[-1][:-4]
                RSSI = scan[j+2].split("\t\t")[-1][:-4]

                print(f"{BSSID}\t{RSSI}\t{SSID}")

                #if it is the first time the router is scanned
                if not BSSID in wifi_map:
                    wifi_map[BSSID] = [SSID, []]

                #append the data into the dict
                wifi_map[BSSID][1].append(RSSI)

print(wifi_map)

#write data to file
output = open(f"{dir_path}/forged_logs/log-{max(time)}", "w")
output.write(f"{wifi_map}")
output.close()