import math
import serial
import struct
import time
import threading

def get_msg():
	while(True):
		found = False;
		while(found == False):
			x=ser.read(1)
			if x == '>':
				found = True;
				break;
			return 0
						
		x=ser.read(2)
		x=ser.read(2) #read length
		length=struct.unpack('<H',x)
		x=ser.read(1) #read descriptor
		descriptor=struct.unpack('<B',x)
		valid = True;
		if descriptor == (1,):
			format = 'ihhhhhhhhhHIH' #IMURAWDATA
		elif descriptor == (2,):
			format = 'hhhhBBBBhhH' #LLSTATUS
		elif descriptor == (3,):
			format = 'iiiiiihhhhhhiiiiiiiiiiiiiiH' #IMUCALCDATA
		elif descriptor == (11,):
			format = 'iiiiH' #CTRLOUT
		elif descriptor == (15,):
			format = 'HHBH' #RCDATA
		elif descriptor == (20,):
			format = 'BBHBBHHhiiiiH' #WAYPOINT
		elif descriptor == (23,):
			format = 'iiiiiiIIIIiH' #GPSDATA
		elif descriptor == (29,):
			format = 'iiiiiiIIIIiiiiiH' #GPSDATAADVANCED
		else:
			valid = False;
		if valid == True:
			msg_size=struct.calcsize(format)
			x=ser.read(msg_size)
			if len(x) == msg_size:
				y=struct.unpack(format,x)
				print y
			else:
				print "Timeout"
def send_msg():
	while(True):
		y=struct.pack('<ccccH','>','*','>','p',5)
		#ser.write(y)

#main
if __name__ == "__main__":
	ser = serial.Serial('COM5',57600)
	result = ser.isOpen()
	ser.setTimeout(1)
	print "Serial port opened:", result
	f=open('C:/test.txt','w')

	getThread = threading.Thread(target=get_msg)
	sendThread = threading.Thread(target=send_msg)
	getThread.start()
	sendThread.start()

	ser.close()
	print "closing"

