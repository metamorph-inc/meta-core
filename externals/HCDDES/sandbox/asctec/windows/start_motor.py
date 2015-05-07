import serial
import ctypes as CT
import struct

ser = serial.Serial('COM5',57600)
ser.write(struct.pack('<cccch','>','*','>','m',1))
ctrl = 0x08;
csum = CT.c_short(ctrl+0xAAAA).value;
print csum;
cmd = struct.pack('<5c6h','>','*','>','d','i',0,0,0,0,ctrl,csum);
ser.write(cmd);