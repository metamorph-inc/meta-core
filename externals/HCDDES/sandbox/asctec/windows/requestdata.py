import serial
import struct

ser = serial.Serial('COM5',57600)
result = ser.isOpen()
ser.setTimeout(1)
print "Serial port opened:",result

x1='>'
y=struct.pack('>ccccH','>','*','>','p',1)
print y
ser.write(y)
while 1:
	x=1