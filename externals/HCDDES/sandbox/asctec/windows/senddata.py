import math
import serial
import struct
import time

def next_msg():
	#y=struct.pack('<ccccchhhhhH','>','*','>','d','i',0,0,0,4095,8,15025) #thrust=100%
	y=struct.pack('<ccccchhhhhh','>','*','>','d','i',0,0,0,1024,8,11954) #thrust=25%
	ser.write(y)

#main
ser = serial.Serial('COM5',57600)
result = ser.isOpen()
ser.setTimeout(1)
print "Serial port opened:", result

found = False;
while(found == False):
	x=ser.read(1)
	if x == '>':
		found = True;
		print "now entering send mode"
		break;
while(True):
	next_msg()
	

ser.close()
print "closing"
