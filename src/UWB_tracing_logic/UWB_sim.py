import random
import serial
import time
import re
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.patches import Circle


def calculate_distance(point1, point2):
    x = point1[0] - point2[0]
    y = point1[1] - point2[1]
    z = point1[2] - point2[2]

    noise = random.random() * 0.1
    noise *= 2
    noise -= 0.1

    return (x**2 + y**2 + z**2)**0.5

def get_random_point():
    x = (random.random() - 0.5) * 60
    y = (random.random() - 0.5) * 60
    z = (random.random() - 0.5) * 60

    return [x, y, 0]

def parse_data(data):
    global tagGuess
    match = re.search(r'Current state: x=([-+]?\d*\.\d+|\d+), y=([-+]?\d*\.\d+|\d+), z=([-+]?\d*\.\d+|\d+)', data)
    if match:
        x = float(match.group(1))
        y = float(match.group(2))
        z = float(match.group(3))
        tagGuess = [x, y, z]
        print(f"Parsed coordinates: x={x}, y={y}, z={z}")





def ESP_communication(): 
    global indexOfAnchor
    # Read the ESP32's response if any
    while ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        print("ESP32:", line)
        parse_data(line)


    # Send the anchor coordinates

    print(f"ANCHOR {indexOfAnchor}: {anchor[0]:.2f} {anchor[1]:.2f} {anchor[2]:.2f} {distance:.2f}")
    # ser.write(f"cords[{anchor[0]:.2f},{anchor[1]:.2f},{anchor[2]:.2f}],{distance:.2f}\n".encode())
    ser.write(f"cords[{anchor[0]:.2f},{anchor[1]:.2f},0],{distance:.2f}\n".encode())


indexOfAnchor = 0
tagGuess = [0, 0, 0]
anchor = [0, 0, 0]
distance  = 0

#tag = get_random_point()
tag = [15, -23, -15]
print(f"TAG:   {tag[0]:.<20} {tag[1]:.<20} {tag[2]:.<20}")

ser = serial.Serial('/dev/ttyUSB0', 115200)  # Change to the correct port
time.sleep(2)  # Give some time for the connection to establish






# ========= Animation =========

# Settings
N = 10  # Number of points to keep in the trail

# Initial state
points = np.array([[0, 0]])
radii = np.array([0])

# Setup plot
fig, ax = plt.subplots()
trail_scatter = ax.scatter([], [], color='gray')  # Trail points
head_scatter = ax.scatter([], [], color='red', marker="D")  # Current anchor point
tag_scatter = ax.scatter([], [], color='green', marker="x")  # Tag position
ax.set_xlim(-30, 30)
ax.set_ylim(-30, 30)
ax.grid(True)

# Initialize circles
circles = []

# Animation update
def update(frame):
    global tagGuess, points, radii, circles, indexOfAnchor, anchor, distance

    # Update anchor position and distance
    anchor = get_random_point()
    distance = calculate_distance(anchor, tag)
    indexOfAnchor += 1

    # Read data from ESP
    ESP_communication()

    # Use the updated `anchor` and `distance` values
    points = np.vstack((points, anchor[:2]))[-N:]  # Keep only the last N points
    radii = np.append(radii, distance)[-N:]  # Keep only the last N radii

    # Recenter view
    center = np.mean(points, axis=0)
    span = 60  # Adjust the span to fit the range of your data
    ax.set_xlim(center[0] - span / 2, center[0] + span / 2)
    ax.set_ylim(center[1] - span / 2, center[1] + span / 2)

    # Update trail and head
    if len(points) > 1:
        trail_scatter.set_offsets(points[:-1])  # Update trail points
    else:
        trail_scatter.set_offsets([])

    head_scatter.set_offsets(points[-1:])  # Update the current anchor point

    # Update tag position
    tag_scatter.set_offsets(tagGuess[:2])  # Use the guessed tag position
    tag_scatter.set_sizes([100])

    # Sync circle count
    while len(circles) < len(points):
        c = Circle((0, 0), 0.05, fill=False, color='blue', linestyle='--')
        ax.add_patch(c)
        circles.append(c)
    while len(circles) > len(points):
        c = circles.pop()
        c.remove()

    # Update circles
    for i, c in enumerate(circles):
        c.center = points[i]
        c.set_radius(radii[i])

    return [trail_scatter, head_scatter, tag_scatter] + circles


# Create the animation
ani = animation.FuncAnimation(fig, update, interval=1000, blit=False)
plt.show()
