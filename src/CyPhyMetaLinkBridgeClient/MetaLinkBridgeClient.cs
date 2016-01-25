using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;

using MetaLinkProtobuf = edu.vanderbilt.isis.meta;

namespace CyPhyMetaLinkBridgeClient
{
    public sealed class MetaLinkBridgeClient
    {
        private const string Host = "127.0.0.1";
        private const string Port = "15150";
        private SocketQueue _socketQueue = null;
        public SocketQueue SocketQueue { get { return _socketQueue; } }
        private Thread _socketSendQueueThread = null;
        private Thread _receiveThread = null;
        private object lockObject = new Object();

        public bool ConnectionEnabled
        {
            get;
            private set;
        }

        public bool EstablishConnection(Action<MetaLinkProtobuf.Edit> EditMessageReceived, Action<Exception> connectionClosed, Action<MetaLinkProtobuf.Edit> EditMessageSent = null)
        {
            lock (lockObject)
            {
                _socketQueue = new SocketQueue();
                if (!_socketQueue.establishSocket())
                {
                    _socketQueue = null;
                    return false;
                }
                if (EditMessageSent != null)
                {
                    _socketQueue.EditMessageSent += EditMessageSent;
                }
                _socketQueue.EditMessageReceived += EditMessageReceived;
                _socketQueue.ReceiveError += x =>
                {
                    CloseConnection();
                    connectionClosed(x);
                };

                _socketSendQueueThread = new Thread(new ThreadStart(_socketQueue.sendThread));
                _socketSendQueueThread.Name = "MetaLinkBridge Send";
                _socketSendQueueThread.Start();

                _receiveThread = new Thread(new ThreadStart(_socketQueue.receiveThread));
                _receiveThread.Name = "MetaLinkBridge Receive";
                _receiveThread.Start();

                ConnectionEnabled = true;
                closeRequested = false;

                return true;
            }
        }

        bool closeRequested = false;
        public bool CloseConnection()
        {
            lock (lockObject)
            {
                if (_socketQueue == null)
                    return true;
                if (!closeRequested)
                {
                    closeRequested = true;

                    if (_socketQueue == null)
                    {
                        return true;
                    }
                    _socketQueue.disconnectSocket();
                    if (Thread.CurrentThread != _receiveThread)
                    {
                        _receiveThread.Abort();
                    }
                    if (Thread.CurrentThread != _socketSendQueueThread)
                    {
                        _socketQueue.sendThreadCancellation.Cancel(true);
                        _socketSendQueueThread.Abort();
                    }
                    _socketQueue = null;
                    ConnectionEnabled = false;
                }
            }
            // If send or receive thread is calling CloseConnection, it will exit after this call
            if (Thread.CurrentThread != _receiveThread)
            {
                _receiveThread.Join();
            }
            if (Thread.CurrentThread != _socketSendQueueThread)
            {
                _socketSendQueueThread.Join();
            }
            return true;
        }

        public bool IsConnectedToBridge()
        {
            lock (lockObject)
            {
                return _socketQueue != null && _socketQueue.IsConnected();
            }
        }

        public bool SendToMetaLinkBridge(MetaLinkProtobuf.Edit message)
        {
            bool status = true;
            if (ConnectionEnabled)
            {
                _socketQueue.enQueue(message);
            }
            else
                status = false;

            return status; // FIXME: doesn't really mean anything, since the send thread could fail. Does the consumer want a callback?
        }
    }
}
