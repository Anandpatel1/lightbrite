
#!/usr/bin/python
import serial
import os
import struct
import time

address=255

MSGID = 0X00
ADDRESS = ord(bytes(bytearray([address])))
COMACK = ord('C')
COMDATA = 0X04

HASH = MSGID ^ ADDRESS ^ COMACK ^ COMDATA
	
string = ''
string += struct.pack('!B',MSGID)
string += struct.pack('!B',ADDRESS)
string += struct.pack('!B',HASH)
string += struct.pack('!B',COMACK)
string += struct.pack('!B',COMDATA)
#print(bytes([string]))
ser = serial.Serial('/dev/ttyAMA0', baudrate=9600, timeout=1)
ser.write(string)
ser.close()
	
