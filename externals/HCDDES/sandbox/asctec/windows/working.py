import serial
import struct
import math
import time

format='=ppHBHHppp'
msg_size=struct.calcsize(format)

def next_msg():
	found = False;
	while(found == False):
		x=ser.read(1)
		#if len(x) > 0:
			#print("read byte", x)
			#return 0
		if len(x) == 0:
			return 0
		if x == '>':
			found = True;
							
	print "DONE"
	x=ser.read(msg_size)
	if len(x) == msg_size:
		y=struct.unpack(format,x)
		print y
	else:
		print "Timeout"

#main
ser = serial.Serial('COM5',57600)
result = ser.isOpen()
ser.setTimeout(1)
print "Serial port opened:", result
print "Size of message in bytes = ", msg_size
f=open('C:/test.txt','w')

while(True):
    next_msg()

ser.close()
print "closing"

