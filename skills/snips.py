#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import paho.mqtt.client as mqtt
import json
import struct
import datetime
from threading import Timer 
import sys
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
            if ( m_in["command"] == "printimage" ) and ( "image" in m_in ):
                img = gui.load_image(PICDIR + "/" + m_in["image"])
                gui.generate_image_serial(img)
            if ( m_in["command"] == "clear" ) :
                gui.clear()
            if ( m_in["command"] == "printfile" ) and ( "file" in m_in ):
                if ( "repeat" in m_in ):
                    gui.print_file(PICDIR + "/" + m_in["file"],m_in["repeat"])
                else:
                    gui.print_file(PICDIR + "/" + m_in["file"],1)
            if ( m_in["command"] == "scroll" ) and ( "way" in m_in ) and ( "speed" in m_in ) and ( "steps" in m_in ):
                if ( m_in["way"] == "left" ):
                    gui.scroll_left(m_in["steps"],m_in["speed"])
                else:
                    gui.scroll_right(m_in["steps"],m_in["speed"])
            if ( m_in["command"] == "printdata" ) and ( "data" in m_in ):
                if ( "repeat" in m_in ):
                    gui.serial_print( m_in["data"],m_in["repeat"])
                else:
                    gui.serial_print( m_in["data"],1)
            if ( m_in["command"] == "printimage" ) and ( "image" in m_in ):
                img = gui.load_image(PICDIR + "/" + m_in["image"])
                gui.generate_image_serial(img)
                
def on_timer():
    global tmr
    gui.get_status()
    mqtt.publish('snips/notify/'+ SITE_ID + '/sensors' , '{ "temperature" : "' + gui.temperature + '" , "humidity" : "' + gui.humidity + '" }')   
    tmr = Timer(60, on_timer)     
    tmr.start()

def main():
    global tmr


    try:

        print("init arduino.")
        gui.clear()

        mqtt_parse = MQTT_ADDR.split(":")
        print("connecting to : "+ MQTT_ADDR)
       
        mqtt.on_connect = on_connect
        mqtt.on_message = on_message
        mqtt.connect(str(mqtt_parse[0]), int(mqtt_parse[1]))

        tmr = Timer(60, on_timer)
        tmr.start()

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
        gui.clear()
        #doorbell_seeyou('',False)
    except:
        print("Unexpected error:", sys.exc_info()[0])


if __name__ == "__main__":
	mqtt = mqtt.Client(SITE_ID)
	main()