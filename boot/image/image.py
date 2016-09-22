#!/usr/bin/python

from PIL import Image
import numpy as np
import serial
import os
import struct
import time

im = Image.open("/boot/image/image.png")
p = np.array(im)

print(len(p))
print(len(p[0]))
print(len(p[0][0]))

dividerX = len(p)/22
dividerY = len(p[0])/42

print("dividerX = ", dividerX)
print("dividerY = ", dividerY)

print(type(len(p[0][0])))

#int[][][] showArray = new int[48][22][3]

showArray = [[[0 for k in xrange(3)] for j in xrange(48)] for i in xrange(22)]

print(len(showArray))
print(len(showArray[0]))
print(len(showArray[0][0]))

xIndex = 0
yIndex = 0
for x in range(0, len(p)-dividerX, dividerX):
	#yArray = []
	yIndex = 0
	for y in range(0, len(p[0]) - dividerY, dividerY):
		print('x = ', x, 'y = ', y)
		print('xIndex = ', xIndex, 'yIndex = ', yIndex)
		#yArray.append(p[x][y])
		#showArray[xIndex] = []
		showArray[xIndex][yIndex] = p[x][y]
		yIndex+=1
	xIndex+=1
	#showArray.append(yArray)

'''
for x in range(0, len(showArray)):
	arrs = ''
	for y in range(0, len(showArray[x])):
		arrs = arrs + str(showArray[x][y][0]) + '\t'
		#print(showArray[x][0])
	print(arrs)
	print("\n")

showArray_c = np.asarray(showArray)
for x in range(0, len(showArray_c)):
	arrs = ''
	for y in range(0, len(showArray_c[x])):
		arrs = arrs + str(showArray_c[x][y][0]) + '\t'
		#print(showArray[x][0])
	print(arrs)
	print("\n")


img = Image.fromarray(showArray_c, 'RGB')
img.save('my.bmp')
img.show()
'''
print(len(showArray))
print(len(showArray[0]))
print(len(showArray[0][0]))

fR=open('red.txt', 'w')
fG=open('green.txt', 'w')
fB=open('blue.txt', 'w')

ser = serial.Serial('/dev/ttyAMA0', baudrate=9600, timeout=1)


numIndex = 1
for x in range(0, len(showArray)):
	testprintR = ''
	testprintG = ''
	testprintB = ''
	for y in range(0, len(showArray[x])-6, 6):
	#	print('Set Color')
#		print('x = ', x, 'y = ', y)
		MSGID = 0X00
		ADDRESS = ord(bytes(bytearray(chr(numIndex))))
		numIndex+=1
		COMACK = ord('C')
		COMDATA = 0X02

		COLOR_R1 = showArray[x][y+5][0]
		COLOR_G1 = showArray[x][y+5][1]
		COLOR_B1 = showArray[x][y+5][2]
		COLOR_R2 = showArray[x][y+4][0]
		COLOR_G2 = showArray[x][y+4][1]
		COLOR_B2 = showArray[x][y+4][2]
		COLOR_R3 = showArray[x][y+3][0]
		COLOR_G3 = showArray[x][y+3][1]
		COLOR_B3 = showArray[x][y+3][2]
		COLOR_R4 = showArray[x][y+2][0]
		COLOR_G4 = showArray[x][y+2][1]
		COLOR_B4 = showArray[x][y+2][2]
		COLOR_R5 = showArray[x][y+1][0]
		COLOR_G5 = showArray[x][y+1][1]
		COLOR_B5 = showArray[x][y+1][2]
		COLOR_R6 = showArray[x][y+0][0]
		COLOR_G6 = showArray[x][y+0][1]
		COLOR_B6 = showArray[x][y+0][2]

		testprintR = testprintR + '|' + str(ADDRESS) + '\t|\t' + str(COLOR_R1) + '\t' + str(COLOR_R2) + '\t' + str(COLOR_R3) + '\t' + str(COLOR_R4) + '\t' + str(COLOR_R5) + '\t' + str(COLOR_R6) + '\t'
		testprintG = testprintG + '|' + str(ADDRESS) + '\t|\t' + str(COLOR_G1) + '\t' + str(COLOR_G2) + '\t' + str(COLOR_G3) + '\t' + str(COLOR_G4) + '\t' + str(COLOR_G5) + '\t' + str(COLOR_G6) + '\t'
		testprintB = testprintB + '|' + str(ADDRESS) + '\t|\t' + str(COLOR_B1) + '\t' + str(COLOR_B2) + '\t' + str(COLOR_B3) + '\t' + str(COLOR_B4) + '\t' + str(COLOR_B5) + '\t' + str(COLOR_B6) + '\t'

		HASH = MSGID ^ ADDRESS ^ COMACK ^ COMDATA ^ COLOR_R1 ^ COLOR_G1 ^ COLOR_B1 ^ COLOR_R2 ^ COLOR_G2 ^ COLOR_B2 ^ COLOR_R3 ^ COLOR_G3 ^ COLOR_B3 ^ COLOR_R4 ^ COLOR_G4 ^ COLOR_B4 ^ COLOR_R5 ^ COLOR_G5 ^ COLOR_B5 ^ COLOR_R6 ^ COLOR_G6 ^ COLOR_B6

#		print(MSGID)
#		print(ADDRESS)
#		print(HASH)
#		print(COMACK)
#		print(COMDATA)

	
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
	
#		print(bytes([string]))
		ser.write(string)
		time.sleep(0.1)	
	#print(testprintR)
	#print(testprintG)
	#print(testprintB)
	fR.write(testprintR + '\n')
	fG.write(testprintG + '\n')
	fB.write(testprintB + '\n')
ser.close()
