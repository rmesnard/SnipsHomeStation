#!/usr/bin/env python
# -*- coding: utf-8 -*-

from snipsTools import *
import time
from PIL import Image
from PIL import ImageSequence
import serial
import os

VERSION = '1.0.0'

class Notify(object):

    __serial_out = None
    __serial_speed = None
    __pannel_height = None
    __pannel_width = None
    __arduino = None

    def init(self,serialout,serialspeed,pannelh,pannelw):
        self.__serial_out = serialout
        self.__serial_speed = serialspeed
        self.__pannel_height = pannelh
        self.__pannel_width = pannelw
        self.__arduino = serial.Serial(self.__serial_out,self.__serial_speed, timeout=1)
        time.sleep(2) 

    def test(self,texte):
        self.__arduino.write(texte.encode())
        
    def serial_print(self,serialdata,repeat=1):

        commands = serialdata.split('.')
        for nb in range(repeat):
            for cmd in commands:
                #print(cmd)
                self.__arduino.write(cmd.encode())
                time.sleep(0.01) 

        #f = open("arduino.txt", "w")
        #for  nb in range(repeat):
        #    f.write(serialdata)
        #f.close()

        #hello = self.__arduino.readline()
        #print(hello)

    def scrollright(self,steps,fps):
        serialout = 'R'
        serialout += format(steps, '02x')
        serialout += format(fps, '02x')
        serialout += '.'
        self.serial_print(serialout)

    def scrollleft(self,steps,fps):
        serialout = 'L'
        serialout += format(steps, '02x')
        serialout += format(fps, '02x')
        serialout += '.'
        self.serial_print(serialout)


    def clean(self):
        self.serial_print('C.',2)

    def serial_get(self,command):
        self.__arduino.write(command.encode())
        line = self.__arduino.readline()
        return (line)

    def load_image(self,filename):
        im = Image.open(filename)

        if (im.width != self.__pannel_width) and (im.height != self.__pannel_height):
            print("resize")
            im = im.resize((self.__pannel_width,self.__pannel_height))

        rgb_im = im.convert('RGB')
        return(rgb_im)

    def file_print(self,filename,repeat=1):
        f = open(filename, "r")
        contents = f.read()
        f.close()
        self.serial_print(contents,repeat)

    def generate_image_serial(self,rgbimage):
        serialout = ""
        idx=0
        for x in range(self.__pannel_width):
            for y in range(self.__pannel_height):
                r, g, b = rgbimage.getpixel((x, self.__pannel_height - y - 1))
                if ( r+g+b != 0 ):
                    serialout += 'P'
                    serialout += format(idx, '02x')
                    serialout += format(r, '02x')
                    serialout += format(g, '02x')
                    serialout += format(b, '02x')
                    serialout += '.'
                idx+=1
        serialout += 'S.'
        self.serial_print(serialout)

    def generate_scroll_left_anim_serial(self,filename):
        serialout = ""
        im = Image.open(filename)

        if (im.height != self.__pannel_height):
            print("resize")
            ratio = self.__pannel_height / im.height
            newpicwidth = im.width * ratio
            im = im.resize((newpicwidth,self.__pannel_height))
            print(ratio)
            print(newpicwidth)

        rgb_im = im.convert('RGB')
        idx=0
        for x in range(self.__pannel_width):
            for y in range(self.__pannel_height):
                r, g, b = rgb_im.getpixel((x, self.__pannel_height - y - 1))
                if ( r+g+b != 0 ):
                    serialout += 'P'
                    serialout += format(idx, '02x')
                    serialout += format(r, '02x')
                    serialout += format(g, '02x')
                    serialout += format(b, '02x')
                    serialout += '.'
                idx+=1
        serialout += 'S.'

        idx = (self.__pannel_height * self.__pannel_width ) - self.__pannel_height
        for offset in range(im.width-self.__pannel_width):
            serialout += '.L.'
            for y in range(self.__pannel_height):
                r, g, b = rgb_im.getpixel((x+offset, self.__pannel_height - y - 1))
                serialout += 'P'
                serialout += format(idx, '02x')
                serialout += format(r, '02x')
                serialout += format(g, '02x')
                serialout += format(b, '02x')
                serialout += '.'
                idx+=1
        serialout += 'S.'
        self.serial_print(serialout)

    def generate_anim_serial(self,filename):
        serialout = ""
        im = Image.open(filename)

        for frame in ImageSequence.Iterator(im):

            if (frame.width != self.__pannel_width) and (frame.height != self.__pannel_height):
                print("resize")
                frame = frame.resize((self.__pannel_width,self.__pannel_height))

            rgb_im = frame.convert('RGB')
            idx=0
            for x in range(self.__pannel_width):
                for y in range(self.__pannel_height):
                    r, g, b = rgb_im.getpixel((x, self.__pannel_height - y - 1))
                    if ( r+g+b != 0 ):
                        serialout += 'P'
                        serialout += format(idx, '02x')
                        serialout += format(r, '02x')
                        serialout += format(g, '02x')
                        serialout += format(b, '02x')
                        serialout += '.'
                    idx+=1
            serialout += 'S.'
        self.serial_print(serialout)


    def effect_wave(self,colorR,colorG,colorB,speed):
        serialout = ""
        lastline = False
        r = int(colorR)
        g = int(colorG)
        b = int(colorB)
        leneffect = self.__pannel_width + ( 255 / speed)
        currentintensity= 255
        while leneffect > 0 :
            serialout = ""
            for y in range(self.__pannel_height):
                serialout += 'P'
                serialout += format( y, '02X')
                serialout += format( r, '02X')
                serialout += format( g, '02X')
                serialout += format( b, '02X')
                serialout += '.'
            serialout += 'S.'
            currentintensity -= speed
            if ( currentintensity < 0 ):
                currentintensity =0
                r=0
                g=0
                b=0
            else:
                r = int((r * currentintensity ) / 255)
                g = int((g * currentintensity ) / 255)
                b = int((b * currentintensity ) / 255)
            leneffect -= 1
            serialout += 'R0112.'
            self.serial_print(serialout)
        #print(serialout)
        
