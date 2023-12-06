import paho.mqtt.client as mqtt

# MQTT server details
MQTT_BROKER = "robomqtt.cs.wpi.edu"
MQTT_PORT = 1883
MQTT_USER = "team8"
MQTT_PASSWORD = "mons0892"

# Topics 
TOPIC_Z_ACCELERATION = "team8/imuZ"
TOPIC_IR_SENSOR = "team8/irDist"
TOPIC_SONAR = "team8/usDist"
TOPIC_X_POSITION = "team8/positionX"
TOPIC_Y_POSITION = "team8/positionY"

# Callback when the client receives a CONNACK response from the server
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    
    # Subscribe to multiple topics
    client.subscribe([(TOPIC_Z_ACCELERATION, 0), (TOPIC_IR_SENSOR, 0), (TOPIC_SONAR, 0), (TOPIC_X_POSITION, 0), (TOPIC_Y_POSITION, 0))])

# Callback when a PUBLISH message is received from the server
def on_message(client, userdata, msg):
    message = msg.payload.decode()
    #print(f"Received message '{message}' on topic '{msg.topic}'")

    if msg.topic == TOPIC_Z_ACCELERATION:
        print(f"Z Acceleration Data: {message}")
    if msg.topic == TOPIC_IR_SENSOR:
        print(f"IR Sensor Data: {message}")
    if msg.topic == TOPIC_SONAR:
        print(f"Sonar Data: {message}")
    if msg.topic == TOPIC_X_POSITION:
        print(f"X Position Data: {message}")
    if msg.topic == TOPIC_Y_POSITION:
        print(f"Y Position Data: {message}")
    

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
