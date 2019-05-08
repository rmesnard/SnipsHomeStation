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
    __inputcmds = False
    commandbuffer = ""
    
    buttonclick01_change = False
    buttonclick02_change = False
    buttonclick03_change = False
    buttonclick01_status = 0
    buttonclick02_status = 0
    buttonclick03_status = 0

    def init(self,serialout,serialspeed,pannelh,pannelw):
        self.__serial_out = serialout
        self.__serial_speed = serialspeed
        self.__pannel_height = pannelh
        self.__pannel_width = pannelw
        self.__arduino = serial.Serial(self.__serial_out,self.__serial_speed, timeout=1)
        time.sleep(2) 

    def serial_print(self,serialdata,repeat=1,fileoutput=False):

        commands = serialdata.split('.')
        for nb in range(repeat):
            for cmd in commands:
                #print(cmd)
                self.__arduino.write(cmd.encode())
                time.sleep(0.01) 

        if fileoutput:
            f = open("outfile.sef", "w")
            for  nb in range(repeat):
                f.write(serialdata)
            f.close()

    def scroll_right(self,steps,fps):
        serialout = 'R'
        serialout += format(steps, '02x')
        serialout += format(fps, '02x')
        serialout += '.'
        self.serial_print(serialout)

    def scroll_left(self,steps,fps):
        serialout = 'L'
        serialout += format(steps, '02x')
        serialout += format(fps, '02x')
        serialout += '.'
        self.serial_print(serialout)


    def clean(self):
        self.serial_print('C.',2)

    def check(self):
        if self.__arduino.in_waiting:
            __inputcmds = True
        else:
            __inputcmds = False
        return __inputcmds

    def serial_get(self):
        seq = []
        while self.__arduino.in_waiting:
            for c in self.__arduino.read():
                seq.append(chr(c)) #convert from ANSII
        self.commandbuffer = ''.join(str(v) for v in seq) #Make a string from array

    def process_cmds(self):
        commands = self.commandbuffer.split('.')
        for cmd in commands:
            print(cmd)
            if cmd == 'B0' :
                if self.buttonclick01_status == 1:
                    self.buttonclick01_change = True
                self.buttonclick01_status = 0
            if cmd == 'B1' :
                if self.buttonclick01_status == 0:
                    self.buttonclick01_change = True
                self.buttonclick01_status = 1
            if cmd == 'K0' :
                if self.buttonclick02_status == 1:
                    self.buttonclick02_change = True
                self.buttonclick02_status = 0
            if cmd == 'K1' :
                if self.buttonclick02_status == 0:
                    self.buttonclick02_change = True
                self.buttonclick02_status = 1
            if cmd == 'L0' :
                if self.buttonclick03_status == 1:
                    self.buttonclick03_change = True
                self.buttonclick03_status = 0
            if cmd == 'L1' :
                if self.buttonclick03_status == 0:
                    self.buttonclick03_change = True
                self.buttonclick03_status = 1 

    def update_status(self):
        self.serial_get()
        self.process_cmds()

    def get_status(self):
        serialout = 'G.'
        self.serial_print(serialout)
        time.sleep(0.5)
        self.check()
        self.serial_get()

    def load_image(self,filename):
        im = Image.open(filename)

        if (im.width != self.__pannel_width) and (im.height != self.__pannel_height):
            print("resize")
            im = im.resize((self.__pannel_width,self.__pannel_height))

        rgb_im = im.convert('RGB')
        return(rgb_im)

    def print_file(self,filename,repeat=1):
        f = open(filename, "r")
        contents = f.read()
        f.close()
        self.serial_print(contents,repeat)

    def generate_image_serial(self,rgbimage,fileoutput=False):
        serialout = ""
        idx=0
        for x in range(self.__pannel_width):
            for y in range(self.__pannel_height):
                r, g, b = rgbimage.getpixel((x, self.__pannel_height - y - 1))

                serialout += 'P'
                serialout += format(idx, '02x')
                serialout += format(r, '02x')
                serialout += format(g, '02x')
                serialout += format(b, '02x')
                serialout += '.'
                idx+=1
        serialout += 'S.'
        self.serial_print(serialout,fileoutput)

    def generate_scroll_left_anim_serial(self,filename,fileoutput=False):
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
        self.serial_print(serialout,fileoutput)

    def generate_anim_serial(self,filename,fileoutput=False):
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
        self.serial_print(serialout,fileoutput)


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
        
