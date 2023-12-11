# AprilTags MQTT Publisher
# Written by Christian Piper
# 12-8-2023

# Add bounding boxes and centers to frames
# Helps to see where the tags are detected
# Expensive to compute, so only use for debugging
annotateFrames = False

# Libraries
import image, math, sensor, time
from pyb import UART

# Camera Setup
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)

# Create a UART object to interface with ESP32
uart = UART(3, 115200)

# Main Loop
clock = time.clock()
while(True):
    clock.tick()

    # Pull a frame from the camera
    img = sensor.snapshot()

    # Find the tags in the image
    # Use TAG16H5 family for furthest range
    # Costs potential ID slots, but we don't need many
    tags = img.find_apriltags(families=image.TAG16H5)
    if tags:

        # Create a index for the tags
        # Helps to separate the tags in the MQTT messages
        tagIndex = 0
        for tag in sorted(tags, key = lambda x: x.id()):

            # Visual tag detection (for debugging)
            # Expensive to compute, so only use for debugging
            if annotateFrames:
                img.draw_rectangle(tag.rect())
                img.draw_cross(tag.cx(), tag.cy())

            # Send the tag data over UART
            uart.write("tag" + str(tagIndex) + "/id:" + str(tag.id()) + "\n")
            uart.write("tag" + str(tagIndex) + "/x:" + str(tag.cx()) + "\n")
            uart.write("tag" + str(tagIndex) + "/y:" + str(tag.cy()) + "\n")
            uart.write("tag" + str(tagIndex) + "/w:" + str(tag.w()) + "\n")
            uart.write("tag" + str(tagIndex) + "/h:" + str(tag.h()) + "\n")
            uart.write("tag" + str(tagIndex) + "/rot:" + str((tag.rotation() * 180) / math.pi) + "\n")

            # Increment the tag index so the next tag has a unique name
            tagIndex = tagIndex + 1
