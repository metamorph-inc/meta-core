import math
import serial
import struct
import time
import ctypes as CT
import parallel

def gen_attitude_cmd(pitch,roll,yaw,thrust):
	ctrl = 0x08;
	csum = CT.c_short(pitch+roll+yaw+thrust+ctrl+0xAAAA).value;
	print csum;
	cmd = struct.pack('<5c6h','>','*','>','d','i',pitch,roll,yaw,thrust,ctrl,csum);
	ser.write(cmd);

#main
ser = serial.Serial('COM5',57600)
result = ser.isOpen()
ser.setTimeout(1)
print "Serial port opened:", result
f=open('C:/test.txt','w')
p = parallel.Parallel()
p.setData(0x55)
ser.write(struct.pack('<cccch','>','*','>','m',1))

	

ser.close()
print "closing"