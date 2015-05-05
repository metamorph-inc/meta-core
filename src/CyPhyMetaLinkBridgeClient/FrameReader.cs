using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using System.Net.Sockets;

using ProtoBuf;
using MetaLinkProtobuf = edu.vanderbilt.isis.meta;

namespace CyPhyMetaLinkBridgeClient
{
    class FrameReader {

        private Socket _socket;
        private NetworkStream _networkStream;
        private BufferedStream _bufferedStream;

        private MemoryStream _memoryStream = new MemoryStream();

        private long _mark = 0;
        
        public FrameReader(Socket socket) {
            _socket = socket;
            _networkStream = new NetworkStream(_socket);
            _bufferedStream = new BufferedStream(_networkStream);
        }

        public void enQueue(long count) {
            if (count <= 0)
                return;
            long memoryStreamReadPointer = _memoryStream.Position;
            _memoryStream.Seek(0, SeekOrigin.End);
            byte[] buffer = new byte[1024 * 4];
            while (count > 0)
            {
                int bytesRead = _bufferedStream.Read(buffer, 0, Math.Min((int)Math.Min(Int32.MaxValue, count), buffer.Length));
                _memoryStream.Write(buffer, 0, bytesRead);
                count -= bytesRead;
            }
            _memoryStream.Seek(memoryStreamReadPointer, SeekOrigin.Begin);
        }

        public void Read(byte[] buffer, int offset, int count) {
            enQueue(count - (_memoryStream.Length - _memoryStream.Position));
            _memoryStream.Read(buffer, offset, count);
        }

        public int ReadByte() {
            enQueue(1L - (_memoryStream.Length - _memoryStream.Position));

            return _memoryStream.ReadByte();
        }

        public void getBytes(int memoryStreamOffset, byte[] buffer) {
            Array.Copy(_memoryStream.GetBuffer(), _mark + memoryStreamOffset, buffer, 0, buffer.Length);
        }

        public long Length {
            get { return _memoryStream.Length - _mark; }
        }

        private void rewind() {
            _memoryStream.Seek(_mark, SeekOrigin.Begin);
        }

        private void drop() {
            if (_mark < 1024) return;
            if (_memoryStream.Length < _mark) return;

            long newLength = _memoryStream.Length - _mark;
            long newPosition = _memoryStream.Position - _mark;

            byte[] swapper = new byte[ newLength ];
            Array.Copy(_memoryStream.GetBuffer(), _mark, swapper, 0, newLength);

            _memoryStream = new MemoryStream();
            _memoryStream.Write(swapper, 0, (int)newLength);
            _memoryStream.Seek(newPosition, SeekOrigin.Begin);

            _mark = 0;
        }

        private void dropRelative( int offset ) {
            _mark += offset;
            drop();
        }

        private void dropAbsolute( long position ) {
            _mark = position;
            drop();
        }

        private void dropMagic() {
            dropRelative(4);
            rewind();
        }

        private void dropToCurrent() {
            dropAbsolute(_memoryStream.Position);
        }

        private void getMagic() {
            int state = 0;
            while (true) {
                int byte_var = ReadByte();
                switch (state) {
                    case 0: {
                        if (byte_var == 0xDE) state = 1;
                        else dropRelative(1);
                        break;
                    }
                    case 1: {
                        if (byte_var == 0xAD) {
                            state = 2;
                        } else {
                            state = 0;
                            dropRelative(2);
                        }
                        break;
                    }
                    case 2: {
                        if (byte_var == 0xBE) {
                            state = 3;
                        } else {
                            state = 0;
                            dropRelative(3);
                        }
                        break;
                    }
                    case 3: {
                        if (byte_var == 0xEF) return;
                        state = 0;
                        dropRelative(4);
                        break;
                    }
                }
            }
        }

        private byte[] getCrc32(byte[] data, long dataLength) {
            CRC32 crc32 = new CRC32();
            crc32.Initialize();
            byte[] hash = crc32.ComputeHash(data, 0, (int)dataLength);
            Array.Reverse(hash);
            return hash;
        }

        public MetaLinkProtobuf.Edit getMessage()
        {

            while (true) {

                getMagic();

                byte[] sizeBuf = new byte[4];
                byte[] reserved = new byte[2];
                byte[] payloadHashBuf = new byte[4];
                byte[] headerHashBuf = new byte[4];

                enQueue(4 + 1 + 1 + 2 + 4 + 4);
                Read(sizeBuf, 0, 4);

                ReadByte();
                ReadByte();
                Read(reserved, 0, 2);

                Read(payloadHashBuf, 0, 4);
                Read(headerHashBuf, 0, 4);

                if (BitConverter.IsLittleEndian) {
                    Array.Reverse(sizeBuf);
                    Array.Reverse(payloadHashBuf);
                    Array.Reverse(headerHashBuf);
                }
                int size = BitConverter.ToInt32(sizeBuf, 0);
                int payloadHash = BitConverter.ToInt32(payloadHashBuf, 0);
                int headerHash = BitConverter.ToInt32(headerHashBuf, 0);

                byte[] payload = new byte[size];
                Read(payload, 0, size);

                byte[] finalPayloadHashBuf = new byte[4];
                Read(finalPayloadHashBuf, 0, 4);
                if (BitConverter.IsLittleEndian) {
                    Array.Reverse(finalPayloadHashBuf);
                }
                int finalPayloadHash = BitConverter.ToInt32(finalPayloadHashBuf, 0);

                byte[] frame = new byte[16];
                getBytes(0, frame);

                byte[] expectedPayloadHashBuf = getCrc32(payload, size);
                byte[] expectedFrameHashBuf = getCrc32(frame, frame.Length);
                if (BitConverter.IsLittleEndian) {
                    Array.Reverse(expectedPayloadHashBuf);
                    Array.Reverse(expectedFrameHashBuf);
                }
                int expectedPayloadHash = BitConverter.ToInt32(expectedPayloadHashBuf, 0);
                int expectedFrameHash = BitConverter.ToInt32(expectedFrameHashBuf, 0);

                if (expectedPayloadHash != payloadHash) {
                    dropMagic();
                    continue;
                }

                if (expectedFrameHash != headerHash) {
                    dropMagic();
                    continue;
                }

                if (finalPayloadHash != payloadHash) {
                    dropMagic();
                    continue;
                }
                dropAbsolute((int)_memoryStream.Position);


                MemoryStream payloadMemoryStream = new MemoryStream(payload);

                MetaLinkProtobuf.Edit message = Serializer.Deserialize<MetaLinkProtobuf.Edit>(payloadMemoryStream);
                return message;
            }

        }
    }
}
