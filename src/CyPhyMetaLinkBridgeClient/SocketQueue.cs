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
using System.Threading.Tasks;
using System.Threading;

namespace CyPhyMetaLinkBridgeClient
{
    public class SocketQueue
    {
        public static int port = 15150;
        private BlockingCollection<MetaLinkProtobuf.Edit> _messageQueue = new BlockingCollection<MetaLinkProtobuf.Edit>();

        private Socket _socket = null;
        private NetworkStream _networkStream = null;
        private BufferedStream _bufferedNetworkStream = null;

        private async Task<Socket> connect()
        {
            Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            byte[] byteAddress = new byte[] { 127, 0, 0, 1 };
            IPAddress ipAddress = new IPAddress(byteAddress);
            var args = new SocketAsyncEventArgs();
            args.RemoteEndPoint = new IPEndPoint(ipAddress, port);
            SemaphoreSlim signal = new SemaphoreSlim(0, 1);
            args.Completed += (sender, e) =>
            {
                signal.Release();
            };
            bool waiting = socket.ConnectAsync(args);
            if (waiting)
            {
                await signal.WaitAsync();
                if (args.SocketError != SocketError.Success)
                {
                    return null;
                }
            }
            if (socket.Connected == false)
            {
                return null;
            }

            return socket;
        }

        public async Task<bool> establishSocket()
        {
            _socket = await connect();
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

        private byte[] getCrc32(byte[] data, int dataLength)
        {
            CRC32 crc32 = new CRC32();
            crc32.Initialize();
            byte[] hash = crc32.ComputeHash(data, 0, dataLength);
            Array.Reverse(hash);
            return hash;
        }

        public bool IsConnected()
        {
            return _socket != null && _socket.Connected;
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
                        EditMessageSent?.Invoke(message);
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
