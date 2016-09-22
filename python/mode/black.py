#!/usr/bin/python
import serial
import os
import struct
import time

address=255

MSGID = 0X00
ADDRESS = ord(bytes(bytearray([address])))
COMACK = ord('C')
COMDATA = 0X02

COLOR_R = 0
COLOR_G = 0
COLOR_B = 0

COLOR_R1 = COLOR_R
COLOR_G1 = COLOR_G
COLOR_B1 = COLOR_B
COLOR_R2 = COLOR_R
COLOR_G2 = COLOR_G
COLOR_B2 = COLOR_B
COLOR_R3 = COLOR_R
COLOR_G3 = COLOR_G
COLOR_B3 = COLOR_B
COLOR_R4 = COLOR_R
COLOR_G4 = COLOR_G
COLOR_B4 = COLOR_B
COLOR_R5 = COLOR_R
COLOR_G5 = COLOR_G
COLOR_B5 = COLOR_B
COLOR_R6 = COLOR_R
COLOR_G6 = COLOR_G
COLOR_B6 = COLOR_B

HASH = MSGID ^ ADDRESS ^ COMACK ^ COMDATA ^ COLOR_R1 ^ COLOR_G1 ^ COLOR_B1 ^ COLOR_R2 ^ COLOR_G2 ^ COLOR_B2 ^ COLOR_R3 ^ COLOR_G3 ^ COLOR_B3 ^ COLOR_R4 ^ COLOR_G4 ^ COLOR_B4 ^ COLOR_R5 ^ COLOR_G5 ^ COLOR_B5 ^ COLOR_R6 ^ COLOR_G6 ^ COLOR_B6
	
string = ''
string += struct.pack('!B',MSGID)
string += struct.pack('!B',ADDRESS)
string += struct.pack('!B',HASH)
string += struct.pack('!B',COMACK)
string += struct.pack('!B',COMDATA)

string += struct.pack('!B',COLOR_R1)
string += struct.pack('!B',COLOR_G1)
string += struct.pack('!B',COLOR_B1)
string += struct.pack('!B',COLOR_R2)
string += struct.pack('!B',COLOR_G2)
string += struct.pack('!B',COLOR_B2)
string += struct.pack('!B',COLOR_R3)
string += struct.pack('!B',COLOR_G3)
string += struct.pack('!B',COLOR_B3)
string += struct.pack('!B',COLOR_R4)
string += struct.pack('!B',COLOR_G4)
string += struct.pack('!B',COLOR_B4)
string += struct.pack('!B',COLOR_R5)
string += struct.pack('!B',COLOR_G5)
string += struct.pack('!B',COLOR_B5)
string += struct.pack('!B',COLOR_R6)
string += struct.pack('!B',COLOR_G6)
string += struct.pack('!B',COLOR_B6)
	
#print(bytes([string]))
ser = serial.Serial('/dev/ttyAMA0', baudrate=9600, timeout=1)
ser.write(string)
ser.close()
	
