# AprilTags Pixy I2C Emulation Script
#
# This script allows your OpenMV Cam to transmit AprilTag detection data like
# a Pixy (CMUcam5) tracking colors in I2C mode. This script allows you to
# easily replace a Pixy (CMUcam5) color tracking sensor with an OpenMV Cam
# AprilTag tracking sensor. Note that this only runs on the OpenMV Cam M7.
#
# P4 = SCL
# P5 = SDA
#
# Note: The tag family is TAG36H11. Additionally, in order to for the
#       signature value of a tag detection to be compatible with pixy
#       interface libraries all tag ids have 8 added to them in order
#       to move them in the color code signature range. Finally, tags
#       are all reported as color code blocks...

# Comm Parameters ############################################################

max_blocks = 255
max_blocks_per_id = 255

i2c_address = 0x12

##############################################################################

###
# Changed to using rpc.put_bytes, which has a little more control
# Plan is to make it report back the number
###

import image, math, pyb, sensor, rpc, struct, time
from pyb import UART

# Camera Setup

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)

# Link Setup

#bus = pyb.I2C(2, pyb.I2C.SLAVE, addr = i2c_address)
#interface = rpc.rpc_i2c_slave(slave_addr=0x12)

# UART 3, and baudrate.
uart = UART(3, 115200)

# Helper Stuff
# Main Loop

clock = time.clock()
while(True):
    clock.tick()
    img = sensor.snapshot()
    tags = img.find_apriltags(families=image.TAG16H5) # default TAG16H5 family
    if tags and (max_blocks > 0) and (max_blocks_per_id > 0): # new frame
        # dat_buf = struct.pack("<h", 0xAA55)

        # sort biggest to smallest
        tagIndex = 0
        for tag in sorted(tags, key = lambda x: x.h() * x.w(), reverse = True)[0:max_blocks]:
            #img.draw_rectangle(tag.rect())
            #img.draw_cross(tag.cx(), tag.cy())
            uart.write("tag" + str(tagIndex) + "/id:" + str(tag.id()) + "\n")
            uart.write("tag" + str(tagIndex) + "/x:" + str(tag.cx()) + "\n")
            uart.write("tag" + str(tagIndex) + "/y:" + str(tag.cy()) + "\n")
            uart.write("tag" + str(tagIndex) + "/w:" + str(tag.w()) + "\n")
            uart.write("tag" + str(tagIndex) + "/h:" + str(tag.h()) + "\n")
            uart.write("tag" + str(tagIndex) + "/rot:" + str((tag.rotation() * 180) / math.pi) + "\n")
            tagIndex = tagIndex + 1
