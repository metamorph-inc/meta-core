using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using MetaLinkProtobuf = edu.vanderbilt.isis.meta;

namespace CyPhyMetaLinkBridgeClient
{
    public sealed class MetaLinkBridgeClient
    {
        private SocketQueue _socketQueue = null;
        public SocketQueue SocketQueue { get { return _socketQueue; } }
        private Thread _socketSendQueueThread = null;
        private Thread _socketReceiveThread = null;
        private object lockObject = new Object();

        public MetaLinkBridgeClient()
        {
        }

        Task<bool> _establishSocket;
        public async Task<bool> EstablishConnection(Action<MetaLinkProtobuf.Edit> EditMessageReceived, Action<Exception> connectionClosed, Action<MetaLinkProtobuf.Edit> EditMessageSent = null)
        {
            if (_establishSocket != null)
            {
                // our only client passes the same callbacks, so this isn't necessary:
                // assert connectionClosed == this.connectionClosed
                return await _establishSocket;
            }
            var socketQueue = new SocketQueue();
            lock (lockObject)
            {
                socketQueue = new SocketQueue();
                _establishSocket = socketQueue.establishSocket();
            }
            bool connected = await _establishSocket;
            lock (lockObject)
            {
                if (!connected)
                {
                    _establishSocket = null;
                    return false;
                }
                if (_establishSocket == null)
                {
                    // Disconnect() was called
                    return false;
                }
                _socketQueue = socketQueue;
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

                _socketReceiveThread = new Thread(new ThreadStart(_socketQueue.receiveThread));
                _socketReceiveThread.Name = "MetaLinkBridge Receive";
                _socketReceiveThread.Start();

                return true;
            }
        }

        public bool CloseConnection()
        {
            lock (lockObject)
            {
                if (_socketQueue == null)
                {
                    return true;
                }
                _establishSocket = null;
                var socketQueue = _socketQueue;
                _socketQueue = null;
                socketQueue.disconnectSocket();
                if (Thread.CurrentThread != _socketReceiveThread)
                {
                    _socketReceiveThread.Abort();
                }
                socketQueue.sendThreadCancellation.Cancel(true);
            }
            // If send or receive thread is calling CloseConnection, it will exit after this call
            if (Thread.CurrentThread != _socketReceiveThread)
            {
                _socketReceiveThread.Join();
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
                return _establishSocket != null && _socketQueue != null && _socketQueue.IsConnected();
            }
        }

        public bool SendToMetaLinkBridge(MetaLinkProtobuf.Edit message)
        {
            bool status = true;
            lock (lockObject)
            {
                if (_socketQueue != null)
                {
                    _socketQueue.enQueue(message);
                }
                else
                {
                    status = false;
                }
            }

            return status; // FIXME: doesn't really mean anything, since the send thread could fail. Does the consumer want a callback?
        }
    }
}
