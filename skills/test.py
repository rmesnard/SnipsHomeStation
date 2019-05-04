#!/usr/bin/env python
# -*- coding: latin-1 -*-
 
from snipsTools import *
from notify import Notify

VERSION = '1.0.0'

config = SnipsConfigParser.read_configuration_file('/usr/share/snips/config/snips.toml')
config_notify = config.get('snips-notify')

SERIAL_OUT = str(config_notify.get('serial_out')).replace('"', '')
SERIAL_SPEED = int(config_notify.get('serial_speed'))
PICHEIGHT = int(config_notify.get('pannel_height'))
PICWIDTH = int(config_notify.get('pannel_width'))

ledpannel = Notify()
ledpannel.init(SERIAL_OUT,SERIAL_SPEED,PICHEIGHT,PICWIDTH)  

def main():

    ledpannel.clean()

    img = ledpannel.load_image("test.gif")
    ledpannel.generate_image_serial(img)
    #ledpannel.scrollleft(16,10)

    #ledpannel.clean()
    #ledpannel.effect_wave(0,0,50,10)
    #ledpannel.file_print("test.sef",3)

if __name__ == "__main__":
	main()