using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Diagnostics;
using System.Collections.Concurrent;

using MetaLinkProtobuf = edu.vanderbilt.isis.meta;

namespace CyPhyMetaLinkBridgeClient
{
    public class SocketQueue
    {
        public static int port = 15150;
        private BlockingCollection<MetaLinkProtobuf.Edit> _messageQueue = new BlockingCollection<MetaLinkProtobuf.Edit>();

        private Socket _socket = null;
        private NetworkStream _networkStream = null;
        private BufferedStream _bufferedNetworkStream = null;


        public SocketQueue(/*MgaProject mgaProject*/)
        {
        }

        private Socket tryGetSocket()
        {
            Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            //            socket.Blocking = false;

            byte[] byteAddress = new byte[] { 127, 0, 0, 1 };
            IPAddress ipAddress = new IPAddress(byteAddress);
            try
            {
                socket.Connect(ipAddress, port);
            }
            catch (SocketException socketException)
            {
                if (socketException.ErrorCode == 10035)
                { // WSAEWOULDBLOCK
                    bool socketGood = socket.Poll(10000000, SelectMode.SelectWrite);
                    if (!socketGood) return null;
                }
            }

            return socket;
        }

        private Socket getSocket()
        {
            Socket socket = null;

            try
            {
                socket = tryGetSocket();
                if (!socket.Connected)
                    socket = null;
            }
            catch (Exception)
            {
                socket = null;
            }


            return socket;
        }

        public bool establishSocket()
        {

            _socket = getSocket();
            if (_socket == null)
            {
                return false;
            }

            _networkStream = new NetworkStream(_socket);
            _bufferedNetworkStream = new BufferedStream(_networkStream);
            return true;
        }

        public bool disconnectSocket()
        {
            _socket.Shutdown(SocketShutdown.Both);
            _socket.Close();

            _socket = null;
            return true;
        }

        private void encodeInteger(Stream stream, int number)
        {
            byte[] byteArray = BitConverter.GetBytes(number);
            if (BitConverter.IsLittleEndian) Array.Reverse(byteArray);
            stream.Write(byteArray, 0, byteArray.Length);
        }

        private void encodeInteger(Stream stream, uint number)
        {
            encodeInteger(stream, (int)number);
        }

        private byte[] getCrc32(byte[] data, long dataLength)
        {
            CRC32 crc32 = new CRC32();
            crc32.Initialize();
            byte[] hash = crc32.ComputeHash(data, 0, (int)dataLength);
            Array.Reverse(hash);
            return hash;
        }

        public bool IsConnected()
        {
            bool status = false;
            if (_socket != null)
            {
                try
                {
                    status = !(_socket.Poll(1, SelectMode.SelectRead) && _socket.Available == 0);
                }
                catch (SocketException)
                {
                    status = false;
                }
            }

            return status;
        }

        private void sendMessage(MetaLinkProtobuf.Edit message)
        {
            MemoryStream messageMemoryStream = new MemoryStream();
            ProtoBuf.Serializer.Serialize(messageMemoryStream, message);
            byte[] payload = messageMemoryStream.GetBuffer();
            int payloadLength = (int)messageMemoryStream.Length;

            MemoryStream frameMemoryStream = new MemoryStream();

            encodeInteger(frameMemoryStream, 0xDEADBEEF);
            encodeInteger(frameMemoryStream, (int)messageMemoryStream.Length);
            encodeInteger(frameMemoryStream, 0);

            byte[] payloadHash = getCrc32(payload, payloadLength);
            frameMemoryStream.Write(payloadHash, 0, 4);

            byte[] frame = frameMemoryStream.GetBuffer();
            int frameLength = (int)frameMemoryStream.Length;
            byte[] frameHash = getCrc32(frame, frameLength);
            frameMemoryStream.Write(frameHash, 0, 4);

            frameMemoryStream.Write(payload, 0, payloadLength);
            frameMemoryStream.Write(payloadHash, 0, 4);

            _networkStream.Write(frameMemoryStream.GetBuffer(), 0, (int)frameMemoryStream.Length);
            _networkStream.Flush();
        }

        public void enQueue(MetaLinkProtobuf.Edit message)
        {
            _messageQueue.Add(message);
        }

        public System.Threading.CancellationTokenSource sendThreadCancellation = new System.Threading.CancellationTokenSource();

        public void sendThread()
        {
            try
            {
                while (true)
                {
                    MetaLinkProtobuf.Edit message;
                    if (_messageQueue.TryTake(out message, 1000, sendThreadCancellation.Token))
                    {
                        sendMessage(message);
                        if (EditMessageSent != null)
                        {
                            EditMessageSent(message);
                        }
                    }
                }
            }
            catch (SocketException e)
            {
                ReceiveError(e);
            }
            catch (IOException e)
            {
                ReceiveError(e);
            }
            catch (ObjectDisposedException e)
            {
                ReceiveError(e);
            }
            catch (ProtoBuf.ProtoException e)
            {
                ReceiveError(e);
            }
            catch (System.OperationCanceledException)
            {
            }
        }

        public event Action<MetaLinkProtobuf.Edit> EditMessageReceived;
        public event Action<MetaLinkProtobuf.Edit> EditMessageSent;
        public event Action<Exception> ReceiveError;

        public void receiveThread()
        {
            FrameReader frameReader = new FrameReader(_socket);
            try
            {
                while (true)
                {
                    MetaLinkProtobuf.Edit message = frameReader.getMessage();
                    if (EditMessageReceived != null)
                    {
                        EditMessageReceived(message);
                    }
                }
            }
            catch (SocketException e)
            {
                ReceiveError(e);
            }
            catch (IOException e)
            {
                ReceiveError(e);
            }
            catch (ObjectDisposedException e)
            {
                ReceiveError(e);
            }
            catch (ProtoBuf.ProtoException e)
            {
                ReceiveError(e);
            }
        }

    }
}
