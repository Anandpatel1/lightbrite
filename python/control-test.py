#!/usr/bin/python
import serial
import os
import struct
import time

#ser = serial.Serial('/dev/ttyAMA0', baudrate=9600)
all1 = ['','','','','','']

def selfTest(address):
#	global ser
	print('Self Test')
	MSGID = 0X00
	ADDRESS = ord(bytes(bytearray([address])))
	COMACK = ord('C')
	COMDATA = 0X01

	HASH = MSGID ^ ADDRESS ^ COMACK ^ COMDATA

	print(MSGID)
	print(ADDRESS)
	print(HASH)
	print(COMACK)
	print(COMDATA)
	

	string = ''
	string += struct.pack('!B',MSGID)
	string += struct.pack('!B',ADDRESS)
	string += struct.pack('!B',HASH)
	string += struct.pack('!B',COMACK)
	string += struct.pack('!B',COMDATA)
	
	print(bytes([string]))
#	if(ser.isOpen()):
#		print("Closing the port");
#		ser.close()
	ser = serial.Serial('/dev/ttyAMA0', baudrate=9600, timeout=1)
#	ser.open()	
	ser.write(string)
	ser.close()
#	ser.flush()
#	ser.write(MSGID)
#	ser.write(ADDRESS)
#	ser.write(HASH)
#	ser.write(COMACK)
#	ser.write(COMDATA)
	
def setColor(address):
	print('Set Color')
	MSGID = 0X00
	ADDRESS = ord(bytes(bytearray([address])))
	COMACK = ord('C')
	COMDATA = 0X02

	COLOR_R = 255
	COLOR_G = 0
	COLOR_B = 0

	HASH = MSGID ^ ADDRESS ^ COMACK ^ COMDATA ^ COLOR_R ^ COLOR_G ^ COLOR_B ^ COLOR_R ^ COLOR_G ^ COLOR_B ^ COLOR_R ^ COLOR_G ^ COLOR_B ^ COLOR_R ^ COLOR_G ^ COLOR_B ^ COLOR_R ^ COLOR_G ^ COLOR_B ^ COLOR_R ^ COLOR_G ^ COLOR_B

	print(MSGID)
	print(ADDRESS)
	print(HASH)
	print(COMACK)
	print(COMDATA)

	
	string = ''
	string += struct.pack('!B',MSGID)
	string += struct.pack('!B',ADDRESS)
	string += struct.pack('!B',HASH)
	string += struct.pack('!B',COMACK)
	string += struct.pack('!B',COMDATA)
	
	string += struct.pack('!B',COLOR_R)
	string += struct.pack('!B',COLOR_G)
	string += struct.pack('!B',COLOR_B)
	string += struct.pack('!B',COLOR_R)
	string += struct.pack('!B',COLOR_G)
	string += struct.pack('!B',COLOR_B)
	string += struct.pack('!B',COLOR_R)
	string += struct.pack('!B',COLOR_G)
	string += struct.pack('!B',COLOR_B)
	string += struct.pack('!B',COLOR_R)
	string += struct.pack('!B',COLOR_G)
	string += struct.pack('!B',COLOR_B)
	string += struct.pack('!B',COLOR_R)
	string += struct.pack('!B',COLOR_G)
	string += struct.pack('!B',COLOR_B)
	string += struct.pack('!B',COLOR_R)
	string += struct.pack('!B',COLOR_G)
	string += struct.pack('!B',COLOR_B)
	

	
#	ser.write(MSGID)
#	ser.write(ADDRESS)
#	ser.write(HASH)
#	ser.write(COMACK)
#	ser.write(COMDATA)
#
#	ser.write(COLOR_R)
#	ser.write(COLOR_G)
#	ser.write(COLOR_B)
#	ser.write(COLOR_R)
#	ser.write(COLOR_G)
#	ser.write(COLOR_B)
#	ser.write(COLOR_R)
#	ser.write(COLOR_G)
#	ser.write(COLOR_B)
#	ser.write(COLOR_R)
#	ser.write(COLOR_G)
#	ser.write(COLOR_B)
#	ser.write(COLOR_R)
#	ser.write(COLOR_G)
#	ser.write(COLOR_B)
#	ser.write(COLOR_R)
#	ser.write(COLOR_G)
#	ser.write(COLOR_B)

	print(bytes([string]))
#	if(ser.isOpen()):
#		ser.close()
	ser = serial.Serial('/dev/ttyAMA0', baudrate=9600, timeout=1)
#	ser.open()	
	ser.write(string)
#	ser.flush()
	ser.close()
	
def getColor(address):
#	print('Not implemented yet')
	print('Get Color')
	MSGID = 0X00
	ADDRESS = ord(bytes(bytearray([address])))
	COMACK = ord('C')
	COMDATA = 0X03
	HASH = MSGID ^ ADDRESS ^ COMACK ^ COMDATA
	
#	ser.write(MSGID)
#	ser.write(ADDRESS)
#	ser.write(HASH)
#	ser.write(COMACK)
#	ser.write(COMDATA)

	string = ''
	string += struct.pack('!B',MSGID)
	string += struct.pack('!B',ADDRESS)
	string += struct.pack('!B',HASH)
	string += struct.pack('!B',COMACK)
	string += struct.pack('!B',COMDATA)
	
	print(bytes([string]))
#	if(ser.isOpen()):
#		ser.close()
	ser = serial.Serial('/dev/ttyAMA0', baudrate=9600, timeout=1)
#	ser.open()	
	ser.write(string)
	ser.close()
#	ser.flush()
	ser = serial.Serial('/dev/ttyAMA0', baudrate=9600, timeout=1)
	readValue = ser.read(size=23)
	print(readValue)
	ser.close()
	print('Read Value = ')
	for x in range(0, len(readValue)):
		print(ord(readValue[x]))
	

def normalMode(address):
	print('Normal Mode')
	MSGID = 0X00
	ADDRESS = ord(bytes(bytearray([address])))
	COMACK = ord('C')
	COMDATA = 0X04
	HASH = MSGID ^ ADDRESS ^ COMACK ^ COMDATA
	
#	ser.write(MSGID)
#	ser.write(ADDRESS)
#	ser.write(HASH)
#	ser.write(COMACK)
#	ser.write(COMDATA)

	string = ''
	string += struct.pack('!B',MSGID)
	string += struct.pack('!B',ADDRESS)
	string += struct.pack('!B',HASH)
	string += struct.pack('!B',COMACK)
	string += struct.pack('!B',COMDATA)
	
	print(bytes([string]))
#	if(ser.isOpen()):
#		ser.close()
	ser = serial.Serial('/dev/ttyAMA0', baudrate=9600, timeout=1)
#	ser.open()	
	ser.write(string)
	ser.close()
#	ser.flush()

#try:	
while(True):

	print('0. Exit')
	print('1. Self Test')
	print('2. Set Color')
	print('3. Get Color')
	print('4. Normal Mode')
	command = input('Please enter selection: ')

	if(command == 0):
		exit()
	address = input('Please enter Address: ')
	
	address = int(float(address))

	if(command == 0):
		exit()
	elif(command == 1):
		selfTest(address)
	elif(command == 2):
		setColor(address)
	elif(command == 3):
		getColor(address)
	elif(command == 4):
		normalMode(address)
	
	#time.sleep(2)
	ser = serial.Serial('/dev/ttyAMA0', baudrate=9600, timeout=1)
#		while(True):
	readValue = ser.read(size=5)
	print(readValue)
	ser.close()
#	if(len(readValue) < 5):
#		continue
	print('Read Value = ')

	for x in range(0, len(readValue)):	
		print(ord(readValue[x]))
#	print(ord(readValue[0]))
#	print(ord(readValue[1]))
#	print(ord(readValue[2]))
#	print(ord(readValue[3]))
#	print(ord(readValue[4]))
#except Exception, e:
#	print('Exception: ', str(e))
#	ser.close()
