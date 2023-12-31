# Romi MQTT Printer
# Written by Kang Zhang
# 12-6-2023

# Library imports
import paho.mqtt.client as mqtt

# MQTT server details
MQTT_BROKER = "robomqtt.cs.wpi.edu"
MQTT_PORT = 1883
MQTT_USER = "team8"
MQTT_PASSWORD = "mons0892"

# Topics
TOPIC_Z_ACCELERATION = "team8/z_acceleration"
TOPIC_IR_SENSOR = "team8/ir_sensor"
TOPIC_SONAR = "team8/sonar"

# Callback when the client receives a CONNACK response from the server
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribe to multiple topics
    client.subscribe([(TOPIC_Z_ACCELERATION, 0), (TOPIC_IR_SENSOR, 0), (TOPIC_SONAR, 0)])

# Callback when a PUBLISH message is received from the server
def on_message(client, userdata, msg):
    message = msg.payload.decode()
    print(f"Received message '{message}' on topic '{msg.topic}'")

    # Print out the data based on the topic
    if msg.topic == TOPIC_Z_ACCELERATION:
        print(f"Z Acceleration Data: {message}")
    elif msg.topic == TOPIC_IR_SENSOR:
        print(f"IR Sensor Data: {message}")
    elif msg.topic == TOPIC_SONAR:
        print(f"Sonar Data: {message}")

# Create MQTT client
client = mqtt.Client()

# Set username and password
client.username_pw_set(MQTT_USER, MQTT_PASSWORD)

# Assign the callbacks
client.on_connect = on_connect
client.on_message = on_message

# Connect to MQTT broker
client.connect(MQTT_BROKER, MQTT_PORT, 60)

# Start the loop to process received messages
client.loop_forever()