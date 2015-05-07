import serial
import struct

#open the port
s = serial.Serial(port='COM5', baudrate=57600, timeout=None, bytesize=8, parity='N', stopbits=1, xonxoff=0)
f=open('C:/test.txt','w')
format='HBHH'
while 1:
	#line=s.readline() #for reading lines
	byte=s.read(27) #reads 13 bytes (number of bytes per packet)
	if byte is not None:
		data=byte[3:24]
		lengthOfStructure=byte[3:8]
		#f.write(lengthOfStructure)
		#lengthOfStructure=byte[3]
		struct.unpack('HBHH',data)
		#struct.unpack('HHHHH',lengthOfStructure)
		print("LENGTH OF STRUCTURE IS ")
		print(lengthOfStructure)
		print("DATA IS ")
		print(data)
		print("BYTE IS ")
		print(byte)
s.close()