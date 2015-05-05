!/usr/bin/env python

import sys
import socket
import struct
import zlib

import CdbM_pb2

class GatewayTestClient:
    HEADER_MAGIC_NUMBER = 0xfeedbeef
    def __init__(self, host, port):
          self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
          self.sock.connect((host, int(port)))

    def sendMessageWrapper(self, msg):
          serializedMsg = msg.SerializeToString()
          messageHeader = struct.pack("<IIi", self.HEADER_MAGIC_NUMBER, len(serializedMsg), zlib.crc32(serializedMsg))
          self.sock.sendall(messageHeader) #little-endian byte order for now
          self.sock.sendall(struct.pack("<i", zlib.crc32(messageHeader)))
          print serializedMsg
          self.sock.sendall(serializedMsg)

    def receiveMessage(self):
        messageHeader = self.sock.recv(3*4)
        (magicNumber, messageSize, checksum) = struct.unpack("<IIi", messageHeader)
        (headerChecksum,) = struct.unpack("<i", self.sock.recv(4))

        if magicNumber != self.HEADER_MAGIC_NUMBER:
            raise IOError("Invalid magic number received from gateway: " + hex(magicNumber))

        if headerChecksum != zlib.crc32(messageHeader):
            raise IOError("Invalid header checksum received from gateway")

        protobufMsg = ""
        while len(protobufMsg) < messageSize:
            receivedData = self.sock.recv(messageSize - len(protobufMsg))
            protobufMsg += receivedData
        calculatedChecksum = zlib.crc32(protobufMsg)
        if calculatedChecksum != checksum:
            print "Checksum error!"
            return None
        msg = CdbMsg_pb2.MessageWrapper()
        msg.ParseFromString(protobufMsg)
        return msg

if __name__ == "__main__":
  print "Android Gateway SMS Tester"
  if len(sys.argv) != 4:
    print "Usage:", sys.argv[0], "host port message-type"
    print '''
 where message-type is one of:"
 authenticate : this is always sent regardless of what you request. 
            however as you must send some message this type is provided.
 initial : a message sending a single unit and its location"
'''
   exit(-1)

  print "Creating client"
  client = GatewayTestClient(sys.argv[1], sys.argv[2])
  print "Generating message"
  m = CdbMsg_pb2.MessageWrapper()
  m.type = CdbMsg_pb2.MessageWrapper.AUTHENTICATION_MESSAGE
  m.authentication_message.device_id = "device:test/device1"
  m.authentication_message.user_id = "user:test/user1"
  m.authentication_message.user_key = "dummy"
  print "Sending message"
  client.sendMessageWrapper(m)

  #wait for auth response, then send a data push message
  response = client.receiveMessage()
  if response.authentication_result.result != CdbMsg_pb2.AuthenticationResult.SUCCESS:
     print "Authentication failed..."

  if(sys.argv[3] == "send_message"):
     m = CdbMsg_pb2.MessageWrapper()
     m.type = CdbMsg_pb2.MessageWrapper.DATA_MESSAGE
     m.data_message.uri = "content:edu.vanderbilt.isis.ammo.SmsTest"
     m.data_message.mime_type = "ammo/edu.vu.isis.ammo.sms.message_foo"
     m.data_message.data = '''
{"from":"neemask","threadId":"1","to":"foo","body":"wuz up"}
'''
     print "Sending a test message"
     client.sendMessageWrapper(m)

  elif sys.argv[3] == "subscribe":
     #wait for auth response, then send a data push message
     response = client.receiveMessage()
     if response.authentication_result.result != CdbMsg_pb2.AuthenticationResult.SUCCESS:
        print "Authentication failed..."
     m = CdbMsg_pb2.MessageWrapper()
     m.type = CdbMsg_pb2.MessageWrapper.SUBSCRIBE_MESSAGE
     m.subscribe_message.mime_type = "ammo/edu.vu.isis.ammo.sms.message_neemask"
     print "Sending subscription request..."
     client.sendMessageWrapper(m)

  #while True:
  #  msg = client.receiveMessage()
  #  print msg

  print "Closing socket"

