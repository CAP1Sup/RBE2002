import paho.mqtt.client as mqtt
import matplotlib.pyplot as plt
import numpy as np
import time
import queue
from matplotlib.animation import FuncAnimation

# MQTT server details
MQTT_BROKER = "robomqtt.cs.wpi.edu"
MQTT_PORT = 1883
MQTT_USER = "team8"
MQTT_PASSWORD = "mons0892"

# Topics
TOPIC_ID0 = ["theIlluminati/tag0/x", "theIlluminati/tag0/y", "theIlluminati/tag0/rot", "theIlluminati/tag0/id"]
TOPIC_ID1 = ["theIlluminati/tag1/x", "theIlluminati/tag1/y", "theIlluminati/tag1/rot", "theIlluminati/tag1/id"]
TOPIC_ID2 = ["theIlluminati/tag2/x", "theIlluminati/tag2/y", "theIlluminati/tag2/rot", "theIlluminati/tag2/id"]

# Initialize data structure
tag_data = {
    TOPIC_ID0[0]: -200.0, TOPIC_ID0[1]: -200.0, TOPIC_ID0[2]: 0.0, TOPIC_ID0[3]: -1, # x, y, angle, ID
    TOPIC_ID1[0]: -200.0, TOPIC_ID1[1]: -200.0, TOPIC_ID1[2]: 0.0, TOPIC_ID1[3]: -1,
    TOPIC_ID2[0]: -200.0, TOPIC_ID2[1]: -200.0, TOPIC_ID2[2]: 0.0, TOPIC_ID2[3]: -1
}

# Create a queue
data_queue = queue.Queue()

# Modify the MQTT on_message callback
def on_message(client, userdata, msg):
    message = float(msg.payload.decode())
    tag_data[msg.topic] = message

    # Put the data into the queue
    x_vec = [tag_data[TOPIC_ID0[0]], tag_data[TOPIC_ID1[0]], tag_data[TOPIC_ID2[0]]]
    y_vec = [tag_data[TOPIC_ID0[1]], tag_data[TOPIC_ID1[1]], tag_data[TOPIC_ID2[1]]]
    angles = [tag_data[TOPIC_ID0[2]], tag_data[TOPIC_ID1[2]], tag_data[TOPIC_ID2[2]]]
    id = [tag_data[TOPIC_ID0[3]], tag_data[TOPIC_ID1[3]], tag_data[TOPIC_ID2[3]]]
    
    data_queue.put((x_vec, y_vec, angles, id)) # put the data into the queue

# Callback when the client receives a CONNACK response from the server
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("theIlluminati/tag0/#")
    client.subscribe("theIlluminati/tag1/#")
    client.subscribe("theIlluminati/tag2/#")
 
# Set up the figure and axis
fig, ax = plt.subplots(figsize=(10, 6))
ax.set_xlabel('X Axis')
ax.set_ylabel('Y Axis')
ax.set_title('Live Data Plot with Direction Arrows')
ax.set_xlim(0, 160)
ax.set_ylim(0, 120)

# Initialize lines and arrows
lines = [ax.plot([], [], '-o', alpha=0.8)[0] for _ in range(3)] 
arrows = [ax.quiver([], [], [], [], angles='xy', scale_units='xy', scale=1) for _ in range(3)]

# Function to update the plot
def update_plot(frame):
    try:
        x_vec, y_vec, angles, id = data_queue.get_nowait() # get the data from the queue
        data_queue.queue.clear() # clear the queue (helps update the plot in real time by removing a backlog of data)
        print(x_vec[0], y_vec[0], angles[0], id[0])

        for i in range(3): # update the lines and arrows
            lines[i].set_data(x_vec[i], y_vec[i]) 
            arrows[i].remove()
            scale_factor = 1000
            dx = scale_factor * np.cos(angles[i] * np.pi / 180) 
            dy = scale_factor * np.sin(angles[i] * np.pi / 180)
            arrows[i] = ax.quiver(x_vec[i], y_vec[i], dx, dy, angles='xy', scale_units='xy', scale=80, color=lines[i].get_color()) # generate the arrows for points
       
        return lines + arrows
    except queue.Empty: # if the queue is empty, return the previous lines and arrows
        return lines + arrows

# Use FuncAnimation to update the plot
ani = FuncAnimation(fig, update_plot, blit=True)

# Create and start MQTT client
client = mqtt.Client()
client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_BROKER, MQTT_PORT, 60)
client.loop_start() # start the loop
plt.show()


