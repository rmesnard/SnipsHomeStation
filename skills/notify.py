#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import struct
from snipsTools import *
import datetime
import os
import sys
from PIL import Image
from PIL import ImageSequence
import serial

VERSION = '1.0.0'

config = SnipsConfigParser.read_configuration_file('/usr/share/snips/config/snips.toml')
config_common = config.get('snips-common')
config_notify = config.get('snips-notify')

MQTT_ADDR = str(config_common.get('mqtt')).replace('"', '')
SITE_ID = str(config_common.get('site_id')).replace('"', '')

SERIAL_OUT = str(config_notify.get('serial_out')).replace('"', '')
SERIAL_SPEED = int(config_notify.get('serial_speed'))
PICHEIGHT = int(config_notify.get('pannel_height'))
PICWIDTH = int(config_notify.get('pannel_width'))



def serial_print(serialdata)
    #open SERIAL_OUT  '/dev/ttyUSB0'
    ser = serial.Serial(SERIAL_OUT,SERIAL_SPEED,1)
    ser.write(serialdata)
    ser.close()

    print(serialdata)

def serial_get(command)
    #open SERIAL_OUT  '/dev/ttyUSB0'
    ser = serial.Serial(SERIAL_OUT)
    ser.write(command)
    line = ser.readline()
    ser.close()
    return (line)

def load_image(filename):
    im = Image.open(filename)

    if (im.width != PICWIDTH) and (im.height != PICHEIGHT):
        print("resize")
        im = im.resize((PICWIDTH,PICHEIGHT))

    rgb_im = im.convert('RGB')
    return(rgb_im)

def generate_image_serial(rgbimage):
    serialout = ""
    idx=0
    for x in range(PICWIDTH):
        for y in range(PICHEIGHT):
            r, g, b = rgbimage.getpixel((x, y))
            if ( r+g+b != 0 ):
                serialout += 'P'
                serialout += format(idx, '02x')
                serialout += format(r, '02x')
                serialout += format(g, '02x')
                serialout += format(b, '02x')
            idx+=1
    serialout += '.'
    serial_print(serialout)

def generate_scroll_left_anim_serial(filename):
    serialout = ""
    im = Image.open(filename)

    if (im.height != PICHEIGHT):
        print("resize")
        ratio = PICHEIGHT / im.height
        newpicwidth = im.width * ratio
        im = im.resize((newpicwidth,PICHEIGHT))
        print(ratio)
        print(newpicwidth)

    rgb_im = im.convert('RGB')
    idx=0
    for x in range(PICWIDTH):
        for y in range(PICHEIGHT):
            r, g, b = rgb_im.getpixel((x, y))
            if ( r+g+b != 0 ):
                serialout += 'P'
                serialout += format(idx, '02x')
                serialout += format(r, '02x')
                serialout += format(g, '02x')
                serialout += format(b, '02x')
            idx+=1
    serialout += '.S.'

    idx = (PICHEIGHT * PICWIDTH ) - PICHEIGHT
    for offset in range(im.width-PICWIDTH):
        serialout += '.L.'
        for y in range(PICHEIGHT):
            r, g, b = rgb_im.getpixel((x+offset, y))
            serialout += 'P'
            serialout += format(idx, '02x')
            serialout += format(r, '02x')
            serialout += format(g, '02x')
            serialout += format(b, '02x')
            idx+=1
    serialout += '.S.'
    serial_print(serialout)

def generate_anim_serial(filename):
    serialout = ""
    im = Image.open(filename)

    for frame in ImageSequence.Iterator(im):

        if (frame.width != PICWIDTH) and (frame.height != PICHEIGHT):
            print("resize")
            frame = frame.resize((PICWIDTH,PICHEIGHT))

        rgb_im = frame.convert('RGB')
        idx=0
        for x in range(PICWIDTH):
            for y in range(PICHEIGHT):
                r, g, b = rgb_im.getpixel((x, y))
                if ( r+g+b != 0 ):
                    serialout += 'P'
                    serialout += format(idx, '02x')
                    serialout += format(r, '02x')
                    serialout += format(g, '02x')
                    serialout += format(b, '02x')
                idx+=1
        serialout += '.'
        serialout += 'S.'
    serial_print(serialout)


def main():
    global previous_status
    global button_status

    print("start ")
    rgbim =  load_image("test.gif")
    generate_image_serial(rgbim)


if __name__ == "__main__":
    main()