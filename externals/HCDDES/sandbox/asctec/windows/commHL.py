import math
import serial
import struct
import time
import ctypes as CT
import parallel
import numpy as NU

def gen_thrust_cmd(m1,m2,m3,m4):
	ctrl = 0x08;
	csum = CT.c_short(m1+m2+m3+m4+ctrl+0xAAAA).value;
	cmd = struct.pack('<5c6h','>','*','>','d','i',m1,m2,m3,m4,ctrl,csum);

def gen_attitude_cmd(pitch,roll,yaw,thrust):
	ctrl = 0x08;
	csum = CT.c_short(pitch+roll+yaw+thrust+ctrl+0xAAAA).value;
	print csum;
	cmd = struct.pack('<5c6h','>','*','>','d','i',pitch,roll,yaw,thrust,ctrl,csum);
	ser.write(cmd);

#format='<HBhhhhBBBBhhH'
#format='B'
#msg_size=struct.calcsize(format)
msg_size=1

def read_msg():
	st = 0;
	rt = 0;
	while(True):
		#print "st = "+str(st);
		x=ser.read(1)
		print "x= "+str(x);
		if (st == 0) and (x == '>') :
			st = 1;
		elif (st == 1) and (x == '*'):
			st = 2;
		elif (st == 1) and (x != '*'):
			rt = rt + 1;
			print "(st,rt,x) = "+str((st,rt,x));
			st = 0;
		elif (st == 2) and (x == '>'):
			break;
		else:
			rt = rt + 1;
			print "(st,rt,x) = "+str((st,rt,x));
			st = 0;
		if rt > 20:
			return -1;
						
	x=ser.read(2) #read length
	length=struct.unpack('<H',x)
	#print "length = " + str(length);
	x=ser.read(1) #read descriptor
	descriptor=struct.unpack('<B',x)
	#print "descriptor = " + str(descriptor);
	if descriptor == (1,):
		format = 'ihhhhhhhhhHI' #IMURAWDATA
	elif descriptor == (2,):
		format = 'hhhhBBBBhh' #LLSTATUS
	elif descriptor == (3,):
		format = 'iiiiiihhhhhhiiiiiiiiiiiiii' #IMUCALCDATA
	elif descriptor == (4,):
		format = 'hhhhiihhhh' #HLSTATUS
	elif descriptor == (6,):
		format = 'h' #THRUST
	elif descriptor == (7,):
		format = 'hhh' #ATTITUDE
	elif descriptor == (11,):
		format = 'iiii' #CTRLOUT
	elif descriptor == (15,):
		format = 'HHB' #RCDATA
	elif descriptor == (20,):
		format = 'BBHBBHHhiiii' #WAYPOINT
	elif descriptor == (23,):
		format = 'iiiiiiIIIIi' #GPSDATA
	elif descriptor == (29,):
		format = 'iiiiiiIIIIiiiii' #GPSDATAADVANCED
	else:
		print "Invalid descriptor= "+str(descriptor)
		return -1;
	msg_size=struct.calcsize(format)
	x=ser.read(msg_size)
	z=ser.read(5);
	if len(x) == msg_size:
		y=struct.unpack(format,x)
		print y;
		return 0;
	print "Invalid message size"
	return -1;
		
#main
#ser = serial.Serial('COM5',57600)
ser = serial.Serial('COM3',57600)
result = ser.isOpen()
#ser.setTimeout(1)
print "Serial port opened:", result
f=open('C:/test.txt','w')
#gen_attitude_cmd(0,0,0,0)
#ser.write(struct.pack('<cccch','>','*','>','m',1))

#gen_attitude_cmd(0,0,0,0)
#ser.write(struct.pack('<cccch','>','*','>','m',1))
#ser.write(struct.pack('<ccccH','>','*','>','p',1)) #status request
#ser.write(struct.pack('<ccccc','>','*','>','m','0'))
motor = '1';
thrust = 0;
dTv = NU.zeros((1000));
i = 0;
try:	
	while(True):
		dt = time.time();
		#ser.write(struct.pack('<ccccH','>','*','>','m',motor))
		#ser.write(struct.pack('<ccccc','>','*','>','m',motor))
		ser.write(struct.pack('<cccch','>','*','>','t',thrust))
		if(motor == '1'):
			motor = '0';
		else:
			motor = '1';
		#ser.write(struct.pack('<ccccH','>','*','>','p',1)) #status request
		if read_msg():
			break;
		dT = time.time() - dt;
		dTv[i] = dT;
		i = i + 1;
		print "dT = " + str(dT);
		thrust = thrust + 1;
		if thrust == 99:
			thrust = 0;
		#if i == 1000:
			#break;
except:
	#gen_attitude_cmd(0,0,0,0)
	ser.write(struct.pack('<cccch','>','*','>','m',0))	
ser.write(struct.pack('<cccch','>','*','>','m',0))	
i = 0;
sum = 0;
while(True):
	#print dTv[i];
	sum = dTv[i] + sum;
	i = i + 1;
	if i == 1000:
		break;
avg = sum / 1000;
diff_sum = 0;
i = 0;
while(True):
	diff_sum = diff_sum + ((dTv[i] - avg)*(dTv[i]-avg))
	i=i+1;
	if i == 1000:
		break;
variance = diff_sum / 999;
stddev = math.sqrt(variance);
print "average is "+str(avg);
print "standard deviation is "+str(stddev);
ser.write(struct.pack('<cccch','>','*','>','m',0))	
ser.close()
print "closing"
