#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import paho.mqtt.client as mqtt
import json
import struct
import datetime
from notify import Notify

from snipsTools import *

VERSION = '1.0.0'

config = SnipsConfigParser.read_configuration_file('/usr/share/snips/config/snips.toml')
config_common = config.get('snips-common')

MQTT_ADDR = str(config_common.get('mqtt')).replace('"', '')
SITE_ID = str(config_common.get('site_id')).replace('"', '')

config_notify = config.get('snips-notify')

SERIAL_OUT = str(config_notify.get('serial_out')).replace('"', '')
SERIAL_SPEED = int(config_notify.get('serial_speed'))
PICHEIGHT = int(config_notify.get('pannel_height'))
PICWIDTH = int(config_notify.get('pannel_width'))

PICDIR = str(config_notify.get('image_directory')).replace('"', '')


# Init Arduino Sensors  Led Pannel and IO. 
gui = Notify()
gui.init(SERIAL_OUT,SERIAL_SPEED,PICHEIGHT,PICWIDTH)  


def on_connect(client, userdata, flags, rc):
    print('Connected to MQTT system')
    mqtt.subscribe("snips/notify/" + SITE_ID )

def on_message(client, userdata, msg):
    #print('msg.topic ' + msg.topic)
    if msg.topic == "snips/notify/" + SITE_ID:
        m_decode=str(msg.payload.decode("utf-8","ignore"))
        m_in=json.loads(m_decode) #decode json data
        if "command" in m_in:
            print("command = ",m_in["command"])
            if ( m_in["command"] == "loadimage" ) and ( "image" in m_in ):
                img = gui.load_image(m_in["image"])
                gui.generate_image_serial(img)
            if ( m_in["command"] == "clear" ) :
                gui.clean()


def main():

    try:

        print("init arduino.")
        gui.clean()

        mqtt_parse = MQTT_ADDR.split(":")
        print("connecting to : "+ MQTT_ADDR)
       
        mqtt.on_connect = on_connect
        mqtt.on_message = on_message
        mqtt.connect(str(mqtt_parse[0]), int(mqtt_parse[1]))
       
        while True:
            #process mqtt message
            mqtt.loop()
            
            #check the push button
            #button_status = GPIO.input(BUTTON_GPIO)
            if gui.check():
                gui.update_status()
                
                if (gui.buttonclick01_change) and (gui.buttonclick01_status == 1):
                    gui.buttonclick01_change = False
                    mqtt.publish('snips/doorbell/'+ SITE_ID,'{"command":"ring"}') 
                    print("ring")


    except KeyboardInterrupt: 
        print("End.")
        doorbell_seeyou('',False)
    except:
        print("Unexpected error:", sys.exc_info()[0])


if __name__ == "__main__":
	mqtt = mqtt.Client(SITE_ID)
	main()